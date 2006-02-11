// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id$
//
// Copyright (C) 1998-2005 by DooM Legacy Team.
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
//-----------------------------------------------------------------------------

/// \file
/// \brief Global map utility functions

#ifndef p_maputl_h
#define p_maputl_h 1

#include <vector>
#include "vect.h"
#include "m_fixed.h"
#include "tables.h"


#define USERANGE 64


extern int validcount;

// P_MAP
// variables used by movement functions to communicate
extern bool    floatok;
extern fixed_t tmfloorz, tmceilingz;
extern class Actor *linetarget;

extern std::vector<struct line_t *> spechit;

struct position_check_t
{
  Actor  *thing;
  line_t *line;
};

extern position_check_t Blocking;
extern line_t *ceilingline;


// P_MAPUTL

void P_DelSeclist(struct msecnode_t *p);

struct intercept_t
{
  class Map    *m; // ugly but necessary, since line_t's don't carry a Map *. Actors do.
  fixed_t    frac; // along trace line
  bool    isaline;
  union
  {
    Actor  *thing;
    line_t *line;
  };
};


/// \brief Encapsulates the XY-plane geometry of a linedef for line traces. 
struct divline_t 
{
  fixed_t   x, y; ///< starting point (v1)
  fixed_t dx, dy; ///< v2-v1

  void MakeDivline(const line_t *li);
};


fixed_t P_AproxDistance(fixed_t dx, fixed_t dy);
int     P_PointOnLineSide(fixed_t x, fixed_t y, const line_t *line);
int     P_PointOnDivlineSide(fixed_t x, fixed_t y, divline_t *line);
fixed_t P_InterceptVector(divline_t* v2, divline_t* v1);


struct line_opening_t
{
  fixed_t top, bottom, range, lowfloor;
};

line_opening_t *P_LineOpening(line_t *linedef);


enum
{
  PT_ADDLINES  = 1,
  PT_ADDTHINGS = 2,
  PT_EARLYOUT  = 4
};

extern divline_t trace;
extern class bbox_t tmb;




inline angle_t R_PointToAngle2(const vec_t<fixed_t>& a, const vec_t<fixed_t>& b)
{
  return R_PointToAngle2(a.x, a.y, b.x, b.y);
}

inline fixed_t P_XYdist(const vec_t<fixed_t>& a, const vec_t<fixed_t>& b)
{
  return P_AproxDistance(a.x - b.x, a.y - b.y);
}

#endif
