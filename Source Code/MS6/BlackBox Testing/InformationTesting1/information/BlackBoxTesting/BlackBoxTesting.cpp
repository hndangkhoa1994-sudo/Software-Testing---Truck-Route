#include "pch.h"
#include "CppUnitTest.h"
#include "pch.h"
#include "CppUnitTest.h"
#include "information.h" // Include your function declarations
#include <cstring>
#include <assert.h>
#include "assert.h"
#include <fstream>
#include <cstdio>



using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BlackBoxTesting
{

    TEST_CLASS(BlackBoxTesting)
    {
    public:




       



        // Test 2: Testing addRoute function
        TEST_METHOD(TestAddRoute_ValidRoute)
        {
            // Arrange
            struct Map baseMap = populateMap();
            struct Route blueRoute = getBlueRoute();

            // Act
            struct Map routeMap = addRoute(&baseMap, &blueRoute);

            // Assert
            for (int i = 0; i < blueRoute.numPoints; ++i) {
                int row = blueRoute.points[i].row;
                int col = blueRoute.points[i].col;

                // Ensure the types match and cast if necessary
                Assert::AreEqual((int)blueRoute.routeSymbol, routeMap.squares[row][col], L"Route symbol mismatch.");
            }
        }



        // Test 3: Testing countPackages function
        TEST_METHOD(TestCountPackages) {
            struct Truck truck = { 0, 0, 0, 0 }; // Empty truck

            // Act and Assert
            Assert::AreEqual(0, countPackages(&truck), L"Incorrect count for empty truck.");

            truck.n_package = 50;
            Assert::AreEqual(50, countPackages(&truck), L"Incorrect count for 50 packages.");

            truck.n_package = 50;
            Assert::AreEqual(50, countPackages(&truck), L"Incorrect count for max packages.");
        }

        // Test 4: Testing capacityLeft function

        TEST_METHOD(TestCapacityLeft) {
            struct Truck truck = { 0, 600, 20, 0 }; // Half-loaded truck

            // Act and Assert
            double capacity = capacityLeft(&truck);
            Assert::IsTrue(capacity > 0.0 && capacity < 100.0, L"Capacity calculation is incorrect.");
        }

        //Test 5: Valid Case for findTruckForShipment

        TEST_METHOD(TestFindTruckForShipment_ValidCase) {
            // Arrange
            const int numTrucks = 2;
            struct Truck trucks[] = {
                {500, 300},  // Truck 1
                {200, 100}   // Truck 2
            };
            struct Package shipment = { 100, 50 };  // Valid shipment

            // Act
            int selectedTruckIndex = findTruckForShipment(nullptr, trucks, numTrucks, &shipment);

            // Assert
            Assert::AreEqual(1, selectedTruckIndex, L"Expected Truck 2 to be selected.");
        }

        //Test 6: Exact Fit Case
        TEST_METHOD(TestFindTruckForShipment_ExactFit) {
            // Arrange
            const int numTrucks = 1;
            struct Truck trucks[] = {
                {400, 300}  // Truck 1 with exact remaining capacity
            };
            struct Package shipment = { 100, 50 };

            // Act
            int selectedTruckIndex = findTruckForShipment(nullptr, trucks, numTrucks, &shipment);

            // Assert
            Assert::AreEqual(0, selectedTruckIndex, L"Expected Truck 1 to be selected (exact fit).");
        }

        //Test 7: 
        // Test: No Suitable Truck
        TEST_METHOD(TestFindTruckForShipment_NoFit) {
            // Arrange
            const int numTrucks = 1;
            struct Truck trucks[] = {
                {900, 950}  // Truck 1 is almost full
            };
            struct Package shipment = { 200, 100 };

            // Act
            int selectedTruckIndex = findTruckForShipment(nullptr, trucks, numTrucks, &shipment);

            // Assert
            Assert::AreEqual(-1, selectedTruckIndex, L"No truck should be selected.");
        }
    };
}
