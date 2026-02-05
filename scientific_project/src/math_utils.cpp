// Дана строка: "3.14,2.71,1.41,0.577"
// 1. Разделите её на числа с помощью std::getline и std::stringstream
// 2. Сохраните в vector<double>
// 3. Найдите максимальное и минимальное значение
#include "../include/math_utils.h"

using namespace std;

void print_vector(vector<double> numbers) {
    for (double number : numbers) {
        cout << number << endl;
    }
}

void entrypoint() {
    string str = "3.14,2.71,1.41,0.577";
    stringstream s(str);
    string num;
    vector<double> numbers = {};
    while (getline(s, num, ',')) {
        numbers.push_back(stod(num));
    }
    print_vector(numbers);
}