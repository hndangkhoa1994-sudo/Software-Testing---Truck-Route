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




        // Test 1: ValidateDestination

        TEST_METHOD(ValidateDestinationWrapper_BlackBoxTest) {
            // Inline wrapper function for validateDestination
            auto validateDestinationWrapper = [](const struct Map* map, const struct Point dest) -> int {
                int result = validateDestination(map, dest);

                // Add additional checks (if needed)
                if (result == 0) {
                    return 0; // Invalid destination
                }

                return 1; // Valid destination
                };

            // Arrange
            struct Map map;
            memset(map.squares, 0, sizeof(map.squares));
            map.numRows = 5;
            map.numCols = 5;

            // Set up test cases
            struct Point validCorner1 = { 0, 'A' };  // Top-left corner (valid)
            struct Point validCorner2 = { 4, 'E' };  // Bottom-right corner (valid)
            struct Point invalidRow = { 6, 'A' };   // Row out of bounds (invalid)
            struct Point invalidColumn = { 0, 'Z' }; // Column out of bounds (invalid)
            struct Point building = { 2, 'C' };     // A building (marked as 1 in the grid)
            struct Point empty = { 2, 'D' };        // Open space (marked as 0 in the grid)

            // Mark a building on the grid
            map.squares[2][2] = 1; // 'C' column at row 2 is a building

            // Act
            int result1 = validateDestinationWrapper(&map, validCorner1); // Expected: 1
            int result2 = validateDestinationWrapper(&map, validCorner2); // Expected: 1
            int result3 = validateDestinationWrapper(&map, invalidRow);   // Expected: 0
            int result4 = validateDestinationWrapper(&map, invalidColumn); // Expected: 0
            int result5 = validateDestinationWrapper(&map, building);     // Expected: 0
            int result6 = validateDestinationWrapper(&map, empty);        // Expected: 1

            // Assert
            Assert::IsTrue(result1 == 1, L"Failed: Valid top-left corner.");
            Assert::IsTrue(result2 == 1, L"Failed: Valid bottom-right corner.");
            Assert::IsTrue(result3 == 0, L"Failed: Invalid row out of bounds.");
            Assert::IsTrue(result4 == 0, L"Failed: Invalid column out of bounds.");
            Assert::IsTrue(result5 == 0, L"Failed: Destination is a building.");
            Assert::IsTrue(result6 == 1, L"Failed: Destination is a valid empty space.");
        }



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
