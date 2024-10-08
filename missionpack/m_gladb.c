
/*
==============================================================================

	GLADIATOR BOSS

==============================================================================
*/

#include "g_local.h"
#include "m_gladiator.h"

static int	sound_pain1;
static int	sound_pain2;
static int	sound_die;
static int	sound_gun;
static int	sound_cleaver_swing;
static int	sound_cleaver_hit;
static int	sound_cleaver_miss;
static int	sound_idle;
static int	sound_search;
static int	sound_sight;


void gladb_idle (edict_t *self)
{
	if ( !(self->spawnflags & SF_MONSTER_AMBUSH) )
		gi.sound (self, CHAN_VOICE, sound_idle, 1, ATTN_IDLE, 0);
}

void gladb_sight (edict_t *self, edict_t *other)
{
	gi.sound (self, CHAN_VOICE, sound_sight, 1, ATTN_NORM, 0);
}

void gladb_search (edict_t *self)
{
	gi.sound (self, CHAN_VOICE, sound_search, 1, ATTN_NORM, 0);
}

void gladb_cleaver_swing (edict_t *self)
{
	gi.sound (self, CHAN_WEAPON, sound_cleaver_swing, 1, ATTN_NORM, 0);
}

mframe_t gladb_frames_stand [] =
{
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL
};
mmove_t gladb_move_stand = {FRAME_stand1, FRAME_stand7, gladb_frames_stand, NULL};

void gladb_stand (edict_t *self)
{
	self->monsterinfo.currentmove = &gladb_move_stand;
}


mframe_t gladb_frames_walk [] =
{
	ai_walk, 15, NULL,
	ai_walk, 7,  NULL,
	ai_walk, 6,  NULL,
	ai_walk, 5,  NULL,
	ai_walk, 2,  NULL,
	ai_walk, 0,  NULL,
	ai_walk, 2,  NULL,
	ai_walk, 8,  NULL,
	ai_walk, 12, NULL,
	ai_walk, 8,  NULL,
	ai_walk, 5,  NULL,
	ai_walk, 5,  NULL,
	ai_walk, 2,  NULL,
	ai_walk, 2,  NULL,
	ai_walk, 1,  NULL,
	ai_walk, 8,  NULL
};
mmove_t gladb_move_walk = {FRAME_walk1, FRAME_walk16, gladb_frames_walk, NULL};

void gladb_walk (edict_t *self)
{
	self->monsterinfo.currentmove = &gladb_move_walk;
}


mframe_t gladb_frames_run [] =
{
	ai_run, 23,	NULL,
	ai_run, 14,	NULL,
	ai_run, 14,	NULL,
	ai_run, 21,	NULL,
	ai_run, 12,	NULL,
	ai_run, 13,	NULL
};
mmove_t gladb_move_run = {FRAME_run1, FRAME_run6, gladb_frames_run, NULL};

void gladb_run (edict_t *self)
{
	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
		self->monsterinfo.currentmove = &gladb_move_stand;
	else
		self->monsterinfo.currentmove = &gladb_move_run;
}


void GladbMelee (edict_t *self)
{
	vec3_t	aim;

	VectorSet (aim, MELEE_DISTANCE, self->mins[0], -4);
	if (fire_hit (self, aim, (20 + (rand() %5)), 300))
		gi.sound (self, CHAN_AUTO, sound_cleaver_hit, 1, ATTN_NORM, 0);
	else
		gi.sound (self, CHAN_AUTO, sound_cleaver_miss, 1, ATTN_NORM, 0);
}

mframe_t gladb_frames_attack_melee [] =
{
	ai_charge, 0, NULL,
	ai_charge, 0, NULL,
	ai_charge, 0, NULL,
	ai_charge, 0, NULL,
	ai_charge, 0, gladb_cleaver_swing,
	ai_charge, 0, NULL,
	ai_charge, 0, GladbMelee,
	ai_charge, 0, NULL,
	ai_charge, 0, NULL,
	ai_charge, 0, NULL,
	ai_charge, 0, gladb_cleaver_swing,
	ai_charge, 0, NULL,
	ai_charge, 0, NULL,
	ai_charge, 0, GladbMelee,
	ai_charge, 0, NULL,
	ai_charge, 0, NULL,
	ai_charge, 0, NULL
};
mmove_t gladb_move_attack_melee = {FRAME_melee1, FRAME_melee17, gladb_frames_attack_melee, gladb_run};

void gladb_melee(edict_t *self)
{
	self->monsterinfo.currentmove = &gladb_move_attack_melee;
}


void gladbGun (edict_t *self)
{
	vec3_t	start;
	vec3_t	dir;
	vec3_t	forward, right;

	AngleVectors (self->s.angles, forward, right, NULL);
	G_ProjectSource (self->s.origin, monster_flash_offset[MZ2_GLADIATOR_RAILGUN_1], forward, right, start);

	// calc direction to where we targted

	// Lazarus fog reduction of accuracy
	if (self->monsterinfo.visibility < FOG_CANSEEGOOD)
	{
		self->pos1[0] += crandom() * 640 * (FOG_CANSEEGOOD - self->monsterinfo.visibility);
		self->pos1[1] += crandom() * 640 * (FOG_CANSEEGOOD - self->monsterinfo.visibility);
		self->pos1[2] += crandom() * 320 * (FOG_CANSEEGOOD - self->monsterinfo.visibility);
	}

	VectorSubtract (self->pos1, start, dir);
	VectorNormalize (dir);

#ifdef KMQUAKE2_ENGINE_MOD
	monster_fire_phalanx (self, start, dir, 100, 725, 60, 60, MZ2_GLADBETA_PHALANX_1);	// new muzzleflash for KMQ2
#else
	monster_fire_phalanx (self, start, dir, 100, 725, 60, 60, MZ2_GLADIATOR_RAILGUN_1);
#endif	// KMQUAKE2_ENGINE_MOD
}

void gladbGun_check (edict_t *self)
{
	if (skill->value == 3)
		gladbGun (self);
}

mframe_t gladb_frames_attack_gun [] =
{
	ai_charge, 0, NULL,
	ai_charge, 0, NULL,
	ai_charge, 0, gladbGun,
	ai_charge, 0, NULL,
	ai_charge, 0, NULL,
	ai_charge, 0, gladbGun,
	ai_charge, 0, NULL,
	ai_charge, 0, NULL,
	ai_charge, 0, gladbGun_check
};
mmove_t gladb_move_attack_gun = {FRAME_attack1, FRAME_attack9, gladb_frames_attack_gun, gladb_run};

void gladb_attack (edict_t *self)
{
	float	range;
	vec3_t	v;

	// a small safe zone
	VectorSubtract (self->s.origin, self->enemy->s.origin, v);
	range = VectorLength(v);
	if (range <= (MELEE_DISTANCE + 32))
		return;

	// charge up the railgun
	gi.sound (self, CHAN_WEAPON, sound_gun, 1, ATTN_NORM, 0);
	VectorCopy (self->enemy->s.origin, self->pos1);	// save for aiming the shot
	self->pos1[2] += self->enemy->viewheight;
	self->monsterinfo.currentmove = &gladb_move_attack_gun;
}


mframe_t gladb_frames_pain [] =
{
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL
};
mmove_t gladb_move_pain = {FRAME_pain1, FRAME_pain6, gladb_frames_pain, gladb_run};

mframe_t gladb_frames_pain_air [] =
{
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL
};
mmove_t gladb_move_pain_air = {FRAME_painup1, FRAME_painup7, gladb_frames_pain_air, gladb_run};

void gladb_pain (edict_t *self, edict_t *other, float kick, int damage)
{

	if (self->health < (self->max_health / 2))
		self->s.skinnum |= 1;

	if (level.time < self->pain_debounce_time)
	{
		if ((self->velocity[2] > 100) && (self->monsterinfo.currentmove == &gladb_move_pain))
			self->monsterinfo.currentmove = &gladb_move_pain_air;
		return;
	}

	self->pain_debounce_time = level.time + 3;

	if (random() < 0.5)
		gi.sound (self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM, 0);
	else
		gi.sound (self, CHAN_VOICE, sound_pain2, 1, ATTN_NORM, 0);

	if (skill->value == 3)
		return;		// no pain anims in nightmare

	if (self->velocity[2] > 100)
		self->monsterinfo.currentmove = &gladb_move_pain_air;
	else
		self->monsterinfo.currentmove = &gladb_move_pain;
	
}


void gladb_dead (edict_t *self)
{
	VectorSet (self->mins, -16, -16, -24);
	VectorSet (self->maxs, 16, 16, -8);
	self->movetype = MOVETYPE_TOSS;
	self->svflags |= SVF_DEADMONSTER;
	self->nextthink = 0;
	gi.linkentity (self);
	M_FlyCheck (self);

	// Lazarus monster fade
	if (world->effects & FX_WORLDSPAWN_CORPSEFADE)
	{
		self->think=FadeDieSink;
		self->nextthink=level.time+corpse_fadetime->value;
	}
}

mframe_t gladb_frames_death [] =
{
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL
};
mmove_t gladb_move_death = {FRAME_death1, FRAME_death22, gladb_frames_death, gladb_dead};

void gladb_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int damage, vec3_t point)
{
	int		n;

	self->monsterinfo.power_armor_type = POWER_ARMOR_NONE;

	// check for gib
	if (self->health <= self->gib_health && !(self->spawnflags & SF_MONSTER_NOGIB))
	{
		gi.sound (self, CHAN_VOICE, gi.soundindex ("misc/udeath.wav"), 1, ATTN_NORM, 0);
		for (n = 0; n < 2; n++)
			ThrowGib (self, "models/objects/gibs/bone/tris.md2", 0, 0, damage, GIB_ORGANIC);
		for (n = 0; n < 4; n++)
			ThrowGib (self, "models/objects/gibs/sm_meat/tris.md2", 0, 0, damage, GIB_ORGANIC);
		ThrowHead (self, "models/objects/gibs/head2/tris.md2", 0, 0, damage, GIB_ORGANIC);
		self->deadflag = DEAD_DEAD;
		return;
	}

	if (self->deadflag == DEAD_DEAD)
		return;

	// regular death
	gi.sound (self, CHAN_VOICE, sound_die, 1, ATTN_NORM, 0);
	self->s.skinnum |= 1;
	self->deadflag = DEAD_DEAD;
	self->takedamage = DAMAGE_YES;

	self->monsterinfo.currentmove = &gladb_move_death;
}


//===========
//PGM
qboolean gladb_blocked (edict_t *self, float dist)
{
	if (blocked_checkshot (self, 0.25 + (0.05 * skill->value) ))
		return true;

	if (blocked_checkplat (self, dist))
		return true;

	return false;
}
//PGM
//===========


// Knightmare- added soundcache function
void monster_gladb_soundcache (edict_t *self)
{
	sound_pain1 = gi.soundindex ("gladiator/pain.wav");	
	sound_pain2 = gi.soundindex ("gladiator/gldpain2.wav");	
#ifdef CITADELMOD_FEATURES // Knightmare- special death sound for Citadel
	sound_die = gi.soundindex ("gladiator/glddeth3.wav");	
#else
	sound_die = gi.soundindex ("gladiator/glddeth2.wav");	
#endif
	// note to self
	// need to change to PHALANX sound
	sound_gun = gi.soundindex ("weapons/plasshot.wav");

	sound_cleaver_swing = gi.soundindex ("gladiator/melee1.wav");
	sound_cleaver_hit = gi.soundindex ("gladiator/melee2.wav");
	sound_cleaver_miss = gi.soundindex ("gladiator/melee3.wav");
	sound_idle = gi.soundindex ("gladiator/gldidle1.wav");
	sound_search = gi.soundindex ("gladiator/gldsrch1.wav");
	sound_sight = gi.soundindex ("gladiator/sight.wav");
}


/*QUAKED monster_gladb (1 .5 0) (-32 -32 -24) (32 32 40) Ambush Trigger_Spawn Sight GoodGuy NoGib
*/
void SP_monster_gladb (edict_t *self)
{
	if (deathmatch->value)
	{
		G_FreeEdict (self);
		return;
	}

	// Knightmare- use soundcache function
	monster_gladb_soundcache (self);

	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;

	// Lazarus: special purpose skins
	if ( self->style )
	{
		PatchMonsterModel("models/monsters/gladb/tris.md2");
		self->s.skinnum = self->style * 2;
	}

	self->s.modelindex = gi.modelindex ("models/monsters/gladb/tris.md2");
	VectorSet (self->mins, -32, -32, -24);
	VectorSet (self->maxs, 32, 32, 40);

	if (!self->health)
		self->health = 800;
	if (!self->gib_health)
		self->gib_health = -175;
	if (!self->mass)
		self->mass = 350;

	self->pain = gladb_pain;
	self->die = gladb_die;

	self->monsterinfo.stand = gladb_stand;
	self->monsterinfo.walk = gladb_walk;
	self->monsterinfo.run = gladb_run;
	self->monsterinfo.dodge = NULL;
	self->monsterinfo.attack = gladb_attack;
	self->monsterinfo.melee = gladb_melee;
	self->monsterinfo.sight = gladb_sight;
	self->monsterinfo.idle = gladb_idle;
	self->monsterinfo.search = gladb_search;
	self->monsterinfo.blocked = gladb_blocked;		// PGM

	if (!self->blood_type)
		self->blood_type = 3; //sparks and blood

	gi.linkentity (self);

	self->monsterinfo.currentmove = &gladb_move_stand;
	if (self->health < 0)
	{
		mmove_t	*deathmoves[] = {&gladb_move_death,
								 NULL};
		M_SetDeath (self, (mmove_t **)&deathmoves);
	}
	self->monsterinfo.scale = MODEL_SCALE;

	// Lazarus
	if (self->powerarmor)
	{
		if (self->powerarmortype == 1)
			self->monsterinfo.power_armor_type = POWER_ARMOR_SCREEN;
		else
			self->monsterinfo.power_armor_type = POWER_ARMOR_SHIELD;
		self->monsterinfo.power_armor_power = self->powerarmor;
	}
	else
	{
		self->monsterinfo.power_armor_type = POWER_ARMOR_SHIELD;
		self->monsterinfo.power_armor_power = 400;
	}

	if (!self->monsterinfo.flies)
		self->monsterinfo.flies = 0.025;

	self->common_name = "Beta-Class Gladiator";
	self->class_id = ENTITY_MONSTER_GLADIATOR_BETA;

	walkmonster_start (self);
}

