#include "Enemy.h"
#include <chrono>

using std::chrono::steady_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

vector<Enemy*> Enemy::enemies_;

void Enemy::stepAll()
{
	static bool init = true;
	static int time_passed; //since last step in ms
	static steady_clock::time_point   then;
	static steady_clock::time_point   now;
	static int tick = 0;

	if(init)
	{
		then = steady_clock::now();
		init = false;
	}

	now = steady_clock::now();

	time_passed += duration_cast<milliseconds>(now - then).count();

	if (time_passed >= 1000) //one second passed
	{
		for(Enemy* enemy : enemies_)
		{
			enemy->step();
		}
		time_passed = 0;
	}

	then = now;


}

void Enemy::drawEnemies(sf::RenderWindow& window)
{
	for(Enemy* enemy : enemies_)
	{
		enemy->draw(window);
	}
}

void Enemy::step()
{
	static int x_delta[4] = {0,1,0,-1};
	static int y_delta[4] = {-1,0,1,0};
	static int i = 1;
	Position new_pos = {pos_.x_ + x_delta[i], pos_.y_ + y_delta[i]};
	while(!move(new_pos))
	{
		i = (++i) % 4;
		new_pos = {pos_.x_ + x_delta[i], pos_.y_ + y_delta[i]};
	}
	cout << i << endl;
}

Enemy::Enemy(string name, Position pos, Stats stats, string texture) : Character(name, pos, stats, texture)
{
	enemies_.push_back(this);
}
