#ifndef _STATMETER_HPP_
#define _STATMETER_HPP_

#include <iostream>

class StatMeter : public sf::Drawable, public sf::Transformable
{
public:
	
	sf::Text text;
	sf::Sprite stat;
	sf::Sprite meter;
	
	StatMeter(const sf::Font& font, const sf::Texture &texture, const std::string& n) : stat(texture), meter(texture)
	{
		text.setFont(font);
		text.setString(n);
		text.setCharacterSize(8);
		text.setColor( {0,51,8,255} );
		
		stat.setTextureRect(  {64, 0, 128, 32} );
		
		text.setOrigin( {-70, -22} );
		meter.setOrigin({-3,-1-23});
	}

	void update(float amt)
	{
		meter.setTextureRect( {3, int(amt*249), 58, 8} );
	}
	
	/*void setPosition(const sf::Vector2f& pos)
	{
		setPosition(pos);
		
		text.setPosition(pos);
		stat.setPosition(pos);
		meter.setPosition(pos);
	}*/
	
private:
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		
		target.draw(meter, states);
		target.draw(stat, states);
		target.draw(text, states);
	}
};

#endif