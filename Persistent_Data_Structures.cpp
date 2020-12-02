
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
};

//класс персистентный массив
template <typename T>
class PerMass   
{
public:
	PerMass(int n);							//конструктор с одним параметром
	void inPutElement(int i, T value);		//ввод элемента
	T outPutElement(int i);					//вывод элемента последней версии 
	T outPutElement(int i, int version);	//вывод элемента выбранной версии 
private:
	int version;							//актуальная версия структуры
	int SizePerMass;						//размер массива
	vector <vector <Vers<T>>> vectorPerMass;//хранение данных. внутренний вектр это толстый узел
};

//конструктор с одним параметром
template <typename T>
PerMass<T>::PerMass(int n)
{
	version = 0;
	SizePerMass = n;
	vectorPerMass.resize(n, vector <Vers<T>>(1));

	for (int i = 0; i < n; i++) {
		vectorPerMass[i][0].version = 0;
		vectorPerMass[i][0].value = {};
	}
}

//ввод элемента
template <typename T>
void PerMass<T>::inPutElement(int i, T value)
{
	Vers<T> box;
	version++;
	box.value = value;	
	box.version = version;
	vectorPerMass[i].push_back(box);
}

//вывод элемента последней версии 
template <typename T>
T PerMass<T>::outPutElement(int i)
{
	
	Vers<T> box;
	box = vectorPerMass[i].back();//последняя версия
	return box.value;

}

//вывод элемента выбранной версии 
template <typename T>
T PerMass<T>::outPutElement(int i, int inVersion)
{
	//версия не может быть маньше 0
	if (inVersion < 0) {
		exit(1);
	}
	//запрашиваемая версия не может быть больше чем послед.верс. массива
	if (inVersion > version) {
		exit(1);
	}
	Vers<T> box;
	int size = vectorPerMass[i].size() - 1;
	//как только inVersion больше чем 
	while (inVersion < vectorPerMass[i][size].version) {
		size--;
	}

	return vectorPerMass[i][size].value;
}

int main()
{
	
	PerMass<double> mass1(2);//выделение памяти
	cout << "output elemrnt 0: " << mass1.outPutElement(0) << endl;
	mass1.inPutElement(0, 0.5);
	cout << "output elemrnt 0: "  << mass1.outPutElement(0) << endl;
	cout << "output elemrnt 0 version 0: " << mass1.outPutElement(0 , 0) << endl;
	cout << "output elemrnt 0 version 1: " << mass1.outPutElement(0 , 1) << endl;

	return 0;
}

