#ifndef NEW_STRUCTURE_H
#define NEW_STRUCTURE_H
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
	struct Route route;  // using to following the route
	struct Package p[LIMIT_PACKAGE]; // list the packages that contain inside the truck
};
struct Package // represent to a package
{
	int p_weight; //package weight
	int p_size; // package size
	char p_id; // id of the package
	struct Point dest;
};
struct Path // additional detail to find shortest path
{
	struct Point po[MAX_ROUTE]; // path point
	int p_length; // the path lenght
};

int findTruckForShipment(const struct Map* m, const struct Truck* trucks, int numTrucks, const struct Package* shipment);

void addPackage(struct Truck* tr, struct Package pkg);

double capacityLeft(const struct Truck* tr);

struct Path pathCalculation(const struct Map* m, struct Point begin, struct Point dest);

int canAddPackage(const struct Truck* tr, const struct Package* pkg);

int countPackages(const struct Truck* tr);
#endif // !NEW_STRUCTURE_H