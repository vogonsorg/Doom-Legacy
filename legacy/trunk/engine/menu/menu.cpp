// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// $Id$
//
// Copyright (C) 1993-1996 by id Software, Inc.
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
// $Log$
// Revision 1.1  2005/03/16 21:16:07  smite-meister
// menu cleanup, bugfixes
//
// Revision 1.42  2004/12/08 16:52:06  segabor
// Missing devparm reference added; (Mac only) Input Sprockets conf temporarily(?) disabled
//
// Revision 1.41  2004/11/28 18:02:22  smite-meister
// RPCs finally work!
//
// Revision 1.39  2004/11/13 22:38:44  smite-meister
// intermission works
//
// Revision 1.38  2004/11/04 21:12:53  smite-meister
// save/load fixed
//
// Revision 1.37  2004/10/27 17:37:09  smite-meister
// netcode update
//
// Revision 1.36  2004/09/23 23:21:18  smite-meister
// HUD updated
//
// Revision 1.35  2004/09/09 22:04:38  jussip
// New joy code a bit more finished. Button binding works.
//
// Revision 1.33  2004/08/15 18:08:28  smite-meister
// palette-to-palette colormaps etc.
//
// Revision 1.32  2004/08/13 18:25:10  smite-meister
// sw renderer fix
//
// Revision 1.31  2004/08/12 18:30:27  smite-meister
// cleaned startup
//
// Revision 1.30  2004/07/25 20:19:21  hurdler
// Remove old hardware renderer and add part of the new one
//
// Revision 1.29  2004/07/13 20:23:37  smite-meister
// Mod system basics
//
// Revision 1.28  2004/07/11 14:32:00  smite-meister
// Consvars updated, bugfixes
//
// Revision 1.26  2004/07/05 16:53:28  smite-meister
// Netcode replaced
//
// Revision 1.25  2004/04/25 16:26:50  smite-meister
// Doxygen
//
// Revision 1.23  2004/01/10 16:02:59  smite-meister
// Cleanup and Hexen gameplay -related bugfixes
//
// Revision 1.22  2003/12/31 18:32:50  smite-meister
// Last commit of the year? Sound works.
//
// Revision 1.21  2003/12/06 23:57:47  smite-meister
// save-related bugfixes
//
// Revision 1.20  2003/12/03 10:49:50  smite-meister
// Save/load bugfix, text strings updated
//
// Revision 1.19  2003/11/27 11:28:26  smite-meister
// Doom/Heretic startup bug fixed
//
// Revision 1.18  2003/11/23 19:07:41  smite-meister
// New startup order
//
// Revision 1.17  2003/11/12 11:07:25  smite-meister
// Serialization done. Map progression.
//
// Revision 1.16  2003/06/29 17:33:59  smite-meister
// VFile system, PAK support, Hexen bugfixes
//
// Revision 1.15  2003/06/20 20:56:08  smite-meister
// Presentation system tweaked
//
// Revision 1.12  2003/04/14 08:58:29  smite-meister
// Hexen maps load.
//
// Revision 1.11  2003/04/04 00:01:57  smite-meister
// bugfixes, Hexen HUD
//
// Revision 1.10  2003/03/23 14:24:13  smite-meister
// Polyobjects, MD3 models
//
// Revision 1.9  2003/03/15 20:07:19  smite-meister
// Initial Hexen compatibility!
//
// Revision 1.8  2003/03/08 16:07:11  smite-meister
// Lots of stuff. Sprite cache. Movement+friction fix.
//
// Revision 1.7  2003/02/16 16:54:51  smite-meister
// L2 sound cache done
//
// Revision 1.6  2003/02/08 21:43:50  smite-meister
// New Memzone system. Choose your pawntype! Cyberdemon OK.
//
// Revision 1.5  2002/12/29 18:57:03  smite-meister
// MAPINFO implemented, Actor deaths handled better
//
// Revision 1.4  2002/12/23 23:15:41  smite-meister
// Weapon groups, MAPINFO parser added!
//
// Revision 1.3  2002/12/16 22:12:24  smite-meister
// Actor/DActor separation done!
//
//-----------------------------------------------------------------------------

/// \file
/// \brief Legacy menu. Sliders and icons. Kinda widget stuff.
///
/// NOTE:
///  The menu is scaled to the screen size. The scaling is always an
///  integer multiple of the original size, so that the graphics look good.


#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

#include "doomdef.h"
#include "command.h"
#include "cvars.h"
#include "console.h"

#include "m_menu.h"

#include "d_event.h"
#include "dstrings.h"

#include "r_main.h"
#include "r_data.h"
#include "r_draw.h"
#include "r_sprite.h"

#include "g_game.h"
#include "g_player.h"
#include "g_pawn.h"
#include "g_input.h"

#include "sounds.h"
#include "i_system.h"

#include "v_video.h"
#include "i_video.h"

#include "keys.h"
#include "w_wad.h"
#include "z_zone.h"
#include "m_archive.h"

#include "n_interface.h"


// This is a shorthand macro for constructing menus.
#define ITEMS(x) (sizeof(x)/sizeof(menuitem_t)), (x)


#define LINEHEIGHT       16
#define FONTBHEIGHT      20
#define SMALLLINEHEIGHT   8
#define STRINGHEIGHT     10
#define SLIDER_RANGE     10
#define SLIDER_WIDTH    (8*SLIDER_RANGE+6)
#define MAXSTRINGLENGTH  32

// forward references
extern Menu MainMenuDef, SinglePlayerDef, MultiPlayerDef, SetupPlayerDef,
  EpiDef, ClassDef, SkillDef, OptionsDef, VidModeDef, ControlDef, SoundDef,
  ReadDef2, ReadDef1, SaveDef, LoadDef, ControlDef2, GameOptionDef,
  NetOptionDef, VideoOptionsDef, OpenGLOptionDef, MouseOptionsDef, Mouse2OptionsDef, ServerOptionsDef;


static void M_DrawTextBox(int x, int y, int columns, int lines);


//===========================================================================
//  Menu item class
//===========================================================================

/// flags for the menu items
enum menuflag_t
{
  // Group 1, 4 bits: menuitem action (what we do when a key is pressed)
  IT_NONE           = 0x0, ///< no handling
  IT_SUBMENU        = 0x1, ///< <enter>: go to submenu
  IT_CALL           = 0x2, ///< <enter>: call routine with menuitem number as param (usually a complex submenu)
  IT_CONTROL        = 0x3, ///< <enter>, <backspace>: call specific functions with menuitem number as param
  IT_ARROWS         = 0x4, ///< <left>, <right>: call routine with 0 or 1 as param
  IT_KEYHANDLER     = 0x5, ///< <key>: call routine with <key> as param (except <esc>, which always exits)

  IT_CV_FLAG        = 0x8, ///< all cvar types must have this bit on
  IT_CV             = 0x8, ///< cv->str is printed next to name, <left>, <right>, <enter> to change
  IT_CV_NODRAW      = 0x9, ///< <left>, <right>, <enter> to change, nothing is drawn
  IT_CV_SLIDER      = 0xA, ///< instead of string value, a slider is drawn next to name
  IT_CV_TEXTBOX     = 0xB, ///< <enter>: opens a textbox
  IT_TYPE_MASK      = 0xF,

  // Group 2, 4 bits: display type
  IT_SPACE          = 0x00,  ///< big space (a hack, really...)
  IT_PATCH          = 0x10,  ///< a patch or a string with big font
  IT_BIGSLIDER      = 0x20,  ///< sound volume uses this, can only be used with a consvar
  IT_STRING         = 0x30,  ///< little string (spaced with 10)
  IT_CSTRING        = 0x40,  ///< very little string (spaced with 8)
  IT_DISPLAY_MASK   = 0xF0,

  // misc. flags
  IT_DY             = 0x0100, ///< alphakey is a y offset
  IT_TEXTBOX_IN_USE = 0x0200, // HACK
  IT_DISABLED       = 0x0400, ///< cannot be chosen
  IT_WHITE          = 0x0800, ///< use white colormap
  IT_GRAY           = 0x1000, ///< use gray colormap
  IT_COLORMAP_MASK  = IT_WHITE | IT_GRAY,

  // in short for some common uses
  IT_OFF_BIG     = IT_NONE       | IT_PATCH  | IT_DISABLED | IT_GRAY,
  IT_OFF         = IT_NONE       | IT_CSTRING | IT_DISABLED | IT_GRAY,
  IT_LINK        = IT_SUBMENU    | IT_STRING | IT_WHITE,
  IT_ACT         = IT_CALL       | IT_PATCH,
  IT_CONTROLSTR  = IT_CONTROL    | IT_CSTRING,
  IT_CVAR        = IT_CV         | IT_STRING,
  IT_TEXTBOX     = IT_CV_TEXTBOX | IT_STRING,
};


typedef void (*menufunc_t)(int choice);


/// \brief Describes a single menu item
struct menuitem_t
{
  short  flags; ///< bit flags

  char    *pic; ///< Texture name or NULL
  char   *text; ///< plain text, used when we have a valid font (e.g. FONTBxx lumps)

  union use_t
  {
    struct consvar_t *cvar;    // IT_CV_*
    class       Menu *submenu; // IT_SUBMENU
    menufunc_t        routine; // the rest
  } use;

  /// hotkey in menu OR y offset of the item 
  byte alphaKey;
};



//==========================================================================
//        Message box
//==========================================================================

/// \brief Message box used in the menu
class MsgBox
{
  friend class Menu;
  typedef bool (* eventhandler_t)(struct event_t *ev);

public:
  enum msgbox_t
  {
    NOTHING = 0,   ///< Text is displayed until the user does something.
    YESNO,         ///< routine is called with 'y' or 'n' as parameter
    EVENTHANDLER   ///< Full event handler.
  };

private:
  bool  active;
  int   x, y, rows, columns;
  char *text;
  msgbox_t type;
  union
  {
    menufunc_t     routine;
    eventhandler_t handler;
  } func;

public:

  MsgBox() { active = false; };

  // shows a message in a box
  void Set(const char *str, menufunc_t routine = NULL, msgbox_t btype = NOTHING);
  void SetHandler(const char *str, eventhandler_t routine);
  void SetText(const char *str);
  void Draw();
  void Input(event_t *ev);
  void Close();
  bool Active() const { return active; };
};

/// message box used by the menu
static MsgBox mbox;


void M_StartMessage(const char *str)
{
  Menu::Open(); // open menu
  mbox.Set(str, NULL, MsgBox::NOTHING);
}


void MsgBox::Set(const char *str, menufunc_t f, msgbox_t boxtype)
{
  type = boxtype;
  if (type != EVENTHANDLER)
    func.routine = f;
  else
    I_Error("MsgBox: Wrong messagebox type\n");
  SetText(str);
}


void MsgBox::SetHandler(const char *str, eventhandler_t f)
{
  type = EVENTHANDLER;
  func.handler = f;
  SetText(str);
}


#define MAXMSGLINELEN 256
void MsgBox::SetText(const char *str)
{
  if (text)
    Z_Free(text);

  text = Z_StrDup(str);
  active = true;

  // draw a textbox around the message
  // compute max rowlength and the number of lines

  rows = 1;
  columns = 0;

  int rowlength = 0;
  char *start;
  for (start = text; *start; start++)
    {
      if (*start == '\n')
        {
          if (rowlength > columns)
            columns = rowlength;
          rowlength = 0;
          rows++;
        }
      else
        rowlength ++;
    }
  // last row
  if (rowlength > columns)
    columns = rowlength;

  if (columns > MAXMSGLINELEN)
    {
      CONS_Printf("MsgBox::SetText: too long segment in %s\n", text);
      active = false;
      return;
    }

  int height = rows * hud_font->height;
  int width  = (columns + 1) * hud_font->width;

  x = (BASEVIDWIDTH - width)/2;
  y = (BASEVIDHEIGHT - height)/2;
}


void MsgBox::Draw()
{
  char *p;
  char *s = text;
  int slength;
  int ytemp = y;

  M_DrawTextBox(x-8, y-8, columns + 1, rows);

  for (p = text; *p; p++)
    {
      if (*p == '\n')
        {
          slength = p-s;

          *p = '\0';
          hud_font->DrawString((BASEVIDWIDTH - slength * hud_font->width)/2, ytemp, s);
          ytemp += hud_font->height;
          s = p+1;
          *p = '\n';
        }
    }

  // last line
  slength = p-s;
  hud_font->DrawString((BASEVIDWIDTH - slength * hud_font->width)/2, ytemp, s);
}


// Messagebox receives input (basically a keydown event)
void MsgBox::Input(event_t *ev)
{
  switch (type)
    {
    case YESNO:
      {
	if (ev->type != ev_keydown)
	  return;

	char ch = ev->data1;
	if (ch == ' ' || ch == 'n' || ch == 'y' || ch == KEY_ESCAPE)
	  {
	    func.routine(ch);
	    S_StartLocalAmbSound(sfx_menu_close);
	    Close();
	  }
      }
      break;

    case EVENTHANDLER:
      // an event handler mbox
      if (func.handler(ev))
	Close();
      break;

    default:
      Close();
    }
}


void MsgBox::Close()
{
  active = false;
}


// default message handler
void M_StopMessage(int choice)
{
  //Menu::SetupNextMenu(MessageDef.parent);
  mbox.Close();
  S_StartLocalAmbSound(sfx_menu_close);
}



//===========================================================================
//       Textbox
//===========================================================================

/// \brief For text input in the menu
class TextBox
{
  friend class Menu;

  bool        active;
  menuitem_t *item;
  consvar_t  *cv;
  string      text;
  menufunc_t  callback;
  int         parameter;

public:

  TextBox() { active = false; };
  bool Active() const { return active; };

  void Open(menuitem_t *m)
  {
    active = true;
    item = m;
    item->flags |= IT_TEXTBOX_IN_USE;
    cv = item->use.cvar;
    text = cv->str;
    callback = NULL;
  };

  // perhaps not as nice as it could be...
  void OpenCallback(menuitem_t *m, const char *str, menufunc_t cb, int param)
  {
    active = true;
    item = m;
    item->flags |= IT_TEXTBOX_IN_USE;
    cv = NULL;
    text = str;
    callback = cb;
    parameter = param;
  }

  void Input(unsigned char ch);
  inline const char *GetText() const { return text.c_str(); };
};


/// Textbox for the menu
static TextBox textbox;

/*
// TODO HexBox
void M_HandleFogColor(char key)
{
  int      i, l;
  char     temp[8];

  switch (key)
    {
    case KEY_BACKSPACE:
      S_StartLocalAmbSound(sfx_menu_adjust);
      strcpy(temp, cv_grfogcolor.str);
      strcpy(cv_grfogcolor.str, "000000");
      l = strlen(temp)-1;
      for (i=0; i<l; i++)
        cv_grfogcolor.str[i+6-l] = temp[i];
      break;

    default:
      if ((key >= '0' && key <= '9') ||
          (key >= 'a' && key <= 'f') ||
          (key >= 'A' && key <= 'F'))
        {
          S_StartLocalAmbSound(sfx_menu_adjust);
          strcpy(temp, cv_grfogcolor.str);
          strcpy(cv_grfogcolor.str, "000000");
          l = strlen(temp);
          for (i=0; i<l; i++)
            cv_grfogcolor.str[5-i] = temp[l-i];
          cv_grfogcolor.str[5] = key;
        }
      break;
    }
}
*/

void TextBox::Input(unsigned char ch)
{
  int n = text.length();
  switch (ch)
    {
    case KEY_BACKSPACE:
      if (n > 0)
	text.resize(n - 1);
      break;

    case KEY_ENTER:
      if (callback)
	{
	  callback(parameter);
	  callback = NULL;
	}
      else
	cv->Set(text.c_str());
      S_StartLocalAmbSound(sfx_menu_choose);
      // fallthru

    case KEY_ESCAPE:
      text.clear();
      item->flags &= ~IT_TEXTBOX_IN_USE;
      item = NULL;
      cv = NULL;
      active = false;
      break;

    default:
      if (ch >= 32 && ch <= 127 && n < MAXSTRINGLENGTH-1)
	{
	  text += ch;
	  S_StartLocalAmbSound(sfx_menu_adjust);
	}
      break;
    }
}



//===========================================================================
//  Misc. crap
//===========================================================================

void BeginGame(int episode, int skill, bool public_server);

extern bool devparm;
static vector<int> allowed_pawntypes;

// shhh... what am I doing... nooooo!
static int vidm_testingmode;
static int vidm_previousmode;
static int vidm_current;
static int vidm_nummodes;
static int vidm_column_size;



//===========================================================================
//  Drawing routines
//===========================================================================

/// Draws a menu slider (thermometer? thermostat?)
static void M_DrawThermo(int x, int y, consvar_t *cv)
{
  const char *leftlump, *rightlump, *centerlump[2], *cursorlump;
  bool raven = (game.mode >= gm_heretic);
  int xx = x;

  if (raven)
    {
      xx -= 32-8;
      leftlump      = "M_SLDLT";
      rightlump     = "M_SLDRT";
      centerlump[0] = "M_SLDMD1";
      centerlump[1] = "M_SLDMD2";
      cursorlump    = "M_SLDKB";
    }
  else
    {
      leftlump      = "M_THERML";
      rightlump     = "M_THERMR";
      centerlump[0] = "M_THERMM";
      centerlump[1] = "M_THERMM";
      cursorlump    = "M_THERMO";
    }
  Texture *p = tc.GetPtr(leftlump);
  p->Draw(xx, y, V_SCALE);

  xx += p->width - p->leftoffset;
  for (int i=0;i<16;i++)
    {
      tc.GetPtr(centerlump[i & 1])->Draw(xx, y, V_SCALE);
      xx += 8;
    }
  tc.GetPtr(rightlump)->Draw(xx,y,0 | V_SCALE);

  xx = (cv->value - cv->PossibleValue[0].value) * (15*8) /
    (cv->PossibleValue[1].value - cv->PossibleValue[0].value);

  tc.GetPtr(cursorlump)->Draw((x+8) + xx, raven ? y+7 : y, V_SCALE);
}


///  A smaller 'Thermo', with range given as percents (0-100)
static void M_DrawSlider(int x, int y, int range)
{
  if (range < 0)
    range = 0;
  if (range > 100)
    range = 100;

  tc.GetPtr("M_SLIDEL")->Draw(x-8, y, 0 | V_SCALE);

  for (int i=0 ; i<SLIDER_RANGE ; i++)
    tc.GetPtr("M_SLIDEM")->Draw(x+i*8, y, 0 | V_SCALE);

  tc.GetPtr("M_SLIDER")->Draw(x+SLIDER_RANGE*8, y, 0 | V_SCALE);

  // draw the slider cursor
  current_colormap = whitemap;
  tc.GetPtr("M_SLIDEC")->Draw(x + ((SLIDER_RANGE-1)*8*range)/100, y, V_SCALE | V_MAP);
}


/*
void M_DrawEmptyCell(Menu *menu, int item)
{
  tc.GetPtr("M_CELL1")->Draw(menu->x - 10, menu->y+item*LINEHEIGHT - 1, 0 | V_SCALE);
}

void M_DrawSelCell(Menu *menu, int item)
{
  tc.GetPtr("M_CELL2")->Draw(menu->x - 10, menu->y+item*LINEHEIGHT - 1, 0 | V_SCALE);
}
*/


///  Draw a textbox, like Quake does, because sometimes it's difficult
///  to read the text with all the stuff in the background...
static void M_DrawTextBox(int x, int y, int columns, int lines)
{
  int      cx, cy;
  int      n;
  int      step,boff;

  if (game.mode >= gm_heretic)
    {
      // humf.. border will stand if we do not adjust size ...
      x+=4;
      y+=4;
      lines = (lines+1)/2;
      columns = (columns+1)/2;
      step = 16;
      boff = 4; // borderoffset
    }
  else
    {
      step = 8;
      boff = 8;
    }

  // draw left side
  cx = x;
  cy = y;
  window_border[BRDR_TL]->Draw(cx, cy, 0 | V_SCALE);
  cy += boff;
  for (n = 0; n < lines; n++)
    {
      window_border[BRDR_L]->Draw(cx, cy, 0 | V_SCALE);
      cy += step;
    }
  window_border[BRDR_BL]->Draw(cx, cy, 0 | V_SCALE);

  // draw middle
  window_background->DrawFill(x+boff, y+boff, columns*step, lines*step);

  cx += boff;
  cy = y;
  while (columns > 0)
    {
      window_border[BRDR_T]->Draw(cx, cy, 0 | V_SCALE);
      window_border[BRDR_B]->Draw(cx, y+boff+lines*step, 0 | V_SCALE);
      columns--;
      cx += step;
    }

  // draw right side
  cy = y;
  window_border[BRDR_TR]->Draw(cx, cy, 0 | V_SCALE);
  cy += boff;
  for (n = 0; n < lines; n++)
    {
      window_border[BRDR_R]->Draw(cx, cy, 0 | V_SCALE);
      cy += step;
    }
  window_border[BRDR_BR]->Draw(cx, cy, 0 | V_SCALE);
}


/// Write a string centered using the hud_font
static void M_CenterText(int y, const char *str)
{
  int x = (BASEVIDWIDTH - hud_font->StringWidth(str)) >> 1;
  hud_font->DrawString(x, y, str);
}



void Menu::DrawTitle()
{
  if (font && title)
    {
      int xtitle = (BASEVIDWIDTH - font->StringWidth(title)) / 2;
      int ytitle = (y - font->StringHeight(title)) / 2;
      if (xtitle < 0) xtitle=0;
      if (ytitle < 0) ytitle=0;

      font->DrawString(xtitle, ytitle, title);
    }
  else if (titlepic)
    {
      Texture *p = tc.GetPtr(titlepic);

      //SoM: 4/7/2000: Old code was causing problems with large graphics.
      //        int xtitle = (vid.width / 2) - (p->width / 2);
      //        int ytitle = (y-p->height)/2;
      int xtitle = 94;
      int ytitle = 2;
      //int xtitle = (BASEVIDWIDTH-p->width)/2;
      //int ytitle = (y-p->height)/2;

      if (xtitle < 0) xtitle=0;
      if (ytitle < 0) ytitle=0;
      p->Draw(xtitle, ytitle, V_SCALE);
    }
}



void Menu::DrawMenu()
{
  int cursory = 0;
  int dy = y; // y for drawing

  DrawTitle();

  for (int i=0; i < numitems; i++)
    {
      // extra y spacing in alphakey (HACK)
      if (items[i].flags & IT_DY)
	dy += items[i].alphaKey;

      // colormap?
      int flags = V_SCALE;
      if (items[i].flags & IT_COLORMAP_MASK)
	flags |= V_MAP;
      if (items[i].flags & IT_WHITE)
	current_colormap = whitemap;
      else if (items[i].flags & IT_GRAY)
	current_colormap = graymap;

      if (i == itemOn)
        cursory = dy;

      int h = LINEHEIGHT; // delta dy

      switch (items[i].flags & IT_DISPLAY_MASK)
        {
	  // first the "tall menuitems":
        case IT_SPACE:
          break;

        case IT_PATCH:
          if (font && items[i].text)
            {
              font->DrawString(x, dy, items[i].text, flags);
              h = FONTBHEIGHT;
            }
          else if (items[i].pic && items[i].pic[0])
	    tc.GetPtr(items[i].pic)->Draw(x, dy, flags);
          break;

        case IT_BIGSLIDER:
          M_DrawThermo(x, dy, items[i].use.cvar);
          break;

	  // then the "short" ones:
        case IT_STRING:
	  hud_font->DrawString(x, dy, items[i].text, flags);
          h = STRINGHEIGHT;
          break;

        case IT_CSTRING:
	  hud_font->DrawString(x, dy, items[i].text, flags);
	  h = SMALLLINEHEIGHT;
          break;
	}

      // CVar item handling (also draw the value)
      if (items[i].flags & IT_CV_FLAG)
	{
	  consvar_t *cv = items[i].use.cvar;
	  switch (items[i].flags & IT_TYPE_MASK)
	    {
	    case IT_CV_SLIDER:
	      M_DrawSlider(BASEVIDWIDTH-x-SLIDER_WIDTH, dy,
			   ((cv->value - cv->PossibleValue[0].value) * 100 /
			    (cv->PossibleValue[1].value - cv->PossibleValue[0].value)));
	      break;

	    case IT_CV_TEXTBOX:
	      M_DrawTextBox(x, dy + 4, MAXSTRINGLENGTH, 1);
	      if (items[i].flags & IT_TEXTBOX_IN_USE)
		{
		  const char *t = textbox.GetText();
		  hud_font->DrawString(x+8, dy+12, t);
		  if (AnimCount < 4)
		    hud_font->DrawCharacter(x+8+hud_font->StringWidth(t), dy + 12, '_' | 0x80, V_SCALE);
		}
	      else
		hud_font->DrawString(x+8, dy+12, cv->str);
	      h = 26;
	      break;

	    case IT_CV_NODRAW:
	      break;

	    default:
	      hud_font->DrawString(BASEVIDWIDTH-x-hud_font->StringWidth(cv->str), dy, cv->str, V_WHITEMAP | V_SCALE);
	      break;
	    }
	}

      dy += h;
    }

  // draw the skull cursor (or a blinking star)
  if ((items[itemOn].flags & IT_DISPLAY_MASK) < IT_STRING)
    pointer[which_pointer]->Draw(x-32, cursory-5, V_SCALE);
  else if (AnimCount<4 * NEWTICRATERATIO)  //blink cursor
    hud_font->DrawCharacter(x - 10, cursory, '*' | 0x80, V_SCALE);
}




//===========================================================================
//                               Quitting
//===========================================================================

int quitsounds[8] =
{
  sfx_pldeth,
  sfx_dmpain,
  sfx_popain,
  sfx_gib,
  sfx_teleport,
  sfx_posit1,
  sfx_posit3,
  sfx_sgtatk
};

int quitsounds2[8] =
{
  sfx_vilact,
  sfx_powerup,
  sfx_boscub,
  sfx_gib,
  sfx_skeswg,
  sfx_kntdth,
  sfx_bspact,
  sfx_sgtatk
};


void M_QuitResponse(int ch)
{
  if (ch != 'y')
    return;

  if (!game.netgame)
    {
      //added:12-02-98: quitsounds are much more fun than quisounds2
      //if (game.mode == gm_doom2)
      //    S_StartLocalAmbSound(quitsounds2[(gametic>>2)&7]);
      //else
      S_StartLocalAmbSound(quitsounds[(game.tic>>2)&7]);

      tic_t time = I_GetTics() + TICRATE*2;
      while (time > I_GetTics()) ;
    }
  I_Quit();
}


void M_QuitDOOM(int choice)
{
  // We pick index 0 which is language sensitive,
  //  or one at random, between 1 and maximum number.
  static char s[200];
  sprintf(s, text[TXT_QUIT_OS_Y], text[TXT_QUITMSG + (game.tic % NUM_QUITMESSAGES)]);
  mbox.Set(s, M_QuitResponse, MsgBox::YESNO);
}



//===========================================================================
//                                MAIN MENU
//===========================================================================

static menuitem_t Main_MI[]=
{
  {IT_SUBMENU | IT_PATCH, "M_SINGLE", "SINGLE PLAYER", {(consvar_t *)&SinglePlayerDef} ,'s'},
  {IT_SUBMENU | IT_PATCH, "M_MULTI" , "MULTIPLAYER", {(consvar_t *)&MultiPlayerDef} ,'m'},
  {IT_SUBMENU | IT_PATCH, "M_OPTION", "OPTIONS", {(consvar_t *)&OptionsDef}  ,'o'},
  {IT_SUBMENU | IT_PATCH, "M_RDTHIS", "INFO", {(consvar_t *)&ReadDef1}     ,'r'},
  {IT_ACT, "M_QUITG" , "QUIT GAME", {(consvar_t *)M_QuitDOOM} ,'q'}
};

// The main menu.
Menu MainMenuDef("M_DOOM", "Main menu", NULL, ITEMS(Main_MI), 97, 64);


void Menu::HereticMainMenuDrawer()
{
  int frame = (NowTic/3)%18;

  tc.GetPtrNum(SkullBaseLump+(17-frame))->Draw(40, 10, 0 | V_SCALE);
  tc.GetPtrNum(SkullBaseLump+frame)->Draw(232, 10, 0 | V_SCALE);

  DrawMenu();
}

void Menu::HexenMainMenuDrawer()
{
  int frame = (NowTic/5) % 7;

  tc.GetPtrNum(SkullBaseLump+(frame+2)%7)->Draw(37, 80,  0 | V_SCALE);
  tc.GetPtrNum(SkullBaseLump+frame)->Draw(278, 80, 0 | V_SCALE);

  DrawMenu();
}



//===========================================================================
//                              END GAME
//===========================================================================

void M_EndGameResponse(int ch)
{
  if (ch != 'y')
    return;

  //currentMenu->lastOn = itemOn;
  Menu::Close(true);
  COM_BufAddText("reset\n");
}

void M_EndGame(int choice)
{
  choice = 0;
  /*
  if (demorecording)
    {
      S_StartLocalAmbSound(sfx_menu_fail);
      return;
    }

    if (game.netgame)
    {
    mbox.Set(NETEND,NULL,MsgBox::NOTHING);
    return;
    }
  */
  mbox.Set(ENDGAME,M_EndGameResponse,MsgBox::YESNO);
}


//===========================================================================
//                        SINGLE PLAYER MENU
//===========================================================================

void M_NewGame(int choice);
void M_LoadGame(int choice);
void M_SaveGame(int choice);

enum single_e
{
  MI_newgame = 0,
  MI_mapinfogame = 1,
};

static menuitem_t SinglePlayer_MI[] =
{
  {IT_ACT, "M_NGAME" , "NEW GAME" ,{(consvar_t *)M_NewGame} ,'n'},
  {IT_ACT, NULL, "NEW MAPINFO GAME", {(consvar_t *)M_NewGame} ,'m'},
  {IT_ACT, "M_LOADG" , "LOAD GAME",{(consvar_t *)M_LoadGame},'l'},
  {IT_ACT, "M_SAVEG" , "SAVE GAME",{(consvar_t *)M_SaveGame},'s'},
  {IT_ACT, "M_ENDGAM", "END GAME" ,{(consvar_t *)M_EndGame},'e'}
};

Menu SinglePlayerDef("M_SINGLE", "Single Player", &MainMenuDef, ITEMS(SinglePlayer_MI), 97, 64);


const  char *ALREADYPLAYING = "You are already playing\n\nLeave this game first\n";
static int epi;


void M_NewGame(int choice)
{
  if (game.Playing())
    {
      mbox.Set(ALREADYPLAYING,NULL,MsgBox::NOTHING);
      return;
    }

  if (choice == MI_mapinfogame)
    {
      // use MAPINFO
      epi = -1;
      if (game.mode == gm_hexen)
        Menu::SetupNextMenu(&ClassDef);
      else
        Menu::SetupNextMenu(&SkillDef);
    }
  else if (game.mode == gm_doom2)
    {
      epi = 1;
      Menu::SetupNextMenu(&SkillDef);
    }
  else
    Menu::SetupNextMenu(&EpiDef);

  // TODO build EpiDef items using clustermap!
}


//===========================================================================
//                        CLASS SELECTION MENU
//===========================================================================

extern menuitem_t Skill_MI[];

void M_Class(int choice)
{
  switch (choice)
    {
    case 0:
      //SkillDef.x = 120;
      Skill_MI[0].text = "SQUIRE";
      Skill_MI[1].text = "KNIGHT";
      Skill_MI[2].text = "WARRIOR";
      Skill_MI[3].text = "BERSERKER";
      Skill_MI[4].text = "TITAN";
      break;
    case 1:
      //SkillDef.x = 116;
      Skill_MI[0].text = "ALTAR BOY";
      Skill_MI[1].text = "ACOLYTE";
      Skill_MI[2].text = "PRIEST";
      Skill_MI[3].text = "CARDINAL";
      Skill_MI[4].text = "POPE";
      break;
    case 2:
      //SkillDef.x = 112;
      Skill_MI[0].text = "APPRENTICE";
      Skill_MI[1].text = "ENCHANTER";
      Skill_MI[2].text = "SORCERER";
      Skill_MI[3].text = "WARLOCK";
      Skill_MI[4].text = "ARCHIMAGE";
      break;
    }

  localplayer.ptype = choice + 37;

  Menu::SetupNextMenu(&SkillDef);
}

static menuitem_t Class_MI[] =
{
  {IT_ACT, NULL, "FIGHTER", {(consvar_t *)M_Class}, 'f'},
  {IT_ACT, NULL, "CLERIC", {(consvar_t *)M_Class}, 'c'},
  {IT_ACT, NULL, "MAGE", {(consvar_t *)M_Class}, 'm'}
};

Menu ClassDef(NULL, "Choose class:", &MainMenuDef, ITEMS(Class_MI), 66, 66,
	      0, &Menu::DrawClass);

void Menu::DrawClass()
{
  static char *boxLumpName[3] = {"M_FBOX", "M_CBOX", "M_MBOX"};
  static char *walkLumpName[3] = {"M_FWALK1", "M_CWALK1", "M_MWALK1"};

  int cl = itemOn;
  int p = fc.GetNumForName(walkLumpName[cl]) + ((NowTic/5)& 3);

  tc.GetPtr(boxLumpName[cl])->Draw(174, 50, 0 | V_SCALE);
  tc.GetPtrNum(p)->Draw(174+24, 50+12, 0 | V_SCALE);

  DrawMenu();
}

//===========================================================================
//                       EPISODE SELECTION MENU
//===========================================================================

void M_Episode(int choice);

static menuitem_t DoomEp_MI[]=
{
  {IT_ACT, "M_EPI1", "Knee-Deep in the Dead", {(consvar_t *)M_Episode},'k'},
  {IT_ACT, "M_EPI2", "The Shores of Hell"   , {(consvar_t *)M_Episode},'t'},
  {IT_ACT, "M_EPI3", "Inferno"              , {(consvar_t *)M_Episode},'i'},
  {IT_ACT, "M_EPI4", "Thy Flesh consumed"   , {(consvar_t *)M_Episode},'t'},
  {IT_ACT, "M_EPI5", "Episode 5"            , {(consvar_t *)M_Episode},'t'},
};

static menuitem_t HereticEp_MI[]=
{
  {IT_ACT, "M_EPI1", "City of the Damned",   {(consvar_t *)M_Episode},'c'},
  {IT_ACT, "M_EPI2", "Hell's Maw",           {(consvar_t *)M_Episode},'h'},
  {IT_ACT, "M_EPI3", "The Dome of D'Sparil", {(consvar_t *)M_Episode},'t'},
  {IT_ACT, "M_EPI4", "The Ossuary",          {(consvar_t *)M_Episode},'t'},
  {IT_ACT, "M_EPI5", "The Stagnant Demesne", {(consvar_t *)M_Episode},'t'},
};

Menu EpiDef("M_EPISOD", "Which Episode?", &MainMenuDef, ITEMS(DoomEp_MI), 48, 63);


void M_Episode(int choice)
{
  if ((game.mode == gm_doom1s) && choice)
    {
      Menu::SetupNextMenu(&ReadDef1);
      mbox.Set(SWSTRING,NULL,MsgBox::NOTHING);
      return;
    }

  // Yet another hack...
  if ((game.mode == gm_doom1) && (choice > 2))
    {
      I_Error("M_Episode: 4th episode requires Ultimate DOOM\n");
      choice = 0;
    }

  epi = choice+1;
  Menu::SetupNextMenu(&SkillDef);
}


//===========================================================================
//                             SKILL MENU
//===========================================================================

void M_ChooseSkill(int choice);

enum newgame_e
{
  MI_nightmare = 4,
};

menuitem_t Skill_MI[]=
{
  {IT_ACT, "M_JKILL", "I'm too young to die.",{(consvar_t *)M_ChooseSkill}, 'i'},
  {IT_ACT, "M_ROUGH", "Hey, not too rough."  ,{(consvar_t *)M_ChooseSkill}, 'h'},
  {IT_ACT, "M_HURT" , "Hurt me plenty."      ,{(consvar_t *)M_ChooseSkill}, 'h'},
  {IT_ACT, "M_ULTRA", "Ultra-Violence"       ,{(consvar_t *)M_ChooseSkill}, 'u'},
  {IT_ACT, "M_NMARE", "Nightmare!"           ,{(consvar_t *)M_ChooseSkill}, 'n'}
};

static menuitem_t HereticSkill_MI[]=
{
  {IT_ACT, "M_JKILL", "THOU NEEDETH A WET-NURSE", {(consvar_t *)M_ChooseSkill}, 't'},
  {IT_ACT, "M_ROUGH", "YELLOWBELLIES-R-US",       {(consvar_t *)M_ChooseSkill}, 'y'},
  {IT_ACT, "M_HURT" , "BRINGEST THEM ONETH",      {(consvar_t *)M_ChooseSkill}, 'b'},
  {IT_ACT, "M_ULTRA", "THOU ART A SMITE-MEISTER", {(consvar_t *)M_ChooseSkill}, 't'},
  {IT_ACT, "M_NMARE", "BLACK PLAGUE POSSESSES THEE", {(consvar_t *)M_ChooseSkill}, 'b'}
};

Menu SkillDef("M_SKILL", //"M_NEWG"
	      "Choose skill", &MainMenuDef, ITEMS(Skill_MI), 48, 63, 3);


void M_VerifyNightmare(int ch)
{
  if (ch != 'y') return;

  if (epi < 0)
    COM_BufAddText(va("newgame MAPINFO local 1 %d\n", sk_nightmare));
  else
    BeginGame(epi, sk_nightmare, false);

  Menu::Close(true);
}


void M_ChooseSkill(int choice)
{
  if (localplayer.ptype < 0)
    localplayer.ptype = allowed_pawntypes[0];

  if (localplayer2.ptype < 0)
    localplayer2.ptype = allowed_pawntypes[0];

  if (choice == MI_nightmare)
    {
      mbox.Set(NIGHTMARE, M_VerifyNightmare, MsgBox::YESNO);
      return;
    }

  if (epi < 0)
    COM_BufAddText(va("newgame MAPINFO local 1 %d\n", choice));
  else
    BeginGame(epi, choice, false);

  Menu::Close(true);
}


//===========================================================================
//                            LOAD GAME MENU
//===========================================================================

/// User wants to load this game
void M_LoadSelect(int choice)
{
  game.LoadGame(choice);
  Menu::Close(true);
}


static menuitem_t LoadGame_MI[]=
{
  {IT_CALL | IT_SPACE, NULL, "", {(consvar_t *)M_LoadSelect},'1'},
  {IT_CALL | IT_SPACE, NULL, "", {(consvar_t *)M_LoadSelect},'2'},
  {IT_CALL | IT_SPACE, NULL, "", {(consvar_t *)M_LoadSelect},'3'},
  {IT_CALL | IT_SPACE, NULL, "", {(consvar_t *)M_LoadSelect},'4'},
  {IT_CALL | IT_SPACE, NULL, "", {(consvar_t *)M_LoadSelect},'5'},
  {IT_CALL | IT_SPACE, NULL, "", {(consvar_t *)M_LoadSelect},'6'}
};

Menu LoadDef("M_LOADG", "Load Game", &MainMenuDef, ITEMS(LoadGame_MI), 80, 54,
	     0, &Menu::DrawLoad);


/// Draw border for the savegame description
static void M_DrawSaveLoadBorder(int x,int y)
{
  if (game.mode >= gm_heretic)
    tc.GetPtr("M_FSLOT")->Draw(x-8, y-4, 0 | V_SCALE);
  else
    {
      tc.GetPtr("M_LSLEFT")->Draw(x-8,y+7,0 | V_SCALE);

      for (int i = 0;i < 24;i++)
        {
          tc.GetPtr("M_LSCNTR")->Draw(x,y+7,0 | V_SCALE);
          x += 8;
        }

      tc.GetPtr("M_LSRGHT")->Draw(x,y+7,0 | V_SCALE);
    }
}


static char savegamestrings[10][SAVEGAME_DESC_SIZE];


void Menu::DrawLoad()
{
  DrawMenu();

  for (int i=0; i < 6; i++)
    {
      M_DrawSaveLoadBorder(x, y+LINEHEIGHT*i);
      hud_font->DrawString(x, y+LINEHEIGHT*i, savegamestrings[i]);
    }
}


//  read the strings from the savegame files
//  and put it in savegamestrings global variable
void M_ReadSaveStrings()
{
  extern char savegamename[200];
  char    name[256];

  for (int i=0; i < 6;i++)
    {
      sprintf(name, savegamename, i);

      int handle = open(name, O_RDONLY | 0, 0666);
      if (handle == -1)
        {
          strcpy(&savegamestrings[i][0], EMPTYSTRING);
          LoadGame_MI[i].flags = IT_NONE | IT_SPACE;
          continue;
        }
      lseek(handle, size_t(&savegame_header_t::description), SEEK_SET);
      read(handle, &savegamestrings[i], SAVEGAME_DESC_SIZE);
      close(handle);
      LoadGame_MI[i].flags = IT_CALL | IT_SPACE;
    }
}

/// Selected from DOOM menu
void M_LoadGame(int choice)
{
  if (!game.server)
    {
      mbox.Set("You are not the server");
      return;
    }

  Menu::SetupNextMenu(&LoadDef);
  M_ReadSaveStrings();
}



//===========================================================================
//                            SAVE GAME MENU
//===========================================================================

static int quickSaveSlot = -1; // -1 = no quicksave slot picked!

/// also used by quicksave
static void M_DoSave(int slot)
{
  game.SaveGame(slot, savegamestrings[slot]);
  Menu::Close(true);

  // PICK QUICKSAVE SLOT YET?
  if (quickSaveSlot < 0)
    quickSaveSlot = slot;
}

/// callback for the textbox, hackish
static void M_SaveCallback(int slot)
{
  strncpy(savegamestrings[slot], textbox.GetText(), SAVEGAME_DESC_SIZE);
  M_DoSave(slot);
}

static void M_SaveSelect(int choice);

static menuitem_t Save_MI[]=
{
  {IT_CALL | IT_SPACE, NULL, "", {(consvar_t *)M_SaveSelect},'1'},
  {IT_CALL | IT_SPACE, NULL, "", {(consvar_t *)M_SaveSelect},'2'},
  {IT_CALL | IT_SPACE, NULL, "", {(consvar_t *)M_SaveSelect},'3'},
  {IT_CALL | IT_SPACE, NULL, "", {(consvar_t *)M_SaveSelect},'4'},
  {IT_CALL | IT_SPACE, NULL, "", {(consvar_t *)M_SaveSelect},'5'},
  {IT_CALL | IT_SPACE, NULL, "", {(consvar_t *)M_SaveSelect},'6'}
};

Menu  SaveDef("M_SAVEG", "Save Game", &MainMenuDef, ITEMS(Save_MI), 80, 54,
	      0, &Menu::DrawSave);


/// User wants to save. Open a textbox for the save description string.
static void M_SaveSelect(int choice)
{
  const char *p = savegamestrings[choice];
  if (!strcmp(p, text[TXT_EMPTYSTRING]))
    p = "";

  // we are going to be intercepting all chars
  textbox.OpenCallback(&Save_MI[choice], p, M_SaveCallback, choice);
}


void Menu::DrawSave()
{
  DrawMenu();

  for (int i=0; i < 6; i++)
    {
      M_DrawSaveLoadBorder(x, y+LINEHEIGHT*i);

      if (items[i].flags & IT_TEXTBOX_IN_USE)
	{
	  const char *p = textbox.GetText();
	  hud_font->DrawString(x, y+LINEHEIGHT*i, p);
	  hud_font->DrawString(x+hud_font->StringWidth(p), y+LINEHEIGHT*i, "_");
	}
      else
	hud_font->DrawString(x, y+LINEHEIGHT*i, savegamestrings[i]);
    }
}


/// Selected from DOOM menu
void M_SaveGame(int choice)
{
  /*
  if (demorecording)
    {
      mbox.Set("You can't save while recording demos\n\nPress a key\n",NULL,MsgBox::NOTHING);
      return;
    }

  if (demoplayback)
    {
      mbox.Set(SAVEDEAD,NULL,MsgBox::NOTHING);
      return;
    }
  */

  if (game.state != GameInfo::GS_LEVEL)
    return;

  if (!game.server)
    {
      mbox.Set("You are not the server");
      return;
    }

  Menu::SetupNextMenu(&SaveDef);
  M_ReadSaveStrings();
}



//===========================================================================
//                           MULTIPLAYER MENU
//===========================================================================

void M_StartServerMenu(int choice);
void M_ConnectMenu(int choice);
void M_Splitscreen(int choice);
void M_SetupPlayer(int choice);
void M_NetOption(int choice);

enum multiplayer_e
{
  MI_setupplayer1 = 3,
  MI_setupplayer2 = 4,
};


static menuitem_t MultiPlayer_MI[] =
{
  {IT_ACT, "M_STSERV", "CREATE SERVER", {(consvar_t *)M_StartServerMenu} ,'a'},
  {IT_ACT, "M_CONNEC", "CONNECT SERVER", {(consvar_t *)M_ConnectMenu} ,'c'},
  {IT_ACT, "M_2PLAYR", "TWO PLAYER GAME", {(consvar_t *)M_Splitscreen} ,'n'},
  {IT_ACT, "M_SETUPA", "SETUP PLAYER 1",{(consvar_t *)M_SetupPlayer} ,'s'},
  {IT_ACT, "M_SETUPB", "SETUP PLAYER 2",{(consvar_t *)M_SetupPlayer} ,'t'},
  {IT_ACT, "M_OPTION", "OPTIONS",{(consvar_t *)M_NetOption} ,'o'},
  {IT_ACT, "M_ENDGAM", "END GAME",{(consvar_t *)M_EndGame} ,'e'}
};

Menu MultiPlayerDef("M_MULTI", "Multiplayer", &MainMenuDef, ITEMS(MultiPlayer_MI), 85, 40);


void M_Splitscreen(int choice)
{
  M_StartServerMenu(1);
}


//===========================================================================
//                             OPTIONS MENU
//===========================================================================

void M_GameOption(int choice)
{
  if (!game.server)
    {
      mbox.Set("You are not the server\nYou can't change the options\n");
      return;
    }
  Menu::SetupNextMenu(&GameOptionDef);
}


void M_SetupControlsMenu(int choice);


// note: alphaKey member is the y offset
static menuitem_t Options_MI[] =
{
  {IT_CVAR, NULL, "Messages:"       ,{&cv_showmessages}    ,0},
  {IT_CVAR, NULL, "Always Run"      ,{&cv_autorun}         ,0},
  {IT_CVAR, NULL, "Crosshair"       ,{&cv_crosshair}       ,0},
//{IT_CVAR, NULL, "Crosshair scale" ,{&cv_crosshairscale}  ,0},
  {IT_CVAR, NULL, "Autoaim"         ,{&cv_autoaim}         ,0},
  {IT_CVAR, NULL, "Control per key" ,{&cv_controlperkey}   ,0},

  {IT_LINK | IT_DY, NULL, "Server Options...",{(consvar_t *)&ServerOptionsDef}, 20},
  {IT_CALL | IT_STRING | IT_WHITE, NULL, "Game Options..."  ,{(consvar_t *)M_GameOption}       ,0},
  {IT_LINK, NULL, "Sound Options..."  ,{(consvar_t *)&SoundDef}          ,0},
  {IT_LINK, NULL, "Video Options..." ,{(consvar_t *)&VideoOptionsDef}   ,0},
  {IT_LINK, NULL, "Mouse Options..." ,{(consvar_t *)&MouseOptionsDef}   ,0},
  {IT_CALL | IT_STRING | IT_WHITE, NULL, "Setup Controls...",{(consvar_t *)M_SetupControlsMenu},0}
};

Menu OptionsDef("M_OPTTTL", "OPTIONS", &MainMenuDef, ITEMS(Options_MI), 60, 40);



//===========================================================================
//                        Start Server Menu
//===========================================================================

CV_PossibleValue_t skill_cons_t[] = {{sk_baby, "I'm too young to die"},
                                     {sk_easy, "Hey, not too rough"},
                                     {sk_medium, "Hurt me plenty"},
                                     {sk_hard, "Ultra violence"},
                                     {sk_nightmare, "Nightmare!" },
                                     {0,NULL}};

CV_PossibleValue_t map_cons_t[] = {{ 1,"map01"}, { 2,"map02"}, { 3,"map03"},
                                   { 4,"map04"}, { 5,"map05"}, { 6,"map06"},
                                   { 7,"map07"}, { 8,"map08"}, { 9,"map09"},
                                   {10,"map10"}, {11,"map11"}, {12,"map12"},
                                   {13,"map13"}, {14,"map14"}, {15,"map15"},
                                   {16,"map16"}, {17,"map17"}, {18,"map18"},
                                   {19,"map19"}, {20,"map20"}, {21,"map21"},
                                   {22,"map22"}, {23,"map23"}, {24,"map24"},
                                   {25,"map25"}, {26,"map26"}, {27,"map27"},
                                   {28,"map28"}, {29,"map29"}, {30,"map30"},
                                   {31,"map31"}, {32,"map32"}, {0,NULL}};

CV_PossibleValue_t exmy_cons_t[] ={{1,"e1m1"}, {2,"e1m2"}, {3,"e1m3"},
                                   {4,"e1m4"}, {5,"e1m5"}, {6,"e1m6"},
                                   {7,"e1m7"}, {8,"e1m8"}, {9,"e1m9"},
                                   {11,"e2m1"}, {12,"e2m2"}, {13,"e2m3"},
                                   {14,"e2m4"}, {15,"e2m5"}, {16,"e2m6"},
                                   {17,"e2m7"}, {18,"e2m8"}, {19,"e2m9"},
                                   {21,"e3m1"}, {22,"e3m2"}, {23,"e3m3"},
                                   {24,"e3m4"}, {25,"e3m5"}, {26,"e3m6"},
                                   {27,"e3m7"}, {28,"e3m8"}, {29,"e3m9"},
                                   {31,"e4m1"}, {32,"e4m2"}, {33,"e4m3"},
                                   {34,"e4m4"}, {35,"e4m5"}, {36,"e4m6"},
                                   {37,"e4m7"}, {38,"e4m8"}, {39,"e4m9"},
                                   {41,"e5m1"}, {42,"e5m2"}, {43,"e5m3"},
                                   {44,"e5m4"}, {45,"e5m5"}, {46,"e5m6"},
                                   {47,"e5m7"}, {48,"e5m8"}, {49,"e5m9"},
				   {0,NULL}};

// TODO replace these using a keyhandler which picks the map names from game.mapinfo
// ... but first we must choose a mapinfo lump! which one?
static consvar_t cv_menu_skill     = {"skill"       , "3", CV_HIDDEN, skill_cons_t};
static consvar_t cv_menu_startmap  = {"starting map", "1", CV_HIDDEN, map_cons_t};

void M_StartServer(int choice)
{
  BeginGame(1 + cv_menu_startmap.value / 10, cv_menu_skill.value, true);
  // TODO starting map...
  Menu::Close(true);
}

static menuitem_t  Server_MI[] =
{
  {IT_CVAR, NULL, "Map",       {&cv_menu_startmap} ,0},
  {IT_CVAR, NULL, "Skill",     {&cv_menu_skill}, 0},
  {IT_CVAR, NULL, "Public Server", {&cv_publicserver}, 0},
  {IT_TEXTBOX, NULL, "Server Name",{&cv_servername}, 0},
  {IT_LINK, NULL, "Server Options...",{(consvar_t *)&ServerOptionsDef},0},
  {IT_CALL | IT_STRING | IT_WHITE, NULL, "Game Options..." ,{(consvar_t *)M_GameOption},0},
  {IT_CALL | IT_STRING | IT_WHITE, NULL, "Start", {(consvar_t *)M_StartServer},120}
};

Menu Serverdef("M_STSERV", "Start Server", &MultiPlayerDef, ITEMS(Server_MI), 27, 40);


void M_StartServerMenu(int choice)
{
  if (game.Playing())
    {
      mbox.Set(ALREADYPLAYING,NULL,MsgBox::NOTHING);
      return;
    }

  if (choice == 1)
    cv_splitscreen.Set("1");

  Menu::SetupNextMenu(&Serverdef);
}


//===========================================================================
//                             Connect Menu
//===========================================================================

CV_PossibleValue_t serversearch_cons_t[] = {{0,"LAN"}, {1,"Internet"}, {0,NULL}};
static consvar_t cv_menu_serversearch = {"serversearch", "0", CV_HIDDEN, serversearch_cons_t};

void M_Connect(int choice)
{
  // do not call menuexitfunc
  Menu::Close(false);
  game.net->CL_Connect(game.net->serverlist[choice - 3]->addr);
}


void M_Refresh(int choice)
{
  //  CL_UpdateServerList();
}

static menuitem_t Connect_MI[] =
{
  {IT_CVAR , NULL, "Search the",{(consvar_t *)&cv_menu_serversearch}, 0},
  {IT_CALL | IT_STRING, NULL, "Refresh",  {(consvar_t *)M_Refresh},0},
  {IT_NONE | IT_STRING | IT_WHITE, NULL, "Server Name                    ping   plys type", {NULL}, 0},
  {IT_NONE | IT_STRING, NULL, "",{(consvar_t *)M_Connect}   ,0},
  {IT_NONE | IT_STRING, NULL, "",{(consvar_t *)M_Connect}   ,0},
  {IT_NONE | IT_STRING, NULL, "",{(consvar_t *)M_Connect}   ,0},
  {IT_NONE | IT_STRING, NULL, "",{(consvar_t *)M_Connect}   ,0},
  {IT_NONE | IT_STRING, NULL, "",{(consvar_t *)M_Connect}   ,0},
  {IT_NONE | IT_STRING, NULL, "",{(consvar_t *)M_Connect}   ,0},
  {IT_NONE | IT_STRING, NULL, "",{(consvar_t *)M_Connect}   ,0},
  {IT_NONE | IT_STRING, NULL, "",{(consvar_t *)M_Connect}   ,0},
  {IT_NONE | IT_STRING, NULL, "",{(consvar_t *)M_Connect}   ,0},
  {IT_NONE | IT_STRING, NULL, "",{(consvar_t *)M_Connect}   ,0},
  {IT_NONE | IT_STRING, NULL, "",{(consvar_t *)M_Connect}   ,0},
  {IT_NONE | IT_STRING, NULL, "",{(consvar_t *)M_Connect}   ,0},
  {IT_NONE | IT_STRING, NULL, "",{(consvar_t *)M_Connect}   ,0},
};


void serverinfo_t::Draw(int x, int y)
{
  hud_font->DrawString(x, y, name.c_str());
  char *p = va("%d", ping);
  hud_font->DrawString(x + 184 - hud_font->StringWidth(p), y, p);
  p = va("%d/%d", players, maxplayers);
  hud_font->DrawString(x + 250 - hud_font->StringWidth(p), y, p);
  p = va("%s", gt_name.c_str());
  hud_font->DrawString(x + 270 - hud_font->StringWidth(p), y, p);
}

void Menu::DrawConnect()
{
  int i = 0, n = game.net->serverlist.size();
  int cy = y + 3*STRINGHEIGHT;
  int m = numitems - 3;

  n = min(n, m);

  if (n <= 0)
    hud_font->DrawString(x, cy, "No servers found");
  else for (; i<n; i++)
    {
      game.net->serverlist[i]->Draw(x, cy + i*STRINGHEIGHT);
      Connect_MI[i+3].flags =  IT_CALL | IT_STRING;
    }

  for (; i<m; i++)
    Connect_MI[i+3].flags = IT_NONE | IT_STRING;


  DrawMenu();
}

bool M_CancelConnect()
{
  game.CL_Reset();
  return true;
}

Menu Connectdef("M_CONNEC", "Connect Server", &MultiPlayerDef, ITEMS(Connect_MI), 27, 40,
		0, &Menu::DrawConnect, M_CancelConnect);

void M_ConnectMenu(int choice)
{
  if (game.Playing())
    {
      mbox.Set(ALREADYPLAYING,NULL,MsgBox::NOTHING);
      return;
    }

  Menu::SetupNextMenu(&Connectdef);
  game.net->CL_StartPinging(false);
}



//===========================================================================
//                          PLAYER SETUP MENUS
//===========================================================================

// this is set before entering the MultiPlayer setup menu,
// for either player 1 or 2
static  PlayerInfo *setupm_player;
static presentation_t *multi_pres;

static consvar_t cv_menu_playername =  {"m_playername", "gorak", CV_HIDDEN, NULL};
//static consvar_t cv_menu_playercolor = {"m_playercolor", "0", CV_HIDDEN, Color_cons_t};

static bool M_QuitSetupPlayerMenu()
{
  setupm_player->name = cv_menu_playername.str;

  if (multi_pres)
    {
      // delete the animated guy
      delete multi_pres;
      multi_pres = NULL;
    }

  return true;
}


static menuitem_t SetupPlayer_MI[] =
{
  {IT_TEXTBOX, NULL, "Your name",  {&cv_menu_playername}, 0},
  {IT_CVAR,    NULL, "Your color", {&cv_playercolor}, 0},
  // FIXME
  //{IT_ARROWS | IT_STRING, NULL, "Your skin" , {(consvar_t *)M_HandleSetupPlayerSkin}, 32},
  //{IT_ARROWS | IT_STRING          , NULL, "Your species" , {(consvar_t *)M_HandleSetupPlayerClass}, 32},
  {IT_CVAR, NULL, "Messages:"       ,{&cv_showmessages}, 0},
  {IT_CVAR, NULL, "Always Run"      ,{&cv_autorun}     , 0},
  {IT_CVAR, NULL, "Crosshair"       ,{&cv_crosshair}   , 0},
  //{IT_CVAR, NULL, "Crosshair scale" ,{&cv_crosshairscale}  ,0},
  {IT_CVAR, NULL, "Autoaim"         ,{&cv_autoaim}         ,0},
  {IT_CALL | IT_STRING | IT_WHITE,    0, "Setup Controls...", {(consvar_t *)M_SetupControlsMenu}, 0},
  {IT_LINK, NULL, "Mouse config...",  {(consvar_t *)&MouseOptionsDef}, 0}
};

enum setupplayer_e
{
  setupplayer_name,
  setupplayer_color,
  //setupplayer_skin,
  setupplayer_messages,
  setupplayer_alwaysrun,
  setupplayer_crosshair,
  setupplayer_autoaim,
  setupplayer_controls,
  setupplayer_mouse
};


Menu  SetupPlayerDef("M_MULTI", "Multiplayer", &MultiPlayerDef, ITEMS(SetupPlayer_MI), 90, 40,
		     0, &Menu::DrawSetupPlayer, M_QuitSetupPlayerMenu);



static  bool setupcontrols_player2;

// setup the local player(s)
void M_SetupPlayer(int choice)
{
  int n = Consoleplayer.size();
  if (choice == MI_setupplayer1) // First local player
    {
      if (n >= 1)
        setupm_player = Consoleplayer[0];
      else
        setupm_player = &localplayer;

      setupcontrols_player2 = false;

      // set up menu for player 1
      //setupm_cvskin = &cv_skin;

      SetupPlayer_MI[setupplayer_color].use.cvar = &cv_playercolor;
      SetupPlayer_MI[setupplayer_messages].use.cvar = &cv_showmessages;
      SetupPlayer_MI[setupplayer_alwaysrun].use.cvar = &cv_autorun;
      SetupPlayer_MI[setupplayer_crosshair].use.cvar = &cv_crosshair;
      SetupPlayer_MI[setupplayer_autoaim].use.cvar = &cv_autoaim;
      SetupPlayer_MI[setupplayer_mouse].use.submenu = &MouseOptionsDef;
    }
  else
    {
      if (n >= 2)
        setupm_player = Consoleplayer[1];
      else
        setupm_player = &localplayer2;

      setupcontrols_player2 = true;

      // set up menu for player 2
      //setupm_cvskin = &cv_skin2;

      SetupPlayer_MI[setupplayer_color].use.cvar = &cv_playercolor2;
      SetupPlayer_MI[setupplayer_messages].use.cvar = &cv_showmessages2;
      SetupPlayer_MI[setupplayer_alwaysrun].use.cvar = &cv_autorun2;
      SetupPlayer_MI[setupplayer_crosshair].use.cvar = &cv_crosshair2;
      SetupPlayer_MI[setupplayer_autoaim].use.cvar = &cv_autoaim2;
      SetupPlayer_MI[setupplayer_mouse].use.submenu = &Mouse2OptionsDef;
    }

  cv_menu_playername.Set(setupm_player->name.c_str());

  if (setupm_player->ptype < 0)
    setupm_player->ptype = allowed_pawntypes[0];

  multi_pres = NULL;

  Menu::SetupNextMenu(&SetupPlayerDef);
}


// called at splitscreen changes
void M_SwitchSplitscreen()
{
  // activate setup for player 2
  if (cv_splitscreen.value)
    MultiPlayer_MI[MI_setupplayer2].flags = IT_ACT;
  else
    MultiPlayer_MI[MI_setupplayer2].flags = IT_OFF_BIG;

  //if (MultiPlayerDef.lastOn==MI_setupplayer2) MultiPlayerDef.lastOn= MI_setupplayer1;
}



//  Draw the multi player setup menu, had some fun with player anim
#define PLBOXW    8
#define PLBOXH    9
void Menu::DrawSetupPlayer()
{
  // TODO it would be easier to draw menus if menuitems had an optional x coord as well...
  // use generic drawer for cursor, items and title
  DrawMenu();

  // TODO team selection, draw a team symbol...

  // draw name string
  M_DrawTextBox(x+90,y-8,MAXPLAYERNAME,1);
  hud_font->DrawString(x+98, y, setupm_player->name.c_str());

  // draw text cursor for name
  if (itemOn == 0 && AnimCount < 4)   //blink cursor
    hud_font->DrawCharacter(x+98+hud_font->StringWidth(setupm_player->name.c_str()),y,'_' | 0x80, V_SCALE);

  // draw box around guy
  M_DrawTextBox(4, y+24, PLBOXW, PLBOXH);
  M_DrawTextBox(236, y+24, PLBOXW, PLBOXH);

  // draw skin string
  // TODO draw pawntype name? draw pawntype stats like Hexen?
  //hud_font->DrawString(20, y+120, setupm_cvskin->str);

  if (!multi_pres)
    {
      // if the presentation does not exist (or needed to be changed), create it anew
      multi_pres = new spritepres_t("NONE", &mobjinfo[pawndata[setupm_player->ptype].mt], setupm_player->color);
      multi_pres->SetAnim(presentation_t::Run);
    }

  // animate the player in the box
  multi_pres->Update(1);

  // skin 0 is default player sprite
  //spritedef_t *sprdef = &skins[R_SkinAvailable(setupm_cvskin->str)].spritedef;
  spriteframe_t *sprframe  = multi_pres->GetFrame();

  int color = items[setupplayer_color].use.cvar->value;
  if (color == 0)
    current_colormap = colormaps;
  else
    current_colormap = (byte *)translationtables - 256 + (color << 8);

  // draw player sprites
  sprframe->tex[0]->Draw(12 +(PLBOXW*8/2),y+24+(PLBOXH*8), V_SCALE | V_MAP);
  sprframe->tex[2]->Draw(244+(PLBOXW*8/2),y+24+(PLBOXH*8), V_SCALE | V_MAP);
}


static void M_HandleSetupPlayerClass(int val)
{
  static int i = 0;
  int n = allowed_pawntypes.size();

  if (val == 0)
    {
      if (--i < 0)
        i = n-1;
    }
  else
    {
      if (++i >= n)
        i = 0;
    }

  int ptype = allowed_pawntypes[i];
  setupm_player->ptype = ptype;
  if (multi_pres)
    {
      delete multi_pres;
      multi_pres = NULL;
    }
}

/*
static void M_HandleSetupPlayerSkin(int val)
{
  int myskin  = setupm_cvskin->value;

  if (val == 0)
    myskin--;
  else
    myskin++;

  // check skin
  if (myskin < 0)
    myskin = numskins-1;
  if (myskin > numskins-1)
    myskin = 0;

  // check skin change
  if (myskin != setupm_player->skin)
    COM_BufAddText(va("%s \"%s\"",setupm_cvskin->name ,skins[myskin].name));
}
*/


//===========================================================================
//                        Server OPTIONS MENU
//===========================================================================

static menuitem_t ServerOptions_MI[] =
{
  {IT_CVAR, NULL, "Public server", {&cv_publicserver}, 0},
  {IT_TEXTBOX, NULL, "Server name", {&cv_servername}, 0},
  {IT_TEXTBOX, NULL, "Master server", {&cv_masterserver}, 0},
};

Menu ServerOptionsDef("M_OPTTTL", "OPTIONS", &OptionsDef, ITEMS(ServerOptions_MI), 28, 40);


//===========================================================================
//                        Network OPTIONS MENU
//===========================================================================

static menuitem_t NetOptions_MI[]=
{
  {IT_CVAR, NULL, "Allow joining",   {&cv_allownewplayers} ,0},
  {IT_CVAR, NULL, "Max players",     {&cv_maxplayers}      ,0},
  {IT_CVAR, NULL, "Deathmatch type" ,{&cv_deathmatch}      ,0},
  {IT_CVAR, NULL, "Teamplay"        ,{&cv_teamplay}        ,0},
  {IT_CVAR, NULL, "Team damage"     ,{&cv_teamdamage}      ,0},
  {IT_CVAR, NULL, "Hidden players",  {&cv_hiddenplayers}   ,0},
  {IT_CVAR, NULL, "Level exit mode", {&cv_exitmode}        ,0},
  {IT_CVAR, NULL, "Fraglimit"       ,{&cv_fraglimit}       ,0},
  {IT_CVAR, NULL, "Timelimit"       ,{&cv_timelimit}       ,0},
  {IT_CVAR, NULL, "Allow autoaim"   ,{&cv_allowautoaim}    ,0},
  {IT_CVAR, NULL, "Allow freelook"  ,{&cv_allowmlook}      ,0},
  {IT_CVAR, NULL, "Allow pause"     ,{&cv_allowpause}      ,0},
  {IT_CVAR, NULL, "Frag's Weapon Falling", {&cv_fragsweaponfalling}, 0},

  {IT_CALL | IT_STRING | IT_WHITE, NULL, "Game Options..." ,{(consvar_t *)M_GameOption},0},
};

Menu  NetOptionDef("M_OPTTTL", "OPTIONS", &MultiPlayerDef, ITEMS(NetOptions_MI), 60, 40);


void M_NetOption(int choice)
{
  if (!game.server)
    {
      mbox.Set("You are not the server\nYou can't change the options\n");
      return;
    }
  Menu::SetupNextMenu(&NetOptionDef);
}



//===========================================================================
//                        Game OPTIONS MENU
//===========================================================================

static menuitem_t GameOptions_MI[] =
{
  {IT_CVAR, NULL, "Item respawn"        ,{&cv_itemrespawn}        ,0},
  {IT_CVAR, NULL, "Item respawn time"   ,{&cv_itemrespawntime}    ,0},
  {IT_CVAR, NULL, "Monster respawn"     ,{&cv_respawnmonsters}    ,0},
  {IT_CVAR, NULL, "Monster respawn time",{&cv_respawnmonsterstime},0},
  {IT_CVAR, NULL, "Fast monsters"       ,{&cv_fastmonsters}       ,0},
  // TODO cv_nomonsters?
  {IT_CVAR, NULL, "Gravity"             ,{&cv_gravity}            ,0},
  {IT_CVAR, NULL, "Jumpspeed",           {&cv_jumpspeed}          ,0},
  {IT_CVAR, NULL, "Allow rocket jump",   {&cv_allowrocketjump}    ,0},
  {IT_CVAR, NULL, "Solid corpses"        ,{&cv_solidcorpse}        ,0},
  {IT_CVAR, NULL, "Voodoo dolls"        ,{&cv_voodoodolls}        ,0},
  {IT_CALL | IT_STRING | IT_WHITE, NULL, "Network Options..."  ,{(consvar_t *)M_NetOption},110}
};

Menu GameOptionDef("M_OPTTTL", "OPTIONS", &OptionsDef, ITEMS(GameOptions_MI), 60, 40);


//===========================================================================
//                        SOUND VOLUME MENU
//===========================================================================

void M_SfxVol(int choice);
void M_MusicVol(int choice);
void M_CDAudioVol(int choice);

enum sound_e
{
  MI_sfx_vol = 0,
};

static menuitem_t Sound_MI[] =
{
  {IT_CV_NODRAW | IT_PATCH, "M_SFXVOL", "Sound Volume",{&cv_soundvolume},'s'},
  {IT_NONE | IT_BIGSLIDER,  NULL      , NULL          ,{&cv_soundvolume}    },
  {IT_CV_NODRAW | IT_PATCH, "M_MUSVOL", "Music Volume",{&cv_musicvolume},'m'},
  {IT_NONE | IT_BIGSLIDER,  NULL      , NULL          ,{&cv_musicvolume}    },
  {IT_CV_NODRAW | IT_PATCH, "M_CDVOL" , "CD Volume"   ,{&cd_volume}     ,'c'},
  {IT_NONE | IT_BIGSLIDER,  NULL      , NULL          ,{&cd_volume}         }
};

Menu  SoundDef("M_SVOL", "Sound Volume", &OptionsDef, ITEMS(Sound_MI), 80, 50);


//===========================================================================
//                        Video OPTIONS MENU
//===========================================================================

static void M_OpenGLOption(int choice)
{
  if (rendermode != render_soft)
    Menu::SetupNextMenu(&OpenGLOptionDef);
  else
    mbox.Set("You are in software mode.\n", NULL, MsgBox::NOTHING);
}

static menuitem_t VideoOptions_MI[]=
{
  {IT_LINK, NULL, "Video modes..."  , {(consvar_t *)&VidModeDef}, 0},
  {IT_CVAR, NULL, "Fullscreen"      , {&cv_fullscreen}, 0},
  {IT_CV_SLIDER | IT_STRING, NULL, "Brightness" , {&cv_usegamma}, 0},
  {IT_CV_SLIDER | IT_STRING, NULL, "Screen size", {&cv_viewsize}, 0},
  {IT_CVAR, NULL, "Scale status bar", {&cv_scalestatusbar}, 0},
  {IT_CVAR, NULL, "Translucency"    , {&cv_translucency}  , 0},
  {IT_CVAR, NULL, "Splats"          , {&cv_splats}        , 0},
  {IT_CVAR, NULL, "Bloodtime"       , {&cv_bloodtime}     , 0},
  {IT_CVAR, NULL, "Screenslink effect", {&cv_screenslink}   , 0},
#ifdef HWRENDER
  {IT_CALL | IT_STRING | IT_WHITE | IT_DY, NULL, "3D Card Options...", {(consvar_t *)M_OpenGLOption}, 20},
#endif
};

Menu VideoOptionsDef("M_OPTTTL", "OPTIONS", &OptionsDef, ITEMS(VideoOptions_MI), 60, 40);


//===========================================================================
//                           VIDEO MODE MENU
//===========================================================================

void M_HandleVideoMode(int ch);

static menuitem_t VideoMode_MI[]=
{
  {IT_KEYHANDLER | IT_SPACE, NULL, "", {(consvar_t *)M_HandleVideoMode}, '\0'},     // dummy menuitem for the control func
};


Menu VidModeDef("M_VIDEO", "Video Mode", &OptionsDef, ITEMS(VideoMode_MI), 48, 36,
		0, &Menu::DrawVideoMode);

//added:30-01-98:
#define MAXCOLUMNMODES   10     //max modes displayed in one column
#define MAXMODEDESCS     (MAXCOLUMNMODES*3)


struct modedesc_t
{
  int   modenum;   // video mode number in the vidmodes list
  char *desc;      // XXXxYYY
  int   iscur;     // 1 if it is the current active mode
};

static modedesc_t modedescs[MAXMODEDESCS];



// Draw the video modes list, a-la-Quake
void Menu::DrawVideoMode()
{
  int     i,j,dup,row,col,nummodes;
  char    *desc;
  char    temp[80];

  DrawTitle();

#ifdef LINUX
  I_PrepareVideoModeList(); // FIXME: hack
#endif
  vidm_nummodes = 0;
  nummodes = I_NumVideoModes();

#ifdef __WIN32__
  //faB: clean that later : skip windowed mode 0, video modes menu only shows
  //     FULL SCREEN modes
  if (nummodes<1) {
    // put the windowed mode so that there is at least one mode
    modedescs[0].modenum = 0;
    modedescs[0].desc = I_GetVideoModeName(0);
    modedescs[0].iscur = 1;
    vidm_nummodes = 1;
  }
  for (i=1 ; i<=nummodes && vidm_nummodes<MAXMODEDESCS ; i++)
#else
    // DOS does not skip mode 0, because mode 0 is ALWAYS present
    for (i=0 ; i<nummodes && vidm_nummodes<MAXMODEDESCS ; i++)
#endif
      {
        desc = I_GetVideoModeName(i);
        if (desc)
          {
            dup = 0;

            //when a resolution exists both under VGA and VESA, keep the
            // VESA mode, which is always a higher modenum
            for (j=0 ; j<vidm_nummodes ; j++)
              {
                if (!strcmp(modedescs[j].desc, desc))
                  {
                    //mode(0): 320x200 is always standard VGA, not vesa
                    if (modedescs[j].modenum != 0)
                      {
                        modedescs[j].modenum = i;
                        dup = 1;

                        if (i == vid.modenum)
                          modedescs[j].iscur = 1;
                      }
                    else
                      {
                        dup = 1;
                      }

                    break;
                  }
              }

            if (!dup)
              {
                modedescs[vidm_nummodes].modenum = i;
                modedescs[vidm_nummodes].desc = desc;
                modedescs[vidm_nummodes].iscur = 0;

                if (i == vid.modenum)
                  modedescs[vidm_nummodes].iscur = 1;

                vidm_nummodes++;
              }
          }
      }

  vidm_column_size = (vidm_nummodes+2) / 3;


  row = 16;
  col = y;
  for (i=0; i<vidm_nummodes; i++)
    {
      hud_font->DrawString(row, col, modedescs[i].desc, (modedescs[i].iscur ? V_WHITEMAP : 0) | V_SCALE);

      col += 8;
      if ((i % vidm_column_size) == (vidm_column_size-1))
        {
          row += 8*13;
          col = 36;
        }
    }

  if (vidm_testingmode>0)
    {
      sprintf(temp, "TESTING MODE %s", modedescs[vidm_current].desc);
      M_CenterText(y+80+24, temp);
      M_CenterText(y+90+24, "Please wait 5 seconds...");
    }
  else
    {
      M_CenterText(y+60+24, "Press ENTER to set mode");

      M_CenterText(y+70+24, "T to test mode for 5 seconds");

      sprintf(temp, "D to make %s the default", I_GetVideoModeName(vid.modenum));
      M_CenterText(y+80+24,temp);

      sprintf(temp, "Current default is %dx%d (%d bits)", cv_scr_width.value, cv_scr_height.value, cv_scr_depth.value);
      M_CenterText(y+90+24,temp);

      M_CenterText(y+100+24, "Press ESC to exit");
    }

  // Draw the cursor for the VidMode menu
  if (AnimCount<4)    //use the Skull anim counter to blink the cursor
    {
      i = 16 - 10 + ((vidm_current / vidm_column_size)*8*13);
      j = y + ((vidm_current % vidm_column_size)*8);
      hud_font->DrawCharacter(i, j, '*' | 0x80, V_SCALE);
    }
}


//added:30-01-98: special menuitem key handler for video mode list
void M_HandleVideoMode(int key)
{
  switch (key)
    {
    case KEY_DOWNARROW:
      S_StartLocalAmbSound(sfx_menu_move);
      vidm_current++;
      if (vidm_current>=vidm_nummodes)
        vidm_current = 0;
      break;

    case KEY_UPARROW:
      S_StartLocalAmbSound(sfx_menu_move);
      vidm_current--;
      if (vidm_current<0)
        vidm_current = vidm_nummodes-1;
      break;

    case KEY_LEFTARROW:
      S_StartLocalAmbSound(sfx_menu_move);
      vidm_current -= vidm_column_size;
      if (vidm_current<0)
        vidm_current = (vidm_column_size*3) + vidm_current;
      if (vidm_current>=vidm_nummodes)
        vidm_current = vidm_nummodes-1;
      break;

    case KEY_RIGHTARROW:
      S_StartLocalAmbSound(sfx_menu_move);
      vidm_current += vidm_column_size;
      if (vidm_current>=(vidm_column_size*3))
        vidm_current %= vidm_column_size;
      if (vidm_current>=vidm_nummodes)
        vidm_current = vidm_nummodes-1;
      break;

    case KEY_ENTER:
      S_StartLocalAmbSound(sfx_menu_move);
      if (!vid.setmodeneeded) //in case the previous setmode was not finished
        vid.setmodeneeded = modedescs[vidm_current].modenum+1;
      break;

    case 'T':
    case 't':
      S_StartLocalAmbSound(sfx_menu_close);
      vidm_testingmode = TICRATE*5;
      vidm_previousmode = vid.modenum;
      if (!vid.setmodeneeded) //in case the previous setmode was not finished
        vid.setmodeneeded = modedescs[vidm_current].modenum+1;
      return;

    case 'D':
    case 'd':
      // current active mode becomes the default mode.
      S_StartLocalAmbSound(sfx_menu_close);
      SCR_SetDefaultMode();
      return;

    default:
      break;
    }

}


//===========================================================================
//                        Mouse OPTIONS MENU
//===========================================================================

//added:24-03-00: note: alphaKey member is the y offset
static menuitem_t MouseOptions_MI[]=
{
  {IT_CVAR, NULL, "Use Mouse",        {&cv_usemouse}        ,0},
  {IT_CVAR, NULL, "Always MouseLook", {&cv_automlook}       ,0},
  {IT_CVAR, NULL, "Mouse Move"      , {&cv_mousemove}       ,0},
  {IT_CVAR, NULL, "Invert Mouse"    , {&cv_invertmouse}     ,0},
  {IT_CV_SLIDER | IT_STRING, NULL, "Mouse x-speed", {&cv_mousesensx}       ,0},
  {IT_CV_SLIDER | IT_STRING, NULL, "Mouse y-speed", {&cv_mousesensy}       ,0}
  // #ifdef __MACOS__
  //  ,{IT_CALL | IT_STRING | IT_WHITE, NULL, "Configure Input Sprocket..."  , {(consvar_t *)macConfigureInput}, 60}
  //#endif
};

Menu  MouseOptionsDef("M_OPTTTL", "OPTIONS", &OptionsDef, ITEMS(MouseOptions_MI), 60, 40);


//===========================================================================
// Second mouse config for the splitscreen player
//===========================================================================

static menuitem_t  Mouse2Options_MI[] =
{
  {IT_CVAR, NULL, "Second Mouse Serial Port", {&cv_mouse2port}, 0},
  {IT_CVAR, NULL, "Use Mouse 2"     , {&cv_usemouse2}      , 0},
  {IT_CVAR, NULL, "Always MouseLook", {&cv_automlook2}, 0},
  {IT_CVAR, NULL, "Mouse Move",       {&cv_mousemove2}     , 0},
  {IT_CVAR, NULL, "Invert Mouse",     {&cv_invertmouse2}   , 0},
  {IT_CV_SLIDER | IT_STRING, NULL, "Mouse x-speed", {&cv_mousesensx2}, 0},
  {IT_CV_SLIDER | IT_STRING, NULL, "Mouse y-speed", {&cv_mousesensy2}, 0}
};

Menu  Mouse2OptionsDef("M_OPTTTL", "OPTIONS", &SetupPlayerDef, ITEMS(Mouse2Options_MI), 60, 40);


//===========================================================================
//                          CONTROLS MENU
//===========================================================================

static menuitem_t Control_MI[]=
{
  {IT_CONTROLSTR, NULL, "Fire"        , {NULL}, gc_fire       },
  {IT_CONTROLSTR, NULL, "Use/Open"    , {NULL}, gc_use        },
  {IT_CONTROLSTR, NULL, "Jump"        , {NULL}, gc_jump       },
  {IT_CONTROLSTR, NULL, "Forward"     , {NULL}, gc_forward    },
  {IT_CONTROLSTR, NULL, "Backpedal"   , {NULL}, gc_backward   },
  {IT_CONTROLSTR, NULL, "Turn Left"   , {NULL}, gc_turnleft   },
  {IT_CONTROLSTR, NULL, "Turn Right"  , {NULL}, gc_turnright  },
  {IT_CONTROLSTR, NULL, "Run"         , {NULL}, gc_speed      },
  {IT_CONTROLSTR, NULL, "Strafe On"   , {NULL}, gc_strafe     },
  {IT_CONTROLSTR, NULL, "Strafe Left" , {NULL}, gc_strafeleft },
  {IT_CONTROLSTR, NULL, "Strafe Right", {NULL}, gc_straferight},
  {IT_CONTROLSTR, NULL, "Look Up"     , {NULL}, gc_lookup     },
  {IT_CONTROLSTR, NULL, "Look Down"   , {NULL}, gc_lookdown   },
  {IT_CONTROLSTR, NULL, "Center View" , {NULL}, gc_centerview },
  {IT_CONTROLSTR, NULL, "Mouselook"   , {NULL}, gc_mouseaiming},
  {IT_LINK | IT_DY, NULL, "next", {(consvar_t *)&ControlDef2}, 20}
};

Menu  ControlDef("M_CONTRO", "Setup Controls", &OptionsDef, ITEMS(Control_MI), 50, 40,
		 0, &Menu::DrawControl);

//
//  Controls page 1
//
static menuitem_t Control2_MI[]=
{
  {IT_CONTROLSTR, NULL, "Fist/Chainsaw"  , {NULL}, gc_weapon1},
  {IT_CONTROLSTR, NULL, "Pistol"         , {NULL}, gc_weapon2},
  {IT_CONTROLSTR, NULL, "Shotgun/Double" , {NULL}, gc_weapon3},
  {IT_CONTROLSTR, NULL, "Chaingun"       , {NULL}, gc_weapon4},
  {IT_CONTROLSTR, NULL, "Rocket Launcher", {NULL}, gc_weapon5},
  {IT_CONTROLSTR, NULL, "Plasma rifle"   , {NULL}, gc_weapon6},
  {IT_CONTROLSTR, NULL, "BFG"            , {NULL}, gc_weapon7},
  {IT_CONTROLSTR, NULL, "Chainsaw"       , {NULL}, gc_weapon8},
  {IT_CONTROLSTR, NULL, "Previous Weapon", {NULL}, gc_prevweapon},
  {IT_CONTROLSTR, NULL, "Next Weapon"    , {NULL}, gc_nextweapon},
  {IT_CONTROLSTR, NULL, "Best Weapon"    , {NULL}, gc_bestweapon},
  {IT_CONTROLSTR, NULL, "Inventory Left" , {NULL}, gc_invprev},
  {IT_CONTROLSTR, NULL, "Inventory Right", {NULL}, gc_invnext},
  {IT_CONTROLSTR, NULL, "Inventory Use"  , {NULL}, gc_invuse },
  {IT_CONTROLSTR, NULL, "Fly down"       , {NULL}, gc_flydown},
  {IT_LINK | IT_DY, NULL, "next", {(consvar_t *)&ControlDef}, 20}
};

// FIXME add these to another menu
//  {IT_CONTROLSTR, NULL, "Talk key"       , {NULL}, gc_talkkey},
//  {IT_CONTROLSTR, NULL, "Console"        , {NULL}, gc_console},
//  {IT_CONTROLSTR, NULL, "Rankings/Score",  {NULL}, gc_scores },

Menu  ControlDef2("M_CONTRO", "Setup Controls", &OptionsDef, ITEMS(Control2_MI), 50, 40,
		  0, &Menu::DrawControl);


static short (*setup_gc)[2];  // pointer to the gamecontrols of the player being edited
static int controltochange;


/// Start the controls menu, setting it up for either (local) player 1 or 2
void M_SetupControlsMenu(int choice)
{
  if (setupcontrols_player2 && choice != 10)
    // menuitem # 10 means that the call came from Options menu, which
    // always sets player 1 controls. Hack.
    setup_gc = gamecontrol[1]; // was called from secondary player's multiplayer setup menu
  else
    setup_gc = gamecontrol[0]; // was called from main Options (for console player, then)

  Menu::SetupNextMenu(&ControlDef);
}



//  Draws the Customise Controls menu
void Menu::DrawControl()
{
  // draw title, strings and submenu
  DrawMenu();

  M_CenterText(y-12, (setupcontrols_player2 ? "SET CONTROLS FOR SECONDARY PLAYER" :
                                 "PRESS ENTER TO CHANGE, BACKSPACE TO CLEAR"));

  for (int i=0; i < numitems; i++)
    {
      if ((items[i].flags & IT_TYPE_MASK) != IT_CONTROL)
        continue;

      int keys[2];

      keys[0] = setup_gc[items[i].alphaKey][0];
      keys[1] = setup_gc[items[i].alphaKey][1];

      char tmp[50];
      tmp[0] = '\0';
      if (keys[0] == KEY_NULL && keys[1] == KEY_NULL)
        {
          strcpy(tmp, "---");
        }
      else
        {
          if (keys[0] != KEY_NULL)
            strcat(tmp, G_KeynumToString(keys[0]));

          if (keys[0] != KEY_NULL && keys[1] != KEY_NULL)
            strcat(tmp, " or ");

          if (keys[1] != KEY_NULL)
            strcat(tmp, G_KeynumToString(keys[1]));

        }
      hud_font->DrawString(x+220-hud_font->StringWidth(tmp), y + i*8, tmp, V_WHITEMAP | V_SCALE);
    }
}


// returns true when finished
static bool M_ChangecontrolResponse(event_t* ev)
{
  if (ev->type != ev_keydown)
    return false; // ignore mouse movements, just get buttons

  int ch = ev->data1;
  
  if (ch == KEY_ESCAPE || ch == KEY_PAUSE)
    return true; // ESCAPE cancels

  int control = controltochange;

  // check if we already use this key here
  int found = -1;
  if (setup_gc[control][0] == ch)
    found = 0;
  else if (setup_gc[control][1] == ch)
    found = 1;

  if (found >= 0)
    {
      // replace mouse and joy clicks by double clicks
      if (ch >= KEY_MOUSE1 && ch <= KEY_MOUSE1+MOUSEBUTTONS)
	ch = ch - KEY_MOUSE1 + KEY_DBLMOUSE1;

      /* FIXME new joystick code ignores double clicks for now.
	 else
	 if (ch>=KEY_JOY1 && ch<=KEY_JOY1+JOYBUTTONS)
	 setup_gc[control][found] = ch-KEY_JOY1+KEY_DBLJOY1;
      */
    }
  else
    {
      if (setup_gc[control][0] == KEY_NULL)
	found = 0;
      else
	found = 1;
    }

  G_CheckDoubleUsage(ch);
  setup_gc[control][found] = ch;
  return true;
}


static void M_ChangeControl(const char *str, int key)
{
  static char tmp[55];

  controltochange = key;
  sprintf(tmp, "Hit the new key for\n%s\nESC to cancel", str);

  mbox.SetHandler(tmp, M_ChangecontrolResponse);
}



//===========================================================================
//                          Read This! MENU 1
//===========================================================================

static menuitem_t Read1_MI[] =
{
  {IT_SUBMENU | IT_SPACE, NULL, "", {(consvar_t *)&ReadDef2}, 0}
};

Menu  ReadDef1(NULL, "Readme1", &MainMenuDef, ITEMS(Read1_MI), 280, 185,
	       0, &Menu::DrawReadThis1);

//
// Read This Menus
// Had a "quick hack to fix romero bug"
//
void Menu::DrawReadThis1()
{
  switch (game.mode)
    {
    case gm_doom2:
      tc.GetPtr("HELP")->Draw(0,0,0 | V_SCALE);
      break;
    case gm_doom1s:
    case gm_doom1:
    case gm_udoom:
    case gm_heretic:
    case gm_hexen:
      tc.GetPtr("HELP1")->Draw(0,0,0 | V_SCALE);
      break;
    default:
      break;
    }
  return;
}

//===========================================================================
//                          Read This! MENU 2
//===========================================================================

static menuitem_t Read2_MI[] =
{
  {IT_SUBMENU | IT_SPACE, NULL, "", {(consvar_t *)&MainMenuDef},0}
};

Menu  ReadDef2(NULL, "Readme2", &ReadDef1, ITEMS(Read2_MI), 330, 175,
	       0, &Menu::DrawReadThis2);



// Read This Menus - optional second page.
void Menu::DrawReadThis2()
{
  switch (game.mode)
    {
    case gm_udoom:
    case gm_doom2:
      // This hack keeps us from having to change menus.
      tc.GetPtr("CREDIT")->Draw(0,0,0 | V_SCALE);
      break;
    case gm_doom1s:
    case gm_doom1:
    case gm_heretic:
    case gm_hexen:
      tc.GetPtr("HELP2")->Draw(0,0,0 | V_SCALE);
      break;
    default:
      break;
    }
  return;
}


//===========================================================================
//                        QuickSAVE & QuickLOAD
//===========================================================================

char    tempstring[80];

void M_QuickSaveResponse(int ch)
{
  if (ch == 'y')
    {
      M_DoSave(quickSaveSlot);
      S_StartLocalAmbSound(sfx_menu_close);
    }
}

void M_QuickSave()
{
  /*
  if (demorecording)
    {
      S_StartLocalAmbSound(sfx_menu_fail);
      return;
    }
  */

  if (game.state != GameInfo::GS_LEVEL)
    return;

  if (quickSaveSlot < 0)
    {
      Menu::Open();
      M_ReadSaveStrings();
      Menu::SetupNextMenu(&SaveDef);
      return;
    }
  sprintf(tempstring,QSPROMPT,savegamestrings[quickSaveSlot]);
  mbox.Set(tempstring,M_QuickSaveResponse,MsgBox::YESNO);
}



void M_QuickLoadResponse(int ch)
{
  if (ch == 'y')
    {
      M_LoadSelect(quickSaveSlot);
      S_StartLocalAmbSound(sfx_menu_close);
    }
}


void M_QuickLoad()
{
  if (game.netgame)
    {
      mbox.Set(QLOADNET,NULL,MsgBox::NOTHING);
      return;
    }

  if (quickSaveSlot < 0)
    {
      mbox.Set(QSAVESPOT,NULL,MsgBox::NOTHING);
      return;
    }
  sprintf(tempstring,QLPROMPT,savegamestrings[quickSaveSlot]);
  mbox.Set(tempstring,M_QuickLoadResponse,MsgBox::YESNO);
}




//==========================================================================
//                       Menu class implementation
//==========================================================================

// static class members
font_t  *Menu::font;
short    Menu::AnimCount;
Texture *Menu::pointer[2];
int      Menu::which_pointer;
int      Menu::SkullBaseLump;
tic_t    Menu::NowTic;
Menu    *Menu::currentMenu;
short    Menu::itemOn;
bool     Menu::active;


/// constructor
Menu::Menu(const char *tpic, const char *t, Menu *up, int nitems, menuitem_t *it,
	   short mx, short my, short on, drawfunc_t df, quitfunc_t qf)
{
  titlepic = tpic;
  title = t;
  parent = up;
  numitems = nitems;
  items = it;
  x = mx;
  y = my;
  lastOn = on;
  drawroutine = df;
  quitroutine = qf;
}


/// Updates pointer animation, implements vidmode testing
void Menu::Ticker()
{
  if (--AnimCount <= 0)
    {
      AnimCount = 8;
      which_pointer ^= 1;
    }

  // test mode for five seconds
  if (vidm_testingmode > 0)
    {
      // restore the previous video mode
      if (--vidm_testingmode == 0)
        vid.setmodeneeded = vidm_previousmode + 1;
    }
}


/// Open menu eats all events. Closed menu only eats F-keys and esc.
bool Menu::Responder(event_t *ev)
{
  static  tic_t   mousewait = 0;
  static  int     mousey = 0;
  static  int     lasty = 0;
  static  int     mousex = 0;
  static  int     lastx = 0;

  NowTic = I_GetTics();

  // Menu uses mostly only keydown events (exception: mouse/joystick navigation in an open menu)
  int ch = -1;
  if (ev->type == ev_keydown)
    ch = ev->data1;

  // quick dev screenshot hack...
  if (devparm && ch == KEY_F1)
    {
      COM_BufAddText("screenshot\n");
      return true;
    }

  // F-keys, esc
  if (!active)
    {
      switch (ch)
        {
        case KEY_F1:            // Help key
          Open();
          if (game.mode == gm_udoom)
            currentMenu = &ReadDef2;
          else
            currentMenu = &ReadDef1;

          itemOn = 0;
	  break;

        case KEY_F2:            // Save
          Open();
          S_StartLocalAmbSound(sfx_menu_open);
          M_SaveGame(0);
          return true;

        case KEY_F3:            // Load
          Open();
          S_StartLocalAmbSound(sfx_menu_open);
          M_LoadGame(0);
          return true;

        case KEY_F4:            // Sound Volume
          Open();
          currentMenu = &SoundDef;
          itemOn = MI_sfx_vol;
	  break;

        case KEY_F5:            // Video Menu
          Open();
          Menu::SetupNextMenu(&VidModeDef);
          //M_ChangeDetail(0);
	  break;

        case KEY_F6:            // Quicksave
          S_StartLocalAmbSound(sfx_menu_open);
          M_QuickSave();
          return true;

        case KEY_F7:            // Options menu
          Open();
          Menu::SetupNextMenu(&OptionsDef);
          //M_EndGame(0);
	  break;

        case KEY_F8:            // Toggle messages
          cv_showmessages.AddValue(1);
	  break;

        case KEY_F9:            // Quickload
          S_StartLocalAmbSound(sfx_menu_open);
          M_QuickLoad();
          return true;

        case KEY_F10:           // Quit DOOM
          S_StartLocalAmbSound(sfx_menu_open);
          M_QuitDOOM(0);
          return true;

        case KEY_F11:           // Toggle gamma correction
          cv_usegamma.AddValue(1);
	  break;

        case KEY_ESCAPE:        // Open the main menu
          Open();
	  break;

	default: // not a recognized keydown event
	  return false;
        }

      S_StartLocalAmbSound(sfx_menu_open);
      return true;
    }

  // the menu is active!

  // message box. TODO this is a halfway solution, fix...
  if (mbox.Active())
    {
      mbox.Input(ev);
      return true;
    }

  // remap virtual keys (mouse & joystick buttons)
  switch (ch)
    {
    case KEY_MOUSE1:
    case KEY_JOY0BUT0:
      ch = KEY_ENTER;
      break;
    case KEY_MOUSE1 + 1:
    case KEY_JOY0BUT1:
      ch = KEY_BACKSPACE;
      break;
    }

  // mouse navigation in the menu
  if (ev->type == ev_mouse)
    {
      if (NowTic <= mousewait)
	return true; // just eat the event. NOTE this disables the "mouselook while menu is on" feature...

      mousey += ev->data3;
      if (mousey < lasty-30)
	{
	  ch = KEY_DOWNARROW;
	  mousewait = NowTic + TICRATE/7;
	  mousey = lasty -= 30;
	}
      else if (mousey > lasty+30)
	{
	  ch = KEY_UPARROW;
	  mousewait = NowTic + TICRATE/7;
	  mousey = lasty += 30;
	}

      mousex += ev->data2;
      if (mousex < lastx-30)
	{
	  ch = KEY_LEFTARROW;
	  mousewait = NowTic + TICRATE/7;
	  mousex = lastx -= 30;
	}
      else if (mousex > lastx+30)
	{
	  ch = KEY_RIGHTARROW;
	  mousewait = NowTic + TICRATE/7;
	  mousex = lastx += 30;
	}
    }

  if (ch == -1)
    return true; // Not a keydown event or remapped mouse motion, but eat it anyway.

  // textbox...
  if (textbox.Active())
    {
      textbox.Input(ch);
      return true;
    }


  // esc exits menu
  if (ch == KEY_ESCAPE)
    {
      if (vidm_testingmode > 0)
        {
          // change back to the previous mode quickly
          vid.setmodeneeded = vidm_previousmode+1;
          vidm_testingmode = 0;
          return true;
        }
      currentMenu->lastOn = itemOn;
      if (currentMenu->parent)
        {
          SetupNextMenu(currentMenu->parent);
          S_StartLocalAmbSound(sfx_menu_close); // it�s a matter of taste which sound to choose
        }
      else
        {
          S_StartLocalAmbSound(sfx_menu_close);
          Close(true);
        }
      return true;
    }

  void  (*routine)(int);
  routine = currentMenu->items[itemOn].use.routine;
  int type = currentMenu->items[itemOn].flags & IT_TYPE_MASK;

  // full keyhandler (eats anything but esc)
  if (routine && type == IT_KEYHANDLER)
    {
      routine(ch);
      return true;
    }

  // TODO debug, remove
  if (!routine && type != IT_NONE && type != IT_CONTROL)
    I_Error("menu routine missing!\n");

  // response to keypress depends on menuitem type
  switch (type)
    {
    case IT_SUBMENU:
      if (ch == KEY_ENTER)
	{
	  currentMenu->lastOn = itemOn;
	  SetupNextMenu(currentMenu->items[itemOn].use.submenu);
	  S_StartLocalAmbSound(sfx_menu_choose);
	  return true;
	}
      break;

    case IT_CALL:
      if (ch == KEY_ENTER)
	{
	  routine(itemOn);
	  S_StartLocalAmbSound(sfx_menu_choose);
	  return true;
	}
      break;

    case IT_ARROWS:
      if (ch == KEY_LEFTARROW || ch == KEY_RIGHTARROW)
	{
	  routine(ch == KEY_RIGHTARROW);
	  S_StartLocalAmbSound(sfx_menu_choose);
	  return true;
	}
      break;

    case IT_CONTROL:
      if (ch == KEY_ENTER)
	{
	  S_StartLocalAmbSound(sfx_menu_choose);
	  M_ChangeControl(currentMenu->items[itemOn].text, currentMenu->items[itemOn].alphaKey);
	  return true;
	}
      else if (ch == KEY_BACKSPACE)
        {
          S_StartLocalAmbSound(sfx_menu_adjust);
          // detach any keys associated to the game control
          G_ClearControlKeys(setup_gc, currentMenu->items[itemOn].alphaKey);
	  return true;
        }
      break;

    case IT_CV:
    case IT_CV_SLIDER:
      {
	consvar_t *cv = currentMenu->items[itemOn].use.cvar;
        char change = 0;
        switch (ch)
          {
          case KEY_LEFTARROW:
            change = -1;
            break;
          case KEY_RIGHTARROW:
          case KEY_ENTER:
            change = 1;
            break;
          }
        if (change != 0)
          {
            S_StartLocalAmbSound(sfx_menu_adjust);
	    if (cv->flags & CV_FLOAT)
	      {
		char s[20];
		sprintf(s, "%.4f", float(cv->value)/FRACUNIT + change*(1.0/16.0));
		cv->Set(s);
	      }
	    else
	      cv->AddValue(change);
            return true;
          }
      }
      break;

    case IT_CV_TEXTBOX:
      if (ch == KEY_ENTER)
	{
	  textbox.Open(&currentMenu->items[itemOn]);
	  return true;
	}
      break;

    default:
      break;
    }


  // finally, menu navigation (up/down) and some shortcuts
  switch (ch)
    {
    case KEY_DOWNARROW:
      do
        {
          if (++itemOn >= currentMenu->numitems)
            itemOn = 0;
        } while ((currentMenu->items[itemOn].flags & IT_TYPE_MASK) == IT_NONE);
      S_StartLocalAmbSound(sfx_menu_move);
      return true;

    case KEY_UPARROW:
      do
        {
          if (!itemOn)
            itemOn = currentMenu->numitems-1;
          else itemOn--;
        } while ((currentMenu->items[itemOn].flags & IT_TYPE_MASK) == IT_NONE);
      S_StartLocalAmbSound(sfx_menu_move);
      return true;

    case KEY_BACKSPACE:
      currentMenu->lastOn = itemOn;
      if (currentMenu->parent)
        {
          SetupNextMenu(currentMenu->parent);
          S_StartLocalAmbSound(sfx_menu_close);
        }
      return true;

    default:
      for (int i = itemOn+1; i < currentMenu->numitems; i++)
        if (currentMenu->items[i].alphaKey == ch)
          {
            itemOn = i;
            S_StartLocalAmbSound(sfx_menu_move);
            return true;
          }
      for (int i = 0; i <= itemOn; i++)
        if (currentMenu->items[i].alphaKey == ch)
          {
            itemOn = i;
            S_StartLocalAmbSound(sfx_menu_move);
            return true;
          }
      break;
    }

  return true;
}



/// Called after the view has been rendered, but before it has been blitted.
void Menu::Drawer()
{
  if (active)
    {
      NowTic = I_GetTics();

      //added:18-02-98:
      // center the scaled graphics for the menu,
      //  set it 0 again before return!!!
      vid.scaledofs = vid.centerofs;

      // now that's more readable with a faded background (yeah like Quake...)
      V_DrawFadeScreen();

      if (currentMenu->drawroutine)
        (currentMenu->*(currentMenu->drawroutine))();      // call current menu Draw routine
      else
	currentMenu->DrawMenu();

      //added:18-02-98: it should always be 0 for non-menu scaled graphics.
      vid.scaledofs = 0;
    }

  if (mbox.Active())
    mbox.Draw();
}


void Menu::Open()
{
  // intro might call this repeatedly
  if (active)
    return;

  active = true;
  // pause the game if possible
  if (!game.netgame && game.state != GameInfo::GS_DEMOPLAYBACK)
    game.paused = true;

  currentMenu = &MainMenuDef;
  itemOn = currentMenu->lastOn;

  con.Toggle(true);   // dirty hack : move away console
}


void Menu::Close(bool callexitmenufunc)
{
  if (!active)
    return;

  if (currentMenu->quitroutine && callexitmenufunc)
    {
      if (!currentMenu->quitroutine())
        return; // we can't quit this menu (also used to set parameter from the menu)
    }

  active = false;
  // unpause the game if necessary
  if (!game.netgame)
    game.paused = false;
}


void Menu::SetupNextMenu(Menu *m)
{
  if (currentMenu->quitroutine)
    {
      if (!currentMenu->quitroutine())
        return; // we can't quit this menu (also used to set parameter from the menu)
    }
  currentMenu = m;
  itemOn = currentMenu->lastOn;

  int n = currentMenu->numitems;
  // in case of...
  if (itemOn >= n)
    itemOn = n - 1;

  if (!(currentMenu->items[itemOn].flags & IT_DISABLED))
    return;

  // the current item can be disabled,
  // this code finds an enabled item
  int startitem = itemOn;
  do
    {
      itemOn = (itemOn + 1) % n;
    }
  while (currentMenu->items[itemOn].flags & IT_DISABLED && itemOn != startitem);
}


void Menu::Startup()
{
  // "menu only"-consvars
  cv_menu_skill.Reg();
  cv_menu_startmap.Reg();
  cv_menu_serversearch.Reg();
  cv_menu_playername.Reg();

  active = false;
  AnimCount  = 10;
  which_pointer = 0;

  Menu::Init();
}

// resets the menu system according to current game.mode
// changes menu font, structure etc.
void Menu::Init()
{
  int i;
  // which pawn types can be played?
  int minpawn, maxpawn;
  switch (game.mode)
    {
    case gm_hexen:
      minpawn = 37;
      maxpawn = 40;
      break;
    case gm_heretic:
      minpawn = 19;
      maxpawn = 36;
      break;
    case gm_doom2:
      minpawn = 0;
      maxpawn = 18;
      break;
    default:
      minpawn = 0;
      maxpawn = 10;
    }

  allowed_pawntypes.clear();
  for (i = minpawn; i <= maxpawn; i++)
    allowed_pawntypes.push_back(i);


  currentMenu = &MainMenuDef;
  itemOn = currentMenu->lastOn;

  font = big_font;

  // Here we could catch other version dependencies,
  //  like HELP1/2, and four episodes.

  if (!game.inventory)
    {
      // remove the inventory keys from the menu !
      for (i=0; i<ControlDef2.numitems; i++)
        if (Control2_MI[i].alphaKey == gc_invprev ||
            Control2_MI[i].alphaKey == gc_invnext ||
            Control2_MI[i].alphaKey == gc_invuse)
          Control2_MI[i].flags = IT_OFF;
    }
  else
    {
      for (i=0; i<ControlDef2.numitems; i++)
        if (Control2_MI[i].alphaKey == gc_invprev ||
            Control2_MI[i].alphaKey == gc_invnext ||
            Control2_MI[i].alphaKey == gc_invuse)
          Control2_MI[i].flags = IT_CONTROLSTR;
    }

  if (game.mode < gm_heretic)
    {
      // remove the fly down key from the menu !
      for (i=0;i<ControlDef2.numitems;i++)
        if (Control2_MI[i].alphaKey == gc_flydown)
          Control2_MI[i].flags = IT_OFF;
    }
  else
    {
      for (i=0;i<ControlDef2.numitems;i++)
        if (Control2_MI[i].alphaKey == gc_flydown)
          Control2_MI[i].flags = IT_CONTROLSTR;
    }

  //FIXME change to levelgraph node select!
  if (fc.FindNumForName("E2M1")<0)
    {
      exmy_cons_t[9].value = 0;
      exmy_cons_t[9].strvalue = NULL;
    }
  else if (fc.FindNumForName("E3M1")<0)
    {
      exmy_cons_t[18].value = 0;
      exmy_cons_t[18].strvalue = NULL;
    }
  else if (fc.FindNumForName("E4M1")<0)
    {
      exmy_cons_t[27].value = 0;
      exmy_cons_t[27].strvalue = NULL;
    }
  else if (fc.FindNumForName("E5M1")<0)
    {
      exmy_cons_t[36].value = 0;
      exmy_cons_t[36].strvalue = NULL;
    }

  switch (game.mode)
    {
    case gm_doom2:
      pointer[0] = tc.GetPtr("M_SKULL1");
      pointer[1] = tc.GetPtr("M_SKULL2");
      MainMenuDef.drawroutine = &Menu::DrawMenu;
      SkillDef.items = Skill_MI;
      SkillDef.parent = &MainMenuDef; // no episode menu
      break;
    case gm_doom1s:
      // Episodes 2 and 3 are handled,
      //  branching to an ad screen.
    case gm_doom1:
      // We need to remove the fourth episode.
      EpiDef.numitems = 3;
    case gm_udoom:
      // We are fine.
      pointer[0] = tc.GetPtr("M_SKULL1");
      pointer[1] = tc.GetPtr("M_SKULL2");
      cv_menu_startmap.PossibleValue = exmy_cons_t;
      MainMenuDef.drawroutine = &Menu::DrawMenu;
      SkillDef.items = Skill_MI;
      EpiDef.items = DoomEp_MI;
      if (game.mode == gm_udoom)
        EpiDef.numitems = 4;
      break;

    case gm_heretic:
      cv_menu_startmap.PossibleValue = exmy_cons_t;

      MainMenuDef.titlepic = "M_HTIC";
      MainMenuDef.drawroutine = &Menu::HereticMainMenuDrawer;
      SkullBaseLump = fc.GetNumForName("M_SKL00");
      pointer[0] = tc.GetPtr("M_SLCTR1");
      pointer[1] = tc.GetPtr("M_SLCTR2");

      SkillDef.items = HereticSkill_MI;
      EpiDef.items = HereticEp_MI;
      EpiDef.numitems = 5;
      break;

    case gm_hexen:
      MainMenuDef.titlepic = "M_HTIC";
      MainMenuDef.drawroutine = &Menu::HexenMainMenuDrawer;
      SkullBaseLump = fc.GetNumForName("FBULA0");
      pointer[0] = tc.GetPtr("M_SLCTR1");
      pointer[1] = tc.GetPtr("M_SLCTR2");
      SinglePlayer_MI[MI_newgame].flags = IT_OFF_BIG;
      break;

    default: // use legacy.wad menu graphics
      // TODO
      break;
    }
}



//======================================================================
// OpenGL specific options
//======================================================================

#ifdef HWRENDER

extern Menu OGL_LightingDef, OGL_FogDef, OGL_ColorDef, OGL_DevDef;

static menuitem_t OpenGLOptions_MI[]=
{
  {IT_CVAR, NULL, "Mouse look"          , {&cv_grcrappymlook}     ,  0},
  {IT_CVAR, NULL, "Field of view"       , {&cv_grfov}             , 10},
  {IT_CVAR, NULL, "Quality"             , {&cv_scr_depth}         , 20},
  {IT_CVAR, NULL, "Texture Filter"      , {&cv_grfiltermode}      , 30},

  {IT_LINK, NULL, "Lighting..."       , {(consvar_t *)&OGL_LightingDef}   , 60},
  {IT_LINK, NULL, "Fog..."            , {(consvar_t *)&OGL_FogDef}        , 70},
  {IT_LINK, NULL, "Gamma..."          , {(consvar_t *)&OGL_ColorDef}      , 80},
  {IT_LINK, NULL, "Development..."    , {(consvar_t *)&OGL_DevDef}        , 90},
};

static menuitem_t OGL_Lighting_MI[]=
{
  {IT_CVAR, NULL, "Coronas"                 , {&cv_grcoronas}         ,  0},
  {IT_CVAR, NULL, "Coronas size"            , {&cv_grcoronasize}      , 10},
  {IT_CVAR, NULL, "Dynamic lighting"        , {&cv_grdynamiclighting} , 20},
  {IT_CVAR, NULL, "Static lighting"         , {&cv_grstaticlighting}  , 30},
  {IT_CVAR, NULL, "Monsters' balls lighting", {&cv_grmblighting}      , 40},
};

static menuitem_t OGL_Fog_MI[]=
{
  {IT_CVAR,    NULL, "Fog",         {&cv_grfog},         0},
  {IT_TEXTBOX, NULL, "Fog color",   {&cv_grfogcolor},   10},
  {IT_CVAR,    NULL, "Fog density", {&cv_grfogdensity}, 20},
};

static menuitem_t OGL_Color_MI[]=
{
  //{IT_STRING | NOTHING, "Gamma correction", NULL                   ,  0},
  {IT_CVAR | IT_CV_SLIDER, NULL, "red"  , {&cv_grgammared}     , 10},
  {IT_CVAR | IT_CV_SLIDER, NULL, "green", {&cv_grgammagreen}   , 20},
  {IT_CVAR | IT_CV_SLIDER, NULL, "blue" , {&cv_grgammablue}    , 30},
  //{IT_CVAR | IT_CV_SLIDER, "Constrast", {&cv_grcontrast} , 50},
};

static menuitem_t OGL_Dev_MI[]=
{
  //    {IT_CVAR, "Polygon smooth"  , {&cv_grpolygonsmooth} ,  0},
  {IT_CVAR, NULL, "MD2 models"      , {&cv_grmd2}              , 10},
  {IT_CVAR, NULL, "Translucent walls", {&cv_grtranswall}       , 20},
};

Menu  OpenGLOptionDef("M_OPTTTL", "OPTIONS", &VideoOptionsDef, ITEMS(OpenGLOptions_MI), 60, 40);
Menu  OGL_LightingDef("M_OPTTTL", "OPTIONS", &OpenGLOptionDef, ITEMS(OGL_Lighting_MI), 60, 40);
Menu  OGL_FogDef("M_OPTTTL", "OPTIONS", &OpenGLOptionDef, ITEMS(OGL_Fog_MI), 60, 40);
Menu  OGL_ColorDef("M_OPTTTL", "OPTIONS", &OpenGLOptionDef, ITEMS(OGL_Color_MI), 60, 40);
Menu  OGL_DevDef("M_OPTTTL", "OPTIONS", &OpenGLOptionDef, ITEMS(OGL_Dev_MI), 60, 40);


/*
void Menu::DrawOpenGLMenu()
{
  DrawMenu(); // use generic drawer for cursor, items and title
  hud_font->DrawString(BASEVIDWIDTH-x-hud_font->StringWidth(cv_scr_depth.str),
    y+currentMenu->items[2].alphaKey, cv_scr_depth.str, V_WHITEMAP | V_SCALE);
}

void Menu::OGL_DrawColor()
{
  DrawMenu(); // use generic drawer for cursor, items and title
  hud_font->DrawString(x, y+currentMenu->items[0].alphaKey-10, "Gamma correction", V_WHITEMAP | V_SCALE);
}
*/

#endif