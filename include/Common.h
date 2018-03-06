#pragma once

#include "OS.h"

//--- Constants ---//
const unsigned int TILE_SIZE = 32;
const unsigned int WINDOW_WIDTH = 1024;
const unsigned int WINDOW_HEIGHT = 720;
const int DELTA_X[4] = { 0, 1, 0, -1 };
const int DELTA_Y[4] = { -1, 0, 1, 0 };

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

//--- File I/O ---//
#include <fstream>

//--- STL Containers ---//
#include <vector>
#include <list>
#include <map>
#include <string>
#include <array>
#include <algorithm>
using std::array;
using std::vector;
using std::list;
using std::map;
using std::pair;
using std::string;

#include <memory>
using std::unique_ptr;
using std::shared_ptr;

//--- XML Parsing ---//
#include "pugixml.hpp"
using namespace pugi;

//--- Utility ---//
#include <cstdlib>
#include <cmath>
#include <random>
#include "Utils.h"

//---  ---//
#include "GameObject.h"
