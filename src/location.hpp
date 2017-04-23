#ifndef _LOCATION_HPP_
#define _LOCATION_HPP_

#include <vector>

class Location : public sf::Drawable//, public sf::Transformable
{
public:

	sf::Sprite sprite;
	
	std::vector<Location*> roads;
	std::vector<Location*> seas;
	
	//Mission* card = nullptr;
	
	bool visited = false;
	
	Location(const sf::Texture &texture, const sf::IntRect &rectangle, const sf::Vector2f& origin)
	:
		sprite(texture, rectangle)
	{
		sprite.setOrigin(origin);
		sprite.setPosition(rectangle.left + origin.x, rectangle.top + origin.y);
		sprite.setColor({255,255,128,255});
	}
	
	int get_mode_of_transport(Location* other) const
	{
		for (auto& r : roads)
		{
			if (r == other) return VEHICLE_CAR;
		}
		
		for (auto& s : seas)
		{
			if (s == other) return VEHICLE_BOAT;
		}
		
		return VEHICLE_PLANE;
	}
	
	void connect_road(Location* other)
	{
		roads.emplace_back(other);
		other->roads.emplace_back(this);
	}
	
	void connect_sea(Location* other)
	{
		seas.emplace_back(other);
		other->seas.emplace_back(this);
	}
	
private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{

		//states.transform *= getTransform();
		
		target.draw(sprite, states);
	}

};

#endif
