// scientific_project.cpp: определяет точку входа для приложения.


#include "scientific_project.h"
#include "include/statistics.h"
using namespace std;

void print_vector(vector<double>& v) {
	copy(v.begin(), v.end(), ostream_iterator<double>(cout, " "));
}

int main()
{
	// Тест 1: vector<double>
	std::vector<double> double_data = { 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5 };
	testAndPrintStats(double_data, "vector<double>");
	return 0;
}
