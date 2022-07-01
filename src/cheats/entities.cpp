#include "entities.hpp"

#include <array>
#include <chrono>
#include <thread>
#include <spdlog/spdlog.h>
using namespace std::chrono_literals;

#include "cheats/offsets.hpp"
#include "utils/process.hpp"
#include "config/options.hpp"
#include "sdk/sdk.hpp"

int get_class_id( std::uintptr_t ent )
{
    std::uintptr_t client_nttable = 0u;
    if ( apex::utils::process::get().read<std::uintptr_t>( ent + 24, client_nttable ); !client_nttable ) {
        return -1;
    }

    std::uintptr_t get_client_class_fn = 0u;
    if ( apex::utils::process::get().read<std::uintptr_t>( client_nttable + 24, get_client_class_fn ); !( get_client_class_fn ) ) {
        return -1;
    }

    std::uint32_t relative_table_offs = 0u;
    if ( apex::utils::process::get().read<std::uint32_t>( get_client_class_fn + 3, relative_table_offs ); !( relative_table_offs ) ) {
        return -1;
    }

    auto id = 0;
    apex::utils::process::get().read<std::int32_t>( get_client_class_fn + relative_table_offs + 7 + 0x28, id );

    return id;
}

bool get_class_name( std::uintptr_t ent, char (&class_buffer)[128] )
{
    std::uintptr_t client_nttable = 0u;
    if ( apex::utils::process::get().read<std::uintptr_t>( ent + 24, client_nttable ); !client_nttable ) {
        return false;
    }

    std::uintptr_t get_client_class_fn = 0u;
    if ( apex::utils::process::get().read<std::uintptr_t>( client_nttable + 24, get_client_class_fn ); !( get_client_class_fn ) ) {
        return false;
    }

    std::uint32_t relative_table_offs = 0u;
    if ( apex::utils::process::get().read<std::uint32_t>( get_client_class_fn + 3, relative_table_offs ); !( relative_table_offs ) ) {
        return false;
    }

	apex::sdk::ClientClass client_class;
    if( !apex::utils::process::get().read<apex::sdk::ClientClass>( get_client_class_fn + relative_table_offs + 7, client_class ) )
    {
        return false;
    }

    return apex::utils::process::get().read<char [128]>( client_class.pNetworkName, class_buffer );
}

apex::cheats::entity_list::entity_list()
    : m_local_player_index( 0 )
    , m_ent_info()
    , m_old_ent_info()
{
    m_entity_list.reserve( sdk::MAXENTRIES );
    m_ent_info.reserve( sdk::MAXENTRIES );
    m_old_ent_info.reserve( sdk::MAXENTRIES );

    for ( auto i = 0u; i < sdk::MAXENTRIES; ++i ) {
        m_entity_list.push_back( {} );
        m_ent_info.push_back( {} );
        m_old_ent_info.push_back( {} );
    }
}


// const size_t NUM_ENT_ENTRIES = 0x10000;
// struct EHandle {
// 	uint32_t value = 0xffffffff;

// 	inline bool is_valid() const { return value != 0xffffffff; }
// 	inline size_t index() const { return value & static_cast<uint32_t>(NUM_ENT_ENTRIES - 1); }
// };

void apex::cheats::entity_list::run()
{
    while ( true ) {
        std::this_thread::sleep_for( 1ms );
        if ( !utils::process::get().good() ) {
            continue;
        }

        std::swap( this->m_ent_info, this->m_old_ent_info );

        // read the entire entity list
        utils::process::get().read_ptr( utils::process::get().base_address() + offsets_t::get().entity_list, std::data( this->m_ent_info ), sdk::MAXENTRIES * sizeof( sdk::ent_info_t ) );
        utils::process::get().read( utils::process::get().base_address() + offsets_t::get().local_player_index, this->m_local_player_index );


        for ( auto i = 0u; i < sdk::MAXENTRIES; ++i ) {
            auto &ent = this->m_entity_list[ i ];
            auto &prev_info = this->m_old_ent_info[ i ];
            auto &cur_info = this->m_ent_info[ i ];

            if ( prev_info.entity_ptr == cur_info.entity_ptr ) {
                if ( ent ) ent->update();
                // spdlog::info( "prev_info.entity_ptr == cur_info.entity_ptr" );
            }

            else if ( cur_info.entity_ptr != 0u ) {
                delete ent;
                ent = nullptr;

	            char class_buffer[128];
                // auto c_id = static_cast<sdk::class_id>( get_class_id( cur_info.entity_ptr ) );
                get_class_name(cur_info.entity_ptr, class_buffer);
                // if ( ( c_id == sdk::class_id::CPlayer ) || ( c_id == sdk::class_id::Titan_Cockpit ) ) {
                //     ent = ( new sdk::player_t( cur_info.entity_ptr ) );
                // } else if ( c_id == sdk::class_id::CPropSurvival ) {
                //     ent = ( new sdk::item_t( cur_info.entity_ptr ) );
                // } else if ( c_id == sdk::class_id::CWeaponX ) {
                //     ent = ( new sdk::weapon_t( cur_info.entity_ptr ) );
                // }

                if (!strcmp(class_buffer, "CPlayer") || !strcmp(class_buffer, "Titan_Cockpit") || !strcmp(class_buffer, "CAI_BaseNPC") ) {
                    // spdlog::info( "Class buffer: {}", class_buffer );
                    ent = ( new sdk::player_t( cur_info.entity_ptr ) );
                }
                if (!strcmp(class_buffer, "CPropSurvival")) {
                    // spdlog::info( "Class buffer: {}", class_buffer );
                    ent = ( new sdk::item_t( cur_info.entity_ptr ) );
                }
                if (!strcmp(class_buffer, "CWeaponX")) {
                    // spdlog::info( "Class buffer: {}", class_buffer );
                    ent = ( new sdk::weapon_t( cur_info.entity_ptr ) );
                }
    
                if ( ent ) {
                    ent->index = i;
                    strcpy( ent->class_name, class_buffer) ;
                    ent->update();
                }
            }

            else {
                delete ent;
                ent = nullptr;
            }
        }
    }
}

apex::sdk::entity_t *apex::cheats::entity_list::at( std::size_t i ) noexcept
{
    if ( i >= m_entity_list.size() ) {
        return nullptr;
    }

    return m_entity_list[ i ];
}
bool apex::cheats::entity_list::validate( sdk::entity_t *ent )
{
    if ( !ent ) {
        return false;
    }

    sdk::ent_info_t ei;
    utils::process::get().read( utils::process::get().base_address() + ( offsets_t::get().entity_list + ent->index * sizeof( sdk::ent_info_t ) ), ei );

    constexpr uint32_t invalid_handle = 0xffffffffu;

    if ( ent->is_player() || ent->is_dummy() ) {
        return ( ent->as<sdk::player_t>()->get_handle() != invalid_handle ) && ( ent->as<sdk::player_t>()->get_base() == ei.entity_ptr );
    } else if ( ent->is_item() ) {
        return ( ent->as<sdk::player_t>()->get_handle() != invalid_handle ) && ( ent->as<sdk::item_t>()->get_base() == ei.entity_ptr );
    } else {
        return ( ent->as<sdk::player_t>()->get_handle() != invalid_handle ) && ( ent->as<sdk::weapon_t>()->get_base() == ei.entity_ptr );
    }
}