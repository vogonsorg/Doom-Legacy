// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id$
//
// Copyright (C) 1993-1996 by id Software, Inc.
// Copyright (C) 1998-2004 by DooM Legacy Team.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
//
//
//-----------------------------------------------------------------------------

/// \file
/// \brief Main interface for sound and music
///
/// Originally created by the sound utility written by Dave Taylor.
/// Simple sound interface using sound/music IDs (wrappers).

#ifndef sounds_h
#define sounds_h 1


// conventional Hexen soundsequence offsets
// NOTE! These must be at least 10 units apart!
enum soundseq_e
{
  SEQ_PLAT =  0, // offset for most things
  SEQ_DOOR = 10, // offset for doors and polyobjects
  SEQ_ENV  = 20  // offset for wind etc.
};


// wrappers
int  S_StartAmbSound(class PlayerInfo *p, int sfx_id, float volume = 1.0);
int  S_StartLocalAmbSound(int sfx_id, float volume = 1.0);
int  S_StartSound(struct mappoint_t *origin, int sfx_id, float volume = 1.0);
int  S_StartSound(class Actor *origin, int sfx_id, float volume = 1.0);
bool S_PlayingSound(Actor *origin, int sfx_id);
bool S_StartMusic(int music_id, bool looping = false);

void S_ClearSounds();
int  S_Read_SNDINFO(int lump);
int  S_Read_SNDSEQ(int lump);
void S_PrecacheSounds();
int  S_GetSoundID(const char *tag);



// the complete set of music
extern char* MusicNames[];


// Identifiers for all music in game.
// note! Some pieces are really reused in Doom and Heretic.
enum musicenum_t
{
  mus_None = 0,

  // Doom
  mus_e1m1,
  mus_e1m2,
  mus_e1m3,
  mus_e1m4,
  mus_e1m5,
  mus_e1m6,
  mus_e1m7,
  mus_e1m8,
  mus_e1m9,

  mus_e2m1,
  mus_e2m2,
  mus_e2m3,
  mus_e2m4,
  mus_e2m5,
  mus_e2m6,
  mus_e2m7,
  mus_e2m8,
  mus_e2m9,

  mus_e3m1,
  mus_e3m2,
  mus_e3m3,
  mus_e3m4,
  mus_e3m5,
  mus_e3m6,
  mus_e3m7,
  mus_e3m8,
  mus_e3m9,
  // Ultimate Doom, Episode 4
  mus_e4m1, // mus_e3m4 // American
  mus_e4m2, // mus_e3m2 // Romero
  mus_e4m3, // mus_e3m3 // Shawn
  mus_e4m4, // mus_e1m5 // American
  mus_e4m5, // mus_e2m7 // Tim
  mus_e4m6, // mus_e2m4 // Romero
  mus_e4m7, // mus_e2m6 // J.Anderson CHIRON.WAD
  mus_e4m8, // mus_e2m5 // Shawn
  mus_e4m9, // mus_e1m9 // Tim

  mus_inter,
  mus_intro,
  mus_bunny,
  mus_victor,
  mus_introa,

  // Doom II
  mus_runnin, // map01
  mus_stalks,
  mus_countd,
  mus_betwee,
  mus_doom,
  mus_the_da,
  mus_shawn,
  mus_ddtblu,
  mus_in_cit,
  mus_dead,
  mus_stlks2,
  mus_theda2,
  mus_doom2,
  mus_ddtbl2,
  mus_runni2,
  mus_dead2,
  mus_stlks3,
  mus_romero,
  mus_shawn2,
  mus_messag,
  mus_count2,
  mus_ddtbl3,
  mus_ampie,
  mus_theda3,
  mus_adrian,
  mus_messg2,
  mus_romer2,
  mus_tense,
  mus_shawn3,
  mus_openin,
  mus_evil,
  mus_ultima, //map32
  mus_read_m,
  mus_dm2ttl,
  mus_dm2int,

  // Heretic
  mus_he1m1,
  mus_he1m2,
  mus_he1m3,
  mus_he1m4,
  mus_he1m5,
  mus_he1m6,
  mus_he1m7,
  mus_he1m8,
  mus_he1m9,
            
  mus_he2m1,
  mus_he2m2,
  mus_he2m3,
  mus_he2m4,
  mus_he2m5,
  mus_he2m6,
  mus_he2m7,
  mus_he2m8,
  mus_he2m9,

  mus_he3m1,
  mus_he3m2,
  mus_he3m3,
  mus_he3m4,
  mus_he3m5,
  mus_he3m6,
  mus_he3m7,
  mus_he3m8,
  mus_he3m9,

  mus_he4m1,
  mus_he4m2,
  mus_he4m3,
  mus_he4m4,
  mus_he4m5,
  mus_he4m6,
  mus_he4m7,
  mus_he4m8,
  mus_he4m9,

  mus_he5m1,
  mus_he5m2,
  mus_he5m3,
  mus_he5m4,
  mus_he5m5,
  mus_he5m6,
  mus_he5m7,
  mus_he5m8,
  mus_he5m9,

  mus_he6m1,
  mus_he6m2,
  mus_he6m3,
            
  mus_htitl,
  mus_hintr,
  mus_hcptd,
            
  NUMMUSIC
};


// Identifiers for certain sfx in game. This is the old way of using sounds.
// NOTE! This enum MUST be in sync with the SNDINFO lump in legacy.wad.

enum sfxenum_t
{
  sfx_None,   // 0

  // first the common sounds
  sfx_message,
  sfx_menu_open,
  sfx_menu_close,
  sfx_menu_move,
  sfx_menu_choose,
  sfx_menu_fail,
  sfx_menu_adjust,
  sfx_jump,
  sfx_land,
  sfx_grunt,
  sfx_usefail,
  sfx_itemup,
  sfx_keyup,
  sfx_weaponup,
  sfx_artiup,
  sfx_powerup,
  sfx_artiuse,
  sfx_frag,
  sfx_secret,
  sfx_splash,
  sfx_enterwater,
  sfx_exitwater,
  sfx_quake,
  sfx_teleport,
  sfx_itemrespawn,
  sfx_gib,
  sfx_doorstart,
  sfx_doorstop,
  sfx_floormove,
  sfx_floorstop,
  sfx_ceilmove,
  sfx_ceilstop,
  sfx_platstart,
  sfx_platmove,
  sfx_platstop,
  sfx_switchon,
  sfx_switchoff,

  // Doom sounds
  sfx_Doom, sfx_doropn = sfx_Doom,
  sfx_dorcls,
  sfx_bdopn,
  sfx_bdcls,
  sfx_barexp,
  sfx_firsht,
  sfx_firxpl,
  sfx_punch,
  sfx_pistol,
  sfx_shotgn,
  sfx_sgcock,
  sfx_dshtgn,
  sfx_dbopn,
  sfx_dbcls,
  sfx_dbload,
  sfx_chgun,
  sfx_rlaunc,
  sfx_plasma,
  sfx_bfg,
  sfx_rxplod,
  sfx_sawful,
  sfx_sawup,
  sfx_sawidl,
  sfx_sawhit,
  sfx_plpain,
  sfx_pldeth,
  sfx_pdiehi,
  sfx_posit1,
  sfx_posit2,
  sfx_posit3,
  sfx_posact,
  sfx_popain,
  sfx_podth1,
  sfx_podth2,
  sfx_podth3,
  sfx_bgsit1,
  sfx_bgsit2,
  sfx_bgact,
  sfx_claw,
  sfx_bgdth1,
  sfx_bgdth2,
  sfx_sgtsit,
  sfx_dmact,
  sfx_sgtatk,
  sfx_dmpain,
  sfx_sgtdth,
  sfx_cacsit,
  sfx_cacdth,
  sfx_sklatk,
  sfx_kntsit,
  sfx_kntdth,
  sfx_brssit,
  sfx_brsdth,
  sfx_mansit,
  sfx_manatk,
  sfx_mnpain,
  sfx_mandth,
  sfx_pesit,
  sfx_pepain,
  sfx_pedth,
  sfx_skesit,
  sfx_skeact,
  sfx_skepch,
  sfx_skeswg,
  sfx_skeatk,
  sfx_skedth,
  sfx_bspsit,
  sfx_bspact,
  sfx_bspwlk,
  sfx_bspdth,
  sfx_vilsit,
  sfx_vilact,
  sfx_vilatk,
  sfx_vipain,
  sfx_vildth,
  sfx_flamst,
  sfx_flame,
  sfx_cybsit,
  sfx_hoof,
  sfx_cybdth,
  sfx_spisit,
  sfx_metal,
  sfx_spidth,
  sfx_sssit,
  sfx_ssdth,
  sfx_keenpn,
  sfx_keendt,
  sfx_bossit,
  sfx_bospit,
  sfx_boscub,
  sfx_bospn,
  sfx_bosdth,

  // Heretic sounds
  sfx_Heretic, sfx_waterfall = sfx_Heretic,
  sfx_wind,
  sfx_burn,
  sfx_stfhit,
  sfx_stfpow,
  sfx_stfcrk,
  sfx_gldhit,
  sfx_bowsht,
  sfx_blssht,
  sfx_blshit,
  sfx_ripslop,
  sfx_phosht,
  sfx_phohit,
  sfx_phopow,
  sfx_hrnsht,
  sfx_hrnhit,
  sfx_hrnpow,
  sfx_ramphit,
  sfx_ramrain,
  sfx_lobsht,
  sfx_lobhit,
  sfx_bounce,
  sfx_lobpow,
  sfx_gntful,
  sfx_gntact,
  sfx_gntuse,
  sfx_gnthit,
  sfx_gntpow,
  sfx_plrpai,
  sfx_plrwdth,    // Wimpy
  sfx_plrdth,     // Normal
  sfx_plrcdth,    // Crazy
  sfx_gibdth,     // Extreme
  sfx_impsit,
  sfx_impact,
  sfx_impat1,
  sfx_impat2,
  sfx_imppai,
  sfx_impdth,
  sfx_mumsit,
  sfx_mumact,
  sfx_mumat1,
  sfx_mumat2,
  sfx_mumhed,
  sfx_mumpai,
  sfx_mumdth,
  sfx_bstsit,
  sfx_bstact,
  sfx_bstatk,
  sfx_bstpai,
  sfx_bstdth,
  sfx_clksit,
  sfx_clkact,
  sfx_clkatk,
  sfx_clkpai,
  sfx_clkdth,
  sfx_snksit,
  sfx_snkact,
  sfx_snkatk,
  sfx_snkpai,
  sfx_snkdth,
  sfx_kgtsit,
  sfx_kgtact,
  sfx_kgtatk,
  sfx_kgtat2,
  sfx_kgtpai,
  sfx_kgtdth,
  sfx_wizsit,
  sfx_wizact,
  sfx_wizatk,
  sfx_wizpai,
  sfx_wizdth,
  sfx_minsit,
  sfx_minact,
  sfx_minat1,
  sfx_minat2,
  sfx_minat3,
  sfx_minpai,
  sfx_mindth,
  sfx_hedsit,
  sfx_hedact,
  sfx_hedat1,
  sfx_hedat2,
  sfx_hedat3,
  sfx_hedpai,
  sfx_heddth,
  sfx_sbtsit,
  sfx_sbtact,
  sfx_sbtatk,
  sfx_sbtpai,
  sfx_sbtdth,
  sfx_sorzap,
  sfx_sorrise,
  sfx_sorsit,
  sfx_soract,
  sfx_soratk,
  sfx_sorpai,
  sfx_sordsph,
  sfx_sordexp,
  sfx_sordbon,
  sfx_chicact,
  sfx_chicatk,
  sfx_chicpk1,
  sfx_chicpk2,
  sfx_chicpk3,
  sfx_chicpai,
  sfx_chicdth,
  sfx_newpod,
  sfx_podexp,
  sfx_volsht,
  sfx_volhit,
  sfx_amb1,
  sfx_amb2,
  sfx_amb3,
  sfx_amb4,
  sfx_amb5,
  sfx_amb6,
  sfx_amb7,
  sfx_amb8,
  sfx_amb9,
  sfx_amb10,
  sfx_amb11,

  // Hexen sounds
  sfx_Hexen, SFX_PLAYER_FIGHTER_NORMAL_DEATH = sfx_Hexen,
  SFX_PLAYER_FIGHTER_CRAZY_DEATH,
  SFX_PLAYER_FIGHTER_EXTREME1_DEATH,
  SFX_PLAYER_FIGHTER_EXTREME2_DEATH,
  SFX_PLAYER_FIGHTER_EXTREME3_DEATH,
  SFX_PLAYER_FIGHTER_BURN_DEATH,
  SFX_PLAYER_CLERIC_NORMAL_DEATH,
  SFX_PLAYER_CLERIC_CRAZY_DEATH,
  SFX_PLAYER_CLERIC_EXTREME1_DEATH,
  SFX_PLAYER_CLERIC_EXTREME2_DEATH,
  SFX_PLAYER_CLERIC_EXTREME3_DEATH,
  SFX_PLAYER_CLERIC_BURN_DEATH,
  SFX_PLAYER_MAGE_NORMAL_DEATH,
  SFX_PLAYER_MAGE_CRAZY_DEATH,
  SFX_PLAYER_MAGE_EXTREME1_DEATH,
  SFX_PLAYER_MAGE_EXTREME2_DEATH,
  SFX_PLAYER_MAGE_EXTREME3_DEATH,
  SFX_PLAYER_MAGE_BURN_DEATH,
  SFX_PLAYER_FIGHTER_PAIN,
  SFX_PLAYER_CLERIC_PAIN,
  SFX_PLAYER_MAGE_PAIN,
  SFX_PLAYER_FIGHTER_GRUNT,
  SFX_PLAYER_CLERIC_GRUNT,
  SFX_PLAYER_MAGE_GRUNT,
  SFX_PLAYER_POISONCOUGH,
  SFX_PLAYER_FIGHTER_FALLING_SCREAM,
  SFX_PLAYER_CLERIC_FALLING_SCREAM,
  SFX_PLAYER_MAGE_FALLING_SCREAM,
  SFX_PLAYER_FIGHTER_FAILED_USE,
  SFX_PLAYER_CLERIC_FAILED_USE,
  SFX_PLAYER_MAGE_FAILED_USE,
  SFX_PLATFORM_START,
  SFX_PLATFORM_STARTMETAL,
  SFX_PLATFORM_STOP,
  SFX_STONE_MOVE,
  SFX_METAL_MOVE,
  SFX_DOOR_OPEN,
  SFX_DOOR_LOCKED,
  SFX_DOOR_METAL_OPEN,
  SFX_DOOR_METAL_CLOSE,
  SFX_DOOR_LIGHT_CLOSE,
  SFX_DOOR_HEAVY_CLOSE,
  SFX_DOOR_CREAK,
  SFX_WEAPON_BUILD,
  SFX_ARTIFACT_BLAST,
  SFX_THUNDER_CRASH,
  SFX_FIGHTER_PUNCH_MISS,
  SFX_FIGHTER_PUNCH_HITTHING,
  SFX_FIGHTER_PUNCH_HITWALL,
  SFX_FIGHTER_GRUNT,	
  SFX_FIGHTER_AXE_HITTHING,	
  SFX_FIGHTER_HAMMER_MISS,
  SFX_FIGHTER_HAMMER_HITTHING,
  SFX_FIGHTER_HAMMER_HITWALL,
  SFX_FIGHTER_HAMMER_CONTINUOUS,
  SFX_FIGHTER_HAMMER_EXPLODE,
  SFX_FIGHTER_SWORD_FIRE,
  SFX_FIGHTER_SWORD_EXPLODE,
  SFX_CLERIC_CSTAFF_FIRE,
  SFX_CLERIC_CSTAFF_EXPLODE,
  SFX_CLERIC_CSTAFF_HITTHING,
  SFX_CLERIC_FLAME_FIRE,
  SFX_CLERIC_FLAME_EXPLODE,
  SFX_CLERIC_FLAME_CIRCLE,
  SFX_MAGE_WAND_FIRE,
  SFX_MAGE_LIGHTNING_FIRE,
  SFX_MAGE_LIGHTNING_ZAP,
  SFX_MAGE_LIGHTNING_CONTINUOUS,
  SFX_MAGE_LIGHTNING_READY,
  SFX_MAGE_SHARDS_FIRE,
  SFX_MAGE_SHARDS_EXPLODE,
  SFX_MAGE_STAFF_FIRE,
  SFX_MAGE_STAFF_EXPLODE,
  SFX_SERPENT_SIGHT,
  SFX_SERPENT_ACTIVE,
  SFX_SERPENT_PAIN,
  SFX_SERPENT_ATTACK,
  SFX_SERPENT_MELEEHIT,
  SFX_SERPENT_DEATH,
  SFX_SERPENT_BIRTH,
  SFX_SERPENTFX_CONTINUOUS,
  SFX_SERPENTFX_HIT,
  SFX_POTTERY_EXPLODE,
  SFX_DRIP,
  SFX_CENTAUR_SIGHT,
  SFX_CENTAUR_ACTIVE,
  SFX_CENTAUR_PAIN,
  SFX_CENTAUR_ATTACK,
  SFX_CENTAUR_DEATH,
  SFX_CENTAURLEADER_ATTACK,
  SFX_CENTAUR_MISSILE_EXPLODE,
  SFX_WIND,
  SFX_BISHOP_SIGHT,
  SFX_BISHOP_ACTIVE,
  SFX_BISHOP_PAIN,
  SFX_BISHOP_ATTACK,
  SFX_BISHOP_DEATH,
  SFX_BISHOP_MISSILE_EXPLODE,
  SFX_BISHOP_BLUR,
  SFX_DEMON_SIGHT,
  SFX_DEMON_ACTIVE,
  SFX_DEMON_PAIN,
  SFX_DEMON_ATTACK,
  SFX_DEMON_MISSILE_FIRE,
  SFX_DEMON_MISSILE_EXPLODE,
  SFX_DEMON_DEATH,
  SFX_WRAITH_SIGHT,
  SFX_WRAITH_ACTIVE,
  SFX_WRAITH_PAIN,
  SFX_WRAITH_ATTACK,
  SFX_WRAITH_MISSILE_FIRE,
  SFX_WRAITH_MISSILE_EXPLODE,
  SFX_WRAITH_DEATH,
  SFX_PIG_ACTIVE1,
  SFX_PIG_ACTIVE2,
  SFX_PIG_PAIN,
  SFX_PIG_ATTACK,
  SFX_PIG_DEATH,
  SFX_MAULATOR_SIGHT,
  SFX_MAULATOR_ACTIVE,
  SFX_MAULATOR_PAIN,
  SFX_MAULATOR_HAMMER_SWING,
  SFX_MAULATOR_HAMMER_HIT,
  SFX_MAULATOR_MISSILE_HIT,
  SFX_MAULATOR_DEATH,
  SFX_FREEZE_DEATH,
  SFX_FREEZE_SHATTER,
  SFX_ETTIN_SIGHT,
  SFX_ETTIN_ACTIVE,
  SFX_ETTIN_PAIN,
  SFX_ETTIN_ATTACK,
  SFX_ETTIN_DEATH,
  SFX_FIRED_SPAWN,
  SFX_FIRED_ACTIVE,
  SFX_FIRED_PAIN,
  SFX_FIRED_ATTACK,
  SFX_FIRED_MISSILE_HIT,
  SFX_FIRED_DEATH,
  SFX_ICEGUY_SIGHT,
  SFX_ICEGUY_ACTIVE,
  SFX_ICEGUY_ATTACK,
  SFX_ICEGUY_FX_EXPLODE,
  SFX_SORCERER_SIGHT,
  SFX_SORCERER_ACTIVE,
  SFX_SORCERER_PAIN,
  SFX_SORCERER_SPELLCAST,
  SFX_SORCERER_BALLWOOSH,
  SFX_SORCERER_DEATHSCREAM,
  SFX_SORCERER_BISHOPSPAWN,
  SFX_SORCERER_BALLPOP,
  SFX_SORCERER_BALLBOUNCE,
  SFX_SORCERER_BALLEXPLODE,
  SFX_SORCERER_BIGBALLEXPLODE,
  SFX_SORCERER_HEADSCREAM,
  SFX_DRAGON_SIGHT,
  SFX_DRAGON_ACTIVE,
  SFX_DRAGON_WINGFLAP,
  SFX_DRAGON_ATTACK,
  SFX_DRAGON_PAIN,
  SFX_DRAGON_DEATH,
  SFX_DRAGON_FIREBALL_EXPLODE,
  SFX_KORAX_SIGHT,
  SFX_KORAX_ACTIVE,
  SFX_KORAX_PAIN,
  SFX_KORAX_ATTACK,
  SFX_KORAX_COMMAND,
  SFX_KORAX_DEATH,
  SFX_KORAX_STEP,
  SFX_THRUSTSPIKE_RAISE,
  SFX_THRUSTSPIKE_LOWER,
  SFX_STAINEDGLASS_SHATTER,
  SFX_FLECHETTE_BOUNCE,
  SFX_FLECHETTE_EXPLODE,
  SFX_LAVA_MOVE,
  SFX_WATER_MOVE,
  SFX_ICE_STARTMOVE,
  SFX_EARTH_STARTMOVE,
  SFX_LAVA_SIZZLE,
  SFX_SLUDGE_GLOOP,
  SFX_CHOLY_FIRE,
  SFX_SPIRIT_ACTIVE,
  SFX_SPIRIT_ATTACK,
  SFX_SPIRIT_DIE,
  SFX_VALVE_TURN,
  SFX_ROPE_PULL,
  SFX_FLY_BUZZ,
  SFX_IGNITE,
  SFX_PUZZLE_SUCCESS,
  SFX_PUZZLE_FAIL_FIGHTER,
  SFX_PUZZLE_FAIL_CLERIC,
  SFX_PUZZLE_FAIL_MAGE,
  SFX_BELLRING,
  SFX_TREE_BREAK,
  SFX_TREE_EXPLODE,
  SFX_SUITOFARMOR_BREAK,
  SFX_POISONSHROOM_PAIN,
  SFX_POISONSHROOM_DEATH,
  SFX_AMBIENT1,
  SFX_AMBIENT2,
  SFX_AMBIENT3,
  SFX_AMBIENT4,
  SFX_AMBIENT5,
  SFX_AMBIENT6,
  SFX_AMBIENT7,
  SFX_AMBIENT8,
  SFX_AMBIENT9,
  SFX_AMBIENT10,
  SFX_AMBIENT11,
  SFX_AMBIENT12,
  SFX_AMBIENT13,
  SFX_AMBIENT14,
  SFX_AMBIENT15,
  SFX_STARTUP_TICK,
  SFX_SWITCH_OTHERLEVEL,
  SFX_KORAX_VOICE_1,
  SFX_KORAX_VOICE_2,
  SFX_KORAX_VOICE_3,
  SFX_KORAX_VOICE_4,
  SFX_KORAX_VOICE_5,
  SFX_KORAX_VOICE_6,
  SFX_KORAX_VOICE_7,
  SFX_KORAX_VOICE_8,
  SFX_KORAX_VOICE_9,
  SFX_BAT_SCREAM,
  SFX_MENU_MOVE,
  SFX_CLOCK_TICK,
  SFX_FIREBALL,
  SFX_PUPPYBEAT,
  SFX_MYSTICINCANT,

  NUMSFX
};


#endif
