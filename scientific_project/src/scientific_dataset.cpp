#include "../include/scientific_dataset.h"
#include <cstring>

// Метод для добавления столбца
void ScientificDataset::add_column(const std::string& name, const std::vector<double>& column_data) {
    if (!data_.empty() && column_data.size() != data_.size() / column_names_.size()) {
        throw std::invalid_argument("Column size must match existing data rows");
    }

    column_names_.push_back(name);

    if (data_.empty()) {
        data_ = column_data;
    }
    else {
        // Транспонирование: добавляем новый столбец
        size_t rows = data_.size() / column_names_.size();
        size_t cols = column_names_.size();

        std::vector<double> new_data(rows * cols);

        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols - 1; ++j) {
                new_data[i * cols + j] = data_[i * (cols - 1) + j];
            }
            new_data[i * cols + (cols - 1)] = column_data[i];
        }

        data_ = std::move(new_data);
    }
}

// Получение столбца
std::vector<double> ScientificDataset::get_column(size_t index) const {
    if (index >= column_names_.size()) {
        throw std::out_of_range("Column index out of range");
    }

    size_t rows = this->rows();
    size_t cols = this->cols();
    std::vector<double> column(rows);

    for (size_t i = 0; i < rows; ++i) {
        column[i] = data_[i * cols + index];
    }

    return column;
}

// Сохранение в бинарный файл
bool ScientificDataset::saveBinary(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Cannot open file for writing: " << filename << std::endl;
        return false;
    }

    try {
        // 1. Записываем версию
        file.write(reinterpret_cast<const char*>(&version_), sizeof(version_));

        // 2. Записываем timestamp
        file.write(reinterpret_cast<const char*>(&timestamp_), sizeof(timestamp_));

        // 3. Записываем количество столбцов
        uint64_t num_columns = column_names_.size();
        file.write(reinterpret_cast<const char*>(&num_columns), sizeof(num_columns));

        // 4. Записываем названия столбцов
        for (const auto& name : column_names_) {
            uint64_t name_length = name.size();
            file.write(reinterpret_cast<const char*>(&name_length), sizeof(name_length));
            file.write(name.c_str(), name_length);
        }

        // 5. Записываем количество данных
        uint64_t num_data = data_.size();
        file.write(reinterpret_cast<const char*>(&num_data), sizeof(num_data));

        // 6. Записываем данные
        if (num_data > 0) {
            file.write(reinterpret_cast<const char*>(data_.data()), num_data * sizeof(double));
        }

        return file.good();
    }
    catch (...) {
        std::cerr << "Error writing to file: " << filename << std::endl;
        return false;
    }
}

// Загрузка из бинарного файла
bool ScientificDataset::loadBinary(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Cannot open file for reading: " << filename << std::endl;
        return false;
    }

    try {
        // 1. Читаем версию
        file.read(reinterpret_cast<char*>(&version_), sizeof(version_));

        // 2. Читаем timestamp
        file.read(reinterpret_cast<char*>(&timestamp_), sizeof(timestamp_));

        // 3. Читаем количество столбцов
        uint64_t num_columns = 0;
        file.read(reinterpret_cast<char*>(&num_columns), sizeof(num_columns));

        // 4. Читаем названия столбцов
        column_names_.clear();
        column_names_.reserve(num_columns);

        for (uint64_t i = 0; i < num_columns; ++i) {
            uint64_t name_length = 0;
            file.read(reinterpret_cast<char*>(&name_length), sizeof(name_length));

            std::string name(name_length, '\0');
            file.read(&name[0], name_length);
            column_names_.push_back(std::move(name));
        }

        // 5. Читаем количество данных
        uint64_t num_data = 0;
        file.read(reinterpret_cast<char*>(&num_data), sizeof(num_data));

        // 6. Читаем данные
        data_.resize(num_data);
        if (num_data > 0) {
            file.read(reinterpret_cast<char*>(data_.data()), num_data * sizeof(double));
        }

        return file.good() && !file.eof();
    }
    catch (...) {
        std::cerr << "Error reading from file: " << filename << std::endl;
        return false;
    }
}

// Утилита для печати информации
void ScientificDataset::print_info() const {
    std::cout << "=== Scientific Dataset Info ===\n";
    std::cout << "Version: " << version_ << "\n";
    std::cout << "Timestamp: " << timestamp_ << "\n";
    std::cout << "Columns (" << column_names_.size() << "): ";
    for (const auto& name : column_names_) {
        std::cout << name << " ";
    }
    std::cout << "\n";
    std::cout << "Rows: " << rows() << "\n";
    std::cout << "Data points: " << data_.size() << "\n";

    // Печатаем первые несколько строк
    size_t rows_to_show = std::min(rows(), size_t(5));
    size_t cols = column_names_.size();

    std::cout << "\nFirst " << rows_to_show << " rows:\n";
    for (size_t i = 0; i < rows_to_show; ++i) {
        std::cout << "Row " << i << ": ";
        for (size_t j = 0; j < cols; ++j) {
            std::cout << data_[i * cols + j] << " ";
        }
        std::cout << "\n";
    }
}

// Количество строк
size_t ScientificDataset::rows() const {
    if (column_names_.empty()) return 0;
    return data_.size() / column_names_.size();
}

// Количество столбцов
size_t ScientificDataset::cols() const {
    return column_names_.size();
}