﻿#include <iostream>
#include <vector>
#include<list>
using namespace std;


template<class T>
class persistentList {
public:
	persistentList();
	void set(const int i, const T& value);
	void push_back(const T& value);
	void push_front(const T& value);
	void pop_back();
	void pop_front();
	void insert(const int i, const T& value);
	void erase(const int i);
	void clear();

	const T& get(int num);
	void undo();
	void redo();
private:
	/*
	0-redo 1 - set
	2-push_back 3-push_front()
	4 - pop_back, 5-pop_front
	6 - insert 7 - erase
	8 - clear
	*/
	class act {
	public:
		T past_elem;		//предыдущее значение элемента
		T present_elem;		//нынешнее значение
		int index_elem;		//индекс измененного элемента
		int kind_act;		//вид изменения 1-set 
		int past_version;
		int version;		//версия которую создают
		bool undo;
		list<T> list_data;
	};

	int version;
	int use_version;
	int size;
	vector<act> vector_act;
	vector<act> vector_undo;
	list<T> list_data;

	typename list<T>::iterator iter_b;

	persistentList<T>::act swap_data_part_undu(act box, bool undo);
};

template<class T>
persistentList<T>::persistentList()
{
	version = 0;
	use_version = 0;
	size = 0;
	iter_b = list_data.begin();
}


template<class T>
void persistentList<T>::set(const int i, const T& value)
{
	iter_b = list_data.begin();
	for (int j = 0; j < i; j++, iter_b++)
	{
	}
	act box;
	box.past_version = version;
	box.past_elem = *iter_b;
	box.present_elem = value;
	box.index_elem = i;
	box.kind_act = 1;
	version++;
	use_version = version;
	box.version = version;
	box.undo = 0;
	vector_act.push_back(box);
	*iter_b = value;
}

template<class T>
void persistentList<T>::push_back(const T& value)
{
	size++;
	act box;
	box.past_version = version;
	box.present_elem = value;
	box.index_elem = size-1;
	box.kind_act = 2;
	version++;
	
	use_version = version;
	box.version = version;
	box.undo = 0;
	vector_act.push_back(box);
	list_data.push_back(value);

}

template<class T>
void persistentList<T>::push_front(const T& value)
{
	act box;
	box.past_version = version;
	box.present_elem = value;
	box.index_elem = 0;
	box.kind_act = 3;
	version++;
	size++;
	use_version = version;
	box.version = version;
	box.undo = 0;
	vector_act.push_back(box);
	list_data.push_front(value);
}

template<class T>
void persistentList<T>::pop_back()
{
	act box;
	box.past_version = version;
	box.past_elem = list_data.back();
	box.index_elem = size-1;
	box.kind_act = 4;
	version++;
	size--;
	use_version = version;
	box.version = version;
	box.undo = 0;
	vector_act.push_back(box);
	list_data.pop_back();
}

template<class T>
void persistentList<T>::pop_front()
{
	act box;
	box.past_version = version;
	box.past_elem = list_data.front();
	box.kind_act = 5;
	version++;
	size--;
	use_version = version;
	box.version = version;
	box.undo = 0;
	vector_act.push_back(box);
	list_data.pop_front();
}

template<class T>
void persistentList<T>::insert(const int i, const T& value)
{
	iter_b = list_data.begin();
	for (int j = 0; j < i; j++, iter_b++)
	{
	}
	act box;
	box.past_version = version;
	box.present_elem = value;
	box.index_elem = i;
	box.kind_act = 6;
	version++;
	size++;
	use_version = version;
	box.version = version;
	box.undo = 0;
	vector_act.push_back(box);
	list_data.insert(iter_b , value);
}

template<class T>
void persistentList<T>::erase(const int i)
{
	iter_b = list_data.begin();
	for (int j = 0; j < i; j++, iter_b++)
	{
	}
	act box;
	box.past_version = version;
	box.index_elem = i;
	box.kind_act = 7;
	version++;
	size--;
	use_version = version;
	box.version = version;
	box.undo = 0;
	vector_act.push_back(box);
	list_data.erase(iter_b);
}

template<class T>
void persistentList<T>::clear()
{
	act box;
	box.past_version = version;

	box.kind_act = 8;
	version++;
	size=0;
	use_version = version;
	box.version = version;
	box.undo = 0;
	box.list_data = list_data;
	vector_act.push_back(box);
	list_data.clear();
}

template<class T>
const T& persistentList<T>::get(int num) {
	if (num < 0 || num >= size) {
		exit(1);
	}
	typename list<T>::iterator it;
	it = list_data.begin();
	for (int i=0; i<num;i++, it++)
	{
	}
	return *it;
}

template<class T>
typename persistentList<T>::act persistentList<T>::swap_data_part_undu(act box, bool undo) {
	act box_undo;

	box_undo.past_version = use_version;
	use_version = box.past_version;
	box_undo.version = use_version;

	box_undo.past_elem = box.present_elem;
	box_undo.present_elem = box.past_elem;
	box_undo.index_elem = box.index_elem;
	box_undo.kind_act = box.kind_act;
	box_undo.undo = undo;
	return box_undo;
}

template<class T>
void persistentList<T>::undo()
{
	int i = vector_act.size()-1;
	act box = vector_act[i];

	//определение вида действия
	switch (box.kind_act)
	{
		case 1: {//set
			vector_act.erase(vector_act.begin() + i);
			act box_undo = swap_data_part_undu(box, 1);
			vector_undo.push_back(box_undo);

			iter_b = list_data.begin();
			for (int j = 0; j < box.index_elem; j++, iter_b++)
			{
			}
			*iter_b = box.past_elem;
			break;
		}
		case 2:{//push_back
			vector_act.erase(vector_act.begin() + i);
			size--;
			act box_undo = swap_data_part_undu(box, 1);
			vector_undo.push_back(box_undo);
			list_data.pop_back();
			break;
		}
		case 3: {//push_front
			vector_act.erase(vector_act.begin() + i);
			size--;
			act box_undo = swap_data_part_undu(box, 1);
			vector_undo.push_back(box_undo);
			list_data.pop_front();
			break;
		}
		case 4: {//pop_back
			vector_act.erase(vector_act.begin() + i);
			size++;
			act box_undo = swap_data_part_undu(box, 1);
			vector_undo.push_back(box_undo);
			list_data.push_back(box.past_elem);
			break;
		}
		case 5: {//pop_front
			vector_act.erase(vector_act.begin() + i);
			size++;
			act box_undo = swap_data_part_undu(box, 1);
			vector_undo.push_back(box_undo);
			list_data.push_front(box.past_elem);
			break;
		}
		case 6: {//insert
			vector_act.erase(vector_act.begin() + i);
			size--;
			act box_undo = swap_data_part_undu(box, 1);
			vector_undo.push_back(box_undo);
			iter_b = list_data.begin();
			for (int j = 0; j < box.index_elem; j++, iter_b++)
			{
			}
			list_data.erase(iter_b);
			break;
		}
		case 8: {
			vector_act.erase(vector_act.begin() + i);

			act box_undo;
			box_undo.past_version = use_version;
			use_version = box.past_version;
			box_undo.version = use_version;

			list_data = box.list_data;
			size = list_data.size();

			box_undo.list_data = box.list_data;
			box_undo.kind_act = box.kind_act;
			box_undo.undo = 1;
		}
	default:
		break;
	}
}


template<class T>
void persistentList<T>::redo()
{
	act box = vector_undo.back();
	vector_undo.pop_back();
	//определение вида действия
	switch (box.kind_act)
	{
	case 1: {//set
		act box_redo = swap_data_part_undu(box, 0);
		vector_act.push_back(box_redo);
		iter_b = list_data.begin();
		for (int j = 0; j < box.index_elem; j++, iter_b++)
		{
		}
		*iter_b = box.past_elem;
		break;
	}
	case 2: {//push_back
		list_data.push_back(box.past_elem);
		size++;
		act box_redo = swap_data_part_undu(box, 0);
		vector_act.push_back(box_redo);
		break;
	}
	case 3: {//push_front
		list_data.push_front(box.past_elem);
		size++;
		act box_redo = swap_data_part_undu(box, 0);
		vector_act.push_back(box_redo);
		break;
	}
	case 4: {//pop_back
		list_data.pop_back();
		size--;
		act box_redo = swap_data_part_undu(box, 0);
		vector_act.push_back(box_redo);
		break;
	}
	case 5: {//pop_front
		list_data.pop_front();
		size--;
		act box_redo = swap_data_part_undu(box, 0);
		vector_act.push_back(box_redo);
		break;
	}
	case 6: {//insert
		size++;
		act box_redo = swap_data_part_undu(box, 0);
		vector_act.push_back(box_redo);
		iter_b = list_data.begin();
		for (int j = 0; j < box.index_elem; j++, iter_b++)
		{
		}
		list_data.insert(iter_b, box.past_elem);
		break;
	}
	case 8: {
		act box_redo;
		box_redo.past_version = use_version;
		use_version = box.past_version;
		box_redo.version = use_version;

		list_data.clear();
		size = 0;

		box_redo.list_data = box.list_data;
		box_redo.undo = 0;
	}
	default:
		break;
	}


}

int main()
{
	persistentList<int> L;
	persistentList<persistentList<int>> M;
	L.insert(0,1);
	L.insert(1,2);
	L.insert(2,3);
	
	M.push_back(L);
	L.clear();
	L.undo();

	return 0;
}

