#pragma once

#include "Common.h"
#include "Types.h"

template<typename T>
class Ressource
{
	private:
	T max_value_;
	T min_value_;
	T cur_value_;

	public:
	Ressource(){}
	Ressource(T max_value, T cur_value, T min_value) : max_value_(max_value), min_value_(min_value), cur_value_(cur_value) {}
	Ressource(T max_value, T cur_value) :							 max_value_(max_value), min_value_(0), cur_value_(cur_value) {}
	Ressource(T max_value) :													 max_value_(max_value), min_value_(0), cur_value_(max_value) {}

	Ressource& operator+=(const T& value)
	{
		cur_value_ += value;
		if (cur_value_ > max_value_)
			cur_value_ = max_value_;
		return *this;
	}

	Ressource& operator-=(const T& value)
	{
		cur_value_ -= value;
		if (cur_value_ < min_value_)
			cur_value_ = min_value_;
		return *this;
	}

	bool depleted() { return cur_value_ == min_value_; }
	bool full() { return cur_value_ == max_value_; }

	bool operator==(const Ressource& other)	{	return (this->cur_value_ == other.cur_value_); }
	bool operator>=(const Ressource& other) { return (this->cur_value_ >= other.cur_value_); }
	bool operator<=(const Ressource& other) { return (this->cur_value_ <= other.cur_value_); }
	bool operator>(const Ressource& other) { return (this->cur_value_ > other.cur_value_); }
	bool operator<(const Ressource& other) { return (this->cur_value_ < other.cur_value_); }

	void print()	{	cout << "(" << cur_value_ << "|" << max_value_ << ")" << endl; }

};
