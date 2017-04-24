#ifndef _MISSION_HPP_
#define _MISSION_HPP_

#include <vector>
#include <string>
#include <tuple>

/*
int factor_a = -1;
int factor_b = +1;
int countdown = 100;
bool early_exit = false;
sf::Time duration;
*/
struct Mission
{
	using task_t = std::pair<std::string, int>;
	using stage_t = std::vector<task_t>;
	std::vector<stage_t> stages;
	bool enabled = false;
	int stage = 0;
	int report = 0;
	enum
	{
		FAILURE = -1,
		ONGOING = 0,
		SUCCESS = 1
	};
};


#endif
