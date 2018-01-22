#pragma once

#ifndef particle_hpp
#define particle_hpp

#include <SFML\Graphics.hpp>
#include <stdio.h>


using namespace sf;
using namespace std;

class Particle
{
public:
	Particle()
	{
		acc = Vector2f(0, 0);
		vel = Vector2f(0, 0);
		pos = Vector2f(0, 0);
	}

	void update();
	float distance(float, float, float, float);

	Vector2f acc;
	Vector2f vel;
	Vector2f pos;

};

#endif // !particle_hpp

