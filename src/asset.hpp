#ifndef _LD_ASSET_HPP_
#define _LD_ASSET_HPP_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <cassert>

#include <string>
#include <unordered_map>
#include <memory>

class Asset
{
public:
	
	std::unordered_map<std::string, std::unique_ptr<sf::Shader>> shaders;
	std::unordered_map<std::string, std::unique_ptr<sf::Font>> fonts;
	std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
	std::unordered_map<std::string, std::unique_ptr<sf::SoundBuffer>> soundbuffers;
	std::unordered_map<std::string, std::unique_ptr<sf::Music>> music;
	
	sf::Shader* load_shader(const std::string& file_vert, const std::string& file_frag)
	{
		auto emplace_result = shaders.emplace(file_vert, std::make_unique<sf::Shader>());
		assert(emplace_result.second);
		sf::Shader* shad = emplace_result.first->second.get();
		assert(shad->loadFromFile(file_vert, file_frag) );
		return shad;
	}
	
	sf::Shader* load_shader(const std::string& file_frag)
	{
		auto emplace_result = shaders.emplace(file_frag, std::make_unique<sf::Shader>());
		assert(emplace_result.second);
		sf::Shader* shad = emplace_result.first->second.get();
		assert(shad->loadFromFile(file_frag, sf::Shader::Fragment) );
		return shad;
	}
	
	sf::Font* load_font(const std::string& filepath)
	{
		auto emplace_result = fonts.emplace(filepath, std::make_unique<sf::Font>());
		assert(emplace_result.second);
		sf::Font* font = emplace_result.first->second.get();
		assert( font->loadFromFile(filepath) );
		return font;
	}

	sf::Texture* load_texture(const std::string& filepath)
	{
		auto emplace_result = textures.emplace(filepath, std::make_unique<sf::Texture>());
		sf::Texture* tex = emplace_result.first->second.get();
		assert( tex->loadFromFile(filepath) );
		return tex;
	}

	sf::SoundBuffer* load_soundbuffer(const std::string& filepath)
	{
		auto emplace_result = soundbuffers.emplace(filepath, std::make_unique<sf::SoundBuffer>());
		sf::SoundBuffer* sb = emplace_result.first->second.get();
		assert( sb->loadFromFile(filepath) );
		return sb;
	}
	
	sf::Music* load_music(const std::string& filepath)
	{
		auto emplace_result = music.emplace(filepath, std::make_unique<sf::Music>());
		sf::Music* sb = emplace_result.first->second.get();
		assert( sb->openFromFile(filepath) );
		return sb;
	}

};
#endif
