// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//  $Id$
//
// Copyright (C) 1998-2004 by DooM Legacy Team.
//
// $Log$
// Revision 1.40  2004/10/27 17:37:06  smite-meister
// netcode update
//
// Revision 1.39  2004/09/23 23:21:16  smite-meister
// HUD updated
//
// Revision 1.38  2004/09/13 20:43:29  smite-meister
// interface cleanup, sp map reset fixed
//
// Revision 1.37  2004/09/06 19:58:02  smite-meister
// Doom linedefs done!
//
// Revision 1.36  2004/09/03 17:28:05  smite-meister
// bugfixes
//
// Revision 1.35  2004/09/03 16:28:49  smite-meister
// bugfixes and ZDoom linedef types
//
// Revision 1.34  2004/07/13 20:23:35  smite-meister
// Mod system basics
//
// Revision 1.33  2004/07/07 17:27:19  smite-meister
// bugfixes
//
// Revision 1.32  2004/07/05 16:53:24  smite-meister
// Netcode replaced
//
// Revision 1.31  2004/04/25 16:26:48  smite-meister
// Doxygen
//
// Revision 1.29  2004/01/11 17:19:14  smite-meister
// bugfixes
//
// Revision 1.28  2004/01/06 14:37:45  smite-meister
// six bugfixes, cleanup
//
// Revision 1.27  2004/01/05 11:48:08  smite-meister
// 7 bugfixes
//
// Revision 1.26  2004/01/02 14:25:01  smite-meister
// cleanup
//
// Revision 1.25  2003/12/31 18:32:49  smite-meister
// Last commit of the year? Sound works.
//
// Revision 1.24  2003/12/21 12:29:09  smite-meister
// bugfixes
//
// Revision 1.23  2003/12/13 23:51:03  smite-meister
// Hexen update
//
// Revision 1.22  2003/12/03 10:49:49  smite-meister
// Save/load bugfix, text strings updated
//
// Revision 1.21  2003/11/30 00:09:43  smite-meister
// bugfixes
//
// Revision 1.20  2003/11/23 00:41:55  smite-meister
// bugfixes
//
// Revision 1.19  2003/11/12 11:07:18  smite-meister
// Serialization done. Map progression.
//
// Revision 1.18  2003/06/29 17:33:59  smite-meister
// VFile system, PAK support, Hexen bugfixes
//
// Revision 1.17  2003/06/20 20:56:07  smite-meister
// Presentation system tweaked
//
// Revision 1.16  2003/05/30 13:34:43  smite-meister
// Cleanup, HUD improved, serialization
//
// Revision 1.15  2003/05/11 21:23:49  smite-meister
// Hexen fixes
//
// Revision 1.14  2003/05/05 00:24:48  smite-meister
// Hexen linedef system. Pickups.
//
// Revision 1.13  2003/04/14 08:58:26  smite-meister
// Hexen maps load.
//
// Revision 1.12  2003/04/08 09:46:05  smite-meister
// Bugfixes
//
// Revision 1.11  2003/04/04 00:01:54  smite-meister
// bugfixes, Hexen HUD
//
// Revision 1.10  2003/03/23 14:24:13  smite-meister
// Polyobjects, MD3 models
//
// Revision 1.9  2003/03/15 20:07:14  smite-meister
// Initial Hexen compatibility!
//
// Revision 1.8  2003/03/08 16:07:00  smite-meister
// Lots of stuff. Sprite cache. Movement+friction fix.
//
// Revision 1.7  2003/02/16 16:54:50  smite-meister
// L2 sound cache done
//
// Revision 1.6  2003/01/18 20:17:41  smite-meister
// HUD fixed, levelchange crash fixed.
//
// Revision 1.5  2002/12/29 18:57:03  smite-meister
// MAPINFO implemented, Actor deaths handled better
//
//-----------------------------------------------------------------------------

/// \file
/// \brief Pawn and PlayerPawn class implementations


#include "g_pawn.h"
#include "g_player.h"
#include "g_game.h"
#include "g_map.h"

#include "command.h"
#include "cvars.h"

#include "d_ticcmd.h"
#include "dstrings.h"

#include "p_camera.h" // camera
#include "p_spec.h"

#include "sounds.h"
#include "hardware/hw3sound.h" // ugh.
#include "hud.h"
#include "tables.h" // angle
#include "r_main.h" // PointToAngle functions FIXME which do not belong there
#include "r_sprite.h"
#include "m_random.h"


static int ArmorIncrement[NUMCLASSES][NUMARMOR] =
{
  { 0, 0, 0, 0, 0 },
  { 0, 25, 20, 15, 5 },
  { 0, 10, 25, 5, 20 },
  { 0, 5, 15, 10, 25 },
  { 0, 0, 0, 0, 0 }
};

int MaxArmor[NUMCLASSES] = { 200, 100, 90, 80, 5 };


// lists of mobjtypes that can be played by humans!
pawn_info_t pawndata[] = 
{
  {MT_PLAYER,   wp_pistol,  50, MT_NONE}, // 0
  {MT_POSSESSED, wp_pistol,  20, MT_NONE},
  {MT_SHOTGUY,  wp_shotgun,  8, MT_NONE},
  {MT_TROOP,    wp_none, 0, MT_TROOPSHOT},
  {MT_SERGEANT, wp_none, 0, MT_NONE},
  {MT_SHADOWS,  wp_none, 0, MT_NONE},
  {MT_SKULL,    wp_none, 0, MT_NONE},
  {MT_HEAD,     wp_none, 0, MT_HEADSHOT},
  {MT_BRUISER,  wp_none, 0, MT_BRUISERSHOT},
  {MT_SPIDER,   wp_chaingun, 100, MT_NONE},
  {MT_CYBORG,   wp_missile,  20,  MT_NONE}, //10

  {MT_WOLFSS,   wp_chaingun, 50, MT_NONE},
  {MT_CHAINGUY, wp_chaingun, 50, MT_NONE},
  {MT_KNIGHT,   wp_none, 0,  MT_BRUISERSHOT},
  {MT_BABY,     wp_plasma,  50,  MT_ARACHPLAZ},
  {MT_PAIN,     wp_none, 0,  MT_SKULL},
  {MT_UNDEAD,   wp_none, 0,  MT_TRACER},
  {MT_FATSO,    wp_none, 0,  MT_FATSHOT},
  {MT_VILE,     wp_none, 0,  MT_FIRE}, // 18

  {MT_HPLAYER,  wp_goldwand, 50, MT_NONE},
  {MT_CHICKEN,  wp_beak,      0, MT_NONE},
  {MT_MUMMY,    wp_none, 0, MT_NONE},
  {MT_MUMMYLEADER, wp_none, 0, MT_MUMMYFX1},
  {MT_MUMMYGHOST,  wp_none, 0, MT_NONE},
  {MT_MUMMYLEADERGHOST, wp_none, 0, MT_MUMMYFX1},
  {MT_BEAST,    wp_none, 0, MT_BEASTBALL},
  {MT_SNAKE,    wp_none, 0, MT_SNAKEPRO_A},
  {MT_HHEAD,    wp_none, 0, MT_HEADFX1},
  {MT_CLINK,    wp_none, 0, MT_NONE},
  {MT_WIZARD,   wp_none, 0, MT_WIZFX1},
  {MT_IMP,      wp_none, 0, MT_NONE},
  {MT_IMPLEADER,wp_none, 0, MT_IMPBALL},
  {MT_HKNIGHT,  wp_none, 0, MT_KNIGHTAXE},
  {MT_KNIGHTGHOST, wp_none, 0, MT_REDAXE},
  {MT_SORCERER1, wp_none, 0, MT_SRCRFX1},
  {MT_SORCERER2, wp_none, 0, MT_SOR2FX1},
  {MT_MINOTAUR,  wp_none, 0, MT_MNTRFX1}, // 36

  {MT_PLAYER_FIGHTER, wp_fpunch, 0, MT_NONE},
  {MT_PLAYER_CLERIC, wp_cmace, 0, MT_NONE},
  {MT_PLAYER_MAGE, wp_mwand, 0, MT_NONE},
  {MT_PIGPLAYER, wp_snout, 0, MT_NONE}
};


//=====================================
//  Pawn and PlayerPawn classes
//=====================================

IMPLEMENT_CLASS(Pawn, Actor);
IMPLEMENT_CLASS(PlayerPawn, Pawn);

Pawn::Pawn()
  : Actor()
{
  color = 0;
  maxhealth = 0;
  speed = 0;
  pinfo = NULL;
  attackphase = 0;
  attacker = NULL;
}

PlayerPawn::PlayerPawn()
  : Pawn()
{
  // NOTE! This constructor is only used when Unserializing, so not everything is initialized!
  player = NULL;
  weaponinfo = NULL;
  maxammo = NULL;

  for (int i=0; i<NUMPOWERS; i++)
    powers[i] = 0;

  cheats = refire = 0;
  invTics = morphTics = 0;
}


//=====================================
// Pawn methods
//=====================================

void Pawn::Think() {}

void Pawn::Detach()
{
  attacker = NULL;
  Actor::Detach();
} 

void Pawn::CheckPointers()
{
  if (owner && (owner->eflags & MFE_REMOVE))
    owner = NULL;

  if (target && (target->eflags & MFE_REMOVE))
    target = NULL;

  if (attacker && (attacker->eflags & MFE_REMOVE))
    attacker = NULL;
}


bool Pawn::Damage(Actor *inflictor, Actor *source, int damage, int dtype)
{ return false; }


// creates a pawn based on a Doom/Heretic mobj
Pawn::Pawn(fixed_t x, fixed_t y, fixed_t z, int type)
  : Actor(x, y, z)
{
  if (type < 0)
    {
      if (game.mode == gm_hexen)
	type = 37;
      else if (game.mode == gm_heretic)
	type = 19;
      else
	type = 0;
    }

  pinfo = &pawndata[type];
  const mobjinfo_t *info = &mobjinfo[pinfo->mt];

  mass   = info->mass;
  radius = info->radius;
  height = info->height;
  maxhealth = health = info->spawnhealth;

  speed  = info->speed;

  flags  = info->flags;
  flags2 = info->flags2;
  eflags = 0;

  reactiontime = info->reactiontime;
  attacker = NULL;

  color = 0;
  state_t *state = &states[info->spawnstate];
  pres = new spritepres_t(sprnames[state->sprite], info, 0);
}


//=====================================
// PlayerPawn methods
//=====================================

PlayerPawn::PlayerPawn(fixed_t nx, fixed_t ny, fixed_t nz, int type)
  : Pawn(nx, ny, nz, type)
{
  const float AutoArmorSave[] = { 0.0, 0.15, 0.10, 0.05, 0.0 };
  // TODO fix this kludge when you feel like adding toughness to pawndata array...
  if (type >= 37)
    pclass = type - 36;
  else
    pclass = 0;

  toughness = AutoArmorSave[pclass];

  int i;
  // note! here Map *mp is not yet set! This means you can't call functions such as
  // SetPosition that have something to do with a map.
  flags |= (MF_NOTMONSTER | MF_PICKUP | MF_SHOOTABLE | MF_DROPOFF);
  flags &= ~MF_COUNTKILL;
  flags2 |= (MF2_WINDTHRUST | MF2_SLIDE | MF2_PASSMOBJ | MF2_TELESTOMP);

  player = NULL;

  morphTics = 0;

  invSlot = invTics = 0;
  inventory.resize(2, inventory_t(3,2)); // at least 1 empty slot

  usedown = attackdown = jumpdown = true;  // don't do anything immediately
  refire = 0;

  cheats = 0;
  for (i=0; i<NUMPOWERS; i++)
    powers[i] = 0;

  keycards = 0;
  backpack = false;

  weaponinfo = wpnlev1info;
  maxammo = maxammo1;

  for (i=0; i<NUMAMMO; i++)
    ammo[i] = 0;

  for (i=0; i<NUMWEAPONS; i++)
    weaponowned[i] = false;

  if (game.mode == gm_heretic)
    weaponowned[wp_staff] = true;
  else if (game.mode == gm_hexen)
    {}
  else
    weaponowned[wp_fist] = true;

  weapontype_t w = pinfo->bweapon;
  readyweapon = pendingweapon = w;

  if (w != wp_none)
    {
      weaponowned[w] = true;
      ammotype_t a = wpnlev1info[w].ammo;
      if (a != am_noammo)
	ammo[a] = pinfo->bammo;
    }

  // armor
  toughness = 0;
  for (i = 0; i < NUMARMOR; i++)
    {
      armorpoints[i] = 0;
      armorfactor[i] = 0;
    }

  specialsector = 0;
  extralight = fixedcolormap = 0;

  fly_zspeed = 0;
}


//--------------------------------------------------------

#define BLINKTHRESHOLD  (4*32) // for powers

// Index of the special effects (INVUL inverse) map.
#define INVERSECOLORMAP  32

void PlayerPawn::Think()
{
  ticcmd_t* cmd = &player->cmd;

  // a corpse, for example. Thinks just like an actor.
  if (!player)
    goto actor_think;

  // inventory
  if (invTics)
    invTics--;

  // Turn on required cheats
  if (cheats & CF_NOCLIP)
    flags |= (MF_NOCLIPLINE|MF_NOCLIPTHING);
  else
    flags &= ~(MF_NOCLIPLINE|MF_NOCLIPTHING);

  // chain saw run forward
  if (eflags & MFE_JUSTATTACKED)
    {
      cmd->forward = 100;
      cmd->side = 0;
      eflags &= ~MFE_JUSTATTACKED;
    }

  if (player->playerstate == PST_DEAD)
    {
      DeathThink();
      goto actor_think;
    }

  // morphed state?
  if (morphTics)
    MorphThink();

  // CheckWater is also called at Actor::Think later...
  CheckWater();

  // Move around.
  // Reactiontime is used to prevent movement
  //  for a bit after a teleport.
  if (reactiontime > 0)
    reactiontime--;
  else
    Move();

  // bob the view
  player->CalcViewHeight(z <= floorz);

  // check special sectors : damage & secrets
  PlayerInSpecialSector();

  // water splashes
  if (specialsector >= 887 && specialsector <= 888)
    {
      if ((px > (2*FRACUNIT) || px < (-2*FRACUNIT) ||
	   py > (2*FRACUNIT) || py < (-2*FRACUNIT) ||
	   pz >  (2*FRACUNIT)) &&  // jump out of water
	  !(game.tic % (32 * NEWTICRATERATIO)))
        {
	  //
	  // make sure we disturb the surface of water (we touch it)
	  //
	  int waterz;
	  if (specialsector == 887)
	    //FLAT TEXTURE 'FWATER'
	    waterz = subsector->sector->floorheight + (FRACUNIT/4);
	  else
	    //FIXME: no hacks! faB's current water hack using negative sector tags
	    waterz = - (subsector->sector->tag << FRACBITS);

	  // half in the water
	  if (eflags & MFE_TOUCHWATER)
            {
	      if (z <= floorz) // onground
                {
		  fixed_t whater_height = waterz-subsector->sector->floorheight;

		  if (whater_height < (height >> 2))
		    S_StartSound(this, sfx_enterwater);
		  else
		    S_StartSound(this, sfx_exitwater);
                }
	      else
		S_StartSound(this, sfx_exitwater);
            }                   
        }
    }

  int w;

  // Check for weapon change.
  if ((w = (cmd->buttons & ticcmd_t::WEAPONMASK)))
    {
      // The actual changing of the weapon is done
      //  when the weapon psprite can do it
      //  (read: not in the middle of an attack).

      w = (w >> ticcmd_t::WEAPONSHIFT) - 1;
      CONS_Printf("wp change, %d, %d\n", w, cmd->buttons);
      if (w < NUMWEAPONS && weaponowned[w])
	pendingweapon = weapontype_t(w);

      CONS_Printf("pend %d\n", pendingweapon);

      // Do not go to plasma or BFG in shareware, even if cheated.
      if ((game.mode == gm_doom1s) &&
	  (pendingweapon == wp_plasma || pendingweapon == wp_bfg))
	pendingweapon = wp_none;
    }

  // check for use
  if (cmd->buttons & ticcmd_t::BT_USE)
    {
      if (!usedown)
        {
	  UseLines();
	  usedown = true;
        }
    }
  else
    usedown = false;

  // artifacts
  if (cmd->item)
    UseArtifact(artitype_t(cmd->item - 1));

  // morph counter
  if (morphTics)
    {
      // Chicken attack counter
      if (attackphase)
	attackphase -= 3;
      // Attempt to undo the chicken
      if (--morphTics == 0)
	UndoMorph();
    }

  // cycle psprites
  MovePsprites();

  // Counters, time dependend power ups.
  // Strength counts up to diminish fade.
  if (powers[pw_strength])
    powers[pw_strength]++;

  if (powers[pw_invulnerability])
    powers[pw_invulnerability]--;

  // the MF_SHADOW activates the tr_transhi translucency while it is set
  // (it doesnt use a preset value through FF_TRANSMASK)
  if (powers[pw_invisibility])
    if (--powers[pw_invisibility] == 0)
      flags &= ~MF_SHADOW;

  if (powers[pw_infrared])
    powers[pw_infrared]--;

  if (powers[pw_ironfeet])
    powers[pw_ironfeet]--;

  if (powers[pw_flight])
    {
      if(--powers[pw_flight] == 0)
	{
	  eflags &= ~MFE_FLY;
	  flags &= ~MF_NOGRAVITY;
	}
    }

  if(powers[pw_weaponlevel2])
    {
      if(--powers[pw_weaponlevel2] == 0)
	{
	  weaponinfo = wpnlev1info;
	  // end of weaponlevel2 power
	  if (readyweapon == wp_phoenixrod
	      && (psprites[ps_weapon].state != &weaponstates[S_PHOENIXREADY])
	      && (psprites[ps_weapon].state != &weaponstates[S_PHOENIXUP]))
	    {
	      SetPsprite(ps_weapon, S_PHOENIXREADY);
	      ammo[am_phoenixrod] -= USE_PHRD_AMMO_2;
	      refire = 0;
	    }
	  else if ((readyweapon == wp_gauntlets) || (readyweapon == wp_staff))
	    pendingweapon = readyweapon;
	}
    }

  // damage and bonus counts are now HUD properties and are ticked there

  // Handling colormaps.
  if (powers[pw_invulnerability])
    {
      if (powers[pw_invulnerability] > BLINKTHRESHOLD
	  || (powers[pw_invulnerability] & 8))
	fixedcolormap = INVERSECOLORMAP;
      else
	fixedcolormap = 0;
    }
  else if (powers[pw_infrared])
    {
      if (powers[pw_infrared] > BLINKTHRESHOLD
	  || (powers[pw_infrared] & 8))
        {
	  // almost full bright
	  fixedcolormap = 1;
        }
      else
	fixedcolormap = 0;
    }
  else
    fixedcolormap = 0;

 actor_think:
  // this is where the "actor part" of the thinking begins
  // we call Actor::Think(), because a playerpawn is an actor too
  Actor::Think();
}


//--------------------------------------------------------
// Fall on your face when dying.
// Decrease POV height to floor height.

#define ANG5    (ANG90/18)

void PlayerPawn::DeathThink()
{
  MovePsprites();

  // fall to the ground
  if (player->viewheight > 6*FRACUNIT)
    player->viewheight -= FRACUNIT;

  if (player->viewheight < 6*FRACUNIT)
    player->viewheight = 6*FRACUNIT;

  player->deltaviewheight = 0;

  player->CalcViewHeight(z <= floorz);

  // watch my killer (if there is one)
  if (attacker != NULL && attacker != this)
    {
      angle_t ang = R_PointToAngle2(x, y, attacker->x, attacker->y);
      angle_t delta = ang - angle;

      if (delta < ANG5 || delta > (unsigned)-ANG5)
        {
	  // Looking at killer,
	  //  so fade damage flash down.
	  angle = ang;
        }
      else if (delta < ANG180)
	angle += ANG5;
      else
	angle -= ANG5;

      // change aiming to look up or down at the attacker (DOESNT WORK)
      // FIXME : the aiming returned seems to be too up or down...
      //fixed_t dist = P_AproxDistance(attacker->x - x, attacker->y - y);
      //if (dist)
      //  pitch = FixedMul ((160<<FRACBITS), FixedDiv (attacker->z + (attacker->height>>1), dist)) >>FRACBITS;
      //else pitch = 0;
      aiming = (attacker->z - z) >> 17;
    }

  if (player->cmd.buttons & ticcmd_t::BT_USE)
    mp->RebornPlayer(player);
}



//----------------------------------------------

void PlayerPawn::MorphThink()
{
  if (game.mode == gm_heretic)
    {
      if (health > 0)
	// Handle beak movement
	psprites[ps_weapon].sy = WEAPONTOP + (attackphase << (FRACBITS-1));

      if (morphTics & 15)
	return;

      if (!(px+py) && P_Random() < 160)
	{ // Twitch view angle
	  angle += P_SignedRandom() << 19;
	}
      if ((z <= floorz) && (P_Random() < 32))
	{ // Jump and noise
	  pz += FRACUNIT;
	  pres->SetAnim(presentation_t::Pain);
	  return;
	}
      if(P_Random() < 48)
	{ // Just noise
	  S_StartScreamSound(this, sfx_chicact);
	}
    }
  else if (game.mode == gm_hexen)
    {
      if (morphTics & 15)
	return;

      if (!(px+py) && P_Random() < 64)
	{ // Snout sniff
	  SetPsprite(ps_weapon, S_SNOUTATK2, false);
	  S_StartSound(this, SFX_PIG_ACTIVE1); // snort
	  return;
	}
      if (P_Random() < 48)
	{
	  if(P_Random() < 128)
	    S_StartSound(this, SFX_PIG_ACTIVE1);
	  else
	    S_StartSound(this, SFX_PIG_ACTIVE2);
	}
    }
}

//-----------------------------------------

void PlayerPawn::XYMovement()
{
  fixed_t oldx, oldy;
  oldx = x;
  oldy = y;

  Actor::XYMovement();

  // slow down
  if (cheats & CF_NOMOMENTUM)
    {
      // debug option for no sliding at all
      px = py = 0;
      return;
    }
  else if ((cheats & CF_FLYAROUND) || (eflags & MFE_FLY))
    {
      //XYFriction(oldx, oldy, true);
      return;
    }
  //        if (z <= subsector->sector->floorheight)
  //          XYFriction(oldx, oldy, false);

}

void PlayerPawn::ZMovement()
{
  // check for smooth step up
  if (player && (z < floorz))
    {
      player->viewheight -= floorz - z;

      player->deltaviewheight
	= ((cv_viewheight.value<<FRACBITS) - player->viewheight)>>3;
    }

  fixed_t oldz, oldpz;
  oldz = z;
  oldpz = pz;

  Actor::ZMovement();

  if (oldz + oldpz <= floorz && (oldpz < 0)) // falling
    {
      if ((oldpz < -8*FRACUNIT) && !(eflags & MFE_FLY))
	{
	  // Squat down.
	  // Decrease viewheight for a moment
	  // after hitting the ground (hard),
	  // and utter appropriate sound.
	  player->deltaviewheight = oldpz>>3;
	  S_StartSound(this, sfx_grunt);
	}
    }

  if (oldz+oldpz + height > ceilingz)
    {
      // player avatar hits his head on the ceiling, ouch!
      if (!(cheats & CF_FLYAROUND) && !(eflags & MFE_FLY) && pz > 8*FRACUNIT)
	S_StartSound(this, sfx_grunt);
    }
}

#define STOPSPEED               (0x1000/NEWTICRATERATIO)

void PlayerPawn::XYFriction(fixed_t oldx, fixed_t oldy, bool oldfriction)
{
  if (player == NULL)
    return;

  if (px > -STOPSPEED && px < STOPSPEED && py > -STOPSPEED && py < STOPSPEED && 
      (player->cmd.forward == 0 && player->cmd.side == 0 ))
    {
      // if in a walking frame, stop moving
      int anim = pres->GetAnim();
      if (anim == presentation_t::Run)
	pres->SetAnim(presentation_t::Idle);
    
      px = 0;
      py = 0;
    }
  else Actor::XYFriction(oldx, oldy);
}




//  Called when a player exits a map.
//  Throws away extra items, removes powers, keys, curses
void PlayerPawn::Reset()
{
  int i, n = inventory.size();
  for (i=0; i<n; i++)
    if (inventory[i].count > 1) 
      inventory[i].count = 1; // why? not fair

  /*
  if (!cv_deathmatch.value)
    for(i = 0; i < MAXARTECONT; i++)
      UseArtifact(arti_fly);
  */

  keycards = 0;
  memset(powers, 0, sizeof (powers));
  weaponinfo = wpnlev1info;    // cancel power weapons
  flags &= ~MF_SHADOW;         // cancel invisibility
  extralight = 0;              // cancel gun flashes
  fixedcolormap = 0;           // cancel ir goggles

  if (morphTics)
    {
      readyweapon = weapontype_t(attackphase); // Restore weapon
      morphTics = 0;
    }
}


// returns the next owned weapon in group g
weapontype_t PlayerPawn::FindWeapon(int g)
{
  if (readyweapon != wp_none && g == weapondata[readyweapon].group)
    {
      weapontype_t n = weapondata[readyweapon].next;
      while (n != readyweapon)
	{
	  if (n == wp_none || weaponowned[n])
	    return n;

	  n = weapondata[n].next;
	}
    }
  else
    for (int i=0; i<NUMWEAPONS; i++)
      if (g == weapondata[i].group && weaponowned[i])
	return weapontype_t(i);

  return wp_none;
}



//
// Tries to aim at a nearby monster
//
DActor *PlayerPawn::SPMAngle(mobjtype_t type, angle_t ang)
{
  extern Actor *linetarget;

  fixed_t  slope = 0;

  if (player->autoaim && cv_allowautoaim.value)
    {
      // see which target is to be aimed at
      slope = AimLineAttack(ang, 16*64*FRACUNIT);

      if (!linetarget)
        {
	  ang += 1<<26;
	  slope = AimLineAttack(ang, 16*64*FRACUNIT);

	  if (!linetarget)
            {
	      ang -= 2<<26;
	      slope = AimLineAttack(ang, 16*64*FRACUNIT);
            }

	  if (!linetarget)
	    slope = AIMINGTOSLOPE(aiming);
        }
    }
  else
    slope = AIMINGTOSLOPE(aiming);

  // if not autoaim, or if the autoaim didnt aim something, use the mouseaiming    

  fixed_t mz = z + 32*FRACUNIT - floorclip;

  DActor *th = mp->SpawnDActor(x, y, mz, type);

  if (th->info->seesound)
    S_StartSound(th, th->info->seesound);

  th->owner = this;

  th->angle = ang;
  th->px = int(th->info->speed * finecosine[ang>>ANGLETOFINESHIFT]);
  th->py = int(th->info->speed * finesine[ang>>ANGLETOFINESHIFT]);
    
  th->px = FixedMul(th->px, finecosine[aiming>>ANGLETOFINESHIFT]);
  th->py = FixedMul(th->py, finecosine[aiming>>ANGLETOFINESHIFT]);

  th->pz = int(th->info->speed * slope);

  return (th->CheckMissileSpawn()) ? th : NULL;
}




// checks if the player has the correct key for 'lock'
bool P_CheckKeys(Actor *mo, int lock)
{
  PlayerPawn *p = mo->IsOf(PlayerPawn::_type) ? (PlayerPawn *)mo : NULL;

  if (!p)
    return false;

  if (!lock)
    return true;

  if (lock > NUMKEYS)
    return false;

  if (!(p->keycards & (1 << (lock-1))))
    {
      static char buf[80];
      if (lock <= 11)
	{
	  sprintf(buf, "You need the %s\n",  text[TXT_KEY_STEEL + lock - 1]);
	  p->player->SetMessage(buf);
	  S_StartSound(mo, SFX_DOOR_LOCKED);
	}
      else
	{
	  // skulls and cards are equivalent
	  if (p->keycards & (1 << (lock+2)))
	    return true;

	  p->player->SetMessage(text[TXT_PD_BLUEK + lock - 12]);
	  S_StartScreamSound(p, sfx_usefail);
	}

      return false; // no ticket!
    }
  return true;
}


// Passed a generalized locked door linedef and a player, returns whether
// the player has the keys necessary to unlock that door.
//
// Note: The linedef passed MUST be a generalized locked door type
//       or results are undefined.
//
bool PlayerPawn::CanUnlockGenDoor(line_t *line)
{
  // does this line special distinguish between skulls and keys?
  bool skulliscard = (line->special & LockedNKeys)>>LockedNKeysShift;
  bool ok = true;

  // determine for each case of lock type if player's keys are adequate
  switch ((line->special & LockedKey)>>LockedKeyShift)
    {
    case AnyKey_:
      if (!(keycards & it_redcard) &&
	  !(keycards & it_redskull) &&
	  !(keycards & it_bluecard) &&
	  !(keycards & it_blueskull) &&
	  !(keycards & it_yellowcard) &&
	  !(keycards & it_yellowskull))
	{
	  player->SetMessage(PD_ANY);
	  ok = false;
	}
      break;
    case RCard:
      if
	(
	 !(keycards & it_redcard) &&
	 (!skulliscard || !(keycards & it_redskull))
	 )
	{
	  player->SetMessage(skulliscard? PD_REDK : PD_REDC);
	  ok = false;
	}
      break;
    case BCard:
      if
	(
	 !(keycards & it_bluecard) &&
	 (!skulliscard || !(keycards & it_blueskull))
	 )
	{
	  player->SetMessage(skulliscard? PD_BLUEK : PD_BLUEC);
	  ok = false;
	}
      break;
    case YCard:
      if
	(
	 !(keycards & it_yellowcard) &&
	 (!skulliscard || !(keycards & it_yellowskull))
	 )
	{
	  player->SetMessage(skulliscard? PD_YELLOWK : PD_YELLOWC);
	  ok = false;
	}
      break;
    case RSkull:
      if
	(
	 !(keycards & it_redskull) &&
	 (!skulliscard || !(keycards & it_redcard))
	 )
	{
	  player->SetMessage(skulliscard? PD_REDK : PD_REDS);
	  ok = false;
	}
      break;
    case BSkull:
      if
	(
	 !(keycards & it_blueskull) &&
	 (!skulliscard || !(keycards & it_bluecard))
	 )
	{
	  player->SetMessage(skulliscard? PD_BLUEK : PD_BLUES);
	  ok = false;
	}
      break;
    case YSkull:
      if
	(
	 !(keycards & it_yellowskull) &&
	 (!skulliscard || !(keycards & it_yellowcard))
	 )
	{
	  player->SetMessage(skulliscard? PD_YELLOWK : PD_YELLOWS);
	  ok = false;
	}
      break;
    case AllKeys:
      if
	(
	 !skulliscard &&
	 (
          !(keycards & it_redcard) ||
          !(keycards & it_redskull) ||
          !(keycards & it_bluecard) ||
          !(keycards & it_blueskull) ||
          !(keycards & it_yellowcard) ||
          !(keycards & it_yellowskull)
	  )
	 )
	{
	  player->SetMessage(PD_ALL6);
	  ok = false;
	}
      if
	(
	 skulliscard &&
	 (
          (!(keycards & it_redcard) &&
	   !(keycards & it_redskull)) ||
          (!(keycards & it_bluecard) &&
	   !(keycards & it_blueskull)) ||
          (!(keycards & it_yellowcard) &&
	   !(keycards & it_yellowskull))
	  )
	 )
	{
	  player->SetMessage(PD_ALL3);
	  ok = false;
	}
      break;
    }
  if (ok == false) S_StartScreamSound(this, sfx_usefail);

  return ok;
}



// Function that actually applies the sector special to the player.
void PlayerPawn::ProcessSpecialSector(sector_t *sector, bool instantdamage)
{
  if (instantdamage && sector->damage)
    {
      if ((sector->special & SS_SPECIALMASK) == SS_EndLevelHurt)
	{
          // EXIT SUPER DAMAGE! (for E1M8 finale)
	  cheats &= ~CF_GODMODE;
	  Damage(NULL, NULL, 20);
          if (health <= 10)
	    {
	      mp->ExitMap(this, 0);
	      return;
	    }
	}
      else if (sector->damage < 20 && !powers[pw_ironfeet])
	Damage(NULL, NULL, sector->damage, sector->damagetype);
      else if (!powers[pw_ironfeet] || P_Random() < 5)
	Damage(NULL, NULL, sector->damage, sector->damagetype);
      
      mp->SpawnSmoke(x, y, z);
    }

  if (sector->special & SS_secret)
    {
      // SECRET SECTOR
      player->secrets++;
      player->SetMessage("\3You found a secret area!\n");
      sector->special &= ~SS_secret;
    }
}


// Checks to see if a player is standing on or is inside a 3D floor (water)
// and applies any speicials..
void PlayerPawn::PlayerOnSpecial3DFloor()
{
  sector_t* sec;
  bool   instantdamage = false;
  ffloor_t* rover;

  sec = subsector->sector;
  if(!sec->ffloors)
    return;

  for (rover = sec->ffloors; rover; rover = rover->next)
    {
      if (!rover->master->frontsector->special)
	continue;

      // Check the 3D floor's type...
      if (rover->flags & FF_SOLID)
	{
	  // Player must be on top of the floor to be affected...
	  if (z != *rover->topheight)
	    continue;

	  if ((eflags & MFE_JUSTHITFLOOR) &&
	      sec->heightsec == -1 && (mp->maptic % (2*NEWTICRATERATIO))) //SoM: penalize jumping less.
	    instantdamage = true;
	  else
	    instantdamage = !(mp->maptic % (32*NEWTICRATERATIO));
	}
      else
	{
	  //Water and DEATH FOG!!! heh
	  if(z > *rover->topheight || (z + height) < *rover->bottomheight)
	    continue;
	  instantdamage = !(mp->maptic % (32*NEWTICRATERATIO));
	}

      ProcessSpecialSector(rover->master->frontsector, instantdamage);
    }
}


//
// Called every tic frame
//  that the player origin is in a special sector
//
void PlayerPawn::PlayerInSpecialSector()
{
  // SoM: Check 3D floors...
  PlayerOnSpecial3DFloor();

  sector_t *sec = subsector->sector;

  //Fab: keep track of what sector type the player's currently in
  specialsector = sec->special;

#ifdef OLDWATER
  //Fab: VERY NASTY hack for water QUICK TEST !!!!!!!!!!!!!!!!!!!!!!!
  if (sec->tag < 0)
    {
      specialsector = 888;    // no particular value
      return;
    }
  else if (levelflats[sec->floorpic].iswater)
    // old water (flat texture)
    {
      specialsector = 887;
      return;
    }
#endif

  if (!specialsector)     // nothing special, exit
    return;

  // Falling, not all the way down yet?
  //SoM: 3/17/2000: Damage if in slimey water!
  if (sec->heightsec != -1)
    {
      if(z > mp->sectors[sec->heightsec].floorheight)
	return;
    }
  else if (z != sec->floorheight)
    return;

  int temp = specialsector & SS_DAMAGEMASK;
  bool instantdamage = false;

  if (temp == SS_damage_32)
    instantdamage = !(mp->maptic & 31);
  else if (temp == SS_damage_16)
    instantdamage = !(mp->maptic & 15);
    
  //Fab: jumping in lava/slime does instant damage (no jump cheat)
  if ((eflags & MFE_JUSTHITFLOOR) && (sec->heightsec == -1) && (mp->maptic & 2)) //SoM: penalize jumping less.
    instantdamage = true;

  ProcessSpecialSector(sec, instantdamage);
}





//---------------------------------------------------------------------------
//
// FUNC P_AutoUseChaosDevice
//
//---------------------------------------------------------------------------

bool P_AutoUseChaosDevice(PlayerPawn *p)
{
  int i, n = p->inventory.size();
    
  for (i = 0; i < n; i++)
    {
      if (p->inventory[i].type == arti_teleport)
        {
	  p->UseArtifact(arti_teleport);
	  p->health = (p->health + 1) / 2;
	  return true;
        }
    }
  return false;
}

//---------------------------------------------------------------------------
//
// PROC P_AutoUseHealth
//
//---------------------------------------------------------------------------

void P_AutoUseHealth(PlayerPawn *p, int saveHealth)
{
  int i, n = p->inventory.size();
  int count;
  int normalCount;
  int normalSlot;
  int superCount;
  int superSlot;
    
  normalCount = superCount = 0;
  for(i = 0; i < n; i++)
    {
      if (p->inventory[i].type == arti_health)
        {
	  normalSlot = i;
	  normalCount = p->inventory[i].count;
        }
      else if (p->inventory[i].type == arti_superhealth)
        {
	  superSlot = i;
	  superCount = p->inventory[i].count;
        }
    }
  if((game.skill == sk_baby) && (normalCount*25 >= saveHealth))
    { // Use quartz flasks
      count = (saveHealth+24)/25;
      for(i = 0; i < count; i++)
	p->UseArtifact(arti_health);
    }
  else if(superCount*100 >= saveHealth)
    { // Use mystic urns
      count = (saveHealth+99)/100;
      for(i = 0; i < count; i++)
	p->UseArtifact(arti_superhealth);
    }
  else if((game.skill == sk_baby) && (superCount*100+normalCount*25 >= saveHealth))
    { // Use mystic urns and quartz flasks
      count = (saveHealth+24)/25;
      for(i = 0; i < count; i++)
	p->UseArtifact(arti_health);

      saveHealth -= count*25;
      count = (saveHealth+99)/100;
      for(i = 0; i < count; i++)
	p->UseArtifact(arti_superhealth);
    }
}



//---------------------------------------------
// "inflictor" is the thing that caused the damage
//  creature or missile, can be NULL (slime, etc)
// "source" is the thing to target after taking damage
//  creature or NULL
// Source and inflictor are the same for melee attacks.
// Source can be NULL for slime, barrel explosions
// and other environmental stuff.

bool PlayerPawn::Damage(Actor *inflictor, Actor *source, int damage, int dtype)
{
  if (dtype & dt_always)
    {
      // unavoidable damage
      // pain flash
      player->damagecount += damage;
      return Actor::Damage(inflictor, source, damage, dtype);
    }

  if (game.skill == sk_baby)
    damage >>= 1;   // take half damage in trainer mode
  
  if (inflictor && inflictor->IsOf(DActor::_type))
    {
      DActor *d = (DActor *)inflictor;
      switch (d->type)
	{
	case MT_MACEFX4: // Death ball
	  if (powers[pw_invulnerability])
	    // Can't hurt invulnerable players
	    damage = 0;
	    break;	  
	  if (P_AutoUseChaosDevice(this))
	    // Player was saved using chaos device
	    return false;	
	  damage = 10000; // Something's gonna die
	  break;
        case MT_PHOENIXFX2: // Flame thrower
	  if (P_Random() < 128)
            { // Freeze player for a bit
	      reactiontime += 4;
            }
	  break;
	default:
	  break;
	}
    }

  int i, temp;
  // player specific
  if (!(flags & MF_CORPSE))
    {
      // end of game hellslime hack
      if (subsector->sector->special == 11 && damage >= health)
	damage = health - 1;

      // ignore damage in GOD mode, or with INVUL power.
      if ((cheats & CF_GODMODE) || powers[pw_invulnerability])
	return false;

      // doom armor
      temp = armorpoints[armor_field];
      if (temp > 0)
        {
	  int saved = int(damage * armorfactor[armor_field]);

	  if (temp <= saved)
            {
	      // armor is used up
	      saved = temp;
	      armorfactor[armor_field] = 0;
            }
	  armorpoints[armor_field] -= saved;
	  damage -= saved;
        }

      // hexen armor
      float save = toughness;
      for (i = armor_armor; i < NUMARMOR; i++)
	save += float(armorpoints[i])/100;
      if (save > 0)
	{
	  // armor absorbed some damage
	  if (save > 1)
	    save = 1;

	  // armor deteriorates
	  for (i = armor_armor; i < NUMARMOR; i++)
	    if (armorpoints[i])
	      {
		armorpoints[i] -= int(damage * ArmorIncrement[pclass][i] / (100 * armorfactor[i]));
		if (armorpoints[i] <= 2)
		  armorpoints[i] = 0;
	      }

	  int saved = int(damage * save);
	  if (damage > 200)
	    saved = int(200 * save);
	  damage -= saved;
	}      

      PlayerPawn *s = NULL;
      if (source && source->IsOf(PlayerPawn::_type))
	s = (PlayerPawn *)source;

      // added team play and teamdamage (view logboris at 13-8-98 to understand)
      if (s && (s->player->team == player->team) && !cv_teamdamage.value && (s != this))
	return false;

      // autosavers
      if (damage >= health && (game.skill == sk_baby) && !morphTics)
	{ // Try to use some inventory health
	  P_AutoUseHealth(this, damage-health+1);
	}

      // pain flash
      player->damagecount += damage;

      pres->SetAnim(presentation_t::Pain);
    }

  attacker = source;

  bool ret = Actor::Damage(inflictor, source, damage, dtype);

  return ret;
}



//===========================================================
//   Giving playerpawns stuff
//===========================================================

extern int  p_sound;  // pickupsound
extern bool p_remove; // should the stuff be removed?

// Returns false if the health isn't needed at all
bool Pawn::GiveBody(int num)
{
  if (health >= maxhealth)
    return false;

  health += num;
  if (health > maxhealth)
    health = maxhealth;

  return true;
}


bool PlayerPawn::GivePower(int power)
{
  switch (power)
    {
    case pw_invulnerability:
      // Already have it?
      if (powers[power] > BLINKTHRESHOLD)
	return false;
      powers[power] = INVULNTICS;
      flags2 |= MF2_INVULNERABLE;
      if (pclass == 3) flags2 |= MF2_REFLECTIVE; // Hexen mage
      break;

    case pw_weaponlevel2:
      if (powers[power] > BLINKTHRESHOLD)
	return false;
      powers[power] = WPNLEV2TICS;
      weaponinfo = wpnlev2info;
      break;
  
    case pw_invisibility:
      if (powers[power] > BLINKTHRESHOLD)
	return false;
      powers[power] = INVISTICS;
      flags |= MF_SHADOW;
      break;

    case pw_flight:
      if (powers[power] > BLINKTHRESHOLD)
	return false;
      powers[power] = FLIGHTTICS;
      eflags |= MFE_FLY;
      flags |= MF_NOGRAVITY;
      if (z <= floorz)
	fly_zspeed = 10; // thrust the player in the air a bit
      break;

    case pw_infrared:
      if (powers[power] > BLINKTHRESHOLD)
	return false;
      powers[power] = INFRATICS;
      break;

    case pw_ironfeet:
      powers[power] = IRONTICS;
      break;

    case pw_strength:
      GiveBody(100);
      powers[power] = 1;
      break;

    case pw_speed:
      if (powers[power] > BLINKTHRESHOLD)
	return false;
      powers[power] = SPEEDTICS;
      break;

    case pw_minotaur:
      powers[power] = MAULATORTICS;
      break;

    default:
      if (powers[power] != 0)
	return false;
      powers[power] = 1;
    }

  p_sound = sfx_powerup;

  return true;
}


// Num is the number of clip loads,
// not the individual count (0= 1/2 clip).
// Returns false if the ammo can't be picked up at all

bool PlayerPawn::GiveAmmo(ammotype_t at, int count)
{
  static const weapontype_t GetAmmoChange[] =
  {
    wp_chaingun, wp_shotgun, wp_plasma, wp_missile,
    wp_goldwand,
    wp_crossbow,
    wp_blaster,
    wp_skullrod,
    wp_phoenixrod,
    wp_mace
  };

  if (at == am_noammo)
    return false;

  if (at == am_manaboth)
    {
      bool ret = GiveAmmo(am_mana1, count) || GiveAmmo(am_mana2, count);
      return ret;
    }

  if (at < 0 || at >= NUMAMMO)
    {
      CONS_Printf ("\2P_GiveAmmo: bad type %i", at);
      return false;
    }

  if (ammo[at] >= maxammo[at])
    return false;

  if (game.skill == sk_baby || game.skill == sk_nightmare)
    {
      if (game.mode == gm_heretic || game.mode == gm_hexen)
	count += count>>1;
      else
	// give double ammo in trainer mode,
	// you'll need it in nightmare
	count <<= 1;
    }
  int oldammo = ammo[at];
  ammo[at] += count;

  if (ammo[at] > maxammo[at])
    ammo[at] = maxammo[at];

  // If non zero ammo,
  // don't change up weapons,
  // player was lower on purpose.
  if (oldammo)
    return true;

  // We were down to zero,
  // so select a new weapon.
  // Preferences are not user selectable.

  // Boris hack for preferred weapons order...
  if (!player->originalweaponswitch)
    {
      if (ammo[weaponinfo[readyweapon].ammo]
	  < weaponinfo[readyweapon].ammopershoot)
	UseFavoriteWeapon();
      return true;
    }
  else if (game.mode == gm_heretic)
    {
      if ((readyweapon == wp_staff || readyweapon == wp_gauntlets) 
	  && weaponowned[GetAmmoChange[at]])
	pendingweapon = GetAmmoChange[at];
    }
  else switch (at)
    {
    case am_clip:
      if (readyweapon == wp_fist)
        {
	  if (weaponowned[wp_chaingun])
	    pendingweapon = wp_chaingun;
	  else
	    pendingweapon = wp_pistol;
        }
      break;

    case am_shell:
      if (readyweapon == wp_fist
	  || readyweapon == wp_pistol)
        {
	  if (weaponowned[wp_shotgun])
	    pendingweapon = wp_shotgun;
        }
      break;

    case am_cell:
      if (readyweapon == wp_fist
	  || readyweapon == wp_pistol)
        {
	  if (weaponowned[wp_plasma])
	    pendingweapon = wp_plasma;
        }
      break;

    case am_misl:
      if (readyweapon == wp_fist)
        {
	  if (weaponowned[wp_missile])
	    pendingweapon = wp_missile;
        }
    default:
      break;
    }

  return true;
}


bool PlayerPawn::GiveWeapon(weapontype_t wt, bool dropped)
{
  bool     gaveammo;
  bool     gaveweapon;

  if (game.multiplayer && (cv_deathmatch.value != 2) && !dropped)
    {
      // leave placed weapons forever on net games
      if (weaponowned[wt])
	return false;

      weaponowned[wt] = true;

      if (cv_deathmatch.value)
	GiveAmmo(weaponinfo[wt].ammo, 5*clipammo[weaponinfo[wt].ammo]);
      else
	GiveAmmo(weaponinfo[wt].ammo, weapondata[wt].getammo);

      // Boris hack preferred weapons order...
      if (player->originalweaponswitch
	  || player->weaponpref[wt] > player->weaponpref[readyweapon])
	pendingweapon = wt;     // do like Doom2 original

      S_StartAmbSound(player, sfx_weaponup);
      return false;
    }

  if (weaponinfo[wt].ammo != am_noammo)
    {
      // give one clip with a dropped weapon,
      // two clips with a found weapon
      if (dropped)
	gaveammo = GiveAmmo(weaponinfo[wt].ammo, clipammo[weaponinfo[wt].ammo]);
      else
	gaveammo = GiveAmmo(weaponinfo[wt].ammo, weapondata[wt].getammo);
    }
  else
    gaveammo = false;

  if (weaponowned[wt])
    gaveweapon = false;
  else
    {
      gaveweapon = true;
      weaponowned[wt] = true;
      if (player->originalweaponswitch
	  || player->weaponpref[wt] > player->weaponpref[readyweapon])
	pendingweapon = wt;    // Doom2 original stuff
    }

  p_sound = sfx_weaponup;
  return (gaveweapon || gaveammo);
}


// Returns false if the armor is worse
// than the current armor.
bool PlayerPawn::GiveArmor(armortype_t type, float factor, int points)
{
  // Kludgy mess. The correct way would be making each pickup-item a separate class
  // with a Give method... same thing with weapons and artifacts
  if (factor > 0)
    {
      // new piece of armor
      if (points < 0) // means use standard Hexen armor increments
	points = ArmorIncrement[pclass][type];

      if (armorpoints[type] >= points)
	return false; // don't pick up

      armorfactor[type] = factor;
      armorpoints[type] = points;
    }
  else
    {
      // negative factor means bonus to current armor
      int i, total = int(100 * toughness);
      for (i = 0; i < NUMARMOR; i++)
	total += armorpoints[i];

      if (total >= MaxArmor[pclass])
	return false;

      if (armorfactor[type] < -factor)
	armorfactor[type] = -factor;
      armorpoints[type] += points;
    }

  return true;
}


bool PlayerPawn::GiveKey(keycard_t k)
{
  if (keycards & k)
    return false;

  keycards |= k;

  int i, j = k;
  for (i = -1; j; i++)
    j >>= 1; // count the key number
    
  player->SetMessage(text[TXT_KEY_STEEL + i]);

  p_sound = sfx_keyup;
  return true;
}


//---------------------------------------------------------------------------
//
// Removes the MF_SPECIAL flag, and initiates the artifact pickup
// animation. The artifact is restored after a number of tics by an action function.
//
//---------------------------------------------------------------------------

static void SetDormantArtifact(DActor *arti)
{
  arti->flags &= ~MF_SPECIAL;
  if (cv_deathmatch.value && !(arti->flags & MF_DROPPED))
    {
      // respawn delay
      if (arti->type == MT_XARTIINVULNERABILITY)
	arti->SetState(S_DORMANTARTI3_1);
      //else if (arti->type == MT_ARTIINVISIBILITY) 
      else if (arti->type == MT_SUMMONMAULATOR || arti->type == MT_XARTIFLY)
	arti->SetState(S_DORMANTARTI2_1);
      else
	arti->SetState(S_DORMANTARTI1_1);
    }
  else
    arti->SetState(S_DEADARTI1); // Don't respawn

  S_StartSound(arti, sfx_artiup);
}


void A_RestoreArtifact(DActor *arti)
{
  arti->flags |= MF_SPECIAL;
  arti->SetState(arti->info->spawnstate);
  S_StartSound(arti, sfx_itemrespawn);
}


bool PlayerPawn::GiveArtifact(artitype_t arti, DActor *from)
{
  if (arti >= NUMARTIFACTS || arti <= arti_none)
    return false;

  vector<inventory_t>::iterator i = inventory.begin();

  // find the right slot
  while (i < inventory.end() && i->type != arti)
    i++;

  if (i != inventory.end())
    {
      // player already has some of these
      if (i->count >= MAXARTECONT)
	return false;
    }

  p_remove = false;  // TODO we could just as well remove the artifacts (they will respawn in dm!)

  int j;
  if (arti < arti_firstpuzzitem)
    {
      j = TXT_ARTIINVULNERABILITY - 1 + arti;
      if (from->type == MT_XARTIINVULNERABILITY)
	j = TXT_XARTIINVULNERABILITY; // TODO make it a different item
      player->SetMessage(text[TXT_ARTIINVULNERABILITY - 1 + arti], false);
      SetDormantArtifact(from);
      p_sound = sfx_artiup;
    }
  else if (arti < arti_fsword1)
    {
      // Puzzle item
      j = TXT_ARTIPUZZSKULL + arti - arti_firstpuzzitem;
      if (arti >= arti_puzzgear1)
	j = TXT_ARTIPUZZGEAR;
      player->SetMessage(text[j], false);
      SetDormantArtifact(from);
      /*
	if (!game.multiplayer || cv_deathmatch.value)
	p_remove = true; // TODO remove puzzle items if not cooperative netplay?
      */
    }
  else
    {
      // weapon piece
      p_remove = true;
      int  wclass = 1 + ((arti - arti_fsword1) / 3);

      bool gave_mana = false;
      bool gave_piece = false;
      bool coop_multiplayer = (game.multiplayer && !cv_deathmatch.value);

      if (pclass != wclass)
	{
	  // wrong class, but try to pick up for mana
	  if (coop_multiplayer)
	    return false; // you can't steal other players' weapons

	  gave_mana = GiveAmmo(am_mana1, 20) || GiveAmmo(am_mana2, 20);
	}
      else 
	{
	  // right class
	  gave_piece = (i == inventory.end()); // only pick up pieces you don't have

	  if (coop_multiplayer)
	    {
	      if (!gave_piece)
		return false;

	      // TODO if you die, you lose all pieces gathered so far... leave them near the corpse?
	      GiveAmmo(am_mana1, 20);
	      GiveAmmo(am_mana2, 20);
	      p_remove = false;
	    }
	  else
	    {
	      // dm or single player game
	      gave_mana = GiveAmmo(am_mana1, 20) || GiveAmmo(am_mana2, 20);
	    }
	}

      if (!gave_mana && !gave_piece)
	return false;

      // Picked up the weapon piece (or just the mana)
      player->SetMessage(text[wclass - 1 + TXT_QUIETUS_PIECE], false);
      p_sound = sfx_weaponup;

      if (!gave_piece)
	return true;
    }

  if (from && (from->flags & MF_COUNTITEM))
    player->items++;

  if (i == inventory.end())
    {
      // give one artifact
      inventory.push_back(inventory_t(arti, 1));
    }
  else
    {
      // player already has some of these
      if (i->count >= MAXARTECONT)
	// Player already has 16 of this item
	return false;
      
      i->count++; // one more
    }

  return true;
}
