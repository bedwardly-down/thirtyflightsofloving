#include "g_local.h"
#include "bot.h"

/*
======================================================================

INTERMISSION

======================================================================
*/

void MoveClientToIntermission (edict_t *ent)
{
	if(!(ent->svflags & SVF_MONSTER))
	{
		ent->client->showscores = true;
	//	VectorCopy (level.intermission_origin, ent->s.origin);
		ent->client->ps.pmove.origin[0] = level.intermission_origin[0]*8;
		ent->client->ps.pmove.origin[1] = level.intermission_origin[1]*8;
		ent->client->ps.pmove.origin[2] = level.intermission_origin[2]*8;
		VectorCopy (level.intermission_angle, ent->client->ps.viewangles);
		ent->client->ps.pmove.pm_type = PM_FREEZE;
		ent->client->ps.gunindex = 0;
#ifdef KMQUAKE2_ENGINE_MOD
		ent->client->ps.gunindex2 = 0;
#endif
		ent->client->ps.blend[3] = 0;
	}

	VectorCopy (level.intermission_origin, ent->s.origin);
	// clean up powerup info
	ent->client->quad_framenum = 0;
	ent->client->invincible_framenum = 0;
	ent->client->breather_framenum = 0;
	ent->client->enviro_framenum = 0;
	ent->client->grenade_blew_up = false;
	ent->client->grenade_time = 0;

	// Knightmare- addded double damage
	ent->client->double_framenum = 0;

	// RAFAEL
	ent->client->quadfire_framenum = 0;
	
	// RAFAEL
	ent->client->trap_blew_up = false;
	ent->client->trap_time = 0;

	ent->viewheight = 0;
	ent->s.modelindex = 0;
	ent->s.modelindex2 = 0;
	ent->s.modelindex3 = 0;
	ent->s.modelindex4 = 0;
#ifdef KMQUAKE2_ENGINE_MOD
	ent->s.modelindex5 = 0;
	ent->s.modelindex6 = 0;
#endif
	ent->s.effects = 0;
	ent->s.sound = 0;
	ent->solid = SOLID_NOT;

	// add the layout

	if (deathmatch->value && !(ent->svflags & SVF_MONSTER)) 
	{
		DeathmatchScoreboardMessage (ent, NULL);
		gi.unicast (ent, true);
	}

}
void SetLVChanged ( int i );
void DelBots2(int i);
int GetNumbots ( void );
int GetLVChanged ( void );

void BeginIntermission (edict_t *targ)
{
	int		i;
	edict_t	*ent, *client;

	if (level.intermissiontime)
		return;		// allready activated

//ZOID
	if (deathmatch->value && ctf->value)
		CTFCalcScores();
//ZOID

//	game.autosaved = false;

	// respawn any dead clients
//	for (i=0 ; i<maxclients->value ; i++)
//	{
//		client = g_edicts + 1 + i;
//		if (!client->inuse)
//			continue;
//		if (client->health <= 0)
//			respawn(client);
//	}

	level.intermissiontime = level.time;
	level.changemap = targ->map;

	// if on same unit, return immediately
	if (!deathmatch->value && (targ->map && targ->map[0] != '*') )
	{	// go immediately to the next level
		level.exitintermission = 1;
		return;
	}
	level.exitintermission = 0;

	// find an intermission spot
	ent = G_Find (NULL, FOFS(classname), "info_player_intermission");
	if (!ent)
	{	// the map creator forgot to put in an intermission point...
		ent = G_Find (NULL, FOFS(classname), "info_player_start");
		if (!ent)
			ent = G_Find (NULL, FOFS(classname), "info_player_deathmatch");
	}
	else
	{	// chose one of four spots
		i = rand() & 3;
		while (i--)
		{
			ent = G_Find (ent, FOFS(classname), "info_player_intermission");
			if (!ent)	// wrap around the list
				ent = G_Find (ent, FOFS(classname), "info_player_intermission");
		}
	}

	VectorCopy (ent->s.origin, level.intermission_origin);
	VectorCopy (ent->s.angles, level.intermission_angle);

	// move all clients to the intermission point
	for (i=0 ; i<maxclients->value ; i++)
	{
		client = g_edicts + 1 + i;
		if (!client->inuse)
			continue;
		MoveClientToIntermission (client);
	}
}


/*
==================
DeathmatchScoreboardMessage

==================
*/
void DeathmatchScoreboardMessage (edict_t *ent, edict_t *killer)
{
	char	entry[1024];
	char	string[1400];
	int		stringlength;
	int		i, j, k;
	int		sorted[MAX_CLIENTS];
	int		sortedscores[MAX_CLIENTS];
	int		score, total;
	int		picnum;
	int		x, y;
	gclient_t	*cl;
	edict_t		*cl_ent;
	char	*tag;

//ZOID
	if (ctf->value) {
		CTFScoreboardMessage (ent, killer);
		return;
	}
//ZOID

	// sort the clients by score
	total = 0;
	for (i=0 ; i<game.maxclients ; i++)
	{
		cl_ent = g_edicts + 1 + i;
		if (!cl_ent->inuse)
			continue;
		score = game.clients[i].resp.score;
		for (j=0 ; j<total ; j++)
		{
			if (score > sortedscores[j])
				break;
		}
		for (k=total ; k>j ; k--)
		{
			sorted[k] = sorted[k-1];
			sortedscores[k] = sortedscores[k-1];
		}
		sorted[j] = i;
		sortedscores[j] = score;
		total++;
	}

	// print level name and exit rules
	string[0] = 0;

	stringlength = (int)strlen(string);

	// add the clients in sorted order
	if (total > 12)
		total = 12;

	for (i=0 ; i<total ; i++)
	{
		cl = &game.clients[sorted[i]];
		cl_ent = g_edicts + 1 + sorted[i];

		picnum = gi.imageindex ("i_fixme");
		x = (i>=6) ? 160 : 0;
		y = 32 + 32 * (i%6);

		// add a dogtag
		if (cl_ent == ent)
			tag = "tag1";
		else if (cl_ent == killer)
			tag = "tag2";
		else
			tag = NULL;
		if (tag)
		{
			Com_sprintf (entry, sizeof(entry),
				"xv %i yv %i picn %s ",x+32, y, tag);
			j = (int)strlen(entry);
			if (stringlength + j > 1024)
				break;
		//	strcpy (string + stringlength, entry);
			Com_strcpy (string + stringlength, sizeof(string) - stringlength, entry);
			stringlength += j;
		}

		// send the layout
		Com_sprintf (entry, sizeof(entry),
			"client %i %i %i %i %i %i ",
			x, y, sorted[i], cl->resp.score, cl->ping, (level.framenum - cl->resp.enterframe)/600);
		j = (int)strlen(entry);
		if (stringlength + j > 1024)
			break;
	//	strcpy (string + stringlength, entry);
		Com_strcpy (string + stringlength, sizeof(string) - stringlength, entry);
		stringlength += j;
	}

	gi.WriteByte (svc_layout);
	gi.WriteString (string);
}


/*
==================
DeathmatchScoreboard

Draw instead of help message.
Note that it isn't that hard to overflow the 1400 byte message limit!
==================
*/
void DeathmatchScoreboard (edict_t *ent)
{
	DeathmatchScoreboardMessage (ent, ent->enemy);
	gi.unicast (ent, true);
}


/*
==================
Cmd_Score_f

Display the scoreboard
==================
*/
void Cmd_Score_f (edict_t *ent)
{
	ent->client->showinventory = false;
	ent->client->showhelp = false;

//ZOID
	if (ent->client->menu)
		PMenu_Close(ent);
//ZOID

	if (!deathmatch->value && !coop->value)
		return;

	if (ent->client->showscores)
	{
		ent->client->showscores = false;
		return;
	}

	ent->client->showscores = true;
	DeathmatchScoreboard (ent);
}


/*
==================
HelpComputer

Draw help computer.
==================
*/
void HelpComputer (edict_t *ent)
{
	char	string[1024];
	char	*sk;

	if (skill->value == 0)
		sk = "easy";
	else if (skill->value == 1)
		sk = "medium";
	else if (skill->value == 2)
		sk = "hard";
	else
		sk = "hard+";

	// send the layout
	Com_sprintf (string, sizeof(string),
		"xv 32 yv 8 picn help "			// background
		"xv 202 yv 12 string2 \"%s\" "		// skill
		"xv 0 yv 24 cstring2 \"%s\" "		// level name
		"xv 0 yv 54 cstring2 \"%s\" "		// help 1
		"xv 0 yv 110 cstring2 \"%s\" "		// help 2
		"xv 50 yv 164 string2 \" kills     goals    secrets\" "
		"xv 50 yv 172 string2 \"%3i/%3i     %i/%i       %i/%i\" ", 
		sk,
		level.level_name,
		game.helpmessage1,
		game.helpmessage2,
		level.killed_monsters, level.total_monsters, 
		level.found_goals, level.total_goals,
		level.found_secrets, level.total_secrets);

	gi.WriteByte (svc_layout);
	gi.WriteString (string);
	gi.unicast (ent, true);
}


/*
==================
Cmd_Help_f

Display the current help message
==================
*/
void Cmd_Help_f (edict_t *ent)
{
	// this is for backwards compatability
	if (deathmatch->value)
	{
		Cmd_Score_f (ent);
		return;
	}

	ent->client->showinventory = false;
	ent->client->showscores = false;

	if (ent->client->showhelp && (ent->client->resp.game_helpchanged == game.helpchanged))
	{
		ent->client->showhelp = false;
		return;
	}

	ent->client->showhelp = true;
	ent->client->resp.helpchanged = 0;
	HelpComputer (ent);
}


//=======================================================================

/*
===============
G_SetStats
===============
*/
void G_SetStats (edict_t *ent)
{
	gitem_t		*item;
	int			index, cells;
	int			power_armor_type;

	//
	// health
	//
	ent->client->ps.stats[STAT_HEALTH_ICON] = level.pic_health;
	ent->client->ps.stats[STAT_HEALTH] = ent->health;
#ifdef KMQUAKE2_ENGINE_MOD	// for enhanced HUD
	ent->client->ps.stats[STAT_MAXHEALTH] = min(max(ent->max_health, 0), SHRT_MAX);
#endif

	//
	// ammo
	//
	if (!ent->client->ammo_index /* || !ent->client->pers.inventory[ent->client->ammo_index] */)
	{
		ent->client->ps.stats[STAT_AMMO_ICON] = 0;
		ent->client->ps.stats[STAT_AMMO] = 0;
#ifdef KMQUAKE2_ENGINE_MOD	// for enhanced HUD
		ent->client->ps.stats[STAT_MAXAMMO] = 0;
#endif
	}
	else
	{
		item = &itemlist[ent->client->ammo_index];
		ent->client->ps.stats[STAT_AMMO_ICON] = gi.imageindex (item->icon);
		ent->client->ps.stats[STAT_AMMO] = ent->client->pers.inventory[ent->client->ammo_index];
#ifdef KMQUAKE2_ENGINE_MOD	// for enhanced HUD
		ent->client->ps.stats[STAT_MAXAMMO] = min(max(GetMaxAmmoByIndex(ent->client, ent->client->ammo_index), 0), SHRT_MAX);
#endif
	}
	
	//
	// armor
	//
	power_armor_type = PowerArmorType (ent);
	if (power_armor_type)
	{
		cells = ent->client->pers.inventory[ITEM_INDEX(Fdi_CELLS/*FindItem ("cells")*/)];
		if (cells == 0)
		{	// ran out of cells for power armor
			ent->flags &= ~(FL_POWER_SHIELD|FL_POWER_SCREEN);
			gi.sound(ent, CHAN_ITEM, gi.soundindex("misc/power2.wav"), 1, ATTN_NORM, 0);
			power_armor_type = 0;;
		}
	}

	index = ArmorIndex (ent);
	// Knightmare- show correct icon
	if (power_armor_type && (!index || (level.framenum & 8) ) )
	{	// flash between power armor and other armor icon
		if (power_armor_type == POWER_ARMOR_SHIELD)
			ent->client->ps.stats[STAT_ARMOR_ICON] = gi.imageindex ("i_powershield");
		else	// POWER_ARMOR_SCREEN
			ent->client->ps.stats[STAT_ARMOR_ICON] = gi.imageindex ("i_powerscreen");
		ent->client->ps.stats[STAT_ARMOR] = cells;
#ifdef KMQUAKE2_ENGINE_MOD	// for enhanced HUD
		ent->client->ps.stats[STAT_MAXARMOR] = min(max(ent->client->pers.max_cells, 0), SHRT_MAX);
#endif
	}
	else if (index)
	{
		item = GetItemByIndex (index);
		ent->client->ps.stats[STAT_ARMOR_ICON] = gi.imageindex (item->icon);
		ent->client->ps.stats[STAT_ARMOR] = ent->client->pers.inventory[index];
#ifdef KMQUAKE2_ENGINE_MOD	// for enhanced HUD
//		ent->client->ps.stats[STAT_MAXARMOR] = min(max(ent->client->pers.max_armor, 0), SHRT_MAX);
		ent->client->ps.stats[STAT_MAXARMOR] = min(max(GetMaxArmorByIndex(index), 0), SHRT_MAX);
#endif
	}
	else
	{
		ent->client->ps.stats[STAT_ARMOR_ICON] = 0;
		ent->client->ps.stats[STAT_ARMOR] = 0;
#ifdef KMQUAKE2_ENGINE_MOD	// for enhanced HUD
		ent->client->ps.stats[STAT_MAXARMOR] = 0;
#endif
	}

	//
	// pickup message
	//
	if (level.time > ent->client->pickup_msg_time)
	{
		ent->client->ps.stats[STAT_PICKUP_ICON] = 0;
		ent->client->ps.stats[STAT_PICKUP_STRING] = 0;
	}

	//
	// timers
	//
	if (ent->client->quad_framenum > level.framenum)
	{
		ent->client->ps.stats[STAT_TIMER_ICON] = gi.imageindex ("p_quad");
		ent->client->ps.stats[STAT_TIMER] = (ent->client->quad_framenum - level.framenum)/10;
#ifdef KMQUAKE2_ENGINE_MOD	// for enhanced HUD
		ent->client->ps.stats[STAT_TIMER_RANGE] = min(max((int)sk_quad_time->value, 0), SHRT_MAX);
#endif
	}
	// Knightmare- addded double damage
	else if (ent->client->double_framenum > level.framenum)
	{
		ent->client->ps.stats[STAT_TIMER_ICON] = gi.imageindex ("p_double");
		ent->client->ps.stats[STAT_TIMER] = (ent->client->double_framenum - level.framenum)/10;
#ifdef KMQUAKE2_ENGINE_MOD	// for enhanced HUD
		ent->client->ps.stats[STAT_TIMER_RANGE] = min(max((int)sk_double_time->value, 0), SHRT_MAX);
#endif
	}
	// RAFAEL
	else if (ent->client->quadfire_framenum > level.framenum)
	{
		// note to self
		// need to change imageindex
		ent->client->ps.stats[STAT_TIMER_ICON] = gi.imageindex ("p_quadfire");
		ent->client->ps.stats[STAT_TIMER] = (ent->client->quadfire_framenum - level.framenum)/10;
#ifdef KMQUAKE2_ENGINE_MOD	// for enhanced HUD
		ent->client->ps.stats[STAT_TIMER_RANGE] = min(max((int)sk_quad_fire_time->value, 0), SHRT_MAX);
#endif
	}
	else if (ent->client->invincible_framenum > level.framenum)
	{
		ent->client->ps.stats[STAT_TIMER_ICON] = gi.imageindex ("p_invulnerability");
		ent->client->ps.stats[STAT_TIMER] = (ent->client->invincible_framenum - level.framenum)/10;
#ifdef KMQUAKE2_ENGINE_MOD	// for enhanced HUD
		ent->client->ps.stats[STAT_TIMER_RANGE] = min(max((int)sk_inv_time->value, 0), SHRT_MAX);
#endif
	}
	else if (ent->client->enviro_framenum > level.framenum)
	{
		ent->client->ps.stats[STAT_TIMER_ICON] = gi.imageindex ("p_envirosuit");
		ent->client->ps.stats[STAT_TIMER] = (ent->client->enviro_framenum - level.framenum)/10;
#ifdef KMQUAKE2_ENGINE_MOD	// for enhanced HUD
		ent->client->ps.stats[STAT_TIMER_RANGE] = min(max((int)sk_enviro_time->value, 0), SHRT_MAX);
#endif
	}
	else if (ent->client->breather_framenum > level.framenum)
	{
		ent->client->ps.stats[STAT_TIMER_ICON] = gi.imageindex ("p_rebreather");
		ent->client->ps.stats[STAT_TIMER] = (ent->client->breather_framenum - level.framenum)/10;
#ifdef KMQUAKE2_ENGINE_MOD	// for enhanced HUD
		ent->client->ps.stats[STAT_TIMER_RANGE] = min(max((int)sk_breather_time->value, 0), SHRT_MAX);
#endif
	}
	else
	{
		ent->client->ps.stats[STAT_TIMER_ICON] = 0;
		ent->client->ps.stats[STAT_TIMER] = 0;
	}

	//
	// selected item
	//
	if (ent->client->pers.selected_item == -1)
		ent->client->ps.stats[STAT_SELECTED_ICON] = 0;
	else
		ent->client->ps.stats[STAT_SELECTED_ICON] = gi.imageindex (itemlist[ent->client->pers.selected_item].icon);

	ent->client->ps.stats[STAT_SELECTED_ITEM] = ent->client->pers.selected_item;

	//
	// layouts
	//
	ent->client->ps.stats[STAT_LAYOUTS] = 0;

	if (deathmatch->value)
	{
		if (ent->client->pers.health <= 0 || level.intermissiontime
			|| ent->client->showscores)
			ent->client->ps.stats[STAT_LAYOUTS] |= 1;
		if (ent->client->showinventory && ent->client->pers.health > 0)
			ent->client->ps.stats[STAT_LAYOUTS] |= 2;
	}
	else
	{
		if (ent->client->showscores || ent->client->showhelp)
			ent->client->ps.stats[STAT_LAYOUTS] |= 1;
		if (ent->client->showinventory && ent->client->pers.health > 0)
			ent->client->ps.stats[STAT_LAYOUTS] |= 2;
	}

	//
	// frags
	//
	ent->client->ps.stats[STAT_FRAGS] = ent->client->resp.score;

	//
	// help icon / current weapon if not shown
	//
	if (ent->client->resp.helpchanged && (level.framenum&8) )
		ent->client->ps.stats[STAT_HELPICON] = gi.imageindex ("i_help");
	else if ( (ent->client->pers.hand == CENTER_HANDED || ent->client->ps.fov > 91)
		&& ent->client->pers.weapon)
		ent->client->ps.stats[STAT_HELPICON] = gi.imageindex (ent->client->pers.weapon->icon);
	else
		ent->client->ps.stats[STAT_HELPICON] = 0;

#ifdef KMQUAKE2_ENGINE_MOD	// for enhanced HUD
	if (ent->client->pers.weapon)
		ent->client->ps.stats[STAT_WEAPON] = gi.imageindex (ent->client->pers.weapon->icon);
	else
		ent->client->ps.stats[STAT_WEAPON] = 0;
#endif


//ponpoko
	if(ent->client->zc.aiming == 1)
	{
		ent->client->ps.stats[STAT_SIGHT_PIC] = gi.imageindex ("zsight");
	}
	else if(ent->client->zc.aiming == 3)
	{
		if(ent->client->zc.lockon) ent->client->ps.stats[STAT_SIGHT_PIC] = gi.imageindex ("zsight_l1");
		else ent->client->ps.stats[STAT_SIGHT_PIC] = gi.imageindex ("zsight_l0");
	}
	else ent->client->ps.stats[STAT_SIGHT_PIC] = 0;//gi.imageindex ("i_help"/*"zsight"*/);;
//ponpoko

//ZOID
	SetCTFStats(ent);
//ZOID

}


/*
===============
G_CheckChaseStats
===============
*/
void G_CheckChaseStats (edict_t *ent)
{
	int i;
	gclient_t *cl;

	for (i = 1; i <= maxclients->value; i++) {
		cl = g_edicts[i].client;
		if (!g_edicts[i].inuse || cl->chase_target != ent)
			continue;
		memcpy(cl->ps.stats, ent->client->ps.stats, sizeof(cl->ps.stats));
		G_SetSpectatorStats(g_edicts + i);
	}
}

/*
===============
G_SetSpectatorStats
===============
*/
void G_SetSpectatorStats (edict_t *ent)
{
	gclient_t *cl = ent->client;

	if (!cl->chase_target)
		G_SetStats (ent);

	cl->ps.stats[STAT_SPECTATOR] = 1;

	// layouts are independant in spectator
	cl->ps.stats[STAT_LAYOUTS] = 0;
	if (cl->pers.health <= 0 || level.intermissiontime || cl->showscores)
		cl->ps.stats[STAT_LAYOUTS] |= 1;
	if (cl->showinventory && cl->pers.health > 0)
		cl->ps.stats[STAT_LAYOUTS] |= 2;

	if (cl->chase_target && cl->chase_target->inuse)
		cl->ps.stats[STAT_CHASE] = CS_PLAYERSKINS + 
			(cl->chase_target - g_edicts) - 1;
	else
		cl->ps.stats[STAT_CHASE] = 0;
}

