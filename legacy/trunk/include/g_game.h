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
// $Log$
// Revision 1.10  2004/07/05 16:53:29  smite-meister
// Netcode replaced
//
// Revision 1.9  2004/04/25 16:26:50  smite-meister
// Doxygen
//
// Revision 1.7  2003/12/31 18:32:50  smite-meister
// Last commit of the year? Sound works.
//
// Revision 1.6  2003/11/12 11:07:26  smite-meister
// Serialization done. Map progression.
//
// Revision 1.5  2003/06/10 22:39:59  smite-meister
// Bugfixes
//
// Revision 1.4  2003/05/11 21:23:52  smite-meister
// Hexen fixes
//
// Revision 1.3  2002/12/29 18:57:03  smite-meister
// MAPINFO implemented, Actor deaths handled better
//
// Revision 1.2  2002/12/23 23:19:37  smite-meister
// Weapon groups, MAPINFO parser, WAD2+WAD3 support added!
//
// Revision 1.1.1.1  2002/11/16 14:18:23  hurdler
// Initial C++ version of Doom Legacy
//
//-----------------------------------------------------------------------------

/// \file
/// \brief GameInfo class definition

#ifndef g_game_h
#define g_game_h 1

#include <vector>
#include <map>
#include <string>

#include "doomdef.h"
#include "doomdata.h"

using namespace std;


/// skill levels
enum skill_t
{
  sk_baby,
  sk_easy,
  sk_medium,
  sk_hard,
  sk_nightmare
};


/// Game mode. For game-specific rules, IWAD dependent animations etc.
enum gamemode_t
{
  gm_none,
  gm_doom1s,  // DOOM 1 shareware, E1, M9
  gm_doom1,   // DOOM 1 registered, E3, M27
  gm_doom2,   // DOOM 2 retail (commercial), E1 M34
  gm_udoom,   // DOOM 1 retail (Ultimate DOOM), E4, M36
  gm_heretic,
  gm_hexen
};



/// \brief Game info common to all players.
///
/// There is only one instance in existence, called 'game'.
/// It stores all relevant data concerning one game,
/// including game state, flags, players, teams, maps etc.

class GameInfo
{
  friend class Intermission;
  friend class PlayerInfo;
  friend class LNetInterface;
  friend class LConnection;

private:
  /// delayed game state changes
  enum gameaction_t
  {
    ga_nothing,
    ga_intermission,
    ga_nextlevel,
    //HeXen
    /*
    ga_initnew,
    ga_newgame,
    ga_loadgame,
    ga_savegame,
    ga_leavemap,
    ga_singlereborn
    */
  };

  gameaction_t  action; ///< delayed state changes

public:
  /// current state of the game
  enum gamestate_t
    {
      GS_NULL = 0,      ///< only used during game startup
      GS_INTRO,         ///< no game running, playing intro loop
      GS_WAIT,
      GS_LEVEL,           ///< we are playing
      GS_INTERMISSION,    ///< gazing at the intermission screen
      GS_FINALE,          ///< game final animation
      GS_DEMOPLAYBACK     ///< watching a demo
    };

  gamestate_t   state;  ///< gamestate

  // demoversion is the 'dynamic' version number, this should be == game VERSION.
  // When playing back demos, 'demoversion' receives the version number of the
  // demo. At each change to the game play, demoversion is compared to
  // the game version, if it's older, the changes are not done, and the older
  // code is used for compatibility.

  unsigned demoversion;

  gamemode_t    mode;   ///< which game are we playing?
  skill_t       skill;  ///< skill level

  bool server;      ///< are we running a game locally?
  bool netgame;     ///< only true in a netgame (nonlocal players possible)
  bool multiplayer; ///< only true if >1 players. netgame => multiplayer but not (multiplayer => netgame)
  bool modified;    ///< an external modification-dll is in use
  bool nomonsters;  ///< checkparm of -nomonsters
  bool paused;      ///< is the game currently paused?

  bool inventory;   ///< PlayerPawns have an inventory

  LNetInterface *net; ///< our network interface (contains th enetstate)

  // Demo sequences
  int pagetic; ///< how many tics left until demo is changed?

  unsigned time; ///< how long (in ms) has the game been running?
  unsigned tic;  ///< how many times has the game been ticked?   

public:
  int maxplayers; ///< max # of players allowed
  int maxteams;   ///< max # of teams

  typedef map<int, class PlayerInfo*>::iterator player_iter_t;
  map<int, PlayerInfo*> Players; ///< mapping from player number to Playerinfo*

  vector<class TeamInfo*> teams;

  typedef map<int, class MapInfo*>::iterator mapinfo_iter_t;
  map<int, MapInfo*> mapinfo;

  typedef map<int, class MapCluster*>::iterator cluster_iter_t;
  map<int, MapCluster*> clustermap;

  MapCluster *currentcluster; ///< in which MapCluster are we in the game
  MapCluster *nextcluster; // temp HACK
  MapInfo    *currentmap;     // this is used ONLY for time/scorelimit games


public:

  GameInfo();
  ~GameInfo();



  bool Playing();
  void SV_Reset();
  bool SV_SpawnServer();
  void CL_Reset();

  void TryRunTics(tic_t realtics);

  void Display();

  int  Serialize(class LArchive &a);
  int  Unserialize(LArchive &a);


  // in g_game.cpp
  void StartIntro();
  void AdvanceIntro();

  void Drawer();
  bool Responder(struct event_t *ev);

  /// returns the player if he is in the game, otherwise NULL
  PlayerInfo *FindPlayer(int number);
  PlayerInfo *FindPlayer(const char *name);

  PlayerInfo *AddPlayer(PlayerInfo *p); ///< tries to add a player to the game
  bool RemovePlayer(int number);        ///< erases player from game
  void ClearPlayers();                  ///< erases all players





  void LoadGame(int slot);
  void SaveGame(int slot, char* description);

  bool Downgrade(int version);

  void Ticker(); // ticks the game forward in time




  void UpdateScore(PlayerInfo *killer, PlayerInfo *victim);
  int  GetFrags(struct fragsort_t **fs, int type);
  bool CheckScoreLimit();

  // ----- level-related stuff -----
  // in g_level.cpp
  int  Read_MAPINFO(int lump);
  void Clear_mapinfo_clusterdef();
  MapCluster *FindCluster(int c);
  MapInfo *FindMapInfo(int c);

  int Create_MAPINFO_game(int lump);
  int Create_classic_game(int episode);

  // in g_state.cpp
  bool NewGame(skill_t sk);
  bool StartGame();
  void StartIntermission();
  void EndIntermission();
  void EndFinale();
  void NextLevel();

  // ----- demos -----
  void BeginRecording();
  void PlayDemo(char *defdemoname);
  void ReadDemoTiccmd(struct ticcmd_t* cmd, int playernum);
  void WriteDemoTiccmd(ticcmd_t* cmd, int playernum);
  void StopDemo();
  bool CheckDemoStatus();
};

extern GameInfo game;

// for dedicated server
extern bool dedicated;

// miscellaneous stuff, doesn't really belong here
extern bool       devparm; // development mode (-devparm)


// ======================================
// DEMO playback/recording related stuff.
// ======================================


// demoplaying back and demo recording
extern  bool demorecording;
extern  bool timingdemo;       

// Quit after playing a demo from cmdline.
extern  bool singledemo;



#endif
