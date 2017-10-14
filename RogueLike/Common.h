#pragma once

//--- Constants ---//
const unsigned int TILE_SIZE = 32;
const unsigned int WINDOW_WIDTH = 1024;
const unsigned int WINDOW_HEIGHT = 720;
const int DELTA_X[4] = { 0, 1, 0, -1 };
const int DELTA_Y[4] = { -1, 0, 1, 0 };
//#define OS_WIN
#define OS_LIN

//--- SFML ---//
#ifdef OS_WIN
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#endif // OS_WIN

#ifdef OS_LIN
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#endif // OS_LIN


//--- Console I/O ---//
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

//--- STL Containers ---//
#include <vector>
#include <list>
#include <map>
#include <string>
using std::array;
using std::vector;
using std::list;
using std::map;
using std::pair;
using std::string;

inline size_t absolute(int i) { return i > 0 ? i : i*(-1); };
inline bool roll(size_t count, size_t out_of) {return ((rand() % out_of) < count);}


//--- Utility ---//
#include <cstdlib>

//---  ---//
