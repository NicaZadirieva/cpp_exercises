#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>

// Реализуйте все методы.
class ScientificData {
private:
    std::vector<double> data;
    std::string name;
    std::string unit;
public:
    // Конструкторы
    ScientificData(const std::string& n, const std::string& u);

    // Методы
    void addMeasurement(double value);
    double mean() const;
    double stdDev() const;
    void saveToFile(const std::string& filename) const;

    // Геттеры
    size_t size() const { return data.size(); }
};