
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

#define SCREEN_W 1440
#define SCREEN_H 720
#define ZOOM 2.0f
#define ZOOM_W SCREEN_W/ZOOM
#define ZOOM_H SCREEN_H/ZOOM
#define ZOOM_WH ZOOM_W/2
#define ZOOM_HH ZOOM_H/2
#define ZOOM_WQ ZOOM_W/4
#define ZOOM_HQ ZOOM_H/4

#define FADE 30

#include "sfml_math_extra.hpp"

#include "timer.hpp"
#include "input.hpp"
#include "persist.hpp"

//#include "scene_menu.hpp"
#include "scene_worldgame.hpp"

#include <string>
#include <unordered_map>
#include <memory>

const sf::VideoMode VIDEO_MODE(SCREEN_W, SCREEN_H);

int main()
{
	std::unordered_map<int, std::unordered_map<std::string, bool>> get_command;
	Input input(get_command);
	input.initialise_command(0, "Escape", "quit", 0);

	sf::RenderWindow window(VIDEO_MODE, "shrapx", sf::Style::Close);
	window.setFramerateLimit(60);
	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(false);
	window.setMouseCursorVisible(false);
	
	Persist persist;
	sx::timer t;
	std::unique_ptr<Scene> scene;
	bool the_terminator = false;
	
	while(!the_terminator)
	{
		the_terminator = persist.game_end || input.events(window) || get_command.at(0).at("quit");
		
		if (!scene || persist.scene_end)
		{
			switch (persist.scene)
			{
				case 1: persist.scene = 0; // rotate to start
				//case 0: { scene = std::make_unique<SceneMenu>(input, persist); break; }
				case 0: { scene = std::make_unique<WorldGame>(input, persist); break; }
			}
			scene->load();
		}
		
		if ( t.update() )
		{
			scene->update();
		}
		else
		{
			scene->clear();
			scene->draw();
			window.draw(scene->get_sprite());
			window.display();
		}
	}
	scene.reset();
	window.close();
}
