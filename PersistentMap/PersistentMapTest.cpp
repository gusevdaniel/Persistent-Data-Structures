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
		}
		TEST_METHOD(findTest)
		{
			PersistentMap<int, int> myMap;
			Assert::AreEqual(-842150451, myMap.find(0, 8));

			myMap.insert(0, 8, 11);
			Assert::AreEqual(11, myMap.find(0, 8));
		}
	};
}
