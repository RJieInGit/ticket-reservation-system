/*
this package manage the dynamic information that a reservation system and train simulation system might need.
It is also responsible for the allocation of the thread and runs the simulation system
*/

#include <list>
#include <map>
using namespace std;
#include "thread.h"
#include "../lib/bitmap.h"


//typo of train schedule,LOL
class sechdule {
    public:
        sechdule(int departTime);           // each stop takes 10min, so we only need depart time to construct 
        bool checkAndBook(int start, int destiny,int time, int num);                 // return false if refuse           
        int getDepartTime();
    protected:
        map<int,int>  route;      //pair<station, arrivetime> stations are 0-19
        list <Bitmap> business;            // represent the availablilty of each seat at each stop
        list <Bitmap> coash;    
        int departTime;        
};

class admin {
    public :  
        admin();        //load sechdules in constructor 
        int now();      //return current time
        void startSimulate();
        void createReservation(); //create new request thread 
        void createTrain();       // create train thread
        void requestThread();     // fork and called by the above
        void trainThread();       // fork and called by the above
        int getRequestID();                  //return the next requestID
        int getTrainID();                    //return next trainID

    protected : 
        int currentTime; // timestamp in minutes fromt 6am. readonly to other thread
        list<sechdule*> sechdules; //should be in order of departure time from station 0  
        void loadSechdule();     //called by the constructor
        int requestID;            //requestIDs are ints start from 0. they are unique
        int trainID;              // trainIDs are ints start from 0. they are unique
        map<string,list<Thread*>* >  *boardingMap; //k: time_station v:requestID
        map<string,list<Thread*>* >  *getOffMap;   //k: time_station v: requestID
        map<int,int> *requestPassager;        // k: requestID, v: num of passagers in this request

        //below are the static variables
        int discard;                         // num of the discarded request
        int granted;                         // num of the granted request
        int passagers;                       // num of passagers
        int totaldiscard;
        int totalgranted;
        int totalpassager;
        int boardingreq;
        int getOffreq;
        int boardingPassager;
        int getOffPassager;
};  

