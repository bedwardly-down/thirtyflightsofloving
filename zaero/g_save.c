
#include "g_local.h"


#if defined(_DEBUG) && defined(_Z_TESTMODE)
void InitTestWeapon(void);
void InitTestItem(void);
#endif

mmove_t mmove_reloc;

#if 1
field_t fields[] = {
	{"classname", FOFS(classname), F_LSTRING},
	{"model", FOFS(model), F_LSTRING},
	{"model2", FOFS(model2), F_LSTRING},
	{"model3", FOFS(model3), F_LSTRING},
	{"model4", FOFS(model4), F_LSTRING},
	{"spawnflags", FOFS(spawnflags), F_INT},
	{"speed", FOFS(speed), F_FLOAT},
	{"accel", FOFS(accel), F_FLOAT},
	{"decel", FOFS(decel), F_FLOAT},
	{"aspeed", FOFS(aspeed), F_FLOAT},
	{"target", FOFS(target), F_LSTRING},
	{"targetname", FOFS(targetname), F_LSTRING},
	{"pathtarget", FOFS(pathtarget), F_LSTRING},
	{"deathtarget", FOFS(deathtarget), F_LSTRING},
	{"killtarget", FOFS(killtarget), F_LSTRING},
	{"combattarget", FOFS(combattarget), F_LSTRING},
	{"message", FOFS(message), F_LSTRING},
	{"team", FOFS(team), F_LSTRING},

	{"client", FOFS(client), F_CLIENT, FFL_NOSPAWN},

	{"wait", FOFS(wait), F_FLOAT},
	{"delay", FOFS(delay), F_FLOAT},
	{"random", FOFS(random), F_FLOAT},
	{"move_origin", FOFS(move_origin), F_VECTOR},
	{"move_angles", FOFS(move_angles), F_VECTOR},
	{"style", FOFS(style), F_INT},
	{"count", FOFS(count), F_INT},
	{"health", FOFS(health), F_INT},
	{"sounds", FOFS(sounds), F_INT},
	{"light", 0, F_IGNORE},
	{"dmg", FOFS(dmg), F_INT},
	{"origin", FOFS(s.origin), F_VECTOR},
	{"angles", FOFS(s.angles), F_VECTOR},
	{"angle", FOFS(s.angles), F_ANGLEHACK},

	// Knightmare- hack for setting alpha, allows mappers to specify
	// an entity's alpha value with the key "salpha"
#ifdef KMQUAKE2_ENGINE_MOD
	{"salpha", FOFS(s.alpha), F_FLOAT},
#endif
	{"musictrack", FOFS(musictrack), F_LSTRING},

	// zaero
	{"mangle", FOFS(mangle), F_VECTOR},
	{"mass", FOFS(mass), F_INT},
	{"volume", FOFS(volume), F_FLOAT},
	{"attenuation", FOFS(attenuation), F_FLOAT},
	{"map", FOFS(map), F_LSTRING},
	{"active", FOFS(active), F_INT},
	{"spawnflags2", FOFS(spawnflags2), F_INT},
	{"mins", FOFS(mins), F_VECTOR},
	{"maxs", FOFS(maxs), F_VECTOR},
	{"mteam", FOFS(mteam), F_LSTRING},
	{"mirrortarget", 0, F_IGNORE},
	{"mirrorlevelsave", 0, F_IGNORE},

	{"goalentity", FOFS(goalentity), F_EDICT, FFL_NOSPAWN},
	{"movetarget", FOFS(movetarget), F_EDICT, FFL_NOSPAWN},
	{"enemy", FOFS(enemy), F_EDICT, FFL_NOSPAWN},
	{"oldenemy", FOFS(oldenemy), F_EDICT, FFL_NOSPAWN},
	{"activator", FOFS(activator), F_EDICT, FFL_NOSPAWN},
	{"groundentity", FOFS(groundentity), F_EDICT, FFL_NOSPAWN},
	{"teamchain", FOFS(teamchain), F_EDICT, FFL_NOSPAWN},
	{"teammaster", FOFS(teammaster), F_EDICT, FFL_NOSPAWN},
	{"owner", FOFS(owner), F_EDICT, FFL_NOSPAWN},
	{"mynoise", FOFS(mynoise), F_EDICT, FFL_NOSPAWN},
	{"mynoise2", FOFS(mynoise2), F_EDICT, FFL_NOSPAWN},
	{"target_ent", FOFS(target_ent), F_EDICT, FFL_NOSPAWN},
	{"chain", FOFS(chain), F_EDICT, FFL_NOSPAWN},
	
	// evolve
	{"laser", FOFS(laser), F_EDICT, FFL_NOSPAWN},
	{"zRaduisList", FOFS(zRaduisList), F_EDICT, FFL_NOSPAWN},
	{"zSchoolChain", FOFS(zSchoolChain), F_EDICT, FFL_NOSPAWN},
	{"rideWith0", FOFS(rideWith[0]), F_EDICT, FFL_NOSPAWN},
	{"rideWith1", FOFS(rideWith[1]), F_EDICT, FFL_NOSPAWN},
//	{"mteam", FOFS(mteam), F_LSTRING},

	{"prethink", FOFS(prethink), F_FUNCTION, FFL_NOSPAWN},
	{"think", FOFS(think), F_FUNCTION, FFL_NOSPAWN},
	{"blocked", FOFS(blocked), F_FUNCTION, FFL_NOSPAWN},
	{"touch", FOFS(touch), F_FUNCTION, FFL_NOSPAWN},
	{"use", FOFS(use), F_FUNCTION, FFL_NOSPAWN},
	{"pain", FOFS(pain), F_FUNCTION, FFL_NOSPAWN},
	{"die", FOFS(die), F_FUNCTION, FFL_NOSPAWN},
	
	{"stand", FOFS(monsterinfo.stand), F_FUNCTION, FFL_NOSPAWN},
	{"idle", FOFS(monsterinfo.idle), F_FUNCTION, FFL_NOSPAWN},
	{"search", FOFS(monsterinfo.search), F_FUNCTION, FFL_NOSPAWN},
	{"walk", FOFS(monsterinfo.walk), F_FUNCTION, FFL_NOSPAWN},
	{"run", FOFS(monsterinfo.run), F_FUNCTION, FFL_NOSPAWN},
	{"dodge", FOFS(monsterinfo.dodge), F_FUNCTION, FFL_NOSPAWN},
	{"attack", FOFS(monsterinfo.attack), F_FUNCTION, FFL_NOSPAWN},
	{"melee", FOFS(monsterinfo.melee), F_FUNCTION, FFL_NOSPAWN},
	{"sight", FOFS(monsterinfo.sight), F_FUNCTION, FFL_NOSPAWN},
	{"checkattack", FOFS(monsterinfo.checkattack), F_FUNCTION, FFL_NOSPAWN},
	{"backwalk", FOFS(monsterinfo.backwalk), F_MMOVE, FFL_NOSPAWN},
	{"sidestepright", FOFS(monsterinfo.sidestepright), F_MMOVE, FFL_NOSPAWN},
	{"sidestepleft", FOFS(monsterinfo.sidestepleft), F_MMOVE, FFL_NOSPAWN},
	{"currentmove", FOFS(monsterinfo.currentmove), F_MMOVE, FFL_NOSPAWN},
	
	{"endfunc", FOFS(moveinfo.endfunc), F_FUNCTION, FFL_NOSPAWN},
	
	// temp spawn vars -- only valid when the spawn function is called
	{"lip", STOFS(lip), F_INT, FFL_SPAWNTEMP},
	{"distance", STOFS(distance), F_INT, FFL_SPAWNTEMP},
	{"height", STOFS(height), F_INT, FFL_SPAWNTEMP},
	{"noise", STOFS(noise), F_LSTRING, FFL_SPAWNTEMP},
	{"pausetime", STOFS(pausetime), F_FLOAT, FFL_SPAWNTEMP},
	{"item", STOFS(item), F_LSTRING, FFL_SPAWNTEMP},
	// Knightmare added
	{"phase", STOFS(phase), F_FLOAT, FFL_SPAWNTEMP},
	{"shift", STOFS(shift), F_FLOAT, FFL_SPAWNTEMP},
	{"skydistance", STOFS(skydistance), F_FLOAT, FFL_SPAWNTEMP},
	{"cloudname", STOFS(cloudname), F_LSTRING, FFL_SPAWNTEMP},
	{"lightningfreq", STOFS(lightningfreq), F_FLOAT, FFL_SPAWNTEMP|FFL_DEFAULT_NEG},
	{"cloudxdir", STOFS(cloudxdir), F_FLOAT, FFL_SPAWNTEMP|FFL_DEFAULT_NEG},
	{"cloudydir", STOFS(cloudydir), F_FLOAT, FFL_SPAWNTEMP|FFL_DEFAULT_NEG},
	{"cloud1tile", STOFS(cloud1tile), F_FLOAT, FFL_SPAWNTEMP|FFL_DEFAULT_NEG},
	{"cloud1speed", STOFS(cloud1speed), F_FLOAT, FFL_SPAWNTEMP|FFL_DEFAULT_NEG},
	{"cloud1alpha", STOFS(cloud1alpha), F_FLOAT, FFL_SPAWNTEMP|FFL_DEFAULT_NEG},
	{"cloud2tile", STOFS(cloud2tile), F_FLOAT, FFL_SPAWNTEMP|FFL_DEFAULT_NEG},
	{"cloud2speed", STOFS(cloud2speed), F_FLOAT, FFL_SPAWNTEMP|FFL_DEFAULT_NEG},
	{"cloud2alpha", STOFS(cloud2alpha), F_FLOAT, FFL_SPAWNTEMP|FFL_DEFAULT_NEG},
	{"cloud3tile", STOFS(cloud3tile), F_FLOAT, FFL_SPAWNTEMP|FFL_DEFAULT_NEG},
	{"cloud3speed", STOFS(cloud3speed), F_FLOAT, FFL_SPAWNTEMP|FFL_DEFAULT_NEG},
	{"cloud3alpha", STOFS(cloud3alpha), F_FLOAT, FFL_SPAWNTEMP|FFL_DEFAULT_NEG},
	{"fade_start_dist", STOFS(fade_start_dist), F_INT, FFL_SPAWNTEMP},
	{"fade_end_dist", STOFS(fade_end_dist), F_INT, FFL_SPAWNTEMP},
	{"image", STOFS(image), F_LSTRING, FFL_SPAWNTEMP},
	{"rgba", STOFS(rgba), F_LSTRING, FFL_SPAWNTEMP},

	//need for item field in edict struct, FFL_SPAWNTEMP item will be skipped on saves
	{"item", FOFS(item), F_ITEM},
	
	{"gravity", STOFS(gravity), F_LSTRING, FFL_SPAWNTEMP},
	{"sky", STOFS(sky), F_LSTRING, FFL_SPAWNTEMP},
	{"skyrotate", STOFS(skyrotate), F_FLOAT, FFL_SPAWNTEMP},
	{"skyaxis", STOFS(skyaxis), F_VECTOR, FFL_SPAWNTEMP},
	{"minyaw", STOFS(minyaw), F_FLOAT, FFL_SPAWNTEMP},
	{"maxyaw", STOFS(maxyaw), F_FLOAT, FFL_SPAWNTEMP},
	{"minpitch", STOFS(minpitch), F_FLOAT, FFL_SPAWNTEMP},
	{"maxpitch", STOFS(maxpitch), F_FLOAT, FFL_SPAWNTEMP},
	{"nextmap", STOFS(nextmap), F_LSTRING, FFL_SPAWNTEMP},

	{"blood_type", FOFS(blood_type), F_INT},

	{0, 0, 0, 0}

};

#else

field_t oldfields[] = {
	{"classname", FOFS(classname), F_LSTRING},
	{"origin", FOFS(s.origin), F_VECTOR},
	{"model", FOFS(model), F_LSTRING},
	{"model2", FOFS(model2), F_LSTRING},
	{"model3", FOFS(model3), F_LSTRING},
	{"model4", FOFS(model4), F_LSTRING},
	{"spawnflags", FOFS(spawnflags), F_INT},
	{"speed", FOFS(speed), F_FLOAT},
	{"accel", FOFS(accel), F_FLOAT},
	{"decel", FOFS(decel), F_FLOAT},
	{"aspeed", FOFS(aspeed), F_FLOAT},
	{"target", FOFS(target), F_LSTRING},
	{"targetname", FOFS(targetname), F_LSTRING},
	{"pathtarget", FOFS(pathtarget), F_LSTRING},
	{"deathtarget", FOFS(deathtarget), F_LSTRING},
	{"killtarget", FOFS(killtarget), F_LSTRING},
	{"combattarget", FOFS(combattarget), F_LSTRING},
	{"message", FOFS(message), F_LSTRING},
	{"team", FOFS(team), F_LSTRING},

	{"wait", FOFS(wait), F_FLOAT},
	{"delay", FOFS(delay), F_FLOAT},
	{"random", FOFS(random), F_FLOAT},
	{"move_origin", FOFS(move_origin), F_VECTOR},
	{"move_angles", FOFS(move_angles), F_VECTOR},
	{"style", FOFS(style), F_INT},
	{"count", FOFS(count), F_INT},
	{"health", FOFS(health), F_INT},
	{"sounds", FOFS(sounds), F_INT},
	{"light", 0, F_IGNORE},
	{"dmg", FOFS(dmg), F_INT},
	{"angles", FOFS(s.angles), F_VECTOR},
	{"angle", FOFS(s.angles), F_ANGLEHACK},

	// Knightmare- hack for setting alpha, allows mappers to specify
	// an entity's alpha value with the key "salpha"
#ifdef KMQUAKE2_ENGINE_MOD
	{"salpha", FOFS(s.alpha), F_FLOAT},
#endif

	// zaero
	{"mangle", FOFS(mangle), F_VECTOR},
	{"mass", FOFS(mass), F_INT},
	{"volume", FOFS(volume), F_FLOAT},
	{"attenuation", FOFS(attenuation), F_FLOAT},
	{"map", FOFS(map), F_LSTRING},
	{"active", FOFS(active), F_INT},
	{"spawnflags2", FOFS(spawnflags2), F_INT},
	{"mins", FOFS(mins), F_VECTOR},
	{"maxs", FOFS(maxs), F_VECTOR},
	{"mteam", FOFS(mteam), F_LSTRING},
	{"mirrortarget", 0, F_IGNORE},
	{"mirrorlevelsave", 0, F_IGNORE},

	// temp spawn vars -- only valid when the spawn function is called
	{"lip", STOFS(lip), F_INT, FFL_SPAWNTEMP},
	{"distance", STOFS(distance), F_INT, FFL_SPAWNTEMP},
	{"height", STOFS(height), F_INT, FFL_SPAWNTEMP},
	{"noise", STOFS(noise), F_LSTRING, FFL_SPAWNTEMP},
	{"pausetime", STOFS(pausetime), F_FLOAT, FFL_SPAWNTEMP},
	{"item", STOFS(item), F_LSTRING, FFL_SPAWNTEMP},

	//need for item field in edict struct, FFL_SPAWNTEMP item will be skipped on saves
	{"item", FOFS(item), F_ITEM},

	{"gravity", STOFS(gravity), F_LSTRING, FFL_SPAWNTEMP},
	{"sky", STOFS(sky), F_LSTRING, FFL_SPAWNTEMP},
	{"skyrotate", STOFS(skyrotate), F_FLOAT, FFL_SPAWNTEMP},
	{"skyaxis", STOFS(skyaxis), F_VECTOR, FFL_SPAWNTEMP},
	{"minyaw", STOFS(minyaw), F_FLOAT, FFL_SPAWNTEMP},
	{"maxyaw", STOFS(maxyaw), F_FLOAT, FFL_SPAWNTEMP},
	{"minpitch", STOFS(minpitch), F_FLOAT, FFL_SPAWNTEMP},
	{"maxpitch", STOFS(maxpitch), F_FLOAT, FFL_SPAWNTEMP},
	{"nextmap", STOFS(nextmap), F_LSTRING, FFL_SPAWNTEMP},

	{"blood_type", FOFS(blood_type), F_INT},

	{0, 0, 0, 0}

};

// -------- just for savegames ----------
// all pointer fields should be listed here, or savegames
// won't work properly (they will crash and burn).
// this wasn't just tacked on to the fields array, because
// these don't need names, we wouldn't want map fields using
// some of these, and if one were accidentally present twice
// it would double swizzle (fuck) the pointer.

field_t		savefields[] =
{
	{"", FOFS(classname), F_LSTRING},
	{"", FOFS(target), F_LSTRING},
	{"", FOFS(targetname), F_LSTRING},
	{"", FOFS(killtarget), F_LSTRING},
	{"", FOFS(team), F_LSTRING},
	{"", FOFS(pathtarget), F_LSTRING},
	{"", FOFS(deathtarget), F_LSTRING},
	{"", FOFS(combattarget), F_LSTRING},
	{"", FOFS(model), F_LSTRING},
	{"", FOFS(model2), F_LSTRING},
	{"", FOFS(model3), F_LSTRING},
	{"", FOFS(model4), F_LSTRING},
	{"", FOFS(map), F_LSTRING},
	{"", FOFS(message), F_LSTRING},

	{"", FOFS(client), F_CLIENT},
	{"", FOFS(item), F_ITEM},

	{"", FOFS(goalentity), F_EDICT},
	{"", FOFS(movetarget), F_EDICT},
	{"", FOFS(enemy), F_EDICT},
	{"", FOFS(oldenemy), F_EDICT},
	{"", FOFS(activator), F_EDICT},
	{"", FOFS(groundentity), F_EDICT},
	{"", FOFS(teamchain), F_EDICT},
	{"", FOFS(teammaster), F_EDICT},
	{"", FOFS(owner), F_EDICT},
	{"", FOFS(mynoise), F_EDICT},
	{"", FOFS(mynoise2), F_EDICT},
	{"", FOFS(target_ent), F_EDICT},
	{"", FOFS(chain), F_EDICT},
	
	// evolve
	{"", FOFS(laser), F_EDICT},
	{"", FOFS(zRaduisList), F_EDICT},
	{"", FOFS(zSchoolChain), F_EDICT},
	{"", FOFS(rideWith[0]), F_EDICT},
	{"", FOFS(rideWith[1]), F_EDICT},
	{"", FOFS(mteam), F_LSTRING},
	{NULL, 0, F_INT}
};
#endif

field_t		levelfields[] =
{
	{"", LLOFS(changemap), F_LSTRING},

	{"", LLOFS(sight_client), F_EDICT},
	{"", LLOFS(sight_entity), F_EDICT},
	{"", LLOFS(sound_entity), F_EDICT},
	{"", LLOFS(sound2_entity), F_EDICT},

	{NULL, 0, F_INT}
};

field_t		clientfields[] =
{
	{"", CLOFS(pers.weapon), F_ITEM},
	{"", CLOFS(pers.lastweapon), F_ITEM},
	{"", CLOFS(pers.lastweapon2), F_ITEM},
	{"", CLOFS(newweapon), F_ITEM},

	// evolve
	{"", CLOFS(zCameraTrack), F_EDICT},
	{"", CLOFS(zCameraLocalEntity), F_EDICT},

	{NULL, 0, F_INT}
};

/*
============
InitGame

This will be called when the dll is first loaded, which
only happens when a new game is started or a save game
is loaded.
============
*/
void InitGame (void)
{
	gi.dprintf ("==== InitGame ====\n");

	// Knightmare- init cvars
	InitLithiumVars ();

	gun_x = gi.cvar ("gun_x", "0", 0);
	gun_y = gi.cvar ("gun_y", "0", 0);
	gun_z = gi.cvar ("gun_z", "0", 0);

	//FIXME: sv_ prefix is wrong for these
	sv_rollspeed = gi.cvar ("sv_rollspeed", "200", 0);
	sv_rollangle = gi.cvar ("sv_rollangle", "2", 0);
	sv_maxvelocity = gi.cvar ("sv_maxvelocity", "2000", 0);
	sv_gravity = gi.cvar ("sv_gravity", "800", 0);

	sv_stopspeed = gi.cvar ("sv_stopspeed", "100", 0);		// PGM - was #define in g_phys.c
	sv_step_fraction = gi.cvar ("sv_step_fraction", "0.90", 0);	// Knightmare- this was a define in p_view.c

	g_aimfix = gi.cvar ("g_aimfix", "0", CVAR_ARCHIVE);								// Knightmare- from Yamagi Q2
	g_aimfix_min_dist = gi.cvar ("g_aimfix_min_dist", "128", CVAR_ARCHIVE);			// Knightmare- minimum range for aimfix
	g_aimfix_taper_dist = gi.cvar ("g_aimfix_taper_dist", "128", CVAR_ARCHIVE);	// Knightmare- transition range for aimfix

	// noset vars
	dedicated = gi.cvar ("dedicated", "0", CVAR_NOSET);

	// latched vars
	sv_cheats = gi.cvar ("cheats", "0", CVAR_SERVERINFO|CVAR_LATCH);
	gi.cvar ("gamename", GAMEVERSION , CVAR_SERVERINFO | CVAR_LATCH);
	gi.cvar ("gamedate", __DATE__ , CVAR_SERVERINFO | CVAR_LATCH);

	maxclients = gi.cvar ("maxclients", "4", CVAR_SERVERINFO | CVAR_LATCH);
	deathmatch = gi.cvar ("deathmatch", "0", CVAR_LATCH);
	coop = gi.cvar ("coop", "0", CVAR_LATCH);
	skill = gi.cvar ("skill", "1", CVAR_LATCH);
//	maxentities = gi.cvar ("maxentities", "1024", CVAR_LATCH);
	maxentities = gi.cvar ("maxentities", va("%i",MAX_EDICTS), CVAR_LATCH);

	// change anytime vars
	dmflags = gi.cvar ("dmflags", "0", CVAR_SERVERINFO);
	zdmflags = gi.cvar ("zdmflags", "0", CVAR_SERVERINFO);
	fraglimit = gi.cvar ("fraglimit", "0", CVAR_SERVERINFO);
	timelimit = gi.cvar ("timelimit", "0", CVAR_SERVERINFO);
	password = gi.cvar ("password", "", CVAR_USERINFO);

	g_select_empty = gi.cvar ("g_select_empty", "0", CVAR_ARCHIVE);

	run_pitch = gi.cvar ("run_pitch", "0.002", 0);
	run_roll = gi.cvar ("run_roll", "0.005", 0);
	bob_up  = gi.cvar ("bob_up", "0.005", 0);
	bob_pitch = gi.cvar ("bob_pitch", "0.002", 0);
	bob_roll = gi.cvar ("bob_roll", "0.002", 0);

	gamedir = gi.cvar ("gamedir", "baseq2", CVAR_SERVERINFO);
#ifdef CACHE_SOUND
	printSoundRejects = gi.cvar("printsoundrejects", "0", CVAR_SERVERINFO);
#endif

#ifndef KMQUAKE2_ENGINE_MOD
    // From Q2Pro- export our own features
    gi.cvar_forceset ("g_features", va("%d", GMF_ENHANCED_SAVEGAMES));
#endif

	// items
	InitItems ();

#if defined(_DEBUG) && defined(_Z_TESTMODE)

	gi.dprintf ("==== InitTestWeapon ====\n");
	InitTestWeapon ();

	gi.dprintf ("==== InitTestItem ====\n");
	InitTestItem ();

#endif

	Com_sprintf (game.helpmessage1, sizeof(game.helpmessage1), "");

	Com_sprintf (game.helpmessage2, sizeof(game.helpmessage2), "");

	// initialize all entities for this game
	game.maxentities = maxentities->value;
	g_edicts =  gi.TagMalloc (game.maxentities * sizeof(g_edicts[0]), TAG_GAME);
	globals.edicts = g_edicts;
	globals.max_edicts = game.maxentities;

	// initialize all clients for this game
	game.maxclients = maxclients->value;
	game.clients = gi.TagMalloc (game.maxclients * sizeof(game.clients[0]), TAG_GAME);
	globals.num_edicts = game.maxclients+1;

	// get at the gl_polyblend client variable
	gi.cvar("gl_polyblend", "1", CVAR_USERINFO);
}

//=========================================================

#ifdef SAVEGAME_USE_FUNCTION_TABLE

#ifdef _MSC_VER
#pragma warning(disable : 4054)	// type cast for function pointers
#endif	// _MSC_VER

#include "z_list.h"

typedef struct {
	char *funcStr;
	byte *funcPtr;
} functionList_t;

typedef struct {
	char	*mmoveStr;
	mmove_t *mmovePtr;
} mmoveList_t;

#include "g_func_decs.h"

functionList_t functionList[] = {
#include "g_func_list.h"
};

#include "g_mmove_decs.h"

mmoveList_t mmoveList[] = {
#include "g_mmove_list.h"
};

functionList_t *GetFunctionByAddress (byte *adr)
{
	int i;
	for (i=0; functionList[i].funcStr; i++)
	{
		if (functionList[i].funcPtr == adr)
			return &functionList[i];
	}
	return NULL;
}

byte *FindFunctionByName (char *name)
{
	int i;
	for (i=0; functionList[i].funcStr; i++)
	{
		if (!strcmp(name, functionList[i].funcStr))
			return functionList[i].funcPtr;
	}
	return NULL;
}

mmoveList_t *GetMmoveByAddress (mmove_t *adr)
{
	int i;
	for (i=0; mmoveList[i].mmoveStr; i++)
	{
		if (mmoveList[i].mmovePtr == adr)
			return &mmoveList[i];
	}
	return NULL;
}

mmove_t *FindMmoveByName (char *name)
{
	int i;
	for (i=0; mmoveList[i].mmoveStr; i++)
	{
		if (!strcmp(name, mmoveList[i].mmoveStr))
			return mmoveList[i].mmovePtr;
	}
	return NULL;
}

#endif // SAVEGAME_USE_FUNCTION_TABLE

//=========================================================

void WriteField1 (FILE *f, field_t *field, byte *base)
{
	void		*p;
	int			len;
	int			index;
#ifdef SAVEGAME_USE_FUNCTION_TABLE
	functionList_t *func;
	mmoveList_t *mmove;
#endif

	if (field->flags & FFL_SPAWNTEMP)
		return;

	p = (void *)(base + field->ofs);
	switch (field->type)
	{
	case F_INT:
	case F_FLOAT:
	case F_ANGLEHACK:
	case F_VECTOR:
	case F_IGNORE:
		break;

	case F_LSTRING:
	case F_GSTRING:
		if ( *(char **)p )
			len = (int)strlen(*(char **)p) + 1;
		else
			len = 0;
		*(int *)p = len;
		break;
	case F_EDICT:
		if ( *(edict_t **)p == NULL)
			index = -1;
		else
			index = *(edict_t **)p - g_edicts;
		*(int *)p = index;
		break;
	case F_CLIENT:
		if ( *(gclient_t **)p == NULL)
			index = -1;
		else
			index = *(gclient_t **)p - game.clients;
		*(int *)p = index;
		break;
	case F_ITEM:
		if ( *(edict_t **)p == NULL)
			index = -1;
		else
			index = *(gitem_t **)p - itemlist;
		*(int *)p = index;
		break;
#ifdef SAVEGAME_USE_FUNCTION_TABLE
	// Matches with an address in the function list, which is generated by extractfuncs.exe.
	// Actual name of function is saved as a string, allowing version-independent savegames.
	case F_FUNCTION:
		if (*(byte **)p == NULL)
			len = 0;
		else
		{
			func = GetFunctionByAddress (*(byte **)p);
			if (!func)
				gi.error ("WriteField1: function not in list, can't save game");
			len = (int)strlen(func->funcStr)+1;
		}
		*(int *)p = len;
		break;
	// Matches with an address in the mmove list, which is generated by extractfuncs.exe.
	// Actual name of mmove is saved as a string, allowing version-independent savegames.
	case F_MMOVE:
		if (*(byte **)p == NULL)
			len = 0;
		else
		{
			mmove = GetMmoveByAddress (*(mmove_t **)p);
			if (!mmove)
				gi.error ("WriteField1: mmove not in list, can't save game");
			len = (int)strlen(mmove->mmoveStr)+1;
		}
		*(int *)p = len;
		break;
#else // SAVEGAME_USE_FUNCTION_TABLE
	//relative to code segment
	case F_FUNCTION:
		if (*(byte **)p == NULL)
			index = 0;
		else
			index = *(byte **)p - ((byte *)InitGame);
		*(int *)p = index;
		break;

	//relative to data segment
	case F_MMOVE:
		if (*(byte **)p == NULL)
			index = 0;
		else
			index = *(byte **)p - (byte *)&mmove_reloc;
		*(int *)p = index;
		break;
#endif // SAVEGAME_USE_FUNCTION_TABLE
	default:
		gi.error ("WriteEdict: unknown field type");
	}
}

void WriteField2 (FILE *f, field_t *field, byte *base)
{
	int			len;
	void		*p;
#ifdef SAVEGAME_USE_FUNCTION_TABLE
	functionList_t *func;
	mmoveList_t *mmove;
#endif

	if (field->flags & FFL_SPAWNTEMP)
		return;

	p = (void *)(base + field->ofs);
	switch (field->type)
	{
	case F_LSTRING:
	case F_GSTRING:
		if ( *(char **)p )
		{
			len = (int)strlen(*(char **)p) + 1;
			fwrite (*(char **)p, len, 1, f);
		}
		break;
#ifdef SAVEGAME_USE_FUNCTION_TABLE
	case F_FUNCTION:
		if ( *(byte **)p )
		{
			func = GetFunctionByAddress (*(byte **)p);
			if (!func)
				gi.error ("WriteField2: function not in list, can't save game");
			len = (int)strlen(func->funcStr)+1;
			fwrite (func->funcStr, len, 1, f);
		}
		break;
	case F_MMOVE:
		if ( *(byte **)p )
		{
			mmove = GetMmoveByAddress (*(mmove_t **)p);
			if (!mmove)
				gi.error ("WriteField2: mmove not in list, can't save game");
			len = (int)strlen(mmove->mmoveStr)+1;
			fwrite (mmove->mmoveStr, len, 1, f);
		}
		break;
#endif
	}
}

void ReadField (FILE *f, field_t *field, byte *base)
{
	void		*p;
	int			len;
	int			index;
#ifdef SAVEGAME_USE_FUNCTION_TABLE
	char		funcStr[512];
#endif

	if (field->flags & FFL_SPAWNTEMP)
		return;

	p = (void *)(base + field->ofs);
	switch (field->type)
	{
	case F_INT:
	case F_FLOAT:
	case F_ANGLEHACK:
	case F_VECTOR:
	case F_IGNORE:
		break;

	case F_LSTRING:
		len = *(int *)p;
		if (!len)
			*(char **)p = NULL;
		else
		{
			*(char **)p = gi.TagMalloc (len, TAG_LEVEL);
			fread (*(char **)p, len, 1, f);
		}
		break;
	case F_GSTRING:
		len = *(int *)p;
		if (!len)
			*(char **)p = NULL;
		else
		{
			*(char **)p = gi.TagMalloc (len, TAG_GAME);
			fread (*(char **)p, len, 1, f);
		}
		break;
	case F_EDICT:
		index = *(int *)p;
		if ( index == -1 )
			*(edict_t **)p = NULL;
		else
			*(edict_t **)p = &g_edicts[index];
		break;
	case F_CLIENT:
		index = *(int *)p;
		if ( index == -1 )
			*(gclient_t **)p = NULL;
		else
			*(gclient_t **)p = &game.clients[index];
		break;
	case F_ITEM:
		index = *(int *)p;
		if ( index == -1 )
			*(gitem_t **)p = NULL;
		else
			*(gitem_t **)p = &itemlist[index];
		break;

#ifdef SAVEGAME_USE_FUNCTION_TABLE
	// Matches with a string in the function list, which is generated by extractfuncs.exe.
	// Actual address of function is loaded from list, allowing version-independent savegames.
	case F_FUNCTION:
		len = *(int *)p;
		if (!len)
			*(byte **)p = NULL;
		else
		{
			if (len > sizeof(funcStr))
				gi.error ("ReadField: function name is longer than buffer (%i chars)", sizeof(funcStr));
			fread (funcStr, len, 1, f);
			if ( !(*(byte **)p = FindFunctionByName (funcStr)) )
				gi.error ("ReadField: function %s not found in table, can't load game", funcStr);
		}
		break;
	// Matches with a string in the mmove list, which is generated by extractfuncs.exe.
	// Actual address of mmove is loaded from list, allowing version-independent savegames.
	case F_MMOVE:
		len = *(int *)p;
		if (!len)
			*(byte **)p = NULL;
		else
		{
			if (len > sizeof(funcStr))
				gi.error ("ReadField: mmove name is longer than buffer (%i chars)", sizeof(funcStr));
			fread (funcStr, len, 1, f);
			if ( !(*(mmove_t **)p = FindMmoveByName (funcStr)) )
				gi.error ("ReadField: mmove %s not found in table, can't load game", funcStr);
		}
		break;
#else // SAVEGAME_USE_FUNCTION_TABLE
	// Relative to code segment
	case F_FUNCTION:
		index = *(int *)p;
		if ( index == 0 )
			*(byte **)p = NULL;
		else
			*(byte **)p = ((byte *)InitGame) + index;
		break;

	// Relative to data segment
	case F_MMOVE:
		index = *(int *)p;
		if (index == 0)
			*(byte **)p = NULL;
		else
			*(byte **)p = (byte *)&mmove_reloc + index;
		break;
#endif // SAVEGAME_USE_FUNCTION_TABLE

	default:
		gi.error ("ReadEdict: unknown field type");
	}
}

//=========================================================

/*
==============
WriteClient

All pointer variables (except function pointers) must be handled specially.
==============
*/
void WriteClient (FILE *f, gclient_t *client)
{
	field_t		*field;
	gclient_t	temp;
	
	// all of the ints, floats, and vectors stay as they are
	temp = *client;

	// change the pointers to lengths or indexes
	for (field=clientfields ; field->name ; field++)
	{
		WriteField1 (f, field, (byte *)&temp);
	}

	// write the block
	fwrite (&temp, sizeof(temp), 1, f);

	// now write any allocated data following the edict
	for (field=clientfields ; field->name ; field++)
	{
		WriteField2 (f, field, (byte *)client);
	}
}

/*
==============
ReadClient

All pointer variables (except function pointers) must be handled specially.
==============
*/
void ReadClient (FILE *f, gclient_t *client)
{
	field_t		*field;

	fread (client, sizeof(*client), 1, f);

	for (field=clientfields ; field->name ; field++)
	{
		ReadField (f, field, (byte *)client);
	}
}

/*
============
WriteGame

This will be called whenever the game goes to a new level,
and when the user explicitly saves the game.

Game information include cross level data, like multi level
triggers, help computer info, and all client states.

A single player death will automatically restore from the
last save position.
============
*/
void WriteGame (char *filename, qboolean autosave)
{
	FILE	*f;
	int		i;
	char	str[16];
#ifdef SAVEGAME_USE_FUNCTION_TABLE
	char	str2[64];
#endif

	if (!autosave)
		SaveClientData ();

	f = fopen (filename, "wb");
	if (!f)
		gi.error ("Couldn't open %s", filename);

	memset (str, 0, sizeof(str));
	Com_strcpy (str, sizeof(str), __DATE__);
	fwrite (str, sizeof(str), 1, f);

#ifdef SAVEGAME_USE_FUNCTION_TABLE
	// use modname and save version for compatibility instead of build date
	memset (str2, 0, sizeof(str2));
	Com_strcpy (str2, sizeof(str2), SAVEGAME_DLLNAME);
	fwrite (str2, sizeof(str2), 1, f);

	i = SAVEGAME_VERSION;
	fwrite (&i, sizeof(i), 1, f);
#endif

	game.autosaved = autosave;
	fwrite (&game, sizeof(game), 1, f);
	game.autosaved = false;

	for (i=0 ; i<game.maxclients ; i++)
		WriteClient (f, &game.clients[i]);

	fclose (f);
}

void ReadGame (char *filename)
{
	FILE	*f;
	int		i;
	char	str[16];
#ifdef SAVEGAME_USE_FUNCTION_TABLE
	char	str2[64];
#endif

	gi.FreeTags (TAG_GAME);
#ifdef CACHE_SOUND
	initSoundList();
#endif
	f = fopen (filename, "rb");
	if (!f)
		gi.error ("Couldn't open %s", filename);

	fread (str, sizeof(str), 1, f);
#ifndef SAVEGAME_USE_FUNCTION_TABLE
	if (strcmp (str, __DATE__))
	{
		fclose (f);
		gi.error ("Savegame from an older version.\n");
	}
#else // SAVEGAME_USE_FUNCTION_TABLE
	// check modname and save version for compatibility instead of build date
	fread (str2, sizeof(str2), 1, f);
	if (strcmp (str2, SAVEGAME_DLLNAME))
	{
		fclose (f);
		gi.error ("Savegame from a different game DLL.\n");
	}

	fread (&i, sizeof(i), 1, f);
	if (i != SAVEGAME_VERSION)
	{
		fclose (f);
		gi.error ("ReadGame: savegame %s is wrong version (%i, should be %i)\n", filename, i, SAVEGAME_VERSION);
	}
#endif // SAVEGAME_USE_FUNCTION_TABLE

	g_edicts =  gi.TagMalloc (game.maxentities * sizeof(g_edicts[0]), TAG_GAME);
	globals.edicts = g_edicts;

	fread (&game, sizeof(game), 1, f);
	game.clients = gi.TagMalloc (game.maxclients * sizeof(game.clients[0]), TAG_GAME);
	for (i=0 ; i<game.maxclients ; i++)
		ReadClient (f, &game.clients[i]);

	fclose (f);
}

//==========================================================



/*
==============
WriteEdict

All pointer variables (except function pointers) must be handled specially.
==============
*/
void WriteEdict (FILE *f, edict_t *ent)
{
	field_t		*field;
	edict_t		temp;

	if (Q_stricmp(ent->classname, "misc_viper") == 0)
	{
		temp = *ent;
	}
	// all of the ints, floats, and vectors stay as they are
	temp = *ent;

#ifdef SAVEGAME_USE_FUNCTION_TABLE // FIXME: remove once this is working reliably
	if (readout->value)
	{
		if (ent->classname && strlen(ent->classname))
			gi.dprintf("WriteEdict: %s\n", ent->classname);
		else
			gi.dprintf("WriteEdict: unknown entity\n");
	}
#endif

	// change the pointers to lengths or indexes
//	for (field=savefields ; field->name ; field++)
	for (field=fields ; field->name ; field++)
	{
		WriteField1 (f, field, (byte *)&temp);
	}

	// write the block
	fwrite (&temp, sizeof(temp), 1, f);

	// now write any allocated data following the edict
//	for (field=savefields ; field->name ; field++)
	for (field=fields ; field->name ; field++)
	{
		WriteField2 (f, field, (byte *)ent);
	}
}

/*
==============
WriteLevelLocals

All pointer variables (except function pointers) must be handled specially.
==============
*/
void WriteLevelLocals (FILE *f)
{
	field_t		*field;
	level_locals_t		temp;

	// all of the ints, floats, and vectors stay as they are
	temp = level;

	// change the pointers to lengths or indexes
	for (field=levelfields ; field->name ; field++)
	{
		WriteField1 (f, field, (byte *)&temp);
	}

	// write the block
	fwrite (&temp, sizeof(temp), 1, f);

	// now write any allocated data following the edict
	for (field=levelfields ; field->name ; field++)
	{
		WriteField2 (f, field, (byte *)&level);
	}
}


/*
==============
ReadEdict

All pointer variables (except function pointers) must be handled specially.
==============
*/
void ReadEdict (FILE *f, edict_t *ent)
{
	field_t		*field;

	fread (ent, sizeof(*ent), 1, f);

//	for (field=savefields ; field->name ; field++)
	for (field=fields ; field->name ; field++)
	{
		ReadField (f, field, (byte *)ent);
	}
}

/*
==============
ReadLevelLocals

All pointer variables (except function pointers) must be handled specially.
==============
*/
void ReadLevelLocals (FILE *f)
{
	field_t		*field;

	fread (&level, sizeof(level), 1, f);

	for (field=levelfields ; field->name ; field++)
	{
		ReadField (f, field, (byte *)&level);
	}
}

//==========================================================



/*
=================
WriteLevel

=================
*/
void WriteLevel (char *filename)
{
	int		i;
	edict_t	*ent;
	FILE	*f;
	void	*base;

	f = fopen (filename, "wb");
	if (!f)
		gi.error ("Couldn't open %s", filename);

	// write out edict size for checking
	i = sizeof(edict_t);
	fwrite (&i, sizeof(i), 1, f);

	// write out a function pointer for checking
	base = (void *)InitGame;
	fwrite (&base, sizeof(base), 1, f);

	// write out level_locals_t
	WriteLevelLocals (f);

	// write out all the entities
	for (i=0 ; i<globals.num_edicts ; i++)
	{
		ent = &g_edicts[i];
		if (!ent->inuse)
			continue;
		fwrite (&i, sizeof(i), 1, f);
		WriteEdict (f, ent);
	}
	i = -1;
	fwrite (&i, sizeof(i), 1, f);

	fclose (f);

  // write out zaero between level 
}


/*
=================
ReadLevel

SpawnEntities will already have been called on the
level the same way it was when the level was saved.

That is necessary to get the baselines
set up identically.

The server will have cleared all of the world links before
calling ReadLevel.

No clients are connected yet.
=================
*/
void ReadLevel (char *filename)
{
	int		entnum;
	FILE	*f;
	int		i;
	void	*base;
	edict_t	*ent;

	f = fopen (filename, "rb");
	if (!f)
		gi.error ("Couldn't open %s", filename);

	// free any dynamic memory allocated by loading the level
	// base state
	gi.FreeTags (TAG_LEVEL);
#ifdef CACHE_SOUND
	initSoundList();
#endif
	// wipe all the entities
	memset (g_edicts, 0, game.maxentities*sizeof(g_edicts[0]));
	globals.num_edicts = maxclients->value+1;

	// check edict size
	fread (&i, sizeof(i), 1, f);
	if (i != sizeof(edict_t))
	{
		fclose (f);
		gi.error ("ReadLevel: mismatched edict size");
	}

	// check function pointer base address
	fread (&base, sizeof(base), 1, f);
	// Removed extraneous check
	/*if (base != (void *)InitGame)
	{
		fclose (f);
		gi.error ("ReadLevel: function pointers have moved");
	}*/

	// load the level locals
	ReadLevelLocals (f);

	// load all the entities
	while (1)
	{
		if (fread (&entnum, sizeof(entnum), 1, f) != 1)
		{
			fclose (f);
			gi.error ("ReadLevel: failed to read entnum");
		}
		if (entnum == -1)
			break;
		if (entnum >= globals.num_edicts)
			globals.num_edicts = entnum+1;

		ent = &g_edicts[entnum];
		ReadEdict (f, ent);

		// let the server rebuild world links for this ent
		memset (&ent->area, 0, sizeof(ent->area));
		gi.linkentity (ent);
	}

	fclose (f);

	// mark all clients as unconnected
	for (i=0 ; i<maxclients->value ; i++)
	{
		ent = &g_edicts[i+1];
		ent->client = game.clients + i;
		ent->client->pers.connected = false;
	}

	// do any load time things at this point
	for (i=0 ; i<globals.num_edicts ; i++)
	{
		ent = &g_edicts[i];

		if (!ent->inuse)
			continue;

		// fire any cross-level triggers
		if (ent->classname)
			if (strcmp(ent->classname, "target_crosslevel_target") == 0)
				ent->nextthink = level.time + ent->delay;
	}

	// Knightmare- reload static cached sounds for entities
	G_SoundcacheEntities ();

	// Knightmare- precache transitioning player inventories here
	// Fixes lag when changing weapons after level transition
	G_PrecachePlayerInventories ();
}
