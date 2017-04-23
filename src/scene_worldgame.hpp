#ifndef _WORLDGAME_HPP_
#define _WORLDGAME_HPP_


#include "scene.hpp"
#include "agent.hpp"
#include "travel.hpp"
#include "statmeter.hpp"

#include <iostream>

class WorldGame : public Scene
{
	using Scene::Scene;
	
	std::vector<std::unique_ptr<sf::Sprite>> bgs;
	std::vector<std::unique_ptr<sf::Sprite>> sprites;
	std::vector<std::unique_ptr<sf::Sound>> sounds;
	
	std::vector<std::unique_ptr<sf::Text>> texts;
	
	sf::Font* font = nullptr;
	sf::Sprite* mouse_pointer_sprite = nullptr;
	sf::Sound* sound_click = nullptr;
	bool mouse_click = false;
	bool click(bool click_in)
	{
		if (!mouse_click)
		{
			if (click_in)
			{
				mouse_click = true;
				sound_click->play();
				return true;
			}
		}
		else if(!click_in)
		{
			mouse_click = false;
		}
		return false;
	}
	
	sf::Clock clock;
	sf::Time next;
	
	std::vector<std::unique_ptr<Location>> locations;
	std::vector<std::unique_ptr<Agent>> agents;
	std::vector<std::unique_ptr<Mission>> missions;
	std::vector<std::unique_ptr<effect_t>> active_effects;
	
	//std::vector<std::unique_ptr<StatMeter>> statmeters;
	std::unique_ptr<StatMeter> statmeter;
	std::unique_ptr<Travel> travel;
	Agent* player = nullptr;
	
	bool traveling = false;
	
	Location* add_location(const sf::Texture& tex, const sf::IntRect& rect, const sf::Vector2f& pos)
	{
		locations.emplace_back(std::make_unique<Location>(tex, rect, pos));
		Location* a = locations.back().get();
		return a;
	}
	
	Agent* add_agent(const sf::Texture& tex, const sf::IntRect& rect, uint c, Location* loc)
	{
		agents.emplace_back(std::make_unique<Agent>(tex, rect, c, loc));
		Agent* a = agents.back().get();
		return a;
	}
	
	void load()
	{
		// input
		input.initialise_command(0, "W", "up", 0);
		input.initialise_command(0, "A", "left", 0);
		input.initialise_command(0, "S", "down", 0);
		input.initialise_command(0, "D", "right", 0);
		
		input.initialise_command(0, "E", "frob", 0);
		input.initialise_command(0, "F", "flashlight", 0);
		input.initialise_command(0, "MouseLeft", "click", 0);
		
		font = asset.load_font("data/LiberationSans-Regular.ttf");
		
		auto sb = asset.load_soundbuffer("data/blip.ogg");
		sounds.emplace_back( std::make_unique<sf::Sound>(*sb) );
		sound_click = sounds.back().get();
		
		sf::Music* music = asset.load_music("data/loop.ogg");
		music->setLoop(true);
		music->play();
		
		sf::Texture* mouse_pointer_tex = asset.load_texture("data/mouse_pointer.png");
		sprites.emplace_back( std::make_unique<sf::Sprite>(*mouse_pointer_tex) );
		mouse_pointer_sprite = sprites.back().get();
		
		/*
		sf::Shader* sha = asset.load("data/darkness_v.glsl", "data/darkness_f.glsl");
		sf::Texture* p1 = *asset.textures.at("player");
		*/
		
		// bg
		
		sf::Texture* bg_tex = asset.load_texture("data/map.png");
		bgs.emplace_back(std::make_unique<sf::Sprite>(*bg_tex));
		
		// travel vehicle display
		sf::Texture* travel_tex = asset.load_texture("data/travel.png");
		travel = std::make_unique<Travel>(*travel_tex);
		
		// location
		
		sf::Texture* loc_tex = asset.load_texture("data/locations.png");
		
		Location* loc_uk = add_location(*loc_tex, {373,131,38,39}, {29,35}); // UK
		
		//travel->pos = {373,177};
		travel->pos = loc_uk->sprite.getPosition();
		add_location(*loc_tex, {206,177,38,74}, {20,24}); // USA west
		add_location(*loc_tex, {101,172,36,62}, {11,24}); // USA east
		
		// agent
		
		sf::Texture* agents_tex = asset.load_texture("data/agents.png");

		player = add_agent(*agents_tex, {0,0,64,64}, Agent::TRAVEL_AGENT, loc_uk);
		
		// fg / stats
		
		sf::Texture* stat_tex = asset.load_texture("data/statmeter.png");
		
		statmeter = std::make_unique<StatMeter>(*font, *stat_tex, "TestStat");
		
	}
	
	void update()
	{
		sf::Vector2f target = (traveling) ? travel->pos : player->sprite.getPosition();
		
		view.setCenter(target);
		
		// mouse position set
		sf::Vector2f pointer_pos = target + (sf::Vector2f{input.mouse}/ZOOM) - sf::Vector2f(ZOOM_WH, ZOOM_HH);
		mouse_pointer_sprite->setPosition(pointer_pos);
		
		if (click(input.get_command.at(0).at("click")))
		{
			look_for_click(pointer_pos);
		}
		
		travel->update();
		player->update();
		
		update_effects();
		
		if (traveling && !travel->traveling)
		{
			traveling = false;
			//
			player->sprite.setPosition(travel->pos);
			player->traveling = false;
		}
		else if (!traveling && travel->traveling)
		{
			traveling = true;
			player->traveling = true;
		}
		statmeter->update(1.0f);
	}
	
	void end_scene()
	{
		++persist.scene;
		persist.scene_end = true;
	}
	
	void look_for_click(const sf::Vector2f& pos)
	{
		for (auto& a : agents)
		{
			if (a->sprite.getGlobalBounds().contains(pos))
			{
				return;
			}
		}
		
		for (auto& a : locations)
		{
			if (a->sprite.getGlobalBounds().contains(pos))
			{
				if (!travel->traveling)
				{
					travel->set_destination(Travel::VEHICLE_CAR, 100, pos);
				}
				return;
			}
		}
	}
	
	void update_effects()
	{
		for (auto& e : active_effects)
		{
			if (e) e->update();
		}
		
		active_effects.erase(std::remove_if(
			active_effects.begin(),
			active_effects.end(),
			[](std::unique_ptr<effect_t>& e) {return !e || e->remove_me;}),
			active_effects.end());
	}
	
	
	void render()
	{
		for (auto& b : bgs      ) { render_texture.draw(*b); }
		for (auto& l : locations) { render_texture.draw(*l); }
		
		render_texture.draw(*travel);
		
		for (auto& a : agents   ) { render_texture.draw(*a); }
		
		for (auto& s : sprites) { render_texture.draw(*s); }
		
		sf::RenderStates s;
		s.transform *= view.getTransform();
		render_texture.draw(*statmeter, s);
		
		// draw with shader
		/*if (night)
		{
			sf::RenderStates states;
			states.blendMode = sf::BlendMultiply; //sf::BlendAlpha;
			states.shader = shadow_shader;
			render_texture.draw(shadows_drawable, states);
		}*/
		
		// foregrounds
		
		//render_texture.draw(*pointer);
	}
};

#endif