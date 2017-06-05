#ifndef ACTORNODE_HPP
#define ACTORNODE_HPP

#include <vector>
#include <utility>
#include <limits>
#include "Movie.hpp"
#include "ActorEdge.hpp"

class ActorNode{
public:
typedef std::pair<string,int>moviem;
    string name;//actors name
    vector<ActorEdge*> connections;//adjacency list of pointers to other actors
    vector<pair<string,int>> movies;//list of movies the actor has been in. TITLE,YEAR
    int dist; //distance from origin
    int id;//and id number for the actor. id num is easier than looking at strings
    int prev; //id number of previous node
    bool done;


    //constructor
    ActorNode(string actorName, string title, int year, int id_num){
        name=actorName;
        id=id_num;
        prev=-1;//no previous nodes
        movies.push_back(make_pair(title,year));//add this movie to the list
        done=0;//done is false
        dist=std::numeric_limits<int>::max(); //infinity
    }


    //destructor
    ~ActorNode(){
        ActorEdge* temp;
        while(connections.size()>0){
            temp=connections.back();
            delete temp;
            connections.pop_back();
        }
    }
    
    void reset(){
        prev=-1;
        dist=numeric_limits<int>::max();
        done=false;
    }


};
#endif 
