#ifndef SCIENTIFIC_DATASET_H
#define SCIENTIFIC_DATASET_H

#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <chrono>
#include <iostream>

class ScientificDataset {
private:
    uint32_t version_ = 1;
    uint64_t timestamp_;
    std::vector<std::string> column_names_;
    std::vector<double> data_;

public:
    // Конструкторы
    ScientificDataset() = default;
    ScientificDataset(const std::vector<std::string>& columns, const std::vector<double>& data)
        : column_names_(columns), data_(data) {
        timestamp_ = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
    }

    // Геттеры
    uint32_t version() const { return version_; }
    uint64_t timestamp() const { return timestamp_; }
    const std::vector<std::string>& column_names() const { return column_names_; }
    const std::vector<double>& data() const { return data_; }

    // Методы для работы с данными
    void add_column(const std::string& name, const std::vector<double>& column_data);
    std::vector<double> get_column(size_t index) const;

    // Сериализация/десериализация
    bool saveBinary(const std::string& filename) const;
    bool loadBinary(const std::string& filename);

    // Утилиты
    void print_info() const;
    size_t rows() const;
    size_t cols() const;
};

#endif // SCIENTIFIC_DATASET_H