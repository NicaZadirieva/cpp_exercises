#include "../include/scientific_file.h"

// Конструктор: захват ресурса (открытие файла)
ScientificFile::ScientificFile(const std::string& name, std::ios_base::openmode mode)
    : filename(name) {

    file.open(filename, mode);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    is_open_ = true;
    std::cout << "[RAII] File opened: " << filename << std::endl;
}

// Деструктор: освобождение ресурса (закрытие файла)
ScientificFile::~ScientificFile() {
    if (is_open_) {
        file.close();
        std::cout << "[RAII] File closed: " << filename << std::endl;
    }
}


// Разрешаем перемещение
ScientificFile::ScientificFile(ScientificFile&& other) noexcept
    : file(std::move(other.file)),
    filename(std::move(other.filename)),
    is_open_(other.is_open_) {
    other.is_open_ = false;
}

// Методы для работы с файлом
void ScientificFile::write(const std::string& data) {
    if (!is_open_) throw std::runtime_error("File is not opened");
    file << data;
}

std::string ScientificFile::read_line() {
    if (!is_open_) throw std::runtime_error("File is not opened");
    std::string line;
    std::getline(file, line);
    return line;
}