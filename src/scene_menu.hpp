#ifndef _SCENE_MENU_HPP_
#define _SCENE_MENU_HPP_

#include "scene.hpp"

#include <vector>
#include <array>

class SelectBox : public sf::Sprite
{
public:
	using Sprite::Sprite;
	
	SelectBox* u = nullptr;
	SelectBox* d = nullptr;
	SelectBox* l = nullptr;
	SelectBox* r = nullptr;
	uint trigger_enum = 0;
};

class SceneMenu : public Scene
{
	using Scene::Scene;
	
	std::vector<std::unique_ptr<sf::Sprite>> sprites;
	std::vector<std::unique_ptr<sf::Sound>> sounds;
	
	sf::Sprite* mouse_pointer_sprite = nullptr;
	sf::Sound* sound_click = nullptr;
	bool click = false;
	uint clicked = 0;
	
	uint scene_state = 0;
	enum
	{
		SCENE_BLANK = 0,
		SCENE_LOGO,
		SCENE_TITLE,
		SCENE_MENU,
		SCENE_LEVELS,
		SCENE_CREDITS,
		SCENE_EXIT,
		SCENE_COUNT
	};
	
	sf::Clock clock;
	std::array<sf::Time, SCENE_COUNT> scene_times;
	
	std::array<std::vector<std::unique_ptr<SelectBox>>, SCENE_COUNT> scene_boxes;
	
	// box graph + clickable:
	// mousemove / updownleftright selects
	// spacebar / return advances
	
	SelectBox* add_box(uint scene_enum, uint target_enum, const sf::Texture& tex, const sf::Vector2f& pos)
	{
		scene_boxes[scene_enum].emplace_back(std::make_unique<SelectBox>(tex));
		SelectBox* box = scene_boxes[scene_enum].back().get();
		box->setPosition(pos);
		box->trigger_enum = target_enum;
		return box;
	}
	
	void load()
	{
		scene_times[SCENE_BLANK   ] = sf::seconds(1.0f);
		scene_times[SCENE_LOGO    ] = sf::seconds(5.0f);
		scene_times[SCENE_TITLE   ] = sf::seconds(30.0f);
		scene_times[SCENE_MENU    ] = sf::seconds(30.0f);
		scene_times[SCENE_LEVELS  ] = sf::seconds(30.0f);
		scene_times[SCENE_CREDITS ] = sf::seconds(60.0f);
		scene_times[SCENE_EXIT    ] = sf::seconds(10.0f);
		
		// input
		
		input.initialise_command(0, "W", "up", 0);
		input.initialise_command(0, "A", "left", 0);
		input.initialise_command(0, "S", "down", 0);
		input.initialise_command(0, "D", "right", 0);
		
		input.initialise_command(0, "E", "frob", 0);
		input.initialise_command(0, "F", "flashlight", 0);
		input.initialise_command(0, "MouseLeft", "click", 0);
		
		auto sb = asset.load_soundbuffer("data/blip.ogg");
		sounds.emplace_back( std::make_unique<sf::Sound>(*sb) );
		sound_click = sounds.back().get();
		
		sf::Music* music = asset.load_music("data/loop.ogg");
		music->setLoop(true);
		music->play();
		
		sf::Texture* mouse_pointer_tex = asset.load_texture("data/mouse_pointer.png");
		sprites.emplace_back( std::make_unique<sf::Sprite>(*mouse_pointer_tex) );
		mouse_pointer_sprite = sprites.back().get();
		
		clear_colour = {128,32,128};
	
		// create menu buttons
		
		// menu
		sf::Texture* tex_levels  = asset.load_texture("data/tex_levels.png");
		sf::Texture* tex_credits = asset.load_texture("data/tex_credits.png");
		sf::Texture* tex_exit    = asset.load_texture("data/tex_exit.png");
		SelectBox* box_levels  = add_box(SCENE_MENU, SCENE_LEVELS,  *tex_levels,  {20, 20});
		SelectBox* box_credits = add_box(SCENE_MENU, SCENE_CREDITS, *tex_credits, {20, 60});
		SelectBox* box_exit    = add_box(SCENE_MENU, SCENE_EXIT,    *tex_exit,    {20, 100});
		// link to ludum dare page
		// link to github
		
		box_levels->d = box_credits;
		box_credits->u = box_levels;
		box_credits->d = box_exit;
		box_exit->u = box_credits;

		// levels
			// each unlocked level (according to persist)

		// credits
			// back
			
		// exit
			// are you sure
	}
	
	void update()
	{
		switch (scene_state)
		{
			case SCENE_BLANK:       { update_blank();       return; }
			case SCENE_LOGO:        { update_logo();        return; }
			case SCENE_TITLE:       { update_title();       return; }
			case SCENE_MENU:    { update_mainmenu();    return; }
			case SCENE_LEVELS: { update_levelselect(); return; }
			case SCENE_CREDITS:     { update_credits();     return; }
			case SCENE_EXIT:        { update_exit();        return; }
		}
	}
	
	void update_blank()
	{
		
		if (clock.getElapsedTime() > scene_times[SCENE_BLANK])
		{
			scene_state = SCENE_LOGO;
			clock.restart();
		}
	}
	
	void update_logo()
	{
		
		if (clock.getElapsedTime() > scene_times[SCENE_LOGO])
		{
			scene_state = SCENE_TITLE;
			clock.restart();
		}
	}
	
	void update_title()
	{
		
		if (clock.getElapsedTime() > scene_times[SCENE_TITLE])
		{
			scene_state = SCENE_MENU;
			clock.restart();
		}
	}
	
	void update_mainmenu()
	{
		
		if (clock.getElapsedTime() > scene_times[SCENE_MENU])
		{
			scene_state = SCENE_LEVELS;
			clock.restart();
		}
	}
	
	void update_levelselect()
	{
		
	}
	
	void update_credits()
	{
		
		if (clock.getElapsedTime() > scene_times[SCENE_CREDITS])
		{
			scene_state = SCENE_CREDITS;
			clock.restart();
		}
	}
	
	void update_exit()
	{
		
		if (clock.getElapsedTime() > scene_times[SCENE_EXIT])
		{
			persist.game_end = true;
		}
	}
	
	void _update_()
	{
		sf::Vector2f target;
		sf::Vector2f center = constrain({0.f, 0.f, 128.f, 128.f}, target);
		
		view.setCenter(center);
		
		// mouse position set
		sf::Vector2f pointer_pos = center + (sf::Vector2f{input.mouse}/ZOOM) - sf::Vector2f(ZOOM_WH, ZOOM_HH);
		mouse_pointer_sprite->setPosition(pointer_pos);
		
		bool click_in = input.get_command.at(0).at("click");
		if (!click)
		{
			if (click_in)
			{
				click = true;
				
				if (clicked < 3)
				{
					++clicked;
					sound_click->play();
				}
				else
				{
					++persist.scene;
					persist.scene_end = true;
				}
			}
		}
		else if(!click_in)
		{
			click = false;
		}
		
		/*if (!player_dead)
		{
			if ( (frame_index % gait) == 0)
			{
				sf::Vector2f control;
				{
					bool r = input.get_command.at(0).at("right");
					bool l = input.get_command.at(0).at("left");
					bool d = input.get_command.at(0).at("down");
					bool u = input.get_command.at(0).at("up");
					control = sf::Vector2f( (r?1.0f:0.0f) - (l?1.0f:0.0f), (d?1.0f:0.0f) - (u?1.0f:0.0f) );
				}
				velocity += control * speed;
			}
		}*/
	}
	
	void render()
	{
		// backgrounds
		
		//render_texture.draw(room);
		
		// objects
		
		for (auto& sprite : sprites)
		{
			render_texture.draw(*sprite);
		}
		
		// example: draw shadows with shader
		
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