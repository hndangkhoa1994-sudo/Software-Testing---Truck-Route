#include "pch.h"
#include "CppUnitTest.h"
#include "information_r.h" // Include your function declarations
#include <cstring>
#include <assert.h>
#include "assert.h"
#include <fstream>
#include <cstdio>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PostalCodeTests {

    TEST_CLASS(BlackBoxPostalTests) {
    public:

    TEST_METHOD(FindTruck_BlackboxTest1)
    {
        Map map = { 0 };
        Truck trucks[] = {
            { 0, 100, 50, {} }, // Partially loaded truck
            { 1, 200, 100, {} }, // Heavily loaded truck
            { 2, 0, 0, {} }      // Empty truck
        };
        Package shipment = { 50, 20, 3, {5, 5} }; // Small shipment

        int truckIndex = findTruckForShipment(&map, trucks, 3, &shipment);
        Assert::AreEqual(2, truckIndex); // Expect empty truck is chosen
    }

    TEST_METHOD(AddPackageTest)
    {
        Truck truck = { 0, 0, 0, {} };
        Package pkg = { 20, 10 };
        addPackage(&truck, pkg);

        Assert::AreEqual(1, truck.n_package);
        Assert::AreEqual(20, truck.currentWeight);
        Assert::AreEqual(10, truck.currentVolume);


        truck.n_package = LIMIT_PACKAGE - 1;
        addPackage(&truck, pkg);
        Assert::AreEqual(LIMIT_PACKAGE, truck.n_package);


        addPackage(&truck, pkg);
        Assert::AreEqual(LIMIT_PACKAGE, truck.n_package);
    }

    TEST_METHOD(FindTruck_WhiteboxTest1)
    {
        Truck trucks[] = {
            { 0, 100, 50, {} },
            { 1, 50, 30, {} }
        };
        Package shipment = { 40, 20, 3, {3, 3} };
        int truckIndex = findTruckForShipment(NULL, trucks, 2, &shipment);
        Assert::AreEqual(1, truckIndex); // Best capacity score
    }

    TEST_METHOD(FindTruck_WhiteboxTest2)
    {
        Truck trucks[] = {
            { 0, LIMIT_WEIGHT, LIMIT_VOLUME, {} }
        };
        Package shipment = { 10, 5, 3, {2, 2} };
        int truckIndex = findTruckForShipment(NULL, trucks, 1, &shipment);
        Assert::AreEqual(-1, truckIndex); // No truck can hold
    }

    TEST_METHOD(FindTruck_WhiteboxTest3) {
        Truck trucks[] = { { 0, 200, 20, {} } };
        Package shipment = { 200, 30, 3, {1, 1} };
        int truckIndex = findTruckForShipment(NULL, trucks, 1, &shipment);
        Assert::AreEqual(0, truckIndex); // Expect exact fit
    }
    
    TEST_METHOD(FindTruck_WhiteboxTest4)
    {
        Truck trucks[] = {
            { 0, 0, 0, {} }
        };
        Package shipment = { 0, 0, 3, {0, 0} };
        int truckIndex = findTruckForShipment(NULL, trucks, 1, &shipment);
        Assert::AreEqual(0, truckIndex); // Even a 0 package fits
    }

    TEST_METHOD(FindTruck_WhiteboxTest5) {
        Truck trucks[] = {
            { 0, 200, 150, {} },
            { 1, 300, 25, {} }
        };
        Package shipment = { 100, 25, 3, {5, 5} };
        int truckIndex = findTruckForShipment(NULL, trucks, 2, &shipment);
        Assert::AreEqual(1, truckIndex); // Truck 1 has fit capacity
    }

    TEST_METHOD(AddPackage_WhiteboxTest1) {
        Truck truck = { 0, 100, 30, 0 }; //change 50 to 30
        Package pkg = { 30, 20 };
        addPackage(&truck, pkg);
        Assert::AreEqual(1, truck.n_package); // Expect 1 package to be added
    }

    TEST_METHOD(AddPackage_WhiteboxTest2)
    {
        Truck truck = { 0, LIMIT_WEIGHT - 10, LIMIT_VOLUME - 5, {} };
        Package pkg = { 11, 6 };
        addPackage(&truck, pkg);
        Assert::AreEqual(0, truck.n_package); // No package added
    }

    TEST_METHOD(AddPackage_WhiteboxTest3)
    {
        Truck truck = { 0, 0, 0, {} };
        Package pkg = { 0, 0 };
        addPackage(&truck, pkg);
        Assert::AreEqual(1, truck.n_package);
    }

    TEST_METHOD(AddPackage_WhiteboxTest4) {
        Truck truck;
        truck.m_truckColor = 0;
        truck.currentWeight = 200;
        truck.currentVolume = 50;
        truck.n_package = LIMIT_PACKAGE;  // Explicitly set to LIMIT_PACKAGE
        Package pkg = { 10, 5 };
        addPackage(&truck, pkg);
        Assert::AreEqual(LIMIT_PACKAGE, truck.n_package); // No package should be added
    }

    TEST_METHOD(AddPackage_WhiteboxTest5)
    {
        Truck truck = { 0, 0, 0, {} };
        Package pkg = { LIMIT_WEIGHT, LIMIT_VOLUME };
        addPackage(&truck, pkg);
        Assert::AreEqual(1, truck.n_package);
    }

    TEST_METHOD(CapacityLeft_WhiteboxTest1) {
        Truck truck = { 0, 200, 50, {} };
        double left = capacityLeft(&truck);
        Assert::AreEqual(83.33,  left); // Expect 83.33% capacity left. Apply Rounding Helper Function.
    }

    TEST_METHOD(CapacityLeft_WhiteboxTest2)
    {
        Truck truck = { 0, 0, 0, {} };
        double left = capacityLeft(&truck);
        Assert::AreEqual(100.0, left);
    }

    TEST_METHOD(CapacityLeft_WhiteboxTest3)
    {
        Truck truck = { 0, LIMIT_WEIGHT, LIMIT_VOLUME, {} };
        double left = capacityLeft(&truck);
        Assert::AreEqual(0.0, left);
    }

    TEST_METHOD(CapacityLeft_WhiteboxTest4)
    {
        Truck truck = { 0, LIMIT_WEIGHT / 2, LIMIT_VOLUME / 2, {} };
        double left = capacityLeft(&truck);
        Assert::AreEqual(50.0, left);
    }

    TEST_METHOD(CapacityLeft_WhiteboxTest5)
    {
        Truck truck = { 0, LIMIT_WEIGHT - 10, LIMIT_VOLUME - 20, {} };
        double left = capacityLeft(&truck);
        Assert::IsTrue(left > 0);
    }

    TEST_METHOD(PathCalculation_WhiteboxTest1)
    {
        Point begin = { 0, 0 };
        Point dest = { 5, 5 };
        Path path = pathCalculation(NULL, begin, dest);
        Assert::AreEqual(10, path.p_length);
    }

    TEST_METHOD(PathCalculation_WhiteboxTest2)
    {
        Point begin = { 0, 0 };
        Point dest = { 0, 0 };
        Path path = pathCalculation(NULL, begin, dest);
        Assert::AreEqual(1, path.p_length);
    }

    TEST_METHOD(PathCalculation_WhiteboxTest3)
    {
        Point begin = { 0, 0 };
        Point dest = { 0, 5 };
        Path path = pathCalculation(NULL, begin, dest);
        Assert::AreEqual(5, path.p_length);
    }

    TEST_METHOD(PathCalculation_WhiteboxTest4)
    {
        Point begin = { 5, 5 };
        Point dest = { 0, 0 };
        Path path = pathCalculation(NULL, begin, dest);
        Assert::AreEqual(10, path.p_length);
    }

    TEST_METHOD(PathCalculation_WhiteboxTest5)
    {
        Point begin = { 1, 1 };
        Point dest = { 1, 3 };
        Path path = pathCalculation(NULL, begin, dest);
        Assert::AreEqual(2, path.p_length);
    }

    TEST_METHOD(CanAddPackage_WhiteboxTest1) {
        Truck truck = { 0, 100, 40, {} };  //current volume is 40, Limit volume is 50
        Package pkg = { 30, 10 };
        Assert::AreEqual(1, canAddPackage(&truck, &pkg));
    }

    TEST_METHOD(CanAddPackage_WhiteboxTest2)
    {
        Truck truck = { 0, LIMIT_WEIGHT, LIMIT_VOLUME, {} };
        Package pkg = { 10, 5 };
        Assert::AreEqual(0, canAddPackage(&truck, &pkg));
    }

    TEST_METHOD(CanAddPackage_WhiteboxTest3)
    {
        Truck truck = { 0, 0, 0, {} };
        Package pkg = { 0, 0 };
        Assert::AreEqual(1, canAddPackage(&truck, &pkg));
    }

    TEST_METHOD(CanAddPackage_WhiteboxTest4)
    {
        Truck truck = { 0, LIMIT_WEIGHT - 1, LIMIT_VOLUME - 1, {} };
        Package pkg = { 2, 2 };
        Assert::AreEqual(0, canAddPackage(&truck, &pkg));
    }

    TEST_METHOD(CanAddPackage_WhiteboxTest5)
    {
        Truck truck = { 0, 200, 100, {} };
        Package pkg = { -1, -1 };
        Assert::AreEqual(0, canAddPackage(&truck, &pkg)); // Invalid input
    }
    };

}







