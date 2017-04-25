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
	
	std::array<std::unique_ptr<StatMeter>, STAT_COUNT> meters;
	sf::Sprite endcap;
		

		
	UI(const Persist& p, const sf::Font& f, const sf::Texture &t) : persist(p), font(f), texture(t), endcap(texture)
	{
		//meters.emplace_back();
		meters[STAT_CHARISMA] = std::make_unique<StatMeter>(f, t, "CHARISMA");
		meters[STAT_WILL]     = std::make_unique<StatMeter>(f, t, "WILL");
		meters[STAT_INTEL]    = std::make_unique<StatMeter>(f, t, "INTEL");
		meters[STAT_ECONOMY]  = std::make_unique<StatMeter>(f, t, "TRADE");
		meters[STAT_ENERGY]   = std::make_unique<StatMeter>(f, t, "ENERGY");
		meters[STAT_MONEY]    = std::make_unique<StatMeter>(f, t, "MONEY");
		meters[STAT_CREDIT]   = std::make_unique<StatMeter>(f, t, "CREDIT"); 
		
		int y = -ZOOM_HH;
		for (auto& m : meters)
		{
			m->setPosition(-ZOOM_WH,y);
			y+= 16;
		}
		endcap.setPosition(-ZOOM_WH, y);
		endcap.setTextureRect(  {64, 32, 160, 16} );
	}
	
	int counter = 0;
	void update()
	{
		counter++;
		float fluctuate = sin(counter*0.1f) * 0.01f;
		
		
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
		
		target.draw(endcap, states);
		//target.draw(*statmeter, states);
		
	}
};

#endif