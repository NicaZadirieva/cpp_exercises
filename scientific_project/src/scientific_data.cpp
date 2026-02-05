#include "../include/scientific_data.h"
using namespace std;


ScientificData::ScientificData(const string& n, const string& u) {
    this->name = n;
    this->unit = u;
}

void ScientificData::addMeasurement(double value) {
    this->data.push_back(value);
}
double ScientificData::mediana() const {
    vector<double> sorted_numbers = this->data;
    sort(sorted_numbers.begin(), sorted_numbers.end());
    const int vector_size = sorted_numbers.size();
    if (vector_size % 2 == 0) {
       return (double)(sorted_numbers[(vector_size - 1) / 2] + sorted_numbers[vector_size / 2]) / 2.0;
    }
    else {
       return (double)sorted_numbers[vector_size / 2];
    }
}
double ScientificData::stdDev() const {
    double sum = 0.0, mean, standardDeviation = 0.0;
    int size = this->data.size();
    // Вычислить сумму элементов в векторе
    for (int i = 0; i < size; ++i) {
        sum += this->data[i];
    }
    // Вычислить среднее значение
    mean = sum / size;
    // Вычислить сумму квадратов отклонений от среднего значения
    for (int i = 0; i < size; ++i) {
        standardDeviation += pow(this->data[i] - mean, 2);
    }
    // Вычислить квадратный корень из дисперсии, чтобы получить стандартное отклонение
    return sqrt(standardDeviation / size);
}
void ScientificData::saveToFile() const {
    time_t now = time(nullptr);
    tm* local = localtime(&now);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", local);

    // Создаем имя файла без недопустимых символов
    std::string clean_name = this->name;
    std::replace(clean_name.begin(), clean_name.end(), ' ', '_');
    std::string filename = "data/" + clean_name + "_" + this->unit + "_" + timestamp + ".txt";

    // Пробуем создать директорию, если её нет
    try {
        filesystem::create_directories("data");
    }
    catch (...) {
        std::cerr << "Warning: Could not create data directory" << std::endl;
    }

    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for writing" << std::endl;
        std::cerr << "Current directory: " << filesystem::current_path() << std::endl;
        return;
    }

    std::cout << "Saving to file: " << filename << std::endl;

    for (size_t i = 0; i < this->data.size(); ++i) {
        file << this->data[i];
        if (i != this->data.size() - 1) file << ",";
    }

    file.close();
    std::cout << "File saved successfully! " << this->data.size() << " measurements written." << std::endl;
}
