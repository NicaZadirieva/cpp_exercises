#include "../include/matrix.h"

// Базовый конструктор
Matrix::Matrix(size_t rows, size_t cols)
    : rows_(rows), cols_(cols), data_(rows* cols, 0.0) {
}

// Конструктор копирования
Matrix::Matrix(const Matrix& other)
    : rows_(other.rows_), cols_(other.cols_), data_(other.data_) {
    std::cout << "Copy constructor called\n";
}

// Конструктор перемещения
Matrix::Matrix(Matrix&& other) noexcept
    : rows_(other.rows_), cols_(other.cols_), data_(std::move(other.data_)) {
    other.rows_ = 0;
    other.cols_ = 0;
    std::cout << "Move constructor called\n";
}

// Оператор присваивания копированием
Matrix& Matrix::operator=(const Matrix& other) {
    std::cout << "Copy assignment called\n";

    if (this != &other) {
        Matrix temp(other);
        std::swap(rows_, temp.rows_);
        std::swap(cols_, temp.cols_);
        std::swap(data_, temp.data_);
    }
    return *this;
}

// Оператор присваивания перемещением
Matrix& Matrix::operator=(Matrix&& other) noexcept {
    std::cout << "Move assignment called\n";

    if (this != &other) {
        rows_ = other.rows_;
        cols_ = other.cols_;
        data_ = std::move(other.data_);

        other.rows_ = 0;
        other.cols_ = 0;
    }
    return *this;
}

// Оператор доступа к элементам
double& Matrix::operator()(size_t row, size_t col) {
    validate_index(row, col);
    return data_[row * cols_ + col];
}

const double& Matrix::operator()(size_t row, size_t col) const {
    validate_index(row, col);
    return data_[row * cols_ + col];
}

// Валидация индексов
void Matrix::validate_index(size_t row, size_t col) const {
    if (row >= rows_ || col >= cols_) {
        throw std::out_of_range(
            "Matrix index out of range: (" +
            std::to_string(row) + ", " + std::to_string(col) +
            ") for matrix of size (" +
            std::to_string(rows_) + "x" + std::to_string(cols_) + ")"
        );
    }
}

// Валидация размеров
void Matrix::validate_dimensions(const Matrix& other, const std::string& op) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument(
            "Matrix dimensions mismatch for " + op + ": " +
            std::to_string(rows_) + "x" + std::to_string(cols_) +
            " vs " + std::to_string(other.rows_) + "x" + std::to_string(other.cols_)
        );
    }
}

// Печать матрицы
void Matrix::print() const {
    for (size_t i = 0; i < rows_; ++i) {
        for (size_t j = 0; j < cols_; ++j) {
            std::cout << (*this)(i, j) << " ";
        }
        std::cout << std::endl;
    }
}