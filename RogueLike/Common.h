#pragma once

//--- Constants ---//
const unsigned int TILE_SIZE = 64;
const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 640;
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
using std::vector;
using std::list;
using std::map;
using std::pair;
using std::string;


//---  ---//

//---  ---//
