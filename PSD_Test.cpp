#include "pch.h"
#include "CppUnitTest.h"
#include "..\PSD\PSD.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PSDTest
{
	TEST_CLASS(PersistentMassTest)
	{
	public:
		TEST_METHOD(set_test) {
			persistentMass<int> myList;

			for (int i = 0; i < 10; i++)
				myList.push_back(i);

			myList.set(0, 10);
			Assert::AreEqual(10, myList.get_size());
			Assert::AreEqual(10, myList.get(0));

			myList.undo();
			Assert::AreEqual(10, myList.get_size());
			Assert::AreEqual(0, myList.get(0));

			myList.redo();
			Assert::AreEqual(10, myList.get_size());
			Assert::AreEqual(10, myList.get(0));
		}

		TEST_METHOD(pushback_test) {
			persistentMass<int> myList;

			for (int i = 0; i < 10; i++)
				myList.push_back(i);

			for (int i = 0; i < 10; i++)
				Assert::AreEqual(i, myList.get(i));
			Assert::AreEqual(10, myList.get_size());

			for (int i = 0; i < 10; i++)
				myList.undo();
			Assert::AreEqual(0, myList.get_size());

			for (int i = 0; i < 10; i++)
				myList.redo();
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(i, myList.get(i));
			Assert::AreEqual(10, myList.get_size());
		}


		TEST_METHOD(popback_test) {
			persistentMass<int> myList;

			for (int i = 0; i < 10; i++)
				myList.push_back(i);

			for (int i = 0; i < 10; i++)
				myList.pop_back();
			Assert::AreEqual(0, myList.get_size());


			for (int i = 0; i < 10; i++)
				myList.undo();
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(i, myList.get(i));
			Assert::AreEqual(10, myList.get_size());

			for (int i = 0; i < 10; i++)
				myList.redo();
			Assert::AreEqual(0, myList.get_size());
		}

		TEST_METHOD(insert_test) {
			persistentMass<int> myList;

			for (int i = 0; i < 10; i++)
				myList.push_back(i);

			for (int i = 0; i < 10; i++)
				myList.insert(i, 0);
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(0, myList.get(i));

			for (int i = 0; i < 10; i++)
				myList.undo();
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(i, myList.get(i));

			for (int i = 0; i < 10; i++)
				myList.redo();
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(0, myList.get(i));
		}

		TEST_METHOD(erase_test) {
			persistentMass<int> myList;

			for (int i = 0; i < 10; i++)
				myList.push_back(i);

			for (int i = 9; i >= 0; i--)
				myList.erase(i);
			Assert::AreEqual(0, myList.get_size());

			for (int i = 0; i < 10; i++)
				myList.undo();
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(i, myList.get(i));
			Assert::AreEqual(10, myList.get_size());

			for (int i = 0; i < 10; i++)
				myList.redo();
			Assert::AreEqual(0, myList.get_size());
		}

		TEST_METHOD(clear_test) {
			persistentMass<int> myList;

			for (int i = 0; i < 10; i++)
				myList.push_back(i);

			myList.clear();
			Assert::AreEqual(0, myList.get_size());

			myList.undo();
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(i, myList.get(i));
			Assert::AreEqual(10, myList.get_size());

			myList.redo();
			Assert::AreEqual(0, myList.get_size());
		}

		TEST_METHOD(get_size_test) {
			persistentMass<int> myList;

			for (int i = 0; i < 10; i++)
				myList.push_back(i);

			Assert::AreEqual(10, myList.get_size());
		}


		TEST_METHOD(get_test) {
			persistentMass<int> myList;

			for (int i = 0; i < 10; i++)
				myList.push_back(i);

			Assert::AreEqual(9, myList.get(9));
		}
	};

	TEST_CLASS(Persistentlisttest)
	{
	public:
		TEST_METHOD(set_test) {
			persistentList<int> myList;

			for (int i = 0; i < 10; i++)
				myList.push_back(i);

			myList.set(0, 10);
			Assert::AreEqual(10, myList.get_size());
			Assert::AreEqual(10, myList.get(0));

			myList.undo();
			Assert::AreEqual(10, myList.get_size());
			Assert::AreEqual(0, myList.get(0));

			myList.redo();
			Assert::AreEqual(10, myList.get_size());
			Assert::AreEqual(10, myList.get(0));
		}

		TEST_METHOD(pushfront_test) {
			persistentList<int> myList;

			for (int i = 9; i >= 0; i--)
				myList.push_front(i);

			for (int i = 0; i < 10; i++)
				Assert::AreEqual(i, myList.get(i));
			Assert::AreEqual(10, myList.get_size());

			for (int i = 0; i < 10; i++)
				myList.undo();
			Assert::AreEqual(0, myList.get_size());

			for (int i = 0; i < 10; i++)
				myList.redo();
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(i, myList.get(i));
			Assert::AreEqual(10, myList.get_size());
		}

		TEST_METHOD(pushback_test) {
			persistentList<int> myList;

			for (int i = 0; i < 10; i++)
				myList.push_back(i);

			for (int i = 0; i < 10; i++)
				Assert::AreEqual(i, myList.get(i));
			Assert::AreEqual(10, myList.get_size());

			for (int i = 0; i < 10; i++)
				myList.undo();
			Assert::AreEqual(0, myList.get_size());

			for (int i = 0; i < 10; i++)
				myList.redo();
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(i, myList.get(i));
			Assert::AreEqual(10, myList.get_size());
		}

		TEST_METHOD(popfront_test) {
			persistentList<int> myList;

			for (int i = 0; i < 10; i++)
				myList.push_back(i);

			for (int i = 0; i < 10; i++)
				myList.pop_front();
			Assert::AreEqual(0, myList.get_size());


			for (int i = 0; i < 10; i++)
				myList.undo();
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(i, myList.get(i));
			Assert::AreEqual(10, myList.get_size());

			for (int i = 0; i < 10; i++)
				myList.redo();
			Assert::AreEqual(0, myList.get_size());
		}

		TEST_METHOD(popback_test) {
			persistentList<int> myList;

			for (int i = 0; i < 10; i++)
				myList.push_back(i);

			for (int i = 0; i < 10; i++)
				myList.pop_back();
			Assert::AreEqual(0, myList.get_size());


			for (int i = 0; i < 10; i++)
				myList.undo();
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(i, myList.get(i));
			Assert::AreEqual(10, myList.get_size());

			for (int i = 0; i < 10; i++)
				myList.redo();
			Assert::AreEqual(0, myList.get_size());
		}

		TEST_METHOD(insert_test) {
			persistentList<int> myList;

			for (int i = 0; i < 10; i++)
				myList.push_back(i);

			for (int i = 0; i < 10; i++)
				myList.insert(i, 0);
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(0, myList.get(i));

			for (int i = 0; i < 10; i++)
				myList.undo();
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(i, myList.get(i));

			for (int i = 0; i < 10; i++)
				myList.redo();
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(0, myList.get(i));
		}

		TEST_METHOD(erase_test) {
			persistentList<int> myList;

			for (int i = 0; i < 10; i++)
				myList.push_back(i);

			for (int i = 9; i >= 0; i--)
				myList.erase(i);
			Assert::AreEqual(0, myList.get_size());

			for (int i = 0; i < 10; i++)
				myList.undo();
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(i, myList.get(i));
			Assert::AreEqual(10, myList.get_size());

			for (int i = 0; i < 10; i++)
				myList.redo();
			Assert::AreEqual(0, myList.get_size());
		}

		TEST_METHOD(clear_test) {
			persistentList<int> myList;

			for (int i = 0; i < 10; i++)
				myList.push_back(i);

			myList.clear();
			Assert::AreEqual(0, myList.get_size());

			myList.undo();
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(i, myList.get(i));
			Assert::AreEqual(10, myList.get_size());

			myList.redo();
			Assert::AreEqual(0, myList.get_size());
		}

		TEST_METHOD(get_size_test) {
			persistentList<int> myList;

			for (int i = 0; i < 10; i++)
				myList.push_back(i);

			Assert::AreEqual(10, myList.get_size());
		}


		TEST_METHOD(get_test) {
			persistentList<int> myList;

			for (int i = 0; i < 10; i++)
				myList.push_back(i);

			Assert::AreEqual(9, myList.get(9));
		}
	};
}
