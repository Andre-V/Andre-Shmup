#include "pch.h"
#include "CppUnitTest.h"
#include "Component.h"
#include "Component.cpp"
#include "EntityManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ShmupTests
{
	class dummy1 : public Component 
	{
	public:
		int testInt = 5;
	}; 
	class dummy2 : public Component 
	{
	public:
		bool testBool = true;
	};
	class dummy3 : public Component {}; 
	class dummy4 : public Component {};
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
		TEST_METHOD(TestAddAndHas)
		{
			Entity e;
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
			Entity e;
			e.add<dummy1>();
			dummy1 comp1 = e.get<dummy1>();
		}
	};
	TEST_CLASS(EntityFactoryTests)
	{
		TEST_METHOD(TestJustMake)
		{
			EntityFactory ef;
			Entity& e = ef.make<dummy1>();
		}
	};
	TEST_CLASS(EntityManagerTests)
	{
		TEST_METHOD(TestJustMake)
		{
			EntityManager em;
			em.make<dummy1>();
			em.make<dummy3>();
			em.make<dummy3>();
			em.make<dummy2>();
			Assert::IsTrue(em.numEntities() == 0);
			em.update();
			Assert::IsTrue(em.numEntities() == 4);
		}
		TEST_METHOD(TestDeleteOnUpdate)
		{
			EntityManager em;
			em.make<dummy1>();
			em.make<dummy3>();
			em.make<dummy3>();
			Entity& e = em.make<dummy2>();
			em.update();
			Assert::IsTrue(em.numEntities() == 4);
			e.active = false;
			em.update();
			Assert::IsTrue(em.numEntities() == 3);
		}
		TEST_METHOD(TestGet)
		{
			EntityManager em;
			Entity& e = em.make<dummy4>();
			e.add<dummy1>().testInt = 5;
			em.make<dummy4>().add<dummy2>().testBool = 3;
			em.make<dummy4>().add<dummy3>();
			em.make<dummy4>().add<dummy3>();
			Assert::IsTrue(em.get<dummy3>().size() == 0);
			em.update();
			Assert::IsTrue(em.get<dummy3>().size() == 2);
			Assert::IsTrue(&(**(em.get<dummy1>().begin())) == &e);
		}
	};
}
