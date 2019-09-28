/*
this package manage the dynamic information that a reservation system and train simulation system might need.
It is also responsible for the allocation of the thread and runs the simulation system
*/

#include <stdlib.h>
#include <list>
#include "../lib/bitmap.h"
#include <map>
using namespace std;

class sechdule {
    public:
        sechdule(int departTime);           // each stop takes 10min, so we only need depart time to construct 
        bool checkAndBook(int start, int destiny,int time, int num);                 // return false if refuse           

    protected:
        map<int,int>  route;      //pair<station, arrivetime> stations are 0-19
        list <Bitmap> business;            // represent the availablilty of each seat at each stop
        list <Bitmap> coash;            
};

class admin {
    public :  
        admin();        //load sechdules in constructor 
        int now();      //return current time
        void startSimulate();

    protected : 
        int currentTime; // timestamp in minutes fromt 6am. readonly to other thread
        list<sechdule*> sechdules; //should be in order of departure time from station 0
        int nextTrain;                     // range from 0 - sechdules.size()    
        void loadSechdule();     //called by the constructor
};

