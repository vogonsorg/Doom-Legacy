// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// $Id$
//
// Copyright (C) 1998-2003 by DooM Legacy Team.
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
// Revision 1.5  2004/08/29 13:50:22  hurdler
// minor update
//
// Revision 1.4  2004/07/25 20:17:50  hurdler
// Remove old hardware renderer and add part of the new one
//
// Revision 1.3  2004/05/02 21:15:56  hurdler
// add dummy new renderer (bis)
//
// Revision 1.2  2003/01/25 21:33:06  smite-meister
// Now compiles with MinGW 2.0 / GCC 3.2.
// Builder can choose between dynamic and static linkage.
//
// Revision 1.1.1.1  2002/11/16 14:18:32  hurdler
// Initial C++ version of Doom Legacy
//
//
// DESCRIPTION:
//      SDL specific part of the OpenGL API for Doom Legacy
//
//-----------------------------------------------------------------------------

#ifdef FREEBSD
# include <SDL.h>
#else
# include <SDL/SDL.h>
#endif

#include "screen.h"
#include "v_video.h"
#include "hardware/hwr_render.h"
#include "command.h"
#include "cvars.h"

static SDL_Surface *vidSurface = NULL; //use the one from i_video_sdl.c instead?

bool OglSdlSurface()
{
  Uint32 surfaceFlags;

  if (NULL != vidSurface)
    {
      SDL_FreeSurface(vidSurface);
      vidSurface = NULL;
#ifdef VOODOOSAFESWITCHING
      SDL_QuitSubSystem(SDL_INIT_VIDEO);
      SDL_InitSubSystem(SDL_INIT_VIDEO);
#endif
    }

  if (cv_fullscreen.value)
    surfaceFlags = SDL_OPENGL|SDL_FULLSCREEN;
  else
    surfaceFlags = SDL_OPENGL;

  // We want at least 1 bit (???) for R, G, and B, and at least 16 bits for depth buffer.
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 1);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 1);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

  int cbpp = SDL_VideoModeOK(vid.width, vid.height, vid.BitsPerPixel, surfaceFlags);
  if (cbpp < 16)
    return false;
  if ((vidSurface = SDL_SetVideoMode(vid.width, vid.height, cbpp, surfaceFlags)) == NULL)
    return false;

  CONS_Printf("HWRend::Startup(): %dx%d %d bits\n", vid.width, vid.height, cbpp);
  HWR.Startup();

  return true;
}

void OglSdlFinishUpdate(bool vidwait)
{
  SDL_GL_SwapBuffers();
}

void OglSdlShutdown()
{
  if (NULL != vidSurface)
    {
      SDL_FreeSurface(vidSurface);
      vidSurface = NULL;
    }
}

void OglSdlSetGamma(float r, float g, float b)
{
  SDL_SetGamma(r, g, b);
}

/*
void OglSdlSetPalette(RGBA_t *palette, RGBA_t *gamma)
{
    int i;

    for (i=0; i<256; i++) {
        myPaletteData[i].s.red   = MIN((palette[i].s.red   * gamma->s.red)  /127, 255);
        myPaletteData[i].s.green = MIN((palette[i].s.green * gamma->s.green)/127, 255);
        myPaletteData[i].s.blue  = MIN((palette[i].s.blue  * gamma->s.blue) /127, 255);
        myPaletteData[i].s.alpha = palette[i].s.alpha;
    }
    // on a chang� de palette, il faut recharger toutes les textures
    // jaja, und noch viel mehr ;-)
    Flush();
}
*/
