// 1. Создайте vector<double> measurements = {12.5, 13.2, 11.9, 12.8, 13.1, 100.0}
// 2. Удалите "выбросы" - значения, отличающиеся от медианы более чем на 3 сигмы
// 3. Отсортируйте оставшиеся значения
// 4. Вычислите среднее и стандартное отклонение
#include "../include/math_utils.h"

using namespace std;

void print_vector(vector<double> numbers) {
    for (double number : numbers) {
        cout << number << endl;
    }
}

double calc_mediana(vector<double> numbers) {
    vector<double> sorted_numbers = numbers;
    sort(sorted_numbers.begin(), sorted_numbers.end());
    const int vector_size = sorted_numbers.size();
    if (vector_size % 2 == 0) {
        return (double)(sorted_numbers[(vector_size - 1) / 2] + sorted_numbers[vector_size / 2]) / 2.0;
    }
    else {
        return (double)sorted_numbers[vector_size / 2];
    }
}
 
vector<double> remove_outliers(vector<double> numbers, double sigma) {
    double mediana = calc_mediana(numbers);
    vector<double> res = {};
    for (double number : numbers) {
        if (abs(number - mediana) <= 3 * sigma) {
            res.push_back(number);
        }
    }
    sort(res.begin(), res.end());
    return res;
}

double calc_standard_deviation(const vector<double>& numbers) {
    double sum = 0.0, mean, standardDeviation = 0.0;
    int size = numbers.size();
    // Вычислить сумму элементов в векторе
    for (int i = 0; i < size; ++i) {
        sum += numbers[i];
    }
    // Вычислить среднее значение
    mean = sum / size;
    // Вычислить сумму квадратов отклонений от среднего значения
    for (int i = 0; i < size; ++i) {
        standardDeviation += pow(numbers[i] - mean, 2);
    }
    // Вычислить квадратный корень из дисперсии, чтобы получить стандартное отклонение
    return sqrt(standardDeviation / size);
}


void entrypoint() {
    vector<double> measurements = { 12.5, 13.2, 11.9, 12.8, 13.1, 100.0 };
    vector<double> processed_measurements = remove_outliers(measurements, 0.1);
    cout << "Measurements: " << endl;
    print_vector(measurements);
    cout << "After removing: " << endl;
    print_vector(processed_measurements);
    cout << "StandardDeviation: " << calc_standard_deviation(measurements);
}