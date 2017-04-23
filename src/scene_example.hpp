#ifndef _LD_SCENE_EXAMPLE_HPP_
#define _LD_SCENE_EXAMPLE_HPP_

#include "scene.hpp"

class SceneExample : public Scene
{
	using Scene::Scene;
	
	uint frame_index = 0;
	
	void load()
	{
		// input
		
		input.initialise_command(0, "W", "up", 0);
		input.initialise_command(0, "A", "left", 0);
		input.initialise_command(0, "S", "down", 0);
		input.initialise_command(0, "D", "right", 0);
		
		input.initialise_command(0, "E", "frob", 0);
		input.initialise_command(0, "F", "flashlight", 0);
		input.initialise_command(0, "MouseLeft", "frob", 0);
		
		// assets
		/*
		sf::Texture* tex    = asset.load("data/exterior_light_collide.png");
		sf::Shader* sha     = asset.load("data/darkness_v.glsl", "data/darkness_f.glsl");
		sf::SoundBuffer* sb = asset.load("data/blip.ogg");
		
		sf::Music* mus = asset.load("data/loop.ogg");
		mus->setLoop(true);
		mus->play();
		
		// after load
		sf::Texture* p1 = *asset.textures.at("player");
		
		// variables
		
		frame_index = 100;
		*/
	}
	
	void update()
	{
		++frame_index;
		/*
		view.setCenter( constrain({60.f, 32.f, 68.f, 98.f}, sf::Vector2f(0,0) );
		
		// mouse position set
		sf::Vector2f pointer_pos = pos + (sf::Vector2f{input.mouse}/ZOOM) - sf::Vector2f(ZOOM_WH, ZOOM_HH);
		pointer->setPosition(pointer_pos);
		
		if (!player_dead)
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
		/*
		render_texture.draw(room);
		
		// objects
		
		render_texture.draw(*player);
		
		// example: draw shadows with shader
		
		if (night)
		{
			sf::RenderStates states;
			states.blendMode = sf::BlendMultiply; //sf::BlendAlpha;
			states.shader = shadow_shader;
			render_texture.draw(shadows_drawable, states);
		}
		
		// foregrounds
		
		render_texture.draw(*pointer);*/
	}
};

#endif