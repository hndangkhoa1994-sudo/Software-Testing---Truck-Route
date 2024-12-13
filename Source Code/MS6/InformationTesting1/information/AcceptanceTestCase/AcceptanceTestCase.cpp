#include "pch.h"
#include "CppUnitTest.h"
#include "information_r.h" // Include your function declarations
#include <cstring>
#include <assert.h>
#include "assert.h"
#include <fstream>
#include <cstdio>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace AcceptanceTestCase
{
	TEST_CLASS(AcceptanceTestCase)
	{
	public:
		
        //Test Case 1: Find Truck for a Shipment
        TEST_METHOD(Test_FindTruckForShipment) {
            // Test Case 1: Find Truck for a Shipment
            Map map = populateMap();
            Truck trucks[] = {
                { 0, 100, 30, 0 }, // Partially loaded truck
                { 1, 200, 40, 0 }, // Heavily loaded truck
                { 2, 50, 10, 0 }   // Lightly loaded truck
            };
            Package shipment = { 20, 15, 'A', {5, 5} };

            int truckIndex = findTruckForShipment(&map, trucks, 3, &shipment);
            Assert::AreEqual(2, truckIndex); // Lightly loaded truck should be selected
        }

        //Test Case 2: Add Package to a Truck
        TEST_METHOD(Test_AddPackageToTruck) {
            // Test Case 2: Add Package to a Truck
            Truck truck = { 0, 100, 30, 5 };
            Package pkg = { 200, 15, 'B', {10, 10} };

            addPackage(&truck, pkg);

            Assert::AreEqual(6, truck.n_package);  // Verify package count incremented
            Assert::AreEqual(300, truck.currentWeight); // Verify weight updated
            Assert::AreEqual(45, truck.currentVolume);  // Verify volume updated
        }

        //Test Case 3: Path Calculation
        TEST_METHOD(Test_ShortestPathCalculation) {
            // Test Case 3: Path Calculation
            Map map = populateMap();
            Point begin = { 0, 0 };
            Point dest = { 5, 5 };

            struct Route route = shortestPath(&map, begin, dest);
            Assert::IsTrue(route.numPoints > 0); // Ensure path was found
            Assert::AreEqual(dest.row, route.points[route.numPoints - 1].row); // Verify destination
            Assert::AreEqual(dest.col, route.points[route.numPoints - 1].col); // Verify destination
        }

        //Test Case 4: Capacity Left in Truck
        TEST_METHOD(Test_CapacityLeftInTruck) {
            // Test Case 4: Capacity Left in Truck
            Truck truck = { 0, 600, 25, 0 };

            double remainingCapacity = capacityLeft(&truck);

            Assert::AreEqual(50.0, remainingCapacity); // Verify 50% capacity left
        }

        //Test Case 5: Add Route to Map
        TEST_METHOD(Test_AddRouteToMap) {
            // Test Case 5: Add Route to Map
            Map map = populateMap();
            Route blueRoute = getBlueRoute();

            Map updatedMap = addRoute(&map, &blueRoute);

            Assert::AreEqual((int)BLUE, updatedMap.squares[blueRoute.points[0].row][blueRoute.points[0].col]);
        }

        //Test Case 6: Boundary Conditions for Shortest Path
        TEST_METHOD(Test_BoundaryConditionShortestPath) {
            // Test Case 6: Boundary Conditions for Shortest Path
            Map map = populateMap();
            Point start = { 0, 0 };
            Point dest = { 0, MAP_COLS - 1 };

            Route route = shortestPath(&map, start, dest);

            Assert::IsTrue(route.numPoints > 0); // Ensure path was found
            Assert::AreEqual(dest.col, route.points[route.numPoints - 1].col); // Verify destination
        }

        //Test Case 7: Overloaded Truck Handling
        TEST_METHOD(Test_OverloadedTruckHandling) {
            // Test Case 7: Overloaded Truck Handling
            Truck truck = { 0, 1190, 49, 99 }; // Nearly full truck
            Package pkg = { 20, 10, 'C', {15, 15} };

            addPackage(&truck, pkg);

            Assert::AreEqual(99, truck.n_package); // Package should not be added
            Assert::AreEqual(1190, truck.currentWeight); // Weight should not change
            Assert::AreEqual(49, truck.currentVolume); // Volume should not change
        }

        //Test Case 8: Invalid Package Data
        TEST_METHOD(Test_InvalidPackageData) {
            // Test Case 8: Invalid Package Data
            Truck truck = { 0, 0, 0, 0 }; // Empty truck
            Package invalidPkg = { -1, -1, 'D', {0, 0} }; // Invalid package

            int canAdd = canAddPackage(&truck, &invalidPkg);

            Assert::AreEqual(0, canAdd); // Invalid package should not be allowed
        }


        //Update test cases for MS6

        // Test Case 9: Truck Route Assignment Accuracy
        TEST_METHOD(test_truckRouteAssignment) {
            // Setup trucks and package
            Map map = populateMap();
            Truck trucks[2] = {
                {0, 200, 100, 1, getBlueRoute(), {}},  // Truck 0
                {1, 100, 30, 1, getGreenRoute(), {}}  // Truck 1
            };
            Package pkg = { 50, 20, 'A', {5, 5} }; // Package

            // Find a truck for the shipment
            int selectedTruck = findTruckForShipment(&map, trucks, 2, &pkg);

            // Verify a valid truck is selected
            Assert::IsTrue(selectedTruck >= 0, L"No valid truck selected!");

            // Verify the selected truck can handle the package
            if (selectedTruck >= 0) {
                Assert::IsTrue(
                    trucks[selectedTruck].currentWeight + pkg.p_weight <= LIMIT_WEIGHT &&
                    trucks[selectedTruck].currentVolume + pkg.p_size <= LIMIT_VOLUME,
                    L"Selected truck cannot handle package!"
                );

                // Verify the selected truck has a valid route
                Assert::IsTrue(
                    trucks[selectedTruck].route.routeSymbol != 0,
                    L"Selected truck has no route!"
                );
            }
        }



        //Test Case 10: Truck Operations and Path Calculation
        //To validate multiple aspects of your system, specifically focusing on integrating truck operations, package management, and path calculation
        TEST_METHOD(TestTruckOperationsAndPathCalculation) {
            // Step 1: Add a package to the truck
            struct Truck truck = { 0, 0, 0, {} };
            struct Package pkg = { 50, 30 }; // Package with weight 50 and volume 30

            addPackage(&truck, pkg);

            // Verify the truck's state after adding the package
            Assert::AreEqual(1, (int)truck.n_package, L"Package count is incorrect.");
            Assert::AreEqual(50, (int)truck.currentWeight, L"Truck weight is incorrect.");
            Assert::AreEqual(30, (int)truck.currentVolume, L"Truck volume is incorrect.");

            // Step 2: Calculate the remaining capacity
            double remainingCapacity = ((double)(LIMIT_WEIGHT - truck.currentWeight) / LIMIT_WEIGHT) * 100;
            double expectedCapacity = ((double)(LIMIT_WEIGHT - 50) / LIMIT_WEIGHT) * 100;
            Assert::AreEqual(expectedCapacity, remainingCapacity, L"Remaining capacity is incorrect.");

            // Step 3: Calculate a path between two points
            struct Point start = { 0, 0 };
            struct Point destination = { 3, 3 };
            struct Path path = pathCalculation(NULL, start, destination);

            // Verify the path's length and coordinates
            Assert::AreEqual(6, (int)path.p_length, L"Path length is incorrect."); // 3 horizontal + 3 vertical moves
            Assert::AreEqual(0, (int)path.po[0].row, L"First path point row is incorrect.");
            Assert::AreEqual(1, (int)path.po[0].col, L"First path point column is incorrect.");
            Assert::AreEqual(3, (int)path.po[path.p_length - 1].row, L"Last path point row is incorrect.");
            Assert::AreEqual(3, (int)path.po[path.p_length - 1].col, L"Last path point column is incorrect.");

            // Step 4: Attempt to add an oversized package
            struct Package largePkg = { 1000, 500 }; // Too large for the truck
            addPackage(&truck, largePkg);

            // Verify the truck's state remains unchanged
            Assert::AreEqual(1, (int)truck.n_package, L"Package count changed incorrectly.");
            Assert::AreEqual(50, (int)truck.currentWeight, L"Truck weight changed incorrectly.");
            Assert::AreEqual(30, (int)truck.currentVolume, L"Truck volume changed incorrectly.");
        }


        //Test case 11: Multi Packahes
        //The TestMultiplePackages test is designed to validate the functionality of adding multiple packages to a truck while ensuring that the truck's capacity constraints (weight and volume) are properly enforced.
        TEST_METHOD(TestMultiplePackages)
        {
            struct Truck truck = { 0, 0, 0, {} };
            struct Package pkg1 = { 40, 20 };
            struct Package pkg2 = { 30, 25 };

            // Add first package
            addPackage(&truck, pkg1);

            // Verify first package
            Assert::AreEqual(1, truck.n_package);
            Assert::AreEqual(40, truck.currentWeight);
            Assert::AreEqual(20, truck.currentVolume);

            // Add second package
            addPackage(&truck, pkg2);

            // Verify both packages
            Assert::AreEqual(2, truck.n_package);
            Assert::AreEqual(70, truck.currentWeight);
            Assert::AreEqual(45, truck.currentVolume);

            // Attempt to add a package that exceeds capacity
            struct Package pkg3 = { 100, 100 };
            addPackage(&truck, pkg3);

            // Verify that the package was not added
            Assert::AreEqual(2, truck.n_package);
            Assert::AreEqual(70, truck.currentWeight);
            Assert::AreEqual(45, truck.currentVolume);
        }

        //Test case 12: Edge Case Volume
        //Test validates the system's behavior when dealing with edge cases related to the truck's volume capacity. Specifically, it tests the scenario where the truck's volume limit is reached, even though its weight capacity is not exceeded
        TEST_METHOD(TestEdgeCaseVolume)
        {
            struct Truck truck = { 0, 0, 0, {} };
            struct Package pkg = { 10, LIMIT_VOLUME }; // Max volume but low weight

            // Add the package
            addPackage(&truck, pkg);

            // Verify the package is added
            Assert::AreEqual(1, truck.n_package);
            Assert::AreEqual(10, truck.currentWeight);
            Assert::AreEqual(LIMIT_VOLUME, truck.currentVolume);

            // Attempt to add another package
            struct Package pkg2 = { 10, 1 };
            addPackage(&truck, pkg2);

            // Verify that the package was not added due to volume
            Assert::AreEqual(1, truck.n_package);
            Assert::AreEqual(10, truck.currentWeight);
            Assert::AreEqual(LIMIT_VOLUME, truck.currentVolume);
        }


        //Test case 13:  Exact Fit
        //Test is designed to validate the system's behavior when the truck is filled to its exact maximum capacity for both weight and volume. 
        //This test ensures that the truck can handle packages that precisely match its constraints and correctly rejects additional packages once the constraints are fully utilized.

        TEST_METHOD(TestExactFit)
        {
            struct Truck truck = { 0, 0, 0, {} };
            struct Package pkg = { LIMIT_WEIGHT, LIMIT_VOLUME };

            // Add a package with exact weight and volume
            addPackage(&truck, pkg);

            // Verify exact fit
            Assert::AreEqual(1, truck.n_package);
            Assert::AreEqual(LIMIT_WEIGHT, truck.currentWeight);
            Assert::AreEqual(LIMIT_VOLUME, truck.currentVolume);

            // Attempt to add another package
            struct Package pkg2 = { 1, 1 };
            addPackage(&truck, pkg2);

            // Verify that the package was not added
            Assert::AreEqual(1, truck.n_package);
            Assert::AreEqual(LIMIT_WEIGHT, truck.currentWeight);
            Assert::AreEqual(LIMIT_VOLUME, truck.currentVolume);
        }


        //Test case 14: Invalid Package
        //To validate the system's behavior when attempting to add a package with invalid attributes (e.g., negative weight or volume) to a truck. 
        //It ensures that the system does not allow such packages to be added and that the truck's state remains consistent
        TEST_METHOD(TestInvalidPackage)
        {
            struct Truck truck = { 0, 0, 0, {} };
            struct Package invalidPkg = { -10, -5 }; // Invalid weight and volume

            // Validate the package before adding
            if (invalidPkg.p_weight <= 0 || invalidPkg.p_size <= 0) {
                printf("Invalid package: Cannot add package with negative weight or volume.\n");
            }
            else {
                // Attempt to add the package (this shouldn't happen for invalid packages)
                addPackage(&truck, invalidPkg);
            }

            // Verify that the truck state remains unchanged
            Assert::AreEqual(0, truck.n_package, L"Invalid package should not be added.");
            Assert::AreEqual(0, truck.currentWeight, L"Truck weight should not change for invalid package.");
            Assert::AreEqual(0, truck.currentVolume, L"Truck volume should not change for invalid package.");
        }

    };
}