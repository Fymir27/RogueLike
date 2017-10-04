#pragma once

//--- Constants ---//
const unsigned int TILE_SIZE = 32;
const unsigned int WINDOW_WIDTH = 860;
const unsigned int WINDOW_HEIGHT = 480;
//#define OS_WIN
#define OS_WIN

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
using std::vector;
using std::list;
using std::map;
using std::pair;
using std::string;

inline size_t absolute(int i) { return i > 0 ? i : i*(-1); };


//---  ---//

//---  ---//
