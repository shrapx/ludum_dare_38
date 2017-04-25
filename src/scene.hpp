#ifndef _SCENE_HPP_
#define _SCENE_HPP_

#include "input.hpp"
#include "persist.hpp"
#include "asset.hpp"

class Scene
{
	
public:

	Input& input;
	Persist& persist;
	
	Asset asset;
	
	sf::RenderTexture render_texture;
	sf::View view;
	sf::Sprite sprite;
	sf::Color clear_colour = sf::Color(110,149,181);
	
	bool advance = false;
	
	Scene(Input& input, Persist& persist) : input(input), persist(persist)
	{
		persist.scene_end = false;
		render_texture.create(ZOOM_W, ZOOM_H, false);
		view = render_texture.getDefaultView();
		sprite.setTexture(render_texture.getTexture());
		sprite.setScale( float(ZOOM), float(ZOOM) );
		
		input.initialise_command(0, "Return", "advance", 0);
	}
	
	virtual void load() {}
	virtual void update()
	{
		bool in_advance = input.get_command.at(0).at("advance");
		
		if (!advance)
		{
			if (in_advance) advance = true;
		}
		else if (!in_advance)
		{
			++persist.scene;
			persist.scene_end = true;
		}
	}
	virtual void render() {}

	void clear()
	{
		render_texture.setView(view);
		render_texture.clear(clear_colour);
	}
	
	void draw()
	{
		render();
		render_texture.display();
	}
	
	const sf::Sprite& get_sprite() const
	{
		return sprite;
	}
};

#endif
