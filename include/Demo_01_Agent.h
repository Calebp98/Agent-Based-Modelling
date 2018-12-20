/* Demo_01_Agent.h */
// (agent)

#ifndef DEMO_01_AGENT
#define DEMO_01_AGENT

#include "repast_hpc/AgentId.h"
#include "repast_hpc/SharedContext.h"

#include "repast_hpc/SharedDiscreteSpace.h"


#include <map>


/* Agents */
class RepastHPCDemoAgent{

private:
    repast::AgentId   id_; // agent id
    double              c; //
    double          total;

		double age; //age of agent
		char mode; //mode of transport that agent is using
		char newMode; //potential new mode of agent
		int income; //personal income of agent
		int travelDistance; //distance of agents journey
    std::map<char,int> influence; //map containing influence of agents for certain modes from network
    bool carOwnership;
    bool bikeOwnership;
    bool lastSearch; //did agent search last time ?



public:
    RepastHPCDemoAgent(repast::AgentId id);

		RepastHPCDemoAgent(repast::AgentId id, double age, char mode, int income, int travelDistance, std::map<char,int> influence,  bool carOwnership, bool bikeOwnership); //added to set mode
	//RepastHPCDemoAgent(repast::AgentId id, char mode); //added to set mode


    RepastHPCDemoAgent(repast::AgentId id, double newC, double newTotal);

    ~RepastHPCDemoAgent();

    /* Required Getters */
    virtual repast::AgentId& getId(){                   return id_;    }
    virtual const repast::AgentId& getId() const {      return id_;    }

    /* Getters specific to this kind of Agent */
    double getC(){                                      return c;      }
    double getTotal(){                                  return total;  }

		//getters added for searching and decisions
		double getAge(){ 				return age;}
		char getMode(){				return mode;}
		int getIncome(){			return income;}
		int getTravelDistance(){	return travelDistance;}
    bool getLastSearch(){return lastSearch;}
    char getNewMode(){return newMode;}


    /* Setter */
    void set(int currentRank, double newC, double newTotal);

    /* Actions */
    bool cooperate();                                                 // Will indicate whether the agent cooperates or not; probability determined by = c / total
    void play(repast::SharedContext<RepastHPCDemoAgent>* context,repast::SharedDiscreteSpace<RepastHPCDemoAgent, repast::WrapAroundBorders, repast::SimpleAdder<RepastHPCDemoAgent> >* space);    // Choose three other agents from the given context and see if they cooperate or not

		bool decideToSearch();
		char search();
		void decide(char newMode);
};

#endif
