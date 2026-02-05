#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <ctime>
#include <filesystem>

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
    double mediana() const;
    double stdDev() const;
    void saveToFile() const;

    // Геттеры
    size_t size() const { return data.size(); }
};