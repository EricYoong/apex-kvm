#include "aim.hpp"
#include "config/options.hpp"
#include "utils/utils.hpp"
#include "math/solver.hpp"
#include "entities.hpp"
#include "offsets.hpp"

#include <spdlog/spdlog.h>
#include <thread>
#include <map>

using apex::cheats::aim;
using namespace std::chrono_literals;

void aim::run()
{
    while ( true ) {
        std::this_thread::sleep_for( 1ms );
        if ( !this->should_iterate() ) {
            this->reset_state();
            continue;
        }

        auto local_player = entity_list::get().get_local_player();
        if ( !local_player || !local_player->is_alive() ) {
            this->reset_state();
            continue; // invalid local player
        }

        if ( !this->get_bullet_info() ) {
            this->reset_state();
            continue; // invalid weapon somehow
        }

        auto target = this->get_best_entity();
        if ( !target ) {
            this->reset_state();
            continue; // no target to aim
        }

        this->aim_at( target );
    }
}

bool aim::should_iterate() const noexcept
{
    if ( !utils::process::get().good() ) {
        return false;
    }

    return options->aimbot.enabled && utils::is_key_down( static_cast<apex::utils::keycode_t>( options->aimbot.key ) );
}

std::optional<std::pair<float, float>> aim::get_bullet_info()
{
    auto local_player = entity_list::get().get_local_player();

    auto weapon_handle = local_player->get_primary_weapon_handle();
    if ( !weapon_handle || ( weapon_handle == static_cast<std::uint32_t>( -1 ) ) ) {
        return std::nullopt;
    }

    auto weapon = static_cast<sdk::weapon_t *>( entity_list::get().at( weapon_handle & 0xffff ) );
    if ( !weapon || !weapon->is_weapon() ) {
        return std::make_pair( 1.f, 1.f );
    }

    return std::make_pair( weapon->get_bullet_velocity(), weapon->get_bullet_gravity_scale() );
}
apex::sdk::entity_t *aim::get_best_entity()
{
    // 'true' if user wants target selection to use distance instead of fov
    const auto use_distance = options->aimbot.target_selection_switch_on_key && utils::is_key_down( static_cast<utils::keycode_t>( options->aimbot.target_selection_key ) );

    auto local_player = entity_list::get().get_local_player();
    auto smallest_unit = use_distance ? std::numeric_limits<float>::max() : options->aimbot.fov;
    auto ans = static_cast<sdk::entity_t *>( nullptr );
    // spdlog::info( "use_distance: {}, options->aimbot.target_lock: {}, ( this->m_target_index != 0 ): {}", use_distance, options->aimbot.target_lock, ( this->m_target_index != 0 )  );

    if ( options->aimbot.target_lock && !use_distance && ( this->m_target_index != 0 ) ) {
        auto target = entity_list::get().at( this->m_target_index );
            // spdlog::info( "same target" );

        // as long as the same target is valid keep at it
        if ( validate_entity( target ) ) {
            return target;
        }
    }

    for ( auto &e : entity_list::get() ) {
        if ( !validate_entity( e ) ) {
            continue;
        }

        auto entity_position = this->select_pos( e );
        // spdlog::info( "entity_position x {}", entity_position.x() );

        float current_unit = 0.f;
        if ( use_distance ) {
            current_unit = entity_position.distance_from( local_player->get_pos() ) * 0.01905f;
        } else {
            current_unit = math::get_fov(
                local_player->get_angles(),
                math::vector_angles( local_player->get_pos(), entity_position ) );
            // spdlog::info( "current_unit {}", current_unit );

        }

        if ( current_unit >= smallest_unit ) {
            continue;
        }
        // spdlog::info( "smallest unit {}", smallest_unit);
        ans = e;
        smallest_unit = current_unit;
    }

    return ans;
}
void aim::aim_at( sdk::entity_t *entity )
{
    auto entity_pos = this->select_pos( entity );
    if ( entity_pos.is_zero() ) {
        this->reset_state();
        return;
    }

    auto local_player = entity_list::get().get_local_player();
    auto local_pos = local_player->get_camera_pos();

    auto angles = math::vector_angles( local_pos, entity_pos );
    if ( !this->compensate_pos( entity->as<sdk::player_t>(), entity_pos, angles ) ) {
        this->reset_state();
        return;
    }

    if ( options->aimbot.rcs_enabled && !this->compensate_recoil( angles ) ) {
        this->reset_state();
        return;
    }

    if ( options->aimbot.smooth_enabled && !this->smooth( angles ) ) {
        this->reset_state();
        return;
    }

    if ( entity->is_player() ) {
        options->aimbot.current_target = entity->as<sdk::player_t>()->get_base();
    }

    angles.clamp();
    this->m_target_index = entity->index;
    utils::process::get().write( local_player->get_base() + offsets_t::get().local_angles, angles, sizeof( float ) * 2 );
}
bool aim::validate_entity( sdk::entity_t *entity ) const noexcept
{
    if ( !entity ) {
        return false;
    }

    auto local_player = entity_list::get().get_local_player();
    if ( entity->is_player() ) {
        auto player = entity->as<sdk::player_t>();
        if ( player->get_base() == local_player->get_base() ) {
            return false;
        }

        if ( player->get_pos().is_zero() ) {
            return false;
        }

        if ( ( player->get_pos().distance_from( local_player->get_pos() ) * 0.01905f ) > options->aimbot.maximum_distance ) {
            return false;
        }

        if ( !player->is_alive()) {
            return false;
        }

        auto is_friend = local_player->get_team() == player->get_team();
        if ( is_friend && !options->aimbot.aim_on_friends ) {
            return false;
        }

        if ( options->aimbot.vis_check && !player->is_visible() ) {
            return false;
        }

        // marked as 'friend'
        /*if ( player->priority == -1 ) {
            return false;
        }*/

        return true;
    }

    // for debugging
    else if ( entity->is_dummy() && options->aimbot.aim_on_dummies ) {
        auto dummy = entity->as<sdk::player_t>();
        if ( dummy->get_pos().is_zero() ) {
            return false;
        }

        if ( options->aimbot.vis_check && !dummy->is_visible() ) {
            return false;
        }

        if ( ( dummy->get_pos().distance_from( local_player->get_pos() ) * 0.01905f ) > options->aimbot.maximum_distance ) {
            return false;
        }
        
        return true;
    }

    return false;
}

apex::math::vector3 aim::select_pos( sdk::entity_t *entity )
{
    math::vector3 random_factor { rand_aim_x, rand_aim_y, rand_aim_z };

    // if ( entity->is_dummy() ) {
    //     auto pos = entity->as<sdk::player_t>()->get_pos();
    //     pos.z() += 64.f;

    //     if ( options->aimbot.random_aim_spot ) {
    //         pos = pos + random_factor;
    //     }

    //     return pos;
    // }

    auto player = entity->as<sdk::player_t>();
    auto bones = player->get_bone_matrix();

    auto bone_index = options->aimbot.primary_hitbox;
    if ( utils::is_key_down( static_cast<utils::keycode_t>( options->aimbot.secondary_hitbox_key ) ) ) {
        bone_index = options->aimbot.secondary_hitbox;
    }

    math::vector3 bone_off;
	int bone = player->bone_by_hit_box(bone_index);

	math::matrix3x4 bone_matrix = math::matrix3x4();
	utils::process::get().read(bones + (bone*sizeof(math::matrix3x4)), bone_matrix);
	bone_off = math::vector3(bone_matrix[0][3], bone_matrix[1][3], bone_matrix[2][3]);

    // float x = 0.f, y = 0.f, z = 0.f;
    // utils::process::get().read( bone_matrix + 0xcc + static_cast<uintptr_t>( bone_index ) * 0x30, x );
    // utils::process::get().read( bone_matrix + 0xdc + static_cast<uintptr_t>( bone_index ) * 0x30, y );
    // utils::process::get().read( bone_matrix + 0xec + static_cast<uintptr_t>( bone_index ) * 0x30, z );
    // if ( bone_off.is_zero() ) return math::vector3 {0};

    auto ans = bone_off + player->get_pos();
    if ( options->aimbot.random_aim_spot ) {
        ans = ans + random_factor;
    }

    return ans;
}
bool aim::compensate_pos( sdk::player_t *player, math::vector3 &pos, math::qangle &ang )
{
    auto local_player = entity_list::get().get_local_player();

    auto bullet_info = this->get_bullet_info();
    if ( !bullet_info ) {
        return false;
    }

    auto [ bullet_vel, bullet_grav ] = *bullet_info;
    if ( bullet_vel > 1.f ) {
        auto velocity = player->get_velocity() - local_player->get_velocity();

        math::LinearPredictor pred { pos, velocity };
        math::Solution sol;

        math::ProjectileWeapon weapon { bullet_vel, bullet_grav * 750.f };
        if ( math::solve( local_player->get_camera_pos(), weapon, pred, sol ) ) {
            ang = { -math::to_degrees( sol.pitch ), math::to_degrees( sol.yaw ), 0.f };
            return true;
        }

        return false;
    }

    return true;
}
bool aim::compensate_recoil( math::qangle &angles )
{
    auto local_player = entity_list::get().get_local_player();

    auto punch = local_player->get_recoil();
    if ( !punch.is_zero() ) {
        angles = angles - punch;
    }

    return true;
}
void aim::reset_state()
{
    options->aimbot.current_target = 0ull;
    this->m_target_index = 0;

    // smooth values must be at least 0.3 apart
    this->smooth_x = 0.f;
    this->smooth_y = 0.f;
    while ( abs( smooth_x - smooth_y ) < 0.3f ) {
        this->smooth_x = options->aimbot.smooth_factor + math::rand_float( -1.f, 2.3f );
        this->smooth_y = options->aimbot.smooth_factor + math::rand_float( -0.8f, 1.3f );
    }

    if ( options->aimbot.random_aim_spot ) {
        rand_aim_x = 0.f;
        rand_aim_y = math::rand_float( -1.7f, 2.f );
        rand_aim_z = math::rand_float( -0.3f, 2.1f );
    }
}

float aim::get_fov_scale() {
	if (entity_list::get().get_local_player()->is_in_zoom()) {
        auto weapon_handle = entity_list::get().get_local_player()->get_primary_weapon_handle();
        if ( !weapon_handle || ( weapon_handle == static_cast<std::uint32_t>( -1 ) ) ) {
            return 1.0f;
        }

		if (const auto weapon = static_cast<sdk::weapon_t *>( entity_list::get().at( weapon_handle & 0xffff ) )) {
			if (weapon->get_target_zoom_fov() != 0.0f && weapon->get_target_zoom_fov() != 1.0f) {
				return weapon->get_target_zoom_fov() / 90.0f;
			}
		}
	}
	return 1.0f;
}

bool aim::smooth( math::qangle &angles )
{
    auto current_angle = entity_list::get().get_local_player()->get_angles();

    // // start at 85% speed and go up to 120% depending on fov
    // float multiplier = std::min( std::max( 1.2f - ( math::get_fov( angles, current_angle ) * 0.8f ), 0.85f ), 1.2f );

    auto delta = angles - current_angle;
    delta.clamp();

    if ( !delta.is_zero() ) {
        const float fov_scale = get_fov_scale();
        // Stop aiming when this close to a target to prevent mouse jitter.
        float fov_min = 0.25f;
        if (delta.length() < fov_min * fov_scale) {
            angles = math::qangle(0);
            return false;
        }
        // Magic aim smoothing formula :)
        const float aim_strength = options->aimbot.smooth_factor;
        const float speed = logf(aim_strength + delta.length() / (fov_scale * fov_scale) * aim_strength) * aim_strength + aim_strength;
        delta.x() /= speed;
        delta.y() /= speed;
    //     delta.x() /= std::max( smooth_x * multiplier, 1.5f );
    //     delta.y() /= std::max( smooth_y * multiplier, 1.5f );
    }

    angles = current_angle + delta;

    return true;
}