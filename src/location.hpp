#ifndef _LOCATION_HPP_
#define _LOCATION_HPP_

#include "mission.hpp"

class Location : public sf::Sprite
{
	using Sprite::Sprite;

public:

	Mission* card = nullptr;
	
	Location(const sf::Texture &texture, const sf::IntRect &rectangle, const sf::Vector2f& pos)
	:
		Sprite(texture, rectangle)
	{
		setOrigin(rectangle.width/2 , rectangle.height/2);
		setPosition(pos);
	}
};

#endif
