/* policy.h */


#ifndef POLICY
#define POLICY

class policy{
private:
//for agents
  int bikeAge;
  int carToBikeSal;
  int bikeDist;

  int walkAge;
  int carToWalkSal;
  int walkDist;

  int bikeToCarSal;
  int carDist;

  int walkToCarSal;

//for world
  float bikeProlif; //availibility of bicycles upon initialisation
  float community; //influences network connectivity


public:
//constructor
  policy(int bikeAge_,int carToBikeSal_,int bikeDist_,int walkAge_,int carToWalkSal_,int walkDist_,int bikeToCarSal_,int carDist_,int walkToCarSal_);
  policy();

  ~policy();

  void start();

//policies
  //We will develop an investment plan that removes barriers to walking and cycling
  //effect make bikes more proliferate (barrier is presumably cost)
  void investment();
  //encourage our people to choose greener and healthier forms of transport. We will invest in schemes that enhance and better connect cycle ways and path ways to our public transport network, enabling people to easily switch from public transport services to cycling or walking.
  //effect make cycling safer (affect influence), suitable for longer journeys (travel distance)
  void paths();

  //deliver local walking and cycling schemes in partnership with local authorities and other partners, while also considering more strategic cycle schemes for the future.
  //group cycle/walk schemes (influence maybe other network dynamics)
  void groupSchemes();

  //electric bikes
  // make bikes suitable for older people and longer distances
  void electricBikes();


//getters
  int getBikeAge();
  int getCarToBikeSal();
  int getBikeDist();

  int getWalkAge();
  int getCarToWalkSal();
  int getWalkDist();

  int getBikeToCarSal();
  int getCarDist();

  int getWalkToCarSal();
  float getBikeProlif();
  float getCommunity();
};


#endif
