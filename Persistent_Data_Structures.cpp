
#include <iostream>
#include <vector>
using namespace std;

//структура хранение версий. т.е. тип данных узла
template <typename T>
struct FatNode 
{
public:
	T value;
	int version = 0;
	int parentVersion = -1;
	int numLastChangeElem = -1;
};

//класс персистентный массив
template <typename T>
class PersistMass   
{
public:
	PersistMass(int n);								//конструктор с одним параметром
	void set(int i, T value);					//ввод элемента
	T get(int i);								//вывод элемента последней версии 
	T get(int i, int version);					//вывод элемента выбранной версии 
	int getSize();								//получить размер массива
	void undo();

	T operator[](int i) const;					//перегрузка оператора[]
	T& operator=(const T& elem);				//перегрузка оператора = элемента

private:
	int version;							//последняя версия структуры, счетчик
	int useVersion;							//используемая версия (для undo/redo)
	int numLastChangeElem;					//номер последнего измененного элемента
	int SizePersistMass;						//размер массива
	vector <vector <FatNode<T>>> vectorPersistMass;//хранение данных. внутренний вектр это толстый узел
};

//конструктор с одним параметром
template <typename T>
PersistMass<T>::PersistMass(int n)
{
	version = 0;
	useVersion = 0;
	numLastChangeElem = -1;				//не изменяли
	SizePersistMass = n;
	vectorPersistMass.resize(n, vector <FatNode<T>>(1));

	for (int i = 0; i < n; i++) {
		vectorPersistMass[i][0].version = 0;
		vectorPersistMass[i][0].value = {};
		vectorPersistMass[i][0].parentVersion = -1;
	}
}

//ввод элемента
template <typename T>
void PersistMass<T>::set(int i, T value)
{
	if (i < 0 || i >= SizePersistMass)
		exit(1);

	FatNode<T> box;
	version++;
	box.value = value;	
	box.version = version;
	box.parentVersion = useVersion;
	box.numLastChangeElem = numLastChangeElem;

	useVersion = version;
	numLastChangeElem = i;
	vectorPersistMass[i].push_back(box);
}

//вывод элемента последней версии 
template <typename T>
T PersistMass<T>::get(int i)
{
	if (i < 0 || i >= SizePersistMass)
		exit(1);
	if (useVersion == version) {
		FatNode<T> box;
		box = vectorPersistMass[i].back();//последняя версия
		return vectorPersistMass[i].back().value;
	}
	else {
		return get(i, useVersion);
	}
}

//вывод элемента выбранной версии 
template <typename T>
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

	int size = vectorPersistMass[i].size() - 1;
	//как только inVersion больше чем 
	while (inVersion < vectorPersistMass[i][size].version) {
		size--;
	}
	return vectorPersistMass[i][size].value;
}

//получить размер массива
template <typename T>
int PersistMass<T>::getSize() {
	return SizePersistMass;
}

//перегрузка оператора[]
template <typename T>
T PersistMass<T>::operator[](int i) const
{
	if (i < 0 || i >= SizePersistMass)
		exit(1);
	
	return vectorPersistMass[i].back().value;
}

//получить размер массива
template <typename T>
void PersistMass<T>::undo() {
	if (useVersion == 0) {
		cout << "use version 0. You can't now use 'undo'" << endl;
		exit(1);
	}

	int size = vectorPersistMass[numLastChangeElem].size() - 1;
	//как только inVersion больше чем 
	while (useVersion < vectorPersistMass[numLastChangeElem][size].version) {
		size--;
	}

	FatNode<T> box;
	box = vectorPersistMass[numLastChangeElem][size];//последняя версияsize--;
	useVersion = box.parentVersion;						//смена используемой версии
	numLastChangeElem =box.numLastChangeElem;	//запись последнего измененного элемента в этой версии 
}

int main()
{
	PersistMass<int> mass1(2);//выделение памяти
	/*cout << "output elemrnt 0: \t" << mass1.outPutElement(0) << endl;
	cout << "mass[0] = \t\t" << mass1[0] << endl;*/
	cout << "output elemrnt 0 : " << mass1.get(0) << endl;
	cout << "output elemrnt 1 : " << mass1.get(1) << endl;
	cout << "===========================" << endl;
	mass1.set(0, 1);
	cout << "output elemrnt 0 : " << mass1.get(0) << endl;
	cout << "output elemrnt 1 : " << mass1.get(1) << endl;
	cout << "===========================" << endl;
	mass1.set(1, 2);
	cout << "output elemrnt 0 : " << mass1.get(0) << endl;
	cout << "output elemrnt 1 : " << mass1.get(1) << endl;
	cout << "===========================" << endl;
	mass1.set(0, 3);
	cout << "output elemrnt 0 : " << mass1.get(0) << endl;
	cout << "output elemrnt 1 : " << mass1.get(1) << endl;
	cout << "===========================" << endl;
	cout << "===========================" << endl;
	cout << "===========================" << endl;
	mass1.undo();
	cout << "output elemrnt 0 : " << mass1.get(0) << endl;
	cout << "output elemrnt 1 : " << mass1.get(1) << endl;
	cout << "===========================" << endl;
	mass1.undo();
	cout << "output elemrnt 0 : " << mass1.get(0) << endl;
	cout << "output elemrnt 1 : " << mass1.get(1) << endl;
	cout << "===========================" << endl;
	mass1.undo();
	cout << "output elemrnt 0 : " << mass1.get(0) << endl;
	cout << "output elemrnt 1 : " << mass1.get(1) << endl;
	cout << "===========================" << endl;
	return 0;
}

