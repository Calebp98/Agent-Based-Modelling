#************************************************************************************************************
#
# Repast HPC Makefile for assignment
#
#************************************************************************************************************

include ./env

Processes = 1


# Right, now for some recipes.PHONY
.PHONY : init all
all: bin/executable

.PHONY : build
build: # This is where all the crazy linking occurs, see other makefile for ref
	@echo nothing to see here

.PHONY : init
init:
	@echo init ran
	- source ~/.profile

bin/executable:  objects/Demo_01_Agent.o objects/Demo_01_Model.o objects/policy.o objects/Demo_01_Main.o  #objects/Fuzzy.o
	@echo Building Executable ...
	@$(MPICXX) $(BOOST_LIB_DIR) $(REPAST_HPC_LIB_DIR)  -o ./bin/executable ./objects/*.o $(REPAST_HPC_LIB) $(BOOST_LIBS) -w
	@echo Executable built ...

objects/Demo_01_Agent.o: src/Demo_01_Agent.cpp
	@echo Creating agent.o ...
	@$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE)  -I ./include/ -c ./src/Demo_01_Agent.cpp -o ./objects/Demo_01_Agent.o  -w
	@echo Demo_01_Agent.o created

objects/Demo_01_Model.o: src/Demo_01_Model.cpp
	@echo Creating model.o ...
	@$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE)  -I ./include/ -c ./src/Demo_01_Model.cpp -o ./objects/Demo_01_Model.o  -w
	@echo Demo_01_Model.o created

# objects/Fuzzy.o: src/Demo_01_Model.cpp
# 	@echo Creating Fuzzy.o ...
# 	@$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE)  -I ./include/ -c ./src/Fuzzy.cpp -o ./objects/Fuzzy.o  -w
# 	@echo Fuzzy.o created

objects/policy.o: src/policy.cpp
	@echo Creating policy.o ...
	@$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE)  -I ./include/ -c ./src/policy.cpp -o ./objects/policy.o  -w
	@echo policy.o created

objects/Demo_01_Main.o: src/Demo_01_Main.cpp
	@echo Creating main.o ...
	@$(MPICXX) $(REPAST_HPC_DEFINES) $(BOOST_INCLUDE) $(REPAST_HPC_INCLUDE)  -I ./include/ -c ./src/Demo_01_Main.cpp -o ./objects/Demo_01_Main.o  -w
	@echo Demo_01_Main.o created


.PHONY : run
run: bin/executable ./props/config.props ./props/model.props
	@echo Running Simulation
	mpirun -n 1 ./bin/executable ./props/config.props ./props/model.props


.PHONY : clean
clean:
	@echo Cleaning!
	-rm objects/*.o

.PHONY : clear
clear: clean
	@echo Clearing!
	-rm bin/*
