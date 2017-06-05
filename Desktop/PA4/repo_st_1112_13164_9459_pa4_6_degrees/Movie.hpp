#ifndef MOVIE_HPP
#define MOVIE_HPP


#include <vector>

using namespace std;

class Movie{

public:
    vector<int> cast;//indexes of all the actors in the movie
    
    Movie(){}

    void addActor(int actor){
        cast.push_back(actor);
    }

};

#endif
