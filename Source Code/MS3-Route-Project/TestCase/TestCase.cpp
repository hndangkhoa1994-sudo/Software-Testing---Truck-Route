#include "pch.h"
#include "CppUnitTest.h"
#include "truck.h"
#include "mapping.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestCase
{
	TEST_CLASS(TestCase)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
            int result = findTruckForShipment(&map, trucks, 3, &shipment);
            assert(result == 0); 
            printf("Test 1 passed: Truck %d selected for shipment\n", result + 1);
		}
	};
}
