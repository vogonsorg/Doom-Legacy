// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id$
//
// Copyright (C) 1998-2000 by DooM Legacy Team.
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
// $Log: dosstr.c,v $
// Revision 1.3  2000/08/11 19:11:07  metzgermeister
// *** empty log message ***
//
// Revision 1.2  2000/02/27 00:42:11  hurdler
// fix CR+LF problem
//
// Revision 1.1.1.1  2000/02/22 20:32:33  hurdler
// Initial import into CVS (v1.29 pr3)
//
//
// DESCRIPTION:
//
//
//-----------------------------------------------------------------------------


#include <stdlib.h>
#include <ctype.h>

int strupr(char *n) {
      int i;
      for (i=0;n[i];i++) {
               n[i] = toupper(n[i]);
      }
      return 1;
}

int strlwr(char *n) {
      int i;
      for (i=0;n[i];i++) {
               n[i] = tolower(n[i]);
      }
      return 1;
}
