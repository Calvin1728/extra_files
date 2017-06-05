/*
 * ActorGraph.cpp
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "ActorGraph.h"
#include "Movie.hpp"
#include "ActorNode.hpp"
#include "ActorEdge.hpp"

using namespace std;

ActorGraph::ActorGraph(void) {}

//destructor?


void ActorGraph::adjacencyVector(){
    int num_edge=0;
    ActorEdge* edge;
    map<pair<string, int>,Movie*>:: const_iterator done;
    for(int i = 0;i<actors.size();i++){
        for(int j = 0;j<actors[i]->movies.size();j++){
            done=movies.find(actors[i]->movies[j]);
            if(done!=movies.end())
                for(int k = 0;k<done->second->cast.size();k++){
                    if(done->second->cast[k]!=i){
                        edge=new ActorEdge(done->second->cast[k]);
                        actors[i]->connections.push_back(edge);
                        num_edge++;
                    }
                }
        }
    }
    cout<<"number of edges  "<<num_edge<<endl;
}

bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {

    string currActor;
    Movie* currMovie;
    int index;

    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;

    // keep reading lines until the end of file is reached
    while (infile) {
        string s;

        // get the next line
        if (!getline( infile, s )) break;

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;

            // get the next string before hitting a tab character and put it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }

        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);

        // we have an actor/movie relationship, now what?
        // How do we deal with having the same actor name?

        //setting index of actors
        //
        //if the next line repeats the actor
        if (actor_name==currActor) {
            index=actors.back()->id;
        }

        //if the next line has a new actor
        else index=actors.size(); 

        //movies

        //correct usage of auto?
        auto finding=movies.find(make_pair(movie_title,movie_year));

        if(finding==movies.end()){ //this means its a new movie
            Movie* insertingMovie=new Movie();
            (*insertingMovie).addActor(index);
            //adding the movie to the map w/ all the movies in it
            movies.insert(make_pair(make_pair(movie_title,movie_year)
                        ,insertingMovie));
        }

        //if its the same movie title, but not same year
        else if(finding->first.second!=movie_year)
        {
            Movie* insertingMovie=new Movie();
            (*insertingMovie).addActor(index);
            //adding the movie to the map w/ all the movies in it
            movies.insert(make_pair(make_pair(movie_title,movie_year),insertingMovie));
        }
        
        else{
            (*finding->second).addActor(index);//(*second).addActor(index)...?
        }

        //actors
        
        //if its the same actor
        if(actor_name==currActor){
            auto tempMov=make_pair(movie_title,movie_year);
            actors.back()->movies.push_back(tempMov);
        }

        else if(actor_name!=currActor){//if its a different actor
            ActorNode* insertActor=new ActorNode(actor_name,movie_title,movie_year,actors.size());
            actors.push_back(insertActor);
            currActor=actor_name;
        }

    }
    
    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();
    
    cout<<"number of nodes:  "<<actors.size()<<endl;
    cout<<"number of movies:  "<< movies.size() <<endl;
    
    return true;
}
