#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>

class ScientificFile {
private:
    std::fstream file;
    std::string filename;
    bool is_open_ = false;

public:
    // Конструктор: захват ресурса (открытие файла)
    ScientificFile(const std::string& name, std::ios_base::openmode mode);

    // Деструктор: освобождение ресурса (закрытие файла)
    ~ScientificFile();

    // Запрещаем копирование
    ScientificFile(const ScientificFile&) = delete;
    ScientificFile& operator=(const ScientificFile&) = delete;

    // Разрешаем перемещение
    ScientificFile(ScientificFile&& other) noexcept;

    // Методы для работы с файлом
    void write(const std::string& data);

    std::string read_line();

    bool eof() const { return file.eof(); }
    bool is_open() const { return is_open_; }
};