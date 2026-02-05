// scientific_project.cpp: определяет точку входа для приложения.


#include "scientific_project.h"
using namespace std;

void print_vector(vector<double>& v) {
	copy(v.begin(), v.end(), ostream_iterator<double>(cout, " "));
}

int main()
{
	//
	// Имеется вектор зашумленного сигнала
	vector<double> signal(100);
	generate(signal.begin(), signal.end(),
		[n = 0]() mutable { return sin(n++ * 0.1) + 0.1 * (rand() / (double)RAND_MAX - 0.5); });
	cout << "Signal list: " << endl;
	print_vector(signal);
	cout << endl;
	// 1. Примените transform для вычисления квадрата каждого значения
	vector<double> transformed_signal = signal;
	transform(transformed_signal.begin(), transformed_signal.end(), transformed_signal.begin(),
		[](double x) { return x * x; });
	cout << "Tranformed Signal list: " << endl;
	print_vector(transformed_signal);
	cout << endl;
	// 2. Используйте accumulate для вычисления энергии сигнала
	double energy = accumulate(transformed_signal.begin(), transformed_signal.end(), 0.0);
	cout << "Energy = " << energy << endl;
	// 3. Используйте remove_if для удаления значений ниже порога 0.01
	remove_if(signal.begin(), signal.end(), [](double x) { return x < 0.01; });
	cout << "Signal list after removing x < 0.01: " << endl;
	print_vector(signal);
	cout << endl;
	// 4. Вычислите новые min/max после фильтрации
	auto [minSignal, maxSignal] = minmax_element(signal.begin(), signal.end());
	cout << "min new signal = " << *minSignal << endl;
	cout << "max new signal = " << *maxSignal << endl;
	return 0;
}
