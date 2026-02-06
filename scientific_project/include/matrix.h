#ifndef MATRIX_H
#define MATRIX_H

#include <cstddef>
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <utility>

class Matrix {
private:
    size_t rows_;
    size_t cols_;
    std::vector<double> data_;

    void validate_index(size_t row, size_t col) const;
    void validate_dimensions(const Matrix& other, const std::string& op) const;

public:
    // Конструкторы
    Matrix(size_t rows = 0, size_t cols = 0);
    Matrix(const Matrix& other);
    Matrix(Matrix&& other) noexcept;

    // Операторы присваивания
    Matrix& operator=(const Matrix& other);
    Matrix& operator=(Matrix&& other) noexcept;

    // Деструктор
    ~Matrix() = default;

    // Геттеры
    size_t rows() const { return rows_; }
    size_t cols() const { return cols_; }
    size_t size() const { return rows_ * cols_; }

    // Операторы доступа к элементам
    double& operator()(size_t row, size_t col);
    const double& operator()(size_t row, size_t col) const;

    // Утилиты
    void print() const;
};

#endif // MATRIX_H