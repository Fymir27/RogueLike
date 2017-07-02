#include "Player.h"
#include "Room.h"

Player* current_player = NULL;

Player::Player(string name, Position pos, Stats stats) : Character(name, pos, stats)
{
	if (!texture_.loadFromFile("../images/player.png"))
		cout << "Failed to load player texture!" << endl;

	if(!font_.loadFromFile("../fonts/Arcade.ttf"))
		cout << "Failed to load player font!" << endl;

	text_.setFont(font_);
	text_.setFillColor(sf::Color::White);

	sprite_.setTexture(texture_);	

	cout << this;
}

void Player::processInput(const sf::Event& event)
{
	auto key = event.key.code;
	Position new_pos = pos_;
	if(key == sf::Keyboard::Right)     new_pos.x_++;
	else if(key == sf::Keyboard::Left) new_pos.x_--;
	else if(key == sf::Keyboard::Up)   new_pos.y_--;
	else if(key == sf::Keyboard::Down) new_pos.y_++;



	Field* field = current_room->getField(new_pos.x_, new_pos.y_);

	if(field == NULL)
		return;

	if(field->stepOn(this))
	{
		pos_ = new_pos;
	}
}

void Player::draw(sf::RenderWindow& window)
{
	sprite_.setPosition(sf::Vector2f(pos_.x_ * TILE_SIZE, pos_.y_ * TILE_SIZE));
	window.draw(sprite_);
	text_.setPosition(inventory_->getSprite().getPosition() + sf::Vector2f(10, TILE_SIZE * 3));
	char string[1024];
	sprintf_s(string, 1024, "%s Stats\n%s", name_.c_str(), getStatString(stats_));
	text_.setString(string);
	window.draw(text_);
}

Inventory& Player::getInventory() const
{
	return *inventory_;
}


