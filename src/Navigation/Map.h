#ifndef ASS2_MAP_H
#define ASS2_MAP_H

#include <vector>
#include <queue>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <sstream>
#include "Bfs.h"
#include "Trip.h"
#include "Point.h"

using namespace std;

/******************************************************************************
* Map: defines a width*length grid, with a logic of finding adjacent points
* in order to implemnet Graph interface. this class will help to navigate
* and control the drivers
******************************************************************************/
class Map: public Graph{
	friend class MapTest;
	private:
		int length;
		int width;
        vector<bool> obstaclesHash;
		vector<Point*>* obstacles;
        bool isInMap(Point *p);
	public:
        Map(int width, int length, vector<Point*>* obstacles = NULL);
		~Map();
		vector<Point*>* getRoute(Point *start, Point *end);
		queue<Node*>* getAdjacent(const Node *point);
        static Map* deserialize(string s);
        string toString();
		bool isTripInMap(Trip* trip);
        /**********************************************************************
        * The Function Operation: print a represantation of a 2d map
        * we were unable to locate the function in the .cpp file
        ***********************************************************************/
		friend ostream& operator<<(ostream &output, const Map& g) {
			for (int i = g.length - 1; i >= 0; i--){
				output << i << " ";
				for (int j = 0; j < g.width; j++){
					output << "-";
				}
				output << endl;
			}
            output << "  ";
            for (int i = 0; i < g.width; i++) output << i;
			return output;

		}
};

#endif //ASS2_MAP_H