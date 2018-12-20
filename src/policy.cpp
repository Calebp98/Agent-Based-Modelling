/*policy.cpp*/

#include "policy.h"

//policy::policy(int bikeAge,int carToBikeSal,int bikeDist,int walkAge,int carToWalkSal,int walkDist,int bikeToCarSal,int carDist,int walkToCarSal): bikeAge(bikeAge),carToBikeSal(carToBikeSal),bikeDist(bikeDist),walkAge(walkAge),carToWalkSal(carToWalkSal),walkDist(walkDist),bikeToCarSal(bikeToCarSal),carDist(carDist),walkToCarSal(walkToCarSal){}
//policy::policy(){}
policy::policy(int bikeAge_,int carToBikeSal_,int bikeDist_,int walkAge_,int carToWalkSal_,int walkDist_,int bikeToCarSal_,int carDist_,int walkToCarSal_): bikeAge(bikeAge_),carToBikeSal(carToBikeSal_),bikeDist(bikeDist_),walkAge(walkAge_),carToWalkSal(carToWalkSal_),walkDist(walkDist_),bikeToCarSal(bikeToCarSal_),carDist(carDist_),walkToCarSal(walkToCarSal_){};
policy::policy(){}

policy::~policy(){ }

void policy::start(){
  // bikeAge = 60; //changed from 80
  // carToBikeSal = 80000;
  // bikeDist = 10;
  //
  // walkAge = 60; //changed from 80
  // carToWalkSal = 30000;
  // walkDist = 3;
  //
  // bikeToCarSal = 25000;
  // carDist = 5;
  //
  // walkToCarSal = 25000;
  // bikeProlif = 0.4;
  //
  // community = 1;

  bikeAge = 60; //changed from 80
  carToBikeSal = 80000;
  bikeDist = 5;

  walkAge = 60; //changed from 80
  carToWalkSal = 30000;
  walkDist = 3;

  bikeToCarSal = 25000;
  carDist = 10;

  walkToCarSal = 25000;
  bikeProlif = 0.4;

  community = 1;

//modifications
walkAge = 80;
bikeAge = 80;

bikeDist = 15;
walkDist = 5;

}

void policy::investment(){
  //increases the number of agents that start with bikes
  //increase carToBikeSal
  this -> start();
  bikeProlif = 0.8;
}

void policy::paths(){
  //increase walkAge
  //increase bikeAge
  //increase bikeDist
  //increase walkDist
  walkAge = 80;
  bikeAge = 80;

  bikeDist = 15;
  walkDist = 5;
}

void policy::groupSchemes(){
  //network influence (maybe can see more agents) bias towards active group activity
  //agents walking and cycling see more people like themselves
  community = 100;

}

void policy::electricBikes(){
  //bike dist increases significantly
    bikeDist = 30;
}


//getters
int policy:: getBikeAge(){
  return bikeAge;
}
int policy:: getCarToBikeSal(){
  return carToBikeSal;
}
int policy:: getBikeDist(){
  return bikeDist;
}

int policy:: getWalkAge(){
  return walkAge;
}
int policy:: getCarToWalkSal(){
  return carToWalkSal;
}
int policy:: getWalkDist(){
  return walkDist;
}

int policy:: getBikeToCarSal(){
  return bikeToCarSal;
}
int policy:: getCarDist(){
  return carDist;
}

int policy::getWalkToCarSal(){
  return walkToCarSal;
}

float policy::getBikeProlif(){
  return bikeProlif;
}


float policy::getCommunity(){
  return community;
}
