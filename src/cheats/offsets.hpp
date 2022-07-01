#pragma once

#include <cstdint>
#include "utils/singleton.hpp"

#include "memflow.h"
#include "memflow_win32.h"

namespace apex::cheats
{
    class offsets_t : public utils::singleton<offsets_t>
    {
    public:
        void init();

        std::uintptr_t entity_list;
        std::uintptr_t local_player_index;
        std::uintptr_t clientstate;
        std::uintptr_t timescale;
        std::uintptr_t name_list;
        std::uintptr_t global_vars;
        uint32_t team = 0x0448; // [RecvTable.DT_Player] m_iTeamNum
        uint32_t health = 0x0438; // [RecvTable.DT_Player] m_iHealth
        uint32_t max_health = 0x0578; //[RecvTable.DT_Player] m_iMaxHealth
        uint32_t bleedout = 0x2720; // m_bleedoutState, >0 = knocked

        uint32_t camera = 0x1f40; // [Miscellaneous] CPlayer!camera_origin
        uint32_t local_angles = 0x2594 - 0x14; //m_ammoPoolCapacity - 0x14
        uint32_t breath = local_angles - 0x10;
        uint32_t origin = 0x014c; // [DataMap.C_BaseEntity] m_vecAbsOrigin

        uint32_t is_zooming = 0x1c51; // [RecvTable.DT_Player] m_bZooming
        uint32_t sign_on_state = 0x137e258; // [Miscellaneous] SignonState
        uint32_t level_name_short_offset = 0x137e370; // [Miscellaneous] LevelName

        uint32_t latest_primary_weapon = 0x1a6c; // m_latestPrimaryWeapons=0x1a6c
        uint32_t latest_non_offhand_weapon = 0x1a7c; // m_latestNonOffhandWeapons=0x1a7c

        uint32_t entity_bones = 0x0ef0 + 0x48 ; // m_nForceBone + 0x48
        uint32_t collision = 0x04c0; // [DataMapTypes.C_BaseEntity] m_Collision 
        uint32_t last_visible_time = 0x1ad4; // CPlayer!lastVisibleTime ("First One")
        uint32_t weaponx_target_fov = 0x1718 + 0x00bc; // m_playerData + m_targetZoomFOV

        uint32_t bullet_velocity = 0x1f28; // CWeaponX!m_flProjectileSpeed=0x1f28
        uint32_t bullet_gravity_scale = 0x1f30; // CWeaponX!m_flProjectileScale=0x1f30

        uint32_t head_component = 0x3F04;
        uint32_t eye_pos = 0x3F04;
        uint32_t velocity = 0x0428; // [RecvTable.DT_LocalPlayerExclusive] m_vecBaseVelocity
        uint32_t signifier_name_ptr = 0x0580;

        uint32_t recoil = 0x2498;  // [DataMap.C_Player] m_currentFrameLocalPlayer.m_vecPunchWeapon_Angle
        uint32_t sway = local_angles - 0x10; // OFFSET_BREATH_ANGLES
        uint32_t duckstate = 0x2a1c; // m_duckState
        uint32_t fflags = 0x0098; // [RecvTable.DT_Player] m_fFlags
        uint32_t jump_state = 0x07544298 + 0x8; // kbutton_t::state
        uint32_t studiohdr = 0x1150;
        uint32_t custom_script_int = 0x16a8; // [RecvTableTypes.DT_PropSurvival] m_customScriptInt
        uint32_t weapon_name_index = 0x1874; // [RecvTable.DT_WeaponX] m_weaponNameIndex
        uint32_t shield_health = 0x0170; // [RecvTable.DT_BaseEntity] m_shieldHealth
        uint32_t shield_health_max = 0x0174; // [RecvTable.DT_BaseEntity] m_shieldHealthMax
        uint32_t player_name = 0x0589; // [RecvTable.DT_BaseEntity] m_iName
        std::uintptr_t gafAsyncKeyState;
    };
}