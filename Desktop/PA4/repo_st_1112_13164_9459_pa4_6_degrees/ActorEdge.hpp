#ifndef ACTOREDGE_HPP
#define ACTOREDGE_HPP

//i don't think i need any #include


using namespace std;
class ActorEdge{
public:
    int id; //id of actor edge is pointing to...?
    int weight;//I don't think i'll need this for checkpoint
    
    //constructors
    
    //when I don't need the weight
    ActorEdge(int num){
        id=num;
        weight=1;
    }
    
    
    ActorEdge(int num, int wght){
        id=num;
        weight=wght;
    }

};
#endif
