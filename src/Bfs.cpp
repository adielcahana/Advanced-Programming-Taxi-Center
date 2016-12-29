#include"Bfs.h"

/******************************************************************************
* The Function Operation: return the node's father.
******************************************************************************/
Node* Node::getFather() {
    return this->father;

}

/******************************************************************************
* The Function Operation: set the node's father.
******************************************************************************/
void Node::setFather(Node *father){
    this->father = father;
}

/******************************************************************************
* The Function Operation: set that the node is in the route
******************************************************************************/
void Node::setInRoute(){
    inRoute = true;
}

/******************************************************************************
* The Function Operation: return the if the node in the route.
******************************************************************************/
bool Node::isInRoute(){
    return inRoute;
}

/******************************************************************************
* The Function Operation: create a hash function with the size of the map.
******************************************************************************/
Bfs::Bfs(Node* maxNode){
    visited.resize(maxNode->hash());
}

/******************************************************************************
* The Function Operation: reset the hash table
******************************************************************************/
void Bfs::resetVisited(){
    unsigned long size = visited.size();
    visited.resize(0);
    visited.resize(size);
}

/******************************************************************************
* The Function Operation: get a graph and start node and end node
* and find the shortest way between them by using a node compertor.
* the algorithm find the node's nieghbors and get advanced until it
* get the end point, then it return a vector with the shortest way.
******************************************************************************/
vector <Node*>* Bfs::find_shortest_route(Graph* graph, Node* start, Node* end,
                                         NodeComparator* comparator){
    resetVisited();
    vector <Node*> garbage;
    Node* current = start;
    queue <Node*> nodes;
    queue <Node*>* adjacents = NULL;
    vector <Node*>* route = NULL;
    Node* temp = NULL;
    // push the start node.
    nodes.push(current);
    while (!nodes.empty()) {
        nodes.pop();
        // if find the end node
        if(comparator->equals(current,end)){
            // Create a route between the start to the end node.
            route = this->createRoute(start, current, comparator);
            // release all the allocated memory.
            while (!nodes.empty()){
                temp = nodes.front();
                delete temp;
                nodes.pop();
            }
        }
        else {
            // find the current node nieghbors.
            adjacents = graph->getAdjacent(current);
            while(!adjacents->empty()) {
                // check if visited already
                temp = adjacents->front();
                if (visited[temp->hash()] == 1) {
                    delete temp;
                    adjacents->pop();
                    continue;
                }
                // if not visited set, current == nighbor's father
                visited[temp->hash()] = 1;
                temp->setFather(current);
                nodes.push(temp);
                adjacents->pop();
            }
            delete adjacents;
            // save current in garbage for a future delete.
            garbage.push_back(current);
            current = nodes.front();
        }
    }
    //delete all the memory that kept.
    for(int i = 0; i < garbage.size(); i++){
        temp = garbage.at(i);
        if(!temp->isInRoute()) {
            delete temp;
        }
    }
    return route;
}

/******************************************************************************
* The Function Operation: get the start and end nodes and set all the
 * end node fathers until he get the start node (the first father).
******************************************************************************/
vector <Node*>* Bfs::createRoute(Node* start ,Node* end, NodeComparator* comparator){
    Node* temp = end;
    vector <Node*>* route = new vector<Node*>();
    while (!comparator->equals(temp, start)){
        // push all the end node's fathers to the vector.
        temp->setInRoute();
        route->push_back(temp);
        temp = temp->getFather();
    }
    // push the start node (the first father).
    start->setInRoute();
    route->push_back(start);
    reverse(route->begin(), route->end());
    return route;
}
