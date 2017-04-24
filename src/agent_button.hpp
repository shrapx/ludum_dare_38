#ifndef _AGENT_BUTTON_HPP_
#define _AGENT_BUTTON_HPP_

#include "agent.hpp"

class AgentButton : public sf::Drawable//, public sf::Transformable
{
	
public:

	Agent* agent = nullptr;

	sf::Sprite sprite;
	bool is_enabled = true;
	bool is_hiding = true;
	float amt = 0.0f;
	int val = 0;
	AgentButton(const sf::Texture &texture, const sf::IntRect& rectangle, int v)
	:
		sprite(texture, rectangle)
	{
		val = v;
		float radians =  (90+(-22.5f * v)) / (180 / M_PI);
		float d = 96.f;
		sprite.setOrigin({16 + (sin(radians) * d), 16 + (cos(radians) * d)});
	}
	
	void set_agent(Agent* a)
	{
		agent = a;
	}
	
	void clear_agent()
	{
		agent = nullptr;
	}
	
	void update()
	{
		if (!agent && !is_hiding)
		{
			amt -= 0.1f;
			if (amt < 0.0f)
			{
				amt = 0.0f;
				is_hiding = true;
			}
			sprite.setScale(amt,amt);
		}
		else if (agent && is_hiding)
		{
			amt += 0.1f;
			if (amt > 1.0f)
			{
				amt = 1.0f;
				is_hiding = false;
			}
			sprite.setScale(amt,amt);
		}
		
		if (!agent) return;
		sf::Color c = sf::Color::White;
		if (!is_enabled)
		{
			c.a = 128;
		}
		sprite.setColor(c);
		sprite.setPosition(agent->sprite.getPosition());
	}
	
private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		//states.transform *= getTransform();
		
		target.draw(sprite, states);
	}

};

#endif