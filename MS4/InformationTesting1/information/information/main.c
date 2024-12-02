#include <stdio.h>
#include "mapping.h"
#include "truck.h"

int main(void)
{
    // Step 1: Initialize the map and route
    struct Map baseMap = populateMap();
    struct Route blueRoute = getBlueRoute();
    struct Map routeMap = addRoute(&baseMap, &blueRoute);
    printMap(&routeMap, 1, 1);

    // Step 2: Initialize trucks and a package
    struct Truck trucks[2] = {
        {0, 200, 100, 1, {{ { {0}, {2} }, 2 }}}, // Route với 2 điểm
        {1, 100, 50, 2, {{ { {1}, {4} }, 2 }},}  // Route với 2 điểm
    };

    struct Package shipment = { 50, 20, {5} };

    // Step 3: Test findTruckForShipment
    int selectedTruck = findTruckForShipment(&baseMap, trucks, 2, &shipment);
    printf("Selected truck: %d\n", selectedTruck);

    // Step 4: Test addPackage
    printf("Adding package to truck %d...\n", selectedTruck);
    addPackage(&trucks[selectedTruck], shipment);

    // Step 5: Test capacityLeft
    double remainingCapacity = capacityLeft(&trucks[selectedTruck]);
    printf("Remaining capacity: %.2f%%\n", remainingCapacity);

    // Step 6: Test pathCalculation
    struct Point start = { 0, 0 };
    struct Point dest = { 5, 5 };
    struct Path path = pathCalculation(&baseMap, start, dest);
    printf("Calculated path length: %d\n", path.p_length);

    // Step 7: Test canAddPackage
    int canAdd = canAddPackage(&trucks[selectedTruck], &shipment);
    printf("Can add package: %s\n", canAdd ? "Yes" : "No");

    // Step 8: Test countPackages
    int packageCount = countPackages(&trucks[selectedTruck]);
    printf("Number of packages in truck: %d\n", packageCount);

    return 0;
}
