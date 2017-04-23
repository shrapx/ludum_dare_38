#ifndef _PERSIST_HPP_
#define _PERSIST_HPP_

#include <iostream>

class stat_t
{
public:
	int amt = 0;
	int min = 0;
	int max = 100;
	
	std::string name;
	
	stat_t() {}
	stat_t(int mi, int ma, int a) : min(mi), max(ma), amt(a) {}
	
	operator int() const
	{
		return amt;
	}
	
	operator float() const
	{
		return (1.0f/(max+min)) * (amt+min);
	}
	
	stat_t& operator+=(int i)
	{
		 amt += i;
		 if (amt > max) amt = max;
		 return *this;
	}
	
	stat_t& operator-=(int i)
	{
		 amt -= i;
		 if (amt > min) amt = min;
		 return *this;
	}
};

struct stats_t
{
	enum
	{
		CHARISMA=0, // stats
		WILL,
		INTEL,
		ECONOMY,
		ENERGY, // pools
		MONEY,
		CREDIT,
		COUNT
	};
	
	int strategy = CHARISMA;
	std::array<stat_t,COUNT> stat;
	
	static bool compare(const stats_t& a, const stats_t& b)
	{
		// compare a's strategy stats
		int ia = a.stat[a.strategy];
		int ib = b.stat[a.strategy];
		
		return ia > ib;
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
	
	Persist()
	{
		int r1 = rand() % 10;
		int r2 = rand() % 10;
		int r3 = rand() % 500;
		std::cout << r1 << std::endl;
		std::cout << r2 << std::endl;
		std::cout << r3 << std::endl;
		stats.stat[stats_t::CHARISMA] = {0, 100, 10+r1};
		stats.stat[stats_t::WILL]     = {0, 100, 10+r2};
		stats.stat[stats_t::INTEL]    = {0, 100, 20-r2};
		stats.stat[stats_t::ECONOMY]  = {0, 100, 20-r1};
		stats.stat[stats_t::ENERGY]   = {0, 100, 100  };
		stats.stat[stats_t::MONEY]    = {0, 10000,  500+r3};
		stats.stat[stats_t::CREDIT]   = {0, 10000, 1000-r3};
	}
};

#endif 
