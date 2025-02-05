#pragma once

#include <cstdint>

namespace apex::sdk
{
    constexpr auto MAXPLAYERS = 100;
    constexpr auto MAXENTRIES = 10000;

    enum class bleedout_state_t : int32_t
    {
        alive,
        entering_bleedstate,
        bleeding,
        exiting_bleedstate
    };

    enum class class_id : std::int32_t
    {
        CSkyCamera = 0x4f,
        CBaseEntity = 0x5,
        CZipline = 0x6d,
        CZiplineEnd = 0x6e,
        CWeaponX = 0x6b,
        CTEProjectileTrail = 0x58,
        CProjectile = 0x3c,
        CMissile = 0x2b,
        CCrossbowBolt = 0x11,
        CVortexSphere = 0x69,
        CTurret = 0x67,
        CScriptTraceVolume = 0x4d,
        CGrappleHook = 0x21,
        CPredictedFirstPersonProxy = 0x3b,
        CFirstPersonProxy = 0x1b,
        Titan_Cockpit = 0,
        CPortal_PointPush = 0x39,
        CInfoPlacementHelper = 0x26,
        CHealthKit = 0x24,
        CBaseParticleEntity = 0,
        CTriggerSlip = 0x65,
        CTriggerPlayerMovement = 0x63,
        CTriggerPointGravity = 0x64,
        CTriggerCylinderHeavy = 0x5f,
        CStatusEffectPlugin = 0x50,
        CScriptMoverTrainNode = 0x41,
        DoorMover = 0x6f,
        ScriptMoverLightweight = 0x70,
        CScriptMover = 0x40,
        CScriptNetDataGlobalNonRewind = 0x4b,
        CScriptNetDataGlobal = 0x4a,
        CScriptNetData_SNDC_DEATH_BOX = 0x43,
        CScriptNetData_SNDC_TITAN_SOUL = 0x49,
        CScriptNetData_SNDC_PLAYER_EXCLUSIVE_EXPANDED = 0x47,
        CScriptNetData_SNDC_PLAYER_EXCLUSIVE = 0x46,
        CScriptNetData_SNDC_PLAYER_GLOBAL = 0x48,
        CScriptNetData_SNDC_GLOBAL_NON_REWIND = 0x45,
        CScriptNetData_SNDC_GLOBAL = 0x44,
        CScriptNetData = 0x42,
        CRopeKeyframe = 0x3f,
        CPlayerWaypoint = 0x37,
        CPlayerTasklist = 0x35,
        CEnvDecoy = 0x19,
        CPlayerDecoy = 0x34,
        CGlobalNonRewinding = 0x20,
        CGameRulesProxy = 0x1f,
        CEntityBlocker = 0x16,
        CTEScriptParticleSystemOnEntityWithPos = 0x5b,
        CTEScriptParticleSystemOnEntity = 0x5a,
        CTEScriptParticleSystem = 0x59,
        CPropDoor = 0x3d,
        CInfoTargetGravity = 0x28,
        CInfoTarget = 0x27,
        CInfoTargetMinimap = 0x29,
        CBaseViewModel = 0xa,
        CBaseGrenade = 0x6,
        CWorld = 0x6c,
        CEntityLinkPage = 0x18,
        CWaterLODControl = 0x6a,
        CVGuiScreen = 0x68,
        CPlayerVehicle = 0x36,
        CTriggerUpdraft = 0x66,
        CTriggerNoObjectPlacement = 0x61,
        CTriggerNoZipline = 0x62,
        CTriggerNoGrapple = 0x60,
        CBaseVPhysicsTrigger = 0xb,
        CBaseTrigger = 0x9,
        CTESoundDispatch = 0x5d,
        CTEPhysicsProp = 0x57,
        CTEParticleSystem = 0x56,
        CTEShatterSurface = 0x5c,
        CTEExplosion = 0x54,
        CTEEffectDispatch = 0x53,
        CTEBreakModel = 0x52,
        CHardPointFrontierEntity = 0x23,
        CHardPointEntity = 0x22,
        CTeam = 0x51,
        CImportantOnEntSound = 0x25,
        CShieldProp = 0x4e,
        CLootGrabber = 0x2a,
        CDeathBoxProp = 0x12,
        CScriptProp = 0x4c,
        CPropSurvival = 0x3e,
        CDynamicPropLightweight = 0x15,
        CDynamicProp = 0x14,
        CPostProcessController = 0x3a,
        CPointCamera = 0x38,
        CStatueProp = 0,
        CPhysicsProp = 0x32,
        CParticleSystem = 0x31,
        CNPC_SentryTurret = 0x2f,
        CNPC_Titan = 0x30,
        CNPC_Dropship = 0x2e,
        CNPC_Drone = 0x2d,
        CMovieDisplay = 0x2c,
        CTEGibEvent = 0x55,
        CFuncMoveLinear = 0x1e,
        CFuncBrushLightweight = 0x1d,
        CFuncBrush = 0x1c,
        CBreakableSurface = 0xe,
        CCascadeLight = 0xf,
        CEntityDissolve = 0x17,
        CEnvWind = 0x1a,
        CDynamicLight = 0x13,
        CColorCorrection = 0x10,
        CBreakableProp = 0xd,
        CBaseButton = 0,
        CBaseToggle = 0x8,
        CBaseTempEntity = 0x7,
        CBaseCombatCharacter = 0x4,
        CBaseAnimatingOverlay = 0x3,
        CAmbientGeneric = 0x1,
        CTitanSoul = 0x5e,
        CAI_BaseNPC = 0,
        CPlayer = 0xa4,
        CBoneFollower = 0xc,
        CBaseAnimating = 0x2
    };

    enum class script_int_id : std::int32_t
    {
        undefined = -1,
        // weapons
        kraber = 1,
        mastiff = 2,
        lstar = 3,
        havoc = 4,
        havoc_legendary = 5,
        devotion = 6,
        triple_take = 7,
        triple_take_legendary = 8,
        flatline = 9,
        flatline_legendary = 10,
        hemlok = 11,
        g7_scout = 12,
        alternator = 14,
        alternator_legendary = 15,
        r99 = 16,
        prowler = 17,
        prowler_legendary = 18,
        longbow = 19,
        longbow_legendary = 21,
        charge_rifle = 21,
        charge_rifle_legendary = 22,
        spitfire = 23,
        r301 = 24,
        eva8_auto = 26,
        peacekeeper = 28,
        peacekeeper_legendary = 30,
        mozambique = 30,
        mozambique_legendary = 31,
        wingman = 32,
        wingman_legendary = 34,
        p2020 = 34,
        p2020_legendary = 35,
        re45 = 36,
        re45_leg = 37,
        sentinel = 38,
        // ammo
        light_rounds = 40,
        energy_ammo,
        shotgun_shells,
        heavy_rounds,
        sniper_ammo,
        // healing items
        ultimate_accelerant,
        phoenix_kit,
        med_kit,
        syringe,
        shield_battery,
        shield_cell,
        // shield items
        helmet_lv1 = 51,
        helmet_lv2,
        helmet_lv3,
        helmet_lv4,
        body_armor_lv1,
        body_armor_lv2,
        body_armor_lv3,
        body_armor_lv4,
        knockdown_shield_lv1,
        knockdown_shield_lv2,
        knockdown_shield_lv3,
        knockdown_shield_lv4,
        backpack_lv1 = 63,
        backpack_lv2,
        backpack_lv3,
        backpack_lv4,
        // grenades
        thermite,
        frag_grenade,
        arc_star,
        // scopes
        hcog_1x,
        hcog_2x,
        holo_1x,
        variable_holo_1x2x,
        digital_thread_1x,
        hcog_3x,
        variable_aog_2x4x,
        sniper_6x,
        variable_sniper_4x8x,
        digital_sniper_thread_4x10x,

        // attachments
        barrel_stabilizer_lvl1,
        barrel_stabilizer_lvl2,
        barrel_stabilizer_lvl3,
        barrel_stabilizer_lvl4,
        light_mag_lvl1,
        light_mag_lvl2,
        light_mag_lvl3,
        heavy_mag_lvl1,
        heavy_mag_lvl2,
        heavy_mag_lvl3,
        energy_mag_lvl1,
        energy_mag_lvl2,
        energy_mag_lvl3,
        shotgun_bolt_lv1,
        shotgun_bolt_lv2,
        shotgun_bolt_lv3,
        standard_stock_lv1,
        standard_stock_lv2,
        standard_stock_lv3,
        sniper_stock_lv1,
        sniper_stock_lv2,
        sniper_stock_lv3,

        // hop ups
        turbocharger = 100,
        selectfire_receiver = 101,
        precision_choke = 102,
        hammerpoint = 103,
        anvil_receiver = 104,
        double_tap_trigger = 105
    };

    enum class crouchstate_t
    {
        standing,
        crouching,
        crouched,
        uncrouching
    };

    enum class aimbones_t
    {
        head = 10,
        upper_neck = 9,
        neck = 8,
        spine_c = 7,
        spine_b = 6,
        spine_a = 5,
        hip = 4,
        left_shoulder = 14,
        left_elbow = 18,
        left_wrist = 20,
        right_shoulder = 39,
        right_elbow = 43,
        right_wrist = 45,
        left_thigh = 63,
        left_knee = 65,
        left_ankle = 66,
        right_thigh = 68,
        right_knee = 70,
        right_ankle = 71,
    };

    enum class signonstate_t : std::uint32_t
    {
        none,
        challenge,
        connected,
        state_new,
        prespawn,
        gettingdata,
        spawn,
        first_snap,
        full,
        changelevel
    };

    // studio.h
    struct studiohdr_t
    {
        int id;
        int version;
        int checksum;
        char name[ 64 ];
    };

    struct ClientClass {
        uint64_t pCreateFn;
        uint64_t pCreateEventFn;
        uint64_t pNetworkName;
        uint64_t pRecvTable;
        uint64_t pNext;
        uint32_t ClassID;
        uint32_t ClassSize;
    };

    struct GlowMode
    {
        uint8_t GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
    };
    struct Fade
    {
        int a, b;
        float c, d, e, f;
    };

    class CGlobalVars
    {
    public:
        double realtime;
        int32_t framecount; //0x0008
        float absoluteframetime; //0x000C
        float curtime; //0x0010
        float N00000047; //0x0014
        float N00000018; //0x0018
        float N0000004A; //0x001C
        float N00000019; //0x0020
        float N0000004D; //0x0024
        float N0000001A; //0x0028
        char pad_002C[4]; //0x002C
        float N0000001B; //0x0030
        char pad_0034[12]; //0x0034
        int32_t tickCount; //0x0040
        float intervalPerTick; //0x0044
    }; //Size: 0x0048
}