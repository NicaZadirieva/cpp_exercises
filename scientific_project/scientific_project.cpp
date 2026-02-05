// scientific_project.cpp: определяет точку входа для приложения.


#include "scientific_project.h"
using namespace std;

int main()
{
	//
	// Имеется вектор зашумленного сигнала
	vector<double> signal(100);
	generate(signal.begin(), signal.end(),
		[n = 0]() mutable { return sin(n++ * 0.1) + 0.1 * (rand() / (double)RAND_MAX - 0.5); });

	// 1. Примените transform для вычисления квадрата каждого значения
	// 2. Используйте accumulate для вычисления энергии сигнала
	// 3. Используйте remove_if для удаления значений ниже порога 0.01
	// 4. Вычислите новые min/max после фильтрации
	return 0;
}
