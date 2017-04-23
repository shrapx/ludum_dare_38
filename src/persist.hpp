#ifndef _PERSIST_HPP_
#define _PERSIST_HPP_

struct stats_t
{
	enum
	{
		CHARISMA=0, // stats
		ECONOMY,
		WILL,
		INTEL,
		ENERGY, // pools
		MONEY,
		CREDIT,
		COUNT
	};
	int strategy = CHARISMA;
	std::array<int,COUNT> stat;
	
	static bool compare(const stats_t& a, const stats_t& b)
	{
		// compare a's strategy stats
		return a.stat[a.strategy] > b.stat[a.strategy];
	};
};

struct effect_t
{
	stats_t& a;
	stats_t& b;
	
	effect_t(stats_t& a, stats_t& b) : a(a),b(b) {}
	
	int strategy_a = 0;
	int strategy_b = 0;
	int factor_a = -1;
	int factor_b = +1;
	int countdown = 100;
	bool early_exit = false;
	bool remove_me = false;
	//sf::Time duration;
	
	void update()
	{
		a.stat[strategy_a] += factor_a;
		b.stat[strategy_b] += factor_b;
		--countdown;
		remove_me = countdown < 0;
	}
	// intelligence += everything
	// ..CHARISMA > WILLPOWER > MONEY > CHARISMA..
	// energy++ = money--
	// money >= -credit;
	// if (a.strategy == CHARISMA && b.strategy == WILLPOWER)
};

struct Persist
{
	stats_t stats;
	
	uint scene = 0;
	bool scene_end = false;
	bool game_end = false;
	//std::set<std::string> event_flags;
};

#endif 
