// 1. Создайте массив из 10 double в куче (new)
// 2. Заполните его значениями синуса: arr[i] = sin(i * 0.1)
// 3. Вычислите среднее значение
// 4. Не забудьте delete[]
#include "../include/math_utils.h"
using namespace std;

double* create_array() {
	double* arr = new double[ARRAY_LENGTH];
	for (int i = 0; i < ARRAY_LENGTH; i++) {
		arr[i] = sin(i * 0.1);
	}
	return arr;
}

double calc_average(double* arr) {
	double sum = 0;
	for (int i = 0; i < 10; i++) {
		sum += arr[i];
	}
	return sum / ARRAY_LENGTH;
}

void entrypoint() {
	double* arr = create_array();
	cout << "average = " << calc_average(arr);

	delete[] arr;
}