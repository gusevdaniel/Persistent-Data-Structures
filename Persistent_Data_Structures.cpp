
#include <iostream>
#include <vector>
using namespace std;

//структура хранение версий. т.е. тип данных узла
template <typename T>
struct Vers 
{
public:
	T value;
	int version = 0;
	int parentVersion = -1;
	int numLastChangeElem = -1;
};

//класс персистентный массив
template <typename T>
class PerMass   
{
public:
	PerMass(int n);								//конструктор с одним параметром
	void inPutElement(int i, T value);			//ввод элемента
	T outPutElement(int i);						//вывод элемента последней версии 
	T outPutElement(int i, int version);		//вывод элемента выбранной версии 
	int getSize();								//получить размер массива
	void undo();
	T operator[](int i) const;					//перегрузка оператора[]
	T& operator=(const T& elem);				//перегрузка оператора = элемента

private:
	int version;							//последняя версия структуры, счетчик
	int useVersion;							//используемая версия (для undo/redo)
	int numLastChangeElem;					//номер последнего измененного элемента
	int SizePerMass;						//размер массива
	vector <vector <Vers<T>>> vectorPerMass;//хранение данных. внутренний вектр это толстый узел
};


//конструктор с одним параметром
template <typename T>
PerMass<T>::PerMass(int n)
{
	version = 0;
	useVersion = 0;
	numLastChangeElem = -1;				//не изменяли
	SizePerMass = n;
	vectorPerMass.resize(n, vector <Vers<T>>(1));

	for (int i = 0; i < n; i++) {
		vectorPerMass[i][0].version = 0;
		vectorPerMass[i][0].value = {};
		vectorPerMass[i][0].parentVersion = -1;
	}
}

//ввод элемента
template <typename T>
void PerMass<T>::inPutElement(int i, T value)
{
	if (i < 0 || i >= SizePerMass)
		exit(1);

	Vers<T> box;
	version++;
	box.value = value;	
	box.version = version;
	box.parentVersion = useVersion;
	box.numLastChangeElem = numLastChangeElem;

	useVersion = version;
	numLastChangeElem = i;
	vectorPerMass[i].push_back(box);
}

//вывод элемента последней версии 
template <typename T>
T PerMass<T>::outPutElement(int i)
{
	if (i < 0 || i >= SizePerMass)
		exit(1);
	if (useVersion == version) {
		Vers<T> box;
		box = vectorPerMass[i].back();//последняя версия
		return vectorPerMass[i].back().value;
	}
	else {
		return outPutElement(i, useVersion);
	}
}

//вывод элемента выбранной версии 
template <typename T>
T PerMass<T>::outPutElement(int i, int inVersion)
{
	if (i < 0 || i >= SizePerMass)
		exit(1);
	//версия не может быть маньше 0
	if (inVersion < 0) {
		exit(1);
	}
	//запрашиваемая версия не может быть больше чем послед.верс. массива
	if (inVersion > version) {
		exit(1);
	}

	int size = vectorPerMass[i].size() - 1;
	//как только inVersion больше чем 
	while (inVersion < vectorPerMass[i][size].version) {
		size--;
	}
	return vectorPerMass[i][size].value;
}



//получить размер массива
template <typename T>
int PerMass<T>::getSize() {
	return SizePerMass;
}

//перегрузка оператора[]
template <typename T>
T PerMass<T>::operator[](int i) const
{
	if (i < 0 || i >= SizePerMass)
		exit(1);
	
	return vectorPerMass[i].back().value;
}

//получить размер массива
template <typename T>
void PerMass<T>::undo() {
	if (useVersion == 0) {
		cout << "use version 0. You can't now use 'undo'" << endl;
		exit(1);
	}

	int size = vectorPerMass[numLastChangeElem].size() - 1;
	//как только inVersion больше чем 
	while (useVersion < vectorPerMass[numLastChangeElem][size].version) {
		size--;
	}

	Vers<T> box;
	box = vectorPerMass[numLastChangeElem][size];//последняя версияsize--;
	useVersion = box.parentVersion;						//смена используемой версии
	numLastChangeElem =box.numLastChangeElem;	//запись последнего измененного элемента в этой версии 
}

int main()
{
	
	PerMass<int> mass1(2);//выделение памяти
	/*cout << "output elemrnt 0: \t" << mass1.outPutElement(0) << endl;
	cout << "mass[0] = \t\t" << mass1[0] << endl;*/
	cout << "output elemrnt 0 : " << mass1.outPutElement(0) << endl;
	cout << "output elemrnt 1 : " << mass1.outPutElement(1) << endl;
	cout << "===========================" << endl;
	mass1.inPutElement(0, 1);
	cout << "output elemrnt 0 : " << mass1.outPutElement(0) << endl;
	cout << "output elemrnt 1 : " << mass1.outPutElement(1) << endl;
	cout << "===========================" << endl;
	mass1.inPutElement(1, 2);
	cout << "output elemrnt 0 : " << mass1.outPutElement(0) << endl;
	cout << "output elemrnt 1 : " << mass1.outPutElement(1) << endl;
	cout << "===========================" << endl;
	mass1.inPutElement(0, 3);
	cout << "output elemrnt 0 : " << mass1.outPutElement(0) << endl;
	cout << "output elemrnt 1 : " << mass1.outPutElement(1) << endl;
	cout << "===========================" << endl;
	cout << "===========================" << endl;
	cout << "===========================" << endl;
	mass1.undo();
	cout << "output elemrnt 0 : " << mass1.outPutElement(0) << endl;
	cout << "output elemrnt 1 : " << mass1.outPutElement(1) << endl;
	cout << "===========================" << endl;
	mass1.undo();
	cout << "output elemrnt 0 : " << mass1.outPutElement(0) << endl;
	cout << "output elemrnt 1 : " << mass1.outPutElement(1) << endl;
	cout << "===========================" << endl;
	mass1.undo();
	cout << "output elemrnt 0 : " << mass1.outPutElement(0) << endl;
	cout << "output elemrnt 1 : " << mass1.outPutElement(1) << endl;
	cout << "===========================" << endl;
	return 0;
}

