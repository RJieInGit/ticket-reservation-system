#include "admin.h"
#include <iostream>
#include <fstream>
#include <string>

Kernel *kernel;
sechdule :: sechdule(int depart){
    departTime=depart;
}

bool sechdule :: checkAndBook(int start, int destiny,int time, int num){
    return true;
} 
int sechdule :: getDepartTime(){
    return departTime;
}

admin:: admin(){
    printf("2\n");
    loadSechdule();
    printf("3\n");
    boardingMap= new map<string,list<Thread*>* >();
    getOffMap =new map<string,list<Thread*>* >();

}

void admin:: loadSechdule(){
    ifstream in("../file.txt");  
    if(!in) {
    cout << "Cannot open input file.\n";
    return ;
  }

  char str[255];

  while(in) {
    in.getline(str, 255);  // delim defaults to '\n'
    int time = atoi(str);
    if(in.eof())
        break;
    sechdules.push_back(new sechdule(time));
  }

  in.close();
}

void admin:: startSimulate(){
    currentTime=0;
    //timestamp in min start from 6am
    list<sechdule*> :: iterator nextTrain=sechdules.begin();
    while(currentTime<=960)  {
        if(currentTime%10==0){
            //create 5 requests every 10min
            createReservation();
        }
        //create train thread when according to the sechdules
        if(nextTrain== sechdules.end()){
            printf("no train left today,end of simulation \n");
            break;
        }
        if((*nextTrain)->getDepartTime()==currentTime){
          createTrain();
          nextTrain++;
        }
        
        currentTime++;
        kernel->currentThread->Yield();
    }
    printf("--------------------------------------------\n");
    printf("already passed 10pm, simulation ends\n");
    printf("total granted request: %d\n",totalgranted);
    printf("total discard request: %d\n", totaldiscard);
    printf("total passagers: %d\n", passagers);

}

void admin:: createReservation(){
    granted=0;
    discard=0;
    passagers=0;
  for (int i=0;i<5;i++){
       char name[200];
       name=sprintf(name,"%d",requestID);
       Thread *t = new Thread(name);
        t->Fork((VoidFunctionPtr) requestThread, (void *) &requestID);
        requestID++;
  }
  printf("###################################################\n");
  printf("at time %d hour, %d min\n",currentTime/60+6,currentTime%60);
  printf(" granted : %d requests and %d passagers\n ",totalgranted,passagers);
  printf(" discard : %d requests\n ", totaldiscard);

}

void admin:: requestThread(int *requestID){
     int start = rand()%19;         //can not start from last station
     int destiny = rand()%(19-start);   
     destiny+=start;
     int dTime = (rand()%72)*10;    //assume we find the most recent train for them
     int num = (rand()%8)+1;
     bool success=false;
     //find the next train for the request. if next train is not available, then discard.
     for(list<sechdule*> :: iterator it= this->sechdules.begin();it!=this->sechdules.end();++it){
         int time= (*it)->getDepartTime() + start*10;
         if(time>dTime&&time-dTime<30){
             if((*it)->checkAndBook(start,destiny,dTime,num))
             success=true;
         }
     }
     if(!success){
         discard++; totaldiscard++;
        return;
     }
     // log boarding
     char keyBoarding[50];
     sprintf (keyBoarding, "%d_%d",dTime,start);
     if(!boardingMap->count(keyBoarding))
        boardingMap->emplace(keyBoarding,new list<Thread*>());
     boardingMap->at(keyBoarding)->push_back(kernel->currentThread);

     //log get off
       char keyOff[50];
     sprintf (keyOff, "%d_%d",dTime+(destiny-start)*10,destiny);
     if(!getOffMap->count(keyOff))
        getOffMap->emplace(keyOff,new list<Thread*>());
     getOffMap->at(keyOff)->push_back(kernel->currentThread);

     requestPassager.emplace(requestID,num);
     //log static data
     granted++; totalgranted++; passagers+=num; totalpassager+=num;
     
      kernel->interrupt->SetLevel(IntOff);
      kernel->currentThread->Sleep(false);
      //boarding
      boardingreq++;
      boardingPassager+=num;

      kernel->interrupt->SetLevel(IntOff);
      kernel->currentThread->Sleep(false);
      //getoff
      getOffreq++;
      getOffPassager+=num;
      //thread ends
}
void admin :: createTrain(){
    char name[200];
    sprintf(name,"%d",trainID+1000);
     Thread *t = new Thread(name);
        t->Fork((VoidFunctionPtr) trainThread, (void *) &trainID);
        trainID++;
}

void admin::trainThread(int *trainID){
    int currentstation=-1;
    // assuming 10min to arrive at next station.
  while(currentstation<=20){
        currentstation++;
        boardingreq=0; boardingPassager=0; getOffreq=0; getOffPassager=0;
        if(currentTime%10==0){
        //boarding key and getoff key
        char keyBoarding[50];
        sprintf (keyBoarding, "%d_%d",currentTime,currentstation);
         char keyOff[50];
        sprintf (keyOff, "%d_%d",currentTime,currentstation);
        if(boardingMap->count(keyBoarding)){
            //wake up the request thread to boarding
            list<Thread*> *boarding = boardingMap->at(keyBoarding);
            for(list<Thread*> ::iterator it= boarding->begin(); it!=boarding->end();++it){
                kernel->interrupt->SetLevel(IntOff);
                kernel->scheduler->ReadyToRun(*it);
                kernel->interrupt->SetLevel(IntOn);
            }
            printf("$$$$$$$$$$$$$$$$ NOW BOARDING $$$$$$$$$$$$$$$$$$$$$$$\n");
            printf("at time %d hour, %d min! the %dth station, trainID: %d\n",currentTime/60+6,currentTime%60,currentstation,*trainID);
            printf("%d itinerary and %d passagers are boarding\n",boardingreq,boardingPassager);
        }
        if(getOffMap->count(keyOff)){
            //wake up the request thread to getoff
            list<Thread*> *getOff = getOffMap->at(keyBoarding);
            for(list<Thread*> ::iterator it= getOff->begin(); it!=getOff->end();++it){
                kernel->interrupt->SetLevel(IntOff);
                kernel->scheduler->ReadyToRun(*it);
                kernel->interrupt->SetLevel(IntOn);
            }
            printf("==================== NOW GETOFF ===========================\n");
            printf("at time %d hour, %d min! the %dth station\n",currentTime/60+6,currentTime%60,currentstation);
            printf("%d itinerary and %d passagers are boarding\n",getOffreq,getOffPassager);
        }
    }
    kernel->currentThread->Yield(); 
  }      

}