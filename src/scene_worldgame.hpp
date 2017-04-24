#ifndef _WORLDGAME_HPP_
#define _WORLDGAME_HPP_


#include "scene.hpp"
#include "agent.hpp"
#include "travel.hpp"
#include "ui.hpp"
#include "transport_button.hpp"

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
	
	// stages of game activity
	enum
	{
		CHOOSE_LOCATION = 0,
		CHOOSE_TRANSPORT,
		TRAVELING,
	};
	int click_stage = 0;
	int mode_of_transport = 0;
	
	sf::Clock clock;
	sf::Time next;
	
	std::vector<std::unique_ptr<Location>> locations;
	std::vector<std::unique_ptr<Agent>> agents;
	std::vector<std::unique_ptr<Mission>> missions;
	std::vector<std::unique_ptr<effect_t>> active_effects;
	
	std::vector<std::unique_ptr<TransportButton>> travel_buttons;
	//std::vector<std::unique_ptr<TransportButton>> buttons;
	
	Location* to_loc = nullptr;
	
	//std::vector<std::unique_ptr<StatMeter>> statmeters;
	std::unique_ptr<UI> ui;
	
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
		
		// location buttons
		sf::Texture* buttons_tex = asset.load_texture("data/buttons.png");
		
		
		travel_buttons.emplace_back(std::make_unique<TransportButton>(*buttons_tex, sf::IntRect{  0,  0,32,32}, VEHICLE_CAR));
		travel_buttons.emplace_back(std::make_unique<TransportButton>(*buttons_tex, sf::IntRect{ 32,  0,32,32}, VEHICLE_BUS));
		travel_buttons.emplace_back(std::make_unique<TransportButton>(*buttons_tex, sf::IntRect{ 64,  0,32,32}, VEHICLE_BOAT));
		travel_buttons.emplace_back(std::make_unique<TransportButton>(*buttons_tex, sf::IntRect{ 96,  0,32,32}, VEHICLE_PLANE));
		
		// location
		
		sf::Texture* loc_tex1 = asset.load_texture("data/locations1.png");
		sf::Texture* loc_tex2 = asset.load_texture("data/locations2.png");
		
		Location* UK  = add_location(*loc_tex1, {373,131,38,39}, {29,35}); // UK
		Location* UAE = add_location(*loc_tex1, {538,240,24,17}, {16,6}); // UAE
		Location* HK  = add_location(*loc_tex1, {696,257,12,10}, {7,5}); // HK
		Location* SIN = add_location(*loc_tex2, {660,281,34,22}, {24,13}); // SINGAPORE
		Location* JAP = add_location(*loc_tex2, {733,165,59,69}, {37,48}); // JAPAN
		
		Location* EUR = add_location(*loc_tex2, {381,56,126,153}, {40,114}); // EURO
		Location* CHI = add_location(*loc_tex2, {606,168,127,93}, {88,24}); // CHINA
		Location* RUS = add_location(*loc_tex1, {484,29,383,155}, {35,91}); // USSR
		Location* USW = add_location(*loc_tex1, {101,172,36,62}, {11,24}); // USA west
		Location* USE = add_location(*loc_tex1, {206,177,38,74}, {20,24}); // USA east
		
		USW->connect_road(USE);
		SIN->connect_road(CHI);
		EUR->connect_road(RUS);
		RUS->connect_road(CHI);
		            
		USE->connect_sea(UK);
		USE->connect_sea(EUR);
		UK ->connect_sea(EUR);
		JAP->connect_sea(CHI);
		HK ->connect_sea(CHI);
		HK ->connect_sea(SIN);
		
		// start location
		travel->pos = UK->sprite.getPosition();

		// agent
		
		sf::Texture* agents_tex = asset.load_texture("data/agents.png");
		
		int ax = 64;
		agents.emplace_back(std::make_unique<Agent>( *agents_tex, sf::IntRect{0,0,64,64}, Agent::TRAVEL_AGENT, UK ));
		player = agents.back().get();
		player->loc = UK;
		
		agents.emplace_back(std::make_unique<Agent>( *agents_tex, sf::IntRect{ax*1,0,64,64}, Agent::TRAVEL_AGENT, USE ));
		agents.emplace_back(std::make_unique<Agent>( *agents_tex, sf::IntRect{ax*2,0,64,64}, Agent::TRAVEL_AGENT, EUR ));
		agents.emplace_back(std::make_unique<Agent>( *agents_tex, sf::IntRect{ax*3,0,64,64}, Agent::TRAVEL_AGENT, JAP ));
		agents.emplace_back(std::make_unique<Agent>( *agents_tex, sf::IntRect{ax*4,0,64,64}, Agent::TRAVEL_AGENT, RUS ));
		agents.emplace_back(std::make_unique<Agent>( *agents_tex, sf::IntRect{ax*5,0,64,64}, Agent::TRAVEL_AGENT, SIN ));
		agents.emplace_back(std::make_unique<Agent>( *agents_tex, sf::IntRect{ax*6,0,64,64}, Agent::TRAVEL_AGENT, CHI ));
		agents.emplace_back(std::make_unique<Agent>( *agents_tex, sf::IntRect{ax*7,0,64,64}, Agent::TRAVEL_AGENT, HK ));
		
		// fg / stats
		sf::Texture* stat_tex = asset.load_texture("data/statmeter.png");
		
		ui = std::make_unique<UI>(persist, *font, *stat_tex);
		//statmeter = std::make_unique<StatMeter>(*font, *stat_tex, "TestStat");
		//statmeter->setPosition(0,0)
		
	}
	
	void update()
	{
		sf::Vector2f target = (traveling) ? travel->pos : player->sprite.getPosition();
		
		view.setCenter(target);
		
		ui->setPosition(target);
		ui->update();
		
		// mouse position set
		sf::Vector2f pointer_pos = target + (sf::Vector2f{input.mouse}/ZOOM) - sf::Vector2f(ZOOM_WH, ZOOM_HH);
		mouse_pointer_sprite->setPosition(pointer_pos);
		
		if (!travel->traveling)
		{
			if (click(input.get_command.at(0).at("click")))
			{
				switch(click_stage)
				{
					case CHOOSE_LOCATION:
					{
						for (auto& a : locations)
						{
							if (a->sprite.getGlobalBounds().contains(pointer_pos))
							{
								for (auto& b : travel_buttons)
								{
									b->set_location(player->loc, a.get());
									to_loc = a.get();
								}
								click_stage = CHOOSE_TRANSPORT;
								break;
							}
						}
						break;
					}
					
					case CHOOSE_TRANSPORT:
					{
						for (auto& b : travel_buttons)
						{
							if ((b->is_enabled) && b->sprite.getGlobalBounds().contains(pointer_pos))
							{
								// activate this from pressing button
								travel->set_destination(player->loc, to_loc, b->val);
								mode_of_transport = b->val;
								break;
							}
						}
						
						for (auto& b2 : travel_buttons)
						{
							b2->clear_location();
						}
						click_stage = CHOOSE_LOCATION;
						break;
					}
					
					case TRAVELING:
					{
						switch(mode_of_transport)
						{
							// costs
						}
						break;
					}
				}
				// replace with options around character
				/*for (auto& a : agents)
				{
					if (a->sprite.getGlobalBounds().contains(pos))
					{
						return;
					}
				}*/
			}
		}
		
		travel->update();
		
		for (auto& a : agents)
		{
			a->update(traveling, player->loc);
		}
		
		for (auto& b : travel_buttons)
		{
			b->update();
		}
		
		update_effects();
		
		if (traveling && !travel->traveling)
		{
			traveling = false;
			//
			player->sprite.setPosition(travel->pos);
			//player->traveling = false;
			player->loc = travel->to;
			
		}
		else if (!traveling && travel->traveling)
		{
			traveling = true;
			//player->traveling = true;
		}
		
		//statmeter->update(1.0f);
	}
	
	void end_scene()
	{
		++persist.scene;
		persist.scene_end = true;
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
		
		for (auto& a : agents   ) { render_texture.draw(*a); }
		
		render_texture.draw(*travel);
		
		for (auto& b : travel_buttons) { render_texture.draw(*b); }
		
		// mouse
		for (auto& s : sprites) { render_texture.draw(*s); }
		
		render_texture.draw(*ui);
		
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