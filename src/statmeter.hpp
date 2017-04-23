#ifndef _STATMETER_HPP_
#define _STATMETER_HPP_

#include <iostream>

class StatMeter : public sf::Drawable, public sf::Transformable
{
public:
	
	sf::Text text;
	sf::Text num;
	sf::Sprite cover;
	sf::Sprite meter;
	
	StatMeter(const sf::Font& font, const sf::Texture &texture, const std::string& n) : cover(texture), meter(texture)
	{
		

		text.setString(n);
		text.setFont(font);
		text.setCharacterSize(9);
		text.setColor( {0,51,8,255} );
		
		num.setFont(font);
		num.setCharacterSize(9);
		num.setColor( {0,51,8,255} );
		
		cover.setTextureRect(  {64, 16, 160, 16} );
		
		text.setOrigin( {-100, -6} );
		num.setOrigin( {-75, -6} );
		meter.setOrigin({-3,-1-6});
	}

	void update(int iamt, float amt)
	{
		num.setString( std::to_string(iamt) );
		meter.setTextureRect( {3, int(amt*(512-8)), 58, 8} );
	}
	
private:
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		
		target.draw(meter, states);
		target.draw(cover, states);
		target.draw(num, states);
		target.draw(text, states);
	}
};

#endif