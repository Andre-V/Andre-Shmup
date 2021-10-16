#include "pch.h"
#include "CppUnitTest.h"
#include "Component.h"
#include "Component.cpp"
#include "Entity.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ShmupTests
{
	class dummy1 : public Component {}; class dummy2 : public Component {};
	class dummy3 : public Component {}; class dummy4 : public Component {};
	// NOTE: CompManager has static members, so values will
	// remain consistent throughout all tests.
	TEST_CLASS(CompManagerTests)
	{
	public:
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
	TEST_CLASS(EntityTests)
	{
	public:
		Entity e;
		TEST_METHOD(TestAddAndHas)
		{
			e.add<dummy1>();
			e.add<dummy2>();

			Assert::IsTrue(e.has<dummy1>());
			Assert::IsTrue(e.has<dummy2>());
			Assert::IsTrue(e.has<dummy1, dummy2>());

			Assert::IsFalse(e.has<dummy3>());
			Assert::IsFalse(e.has<dummy1, dummy3>());
		}
		TEST_METHOD(TestGet)
		{
			e.add<dummy1>();
			dummy1 dummy = e.get<dummy1>();
		}
	};
}
