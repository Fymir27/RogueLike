#pragma once

template <class T>
class Singleton
{
public:
    static T* get();
    static void destroy();

private:
    static T* instance_;
};

template<class T>
T* Singleton<T>::instance_ = nullptr;

template<class T>
T* Singleton<T>::get()
{
	if(!instance_)
	{
		instance_ = new T();
	}
	return instance_;
};

template <class T>
void Singleton<T>::destroy()
{
    delete instance_;
    instance_ = nullptr;
}