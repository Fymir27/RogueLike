#include "Enemy.h"

vector<Enemy*> Enemy::enemies_;

void Enemy::moveEnemies()
{
	for(Enemy* enemy : enemies_)
	{
		enemy->move();
	}
}

void Enemy::drawEnemies(sf::RenderWindow& window)
{
	for(Enemy* enemy : enemies_)
	{
		enemy->draw(window);
	}
}

void Enemy::move()
{
	//cout << name_ << " moving!" << endl;
}

Enemy::Enemy(string name, Position pos, Stats stats, string texture) : Character(name, pos, stats, texture)
{
	enemies_.push_back(this);
}
