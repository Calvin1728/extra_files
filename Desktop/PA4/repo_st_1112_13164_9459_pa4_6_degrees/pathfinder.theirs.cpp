#include <iostream>
#include <limits>
#include <vector>
#include <queue>
#include "ActorGraph.h"
#include "ActorNode.hpp"
#include "ActorEdge.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stack>
#define numberofargu 5
typedef pair<int,ActorNode*> p;
using namespace std;
//function protopyte 
void BFSTraverse(vector<ActorNode*> theGraph, int from);
void 
WeightedShortestpath(vector<ActorNode*> theGraph, int dest, ofstream& out);
void 
UnweightedShortestpath(vector<ActorNode*> theGraph, int dest, ofstream& out);
void DijkstraTraverse(vector<ActorNode*> theGraph, int from);
/*-------------------------------------------------------------------------- 
Function Name: main
Description:   This main func will run the file and output the shortest path
Input:         argc : number of arugment, argv[] : arg vector from input
return -       return 0 for success. 
--------------------------------------------------------------------------*/
int
main( int argc, char *argv[] ){
  int check =-1;
  // check if have enough argumen
  if(argc!=5)
  {
         cout<<"\n-----------------------------------------------------\n\n\n";
        cout<<"ERROR: This command takes exactly 4 arguments\n\n\n"<<endl;
        return -1;  }
  ActorGraph Agraph = ActorGraph();
  if(strcmp(argv[2], "u")==0)
  {
    if(!Agraph.loadFromFile(argv[1],false))
    {
      cout<<"Could not load a file"<<endl;
      return 0;
    }
  }
  if(strcmp(argv[2], "w")==0)
  {
    if(!Agraph.loadFromFile(argv[1],true))
    {
      cout<<"Could not load a file"<<endl;
      return 0;
    }
  }

  Agraph.adjacencyVector(); // make a adjency list and graph

  ofstream ofile (argv[4]);
  if(!ofile.is_open())
  {
    cout<<"Could not open output file"<<endl;
    return 0;
  }

  ifstream ifile (argv[3]);
  if(!ifile.is_open())
  {
    cout<<"Could not open input file"<<endl;
    return 0;
  }
  ofile<<"(actor)--[movie#@year]-->(actor)--..."<<endl;
  // get the actors pair and print out the path
  bool have_header = false;
  while(ifile)
  {
    string s;
    // get the next line
    if (!getline( ifile, s )) 
      break;
    if (!have_header)
    {
      // skip the header
      have_header = true;
      continue;
    }
    istringstream ss( s );
    vector <string> actorcomp;
    int from =-1, dest=-1;
    while (ss)
    {
      string next;
      // get the next string
      if (!getline( ss, next, '\t' )) break;
      actorcomp.push_back( next );
    }
    if (actorcomp.size() != 2)
      continue;// have exactly 2 columns

    for(int i=0;i<Agraph.actors.size();i++)
    {
      if(Agraph.actors[i]->name==actorcomp[0])
        from = i;
      if(Agraph.actors[i]->name==actorcomp[1])
        dest= i;
    }
    // check if there is the actor in the graph
    if(from<0||dest<0)
    {
      if(from>0)
        cout<<"There's no actor "<<actorcomp[1] <<endl;
      else
        cout<<"There's no actor "<<actorcomp[0] <<endl;
      return 0;
    }

    cout<<"Computing path for: "<<actorcomp[0]<<" (#"<<from <<")"
        <<" --> " << actorcomp[1] <<" (#" << dest<<")" <<endl;
    if(strcmp(argv[2], "u")==0) // unweighed path
    {
      if(check!=from)
      {
        for(int i= 0;i<Agraph.actors.size();i++)
          Agraph.actors[i]->reset();
        BFSTraverse(Agraph.actors,from);
        check = from;
      }
      UnweightedShortestpath(Agraph.actors,dest, ofile);
    }
/*    if(strcmp(argv[2], "w")==0) // weighted path
    {
      if(check!=from)
      {
        for(int i= 0;i<Agraph.actors.size();i++)
          Agraph.actors[i]->reset();
        DijkstraTraverse(Agraph.actors,from);
        check =from;
      }
      WeightedShortestpath(Agraph.actors,dest, ofile);
    }
*/
  }
  return 0;
}

/** Traverse the graph using BFS */
/*-------------------------------------------------------------------------- 
Function Name: DijkstraTraverse
Description:   traverse the graph from the input actor and find all possible
             weighted path to other actors
--------------------------------------------------------------------------*/
void BFSTraverse(vector<ActorNode*> theGraph, int from) {
  queue<ActorNode*> toExplore;  
  ActorNode* start = theGraph[from];
  start->dist = 0;
  toExplore.push(start);
  while (!toExplore.empty()) {
    ActorNode* next = toExplore.front();
    toExplore.pop();
    for (int i = 0; i< next->connections.size(); i++) {
      ActorNode* neighbor = theGraph[next->connections[i]->id];
      if (neighbor->dist == numeric_limits<int>::max()) {
        neighbor->dist = next->dist + 1;
        neighbor->prev = next->id;
        toExplore.push(neighbor);
      }
    }
  }
}

/** Traverse the graph using dijkstra algorithm */
/*-------------------------------------------------------------------------- 
Function Name: DijkstraTraverse
Description:   traverse the graph from the input actor and find all possible
             weighted path to other actors
--------------------------------------------------------------------------*/
/*void DijkstraTraverse(vector<ActorNode*> theGraph, int from) {
  priority_queue<p,vector<p>,greater<p>>toExplore;
  int cost;
  ActorNode* start = theGraph[from];
  start->dist = 0;
  toExplore.push(make_pair(0,start));
  while (!toExplore.empty()) {
    ActorNode* next = toExplore.top().second;
    toExplore.pop();
    if(next->done)
      continue;
    else
      next->done= true;
    for (int i = 0; i< next->connections.size(); i++) {
     ActorNode* neighbor = theGraph[next->connections[i]->id];
     cost = next->dist + next->connections[i]->cost;
     if(neighbor->dist>cost)
     {
      neighbor->dist = cost;
      neighbor->prev = next->id;
    }
    toExplore.push(make_pair(cost,neighbor));
  }
 }
}
*/
/*-------------------------------------------------------------------------- 
Function Name: UnweightedShortestpath
Description:   calculate the path between two actors and output shortest
            path between 2 actors
--------------------------------------------------------------------------*/
void 
UnweightedShortestpath(vector<ActorNode*> theGraph, int dest, ofstream& out){
  stack<int>reverse;
  ActorNode* working = theGraph[dest];
  // store id of actor path
  while(working->prev!=-1)
  {
    reverse.push(working->id);
    working = theGraph[working->prev];
  }
  // print out the path
  if(!reverse.empty())
  {
    ActorNode* tmp1;
    ActorNode* tmp2;
    tmp1=theGraph[working->id];
    while(!reverse.empty())
    {
      tmp2=theGraph[reverse.top()];
      reverse.pop();
      bool check=false;
      for(int i = 0; i<tmp1->movies.size();i++)
      {
        if(check)
          break;
        // check and output the movie path to the actor
        for(int j = 0; j<tmp2->movies.size();j++)
        {
          if(tmp1->movies[i].first==tmp2->movies[j].first&&
              tmp1->movies[i].second==tmp2->movies[j].second)
          {
            out<<"("<<tmp1->name<<")--"<<"["<<tmp1->movies[i].first
                <<"#@"<<tmp1->movies[i].second<<"]-->";
            check =true;
            break;
          }
        }
      }
      tmp1 = tmp2;
    }
    out<<"("<<tmp1->name<<")";
    out<<endl;
  }
}
/*-------------------------------------------------------------------------- 
Function Name: WeightedShortestpath
Description:   calculate the path between two actors and output shortest
            path between 2 actors
--------------------------------------------------------------------------*/
void 
WeightedShortestpath(vector<ActorNode*> theGraph, int dest, ofstream& out){
  stack<int>reverse;
  ActorNode* working = theGraph[dest];
  // store id of actor path
  while(working->prev!=-1)
  {
    reverse.push(working->id);
    working = theGraph[working->prev];
  }
  // print out the path
  if(!reverse.empty())
  {
    ActorNode* tmp1;
    ActorNode* tmp2;
    tmp1=theGraph[working->id];
    while(!reverse.empty())
    {
      tmp2=theGraph[reverse.top()];
      reverse.pop();
      bool check=false;
      // check to find correct path of movie
      for(int i = 0; i<tmp1->movies.size();i++)
      {
        if(check)
          break;
        for(int j = 0; j<tmp2->movies.size();j++)
        {
          if(tmp1->movies[i].first==tmp2->movies[j].first&&
              tmp1->movies[i].second==tmp2->movies[j].second)
          {
            int path = 1 + (2015-tmp1->movies[i].second) + tmp1->dist; 
            if(path == tmp2->dist)
            {
              // output the file
              out<<"("<<tmp1->name<<")--"<<"["<<tmp1->movies[i].first
                  <<"#@"<<tmp1->movies[i].second<<"]-->";
              check =true;
              break;
            } 
          }
        }
      }
      tmp1 = tmp2;
    }
    out<<"("<<tmp1->name<<")";
    out<<endl;
  }
}
