#include <iostream>
#include <vector>
#include <list>
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

	int get_size();
	const T& get(int i);
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
	int iter;

	persistentList<T>::act swap_data_part_undu(act box, bool undo);
};

template<class T>
persistentList<T>::persistentList()
{
	version = 0;
	use_version = 0;
	size = 0;
	iter_b = list_data.begin();
	iter = 0;
}


template<class T>
void persistentList<T>::set(const int i, const T& value)
{
	while (iter < i) {
		iter++;
		iter_b++;
	}

	while (iter > i) {
		iter--;
		iter_b--;
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
	iter_b = list_data.begin();
	iter = 0;
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
	iter_b = list_data.begin();
	iter = 0;
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
	iter_b = list_data.begin();
	iter = 0;
}

template<class T>
void persistentList<T>::insert(const int i, const T& value)
{
	iter_b = list_data.begin();
	iter = 0;
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
	box.past_elem = *iter_b;
	box.index_elem = i;
	box.kind_act = 7;
	version++;
	size--;
	use_version = version;
	box.version = version;
	box.undo = 0;
	vector_act.push_back(box);
	list_data.erase(iter_b);
	iter_b = list_data.begin();
	iter = 0;
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
int persistentList<T>::get_size()
{
	return size;
}

template<class T>
const T& persistentList<T>::get(int i) {
	typename list<T>::iterator it;
	it = list_data.begin();
	iter = 0;
	for (int j=0; j<i;j++, it++)
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
	act box = vector_act.back();
	vector_act.pop_back();
	//определение вида действия
	switch (box.kind_act)
	{
		case 1: {//set
			
			while (iter < box.index_elem) {
				iter++;
				iter_b++;
			}

			while (iter > box.index_elem) {
				iter--;
				iter_b--;
			}
			*iter_b = box.past_elem;

			act box_undo = swap_data_part_undu(box, 1);
			vector_undo.push_back(box_undo);
			break;
		}
		case 2:{//push_back
			list_data.pop_back();
			size--;

			act box_undo = swap_data_part_undu(box, 1);
			vector_undo.push_back(box_undo);
			break;
		}
		case 3: {//push_front
			list_data.pop_front();
			size--;

			act box_undo = swap_data_part_undu(box, 1);
			vector_undo.push_back(box_undo);
			iter_b = list_data.begin();
			iter = 0;
			break;
		}
		case 4: {//pop_back
			list_data.push_back(box.past_elem);
			size++;

			act box_undo = swap_data_part_undu(box, 1);
			vector_undo.push_back(box_undo);
			break;
		}
		case 5: {//pop_front
			list_data.push_front(box.past_elem);
			size++;

			act box_undo = swap_data_part_undu(box, 1);
			vector_undo.push_back(box_undo);
			iter_b = list_data.begin();
			iter = 0;
			break;
		}
		case 6: {//insert
			iter_b = list_data.begin();
			iter = 0;
			for (int j = 0; j < box.index_elem; j++, iter_b++)
			{
			}
			list_data.erase(iter_b);
			size--;

			act box_undo = swap_data_part_undu(box, 1);
			vector_undo.push_back(box_undo);
			break;
		}
		case 7: {//erase
			iter_b = list_data.begin();
			iter = 0;
			for (int j = 0; j < box.index_elem; j++, iter_b++)
			{
			}
			list_data.insert(iter_b, box.past_elem);
			size++;

			act box_undo = swap_data_part_undu(box, 1);
			vector_undo.push_back(box_undo);
			break;
		}
		case 8: {//clear
			act box_undo;

			box_undo.past_version = use_version;
			box_undo.version = use_version;
			box_undo.list_data = box.list_data;
			box_undo.kind_act = box.kind_act;
			box_undo.undo = 1;
			vector_undo.push_back(box_undo);

			use_version = box.past_version;
			list_data = box.list_data;
			size = list_data.size();
			break;
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
		iter_b = list_data.begin();
		for (int j = 0; j < box.index_elem; j++, iter_b++)
		{
		}
		*iter_b = box.past_elem;

		act box_redo = swap_data_part_undu(box, 0);
		vector_act.push_back(box_redo);
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
		iter_b = list_data.begin();
		for (int j = 0; j < box.index_elem; j++, iter_b++)
		{
		}
		list_data.insert(iter_b, box.past_elem);
		size++;

		act box_redo = swap_data_part_undu(box, 0);
		vector_act.push_back(box_redo);
		break;
	}
	case 7: {//erase
		iter_b = list_data.begin();
		for (int j = 0; j < box.index_elem; j++, iter_b++)
		{
		}
		list_data.erase(iter_b);
		size--;

		act box_redo = swap_data_part_undu(box, 0);
		vector_act.push_back(box_redo);
		break;
	}
	case 8: {//clear
		use_version = box.past_version;
		list_data.clear();
		size = 0;

		act box_redo;
		box_redo.past_version = use_version;
		box_redo.version = use_version;
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
	
	L.push_back(1);
	L.push_back(1);
	L.push_back(1);
	L.push_back(1);
	
	L.set(1, 2);
	L.set(3, 2);
	L.set(0, 2);

	return 0;
}

