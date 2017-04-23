#ifndef _UI_HPP_
#define _UI_HPP_

#include "statmeter.hpp"

#include <array>
#include <memory>

class UI : public sf::Drawable, public sf::Transformable
{

public:
	
	const Persist& persist;
	const sf::Font& font;
	const sf::Texture &texture;
	
	std::array<std::unique_ptr<StatMeter>, stats_t::COUNT> meters;
	
	UI(const Persist& p, const sf::Font& f, const sf::Texture &t) : persist(p), font(f), texture(t)
	{
		//meters.emplace_back();
		meters[stats_t::CHARISMA] = std::make_unique<StatMeter>(f, t, "CHARISMA");
		meters[stats_t::WILL]     = std::make_unique<StatMeter>(f, t, "WILL");
		meters[stats_t::INTEL]    = std::make_unique<StatMeter>(f, t, "INTEL");
		meters[stats_t::ECONOMY]  = std::make_unique<StatMeter>(f, t, "TRADE");
		meters[stats_t::ENERGY]   = std::make_unique<StatMeter>(f, t, "ENERGY");
		meters[stats_t::MONEY]    = std::make_unique<StatMeter>(f, t, "MONEY");
		meters[stats_t::CREDIT]   = std::make_unique<StatMeter>(f, t, "CREDIT"); 
		
		int y = -ZOOM_HH;
		for (auto& m : meters)
		{
			m->setPosition(-ZOOM_WH,y);
			y+= 16;
		}
	}
	
	int counter = 0;
	void update()
	{
		counter++;
		float fluctuate = sin(counter*0.02f) * 0.025f;
		
		
		int strategy = 0;
		for (auto& s : persist.stats.stat)
		{
			meters[strategy]->update( int(s), float(s) + fluctuate );
			strategy++;
		}
	}

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		
		for (auto& s : meters) { target.draw(*s, states); }
		
		//target.draw(*statmeter, states);
		
	}
};

#endif