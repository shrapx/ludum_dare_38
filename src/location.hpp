#ifndef _LOCATION_HPP_
#define _LOCATION_HPP_

#include "mission.hpp"

class Location : public sf::Drawable//, public sf::Transformable
{
public:

	sf::Sprite sprite;
	
	Mission* card = nullptr;
	
	bool enabled = true;
	bool visited = false;
	
	Location(const sf::Texture &texture, const sf::IntRect &rectangle, const sf::Vector2f& origin)
	:
		sprite(texture, rectangle)
	{
		sprite.setOrigin(origin);
		
		//sf::Vector2f half = { -rectangle.width/2 , -rectangle.height/2 };
		sprite.setPosition(rectangle.left + origin.x, rectangle.top + origin.y);
		sprite.setColor( {255,255,128,255});
	}
	
private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (!enabled) return;
		
		//states.transform *= getTransform();
		
		target.draw(sprite, states);
	}

};

#endif
