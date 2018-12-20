/* Demo_01_Main.cpp */

#include <boost/mpi.hpp>
#include "repast_hpc/RepastProcess.h"

#include "Demo_01_Model.h"


int main(int argc, char** argv){
	std::cout << "Running main ..." << endl;

	std::string configFile = argv[1]; // The name of the configuration file is Arg 1
	std::string propsFile  = argv[2]; // The name of the properties file is Arg 2

	boost::mpi::environment env(argc, argv); //some mpi stuff
	boost::mpi::communicator world;

	repast::RepastProcess::init(configFile); //intiate repast process

	RepastHPCDemoModel* model = new RepastHPCDemoModel(propsFile, argc, argv, &world); //create model (arena for repast agents)
	repast::ScheduleRunner& runner = repast::RepastProcess::instance()->getScheduleRunner(); //create scheduler so agents can take actions

	model->init(); //initialise model with agents
	model->initSchedule(runner); //start scheduler

	runner.run(); //Starts and runs the simulation schedule.

	model->recordResults();

//	model->getData();

	delete model;

	repast::RepastProcess::instance()->done(); //Notifes this RepastProcess that simulation has completed.



	std::cout << "Main ran" << endl;

}
