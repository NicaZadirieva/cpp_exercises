#ifndef STATISTICS_H
#define STATISTICS_H

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <numeric>
#include <type_traits>
#include <cmath>
#include <iomanip>

template<typename T>
struct Statistics {
    T min, max, mean, median;

    // Перегрузка оператора вывода для удобства
    friend std::ostream& operator<<(std::ostream& os, const Statistics<T>& stats) {
        os << std::fixed << std::setprecision(6);
        os << "Statistics:\n";
        os << "  Min: " << stats.min << "\n";
        os << "  Max: " << stats.max << "\n";
        os << "  Mean: " << stats.mean << "\n";
        os << "  Median: " << stats.median << "\n";
        return os;
    }
};

// Вспомогательная функция для проверки, что тип является арифметическим
template<typename T>
using IsArithmetic = typename std::enable_if<std::is_arithmetic<T>::value, bool>::type;

// Основная шаблонная функция
template<typename Container, typename = IsArithmetic<typename Container::value_type>>
Statistics<typename Container::value_type> calculateStats(const Container& data) {
    using T = typename Container::value_type;

    // Проверка на пустой контейнер
    if (data.empty()) {
        throw std::invalid_argument("Container is empty");
    }

    Statistics<T> stats;

    // 1. Минимум и максимум
    auto [min_it, max_it] = std::minmax_element(std::begin(data), std::end(data));
    stats.min = *min_it;
    stats.max = *max_it;

    // 2. Среднее значение
    T sum = std::accumulate(std::begin(data), std::end(data), T(0));
    stats.mean = sum / static_cast<T>(data.size());

    // 3. Медиана (требует сортировки)
    // Создаем копию данных для сортировки
    std::vector<T> sorted_data(std::begin(data), std::end(data));
    std::sort(sorted_data.begin(), sorted_data.end());

    size_t n = sorted_data.size();
    if (n % 2 == 0) {
        // Четное количество элементов: среднее двух центральных
        stats.median = (sorted_data[n / 2 - 1] + sorted_data[n / 2]) / T(2);
    }
    else {
        // Нечетное количество элементов: центральный элемент
        stats.median = sorted_data[n / 2];
    }

    return stats;
}

// Вспомогательная функция для демонстрации работы с разными типами контейнеров
template<typename Container>
void testAndPrintStats(const Container& data, const std::string& container_name) {
    std::cout << "\n=== Testing with " << container_name << " ===" << std::endl;

    try {
        auto stats = calculateStats(data);
        std::cout << stats;

        // Дополнительная информация
        std::cout << "  Size: " << data.size() << "\n";
        std::cout << "  Range: " << (stats.max - stats.min) << "\n";
        // Убрано: коэффициент вариации, так как нет std_dev
        // std::cout << "  Coefficient of variation: " << (stats.std_dev / stats.mean) * 100 << "%\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

#endif // STATISTICS_H