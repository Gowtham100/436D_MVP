#include <iostream>
#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <math.h>
#include <vector>
#include <cstdlib>
#include "particle.hpp"

using namespace sf;



class Bullet
{
public:
	CircleShape shape;
	Vector2f currVelocity;
	float maxSpeed;

	Bullet(float radius = 5.f)
		: currVelocity(0.f, 0.f), maxSpeed(15.f)
	{
		this->shape.setRadius(radius);
		this->shape.setFillColor(Color::Red);
	}
};

vector<Particle> generateEnemyDots(int);
vector<Vertex> generateEnemySpecks(vector<Particle>);

vector<Particle> generatePlayerDots(int);
vector<Vertex> generatePlayerSpecks(vector<Particle>);

RectangleShape enemy;
Vector2f enemyPos;
Vector2f playerPos;

int main()
{

	srand(time(NULL));

	RenderWindow window(VideoMode(1920, 1080), "Initial Player Movement", Style::Default);
	window.setFramerateLimit(60);

	

	//Player
	CircleShape player(25.f);
	player.setFillColor(Color::White);
	player.setPointCount(3);
	player.setOrigin(25.f, 25.f);
	player.setPosition(960, 1050);

	//Bullets
	Bullet b1;
	std::vector<Bullet> bullets;

	//Enemy
	
	enemy.setFillColor(Color::Magenta);
	enemy.setSize(Vector2f(50.f, 50.f));
	int spawnCounter = 20;

	RectangleShape e1;
	std::vector<RectangleShape> enemies;

	//Particle
	vector<Particle> dots;
	vector<Vertex> specks;


	//Vectors
	Vector2f playerCenter;
	Vector2f mousePosWindow;
	Vector2f aimDir;
	Vector2f aimDirNorm;
	Vector2f enemyDir;
//RectangleShape enemy;


	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		//Update
		//Vectors
		playerCenter = Vector2f(player.getPosition());
		mousePosWindow = Vector2f(Mouse::getPosition(window));
		aimDir = mousePosWindow - playerCenter;
		aimDirNorm = aimDir / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));

		float PI = 3.14159265f;
		float deg = atan2(aimDirNorm.y, aimDirNorm.x) * 180 / PI;
		player.setRotation(deg + 90);


		//Player
		if (Keyboard::isKeyPressed(Keyboard::A))
			player.move(-10.f, 0.f);
		if (Keyboard::isKeyPressed(Keyboard::D))
			player.move(10.f, 0.f);
		if (Keyboard::isKeyPressed(Keyboard::W))
			player.move(0.f, -10.f);
		if (Keyboard::isKeyPressed(Keyboard::S))
			player.move(0.f, 10.f);

		//Enemies
		if (spawnCounter < 20)
			spawnCounter++;
		
		if(spawnCounter >= 20 )
			{
				enemy.setPosition(Vector2f(rand() % window.getSize().x, rand() % (window.getSize().x/100)));
				enemies.push_back(RectangleShape(enemy));
			
	
				spawnCounter = 0;
			}


		aimDir.x = 0.f;
		aimDir.y = 10.f;



		for (size_t i = 0; i < enemies.size(); i++) {
			enemies[i].move(aimDir);	
		}

	

		


		//Shooting
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			b1.shape.setPosition(playerCenter);
			b1.currVelocity = aimDirNorm * b1.maxSpeed;

			bullets.push_back(Bullet(b1));
		}

		for (size_t i = 0; i < bullets.size(); i++)
		{
			bullets[i].shape.move(bullets[i].currVelocity);

			//Out of bounds
			if (bullets[i].shape.getPosition().x < 0 || bullets[i].shape.getPosition().x > window.getSize().x
				|| bullets[i].shape.getPosition().y < 0 || bullets[i].shape.getPosition().y > window.getSize().y)
			{
				bullets.erase(bullets.begin() + i);
				
			}
			else
				{
					//Enemy collision
					for (size_t k = 0; k < enemies.size(); k++)
						{
							enemyPos = enemies[k].getPosition();
							if (bullets[i].shape.getGlobalBounds().intersects(enemies[k].getGlobalBounds()))
							{
								//dots.clear();
								//specks.clear();

								int num = 1000;

								dots = generateEnemyDots(num);
								specks = generateEnemySpecks(dots);

								bullets.erase(bullets.begin() + i);
								enemies.erase(enemies.begin() + k);
								
								break;
							}
						}
					
				}

		}

		//ship collision
		for (size_t i = 0; i < enemies.size(); i++) {
			if (player.getGlobalBounds().intersects(enemies[i].getGlobalBounds()))
			{
				int num = 1000;
				playerPos = player.getPosition();

				dots = generatePlayerDots(num);
				specks = generatePlayerSpecks(dots);

				enemies.erase(enemies.begin() + i);
				player.setPosition(960, 1050);
				
				
				
			}
		}

		//update part
		for (int i = 0; i<dots.size(); i++)
		{
			dots[i].update();
			specks[i].position = dots[i].pos;
		}


		//fix Window Position
		if (player.getPosition().x >= window.getSize().x) {
			player.setPosition(window.getSize().x, player.getPosition().y);
		}
		if (player.getPosition().x <= 0) {
			player.setPosition(0, player.getPosition().y);
		}

		if (player.getPosition().y >= window.getSize().y) {
			player.setPosition(player.getPosition().x, window.getSize().y);
		}

		if (player.getPosition().y <= 0) {
			player.setPosition(player.getPosition().x, 0);
		}

		//Draw
		window.clear();
		

		for (size_t i = 0; i < specks.size(); i++)
		{
			window.draw(&specks[0], specks.size(), Points);
		}

		for (size_t i = 0; i < enemies.size(); i++)
				{
					window.draw(enemies[i]);
			}

		window.draw(player);

		for (size_t i = 0; i < bullets.size(); i++)
		{
			window.draw(bullets[i].shape);
		}

		window.display();
	}

	return 0;
}

vector<Particle> generateEnemyDots(int num)
{
	vector <Particle> parts;
	float radius, theta, x, y;

	for (int i = 0; i < num; i++) {
		Particle temp;

		radius = rand() % 1000;
		radius = (radius / 1000) + 0.005;

		theta = rand() % 10000;
		
		x = radius * cos(theta);
		y = radius * sin(theta);

		temp.vel = Vector2f(x, y);
		temp.pos = enemyPos;

		parts.push_back(temp);
	}

	return parts;
}

vector<Vertex> generateEnemySpecks(vector<Particle> dots)
{
	vector<Vertex> verts;

	for (int i = 0; i<dots.size(); i++)
	{
		Vertex temp;
		temp.position = dots[i].pos;
		temp.color = Color::Red;

		verts.push_back(temp);
	}

	return verts;

}

vector<Particle> generatePlayerDots(int num)
{
	vector <Particle> parts;
	float radius, theta, x, y;

	for (int i = 0; i < num; i++) {
		Particle temp;

		radius = rand() % 1000;
		radius = (radius / 1000) + 0.005;

		theta = rand() % 10000;

		x = radius * cos(theta);
		y = radius * sin(theta);

		temp.vel = Vector2f(x, y);
		temp.pos = playerPos;

		parts.push_back(temp);
	}

	return parts;
}

vector<Vertex> generatePlayerSpecks(vector<Particle> dots)
{
	vector<Vertex> verts;

	for (int i = 0; i<dots.size(); i++)
	{
		Vertex temp;
		temp.position = dots[i].pos;
		temp.color = Color::White;

		verts.push_back(temp);
	}

	return verts;

}