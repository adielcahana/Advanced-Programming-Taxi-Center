#ifndef ASS2_MAP_H
#define ASS2_MAP_H

#include <vector>
#include <queue>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include "Bfs.h"

using namespace std;
/******************************************************************************
* Point: defines a 2d vector, that represnt a location on the map. implements
* Node interface in order to be usable by the Bfs algorithm
******************************************************************************/
class Point: public Node{
	private:
		int x;
		int y;
	public:
		Point(int x, int y): x(x), y(y) {};
		Point(Point &p);
	    int getX();
		int getY();
		int hash();
        bool operator==(const Point &rhs) const;
        bool operator!=(const Point &rhs) const;
        static Point* deserialize(string s);
		string toString();
        /**********************************************************************
        * The Function Operation: print a represantation of a 2d point
        * we were unable to locate the function in the .cpp file
        ***********************************************************************/
        friend ostream& operator<<( ostream& output,const Point& p) {
            output << "(" << p.x << "," << p.y << ")";
            return output;
    }
};

/******************************************************************************
* PointComparator: defines a comparing method for points.
* this class provides a clean way to downcast Node pointer to Point.
* implements NodeComparator in order to provide a comparing method for
* the Bfs algorithm
******************************************************************************/
class PointComparator: public NodeComparator{
public:
    virtual bool equals(const Node *n1, const Node *n2) const;
};

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