
#include <iostream>
#include <vector>
using namespace std;

template <typename T>
struct Vers //структура хранение версий. т.е. тип данных узла
{
public:
	T value;
	int version = 0;
};

template <typename T>
class PerMass  //класс персистентный массив 
{
public:
	PerMass(int n);
	void inPutElement(int i, T value);
	T outPutElement(int i);
	T outPutElement(int i, int version);
private:
	int version; //актуальная версия структуры
	int SizePerMass; //размер массива
	vector <vector <Vers<T>>> vectorPerMass; //хранение данных. внутренний вектр это толстый узел
};

template <typename T>//конструктор с одним параметром
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

template <typename T>
void PerMass<T>::inPutElement(int i, T value)//ввод элемента
{
	Vers<T> box;
	version++;
	box.value = value;	
	box.version = version;
	vectorPerMass[i].push_back(box);
}

template <typename T>
T PerMass<T>::outPutElement(int i)//вывод элемента последней версии 
{
	
	Vers<T> box;
	box = vectorPerMass[i].back();//последняя версия
	return box.value;

}

//вывод элемента последней версии 
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

