#ifndef _AGENT_HPP_
#define _AGENT_HPP_

#include "location.hpp"
#include "mission.hpp"

class Agent : public sf::Drawable//, public sf::Transformable
{
	
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
	bool traveling = false;
	
	sf::Sprite sprite;
	
	enum
	{
		NORMAL = 0,
		HIDDEN,
	};
	uint state = NORMAL;
	float amt = 1.0f;
	
	Agent(const sf::Texture &texture, const sf::IntRect &rectangle, uint c, Location* loc)
	:
		sprite(texture, rectangle), classes{c}
	{
		auto pos = loc->sprite.getPosition();
		sprite.setPosition(pos);
	}
	
	void update()
	{
		if (traveling && state==NORMAL)
		{
			amt -= 0.1f;
			if (amt < 0.0f)
			{
				amt = 0.0f;
				state = HIDDEN;
			}
			sprite.setScale(amt,amt);
		}
		else if (!traveling && state==HIDDEN)
		{
			amt += 0.1f;
			if (amt > 1.0f)
			{
				amt = 1.0f;
				state = NORMAL;
			}
			sprite.setScale(amt,amt);
		}
	}
	
private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		//if (traveling) return;
		
		//states.transform *= getTransform();
		
		target.draw(sprite, states);
	}

};

#endif