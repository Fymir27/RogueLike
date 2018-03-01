#include "Ressource.h"

Ressource::Ressource(long max_value, long cur_value, long min_value) : max_value_(max_value), min_value_(min_value), cur_value_(cur_value)
{
}

Ressource::Ressource(long max_value, long cur_value) : max_value_(max_value), min_value_(0), cur_value_(cur_value)
{
}

Ressource::Ressource(long max_value) : max_value_(max_value), min_value_(0), cur_value_(max_value)
{
}

Ressource & Ressource::operator+=(const long value)
{
	cur_value_ += value;
	if (cur_value_ > max_value_)
		cur_value_ = max_value_;
	//cout << "(" << cur_value_ << "|" << max_value_ << ")" << endl;
	return *this;
}

long Ressource::add(const long value)
{
	cur_value_ += value;
	long overflow = cur_value_ - max_value_;
	if (overflow > 0)
	{
		cur_value_ = max_value_;
		return overflow;
	}
	return 0;
}

Ressource & Ressource::operator-=(const long value)
{
	cur_value_ -= value;
	if (cur_value_ < min_value_)
		cur_value_ = min_value_;
	//cout << "(" << cur_value_ << "|" << max_value_ << ")" << endl;
	return *this;
}
