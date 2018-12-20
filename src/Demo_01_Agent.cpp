/* Demo_01_Agent.cpp */


#include <iostream>

#include "Demo_01_Agent.h"
#include "Fuzzy.h" //no longer use

//for spatial network interactions
#include "repast_hpc/Moore2DGridQuery.h"
#include "repast_hpc/Point.h"
#include "repast_hpc/SharedDiscreteSpace.h"//for network

#include <random>

#include "policy.h"



//constructors
RepastHPCDemoAgent::RepastHPCDemoAgent(repast::AgentId id): id_(id), c(100), total(200){ }

RepastHPCDemoAgent::RepastHPCDemoAgent(repast::AgentId id, double age, char mode, int income, int travelDistance, std::map<char,int> influence, bool carOwnership, bool bikeOwnership): id_(id), c(100), total(200), mode(mode), age(age), income(income), travelDistance(travelDistance),newMode(mode),influence(influence),carOwnership(carOwnership),bikeOwnership(bikeOwnership){ } //added to set mode

RepastHPCDemoAgent::RepastHPCDemoAgent(repast::AgentId id, double newC, double newTotal): id_(id), c(newC), total(newTotal){ }

RepastHPCDemoAgent::~RepastHPCDemoAgent(){ } //destructor


void RepastHPCDemoAgent::set(int currentRank, double newC, double newTotal){ //set RepastHPCDemoAgent variables
    id_.currentRank(currentRank); //sets the current rank for the agent
    c     = newC; //set c attribute to newC parameter
    total = newTotal; //set total attribute to newTotal
}

bool RepastHPCDemoAgent::cooperate(){
	return repast::Random::instance()->nextDouble() < c/total; //if the random number is less than c/total cooperate
}

void RepastHPCDemoAgent::play(repast::SharedContext<RepastHPCDemoAgent>* context, repast::SharedDiscreteSpace<RepastHPCDemoAgent, repast::WrapAroundBorders, repast::SimpleAdder<RepastHPCDemoAgent> >* space){ //originally used to play prisoners dilemma game
    //setup for interactions with other agents goes here

    // Fuzzy* fuzzyEngine = new Fuzzy();
    //
    // fuzzyEngine->search();

    policy* pol1 = new policy();
    pol1 -> start();
  //  pol1 -> investment();
     // pol1 -> paths();
//     pol1 -> groupSchemes();
    // pol1 -> electricBikes();

    std::set<RepastHPCDemoAgent*> agentsToPlay; //pointers to playing agents


    agentsToPlay.insert(this); // Prohibit playing against self

    //create gaussian generator
    double 	mean = 0.1; //paramters subject to change
    double 	sigma = 20;
    //normalGen createNormalGenerator(mean,sigma);
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.1,20);



    //calculate distance between ego and all other agents
    std::vector<int> agentLoc1;
    std::vector<int> agentLoc2;

//    space->getLocation((this)->getId(), agentLoc);
    repast::Point<int> agentLocation1(agentLoc1);
    repast::Point<int> agentLocation2(agentLoc2);


    std::vector<RepastHPCDemoAgent*> allAgents;
    //currently can;t remove agent playing against it self (or at least calcualting distance)
  //  allAgents.addAgent(this); // Prohibit playing against self
//    context->addAgent(this );// Prohibit playing against self

    // allAgents.insert(this,0);
    context->selectAgents(allAgents,true);



  	std::vector<RepastHPCDemoAgent*>::iterator agentDistToCalc = allAgents.begin();
    double distanceBetweenAgents =0;
    double networkWeakness = 0;
    float threshold = 0.5;

    while(agentDistToCalc != allAgents.end()){
  //    std::cout << space->getLocation((*agentDistToCalc)->getId(), agentLoc)) <<endl;
      double randomNumber = abs(distribution(generator));

      space->getLocation((*agentDistToCalc)->getId(), agentLoc1);
      space->getLocation((this)->getId(), agentLoc2);
      // std::cout << agentLoc1[0] << agentLoc1[1]<< " vs "<<agentLoc2[0] << agentLoc2[1]<< endl;
      // std::cout << agentLoc1[0]-agentLoc2[0] << endl;

//      std::cout << pow((agentLoc1[0]-agentLoc2[0]),2) + pow((agentLoc1[1]-agentLoc2[1]),2)<< endl;

      distanceBetweenAgents = sqrt( pow((agentLoc1[0]-agentLoc2[0]),2) + pow((agentLoc1[1]-agentLoc2[1]),2));
//      std::cout<< endl  << "Distance between this agent and agent: " << (*agentDistToCalc)->getId() << " is " << distanceBetweenAgents;

      networkWeakness = distanceBetweenAgents * randomNumber/10;
//      std::cout << " and the influence is " <<networkWeakness<< std::endl;

      if((*agentDistToCalc)->getMode() == 'c' & this->getMode() == 'c'){
        networkWeakness = networkWeakness / pol1 -> getCommunity();
      }

      if((*agentDistToCalc)->getMode() == 'w' & this->getMode() == 'w'){
        networkWeakness = networkWeakness / pol1 -> getCommunity();
      }


      if ((networkWeakness < threshold) & (networkWeakness != 0)){
        cout <<"networkWeakness" <<networkWeakness << "threshold"<< threshold<< endl;
        agentsToPlay.insert(*agentDistToCalc);
//        std::cout << "Agent was added to the context" << std::endl;
      }
      agentDistToCalc++;
    }


    //multiply randomly generated double by each distance, if above threshold let agent play and to agentsToPlay
//    context->selectAgents(3, agentsToPlay, true); //Gets a set of pointers to 3 randomly selected agents

//used for prisoners dilemma game
    double cPayoff     = 0; //cooperation pay off
    double totalPayoff = 0; //total pay off

//


    std::set<RepastHPCDemoAgent*>::iterator agentToPlay = agentsToPlay.begin();
    while(agentToPlay != agentsToPlay.end()){
      //prisoners dilemma game
        // bool iCooperated = cooperate();                          // Do I cooperate?
        // double payoff = (iCooperated ?
				// 		 ((*agentToPlay)->cooperate() ?  7 : 1) :     // If I cooperated, did my opponent?
				// 		 ((*agentToPlay)->cooperate() ? 10 : 3));     // If I didn't cooperate, did my opponent?
        // if(iCooperated) cPayoff += payoff;
        // totalPayoff             += payoff;

      //collect influence from the network

//        std::cout<< endl <<"Agent interacted with " <<(*agentToPlay)->getId()<<std::endl;
        if((*agentToPlay)->getMode() == 'b'){
          influence['b'] += 1;
        }else if((*agentToPlay)->getMode() == 'c'){
          influence['c'] += 1;
        }else if((*agentToPlay)->getMode() == 'w'){
          influence['w'] += 1;
        }
//        std::cout<<"Agent updated influences" << std::endl<< std::endl;



        agentToPlay++;
    }
    // c      += cPayoff;
    // total  += totalPayoff;

}

bool RepastHPCDemoAgent::decideToSearch(){
  age += 0.02; //agent ages by aprox. 1 week each run
  newMode = 'n'; //reset newMode
  std::cout<<"Agent Deciding whether it is worth Searching..." << std::endl;
  double weight = double(influence[mode])/double((influence['c']+influence['w']+influence['b']+1));
  double threshold = repast::Random::instance()->nextDouble();
  if(weight < 1.5*repast::Random::instance()->nextDouble())
//  if(true)
  {
    std::cout<<"Agent Decided to Search" << std::endl;
    lastSearch = true;
    return true;  //for now agent always decides to search
  }else{
    std::cout<<"Agent Decided not to Search" << std::endl;
    // std::cout << "random"<< threshold <<" "<< influence[mode]<< " "<< (influence['c']+influence['w']+influence['b']+1)<< " "<<weight  <<std::endl;
    lastSearch = false;
    return false;  //for now agent always decides to search
  }
}

char RepastHPCDemoAgent::search(){

  // policy* pol1 = new policy(50,50000,5,80,30000,3,25000,5,25000);
  //
   // policy* pol1 = new policy(80,80000,10,80,30000,3,25000,5,25000);
   // std::cout<<"checking policies match " << pol1->getBikeAge()<< std::endl;
   policy* pol1 = new policy();
   pol1 -> start();
  // pol1 -> investment();
    // pol1 -> paths();
//    pol1 -> electricBikes();

   // std::cout << pol2->getBikeAge() << std::endl;



  //search variables
  int bikeAge = pol1->getBikeAge();
  int carToBikeSal = pol1->getCarToBikeSal();
  int bikeDist = pol1->getBikeDist();

  int walkAge = pol1->getWalkAge();
  int carToWalkSal = pol1->getCarToWalkSal();
  int walkDist = pol1->getWalkDist();

  int bikeToCarSal = pol1->getBikeToCarSal();
  int carDist = pol1->getCarDist();

  int walkToCarSal = pol1->getWalkToCarSal();


  //fuzzy logic conversion
//   char fAge = 'n';
// //  char
//
//   //age
//   if(age < 25){
//     fAge = 'y';//age is young
//   }else if(age >= 25 & age < 80){
//     fAge = 'm';
//   }else if(age > 80){
//     fAge = 'o'
//   }


  // rules very arbitrary, should be informed by literature
  std::cout << "Agent Searching ..."<<std::endl;

//  if(repast::Random::instance()->nextDouble()<0.02){
    if(mode == 'c'){
      std::cout<< "Current mode is car, searching for new mode ..." << std::endl;
      if(age<bikeAge & income < carToBikeSal & travelDistance < bikeDist & bikeOwnership){ //switching to bike
        newMode = 'b';
        std::cout<<"Thinking about switching to bike" <<std::endl;
      }else if(age<walkAge & income < carToWalkSal & travelDistance<walkDist){ //switching to walk
        newMode = 'w';
        std::cout<<"Thinking about switching to walking" <<std::endl;
      // }else if(age>0 & income > 0 & travelDistance>0){ //fake case to catch all (debug)
      //   newMode = 'w';
      //   std::cout<<"Thinking about switching to walking" <<std::endl;
      }else{
        std::cout <<"Keep current mode" <<std::endl;
        newMode = mode;
      }

    }else if(mode == 'b' ){
      std::cout<< "Current mode is bike, searching for new mode ..." << std::endl;

      if(carOwnership & income > bikeToCarSal & travelDistance > carDist){ //switching to car
        newMode = 'c';
        std::cout<<"Thinking about switching to car" <<std::endl;
      }else if(age<walkAge & travelDistance<walkDist){ //swithing to walk
        newMode = 'w';
        std::cout<<"Thinking about switching to walking" <<std::endl;
      }else{
        std::cout <<"Keep current mode" <<std::endl;
        newMode = mode;
      }

    }else if(mode == 'w'){
      std::cout<< "Current mode is walk, searching for new mode ..." << std::endl;

      if(age<bikeAge & bikeOwnership & travelDistance>bikeDist){
        newMode = 'b';
        std::cout<<"Thinking about switching to car" <<std::endl;
      }else if(carOwnership & income > walkToCarSal & travelDistance>carDist){
      newMode = 'c';
        std::cout<<"Thinking about switching to bike " <<std::endl;
      }else{
        std::cout <<"Keep current mode" <<std::endl;
        newMode = mode;
      }
    }else{
      std::cout<< "Current mode is unknown!!" << std::endl;
    }

    if(newMode!=mode){
      std::cout<<"Agent Completed Search and thought about switching to " << newMode<<  endl;
    }else{
    std::cout << "Agent Completed Search"<<std::endl;
  }
    return newMode;
//}
}

void RepastHPCDemoAgent::decide(char newMode){
  policy* pol1 = new policy();
  pol1 -> start();
//   pol1 -> investment();
   // pol1 -> paths();
   // pol1 -> groupSchemes();
//      pol1 -> electricBikes();

  double networkPersuasion = 0;
  double totalInfluence = 0;

  std::cout << "Agent Deciding ..."<<std::endl;
  //paramters are arbitrary for now, will be modified later
  //decision may take into account network information
  if(repast::Random::instance()->nextDouble()<0.1){
  if(mode != newMode){
    std::cout << "agent deciding between switching to " << newMode << " or staying on " << mode << endl;
    std::cout << "Influence from network :" <<" Bike: " <<influence['b'] <<" Car: "  <<influence['c'] <<" Walk: " << influence['w'] << std::endl;
    totalInfluence = (influence['c'] + influence['b'] + influence['w']) +1;

    networkPersuasion = influence[newMode]/totalInfluence;

    std::cout << "newMode network support: "<< networkPersuasion <<std::endl;

    if(newMode == 'b' & networkPersuasion > (0.02 / pol1 -> getCommunity())){
      mode = newMode;
      std::cout << "mode was upated" << std::endl;
    } else if(newMode == 'c' & networkPersuasion > 0.2){
      mode = newMode;
      std::cout << "mode was upated" << std::endl;
    }else if(newMode == 'w' & networkPersuasion > (0.01 / pol1 -> getCommunity())){
      mode = newMode;
      std::cout << "mode was upated" << std::endl;
    }


    std::cout << "Agent completed decision, mode is now: "<< mode <<std::endl;

  }
  else {
//    std::cout <<"test"<< mode << newMode<< endl;
    std::cout<< "Agent decided to stick with " << mode << endl;
  }
}
}

// void RepastHPCDemoAgent::getData(){
//
// }
