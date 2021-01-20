#include "pch.h"
#include "CppUnitTest.h"
#include "..\PersistentDataStructures\PersistentMap.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PersistentMapTest
{
	TEST_CLASS(PersistentMapTest)
	{
	public:
		TEST_METHOD(sizeTest)
		{
			PersistentMap<int, int> myMap;
			Assert::AreEqual(0, myMap.size());
		}

		TEST_METHOD(insertTest)
		{
			PersistentMap<int, int> myMap;
			Assert::AreEqual(0, myMap.size());

			myMap.insert(0, 8, 11);
			Assert::AreEqual(1, myMap.size());

			myMap.insert(1, 5, 22);
			Assert::AreEqual(2, myMap.size());

			myMap.insert(0, 8, 11);
			Assert::AreEqual(2, myMap.size());

			myMap.insert(0, 8, 88);
			Assert::AreEqual(3, myMap.size());

			myMap.insert(17, 47);
			Assert::AreEqual(4, myMap.size());
		}

		TEST_METHOD(getTest)
		{
			PersistentMap<int, int> myMap;
			Assert::AreEqual(-842150451, myMap.get(0, 8));

			myMap.insert(0, 8, 11);
			Assert::AreEqual(11, myMap.get(0, 8));

			Assert::AreEqual(11, myMap.get(8));

			Assert::AreEqual(-842150451, myMap.get(0, 10));
			Assert::AreEqual(-842150451, myMap.get(1, 8));
		}

		TEST_METHOD(deleteKeyTest)
		{
			PersistentMap<int, int> myMap;
			myMap.insert(0, 8, 11);
			myMap.insert(1, 5, 22);
			myMap.insert(2, 18, 28);
			myMap.insert(3, 15, 35);
			myMap.insert(4, 17, 47);
			myMap.insert(5, 25, 55);
			myMap.insert(6, 40, 60);
			myMap.insert(7, 80, 78);
			myMap.insert(7, 90, 88);
			myMap.insert(7, 8, 88);
			Assert::AreEqual(10, myMap.size());

			myMap.deleteKey(7, 25);
			Assert::AreEqual(11, myMap.size());

			myMap.deleteKey(7, 35);
			Assert::AreEqual(11, myMap.size());
		}

		TEST_METHOD(undoTest)
		{
			PersistentMap<int, int> myMap;
			myMap.insert(8, 11);
			myMap.insert(5, 22);
			myMap.insert(18, 28);
			myMap.insert(15, 35);

			Assert::AreEqual(35, myMap.get(15));

			myMap.undo();
			Assert::AreEqual(-842150451, myMap.get(15));

		}

		TEST_METHOD(redoTest)
		{
			PersistentMap<int, int> myMap;
			myMap.insert(8, 11);
			myMap.insert(5, 22);
			myMap.insert(18, 28);
			myMap.insert(15, 35);

			Assert::AreEqual(35, myMap.get(15));
			myMap.redo();
			Assert::AreEqual(35, myMap.get(15));

			myMap.undo();
			Assert::AreEqual(-842150451, myMap.get(15));

			myMap.redo();
			Assert::AreEqual(35, myMap.get(15));

		}
	};
}
