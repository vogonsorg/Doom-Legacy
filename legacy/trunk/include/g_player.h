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
// $Log$
// Revision 1.19  2004/11/04 21:12:54  smite-meister
// save/load fixed
//
// Revision 1.18  2004/10/27 17:37:09  smite-meister
// netcode update
//
// Revision 1.17  2004/08/12 18:30:29  smite-meister
// cleaned startup
//
// Revision 1.16  2004/07/13 20:23:37  smite-meister
// Mod system basics
//
// Revision 1.15  2004/07/09 19:43:40  smite-meister
// Netcode fixes
//
// Revision 1.14  2004/07/05 16:53:29  smite-meister
// Netcode replaced
//
// Revision 1.13  2004/04/25 16:26:51  smite-meister
// Doxygen
//
//-----------------------------------------------------------------------------

/// \file
/// \brief PlayerInfo class definition.

#ifndef g_player_h
#define g_player_h 1

#include <map>
#include <vector>
#include <deque>
#include <string>
#include "tnl/tnlNetObject.h"
#include "doomtype.h"
#include "d_ticcmd.h"
#include "d_items.h"

using namespace std;
using namespace TNL;


/// Player states.
enum playerstate_t
{
  PST_NEEDMAP,      ///< waiting to be assigned to a map respawn queue (by GameInfo)
  PST_RESPAWN,      ///< waiting in a respawn queue (of a certain Map)
  PST_ALIVE,        ///< playing, camping or spectating
  PST_DEAD,         ///< dead on the ground, view follows killer
  PST_INTERMISSION, ///< viewing an intermission
  PST_REMOVE        ///< waiting to be removed from the game
};


/// \brief Describes a single player, either human or AI.
///
/// Created when a player joins the game, deleted when he leaves.
/// Ghosted over network.
class PlayerInfo : public NetObject
{
  friend class GameInfo;
  typedef NetObject Parent;
  TNL_DECLARE_CLASS(PlayerInfo);

  virtual bool onGhostAdd(class GhostConnection *c);
  virtual void onGhostRemove();
  virtual U32  packUpdate(GhostConnection *c, U32 updateMask, class BitStream *s);
  virtual void unpackUpdate(GhostConnection *c, BitStream *s);

public:
  int    number;   ///< player number
  int    team;     ///< index into game.teams vector
  string name;     ///< name of the player

  class LConnection *connection; ///< network connection
  unsigned client_hash;          ///< hash of the client network address

  playerstate_t playerstate;
  bool spectator;  ///< ghost spectator in a map?
  bool map_completed; ///< TEST has finished the map, but continues playing

  int requestmap;  ///< the map which we wish to enter
  int entrypoint;  ///< which spawning point to use

  ticcmd_t  cmd;   ///< current state of the player's controls

  class Map        *mp;   ///< the map with which the player is currently associated
  class PlayerPawn *pawn; ///< the thing that is being controlled by this player (marine, imp, whatever)
  class Actor      *pov;  ///< the POV of the player. usually same as pawn, but can also be a chasecam etc...


  //============ Score ============

  map<int, int> Frags; ///< mapping from player number to how many times you have fragged him
  int score;           ///< game-type dependent scoring based on frags, updated in real time
  int kills, items, secrets, time; ///< accomplishments in the current Map


  //============ Messages ============

  enum messagetype_t
  {
    M_CONSOLE = 0, ///< print message on console (echoed briefly on HUD)
    M_HUD          ///< glue message to HUD for a number of seconds
  };

  struct message_t
  {
    int priority;
    int type;
    string msg;
  };

  int messagefilter; ///< minimum message priority the player wants to receive
  deque<message_t> messages; ///< local message queue


  //============ Preferences ============

  // Weapon preferences
  char weaponpref[NUMWEAPONS];  ///< 
  bool originalweaponswitch;    ///< 
  bool autoaim;                 ///< using autoaim?

  // Pawn preferences. Can be changed during the game, take effect at next respawn.
  int ptype; ///< what kind of pawn are we playing?
  int color; ///< skin color to be copied to each pawn
  int skin;  ///< skin to be copied to each pawn


  //============ Feedback ============

  // POV height and bobbing during movement.
  fixed_t  viewz;           ///< absolute viewpoint z coordinate
  fixed_t  viewheight;      ///< distance from feet to eyes
  fixed_t  deltaviewheight; ///< bob/squat speed.
  fixed_t  bob_amplitude;   ///< basically pawn speed squared, affects weapon movement

  // HUD flashes
  int palette;
  int damagecount;
  int bonuscount;
  //int poisoncount;
  int itemuse;


public:
  PlayerInfo(const string & n = "");

  int Serialize(class LArchive &a);
  int Unserialize(LArchive &a);

  virtual void GetInput(int localpnum, int elapsed) { cmd.Build(localpnum, elapsed); }
  void ExitLevel(int nextmap, int ep);
  void Reset(bool resetpawn, bool resetfrags);  // resets the player (when starting a new level, for example)

  virtual void SetMessage(const char *msg, int priority = 0, int type = M_CONSOLE);

  void CalcViewHeight(bool onground); // update bobbing view height
};



// model PI's for both local players
extern PlayerInfo localplayer;
extern PlayerInfo localplayer2;

extern vector<PlayerInfo *> Consoleplayer;  // local players

#endif
