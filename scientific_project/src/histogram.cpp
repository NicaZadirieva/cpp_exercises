#include "../include/histogram.h"
#include <fstream>
#include <numeric>
#include <stdexcept>

Histogram::Histogram(const std::vector<double>& data, size_t num_bins)
    : data_(data), num_bins_(num_bins) {

    if (data.empty()) {
        throw std::invalid_argument("Data vector is empty");
    }

    if (num_bins < 1) {
        throw std::invalid_argument("Number of bins must be at least 1");
    }

    min_val_ = *std::min_element(data.begin(), data.end());
    max_val_ = *std::max_element(data.begin(), data.end());

    // Расширяем диапазон немного для краевых случаев
    double range = max_val_ - min_val_;
    if (range < 1e-10) {
        min_val_ -= 0.5;
        max_val_ += 0.5;
    }
    else {
        double epsilon = range * 0.01;
        min_val_ -= epsilon;
        max_val_ += epsilon;
    }

    compute();
}

void Histogram::compute() {
    compute_bins();
    compute_frequencies();
}

void Histogram::compute_bins() {
    bins_.clear();
    bins_.reserve(num_bins_ + 1);

    double bin_width = (max_val_ - min_val_) / num_bins_;

    for (size_t i = 0; i <= num_bins_; ++i) {
        bins_.push_back(min_val_ + i * bin_width);
    }
}

void Histogram::compute_frequencies() {
    frequencies_.clear();
    frequencies_.resize(num_bins_, 0);

    double bin_width = bins_[1] - bins_[0];

    for (double value : data_) {
        if (value < min_val_ || value >= max_val_) {
            continue; // Значение вне диапазона (маловероятно после расширения)
        }

        size_t bin_index = static_cast<size_t>((value - min_val_) / bin_width);

        // Обработка граничного случая: value == max_val_
        if (bin_index == num_bins_) {
            bin_index = num_bins_ - 1;
        }

        frequencies_[bin_index]++;
    }
}

void Histogram::print_ascii(size_t max_height) const {
    if (frequencies_.empty()) {
        std::cout << "No data to display.\n";
        return;
    }

    int max_freq = *std::max_element(frequencies_.begin(), frequencies_.end());

    if (max_freq == 0) {
        std::cout << "All frequencies are zero.\n";
        return;
    }

    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "            ASCII HISTOGRAM\n";
    std::cout << "========================================\n";
    std::cout << "Data points: " << data_.size() << "\n";
    std::cout << "Bins: " << num_bins_ << "\n";
    std::cout << "Range: [" << min_val_ << ", " << max_val_ << "]\n\n";

    // Горизонтальная гистограмма
    print_horizontal(max_height);

    // Вертикальная гистограмма
    print_vertical();

    // Статистика
    print_statistics();
}

void Histogram::print_horizontal(size_t max_height) const {
    int max_freq = *std::max_element(frequencies_.begin(), frequencies_.end());
    double scale = static_cast<double>(max_height) / max_freq;

    std::cout << "Horizontal Histogram:\n";
    std::cout << std::string(60, '-') << "\n";

    for (size_t i = 0; i < num_bins_; ++i) {
        // Метка интервала
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "[" << std::setw(6) << bins_[i]
            << " - " << std::setw(6) << bins_[i + 1] << "): ";

        // Столбец гистограммы
        int bar_length = static_cast<int>(frequencies_[i] * scale);

        // Символы для разных уровней заполнения
        std::string bar;
        if (bar_length > 0) {
            bar = std::string(bar_length, '#');

            // Добавляем полублоки для более точного отображения
            double remainder = frequencies_[i] * scale - bar_length;
            if (remainder > 0.66) {
                bar += "#";
            }
            else if (remainder > 0.33) {
                bar += "#";
            }
            else if (remainder > 0.0) {
                bar += "#";
            }
        }

        // Частота
        std::cout << std::setw(40) << std::left << bar
            << " [" << std::setw(4) << frequencies_[i] << "]";

        // Процент
        double percent = 100.0 * frequencies_[i] / data_.size();
        std::cout << " " << std::setw(5) << std::setprecision(1) << percent << "%";

        std::cout << "\n";
    }

    std::cout << std::string(60, '-') << "\n";
}

void Histogram::print_vertical() const {
    if (frequencies_.empty() || num_bins_ == 0) {
        std::cout << "No data to display vertical histogram.\n";
        return;
    }

    int max_freq = *std::max_element(frequencies_.begin(), frequencies_.end());
    const size_t max_height = 20;

    if (max_freq == 0) {
        std::cout << "All frequencies are zero.\n";
        return;
    }

    double scale = static_cast<double>(max_height) / max_freq;

    std::cout << "\nVertical Histogram:\n";

    // Рисуем сверху вниз
    for (int level = max_height; level >= 0; --level) {
        // Левая ось Y с подписями
        if (level == max_height) {
            std::cout << std::setw(4) << max_freq << " + ";
        }
        else if (level == 0) {
            std::cout << "   0 + ";
        }
        else if (level % 5 == 0) {
            // Промежуточные метки каждые 5 уровней
            int value = static_cast<int>(level * max_freq / max_height);
            std::cout << std::setw(4) << value << " | ";
        }
        else {
            std::cout << "     | ";
        }

        // Рисуем столбцы гистограммы
        for (size_t i = 0; i < num_bins_; ++i) {
            if (frequencies_[i] * scale >= level) {
                if (level == 0) {
                    std::cout << "-"; // Основание столбца
                }
                else {
                    std::cout << "#"; // Заполненная часть столбца
                }
            }
            else if (level == 0) {
                std::cout << "-"; // Ось X
            }
            else {
                std::cout << " "; // Пустое пространство
            }

            // Расстояние между столбцами
            if (i < num_bins_ - 1) {
                if (level == 0) {
                    std::cout << "-"; // Продолжение оси X
                }
                else {
                    std::cout << " "; // Пробел между столбцами
                }
            }
        }

        std::cout << "\n";
    }

    // Ось X с отметками
    std::cout << "      +";
    for (size_t i = 0; i < num_bins_; ++i) {
        std::cout << "--";
    }
    std::cout << "\n";

    // Подписи на оси X (только для некоторых столбцов)
    std::cout << "       ";
    for (size_t i = 0; i < num_bins_; ++i) {
        // Показываем подписи для каждого 3-го столбца или последнего
        if (i % 3 == 0 || i == num_bins_ - 1) {
            // Сокращаем числа для лучшего отображения
            std::string label;
            double value = bins_[i];

            if (std::abs(value) >= 100) {
                label = std::to_string(static_cast<int>(value));
            }
            else if (std::abs(value) >= 10) {
                label = std::to_string(static_cast<int>(value));
            }
            else {
                label = std::to_string(value).substr(0, 4);
            }

            std::cout << std::setw(3) << label;
        }
        else {
            std::cout << "   ";
        }

        if (i < num_bins_ - 1) {
            std::cout << " ";
        }
    }
    std::cout << "\n\n";
}

void Histogram::print_statistics() const {
    std::cout << "STATISTICS:\n";
    std::cout << std::string(40, '-') << "\n";

    std::cout << "Total data points: " << data_.size() << "\n";
    std::cout << "Data range: [" << min_val_ << ", " << max_val_ << "]\n";
    std::cout << "Bin width: " << (bins_[1] - bins_[0]) << "\n";
    std::cout << "Mean: " << mean() << "\n";
    std::cout << "Std deviation: " << stddev() << "\n";
    std::cout << "Variance: " << variance() << "\n";

    // Мода (самый частый интервал)
    auto max_it = std::max_element(frequencies_.begin(), frequencies_.end());
    size_t mode_index = std::distance(frequencies_.begin(), max_it);
    std::cout << "Mode: [" << bins_[mode_index] << ", "
        << bins_[mode_index + 1] << ") with "
        << frequencies_[mode_index] << " occurrences\n";

    // Распределение частот
    std::cout << "\nFrequency distribution:\n";
    for (size_t i = 0; i < num_bins_; ++i) {
        double percent = 100.0 * frequencies_[i] / data_.size();
        std::cout << "  Bin " << std::setw(2) << i << ": "
            << std::setw(4) << frequencies_[i] << " ("
            << std::setw(5) << std::setprecision(1) << percent << "%)\n";
    }
}

double Histogram::mean() const {
    if (data_.empty()) return 0.0;

    double sum = std::accumulate(data_.begin(), data_.end(), 0.0);
    return sum / data_.size();
}

double Histogram::variance() const {
    if (data_.size() < 2) return 0.0;

    double m = mean();
    double sum = 0.0;

    for (double value : data_) {
        double diff = value - m;
        sum += diff * diff;
    }

    return sum / (data_.size() - 1); // Выборочная дисперсия
}

double Histogram::stddev() const {
    return std::sqrt(variance());
}

void Histogram::save_to_csv(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    file << "bin_start,bin_end,midpoint,frequency,relative_frequency,density\n";

    double bin_width = bins_[1] - bins_[0];

    for (size_t i = 0; i < num_bins_; ++i) {
        double bin_start = bins_[i];
        double bin_end = bins_[i + 1];
        double midpoint = (bin_start + bin_end) / 2.0;
        double rel_freq = static_cast<double>(frequencies_[i]) / data_.size();
        double density = rel_freq / bin_width; // Плотность вероятности

        file << std::scientific << std::setprecision(10)
            << bin_start << ","
            << bin_end << ","
            << midpoint << ","
            << frequencies_[i] << ","
            << rel_freq << ","
            << density << "\n";
    }

    file.close();
    std::cout << "Histogram data saved to: " << filename << "\n";
}