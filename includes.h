#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <list>
#include <stack>
#include <ctime>
#include <sstream>
#include <cstring>
#include <math.h>
#include <typeinfo>

#define vector std::vector
#define list std::list
#define queue std::queue
#define string std::string

#define KING 0
#define PAWN 1
#define ROOK 2
#define BISHOP 3
#define KNIGHT 4
#define QUEEN 5
#define BLACK 0
#define WHITE 1


#define CHECK 1
#define MAT 3   // end of game by winning
#define PAT 2   // end of game by no more possible moves
#define TIMEOUT_W 4
#define TIMEOUT_B 5

#define DEBUG
//#define PAWN_DEBUG
//#define SUPER_DEBUG

#include "settings.cpp"
#include "position.cpp"
#include "figures.cpp"
#include "chess.cpp"
#include "AI.cpp"

