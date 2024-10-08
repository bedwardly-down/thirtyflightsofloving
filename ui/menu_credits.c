/*
===========================================================================
Copyright (C) 1997-2001 Id Software, Inc.

This file is part of Quake 2 source code.

Quake 2 source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake 2 source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake 2 source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

// menu_game_credits.c -- the credits scroll

#include "../client/client.h"
#include "ui_local.h"

/*
=============================================================================

CREDITS MENU

=============================================================================
*/

static menuFramework_s	s_credits_menu;
static menuTextScroll_s	s_credits_textscroll;
static menuAction_s		s_credits_back_action;

#ifndef NOTTHIRTYFLIGHTS
static int credits_start_time;
static int credits_start_line;
static char *creditsBuffer;
#endif

//=======================================================================

static const char *ui_idcredits[] =
{
#include "menu_credits_id.h"
};

static const char *ui_xatrixcredits[] =
{
#include "menu_credits_xatrix.h"
};

static const char *ui_roguecredits[] =
{
#include "menu_credits_rogue.h"
};

//=======================================================================

void Menu_Credits_Init (void)
{
	s_credits_menu.x				= 0;
	s_credits_menu.y				= 0;
	s_credits_menu.nitems			= 0;
	s_credits_menu.hide_statusbar	= true;
	s_credits_menu.isPopup			= false;
	s_credits_menu.background		= NULL;
	s_credits_menu.drawFunc			= UI_DefaultMenuDraw;
	s_credits_menu.keyFunc			= UI_DefaultMenuKey;
	s_credits_menu.canOpenFunc		= NULL;

	s_credits_textscroll.generic.type			= MTYPE_TEXTSCROLL;
	s_credits_textscroll.generic.x				= 0;
	s_credits_textscroll.generic.y				= 0;
	s_credits_textscroll.generic.textSize		= 10;
	s_credits_textscroll.width					= SCREEN_WIDTH;
	s_credits_textscroll.height					= SCREEN_HEIGHT;
	s_credits_textscroll.lineSize				= 12;
	s_credits_textscroll.time_scale				= 0.025f;
	s_credits_textscroll.fileName				= "credits";
	if ( FS_ModType("xatrix") )			// Xatrix
		s_credits_textscroll.scrollText			= ui_xatrixcredits;
	else if ( FS_ModType("rogue") )		// Rogue
		s_credits_textscroll.scrollText			= ui_roguecredits;
	else
		s_credits_textscroll.scrollText			= ui_idcredits;	
	s_credits_textscroll.generic.isHidden		= false;

	s_credits_back_action.generic.type			= MTYPE_ACTION;
	s_credits_back_action.generic.flags			= QMF_LEFT_JUSTIFY;
	s_credits_back_action.generic.x				= MENU_FONT_SIZE*6;
	s_credits_back_action.generic.y				= 460;
	s_credits_back_action.generic.name			= "Back";
	s_credits_back_action.generic.callback		= UI_BackMenu;

	UI_AddMenuItem (&s_credits_menu, (void *) &s_credits_textscroll);
	UI_AddMenuItem (&s_credits_menu, (void *) &s_credits_back_action);
}

// Forward ported from old engine; probably broken - Brad
int stringLengthExtra (const char *string);
void Menu_Credits_Draw (void)
{
#ifdef NOTTHIRTYFLIGHTS
	float		alpha, time = (cls.realtime - credits_start_time) * 0.05;
	int			i, y, x, len, stringoffset;
	qboolean	bold;

	if ((SCREEN_HEIGHT - ((cls.realtime - credits_start_time)/40.0F)
		+ credits_start_line * MENU_LINE_SIZE) < 0)
	{
		credits_start_line++;
		if (!credits[credits_start_line])
		{
			credits_start_line = 0;
			credits_start_time = cls.realtime;
		}
	}

	//
	// draw the credits
	//
	for (i=credits_start_line, y=SCREEN_HEIGHT - ((cls.realtime - credits_start_time)/40.0F) + credits_start_line * MENU_LINE_SIZE;
		credits[i] && y < SCREEN_HEIGHT; y += MENU_LINE_SIZE, i++)
	{
		stringoffset = 0;
		bold = false;

		if (y <= -MENU_FONT_SIZE)
			continue;
		if (y > SCREEN_HEIGHT)
			continue;

		if (credits[i][0] == '+')
		{
			bold = true;
			stringoffset = 1;
		}
		else
		{
			bold = false;
			stringoffset = 0;
		}

		if (y > SCREEN_HEIGHT*(7.0/8.0))
		{
			float y_test, h_test;
			y_test = y - SCREEN_HEIGHT*(7.0/8.0);
			h_test = SCREEN_HEIGHT/8;

			alpha = 1 - (y_test/h_test);

			alpha = max(min(alpha, 1), 0);
		}
		else if (y < SCREEN_HEIGHT/8)
		{
			float y_test, h_test;
			y_test = y;
			h_test = SCREEN_HEIGHT/8;

			alpha = y_test/h_test;

			alpha = max(min(alpha, 1), 0);
		}
		else
			alpha = 1;

		len = (int)strlen(credits[i]) - stringLengthExtra(credits[i]);

		x = ( SCREEN_WIDTH - len * MENU_FONT_SIZE - stringoffset * MENU_FONT_SIZE ) / 2
			+ stringoffset * MENU_FONT_SIZE;
		UI_DrawString (x, y, MENU_FONT_SIZE, ALIGN_CENTER, credits[i], FONT_UI, alpha*255);
	}
#else
	float alpha, time = (cls.realtime - credits_start_time) * 0.05;
	int i, y, x, len;

	int leftMargin = 20;
	int leftIndent = 30;

	#define Menu_DrawString(a, b, c, d) UI_DrawString(a, b, MENU_FONT_SIZE, ALIGN_CENTER, c, FONT_UI, d)

	Menu_DrawString( leftMargin,  15, "^0Blendo Games presents", 255);
	Menu_DrawString( leftMargin + 175,  15, "CITIZEN ABEL: ^9THIRTY FLIGHTS OF LOVING", 255);

	//float x, float y, float width, float height, scralign_t align, int red, int green, int blue, int alpha)
	//SCR_DrawFill( 25, 60, 550, 4, ALIGN_STRETCH, 255,128,0, 254 );

	Menu_DrawString(leftMargin, 40, "^7CREATED BY", 128);
	Menu_DrawString(leftIndent, 50, " ^7Brendon Chung", 255);	Menu_DrawString(210, 50, "^7blendogames.com", 128);


	//SCR_DrawFill2( 25, 110, 540, 2, ALIGN_STRETCH, 255,128,0, 160 );
	

	Menu_DrawString(leftMargin, 80, "^7POWERED BY", 128);
	Menu_DrawString(leftIndent, 90, "^7Lazarus", 255);		Menu_DrawString(210, 90, "^7David Hyde & Mad Dog", 128);
	Menu_DrawString(leftIndent, 100, "^7KMQuake II", 255);		Menu_DrawString(210, 100, "^7markshan.com/knightmare", 128);
	Menu_DrawString(leftIndent, 110, "^7Quake II", 255);		Menu_DrawString(210, 110, "^7idsoftware.com", 128);


	//SCR_DrawFill2( 25, 180, 540, 2, ALIGN_STRETCH, 255,128,0, 160 );


	// AUDIO

	Menu_DrawString(leftMargin, 140, "^7MUSIC BY", 128);
	Menu_DrawString(leftIndent, 150, "^7Chris Remo", 255);				Menu_DrawString(210, 150, "^7about.me/chrisremo", 128);

	


	Menu_DrawString(leftMargin, 180, "^7SOUND LIBRARY", 128);
	Menu_DrawString(leftIndent, 190, "^7Soundsnap", 255);					Menu_DrawString(210, 190, "^7soundsnap.com", 128);


	Menu_DrawString(leftMargin, 220, "^7ADDITIONAL AUDIO", 128);
	Menu_DrawString(leftIndent, 230, "^7Jared Emerson-Johnson", 255);		Menu_DrawString(210, 230, "^7basound.com", 128);
	Menu_DrawString(leftIndent, 240, "^7AJ Locascio", 255);


	

	Menu_DrawString(leftMargin , 270, "^7SPECIAL THANKS", 128);

	Menu_DrawString(leftIndent , 280, "^7Brian Chan", 255);			Menu_DrawString(210, 280, "^7bchan.org", 128);
	Menu_DrawString(leftIndent , 290, "^7Dustin Darcy", 255);
	Menu_DrawString(leftIndent , 300, "^7MJ Kwan", 255);
	Menu_DrawString(leftIndent , 310, "^7Mimi Kwan", 255);
	Menu_DrawString(leftIndent , 320, "^7Mike Tipul", 255);
	Menu_DrawString(leftIndent , 330, "^7Neil Mehta", 255);
	Menu_DrawString(leftIndent , 340, "^7Rob MacBride", 255);		Menu_DrawString(210, 340, "^7cargocollective.com/robmacbfolio", 128);
	Menu_DrawString(leftIndent , 350, "^7Robert Yang", 255);			Menu_DrawString(210, 350, "^7blog.radiator.debacle.us", 128);
	Menu_DrawString(leftIndent , 360, "^7Sanjay Madhav", 255);
	Menu_DrawString(leftIndent , 370, "^7Ty Taylor", 255);			Menu_DrawString(210, 370, "^7tytaylor.info", 128);
	Menu_DrawString(leftIndent , 380, "^7Wallace Huang", 255);
	Menu_DrawString(leftIndent , 390, "^7Owen Macindoe", 255);		Menu_DrawString(210, 390, "^7people.csail.mit.edu/owenm", 128);



	Menu_DrawString(leftMargin, 410, "^7LINUX, SDL2 PORT", 128);
	Menu_DrawString(leftIndent, 420, "^7Ethan Lee", 255);

	Menu_DrawString(leftMargin , 435, "^7MAC PORT", 128);
	Menu_DrawString(leftIndent , 445, "^7Corwin Light-Williams", 255);

	
	#undef Menu_DrawString
#endif
}


const char *Menu_Credits_Key (int key)
{
	char *sound = NULL;

	switch (key)
	{
#ifndef NOTTHIRTYFLIGHTS

	case K_JOY1: //BC 1/30/2024 gamepad A button
	case K_JOY2: //BC 1/30/2024 gamepad B button
	case K_SPACE:
	case K_ENTER:
#endif
	case K_ESCAPE:
//	case K_MOUSE2:	
		if (creditsBuffer)
			FS_FreeFile (creditsBuffer);
		UI_PopMenu ();
		sound = ui_menu_out_sound;
		break;
	default:
		break;
	}

	return sound;
}


void Menu_Credits_f (void)
{
	Menu_Credits_Init ();
	UI_PushMenu (&s_credits_menu);
}
