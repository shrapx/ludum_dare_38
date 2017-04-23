#ifndef _SFML_MATH_EXTRA_HPP_
#define _SFML_MATH_EXTRA_HPP_

#include "SFML/System.hpp"

#define M_PI	3.14159265358979323846
#define M_PI_2	1.57079632679489661923

#include <algorithm>
#include <cmath>

using uint = unsigned int;

float dot(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return (a.x*b.x)+(a.y*b.y);
};

float length(const sf::Vector2f& a)
{
	return std::sqrt(a.x * a.x + a.y * a.y);
}

float dist(const sf::Vector2f& a, const sf::Vector2f& b)
{
	sf::Vector2f c {b.x - a.x, b.y - a.y};
	return length(c);
}

sf::Vector2f lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t)
{
	return {a.x + t*(b.x-a.x), a.y + t*(b.y-a.y)};
}

sf::Color lerp(const sf::Color& a, const sf::Color& b, float t)
{
	return
	{
		sf::Uint8(a.r + t*(b.r-a.r)),
		sf::Uint8(a.g + t*(b.g-a.g)),
		sf::Uint8(a.b + t*(b.b-a.b))
	};
}

// example: sf::View view; view.setCenter( constrain({60.f, 32.f, 68.f, 98.f}, sf::Vector2f(0,0));
sf::Vector2f constrain(const sf::FloatRect& border, const sf::Vector2f& target)
{
	return
	{
		std::min(border.width,  std::max(border.left, target.x)),
		std::min(border.height, std::max(border.top,  target.y))
	};
}

#endif
