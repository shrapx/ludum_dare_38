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
	
	bool can_transport(Location* other, int val) const
	{
		switch (val)
		{
			case VEHICLE_CAR:
			case VEHICLE_BUS:  return can_road(other);
			case VEHICLE_BOAT: return can_sea(other);
			case VEHICLE_PLANE: return true;
		}
	}
	
	bool can_road(Location* other) const
	{
		for (auto& r : roads)
		{
			if (r == other) return true;
		}
		return false;
	}
	
	bool can_sea(Location* other) const
	{
		for (auto& s : seas)
		{
			if (s == other) return true;
		}
		return false;
	}
	
	int get_mode_of_transport(Location* other) const
	{
		if (can_road(other)) return VEHICLE_CAR;
		if (can_sea(other)) return VEHICLE_BOAT;
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
