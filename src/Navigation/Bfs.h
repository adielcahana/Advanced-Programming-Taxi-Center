
#ifndef ASS1_BFS_H
#define ASS1_BFS_H

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

/******************************************************************************
* Node: an interface, that represnt a location on the map. implemented by
*  objects in order to be usable by the Bfs algorithm.
******************************************************************************/
class Node {
private:
    Node* father;
    bool inRoute;
public:
	Node(): father(NULL), inRoute(false){};
    virtual int hash()= 0;
    void setFather(Node* father);
    void setInRoute();
    bool isInRoute();
    Node* getFather();
};

/******************************************************************************
* NodeComparator: Abstract Class.
* provide a clean way to downcast Node to the objects that implement node.
* This class give the option to compare between the objects of the class
 * that implement node.
******************************************************************************/
class NodeComparator {
    public:
        virtual bool equals(const Node* n1, const Node* n2) const = 0;
};

/******************************************************************************
* Graph: an interface.
* defines a map, with a logic of finding adjacent nodes.
* implemented by objects that represented some map in order to
* be usable by the Bfs algorithm.
******************************************************************************/
class Graph{
public:
	~Graph(){};
  	virtual queue<Node*>* getAdjacent(const Node *node)= 0;
};

/******************************************************************************
* Bfs: an algorithm of scanning the map. get a start node and end node
 * on a map and find the shortest way between them.
******************************************************************************/
class Bfs {
private:
    std::vector<bool> visited;
    vector<Node*>* createRoute(Node* end ,Node* start, NodeComparator* comparator);
    void resetVisited();
public:
    Bfs(Node* maxNode);
    vector<Node*>* find_shortest_route(Graph* graph, Node* start, Node* end, NodeComparator* comparator);
};

#endif //ASS1_BFS_H
