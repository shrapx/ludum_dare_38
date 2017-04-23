#ifndef _TRAVEL_HPP_
#define _TRAVEL_HPP_

#include <iostream>

class Travel : public sf::Drawable, public sf::Transformable
{
public:

	int vehicle = VEHICLE_CAR;
	
	Location* from;
	Location* to;
	sf::Vector2f pos;
	sf::Vector2f pos_from;
	sf::Vector2f pos_to;
	sf::Sprite vehicle_sprite;
	
	std::vector<sf::RectangleShape> line;
	sf::Vector2f line_size = {4.0,2};
	int draw_line_to = 0;
	
	// time period
	int time_total = 0;
	int time_fade = FADE;
	int time_left = -time_fade;
	bool traveling = false;
	
	Travel(const sf::Texture &texture) : vehicle_sprite(texture)
	{
		
	}
	
	void set_destination(Location* f, Location* t)
	{
		from = f;
		to = t;
		int v = f->get_mode_of_transport(t);
		
		pos_from = pos;
		pos_to = t->sprite.getPosition();
		
		int tmod = 1;
		
		switch(v)
		{
			case VEHICLE_PLANE: tmod = 1; break;
			case VEHICLE_BOAT:  tmod = 2; break;
			case VEHICLE_CAR:   tmod = 3; break;
			case VEHICLE_BUS:   tmod = 4; break;
		}
		
		int d = dist(pos_from, pos_to) * tmod;
		
		time_total = d;
		time_left = d + time_fade;
		
		set_vehicle(v);
		
		vehicle_sprite.setOrigin({60,32});
		vehicle_sprite.setPosition(pos_from);
		
		set_line();
		
		reset_colour();
		
		traveling = true;
	}
	
	void update()
	{
		if (!traveling) return;
		
		if ( time_left > time_total) // fade in
		{
			sf::Uint8 a = 255 - float((1.0f/time_fade) * (time_left-time_total)) * 255;
			
			vehicle_sprite.setColor( {255,255,255,a} );
		}
		else if (time_left > 0) // lerp
		{
			float amt = float(1.0f/time_total) * (time_total-time_left);
			
			draw_line_to = amt*line.size();
			
			pos = lerp(pos_from, pos_to, amt);
			vehicle_sprite.setPosition(pos);
		}
		else if (time_left > -time_fade) // fade out
		{
			sf::Uint8 a = (1.0f/time_fade) * (time_left+time_fade) * 255;
			
			vehicle_sprite.setColor( {255,255,255,a} );
			
			for (auto& l : line)
			{
				l.setFillColor({255,0,0,a});
			}
			pos = pos_to;
		}
		else // end
		{
			draw_line_to = 0;
			pos = pos_to;
			pos = pos;
			
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
		sf::Vector2f diff =  {pos_to.x - pos.x, pos_to.y - pos.y};
		float d = length(diff);//(pos, pos_to);
		int num = d / (2*line_size.x);
		
		float rot =  atan2(diff.y, diff.x) * (180.f/M_PI);
		
		vehicle_sprite.setRotation(rot);
		
		line.clear();
		for (int i = 0; i < num; ++i)
		{
			line.emplace_back(line_size);
			sf::RectangleShape& r = line.back();
			r.setPosition( lerp(pos, pos_to, 1.0f/num * i) );
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