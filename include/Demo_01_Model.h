/* Demo_01_Model.h */
//world

#ifndef DEMO_01_MODEL
#define DEMO_01_MODEL

#include <boost/mpi.hpp>
#include "repast_hpc/Schedule.h"
#include "repast_hpc/Properties.h"
#include "repast_hpc/SharedContext.h"

//for the network
#include "repast_hpc/SharedDiscreteSpace.h"
#include "repast_hpc/GridComponents.h"
#include "repast_hpc/Point.h" //additional for spatial projection


#include "Demo_01_Agent.h"

class RepastHPCDemoModel{
	int stopAt;
	int countOfAgents;
	repast::Properties* props;
	repast::SharedContext<RepastHPCDemoAgent> context;

	repast::SharedDiscreteSpace<RepastHPCDemoAgent, repast::WrapAroundBorders, repast::SimpleAdder<RepastHPCDemoAgent> >* discreteSpace; //for discrete space network

public:
	RepastHPCDemoModel(std::string propsFile, int argc, char** argv, boost::mpi::communicator* comm);
	~RepastHPCDemoModel();
	void init();
	void doSomething();
	void initSchedule(repast::ScheduleRunner& runner);
	void recordResults();
	void clearData(); //clears data in iterative output file
	void getData(std::vector<RepastHPCDemoAgent*> agents); //collects individual simulation data

};

#endif
