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
// Revision 1.6  2003/02/08 21:43:50  smite-meister
// New Memzone system. Choose your pawntype! Cyberdemon OK.
//
// Revision 1.5  2003/01/25 21:33:06  smite-meister
// Now compiles with MinGW 2.0 / GCC 3.2.
// Builder can choose between dynamic and static linkage.
//
// Revision 1.4  2003/01/12 12:56:42  smite-meister
// Texture bug finally fixed! Pickup, chasecam and sw renderer bugs fixed.
//
// Revision 1.3  2002/12/16 22:14:50  smite-meister
// Video unit fix
//
// Revision 1.2  2002/12/03 10:24:47  smite-meister
// Video system overhaul
//
//
// DESCRIPTION:
//   SDL video interface
//
//-----------------------------------------------------------------------------

#include <stdlib.h>

#ifdef FREEBSD
# include <SDL.h>
#else
# include <SDL/SDL.h>
#endif

#include "doomdef.h"

#include "i_video.h"
#include "screen.h"
#include "v_video.h"
#include "keys.h"
#include "m_argv.h"
#include "m_menu.h"
#include "d_main.h"
#include "g_input.h"
#include "command.h"

#include "m_dll.h"
#include "hardware/r_opengl/r_opengl.h"
#include "sdl/ogl_sdl.h"

#ifdef DYNAMIC_LINKAGE
static dll_handle_t ogl_handle = NULL;
static const dll_info_t *ogl_info = NULL;
#endif

// SDL vars
static const SDL_VideoInfo *vidInfo = NULL;
static SDL_Rect     **modeList = NULL;
static SDL_Surface   *vidSurface = NULL;
static SDL_Color      localPalette[256];

//const static Uint32       surfaceFlags = SDL_HWSURFACE|SDL_HWPALETTE|SDL_DOUBLEBUF;
// FIXME : VB: since fullscreen HW surfaces wont't work, we'll use a SW surface.
const static Uint32 surfaceFlags = SDL_SWSURFACE | SDL_HWPALETTE;

static int numVidModes = 0;
static char vidModeName[33][32]; // allow 33 different modes


// maximum number of windowed modes (see windowedModes[][])
#define MAXWINMODES (6) 

//Hudler: 16/10/99: added for OpenGL gamma correction
RGBA_t  gamma_correction = {0x7F7F7F7F};
extern consvar_t cv_grgammared;
extern consvar_t cv_grgammagreen;
extern consvar_t cv_grgammablue;

extern consvar_t cv_fullscreen; // for fullscreen support 

extern consvar_t cv_usemouse;

rendermode_t    rendermode = render_soft;

// synchronize page flipping with screen refresh
// unused and for compatibility reason 
consvar_t       cv_vidwait = {"vid_wait","1",CV_SAVE,CV_OnOff};

bool graphics_started = false; // Is used in console.c and screen.c

// To disable fullscreen at startup; is set in I_PrepareVideoModeList
bool allow_fullscreen = false;


// first entry in the modelist which is not bigger than 1024x768
static int firstEntry = 0;


// windowed video modes from which to choose from.
static int windowedModes[MAXWINMODES][2] =
{
  {MAXVIDWIDTH /*1024*/, MAXVIDHEIGHT/*768*/},
  {800, 600},
  {640, 480},
  {512, 384},
  {400, 300},
  {320, 200}
};
//
//  Translates the SDL key into Doom key
//

static int xlatekey(SDLKey sym)
{
    int rc=0;

    switch(sym)
    {
    case SDLK_LEFT:  rc = KEY_LEFTARROW;     break;
    case SDLK_RIGHT: rc = KEY_RIGHTARROW;    break;
    case SDLK_DOWN:  rc = KEY_DOWNARROW;     break;
    case SDLK_UP:    rc = KEY_UPARROW;       break;

    case SDLK_ESCAPE:   rc = KEY_ESCAPE;        break;
    case SDLK_RETURN:   rc = KEY_ENTER;         break;
    case SDLK_TAB:      rc = KEY_TAB;           break;
    case SDLK_F1:       rc = KEY_F1;            break;
    case SDLK_F2:       rc = KEY_F2;            break;
    case SDLK_F3:       rc = KEY_F3;            break;
    case SDLK_F4:       rc = KEY_F4;            break;
    case SDLK_F5:       rc = KEY_F5;            break;
    case SDLK_F6:       rc = KEY_F6;            break;
    case SDLK_F7:       rc = KEY_F7;            break;
    case SDLK_F8:       rc = KEY_F8;            break;
    case SDLK_F9:       rc = KEY_F9;            break;
    case SDLK_F10:      rc = KEY_F10;           break;
    case SDLK_F11:      rc = KEY_F11;           break;
    case SDLK_F12:      rc = KEY_F12;           break;

    case SDLK_BACKSPACE: rc = KEY_BACKSPACE;    break;
    case SDLK_DELETE:    rc = KEY_DEL;          break;

    case SDLK_PAUSE:     rc = KEY_PAUSE;        break;

    case SDLK_EQUALS:
    case SDLK_PLUS:      rc = KEY_EQUALS;       break;

    case SDLK_MINUS:     rc = KEY_MINUS;        break;

    case SDLK_LSHIFT:
    case SDLK_RSHIFT:
        rc = KEY_SHIFT;
        break;
          
        //case SDLK_XK_Caps_Lock:
        //rc = KEY_CAPSLOCK;
        //break;

    case SDLK_LCTRL:
    case SDLK_RCTRL:
        rc = KEY_CTRL;
        break;
          
    case SDLK_LALT:
    case SDLK_RALT:
        rc = KEY_ALT;
        break;
        
    case SDLK_PAGEUP:   rc = KEY_PGUP; break;
    case SDLK_PAGEDOWN: rc = KEY_PGDN; break;
    case SDLK_END:      rc = KEY_END;  break;
    case SDLK_HOME:     rc = KEY_HOME; break;
    case SDLK_INSERT:   rc = KEY_INS;  break;
      
    case SDLK_KP0: rc = KEY_KEYPAD0;  break;
    case SDLK_KP1: rc = KEY_KEYPAD1;  break;
    case SDLK_KP2: rc = KEY_KEYPAD2;  break;
    case SDLK_KP3: rc = KEY_KEYPAD3;  break;
    case SDLK_KP4: rc = KEY_KEYPAD4;  break;
    case SDLK_KP5: rc = KEY_KEYPAD5;  break;
    case SDLK_KP6: rc = KEY_KEYPAD6;  break;
    case SDLK_KP7: rc = KEY_KEYPAD7;  break;
    case SDLK_KP8: rc = KEY_KEYPAD8;  break;
    case SDLK_KP9: rc = KEY_KEYPAD9;  break;

    case SDLK_KP_MINUS:  rc = KEY_KPADDEL;  break;
    case SDLK_KP_DIVIDE: rc = KEY_KPADSLASH; break;
    case SDLK_KP_ENTER:  rc = KEY_ENTER;    break;

    default:
        if (sym >= SDLK_SPACE && sym <= SDLK_DELETE)
            rc = sym - SDLK_SPACE + ' ';
        if (sym >= 'A' && sym <= 'Z')
            rc = sym - 'A' + 'a';
        break;
    }
    
    return rc;
}



//
// I_StartFrame
//
void I_StartFrame()
{
  if (render_soft == rendermode)
    {
      if (SDL_MUSTLOCK(vidSurface))
        {
	  if (SDL_LockSurface(vidSurface) < 0)
	    return;
        }
    }

  return;
}

static int      lastmousex = 0;
static int      lastmousey = 0;

#ifdef LJOYSTICK
extern void I_GetJoyEvent();
#endif
#ifdef LMOUSE2
extern void I_GetMouse2Event();
#endif


void I_GetEvent()
{
  SDL_Event inputEvent;
  event_t event;    

#ifdef LJOYSTICK
  I_GetJoyEvent();
#endif
#ifdef LMOUSE2
  I_GetMouse2Event();
#endif

  //SDL_PumpEvents(); //SDL_PollEvent calls this automatically
    
  while(SDL_PollEvent(&inputEvent))
    {
      switch(inputEvent.type)
        {
        case SDL_KEYDOWN:
	  event.type = ev_keydown;
	  event.data1 = xlatekey(inputEvent.key.keysym.sym);
	  D_PostEvent(&event);
	  break;
        case SDL_KEYUP:
	  event.type = ev_keyup;
	  event.data1 = xlatekey(inputEvent.key.keysym.sym);
	  D_PostEvent(&event);
	  break;
        case SDL_MOUSEMOTION:
	  if(cv_usemouse.value)
            {
	      // If the event is from warping the pointer back to middle
	      // of the screen then ignore it.
	      if ((inputEvent.motion.x == vid.width/2) &&
		  (inputEvent.motion.y == vid.height/2)) 
                {
		  lastmousex = inputEvent.motion.x;
		  lastmousey = inputEvent.motion.y;
		  break;
                } 
	      else 
                {
		  event.data2 = (inputEvent.motion.x - lastmousex) << 2;
		  lastmousex = inputEvent.motion.x;
		  event.data3 = (lastmousey - inputEvent.motion.y) << 2;
		  lastmousey = inputEvent.motion.y;
                }
	      event.type = ev_mouse;
	      event.data1 = 0;
            
	      D_PostEvent(&event);
            
	      // Warp the pointer back to the middle of the window
	      //  or we cannot move any further if it's at a border.
	      if ((inputEvent.motion.x < (vid.width/2)-(vid.width/4)) || 
		  (inputEvent.motion.y < (vid.height/2)-(vid.height/4)) || 
		  (inputEvent.motion.x > (vid.width/2)+(vid.width/4)) || 
		  (inputEvent.motion.y > (vid.height/2)+(vid.height/4)))
                {
		  SDL_WarpMouse(vid.width/2, vid.height/2);
                }
            }
	  break;
        case SDL_MOUSEBUTTONDOWN:
	  if(cv_usemouse.value)
            {
	      event.type = ev_keydown;
	      event.data1 = KEY_MOUSE1 + inputEvent.button.button -1; // FIXME!
	      D_PostEvent(&event);
            }
	  break;
        case SDL_MOUSEBUTTONUP:
	  if(cv_usemouse.value)
            {
	      event.type = ev_keyup;
	      event.data1 = KEY_MOUSE1 + inputEvent.button.button -1;
	      D_PostEvent(&event);
            }
	  break;
        case SDL_QUIT:
	  M_QuitResponse('y');
	  break;
        default:
	  break;	  
        }
    }
}

static void doGrabMouse()
{
  if(SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_OFF)
  {
    SDL_WM_GrabInput(SDL_GRAB_ON);
  }
}

static void doUngrabMouse()
{
  if(SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_ON)
  {
    SDL_WM_GrabInput(SDL_GRAB_OFF);
  }
}

void I_StartupMouse()
{
    SDL_Event inputEvent;
    
    // warp to center 
    SDL_WarpMouse(vid.width/2, vid.height/2);
    lastmousex = vid.width/2;
    lastmousey = vid.height/2;
    // remove the mouse event by reading the queue
    SDL_PollEvent(&inputEvent);
    
#ifdef HAS_SDL_BEEN_FIXED // FIXME
  if(cv_usemouse.value) 
    {
      doGrabMouse();
    }
  else
    {
      doUngrabMouse();
    }
#endif
    return;
}

//
// I_OsPolling
//
void I_OsPolling()
{
  if (!graphics_started)
    return;

  I_GetEvent();
    
  return;
}


//
// I_UpdateNoBlit
//
void I_UpdateNoBlit()
{
  /* this function intentionally left empty */
}

//
// I_FinishUpdate
//
void I_FinishUpdate()
{
  if (rendermode == render_soft)
    {
      if (vid.screens[0] != vid.direct)
	{
	  memcpy(vid.direct, vid.screens[0], vid.height*vid.rowbytes);
	  //vid.screens[0] = vid.direct; //FIXME: we MUST render directly into the surface
	}
      //SDL_Flip(vidSurface);
      SDL_UpdateRect(vidSurface, 0, 0, 0, 0);
      if (SDL_MUSTLOCK(vidSurface))
	SDL_UnlockSurface(vidSurface);
    }
  else
    OglSdlFinishUpdate(cv_vidwait.value);
    
  I_GetEvent();
    
  return;
}


//
// I_ReadScreen
//
void I_ReadScreen(byte* scr)
{
  if (rendermode != render_soft)
    I_Error ("I_ReadScreen: called while in non-software mode");
    
  memcpy (scr, vid.screens[0], vid.height*vid.rowbytes);
}



//
// I_SetPalette
//
void I_SetPalette(RGBA_t* palette)
{
  int i;

  CONS_Printf("I_SetPalette\n");

  for(i=0; i<256; i++)
    {
      localPalette[i].r = palette[i].s.red;
      localPalette[i].g = palette[i].s.green;
      localPalette[i].b = palette[i].s.blue;
    }

  SDL_SetColors(vidSurface, localPalette, 0, 256);
}


// return number of fullscreen or windowed modes
int I_NumVideoModes() 
{
  if (cv_fullscreen.value) 
    return numVidModes - firstEntry;
  else
    return MAXWINMODES;
}

char *I_GetVideoModeName(int modeNum) 
{
  if (cv_fullscreen.value)
    {
      modeNum += firstEntry;
      if (modeNum >= numVidModes)
	return NULL;
        
      sprintf(&vidModeName[modeNum][0], "%dx%d",
	      modeList[modeNum]->w,
	      modeList[modeNum]->h);
    }
  else
    { // windowed modes
      if (modeNum > MAXWINMODES)
	return NULL;
        
      sprintf(&vidModeName[modeNum][0], "win %dx%d",
	      windowedModes[modeNum][0],
	      windowedModes[modeNum][1]);
    }
  return &vidModeName[modeNum][0];
}

int I_GetVideoModeForSize(int w, int h)
{
  int matchMode = -1;
  int i;
  
  if (cv_fullscreen.value)
    {
      for (i = firstEntry; i<numVidModes; i++)
	{
	  if (modeList[i]->w == w && modeList[i]->h == h)
	    {
	      matchMode = i;
	      break;
	    }
	}

      if (matchMode == -1) // use smallest mode
	matchMode = numVidModes-1;

      matchMode -= firstEntry;
    }
  else
    {
      for(i = 0; i<MAXWINMODES; i++)
	{
	  if (windowedModes[i][0] == w && windowedModes[i][1] == h)
	    {
	      matchMode = i;
	      break;
	    }
	}

      if (matchMode == -1) // use smallest mode
	  matchMode = MAXWINMODES-1;
    }
  
  return matchMode;
}


void I_PrepareVideoModeList()
{
  int i;
    
  if (cv_fullscreen.value) // only fullscreen needs preparation
    {
      if(numVidModes != -1) 
        {
	  for(i=0; i<numVidModes; i++)
            {
	      if(modeList[i]->w <= MAXVIDWIDTH &&
		 modeList[i]->h <= MAXVIDHEIGHT)
                {
		  firstEntry = i;
		  break;
                }
            }
        }
    }
    
  allow_fullscreen = true;
  return;
}

int I_SetVideoMode(int modeNum)
{
  doUngrabMouse();

  Uint32 flags;

  vid.modenum = modeNum;
  // For some reason, under Win98 the combination SDL_HWSURFACE | SDL_FULLSCREEN
  // doesn't give a valid pixels pointer. Odd.
  if (cv_fullscreen.value)
    {
      modeNum += firstEntry;        
      vid.width = modeList[modeNum]->w;
      vid.height = modeList[modeNum]->h;
      flags = surfaceFlags | SDL_FULLSCREEN;

      CONS_Printf ("I_SetVideoMode: fullscreen %d x %d (%d bpp)\n", vid.width, vid.height, vid.BitsPerPixel);
    }
  else
    { // !cv_fullscreen.value
      vid.width = windowedModes[modeNum][0];
      vid.height = windowedModes[modeNum][1];
      flags = surfaceFlags;

      CONS_Printf("I_SetVideoMode: windowed %d x %d (%d bpp)\n", vid.width, vid.height, vid.BitsPerPixel);
        
      // Window title
      SDL_WM_SetCaption("Legacy", "Legacy");
    }

  if (rendermode == render_soft)
    {
      SDL_FreeSurface(vidSurface);
	
      vidSurface = SDL_SetVideoMode(vid.width, vid.height, vid.BitsPerPixel, flags);
      if (vidSurface == NULL)
	I_Error("Could not set vidmode\n");
	              
      if (vidSurface->pixels == NULL)
	I_Error("Didn't get a valid pixels pointer (SDL). Exiting.\n");

      vid.direct = (byte *)vidSurface->pixels;
      // VB: FIXME this stops execution at the latest
      *((Uint8 *)vidSurface->pixels) = 1;
    }
  else
    {
      if (!OglSdlSurface(vid.width, vid.height, cv_fullscreen.value))
	I_Error("Could not set vidmode\n");
    }
    
  I_StartupMouse();

  return 1;
}

bool I_StartupGraphics()
{
  if (graphics_started)
    return true;
  
  CV_RegisterVar(&cv_vidwait);
  
  // Initialize Audio as well, otherwise DirectX can not use audio
  if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0)
    {
      CONS_Printf("Couldn't initialize SDL: %s\n", SDL_GetError());
      return false;
    }

  // Get video info for screen resolutions  
  vidInfo = SDL_GetVideoInfo();
  // now we _could_ do all kinds of cool tests to determine which
  // video modes are available, but...
  // vidInfo->vfmt is the pixelformat of the "best" video mode available

  CONS_Printf("Bpp = %d, bpp = %d\n", vidInfo->vfmt->BytesPerPixel, vidInfo->vfmt->BitsPerPixel);

  // list all available video modes corresponding to the "best" pixelformat
  modeList = SDL_ListModes(NULL, SDL_FULLSCREEN | surfaceFlags);

  numVidModes = 0;
  if (modeList == NULL)
    {
      CONS_Printf("No video modes present\n");
      return false;
    }

  while (modeList[numVidModes])
    numVidModes++;
    
  CONS_Printf("Found %d video modes\n", numVidModes);
  
  //for(k=0; modeList[k]; ++k)
  //  CONS_Printf("  %d x %d\n", modeList[k]->w, modeList[k]->h);

  // even if I set vid.bpp and highscreen properly it does seem to
  // support only 8 bit  ...  strange
  // so lets force 8 bit (software mode only)
  // TODO why not use hicolor in sw mode too? it must work...
  // Set color depth; either 1=256pseudocolor or 2=hicolor
  vid.BytesPerPixel = 1;  //videoInfo->vfmt->BytesPerPixel
  vid.BitsPerPixel = 8;
  //highcolor = (vid.bpp == 2) ? true:false;

  // default resolution
  vid.width = BASEVIDWIDTH;
  vid.height = BASEVIDHEIGHT;

  // Window title
  SDL_WM_SetCaption("Legacy", "Legacy");
  
  if (M_CheckParm("-opengl")) 
    {
      rendermode = render_opengl;

#ifdef DYNAMIC_LINKAGE
      // dynamic linkage
      ogl_handle = OpenDLL("r_opengl.dll");
      if (!ogl_handle)
	I_Error("Could not load r_opengl.dll!\n");

      ogl_info = (dll_info_t *)GetSymbol(ogl_handle, "dll_info");
      if (ogl_info->interface_version != R_OPENGL_INTERFACE_VERSION)
	I_Error("r_opengl.dll interface version does not match with Legacy.exe!\n"
		"You must use the r_opengl.dll that came in the same distribution as your Legacy.exe.");

      hw_renderer_export_t *temp = (hw_renderer_export_t *)GetSymbol(ogl_handle, "r_export");
      memcpy(&HWD, temp, sizeof(hw_renderer_export_t));
      CONS_Printf("%s loaded.\n", ogl_info->dll_name);
#else
      // static linkage
      memcpy(&HWD, &r_export, sizeof(hw_renderer_export_t));
#endif

      vid.width = 640; // hack to make voodoo cards work in 640x480
      vid.height = 480;

      if (!OglSdlSurface(vid.width, vid.height, cv_fullscreen.value))
	rendermode = render_soft;
    }
    
  if (rendermode == render_soft)
    {
      // not fullscreen
      CONS_Printf("I_StartupGraphics: windowed %d x %d x %d bpp\n", vid.width, vid.height, vid.BitsPerPixel);
      vidSurface = SDL_SetVideoMode(vid.width, vid.height, vid.BitsPerPixel, surfaceFlags);
      
      if (vidSurface == NULL)
        {
	  CONS_Printf("Could not set vidmode\n");
	  return false;
        }
      vid.direct = (byte *)vidSurface->pixels;
    }
    
  SDL_ShowCursor(SDL_DISABLE);
  doUngrabMouse();
  
  graphics_started = true;
    
  return true;
}

void I_ShutdownGraphics()
{
  // was graphics initialized anyway?
  if (!graphics_started)
    return;

  if (rendermode == render_soft)
    {
      // vidSurface should be automatically freed
    }
  else
    {
      OglSdlShutdown();

#ifdef DYNAMIC_LINKAGE
      if (ogl_handle)
	CloseDLL(ogl_handle);
#endif
    }
  SDL_Quit(); 
}
