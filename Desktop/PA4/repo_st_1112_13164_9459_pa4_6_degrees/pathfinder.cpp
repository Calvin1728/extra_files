#include <limits>
#include <vector>
#include <string>
#include "ActorGraph.h"
#include <queue>
#include <iostream>
#include <queue>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include "ActorNode.hpp"
#include "ActorEdge.hpp"

//function prototypes



//vector with all actors and index of origin index 
void bfsPath(vector<ActorNode*> actors, int origin); 

//outstream and node you want to finish at
void shortestPath(vector<ActorNode*> actors, int endNode, ofstream& out);


//main goes here
int main(int argc, char*argv[]){

    if(argc!=5){
        cout<<"\n-----------------------------------------------------\n\n\n";
        cout<<"ERROR: This command takes exactly 4 arguments\n\n\n"<<endl;
        return -1;
    }

    string tsvFile=argv[1];
    string weightOrNot=argv[2];//right now this will always be u
    string actorPairs=argv[3];
    string outputFile=argv[4];

    int flag=-1;

    ActorGraph graph=ActorGraph();

    graph.loadFromFile(argv[1],false);
  
    graph.adjacencyVector();
    cout<<"done loading from file"<<endl;

    ifstream in(argv[3]);

    ofstream out(argv[4]);

/*
  if(strcmp(argv[2], "u")==0)
  {
    if(!graph.loadFromFile(argv[1],false))
    {
      cout<<"Could not load a file"<<endl;
      return 0;
    }
  }
*/

 if(!out.is_open())
  {
    cout<<"Could not open output file"<<endl;
    return 0;
  }

  if(!in.is_open())
  {
    cout<<"Could not open input file"<<endl;
    return 0;
  }

    out<<"(actor)==[movie#@year]-->(actor--..."<<endl;

    bool header=false;
    //remove

    while(in){

        string str;

        //uncomment this and delete below if statement
        if(!getline(in, str)) break;//skip header
        

        if(!header){
            header=true;
            continue;
        }


        istringstream ss(str);
        vector<string> compare;
        int origin=-1;
        int finish=-1;
        while(ss){
            string next;
            if(!getline(ss, next,'\t')) break;
            compare.push_back(next);
        }

        if(compare.size()!=2){           
            //cout<<"doing ok"<<endl;
            continue;
        }

        for(int i=0;i<graph.actors.size();i++){
            //cout<<"i is : "<<i<<endl;
            if(graph.actors[i]->name==compare[0]) origin=i;
            if(graph.actors[i]->name==compare[1]) finish=i;
        }

        if(origin==-1||finish==-1) {
            cout<<"ERROR"<<endl;
            cout<<"LINE 70 of pathfinder.cpp"<<endl;
            return -1;
        }



    cout<<"Computing path for: "<<compare[0]<<" (#"<<origin <<")"
<<" --> " << compare[1] <<" (#" << finish<<")" <<endl;

        //if you have any bugs, check if origin!=-1
        if(flag!=-1){
        cout<<"aksajdk"<<endl;
            for(int i=0; graph.actors.size();i++){
                graph.actors[i]->reset();}
            bfsPath(graph.actors,origin);
            flag=origin;
        }
        
        shortestPath(graph.actors,finish,out);
        cout<<"or here"<<endl;



    }




    return 0;
}


void bfsPath(vector<ActorNode*> actors, int origin){
    queue<ActorNode*> actorBFS;

    ActorNode* first=actors.at(origin);
    first->dist=0;
    actorBFS.push(first);
    while(actorBFS.size()>0){
        ActorNode* curr=actorBFS.front();
        actorBFS.pop();
        for(int i=0; i<curr->connections.size();i++){
            ActorNode* next=actors[curr->connections[i]->id];
            //if it is infinity(if it hasnt been traveled to before
            if (next->dist==numeric_limits<int>::max()){
                next->prev=curr->id;//setting what the prev node is
                next->dist=next->dist+1;
                actorBFS.push(curr);
            }
        }
    }
}

void shortestPath(vector<ActorNode*> actors, int endNode, ofstream& out){
    stack<int> revPath;
    ActorNode* curr=actors[endNode];
    while(curr->prev!=-1){
        revPath.push(curr->id);
        curr=actors[curr->prev];
    }
    if(revPath.size()!=0){
        ActorNode* firstN;
        ActorNode* secondN;
        firstN=actors[curr->id];
        while(revPath.size()>0){
    cout<<"hello1"<<endl;
            secondN=actors[revPath.top()];
            revPath.pop();
            bool flag=false;
            for(int i=0; i<firstN->movies.size();i++){
                if(flag) break;

                for(int j=0; j<secondN->movies.size();j++){
                    //if both the title and year match
                    cout<<"helllo"<<endl;
                    if(firstN->movies[i].first==secondN->movies[j].first &&
                            firstN->movies[i].second==secondN->movies[j].second){
                        //formatting and outputing
                        out<<"("<<firstN->name<<")--"<<"["<<firstN->movies[i].first
                            <<"#@"<<firstN->movies[i].second<<"]-->";
                        flag=true;
                        break;
                    }
                }
            }
            firstN=secondN;
        }
        out<<"("<<firstN->name<<")"<<endl;
    }
}





