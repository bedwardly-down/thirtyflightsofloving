/*
==============================================================================

TANK

==============================================================================
*/

#include "g_local.h"
#include "m_tank.h"

void tank_refire_rocket (edict_t *self);
void tank_doattack_rocket (edict_t *self);
void tank_reattack_blaster (edict_t *self);

static int	sound_thud;
static int	sound_pain;
static int	sound_idle;
static int	sound_die;
static int	sound_step;
static int	sound_sight;
static int	sound_windup;
static int	sound_strike;

//
// misc
//

void tank_sight (edict_t *self, edict_t *other)
{
	gi.sound (self, CHAN_VOICE, sound_sight, 1, ATTN_NORM, 0);
}

void tank_footstep (edict_t *self)
{
	gi.sound (self, CHAN_BODY, sound_step, 1, ATTN_NORM, 0);
}

void tank_thud (edict_t *self)
{
	gi.sound (self, CHAN_BODY, sound_thud, 1, ATTN_NORM, 0);
}

void tank_windup (edict_t *self)
{
	gi.sound (self, CHAN_WEAPON, sound_windup, 1, ATTN_NORM, 0);
}

void tank_idle (edict_t *self)
{
	gi.sound (self, CHAN_VOICE, sound_idle, 1, ATTN_IDLE, 0);
}


//
// stand
//

mframe_t tank_frames_stand []=
{
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL,
	ai_stand, 0, NULL
};
mmove_t	tank_move_stand = {FRAME_stand01, FRAME_stand30, tank_frames_stand, NULL};
	
void tank_stand (edict_t *self)
{
	self->monsterinfo.currentmove = &tank_move_stand;
}


//
// walk
//

void tank_walk (edict_t *self);

mframe_t tank_frames_start_walk [] =
{
	ai_walk,  0, NULL,
	ai_walk,  6, NULL,
	ai_walk,  6, NULL,
	ai_walk, 11, tank_footstep
};
mmove_t	tank_move_start_walk = {FRAME_walk01, FRAME_walk04, tank_frames_start_walk, tank_walk};

mframe_t tank_frames_walk [] =
{
	ai_walk, 4,	NULL,
	ai_walk, 5,	NULL,
	ai_walk, 3,	NULL,
	ai_walk, 2,	NULL,
	ai_walk, 5,	NULL,
	ai_walk, 5,	NULL,
	ai_walk, 4,	NULL,
	ai_walk, 4,	tank_footstep,
	ai_walk, 3,	NULL,
	ai_walk, 5,	NULL,
	ai_walk, 4,	NULL,
	ai_walk, 5,	NULL,
	ai_walk, 7,	NULL,
	ai_walk, 7,	NULL,
	ai_walk, 6,	NULL,
	ai_walk, 6,	tank_footstep
};
mmove_t	tank_move_walk = {FRAME_walk05, FRAME_walk20, tank_frames_walk, NULL};

mframe_t tank_frames_stop_walk [] =
{
	ai_walk,  3, NULL,
	ai_walk,  3, NULL,
	ai_walk,  2, NULL,
	ai_walk,  2, NULL,
	ai_walk,  4, tank_footstep
};
mmove_t	tank_move_stop_walk = {FRAME_walk21, FRAME_walk25, tank_frames_stop_walk, tank_stand};

void tank_walk (edict_t *self)
{
	self->monsterinfo.currentmove = &tank_move_walk;
}


//
// run
//

void tank_run (edict_t *self);

mframe_t tank_frames_start_run [] =
{
	ai_run,  0, NULL,
	ai_run,  6, NULL,
	ai_run,  6, NULL,
	ai_run, 11, tank_footstep
};
mmove_t	tank_move_start_run = {FRAME_walk01, FRAME_walk04, tank_frames_start_run, tank_run};

mframe_t tank_frames_run [] =
{
	ai_run, 4,	NULL,
	ai_run, 5,	NULL,
	ai_run, 3,	NULL,
	ai_run, 2,	NULL,
	ai_run, 5,	NULL,
	ai_run, 5,	NULL,
	ai_run, 4,	NULL,
	ai_run, 4,	tank_footstep,
	ai_run, 3,	NULL,
	ai_run, 5,	NULL,
	ai_run, 4,	NULL,
	ai_run, 5,	NULL,
	ai_run, 7,	NULL,
	ai_run, 7,	NULL,
	ai_run, 6,	NULL,
	ai_run, 6,	tank_footstep
};
mmove_t	tank_move_run = {FRAME_walk05, FRAME_walk20, tank_frames_run, NULL};

mframe_t tank_frames_stop_run [] =
{
	ai_run,  3, NULL,
	ai_run,  3, NULL,
	ai_run,  2, NULL,
	ai_run,  2, NULL,
	ai_run,  4, tank_footstep
};
mmove_t	tank_move_stop_run = {FRAME_walk21, FRAME_walk25, tank_frames_stop_run, tank_walk};

void tank_run (edict_t *self)
{
	if (self->enemy && self->enemy->client)
		self->monsterinfo.aiflags |= AI_BRUTAL;
	else
		self->monsterinfo.aiflags &= ~AI_BRUTAL;

	if (self->monsterinfo.aiflags & AI_STAND_GROUND)
	{
		self->monsterinfo.currentmove = &tank_move_stand;
		return;
	}

	if (self->monsterinfo.currentmove == &tank_move_walk ||
		self->monsterinfo.currentmove == &tank_move_start_run)
	{
		self->monsterinfo.currentmove = &tank_move_run;
	}
	else
	{
		self->monsterinfo.currentmove = &tank_move_start_run;
	}
}

//
// pain
//

mframe_t tank_frames_pain1 [] =
{
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL
};
mmove_t tank_move_pain1 = {FRAME_pain101, FRAME_pain104, tank_frames_pain1, tank_run};

mframe_t tank_frames_pain2 [] =
{
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL,
	ai_move, 0, NULL
};
mmove_t tank_move_pain2 = {FRAME_pain201, FRAME_pain205, tank_frames_pain2, tank_run};

mframe_t tank_frames_pain3 [] =
{
	ai_move, -7, NULL,
	ai_move, 0,  NULL,
	ai_move, 0,  NULL,
	ai_move, 0,  NULL,
	ai_move, 2,  NULL,
	ai_move, 0,  NULL,
	ai_move, 0,  NULL,
	ai_move, 3,  NULL,
	ai_move, 0,  NULL,
	ai_move, 2,  NULL,
	ai_move, 0,  NULL,
	ai_move, 0,  NULL,
	ai_move, 0,  NULL,
	ai_move, 0,  NULL,
	ai_move, 0,  NULL,
	ai_move, 0,  tank_footstep
};
mmove_t	tank_move_pain3 = {FRAME_pain301, FRAME_pain316, tank_frames_pain3, tank_run};


void tank_pain (edict_t *self, edict_t *other, float kick, int damage)
{
	if (self->health < (self->max_health / 2))
	{
		self->s.skinnum |= 1;
		if (!(self->fogclip & 2)) // custom bloodtype flag check
			self->blood_type = 3; // sparks and blood
	}
	if (damage <= 10)
		return;

	if (level.time < self->pain_debounce_time)
		return;

	if (damage <= 30) {
		if (random() > 0.2)
			return;
	}
	
	// If hard or nightmare, don't go into pain while attacking
	if ( skill->value >= 2)
	{
		if ( (self->s.frame >= FRAME_attak301) && (self->s.frame <= FRAME_attak330) )
			return;
		if ( (self->s.frame >= FRAME_attak101) && (self->s.frame <= FRAME_attak116) )
			return;
	}

	self->pain_debounce_time = level.time + 3;
	gi.sound (self, CHAN_VOICE, sound_pain, 1, ATTN_NORM, 0);

	if (skill->value == 3)
		return;		// no pain anims in nightmare

	// PMM - blindfire cleanup
	self->monsterinfo.aiflags &= ~AI_MANUAL_STEERING;
	// pmm

	if (damage <= 30)
		self->monsterinfo.currentmove = &tank_move_pain1;
	else if (damage <= 60)
		self->monsterinfo.currentmove = &tank_move_pain2;
	else
		self->monsterinfo.currentmove = &tank_move_pain3;
};


//
// attacks
//

void TankBlaster (edict_t *self)
{
	vec3_t	forward, right;
	vec3_t	start;
	vec3_t	end;
	vec3_t	dir;
	int		flash_number;

	if (!self->enemy || !self->enemy->inuse)		//PGM
		return;									//PGM

	if (self->s.frame == FRAME_attak110)
		flash_number = MZ2_TANK_BLASTER_1;
	else if (self->s.frame == FRAME_attak113)
		flash_number = MZ2_TANK_BLASTER_2;
	else // (self->s.frame == FRAME_attak116)
		flash_number = MZ2_TANK_BLASTER_3;

	AngleVectors (self->s.angles, forward, right, NULL);
	G_ProjectSource (self->s.origin, monster_flash_offset[flash_number], forward, right, start);

	VectorCopy (self->enemy->s.origin, end);
	end[2] += self->enemy->viewheight;

	// Lazarus fog reduction of accuracy
	if (self->monsterinfo.visibility < FOG_CANSEEGOOD)
	{
		end[0] += crandom() * 640 * (FOG_CANSEEGOOD - self->monsterinfo.visibility);
		end[1] += crandom() * 640 * (FOG_CANSEEGOOD - self->monsterinfo.visibility);
		end[2] += crandom() * 320 * (FOG_CANSEEGOOD - self->monsterinfo.visibility);
	}

	VectorSubtract (end, start, dir);
	monster_fire_blaster (self, start, dir, 30, 800, flash_number, EF_BLASTER, BLASTER_ORANGE);
}	

void TankStrike (edict_t *self)
{
	gi.sound (self, CHAN_WEAPON, sound_strike, 1, ATTN_NORM, 0);
}	

void TankRocket (edict_t *self)
{
	vec3_t	forward, right;
	vec3_t	start;
	vec3_t	dir;
	vec3_t	vec;
	int		flash_number;
	trace_t	trace;				// PGM
	int		rocketSpeed;		// PGM
	// pmm - blindfire support
	vec3_t	target;
	qboolean blindfire = false;

	if (!self->enemy || !self->enemy->inuse)		//PGM
		return;									//PGM

	// pmm - blindfire check
	if (self->monsterinfo.aiflags & AI_MANUAL_STEERING)
		blindfire = true;
	else
		blindfire = false;

	if (self->s.frame == FRAME_attak324)
		flash_number = MZ2_TANK_ROCKET_1;
	else if (self->s.frame == FRAME_attak327)
		flash_number = MZ2_TANK_ROCKET_2;
	else // (self->s.frame == FRAME_attak330)
		flash_number = MZ2_TANK_ROCKET_3;

	AngleVectors (self->s.angles, forward, right, NULL);
	G_ProjectSource (self->s.origin, monster_flash_offset[flash_number], forward, right, start);

	rocketSpeed = 500 + (100 * skill->value);	// PGM rock & roll.... :)

	// PMM
	// Knightmare- no blindfire with homing rockets in easy or medium skill
	if (blindfire && (!(self->spawnflags & SF_MONSTER_SPECIAL) || skill->value >= 2))
		VectorCopy (self->monsterinfo.blind_fire_target, target);
	else
		VectorCopy (self->enemy->s.origin, target);
	// pmm

//	VectorCopy (self->enemy->s.origin, vec);
//	vec[2] += self->enemy->viewheight;
//	VectorSubtract (vec, start, dir);

//PGM
	// PMM - blindfire shooting
	if (blindfire)
	{
		VectorCopy (target, vec);
	}
	// pmm
	// don't shoot at feet if they're above me.
	else if (random() < 0.66 || (start[2] < self->enemy->absmin[2]))
	{
//		gi.dprintf("normal shot\n");
		VectorCopy (self->enemy->s.origin, vec);
		vec[2] += self->enemy->viewheight;
	}
	else
	{
//		gi.dprintf("shooting at feet!\n");
		VectorCopy (self->enemy->s.origin, vec);
		vec[2] = self->enemy->absmin[2];
	}
//PGM
	// Lazarus fog reduction of accuracy
	if (self->monsterinfo.visibility < FOG_CANSEEGOOD)
	{
		vec[0] += crandom() * 640 * (FOG_CANSEEGOOD - self->monsterinfo.visibility);
		vec[1] += crandom() * 640 * (FOG_CANSEEGOOD - self->monsterinfo.visibility);
		vec[2] += crandom() * 320 * (FOG_CANSEEGOOD - self->monsterinfo.visibility);
	}

	VectorSubtract (vec, start, dir);
	// lead target, but not if using homers or blindfiring
	// 20, 35, 50, 65 chance of leading
	// Lazarus: Switched this around from Rogue code... it led target more often
	//          for Easy, which seemed backwards
	if ((!blindfire) && (random() < (0.2 + skill->value * 0.15))
		&& !(self->spawnflags & SF_MONSTER_SPECIAL))
	{
		float	dist;
		float	time;

//		gi.dprintf ("leading target\n");
		dist = VectorLength (dir);
		time = dist/rocketSpeed;
		VectorMA(vec, time, self->enemy->velocity, vec);
		VectorSubtract(vec, start, dir);
	}
//PMM - lead target
//======

	VectorNormalize (dir);

//			gi.WriteByte (svc_temp_entity);
//			gi.WriteByte (TE_DEBUGTRAIL);
//			gi.WritePosition (start);
//			gi.WritePosition (vec);
//			gi.multicast (start, MULTICAST_ALL);

	// pmm blindfire doesn't check target (done in checkattack)
	// paranoia, make sure we're not shooting a target right next to us
	trace = gi.trace(start, vec3_origin, vec3_origin, vec, self, MASK_SHOT);
	if (blindfire)
	{
		// blindfire has different fail criteria for the trace
		if (!(trace.startsolid || trace.allsolid || (trace.fraction < 0.5)))
			monster_fire_rocket (self, start, dir, 50, rocketSpeed, flash_number, NULL);
		else 
		{
			// try shifting the target to the left a little (to help counter large offset)
			VectorCopy (target, vec);
			VectorMA (vec, -20, right, vec);
			VectorSubtract(vec, start, dir);
			VectorNormalize (dir);
			trace = gi.trace(start, vec3_origin, vec3_origin, vec, self, MASK_SHOT);
			if (!(trace.startsolid || trace.allsolid || (trace.fraction < 0.5)))
			//	monster_fire_rocket (self, start, dir, 50, rocketSpeed, flash_number);
				monster_fire_rocket (self, start, dir, 50, rocketSpeed, flash_number,
					(self->spawnflags & SF_MONSTER_SPECIAL ? self->enemy : NULL) );

			else 
			{
				// ok, that failed.  try to the right
				VectorCopy (target, vec);
				VectorMA (vec, 20, right, vec);
				VectorSubtract(vec, start, dir);
				VectorNormalize (dir);
				trace = gi.trace(start, vec3_origin, vec3_origin, vec, self, MASK_SHOT);
				if (!(trace.startsolid || trace.allsolid || (trace.fraction < 0.5)))
				//	monster_fire_rocket (self, start, dir, 50, rocketSpeed, flash_number);
					monster_fire_rocket (self, start, dir, 50, rocketSpeed, flash_number,
						(self->spawnflags & SF_MONSTER_SPECIAL ? self->enemy : NULL) );
				else if ((g_showlogic) && (g_showlogic->value))
					// ok, I give up
					gi.dprintf ("tank avoiding blindfire shot\n");
			}
		}
	}
	else
	{
		trace = gi.trace(start, vec3_origin, vec3_origin, vec, self, MASK_SHOT);
		if (trace.ent == self->enemy || trace.ent == world)
		{
			if (trace.fraction > 0.5 || (trace.ent && trace.ent->client))
			//	monster_fire_rocket (self, start, dir, 50, rocketSpeed, MZ2_CHICK_ROCKET_1);
				monster_fire_rocket (self, start, dir, 50, rocketSpeed, MZ2_CHICK_ROCKET_1,
					(self->spawnflags & SF_MONSTER_SPECIAL ? self->enemy : NULL) );

	//		else
	//			gi.dprintf("didn't make it halfway to target...aborting\n");
		}
	}
}	

void TankMachineGun (edict_t *self)
{
	vec3_t	dir;
	vec3_t	vec;
	vec3_t	start;
	vec3_t	forward, right;
	int		flash_number;

	if (!self->enemy || !self->enemy->inuse)		//PGM
		return;									//PGM

	flash_number = MZ2_TANK_MACHINEGUN_1 + (self->s.frame - FRAME_attak406);

	AngleVectors (self->s.angles, forward, right, NULL);
	G_ProjectSource (self->s.origin, monster_flash_offset[flash_number], forward, right, start);

	if (self->enemy)
	{
		VectorCopy (self->enemy->s.origin, vec);
		vec[2] += self->enemy->viewheight;

		// Lazarus fog reduction of accuracy
		if (self->monsterinfo.visibility < FOG_CANSEEGOOD)
		{
			vec[0] += crandom() * 640 * (FOG_CANSEEGOOD - self->monsterinfo.visibility);
			vec[1] += crandom() * 640 * (FOG_CANSEEGOOD - self->monsterinfo.visibility);
			vec[2] += crandom() * 320 * (FOG_CANSEEGOOD - self->monsterinfo.visibility);
		}

		VectorSubtract (vec, start, vec);
		vectoangles (vec, vec);
		dir[0] = vec[0];
	}
	else
	{
		dir[0] = 0;
	}
	if (self->s.frame <= FRAME_attak415)
		dir[1] = self->s.angles[1] - 8 * (self->s.frame - FRAME_attak411);
	else
		dir[1] = self->s.angles[1] + 8 * (self->s.frame - FRAME_attak419);
	dir[2] = 0;

	AngleVectors (dir, forward, NULL, NULL);

	// Zaero
	if (EMPNukeCheck(self, start))
	{
		gi.sound (self, CHAN_AUTO, gi.soundindex("items/empnuke/emp_missfire.wav"), 1, ATTN_NORM, 0);
		return;
	}
	// end Zaero

	monster_fire_bullet (self, start, forward, 20, 4, DEFAULT_BULLET_HSPREAD, DEFAULT_BULLET_VSPREAD, flash_number);
}	


mframe_t tank_frames_attack_blast [] =
{
	ai_charge, 0,	NULL,
	ai_charge, 0,	NULL,
	ai_charge, 0,	NULL,
	ai_charge, 0,	NULL,
	ai_charge, -1,	NULL,
	ai_charge, -2,	NULL,
	ai_charge, -1,	NULL,
	ai_charge, -1,	NULL,
	ai_charge, 0,	NULL,
	ai_charge, 0,	TankBlaster,		// 10
	ai_charge, 0,	NULL,
	ai_charge, 0,	NULL,
	ai_charge, 0,	TankBlaster,
	ai_charge, 0,	NULL,
	ai_charge, 0,	NULL,
	ai_charge, 0,	TankBlaster			// 16
};
mmove_t tank_move_attack_blast = {FRAME_attak101, FRAME_attak116, tank_frames_attack_blast, tank_reattack_blaster};

mframe_t tank_frames_reattack_blast [] =
{
	ai_charge, 0,	NULL,
	ai_charge, 0,	NULL,
	ai_charge, 0,	TankBlaster,
	ai_charge, 0,	NULL,
	ai_charge, 0,	NULL,
	ai_charge, 0,	TankBlaster			// 16
};
mmove_t tank_move_reattack_blast = {FRAME_attak111, FRAME_attak116, tank_frames_reattack_blast, tank_reattack_blaster};

mframe_t tank_frames_attack_post_blast [] =	
{
	ai_move, 0,		NULL,				// 17
	ai_move, 0,		NULL,
	ai_move, 2,		NULL,
	ai_move, 3,		NULL,
	ai_move, 2,		NULL,
	ai_move, -2,	tank_footstep		// 22
};
mmove_t tank_move_attack_post_blast = {FRAME_attak117, FRAME_attak122, tank_frames_attack_post_blast, tank_run};

void tank_reattack_blaster (edict_t *self)
{
	if (skill->value >= 2)
		if (visible (self, self->enemy))
			if (self->enemy->health > 0)
				if (random() <= 0.6)
				{
					self->monsterinfo.currentmove = &tank_move_reattack_blast;
					return;
				}
	self->monsterinfo.currentmove = &tank_move_attack_post_blast;
}


void tank_poststrike (edict_t *self)
{
	self->enemy = NULL;
	tank_run (self);
}

mframe_t tank_frames_attack_strike [] =
{
	ai_move, 3,   NULL,
	ai_move, 2,   NULL,
	ai_move, 2,   NULL,
	ai_move, 1,   NULL,
	ai_move, 6,   NULL,
	ai_move, 7,   NULL,
	ai_move, 9,   tank_footstep,
	ai_move, 2,   NULL,
	ai_move, 1,   NULL,
	ai_move, 2,   NULL,
	ai_move, 2,   tank_footstep,
	ai_move, 2,   NULL,
	ai_move, 0,   NULL,
	ai_move, 0,   NULL,
	ai_move, 0,   NULL,
	ai_move, 0,   NULL,
	ai_move, -2,  NULL,
	ai_move, -2,  NULL,
	ai_move, 0,   tank_windup,
	ai_move, 0,   NULL,
	ai_move, 0,   NULL,
	ai_move, 0,   NULL,
	ai_move, 0,   NULL,
	ai_move, 0,   NULL,
	ai_move, 0,   NULL,
	ai_move, 0,   TankStrike,
	ai_move, 0,   NULL,
	ai_move, -1,  NULL,
	ai_move, -1,  NULL,
	ai_move, -1,  NULL,
	ai_move, -1,  NULL,
	ai_move, -1,  NULL,
	ai_move, -3,  NULL,
	ai_move, -10, NULL,
	ai_move, -10, NULL,
	ai_move, -2,  NULL,
	ai_move, -3,  NULL,
	ai_move, -2,  tank_footstep
};
mmove_t tank_move_attack_strike = {FRAME_attak201, FRAME_attak238, tank_frames_attack_strike, tank_poststrike};

mframe_t tank_frames_attack_pre_rocket [] =
{
	ai_charge, 0,  NULL,
	ai_charge, 0,  NULL,
	ai_charge, 0,  NULL,
	ai_charge, 0,  NULL,
	ai_charge, 0,  NULL,
	ai_charge, 0,  NULL,
	ai_charge, 0,  NULL,
	ai_charge, 0,  NULL,
	ai_charge, 0,  NULL,
	ai_charge, 0,  NULL,			// 10

	ai_charge, 0,  NULL,
	ai_charge, 1,  NULL,
	ai_charge, 2,  NULL,
	ai_charge, 7,  NULL,
	ai_charge, 7,  NULL,
	ai_charge, 7,  tank_footstep,
	ai_charge, 0,  NULL,
	ai_charge, 0,  NULL,
	ai_charge, 0,  NULL,
	ai_charge, 0,  NULL,			// 20

	ai_charge, -3, NULL
};
mmove_t tank_move_attack_pre_rocket = {FRAME_attak301, FRAME_attak321, tank_frames_attack_pre_rocket, tank_doattack_rocket};

mframe_t tank_frames_attack_fire_rocket [] =
{
	ai_charge, -3, NULL,			// Loop Start	22 
	ai_charge, 0,  NULL,
	ai_charge, 0,  TankRocket,		// 24
	ai_charge, 0,  NULL,
	ai_charge, 0,  NULL,
	ai_charge, 0,  TankRocket,
	ai_charge, 0,  NULL,
	ai_charge, 0,  NULL,
	ai_charge, -1, TankRocket		// 30	Loop End
};
mmove_t tank_move_attack_fire_rocket = {FRAME_attak322, FRAME_attak330, tank_frames_attack_fire_rocket, tank_refire_rocket};

mframe_t tank_frames_attack_post_rocket [] =
{	
	ai_charge, 0,  NULL,			// 31
	ai_charge, -1, NULL,
	ai_charge, -1, NULL,
	ai_charge, 0,  NULL,
	ai_charge, 2,  NULL,
	ai_charge, 3,  NULL,
	ai_charge, 4,  NULL,
	ai_charge, 2,  NULL,
	ai_charge, 0,  NULL,
	ai_charge, 0,  NULL,			// 40

	ai_charge, 0,  NULL,
	ai_charge, -9, NULL,
	ai_charge, -8, NULL,
	ai_charge, -7, NULL,
	ai_charge, -1, NULL,
	ai_charge, -1, tank_footstep,
	ai_charge, 0,  NULL,
	ai_charge, 0,  NULL,
	ai_charge, 0,  NULL,
	ai_charge, 0,  NULL,			// 50

	ai_charge, 0,  NULL,
	ai_charge, 0,  NULL,
	ai_charge, 0,  NULL
};
mmove_t tank_move_attack_post_rocket = {FRAME_attak331, FRAME_attak353, tank_frames_attack_post_rocket, tank_run};

mframe_t tank_frames_attack_chain [] =
{
	ai_charge, 0, NULL,
	ai_charge, 0, NULL,
	ai_charge, 0, NULL,
	ai_charge, 0, NULL,
	ai_charge, 0, NULL,
	NULL,      0, TankMachineGun,
	NULL,      0, TankMachineGun,
	NULL,      0, TankMachineGun,
	NULL,      0, TankMachineGun,
	NULL,      0, TankMachineGun,
	NULL,      0, TankMachineGun,
	NULL,      0, TankMachineGun,
	NULL,      0, TankMachineGun,
	NULL,      0, TankMachineGun,
	NULL,      0, TankMachineGun,
	NULL,      0, TankMachineGun,
	NULL,      0, TankMachineGun,
	NULL,      0, TankMachineGun,
	NULL,      0, TankMachineGun,
	NULL,      0, TankMachineGun,
	NULL,      0, TankMachineGun,
	NULL,      0, TankMachineGun,
	NULL,      0, TankMachineGun,
	NULL,      0, TankMachineGun,
	ai_charge, 0, NULL,
	ai_charge, 0, NULL,
	ai_charge, 0, NULL,
	ai_charge, 0, NULL,
	ai_charge, 0, NULL
};
mmove_t tank_move_attack_chain = {FRAME_attak401, FRAME_attak429, tank_frames_attack_chain, tank_run};

void tank_refire_rocket (edict_t *self)
{
	// PMM - blindfire cleanup
	if (self->monsterinfo.aiflags & AI_MANUAL_STEERING)
	{
		self->monsterinfo.aiflags &= ~AI_MANUAL_STEERING;
		self->monsterinfo.currentmove = &tank_move_attack_post_rocket;
		return;
	}
	// pmm

	// Only on hard or nightmare
	if ( skill->value >= 2 )
		if (self->enemy->health > 0)
			if (visible(self, self->enemy) )
				if (random() <= 0.4)
				{
					self->monsterinfo.currentmove = &tank_move_attack_fire_rocket;
					return;
				}
	self->monsterinfo.currentmove = &tank_move_attack_post_rocket;
}

void tank_doattack_rocket (edict_t *self)
{
	self->monsterinfo.currentmove = &tank_move_attack_fire_rocket;
}

void tank_attack(edict_t *self)
{
	vec3_t	vec;
	float	range;
	float	r;
	// PMM
	float	chance;

	// PMM
	if (!self->enemy || !self->enemy->inuse)
		return;

	if (self->enemy->health < 0)
	{
		self->monsterinfo.currentmove = &tank_move_attack_strike;
		self->monsterinfo.aiflags &= ~AI_BRUTAL;
		return;
	}

	// PMM 
	if (self->monsterinfo.attack_state == AS_BLIND)
	{
		// setup shot probabilities
		if (self->monsterinfo.blind_fire_delay < 1.0)
			chance = 1.0;
		else if (self->monsterinfo.blind_fire_delay < 7.5)
			chance = 0.4;
		else
			chance = 0.1;

		r = random();

		self->monsterinfo.blind_fire_delay += 3.2 + 2.0 + random()*3.0;

		// don't shoot at the origin
		if (VectorCompare (self->monsterinfo.blind_fire_target, vec3_origin))
			return;

		// don't shoot if the dice say not to
		if (r > chance)
		{
//			if ((g_showlogic) && (g_showlogic->value))
//				gi.dprintf ("blindfire - NO SHOT\n");
			return;
		}

		// turn on manual steering to signal both manual steering and blindfire
		self->monsterinfo.aiflags |= AI_MANUAL_STEERING;
		self->monsterinfo.currentmove = &tank_move_attack_fire_rocket;
		self->monsterinfo.attack_finished = level.time + 3.0 + 2*random();
		self->pain_debounce_time = level.time + 5.0;	// no pain for a while
		return;
	}
	// pmm

	VectorSubtract (self->enemy->s.origin, self->s.origin, vec);
	range = VectorLength (vec);

	r = random();

	if (range <= 125)
	{
		if (r < 0.4)
			self->monsterinfo.currentmove = &tank_move_attack_chain;
		else 
			self->monsterinfo.currentmove = &tank_move_attack_blast;
	}
	else if (range <= 250)
	{
		if (r < 0.5)
			self->monsterinfo.currentmove = &tank_move_attack_chain;
		else
			self->monsterinfo.currentmove = &tank_move_attack_blast;
	}
	else
	{
		if (r < 0.33)
			self->monsterinfo.currentmove = &tank_move_attack_chain;
		else if (r < 0.66)
		{
			self->monsterinfo.currentmove = &tank_move_attack_pre_rocket;
			self->pain_debounce_time = level.time + 5.0;	// no pain for a while
		}
		else
			self->monsterinfo.currentmove = &tank_move_attack_blast;
	}
}


//
// death
//

void tank_dead (edict_t *self)
{
	VectorSet (self->mins, -16, -16, -16);
	VectorSet (self->maxs, 16, 16, -0);
	self->movetype = MOVETYPE_TOSS;
	self->svflags |= SVF_DEADMONSTER;
	self->nextthink = 0;
	gi.linkentity (self);
	//M_FlyCheck (self);

	//Knightmare- dead tanks are really too big to fade away
	// Lazarus monster fade
/*	if (world->effects & FX_WORLDSPAWN_CORPSEFADE)
	{
		self->think=FadeDieSink;
		self->nextthink=level.time+corpse_fadetime->value;
	}*/
}

mframe_t tank_frames_death1 [] =
{
	ai_move, -7,  NULL,
	ai_move, -2,  NULL,
	ai_move, -2,  NULL,
	ai_move, 1,   NULL,
	ai_move, 3,   NULL,
	ai_move, 6,   NULL,
	ai_move, 1,   NULL,
	ai_move, 1,   NULL,
	ai_move, 2,   NULL,
	ai_move, 0,   NULL,
	ai_move, 0,   NULL,
	ai_move, 0,   NULL,
	ai_move, -2,  NULL,
	ai_move, 0,   NULL,
	ai_move, 0,   NULL,
	ai_move, -3,  NULL,
	ai_move, 0,   NULL,
	ai_move, 0,   NULL,
	ai_move, 0,   NULL,
	ai_move, 0,   NULL,
	ai_move, 0,   NULL,
	ai_move, 0,   NULL,
	ai_move, -4,  NULL,
	ai_move, -6,  NULL,
	ai_move, -4,  NULL,
	ai_move, -5,  NULL,
	ai_move, -7,  NULL,
	ai_move, -15, tank_thud,
	ai_move, -5,  NULL,
	ai_move, 0,   NULL,
	ai_move, 0,   NULL,
	ai_move, 0,   NULL
};
mmove_t	tank_move_death = {FRAME_death101, FRAME_death132, tank_frames_death1, tank_dead};

#ifdef KMQUAKE2_ENGINE_MOD
#define NUM_SM_MEAT_GIBS		8
#define NUM_SM_METAL_GIBS		16
#else
#define NUM_SM_MEAT_GIBS		4
#define NUM_SM_METAL_GIBS		4
#endif	// KMQUAKE2_ENGINE_MOD

void tank_die (edict_t *self, edict_t *inflictor, edict_t *attacker, int damage, vec3_t point)
{
	int		n; // Knightmare- more gibs

	if (!(self->fogclip & 2)) // custom bloodtype flag check
		self->blood_type = 3; // sparks and blood
	self->monsterinfo.power_armor_type = POWER_ARMOR_NONE;

	// check for gib
	if (self->health <= self->gib_health && !(self->spawnflags & SF_MONSTER_NOGIB))
	{
		gi.sound (self, CHAN_VOICE, gi.soundindex ("misc/udeath.wav"), 1, ATTN_NORM, 0);
		for (n = 0; n < NUM_SM_MEAT_GIBS; n++)
			ThrowGib (self, "models/objects/gibs/sm_meat/tris.md2", 0, 0, damage, GIB_ORGANIC);
		for (n = 0; n < NUM_SM_METAL_GIBS; n++)
			ThrowGib (self, "models/objects/gibs/sm_metal/tris.md2", 0, 0, damage, GIB_METALLIC);
#ifdef KMQUAKE2_ENGINE_MOD
		for (n = 0; n < 8; n++)
			ThrowGib (self, "models/objects/gibs/gear/tris.md2", 0, 0, damage, GIB_METALLIC);
#endif	// KMQUAKE2_ENGINE_MOD
		ThrowGib (self, "models/objects/gibs/chest/tris.md2", 0, 0, damage, GIB_ORGANIC);
		ThrowHead (self, "models/objects/gibs/gear/tris.md2", 0, 0, damage, GIB_METALLIC);
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

	self->monsterinfo.currentmove = &tank_move_death;
	
}

//===========
//PGM
qboolean tank_blocked (edict_t *self, float dist)
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
void monster_tank_soundcache (edict_t *self)
{
	sound_pain = gi.soundindex ("tank/tnkpain2.wav");
	sound_thud = gi.soundindex ("tank/tnkdeth2.wav");
	sound_idle = gi.soundindex ("tank/tnkidle1.wav");
	sound_die = gi.soundindex ("tank/death.wav");
	sound_step = gi.soundindex ("tank/step.wav");
	sound_windup = gi.soundindex ("tank/tnkatck4.wav");
	sound_strike = gi.soundindex ("tank/tnkatck5.wav");
	sound_sight = gi.soundindex ("tank/sight1.wav");
}


//
// monster_tank
//

/*QUAKED monster_tank (1 .5 0) (-32 -32 -16) (32 32 72) Ambush Trigger_Spawn Sight GoodGuy NoGib HomingRockets
*/
/*QUAKED monster_tank_commander (1 .5 0) (-32 -32 -16) (32 32 72) Ambush Trigger_Spawn Sight GoodGuy NoGib HomingRockets
*/
void SP_monster_tank (edict_t *self)
{
	if (deathmatch->value)
	{
		G_FreeEdict (self);
		return;
	}

	// Lazarus: special purpose skins
	if (strcmp(self->classname, "monster_tank_commander") == 0) {
		self->s.skinnum = 2;
	}
	if ( self->style )
	{
		PatchMonsterModel("models/monsters/tank/tris.md2");
		self->s.skinnum += self->style * 4;
	}

	self->s.modelindex = gi.modelindex ("models/monsters/tank/tris.md2");
	VectorSet (self->mins, -32, -32, -16);
	VectorSet (self->maxs, 32, 32, 72);
	self->movetype = MOVETYPE_STEP;
	self->solid = SOLID_BBOX;

	// Knightmare- use soundcache function
	monster_tank_soundcache (self);

	// precache
	gi.soundindex ("tank/tnkatck1.wav");
	gi.soundindex ("tank/tnkatk2a.wav");
	gi.soundindex ("tank/tnkatk2b.wav");
	gi.soundindex ("tank/tnkatk2c.wav");
	gi.soundindex ("tank/tnkatk2d.wav");
	gi.soundindex ("tank/tnkatk2e.wav");
	gi.soundindex ("tank/tnkatck3.wav");

	if (strcmp(self->classname, "monster_tank_commander") == 0)
	{
		if (!self->health)
			self->health = 1000;
		if (!self->gib_health)
			self->gib_health = -400;

		self->common_name = "Tank Commander";

		self->moreflags |= FL2_COMMANDER;
	}
	else
	{
		if (!self->health)
			self->health = 750;
		if (!self->gib_health)
			self->gib_health = -400;

		self->common_name = "Tank";
	}
	self->class_id = ENTITY_MONSTER_TANK;

	if (!self->mass)
		self->mass = 500;

	self->pain = tank_pain;
	self->die = tank_die;
	self->monsterinfo.stand = tank_stand;
	self->monsterinfo.walk = tank_walk;
	self->monsterinfo.run = tank_run;
	self->monsterinfo.dodge = NULL;
	self->monsterinfo.attack = tank_attack;
	self->monsterinfo.melee = NULL;
	self->monsterinfo.sight = tank_sight;
	self->monsterinfo.idle = tank_idle;
	self->monsterinfo.blocked = tank_blocked;		// PGM

	if (!self->blood_type)
		self->blood_type = 2;	// sparks
	else
		self->fogclip |= 2;		// custom bloodtype flag

	// Lazarus
	if (self->powerarmor)
	{
		if (self->powerarmortype == 1)
			self->monsterinfo.power_armor_type = POWER_ARMOR_SCREEN;
		else
			self->monsterinfo.power_armor_type = POWER_ARMOR_SHIELD;
		self->monsterinfo.power_armor_power = self->powerarmor;
	}

	if (!self->monsterinfo.flies)
		self->monsterinfo.flies = 0.05;

	gi.linkentity (self);
	
	self->monsterinfo.currentmove = &tank_move_stand;
	if (self->health < 0)
	{
		mmove_t	*deathmoves[] = {&tank_move_death,
								 NULL};
		M_SetDeath (self, (mmove_t **)&deathmoves);
	}
	self->monsterinfo.scale = MODEL_SCALE;

	walkmonster_start(self);

	// PMM
//	self->monsterinfo.aiflags |= AI_IGNORE_SHOTS;
	self->monsterinfo.blindfire = true;
	// pmm
}
