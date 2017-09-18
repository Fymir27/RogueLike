#pragma once

template <class T>
class Singleton
{
public:
    static T* get();

private:
    static T* instance_;
};

template<class T>
T* Singleton<T>::instance_ = NULL;

template<class T>
T* Singleton<T>::get()
{
	if(!instance_)
	{
		instance_ = new T();
	}
	return instance_;
};