#include "pch.h"
#include "CppUnitTest.h"
#include "information_r.h" // Include your function declarations
#include <cstring>
#include <assert.h>
#include "assert.h"
#include <fstream>
#include <cstdio>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(IntegrationTesting)
	{
	public:
		
        // Test Case 2: Add Packages to Truck and Validate Constraints
        TEST_METHOD(Test_AddPackagesToTruckWithConstraints)
        {
            Truck truck = { 0, 1100, 45, 99 }; // Nearly full truck
            Package validPkg = { 50, 5, 'B', {10, 10} }; // Valid package
            Package invalidPkg = { 200, 10, 'C', {15, 15} }; // Invalid package

            // Try adding valid package
            addPackage(&truck, validPkg);
            Assert::AreEqual(100, truck.n_package); // Package count should increment
            Assert::AreEqual(1150, truck.currentWeight);
            Assert::AreEqual(50, truck.currentVolume);

            // Try adding invalid package
            addPackage(&truck, invalidPkg);
            Assert::AreEqual(100, truck.n_package); // Package count should remain same
            Assert::AreEqual(1150, truck.currentWeight);
            Assert::AreEqual(50, truck.currentVolume);
        }

        // Test Case 2: Assigning a Shipment to a Truck with Route
        TEST_METHOD(Test_ShipmentAssignmentWithRoute)
        {
            Map map = populateMap();
            Truck trucks[] = {
                { 0, 100, 30, 0 }, // Partially loaded truck
                { 1, 50, 10, 0 }, // Lightly loaded truck
            };
            Package shipment = { 20, 15, 'A', {5, 5} };

            // Find a suitable truck
            int truckIndex = findTruckForShipment(&map, trucks, 2, &shipment);
            Assert::AreEqual(1, truckIndex); // Expect truck with index 1

            // Assign the route to the truck
            Route route = shortestPath(&map, { 0, 0 }, shipment.dest);
            trucks[truckIndex].route = route;

            // Validate truck route
            Assert::IsTrue(trucks[truckIndex].route.numPoints > 0);
            Assert::AreEqual(shipment.dest.row, trucks[truckIndex].route.points[route.numPoints - 1].row);
            Assert::AreEqual(shipment.dest.col, trucks[truckIndex].route.points[route.numPoints - 1].col);
        }

        // Test Case 3: Path Calculation Between Points
        TEST_METHOD(Test_PathCalculationForShipment)
        {
            Map map = populateMap();
            Point start = { 0, 0 };
            Point dest = { 5, 5 };

            // Calculate shortest path
            Route route = shortestPath(&map, start, dest);

            // Validate path
            Assert::IsTrue(route.numPoints > 0);
            Assert::AreEqual(dest.row, route.points[route.numPoints - 1].row);
            Assert::AreEqual(dest.col, route.points[route.numPoints - 1].col);
        }

        // Test Case 4: Validate Truck Capacity After Adding Packages
        TEST_METHOD(Test_TruckCapacityAfterPackages)
        {
            Truck truck = { 0, 600, 25, 0 }; // Half-filled truck
            Package pkg = { 200, 15, 'D', {10, 10} };

            // Add package
            addPackage(&truck, pkg);

            // Validate remaining capacity
            double capacityLeftPercent = capacityLeft(&truck);
            Assert::AreEqual(33.33, capacityLeftPercent, 0.01); // Expect ~33.33% capacity left
        }

        // Test Case 5: Adding Routes to Map
        TEST_METHOD(Test_AddingRoutesToMap)
        {
            Map map = populateMap();
            Route blueRoute = getBlueRoute();

            // Add route to map
            Map updatedMap = addRoute(&map, &blueRoute);

            // Validate route addition
            Assert::AreEqual((int)BLUE, updatedMap.squares[blueRoute.points[0].row][blueRoute.points[0].col]);
        }

        // Test Case 6: Full Shipment Workflow
        TEST_METHOD(Test_FullShipmentWorkflow)
        {
            // Setup map and trucks
            Map map = populateMap();
            Truck trucks[] = {
                { 0, 800, 40, 5 }, // Partially loaded truck
                { 1, 200, 10, 2 }, // Lightly loaded truck
            };
            Package shipment = { 50, 10, 'E', {10, 10} };

            // Assign truck
            int truckIndex = findTruckForShipment(&map, trucks, 2, &shipment);
            Assert::AreEqual(1, truckIndex); // Expect truck with index 1

            // Calculate path and assign route
            Route route = shortestPath(&map, { 0, 0 }, shipment.dest);
            trucks[truckIndex].route = route;

            // Add package to truck
            addPackage(&trucks[truckIndex], shipment);

            // Validate
            Assert::AreEqual(3, trucks[truckIndex].n_package); // Package count incremented
            Assert::AreEqual(250, trucks[truckIndex].currentWeight);
            Assert::AreEqual(20, trucks[truckIndex].currentVolume);
            Assert::IsTrue(trucks[truckIndex].route.numPoints > 0); // Route assigned
        }
	};
}
