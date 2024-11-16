#ifndef ADDITIONAL_FUNCTIONS_H
#define ADDITIONAL_FUNCTIONS_H
#include "mapping.h" 
#define LIMIT_PACKAGE 100 // limit number for packages on a truck
#define LIMIT_WEIGHT 1200 // limit weight for a truck 
#define LIMIT_VOLUME 50 // limit volume for cubic for a truck to delivery
struct Truck
{
	int m_truckColor; // a number store ID for each color ex: 1 is blue, ...
	int currentWeight; // of truck
	int currentVolume; // of truck load
	int n_package; // number of package that truck contain
	struct Package p[LIMIT_PACKAGE]; // list the packages that contain inside the truck
	double p_truck; //determine percentage of the requirement for any truck
};

struct Package // detail for package
{
	int p_weight; //package weight
	int p_size; // package size
	char p_id; // id of the package
};

//p will be package and po will be point

struct Path // additional detail to find shortest path
{
	struct Point po[MAX_ROUTE]; // path point
	int p_length; // the path lenght
};
//DETERMINE TO HAVE MORE NOT JUST ONLY THIS
#endif 
