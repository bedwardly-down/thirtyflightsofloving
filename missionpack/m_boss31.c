/*
==============================================================================

jorg

==============================================================================
*/

#include "g_local.h"
#include "m_boss31.h"

void MakronPrecache (edict_t *self);
void SP_monster_makron (edict_t *self);
qboolean visible (edict_t *self, edict_t *other);

static int	sound_pain1;
static int	sound_pain2;
static int	sound_pain3;
static int	sound_idle;
static int	sound_death;
static int	sound_search1;
static int	sound_search2;
static int	sound_search3;
static int	sound_attack1;
static int	sound_attack2;
static int	sound_firegun;
static int	sound_step_left;
static int	sound_step_right;
static int	sound_death_hit;

void JorgExplode (edict_t *self);
void MakronToss (edict_t *self);


void jorg_search (edict_t *self)
{
	float r;

	r = random();

	if (r <= 0.3)
		gi.sound (self, CHAN_VOICE, sound_search1, 1, ATTN_NORM, 0);
	else if (r <= 0.6)
		gi.sound (self, CHAN_VOICE, sound_search2, 1, ATTN_NORM, 0);
	else
		gi.sound (self, CHAN_VOICE, sound_search3, 1, ATTN_NORM, 0);
}


void jorg_dead (edict_t *self);
void jorgBFG (edict_t *self);
void jorgMachineGun (edict_t *self);
void jorg_firebullet (edict_t *self);
void jorg_reattack1(edict_t *self);
void jorg_attack1(edict_t *self);
void jorg_idle(edict_t *self);
void jorg_step_left(edict_t *self);
void jorg_step_right(edict_t *self);
void jorg_death_hit(edict_t *self);

//
// stand
//

mframe_t jorg_frames_stand []=
{
	ai_stand, 0, jorg_idle,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,		// 10
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,		// 20
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,		// 30
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 19, NULL,
	ai_stand, 11, jorg_step_left,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 6, NULL,
	ai_stand, 9, jorg_step_right,
	ai_stand, 0, NULL,		// 40
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, -2, NULL,
	ai_stand, -17, jorg_step_left,
	ai_stand, 0, NULL,
	ai_stand, -12, NULL,		// 50
	ai_stand, -14, jorg_step_right	// 51
};
mmove_t	jorg_move_stand = {FRAME_stand01, FRAME_stand51, jorg_frames_stand, NULL};

void jorg_idle (edict_t *self)
{
	gi.sound (self, CHAN_VOICE, sound_idle, 1, ATTN_NORM,0);
}

void jorg_death_hit (edict_t *self)
{
	gi.sound (self, CHAN_BODY, sound_death_hit, 1, ATTN_NORM,0);
}


void jorg_step_left (edict_t *self)
{
	gi.sound (self, CHAN_BODY, sound_step_left, 1, ATTN_NORM,0);
}

void jorg_step_right (edict_t *self)
{
	gi.sound (self, CHAN_BODY, sound_step_right, 1, ATTN_NORM,0);
}


void jorg_stand (edict_t *self)
{
	self->monsterinfo.currentmove = &jorg_move_stand;
}

mframe_t jorg_frames_run [] =
{
	ai_run, 17,	jorg_step_left,
	ai_run, 0,	NULL,
	ai_run, 0,	NULL,
	ai_run, 0,	NULL,
	ai_run, 12,	NULL,
	ai_run, 8,	NULL,
	ai_run, 10,	NULL,
	ai_run, 33,	jorg_step_right,
	ai_run, 0,	NULL,
	ai_run, 0,	NULL,
	ai_run, 0,	NULL,
	ai_run, 9,	NULL,
	ai_run, 9,	NULL,
	ai_run, 9,	NULL
};
mmove_t	jorg_move_run = {FRAME_walk06, FRAME_walk19, jorg_frames_run, NULL};

//
// walk
//

mframe_t jorg_frames_start_walk [] =
{
	ai_walk,	5,	NULL,
	ai_walk,	6,	NULL,
	ai_walk,	7,	NULL,
	ai_walk,	9,	NULL,
	ai_walk,	15,	NULL
};
mmove_t jorg_move_start_walk = {FRAME_walk01, FRAME_walk05, jorg_frames_start_walk, NULL};

mframe_t jorg_frames_walk [] =
{
	ai_walk, 17,	NULL,
	ai_walk, 0,	NULL,
	ai_walk, 0,	NULL,
	ai_walk, 0,	NULL,
	ai_walk, 12,	NULL,
	ai_walk, 8,	NULL,
	ai_walk, 10,	NULL,
	ai_walk, 33,	NULL,
	ai_walk, 0,	NULL,
	ai_walk, 0,	NULL,
	ai_walk, 0,	NULL,
	ai_walk, 9,	NULL,
	ai_walk, 9,	NULL,
	ai_walk, 9,	NULL
};
mmove_t	jorg_move_walk = {FRAME_walk06, FRAME_walk19, jorg_frames_walk, NULL};

mframe_t jorg_frames_end_walk [] =
{
	ai_walk,	11,	NULL,
	ai_walk,	0,	NULL,
	ai_walk,	0,	NULL,
	ai_walk,	0,	NULL,
	ai_walk,	8,	NULL,
	ai_walk,	-8,	NULL
};
mmove_t jorg_move_end_walk = {FRAME_walk20, FRAME_walk25, jorg_frames_end_walk, NULL};

void jorg_walk (edict_t *self)
{
		self->monsterinfo.currentmove = &jorg_move_walk;
}

void jorg_run (edict_t *self)
{
	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
		self->monsterinfo.currentmove = &jorg_move_stand;
	else
		self->monsterinfo.currentmove = &jorg_move_run;
}

mframe_t jorg_frames_pain3 [] =
{
	ai_move,	-28,	NULL,
	ai_move,	-6,	NULL,
	ai_move,	-3,	jorg_step_left,
	ai_move,	-9,	NULL,
	ai_move,	0,	jorg_step_right,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	-7,	NULL,
	ai_move,	1,	NULL,
	ai_move,	-11,	NULL,
	ai_move,	-4,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	10,	NULL,
	ai_move,	11,	NULL,
	ai_move,	0,	NULL,
	ai_move,	10,	NULL,
	ai_move,	3,	NULL,
	ai_move,	10,	NULL,
	ai_move,	7,	jorg_step_left,
	ai_move,	17,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	jorg_step_right
};
mmove_t jorg_move_pain3 = {FRAME_pain301, FRAME_pain325, jorg_frames_pain3, jorg_run};

mframe_t jorg_frames_pain2 [] =
{
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL
};
mmove_t jorg_move_pain2 = {FRAME_pain201, FRAME_pain203, jorg_frames_pain2, jorg_run};

mframe_t jorg_frames_pain1 [] =
{
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL
};
mmove_t jorg_move_pain1 = {FRAME_pain101, FRAME_pain103, jorg_frames_pain1, jorg_run};

mframe_t jorg_frames_death1 [] =
{
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,		// 10
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,		// 20
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,			
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,			
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,		// 30
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,		// 40
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,			
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,			
	ai_move,	0,	NULL,
	ai_move,	0,	MakronToss,
	ai_move,	0,	jorg_dead // was BossExplode
};
mmove_t jorg_move_death = {FRAME_death01, FRAME_death50, jorg_frames_death1, NULL}; // was jorg_dead

mframe_t jorg_frames_attack2 []=
{
	ai_charge,	0,	NULL,
	ai_charge,	0,	NULL,
	ai_charge,	0,	NULL,
	ai_charge,	0,	NULL,
	ai_charge,	0,	NULL,
	ai_charge,	0,	NULL,
	ai_charge,	0,	jorgBFG,		
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL
};
mmove_t jorg_move_attack2 = {FRAME_attak201, FRAME_attak213, jorg_frames_attack2, jorg_run};

mframe_t jorg_frames_start_attack1 [] =
{
	ai_charge,	0,	NULL,
	ai_charge,	0,	NULL,
	ai_charge,	0,	NULL,
	ai_charge,	0,	NULL,
	ai_charge,	0,	NULL,
	ai_charge,	0,	NULL,
	ai_charge,	0,	NULL,
	ai_charge,	0,	NULL
};
mmove_t jorg_move_start_attack1 = {FRAME_attak101, FRAME_attak108, jorg_frames_start_attack1, jorg_attack1};

mframe_t jorg_frames_attack1[]=
{
	ai_charge,	0,	jorg_firebullet,
	ai_charge,	0,	jorg_firebullet,
	ai_charge,	0,	jorg_firebullet,
	ai_charge,	0,	jorg_firebullet,
	ai_charge,	0,	jorg_firebullet,
	ai_charge,	0,	jorg_firebullet
};
mmove_t jorg_move_attack1 = {FRAME_attak109, FRAME_attak114, jorg_frames_attack1, jorg_reattack1};

mframe_t jorg_frames_end_attack1[]=
{
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL,
	ai_move,	0,	NULL
};
mmove_t jorg_move_end_attack1 = {FRAME_attak115, FRAME_attak118, jorg_frames_end_attack1, jorg_run};

void jorg_reattack1(edict_t *self)
{
	if (visible(self, self->enemy))
		if (random() < 0.9)
			self->monsterinfo.currentmove = &jorg_move_attack1;
		else
		{
			self->s.sound = 0;
			self->monsterinfo.currentmove = &jorg_move_end_attack1;	
		}
	else
	{
		self->s.sound = 0;
		self->monsterinfo.currentmove = &jorg_move_end_attack1;	
	}
}

void jorg_attack1(edict_t *self)
{
	self->monsterinfo.currentmove = &jorg_move_attack1;
}

void jorg_pain (edict_t *self, edict_t *other, float kick, int damage)
{

	if (self->health < (self->max_health / 2))
	{
		self->s.skinnum |= 1;
		if ( !(self->fogclip & 2) ) // custom bloodtype flag check
			self->blood_type = 3; // sparks and blood
	}

	self->s.sound = 0;

	if (level.time < self->pain_debounce_time)
			return;

	// Lessen the chance of him going into his pain frames if he takes little damage
	if (damage <= 40)
		if (random()<=0.6)
			return;

	/* 
	If he's entering his attack1 or using attack1, lessen the chance of him
	going into pain
	*/
	
	if ( (self->s.frame >= FRAME_attak101) && (self->s.frame <= FRAME_attak108) )
		if (random() <= 0.005)
			return;

	if ( (self->s.frame >= FRAME_attak109) && (self->s.frame <= FRAME_attak114) )
		if (random() <= 0.00005)
			return;


	if ( (self->s.frame >= FRAME_attak201) && (self->s.frame <= FRAME_attak208) )
		if (random() <= 0.005)
			return;


	self->pain_debounce_time = level.time + 3;
	if (skill->value == 3)
		return;		// no pain anims in nightmare

	if (damage <= 50)
	{
		gi.sound (self, CHAN_VOICE, sound_pain1, 1, ATTN_NORM,0);
		self->monsterinfo.currentmove = &jorg_move_pain1;
	}
	else if (damage <= 100)
	{
		gi.sound (self, CHAN_VOICE, sound_pain2, 1, ATTN_NORM,0);
		self->monsterinfo.currentmove = &jorg_move_pain2;
	}
	else
	{
		if (random() <= 0.3)
		{
			gi.sound (self, CHAN_VOICE, sound_pain3, 1, ATTN_NORM,0);
			self->monsterinfo.currentmove = &jorg_move_pain3;
		}
	}
};

void jorgBFG (edict_t *self)
{
	vec3_t	forward, right;
	vec3_t	start;
	vec3_t	dir;
	vec3_t	vec;

	AngleVectors (self->s.angles, forward, right, NULL);
	G_ProjectSource (self->s.origin, monster_flash_offset[MZ2_JORG_BFG_1], forward, right, start);

	VectorCopy (self->enemy->s.origin, vec);
	vec[2] += self->enemy->viewheight;

	// Lazarus fog reduction of accuracy
	if (self->monsterinfo.visibility < FOG_CANSEEGOOD)
	{
		vec[0] += crandom() * 640 * (FOG_CANSEEGOOD - self->monsterinfo.visibility);
		vec[1] += crandom() * 640 * (FOG_CANSEEGOOD - self->monsterinfo.visibility);
		vec[2] += crandom() * 320 * (FOG_CANSEEGOOD - self->monsterinfo.visibility);
	}

	VectorSubtract (vec, start, dir);
	VectorNormalize (dir);
	gi.sound (self, CHAN_VOICE, sound_attack2, 1, ATTN_NORM, 0);
	/*void monster_fire_bfg (edict_t *self, 
							 vec3_t start, 
							 vec3_t aimdir, 
							 int damage, 
							 int speed, 
							 int kick, 
							 float damage_radius, 
							 int flashtype)*/
	monster_fire_bfg (self, start, dir, 50, 300, 100, 200, MZ2_JORG_BFG_1, false);
}	

void jorg_firebullet_right (edict_t *self)
{
	vec3_t	forward, right, target;
	vec3_t	start;

	AngleVectors (self->s.angles, forward, right, NULL);
	G_ProjectSource (self->s.origin, monster_flash_offset[MZ2_JORG_MACHINEGUN_R1], forward, right, start);

	VectorMA (self->enemy->s.origin, -0.2, self->enemy->velocity, target);
	target[2] += self->enemy->viewheight;

	// Lazarus fog reduction of accuracy
	if (self->monsterinfo.visibility < FOG_CANSEEGOOD)
	{
		target[0] += crandom() * 640 * (FOG_CANSEEGOOD - self->monsterinfo.visibility);
		target[1] += crandom() * 640 * (FOG_CANSEEGOOD - self->monsterinfo.visibility);
		target[2] += crandom() * 320 * (FOG_CANSEEGOOD - self->monsterinfo.visibility);
	}

	VectorSubtract (target, start, forward);
	VectorNormalize (forward);

	// Zaero add
	if (EMPNukeCheck(self, start))
	{
		gi.sound (self, CHAN_AUTO, gi.soundindex("items/empnuke/emp_missfire.wav"), 1, ATTN_NORM, 0);
		return;
	}
	// end Zaero

	monster_fire_bullet (self, start, forward, 6, 4, DEFAULT_BULLET_HSPREAD, DEFAULT_BULLET_VSPREAD, MZ2_JORG_MACHINEGUN_R1);
}	

void jorg_firebullet_left (edict_t *self)
{
	vec3_t	forward, right, target;
	vec3_t	start;

	AngleVectors (self->s.angles, forward, right, NULL);
	G_ProjectSource (self->s.origin, monster_flash_offset[MZ2_JORG_MACHINEGUN_L1], forward, right, start);

	VectorMA (self->enemy->s.origin, -0.2, self->enemy->velocity, target);
	target[2] += self->enemy->viewheight;

	// Lazarus fog reduction of accuracy
	if (self->monsterinfo.visibility < FOG_CANSEEGOOD)
	{
		target[0] += crandom() * 640 * (FOG_CANSEEGOOD - self->monsterinfo.visibility);
		target[1] += crandom() * 640 * (FOG_CANSEEGOOD - self->monsterinfo.visibility);
		target[2] += crandom() * 320 * (FOG_CANSEEGOOD - self->monsterinfo.visibility);
	}

	// Zaero add
	if (EMPNukeCheck(self, start))
	{
		gi.sound (self, CHAN_AUTO, gi.soundindex("items/empnuke/emp_missfire.wav"), 1, ATTN_NORM, 0);
		return;
	}
	// end Zaero

	VectorSubtract (target, start, forward);
	VectorNormalize (forward);

	monster_fire_bullet (self, start, forward, 6, 4, DEFAULT_BULLET_HSPREAD, DEFAULT_BULLET_VSPREAD, MZ2_JORG_MACHINEGUN_L1);
}	

void jorg_firebullet (edict_t *self)
{
	jorg_firebullet_left(self);
	jorg_firebullet_right(self);
};

void jorg_attack(edict_t *self)
{
	vec3_t	vec;
	float	range;
	
	VectorSubtract (self->enemy->s.origin, self->s.origin, vec);
	range = VectorLength (vec);

	if (random() <= 0.75)
	{
		gi.sound (self, CHAN_VOICE, sound_attack1, 1, ATTN_NORM,0);
		self->s.sound = gi.soundindex ("boss3/w_loop.wav");
	#ifdef KMQUAKE2_ENGINE_MOD
		self->s.loop_attenuation = ATTN_IDLE;
	#endif
		self->monsterinfo.currentmove = &jorg_move_start_attack1;
	}
	else
	{
		gi.sound (self, CHAN_VOICE, sound_attack2, 1, ATTN_NORM,0);
		self->monsterinfo.currentmove = &jorg_move_attack2;
	}
}

// Knightmare- explosions and throw a few gibs, but don't gib body
void JorgExplode (edict_t *self)
{
	vec3_t	org;
	int		n;

	self->think = JorgExplode;
	VectorCopy (self->s.origin, org);
	org[2] += 24 + (rand()&15);
	switch (self->count++)
	{
	case 1:
		org[0] -= 24;
		org[1] -= 24;
		break;
	case 2:
		org[0] += 24;
		org[1] += 24;
		break;
	case 3:
		org[0] += 24;
		org[1] -= 24;
		break;
	case 4:
		org[0] -= 24;
		org[1] += 24;
		break;
	case 5:
		org[0] -= 48;
		org[1] -= 48;
		break;
	case 6:
		org[0] += 48;
		org[1] += 48;
		break;
	case 7:
		org[0] -= 48;
		org[1] += 48;
		break;
	case 8:
		org[0] += 48;
		org[1] -= 48;
		break;
	case 9:
		self->s.sound = 0;

		gi.WriteByte (svc_temp_entity);
		gi.WriteByte (TE_EXPLOSION1);
		gi.WritePosition (org);
		gi.multicast (self->s.origin, MULTICAST_PVS);

		for (n = 0; n < 4; n++)
			ThrowGib (self, "models/objects/gibs/gear/tris.md2", 0, 0, 1500, GIB_METALLIC);
		for (n = 0; n < 4; n++)
			ThrowGib (self, "models/monsters/blackwidow/gib3/tris.md2", 0, 0, 1500, GIB_METALLIC);
		for (n = 0; n < 8; n++)
			ThrowGib (self, "models/objects/gibs/sm_metal/tris.md2", 0, 0, 1500, GIB_METALLIC);
		self->count = 0; //reset counter for gibbing explosion
		return;
	}

	gi.WriteByte (svc_temp_entity);
	gi.WriteByte (TE_EXPLOSION1);
	gi.WritePosition (org);
	gi.multicast (self->s.origin, MULTICAST_PVS);

	self->nextthink = level.time + 0.1;
}


void jorg_dead (edict_t *self)
{	// throw some gibs on death
	if ((self->health > self->gib_health) && !(self->fog_index & 1))
		JorgExplode (self);
	// set dead bbox
	VectorSet (self->mins, -48, -48, 0);
	VectorSet (self->maxs, 48, 48, 40);
	self->movetype = MOVETYPE_TOSS;
	self->svflags |= SVF_DEADMONSTER;
	self->s.frame = FRAME_death50;
	//self->s.modelindex2 = 0;
	gi.linkentity (self);

	// explode if gib flag is set
	if (self->fog_index & 1)
	{
		self->think = BossExplode;
		self->nextthink = level.time + 0.8;
		return;
	}
#if 0
	edict_t	*tempent;
	/*
	VectorSet (self->mins, -16, -16, -24);
	VectorSet (self->maxs, 16, 16, -8);
	*/
	
	// Jorg is on modelindex2. Do not clear him.
	VectorSet (self->mins, -60, -60, 0);
	VectorSet (self->maxs, 60, 60, 72);
	self->movetype = MOVETYPE_TOSS;
	self->nextthink = 0;
	gi.linkentity (self);
	M_FlyCheck (self);

	tempent = G_Spawn();
	VectorCopy (self->s.origin, tempent->s.origin);
	VectorCopy (self->s.angles, tempent->s.angles);
	tempent->killtarget = self->killtarget;
	tempent->target = self->target;
	tempent->activator = self->enemy;
	self->killtarget = 0;
	self->target = 0;
	SP_monster_makron (tempent);
#endif
}


void jorg_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int damage, vec3_t point)
{
	if ( !(self->fogclip & 2) ) // custom bloodtype flag check
		self->blood_type = 3; // sparks and blood
	self->s.sound = 0;
	self->monsterinfo.power_armor_type = POWER_ARMOR_NONE;

	// if killed by a trigger_hurt, set gib flag
	// This is needed because Q2 originally lacked a direct spawn function for Makron.
	// Hence, mappers who wanted to use Makron without Jorg had to kill Jorg with a trigger_hurt.
	// The below causes the dead jorg to gib after going through the death animations, as normal.
	if (!strcmp(inflictor->classname, "trigger_hurt") || !strcmp(inflictor->classname, "trigger_hurt_bbox")
		|| Q_stricmp(level.mapname, "grinsp3f") == 0)  { // gross hack for map6 of COS3- Jorg isn't killed with trigger_hurt
		self->fogclip |= 4; // non-jumping makron
		self->fog_index |= 1; // gib flag
	}

	// check for gib
	if (self->health <= self->gib_health)
	{
		if ( (self->deadflag == DEAD_DEAD) && (self->fogclip & 1) && (self->svflags & SVF_DEADMONSTER) )
			BossExplode (self);	// explode if already dead and have tossed makron
		else
			self->fog_index |= 1; // else set gib flag
	}

	if (self->deadflag == DEAD_DEAD)
		return;

	// regular death
	gi.sound (self, CHAN_VOICE, sound_death, 1, ATTN_NORM, 0);
	self->s.skinnum |= 1;
	self->deadflag = DEAD_DEAD;
	self->takedamage = DAMAGE_YES; // was DAMAGE_NO
	self->count = 0;
	self->monsterinfo.currentmove = &jorg_move_death;
}


qboolean Jorg_CheckAttack (edict_t *self)
{
	vec3_t	spot1, spot2;
	vec3_t	temp;
	float	chance;
	trace_t	tr;
	qboolean	enemy_infront;
	int			enemy_range;
	float		enemy_yaw;

	if (self->enemy->health > 0)
	{
	// see if any entities are in the way of the shot
		VectorCopy (self->s.origin, spot1);
		spot1[2] += self->viewheight;
		VectorCopy (self->enemy->s.origin, spot2);
		spot2[2] += self->enemy->viewheight;

		// Knightmare- don't shoot from behind a window
		tr = gi.trace (spot1, NULL, NULL, spot2, self, CONTENTS_SOLID|CONTENTS_MONSTER|CONTENTS_SLIME|CONTENTS_LAVA|CONTENTS_WINDOW);

		// do we have a clear shot?
		if (tr.ent != self->enemy)
			return false;
	}
	
	enemy_infront = infront(self, self->enemy);
	enemy_range = range(self, self->enemy);
	VectorSubtract (self->enemy->s.origin, self->s.origin, temp);
	enemy_yaw = vectoyaw(temp);

	self->ideal_yaw = enemy_yaw;


	// melee attack
	if (enemy_range == RANGE_MELEE)
	{
		if (self->monsterinfo.melee)
			self->monsterinfo.attack_state = AS_MELEE;
		else
			self->monsterinfo.attack_state = AS_MISSILE;
		return true;
	}
	
// missile attack
	if (!self->monsterinfo.attack)
		return false;
		
	if (level.time < self->monsterinfo.attack_finished)
		return false;
		
	if (enemy_range == RANGE_FAR)
		return false;

	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
	{
		chance = 0.4;
	}
	else if (enemy_range == RANGE_MELEE)
	{
		chance = 0.8;
	}
	else if (enemy_range == RANGE_NEAR)
	{
		chance = 0.4;
	}
	else if (enemy_range == RANGE_MID)
	{
		chance = 0.2;
	}
	else
	{
		return false;
	}

	if (random () < chance)
	{
		self->monsterinfo.attack_state = AS_MISSILE;
		self->monsterinfo.attack_finished = level.time + 2*random();
		return true;
	}

	if (self->flags & FL_FLY)
	{
		if (random() < 0.3)
			self->monsterinfo.attack_state = AS_SLIDING;
		else
			self->monsterinfo.attack_state = AS_STRAIGHT;
	}

	return false;
}


// Knightmare- added soundcache function
void monster_jorg_soundcache (edict_t *self)
{
	sound_pain1 = gi.soundindex ("boss3/bs3pain1.wav");
	sound_pain2 = gi.soundindex ("boss3/bs3pain2.wav");
	sound_pain3 = gi.soundindex ("boss3/bs3pain3.wav");
	sound_death = gi.soundindex ("boss3/bs3deth1.wav");
	sound_attack1 = gi.soundindex ("boss3/bs3atck1.wav");
	sound_attack2 = gi.soundindex ("boss3/bs3atck2.wav");
	sound_search1 = gi.soundindex ("boss3/bs3srch1.wav");
	sound_search2 = gi.soundindex ("boss3/bs3srch2.wav");
	sound_search3 = gi.soundindex ("boss3/bs3srch3.wav");
	sound_idle = gi.soundindex ("boss3/bs3idle1.wav");
	sound_step_left = gi.soundindex ("boss3/step1.wav");
	sound_step_right = gi.soundindex ("boss3/step2.wav");
	sound_firegun = gi.soundindex ("boss3/xfire.wav");
	sound_death_hit = gi.soundindex ("boss3/d_hit.wav");
}

/*QUAKED monster_jorg (1 .5 0) (-80 -80 0) (90 90 140) Ambush Trigger_Spawn Sight GoodGuy NoGib
*/
void SP_monster_jorg (edict_t *self)
{
	if (deathmatch->value)
	{
		G_FreeEdict (self);
		return;
	}

	// Knightmare- use soundcache function
	monster_jorg_soundcache (self);

	MakronPrecache (self);

	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;

	// Lazarus: special purpose skins
	if ( self->style )
	{
		PatchMonsterModel("models/monsters/boss3/jorg/tris.md2");
		self->s.skinnum = self->style * 2;
	//	self->style = 0; //clear for custom bloodtype flag
	}

	self->s.modelindex = gi.modelindex ("models/monsters/boss3/jorg/tris.md2");
	self->s.modelindex2 = gi.modelindex ("models/monsters/boss3/rider/tris.md2");
	VectorSet (self->mins, -80, -80, 0);
	VectorSet (self->maxs, 80, 80, 140);
	// Knightmare- hack for starting in floor
	self->s.origin[2] += 8;

	// Knightmare- gross hack for map6 of COS3- 3000 health
	if (Q_stricmp(level.mapname, "grinsp3f") == 0)
		self->health = 3000;
	else
	{
		if (!self->health)
			self->health = 3000 + 1000 * (skill->value);
	}
	if (coop->value)
		self->health += 500 * (skill->value);
	if (!self->gib_health)
		self->gib_health = -999;
	if (!self->mass)
		self->mass = 1000;

	self->pain = jorg_pain;
	self->die = jorg_die;
	self->monsterinfo.stand = jorg_stand;
	self->monsterinfo.walk = jorg_walk;
	self->monsterinfo.run = jorg_run;
	self->monsterinfo.dodge = NULL;
	self->monsterinfo.attack = jorg_attack;
	self->monsterinfo.search = jorg_search;
	self->monsterinfo.melee = NULL;
	self->monsterinfo.sight = NULL;
	self->monsterinfo.checkattack = Jorg_CheckAttack;
	
	if (!self->blood_type)
		self->blood_type = 2;	// sparks
	else
		self->fogclip |= 2;		// custom bloodtype flag

	gi.linkentity (self);
	
	self->monsterinfo.currentmove = &jorg_move_stand;
	if (self->health < 0)
	{
		mmove_t	*deathmoves[] = {&jorg_move_death,
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

	self->common_name = "Jorg";
	self->class_id = ENTITY_MONSTER_JORG;

	walkmonster_start(self);
	//PMM
	//self->monsterinfo.aiflags |= AI_IGNORE_SHOTS;
	//pmm

}
