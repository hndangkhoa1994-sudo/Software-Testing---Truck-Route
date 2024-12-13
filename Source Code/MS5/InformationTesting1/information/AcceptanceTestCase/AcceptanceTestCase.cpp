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



    };
}