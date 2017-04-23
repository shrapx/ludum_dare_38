#ifndef _SX_TIMER_HPP_
#define _SX_TIMER_HPP_

#include <array>
#include <ratio>
#include <chrono>
#include <time.h>

// todo: make sfml sf::Time and sf::Clock version
// sf::Clock frame_clock;
// sf::Time frame_time = sf::Time::Zero;

namespace sx
{

class timer
{
	using duration = std::chrono::milliseconds;
	using clock = std::chrono::high_resolution_clock;
	
	constexpr static float denom = 1000.0f;
	
private:

	clock::time_point time_start;
	
	float time_ipo = 0.0f;
	float time_step = denom / 30.0f;
	float time_last = 0.0f;
	float time_curr = 0.0f;
	float time_next = 0.0f;

public:

	timer()
	{
		srand(time(NULL));
		time_start = clock::now();
	}
	
	bool update()
	{
		time_last = time_curr;
		time_curr = std::chrono::duration_cast<duration>(clock::now() - time_start).count();

		if(time_curr > time_next) // update
		{
			time_next += time_step;

			return true;
		}
		else   // render
		{
			// calc render interpolation
			time_ipo = (time_curr - time_next + time_step) / time_step;
			return false;
		}
	};

	void set_step(float new_rate)
	{
		time_step = denom / new_rate;
	};

	float get_change() const
	{
		return time_curr - time_last;
	};

	float get_current() const
	{
		return time_curr;
	};
	
	float get_step() const
	{
		return time_step;
	};

	float get_ipo() const
	{
		return time_ipo;
	};

	template<typename T, typename U>
	static T lerp(T a, T b, U amt) { return a + (b - a) * amt; };
};

};

#endif
