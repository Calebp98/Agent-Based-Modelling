/* Demo_01_Model.cpp */

#include <stdio.h>
#include <vector>
#include <boost/mpi.hpp>
#include "repast_hpc/AgentId.h"
#include "repast_hpc/RepastProcess.h"
#include "repast_hpc/Utilities.h"
#include "repast_hpc/Properties.h"
#include "repast_hpc/initialize_random.h"

#include "repast_hpc/SharedDiscreteSpace.h"//for network

#include <cmath>
#include <fstream>
#include <random>



#include "Demo_01_Model.h"
#include "policy.h"


RepastHPCDemoModel::RepastHPCDemoModel(std::string propsFile, int argc, char** argv, boost::mpi::communicator* comm): context(comm){
	props = new repast::Properties(propsFile, argc, argv, comm);
	stopAt = repast::strToInt(props->getProperty("stop.at"));
	countOfAgents = repast::strToInt(props->getProperty("count.of.agents"));
	initializeRandom(*props, comm);
	if(repast::RepastProcess::instance()->rank() == 0) props->writeToSVFile("./output/record.csv"); //record model params (agents, seed, iterations) at start of process


	//for network
	repast::Point<double> origin(-100,-100);
	repast::Point<double> extent(201, 201);

	repast::GridDimensions gd(origin, extent);

	std::vector<int> processDims;
	processDims.push_back(2);
	processDims.push_back(2);

	discreteSpace = new repast::SharedDiscreteSpace<RepastHPCDemoAgent, repast::WrapAroundBorders, repast::SimpleAdder<RepastHPCDemoAgent> >("AgentDiscreteSpace", gd, processDims, 2, comm);

	std::cout << "RANK " << repast::RepastProcess::instance()->rank() << " BOUNDS: " << discreteSpace->bounds().origin() << std::endl;

	context.addProjection(discreteSpace);
}


RepastHPCDemoModel::~RepastHPCDemoModel(){
		delete props;
}

void RepastHPCDemoModel::init(){
	policy* pol1 = new policy();
	pol1 -> start();
//	pol1 -> investment();
// pol1 -> paths();
//pol1 -> groupSchemes();
//pol1 -> electricBikes();



	int rank = repast::RepastProcess::instance()->rank();

	//create gaussian generator
	double 	mean = 0.1; //paramters subject to change
	double 	sigma = 20;
	//normalGen createNormalGenerator(mean,sigma);
	std::default_random_engine generator;
	std::normal_distribution<double> distribution(0.1,20);

	for(int i = 0; i < countOfAgents; i++){
		double x = abs(distribution(generator));
		double y = abs(distribution(generator));

		repast::Point<int> initialLocation((int)discreteSpace->dimensions().origin().getX() + x,(int)discreteSpace->dimensions().origin().getY() + y); //choose location for agent

		repast::AgentId id(i, rank, 0);
		id.currentRank(rank);

	  //temporary just to see if i could set things like this!!
		// int income = 30000;
		// int travelDistance = 5;

		double age = repast::Random::instance()->nextDouble() * 100; //assign agetns random (uniform i think) age between 0 and 100
		int income = repast::Random::instance()->nextDouble() * 500000;
		int travelDistance = repast::Random::instance()->nextDouble() * 10;
		char mode = 'n';

		bool carOwnership = false;
		bool bikeOwnership = false;

		if(repast::Random::instance()->nextDouble()>0.5){
			carOwnership = true;
		}else{
			carOwnership = false;
		}
		if(repast::Random::instance()->nextDouble() < pol1->getBikeProlif() ){
			bikeOwnership = true;
		}else{
			bikeOwnership = false;
		}


		double initMode = repast::Random::instance()->nextDouble();
    std::cout << "rand" <<initMode<< endl;
		if(initMode <= 0.0263){
			mode = 'b';
			// bikeOwnership = true;
		}else if(initMode > 0.0263 & initMode <=0.1318){
			mode = 'w';
		}else if(initMode > 0.1318 ){
			mode = 'c';
			// carOwnership = true;
		}

		// switch(int(floor(repast::Random::instance()->nextDouble()*3))){
		// 	case 0 :
		// 		mode = 'c';
		// 		break;
		// 	case 1 :
		// 		mode = 'b';
		// 		break;
		// 	case 2 :
		// 		mode = 'w';
		// 		break;
		// }

		std::map<char,int> influence;
	  influence['c']=0;
	  influence['b']=0;
	  influence['w']=0;
		// bool bikeOwnership = true;
		// bool carOwnership = true;


		RepastHPCDemoAgent* agent = new RepastHPCDemoAgent(id,age,mode,income,travelDistance,influence,carOwnership,bikeOwnership); //adds agents with type b for bike
		context.addAgent(agent);
		discreteSpace->moveTo(id, initialLocation); //move agent to the initial location

	}
}

void RepastHPCDemoModel::doSomething(){
	std::vector<RepastHPCDemoAgent*> agents;
	context.selectAgents(countOfAgents, agents);
	std::vector<RepastHPCDemoAgent*>::iterator it = agents.begin();

//	std::cout << "Rank " << repast::RepastProcess::instance()->rank() << " is doing something: " << repast::Random::instance()->nextDouble() << std::endl;

	while(it != agents.end()){ //for every agent
		std::cout << std::endl<<(*it)->getId() << " Age: " <<(*it)->getAge() << " Mode: " << (*it)->getMode()<< " Income: " << (*it)->getIncome()<<" Travel distance: " << (*it)->getTravelDistance() << endl; //print ID and mode

//agent decsion fuzzy logic
		if((*it)->decideToSearch()) {
			//(*it)->search();
			(*it)->decide((*it)->search()); //agent decision
		}

//for network
		//if(((*it) != 0) && ((*it)->getId().currentRank() == whichRank)){
		std::vector<int> agentLoc;
		discreteSpace->getLocation((*it)->getId(), agentLoc);
		repast::Point<int> agentLocation(agentLoc);
		std::cout << (*it)->getId() << " " << (*it)->getMode()<< " at " << agentLocation << std::endl;
		// }

		//choose agents factoring in agentLocation


		//calculate influence using bayesian updating

//from demo
		(*it)->play(&context,discreteSpace);
		it++;
    }

//		this->getData(agents);
		getData(agents);
		std::cout<< std::endl;

}

void RepastHPCDemoModel::initSchedule(repast::ScheduleRunner& runner){
	runner.scheduleEvent(1,repast::Schedule::FunctorPtr(new repast::MethodFunctor<RepastHPCDemoModel> (this, &RepastHPCDemoModel::clearData)));
	runner.scheduleEvent(2, 1, repast::Schedule::FunctorPtr(new repast::MethodFunctor<RepastHPCDemoModel> (this, &RepastHPCDemoModel::doSomething)));
	runner.scheduleEndEvent(repast::Schedule::FunctorPtr(new repast::MethodFunctor<RepastHPCDemoModel> (this, &RepastHPCDemoModel::recordResults)));
	runner.scheduleStop(stopAt);
}

void RepastHPCDemoModel::recordResults(){ //record simulation info (identifier and iterations) and status
	if(repast::RepastProcess::instance()->rank() == 0){
		props->putProperty("Result","passed");
		std::vector<std::string> keyOrder;
		keyOrder.push_back("RunNumber");
		keyOrder.push_back("stop.at");
		keyOrder.push_back("Result");
		props->writeToSVFile("./output/results.csv", keyOrder);
	}
}

void RepastHPCDemoModel::clearData(){
	std::ofstream myfile;
  myfile.open ("paths2.csv");
	myfile<<"bike"<<","<<"car"<<","<<"walk"<<","<<"search"<< ","<<"age"<<","<<"thoughtBike"<< ","<<"thoughtCar"<<","<<"thoughtWalk"<<std::endl;
	myfile.close();
}


void RepastHPCDemoModel::getData(std::vector<RepastHPCDemoAgent*> agents){
	std::vector<RepastHPCDemoAgent*>::iterator it = agents.begin();


	//returns individual iteration data
	int bike = 0;
	int car = 0;
	int walk = 0;
	int searchCount = 0;
	double avgAge = 0;
	int numOfAgents = 0;

	int thoughtBike = 0;
	int thoughtCar = 0;
	int thoughtWalk = 0;

	std::ofstream myfile;
	myfile.open ("paths2.csv",ofstream::app);

	while(it != agents.end()){
	//from demo
	if((*it)->getMode() == 'b'){
		bike ++;
	}else if((*it)->getMode() == 'c'){
		car ++;
	}else if((*it)->getMode() == 'w'){
		walk ++;
	}
	if((*it)->getLastSearch() == true){
		searchCount++;

		if((*it)->getNewMode() == 'c'){
			thoughtCar++;
		}else if((*it)->getNewMode() == 'b'){
				thoughtBike++;
		}else if((*it)->getNewMode() == 'w'){
				thoughtWalk++;
			}
	}



		avgAge += (*it)->getAge()  ;
		it++;
		numOfAgents++;

  }

	avgAge = avgAge/numOfAgents;

	myfile<<bike<<","<<car<<","<<walk<<","<<searchCount<<","<<avgAge<<","<<thoughtBike<<","<<thoughtCar<<","<<thoughtWalk<<std::endl;

	//myfile << "Data from run \n";
	myfile.close();
	std::cout<<"data was written" << std::endl;
}
