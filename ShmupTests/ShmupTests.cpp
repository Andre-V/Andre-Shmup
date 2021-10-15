#include "pch.h"
#include "CppUnitTest.h"
#include "Component.h"
#include "Component.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ShmupTests
{
	// NOTE: CompManager has static members, so values will
	// remain consistent throughout all tests.
	TEST_CLASS(CompManagerTests)
	{
	public:
		class dummy1; class dummy2; class dummy3; class dummy4;
		// Tests that the addition of 3 newly encountered classes
		// increases the total size by 3.
		TEST_METHOD(TestNewTypes)
		{
			int prevSize = CmpMngr::size();

			CmpMngr::getTypeID<dummy1>();
			CmpMngr::getTypeID<dummy2>();
			CmpMngr::getTypeID<dummy3>();

			Assert::AreEqual(prevSize + 3, CmpMngr::size());
		}
		// Tests that retrieval of ID of same type repeatedly
		// does not increase total size
		TEST_METHOD(TestSameType)
		{
			CmpMngr::getTypeID<dummy1>();

			int prevSize = CmpMngr::size();

			CmpMngr::getTypeID<dummy1>();
			CmpMngr::getTypeID<dummy1>();

			Assert::AreEqual(prevSize, CmpMngr::size());
		}
		// Tests addition of new class has expected ID value
		TEST_METHOD(TestValue)
		{
			int prevSize = CmpMngr::size();

			CmpMngr::getTypeID<dummy4>();

			Assert::AreEqual(prevSize, CmpMngr::getTypeID<dummy4>());
		}
	};
	
}
