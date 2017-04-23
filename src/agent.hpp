#ifndef _AGENT_HPP_
#define _AGENT_HPP_

#include "location.hpp"
#include "mission.hpp"

class Agent : public sf::Sprite
{
	using Sprite::Sprite;
	
public:

	enum
	{
		TRAVEL_AGENT = 0,
		BANKER,
		CEO,
		DIPLOMAT,
		LAWYER,
		SECRET_AGENT,
		BANK_TRADER,
		ASSASSIN
	};
	std::vector<uint> classes;
	
	Location* loc = nullptr;
	Mission* mission = nullptr;
	stats_t stats;
	
	// inventory;
	bool bank_card = false;
	bool phone     = false;
	
	Agent(const sf::Texture &texture, const sf::IntRect &rectangle, uint c, Location* loc)
	:
		Sprite(texture, rectangle), classes{c}
	{
		auto pos = loc->getPosition();
		setPosition(pos);
	}

};

#endif