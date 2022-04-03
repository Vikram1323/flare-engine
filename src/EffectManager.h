/*
Copyright © 2012-2016 Justin Jacobs

This file is part of FLARE.

FLARE is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

FLARE is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
FLARE.  If not, see http://www.gnu.org/licenses/
*/

/**
 * class EffectManager
 *
 * Holds the collection of hazards (active attacks, spells, etc) and handles group operations
 */

#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include "CommonIncludes.h"
#include "Utils.h"

class Animation;
class Hazard;
class StatBlock;

class Effect {
public:
	enum {
		NONE = 0,
		DAMAGE,
		DAMAGE_PERCENT,
		HPOT,
		HPOT_PERCENT,
		MPOT,
		MPOT_PERCENT,
		SPEED,
		ATTACK_SPEED,
		RESIST_ALL,
		STUN,
		REVIVE,
		CONVERT,
		FEAR,
		DEATH_SENTENCE,
		SHIELD,
		HEAL,
		KNOCKBACK,
		TYPE_COUNT
	};

	Effect();
	Effect(const Effect& other);
	Effect& operator=(const Effect& other);
	~Effect();

	void loadAnimation(const std::string &s);
	void unloadAnimation();

	static int getTypeFromString(const std::string& _type);
	static bool typeIsStat(int t);
	static bool typeIsDmgMin(int t);
	static bool typeIsDmgMax(int t);
	static bool typeIsResist(int t);
	static bool typeIsPrimary(int t);
	static bool typeIsEffectResist(int t);
	static int getStatFromType(int t);
	static size_t getDmgFromType(int t);
	static size_t getResistFromType(int t);
	static size_t getPrimaryFromType(int t);

	static bool isImmunityTypeString(const std::string& type_str); // handling of deprecated types

	std::string id;
	std::string name;
	int icon;
	Timer timer;
	int type;
	int magnitude;
	int magnitude_max;
	std::string animation_name;
	Animation* animation;
	bool item;
	int trigger;
	bool render_above;
	size_t passive_id;
	int source_type;
	bool group_stack;
	uint32_t color_mod;
	uint8_t alpha_mod;
	std::string attack_speed_anim;
};

class EffectDef {
public:
	EffectDef();

	std::string id;
	int type;
	std::string name;
	int icon;
	std::string animation;
	bool can_stack;
	int max_stacks;
	bool group_stack;
	bool render_above;
	Color color_mod;
	uint8_t alpha_mod;
	std::string attack_speed_anim;

	bool is_immunity_type; // handling of deprecated types
};

class EffectManager {
private:
	void removeEffect(size_t id);
	void clearStatus();
	void addEffectInternal(StatBlock* stats, EffectDef &effect, int duration, int magnitude, int source_type, bool item, PowerID power_id);

public:
	EffectManager();
	~EffectManager();
	void logic();
	void addEffect(StatBlock* stats, EffectDef &effect, int duration, int magnitude, int source_type, PowerID power_id);
	void addItemEffect(StatBlock* stats, EffectDef &effect, int duration, int magnitude);
	void removeEffectType(const int type);
	void removeEffectPassive(size_t id);
	void removeEffectID(const std::vector< std::pair<std::string, int> >& remove_effects);
	void clearEffects();
	void clearNegativeEffects(int type);
	void clearItemEffects();
	void clearTriggerEffects(int trigger);
	int damageShields(int dmg);
	bool isDebuffed();
	void getCurrentColor(Color& color_mod);
	void getCurrentAlpha(uint8_t& alpha_mod);
	bool hasEffect(const std::string& id, int req_count);
	float getAttackSpeed(const std::string& anim_name);
	int getDamageSourceType(int dmg_mode);

	std::vector<Effect> effect_list;

	int damage;
	int damage_percent;
	int hpot;
	int hpot_percent;
	int mpot;
	int mpot_percent;
	float speed;
	bool stun;
	bool revive;
	bool convert;
	bool death_sentence;
	bool fear;
	float knockback_speed;

	std::vector<int> bonus;
	std::vector<int> bonus_resist;
	std::vector<int> bonus_primary;

	bool triggered_others;
	bool triggered_block;
	bool triggered_hit;
	bool triggered_halfdeath;
	bool triggered_joincombat;
	bool triggered_death;

	bool refresh_stats;

	static const int NO_POWER = 0;
};

#endif
