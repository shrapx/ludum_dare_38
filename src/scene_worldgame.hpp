#ifndef _WORLDGAME_HPP_
#define _WORLDGAME_HPP_


#include "scene.hpp"
#include "agent.hpp"
#include "card.hpp"

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
	
	uint clicked = 0;
	
	
	std::vector<std::unique_ptr<Location>> locations;
	std::vector<std::unique_ptr<Agent>> agents;
	std::vector<std::unique_ptr<Mission>> missions;
	std::vector<std::unique_ptr<effect_t>> active_effects;
	
	
	Agent* player = nullptr;
	Card* show_this_card = nullptr;
	
	Location* add_location(Card* card, const sf::Texture& tex, const sf::IntRect& rect, const sf::Vector2f& pos)
	{
		locations.emplace_back(std::make_unique<Location>(card, tex, rect, pos));
		Location* a = locations.back().get();
		return a;
	}
	
	Agent* add_agent(Card* card, const sf::Texture& tex, const sf::IntRect& rect, uint c, Location* loc)
	{
		agents.emplace_back(std::make_unique<Agent>(card, tex, rect, c, loc));
		Agent* a = agents.back().get();
		return a;
	}
	
	Card* add_card(const sf::Font& f, const sf::Texture& tex, const sf::IntRect& rect, const sf::Vector2f& pos)
	{
		cards.emplace_back(std::make_unique<Card>(f, tex, rect, pos));
		Card* a = cards.back().get();
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
		
		// card
		sf::Texture* cards_tex = asset.load_texture("data/cards.png");
		
		Card* card = add_card(*font, *cards_tex, {0,0,64,64}, {64,64});
		
		card->set_text("New Opportunity", "description");
		
		//card->add_choice( "one", [](){} );
		
		
		//card->set_choices( {"one", "two", "three"} );
		
		// location
		
		sf::Texture* loc_tex = asset.load_texture("data/locations.png");
		
		Location* loc_uk = add_location(card, *loc_tex, {340,125,36,34}, {188,68});
		
		// agent
		
		sf::Texture* agents_tex = asset.load_texture("data/agents.png");

		player = add_agent(card, *agents_tex, {0,0,64,64}, Agent::TRAVEL_AGENT, loc_uk);
		
	}
	
	void update()
	{
		sf::Vector2f target = player->getPosition();
		sf::Vector2f center = constrain({0.f, 0.f, 128.f, 128.f}, target);
		
		view.setCenter(center);
		
		// mouse position set
		sf::Vector2f pointer_pos = center + (sf::Vector2f{input.mouse}/ZOOM) - sf::Vector2f(ZOOM_WH, ZOOM_HH);
		mouse_pointer_sprite->setPosition(pointer_pos);
		
		if (click(input.get_command.at(0).at("click")))
		{
			if (show_this_card)
			{
				int choice = show_this_card->click_card(pointer_pos);
				
				std::cout << choice << std::endl;
				
				if (choice > 0)
				{
					// process choice somehow
				}
				else
				{
					show_this_card = nullptr;
					return;
				}
			}
			else
			{
				// do we hit an active agent?
				click_agents_and_locations(pointer_pos);
			}
			// do we hit a location?
			
			// what else
			/*if (clicked < 3)
			{
				++clicked;
				sound_click->play();
			}
			else
			{
				++persist.scene;
				persist.scene_end = true;
			}*/
		}
	}
	
	void click_cards(const sf::Vector2f& pos)
	{
		show_this_card->click_card(pos);
		for (auto& a : agents)
		{
			if (a->getGlobalBounds().contains(pos))
			{
				//a->setColor({245,32,32});
				
				show_this_card = a->card.get();
				return;
			}
		}
		
		for (auto& a : locations)
		{
			if (a->getGlobalBounds().contains(pos))
			{
				//a->setColor({245,32,32});
				show_this_card = a->card.get();
				return;
			}
		}
		
		// click must have been off the card
	}
	
	void click_agents_and_locations(const sf::Vector2f& pos)
	{
		for (auto& a : agents)
		{
			if (a->getGlobalBounds().contains(pos))
			{
				///a->setColor({245,32,32});
				
				show_this_card = a->card.get();
				return;
			}
		}
		
		for (auto& a : locations)
		{
			if (a->getGlobalBounds().contains(pos))
			{
				//a->setColor({245,32,32});
				show_this_card = a->card.get();
				return;
			}
		}
	}
	
	void update_interaction(stats_t& other)
	{
		stats_t& me = persist.stats;
		
	}
	
	void render()
	{
		for (auto& b : bgs      ) { render_texture.draw(*b); }
		for (auto& l : locations) { render_texture.draw(*l); }
		for (auto& a : agents   ) { render_texture.draw(*a); }
		
		if (show_this_card) { render_texture.draw(*show_this_card); }
		
		for (auto& s : sprites  ) { render_texture.draw(*s); }
		
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