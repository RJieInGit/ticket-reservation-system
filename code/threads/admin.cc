#include "admin.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
sechdule :: sechdule(int depart){

}

bool sechdule :: checkAndBook(int start, int destiny,int time, int num){
    return true;
} 
admin:: admin(){
    nextTrain=0;
    loadSechdule();

}

void admin:: loadSechdule(){
    ifstream in("file.txt");
    if(!in) {
    cout << "Cannot open input file.\n";
    return ;
  }

  char str[255];

  while(in) {
    in.getline(str, 255);  // delim defaults to '\n'
    int time = atoi(str);
    printf("get time %d",time); 
  }

  in.close();
}