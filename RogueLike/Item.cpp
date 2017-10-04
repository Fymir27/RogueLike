#include "Item.h"

Item::Item(string name, string descr, size_t count, size_t max_count, string texture_file) : 
name_(name), description_(descr), max_count_(max_count), count_(count), texture_file_(texture_file)
{
	cout << name << " x" << count << " created." << endl;
	if (!texture_.loadFromFile(texture_file))
		cout << "Couldn't load texture" << texture_file << endl;
	sprite_.setTexture(texture_);
}
