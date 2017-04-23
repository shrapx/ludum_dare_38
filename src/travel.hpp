#ifndef _TRAVEL_HPP_
#define _TRAVEL_HPP_

#include <iostream>

class Travel : public sf::Drawable, public sf::Transformable
{
public:

	enum 
	{
		VEHICLE_NONE = 0,
		VEHICLE_PLANE,
		VEHICLE_CAR,
		VEHICLE_BUS,
		VEHICLE_BOAT,
		VEHICLE_COUNT
	};
	int vehicle = VEHICLE_CAR;
	
	sf::Vector2f pos;
	sf::Vector2f pos_from;
	sf::Vector2f pos_to;
	sf::Sprite vehicle_sprite;
	
	std::vector<sf::RectangleShape> line;
	sf::Vector2f line_size = {4.0,2};
	int draw_line_to = 0;
	
	// time period
	int time_total = 0;
	int time_fade = 50;
	int time_left = -time_fade;
	bool traveling = false;
	
	Travel(const sf::Texture &texture) : vehicle_sprite(texture)
	{
		
	}
	
	void set_destination(int v, int t, const sf::Vector2f& p)
	{
		time_total = t;
		time_left = t;
		
		set_vehicle(v);
		
		pos_to = p;
		
		set_line();
		
		reset_colour();
		
		traveling = true;
	}
	
	void update()
	{
		if (!traveling) return;
		
		if (time_left > 0)
		{
			float amt = float(1.0f/time_total) * (time_total-time_left);
			
			draw_line_to = amt*line.size();
			
			pos = lerp(pos_from, pos_to, amt);
			vehicle_sprite.setPosition(pos);
		}
		else if (time_left > -time_fade)
		{
			sf::Uint8 a = (1.0f/time_fade) * (time_left+time_fade) * 255;
			
			vehicle_sprite.setColor( {255,255,255,a} );
			
			for (auto& l : line)
			{
				l.setFillColor({255,0,0,a});
			}
			pos = pos_to;
		}
		else
		{
			pos_from = pos_to;
			pos = pos_from;
			
			traveling = false;
		}
		--time_left;
	}
	
private:
	
	void reset_colour()
	{
		vehicle_sprite.setColor( sf::Color::White );
		
		for (auto& l : line)
		{
			l.setFillColor(sf::Color::Red);
		}
	}
	
	void set_line()
	{
		sf::Vector2f diff =  {pos_to.x - pos_from.x, pos_to.y - pos_from.y};
		float d = length(diff);//(pos_from, pos_to);
		int num = d / (2*line_size.x);
		
		float rot =  atan2(diff.y, diff.x) * (180.f/M_PI);
		
		vehicle_sprite.setRotation(rot);
		
		line.clear();
		for (int i = 0; i < num; ++i)
		{
			line.emplace_back(line_size);
			sf::RectangleShape& r = line.back();
			r.setPosition( lerp(pos_from, pos_to, 1.0f/num * i) );
			r.setRotation( rot );
		}
	}
	
	void set_vehicle(int v)
	{
		vehicle = v;
		switch(v)
		{
			case VEHICLE_PLANE: vehicle_sprite.setTextureRect({ 0,  0, 64, 64}); break;
			case VEHICLE_CAR:   vehicle_sprite.setTextureRect({64,  0, 64, 64}); break;
			case VEHICLE_BOAT:  vehicle_sprite.setTextureRect({ 0,  0, 64, 64}); break;
			case VEHICLE_BUS:  vehicle_sprite.setTextureRect( {64, 64, 64, 64}); break;
		}
		vehicle_sprite.setOrigin(32,32);
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (!traveling) return;
		
		states.transform *= getTransform();
		
		for (int i=0; i<draw_line_to; ++i )
		{
			target.draw(line[i], states);
		}
		
		target.draw(vehicle_sprite, states);
	}
};

#endif