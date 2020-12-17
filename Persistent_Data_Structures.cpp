
#include <iostream>
#include <vector>
using namespace std;




//класс персистентный массив
template <class T>
class PersistMass
{
public:
	//структура хранение версий. т.е. тип данных узла
	class FatNodes_elem
	{
	public:
		T value = 0 ;
		int version = 0;
	};

	PersistMass();
	PersistMass(int n);							//конструктор с одним параметром
	void set(int i, T value);					//ввод элемента
	T get(int i);								//вывод элемента последней версии 
	T get(int i, int version);					//вывод элемента выбранной версии 
	int getSize();								//получить размер массива
	void add(T value);							//добавить элемент в массив
	void empty();								//опустошить массив
	T& operator[](const int i);					//перегрузка оператора[]	
	
protected:
	int version;								//последняя версия структуры, счетчик
	int useVersion;								//используемая версия (для undo/redo)
	bool inItSelf;								//вложенный сам в себя

	int maxSize;								//максимальный размер массива
	int SizePersistMass;						//размер массива этой версии 
	vector <vector <FatNodes_elem>> vectorFatNode;//хранение данных. внутренний вектр это толстый узел
};

//конструктор без параметров
template <class T>
PersistMass<T>::PersistMass()
{
	version = 0;
	useVersion = 0;

	SizePersistMass = 0;
	vectorFatNode.resize(0);
	//vectorFatNode;
}

//конструктор с одним параметром
//template <class T>
//PersistMass<PersistMass<T>>::PersistMass(int n)
//{
//
//}
//конструктор с одним параметром
template <class T>
PersistMass<T>::PersistMass(int n)
{
	version = 0;
	useVersion = 0;
	SizePersistMass = n;
	vectorFatNode.resize(n);

	if (typeid(T) == typeid(PersistMass<int>)) {
		inItSelf = true;
		FatNodes_elem box;
		for (int i = 0; i < n; i++) {
			vectorFatNode[i].push_back(box);
		}
	}
	else {
		inItSelf = false;
		FatNodes_elem box;
		for (int i = 0; i < n; i++) {
			box.version = 0;
			box.value = {};
			vectorFatNode[i].push_back(box);
		}
	}
}

//ввод элемента
template <class T>
void PersistMass<T>::set(int i, T value)
{
	if (i < 0 || i >= SizePersistMass)
		exit(1);
	version++;

	FatNodes_elem box;
	//расширяем узел новым значением
	box.value = value;
	box.version = version;

	useVersion = version;
	vectorFatNode[i].push_back(box);
}

//вывод элемента последней версии 
template <class T>
T PersistMass<T>::get(int i)
{
	if (i < 0 || i >= SizePersistMass)
		exit(1);
	if (useVersion == version) {
		return vectorFatNode[i].back().value;
	}
	else {
		return get(i, useVersion);
	}
}

//вывод элемента выбранной версии 
template <class T>
T PersistMass<T>::get(int i, int inVersion)
{
	if (i < 0 || i >= SizePersistMass)
		exit(1);
	//версия не может быть маньше 0
	if (inVersion < 0) {
		exit(1);
	}
	//запрашиваемая версия не может быть больше чем послед.верс. массива
	if (inVersion > version) {
		exit(1);
	}

	int size = vectorFatNode[i].size() - 1;
	//как только inVersion больше чем 
	while (inVersion < vectorFatNode[i][size].version) {
		size--;
	}
	return vectorFatNode[i][size].value;
}

//получить размер массива
template <class T>
int PersistMass<T>::getSize() {
	return SizePersistMass;
}

//перегрузка оператора[]
template <class T>
T& PersistMass<T>::operator[](const int i)
{
	if (i < 0 || i >= SizePersistMass)
		exit(1);

	return vectorFatNode[i].back().value;
}

template <class T>
void PersistMass<T>::add(T value) 
{
	
	SizePersistMass++;
	version++;
	useVersion = version;
	FatNodes_elem box;
	vector <FatNodes_elem> vect_box;
	box.version = version;
	box.value = value;
	vect_box.push_back(box);
	vectorFatNode.insert(vectorFatNode.begin(),vect_box);
}

template<class T>
void PersistMass<T>::empty()
{
	this->SizePersistMass = 0;
}

////перегрузка оператора = элемента
//template <class T>
//FatNodes_elem<T>& PersistMass<T>::operator=FatNodes_elem<T>(const T& elem) {
//
//	return elem.version;
//}

int main()
{

	PersistMass<int> mass1(2);//выделение памяти
	mass1.add(2);
	mass1.set(2, 3);
	mass1.empty();
	mass1.add(3);
	//PersistMass<int> mass(mass1);//выделение памяти

	//PersistMass <PersistMass<int>> mass2(2);//выделение памяти
	//mass2.set(0, mass1);

	/*cout << "used 'set' :" << endl;
	cout << "output elemrnt 0 : " << mass1[0] << endl;
	cout << "output elemrnt 1 : " << mass1.get(1) << endl;
	cout << "===========================" << endl;
	 
	mass1.set(0, 1);
	cout << "output elemrnt 0 : " << mass1[0] << endl;
	cout << "output elemrnt 1 : " << mass1.get(1) << endl;
	cout << "===========================" << endl;*/
	//mass1.set(1, 2);
	//cout << "output elemrnt 0 : " << mass1[0] << endl;
	//cout << "output elemrnt 1 : " << mass1.get(1) << endl;
	//cout << "===========================" << endl;
	//mass1.set(0, 3);
	////mass1[0] = 0;
	//cout << "output elemrnt 0 : " << mass1[0] << endl;
	//cout << "output elemrnt 1 : " << mass1.get(1) << endl;
	//cout << "===========================\n" << endl;


	return 0;
}

