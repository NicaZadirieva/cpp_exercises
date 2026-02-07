#define _USE_MATH_DEFINES
#include "include/histogram.h"
#include "include/gnuplot_histogram.h"
#include <iostream>
#include <random>
#include <fstream>
#include <cmath>
#include <vector>
#include <sstream>

// Генерация различных распределений
std::vector<double> generate_normal_distribution(size_t n, double mean = 0.0, double stddev = 1.0) {
    std::vector<double> data;
    data.reserve(n);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> dist(mean, stddev);

    for (size_t i = 0; i < n; ++i) {
        data.push_back(dist(gen));
    }

    return data;
}

std::vector<double> generate_uniform_distribution(size_t n, double min = 0.0, double max = 1.0) {
    std::vector<double> data;
    data.reserve(n);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(min, max);

    for (size_t i = 0; i < n; ++i) {
        data.push_back(dist(gen));
    }

    return data;
}

std::vector<double> generate_exponential_distribution(size_t n, double lambda = 1.0) {
    std::vector<double> data;
    data.reserve(n);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::exponential_distribution<> dist(lambda);

    for (size_t i = 0; i < n; ++i) {
        data.push_back(dist(gen));
    }

    return data;
}

// Пример физических данных: измерения температуры
std::vector<double> generate_temperature_data(size_t n) {
    std::vector<double> data;
    data.reserve(n);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> base_dist(22.5, 0.5);  // Средняя температура 22.5°C
    std::normal_distribution<> noise_dist(0.0, 0.1);  // Шум измерений

    for (size_t i = 0; i < n; ++i) {
        double base = base_dist(gen);
        double noise = noise_dist(gen);
        double daily_variation = 2.0 * std::sin(2.0 * M_PI * i / (n / 24.0)); // Суточные колебания
        double measurement = base + daily_variation + noise;

        data.push_back(measurement);
    }

    return data;
}

void example_normal_distribution() {
    std::cout << "========================================\n";
    std::cout << "   NORMAL DISTRIBUTION EXAMPLE\n";
    std::cout << "========================================\n";

    auto data = generate_normal_distribution(10000, 0.0, 1.0);
    Histogram hist(data, 20);

    // ASCII гистограмма
    hist.print_ascii(15);

    // Сохранение в CSV
    hist.save_to_csv("normal_distribution.csv");

    // Построение графиков через GNUplot
    GnuplotHistogram::plot(hist, "Normal Distribution (N=10000)");
}

void example_temperature_data() {
    std::cout << "\n\n========================================\n";
    std::cout << "   TEMPERATURE MEASUREMENTS EXAMPLE\n";
    std::cout << "========================================\n";

    auto data = generate_temperature_data(500);
    Histogram hist(data, 15);

    hist.print_ascii(12);

    // Сохранение статистики
    std::ofstream stats("temperature_stats.txt");
    stats << "Temperature Measurements Statistics\n";
    stats << "===================================\n";
    stats << "Mean: " << hist.mean() << " °C\n";
    stats << "Std Dev: " << hist.stddev() << " °C\n";
    stats << "Variance: " << hist.variance() << "\n";
    stats << "Range: " << *std::min_element(data.begin(), data.end())
        << " - " << *std::max_element(data.begin(), data.end()) << " °C\n";
    stats.close();

    std::cout << "Statistics saved to: temperature_stats.txt\n";
}

void example_multiple_distributions() {
    std::cout << "\n\n========================================\n";
    std::cout << "   MULTIPLE DISTRIBUTIONS COMPARISON\n";
    std::cout << "========================================\n";

    // Генерация данных разных распределений
    auto normal_data = generate_normal_distribution(5000, 0.0, 1.0);
    auto uniform_data = generate_uniform_distribution(5000, -3.0, 3.0);
    auto exp_data = generate_exponential_distribution(5000, 1.0);

    Histogram hist1(normal_data, 25);
    Histogram hist2(uniform_data, 25);
    Histogram hist3(exp_data, 25);

    // Сравнительная таблица
    std::cout << "\nDistribution Comparison:\n";
    std::cout << std::string(60, '-') << "\n";
    std::cout << std::setw(20) << "Distribution"
        << std::setw(15) << "Mean"
        << std::setw(15) << "Std Dev"
        << std::setw(15) << "Min"
        << std::setw(15) << "Max" << "\n";
    std::cout << std::string(60, '-') << "\n";

    auto print_row = [](const std::string& name, const Histogram& hist,
        const std::vector<double>& data) {
            std::cout << std::setw(20) << name
                << std::setw(15) << std::setprecision(4) << hist.mean()
                << std::setw(15) << hist.stddev()
                << std::setw(15) << *std::min_element(data.begin(), data.end())
                << std::setw(15) << *std::max_element(data.begin(), data.end())
                << "\n";
        };

    print_row("Normal", hist1, normal_data);
    print_row("Uniform", hist2, uniform_data);
    print_row("Exponential", hist3, exp_data);

    std::cout << std::string(60, '-') << "\n";

    // Гистограмма самого интересного распределения
    hist3.print_ascii(15);
}

void interactive_example() {
    std::cout << "\n\n========================================\n";
    std::cout << "   INTERACTIVE HISTOGRAM EXAMPLE\n";
    std::cout << "========================================\n";

    // Пользовательские данные
    std::vector<double> user_data;
    char choice;

    std::cout << "Enter your own data points (space separated):\n";
    std::cout << "Example: 1.2 3.4 5.6 7.8 9.0\n";
    std::cout << "Or type 'r' for random data: ";

    std::string input;
    std::getline(std::cin, input);

    if (input == "r" || input == "R") {
        // Генерируем случайные данные
        user_data = generate_normal_distribution(100, 0.0, 1.0);
        std::cout << "Generated 100 random normal values\n";
    }
    else {
        // Парсим пользовательский ввод
        std::stringstream ss(input);
        double value;

        while (ss >> value) {
            user_data.push_back(value);
        }

        if (user_data.empty()) {
            std::cout << "No data entered. Using default example.\n";
            user_data = { 1.1, 1.2, 1.3, 1.4, 1.5, 2.1, 2.2, 2.3, 3.1, 3.2, 4.1 };
        }
    }

    // Запрашиваем количество интервалов
    std::cout << "Enter number of bins (default 10): ";
    size_t num_bins = 10;
    std::string bins_input;
    std::getline(std::cin, bins_input);

    if (!bins_input.empty()) {
        try {
            num_bins = std::stoi(bins_input);
            if (num_bins < 1) num_bins = 1;
            if (num_bins > 50) num_bins = 50;
        }
        catch (...) {
            std::cout << "Invalid input, using default (10)\n";
        }
    }

    // Создаем и отображаем гистограмму
    try {
        Histogram hist(user_data, num_bins);
        hist.print_ascii(20);

        // Предлагаем сохранить
        std::cout << "\nSave histogram to CSV? (y/n): ";
        std::getline(std::cin, input);

        if (input == "y" || input == "Y") {
            hist.save_to_csv("user_histogram.csv");
            std::cout << "Saved to user_histogram.csv\n";
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

int main() {
    std::cout << "HISTOGRAM BUILDER FOR SCIENTIFIC DATA\n";
    std::cout << "========================================\n\n";

    // Выберите пример для запуска
    std::cout << "Select example:\n";
    std::cout << "1. Normal distribution (10k points)\n";
    std::cout << "2. Temperature measurements\n";
    std::cout << "3. Multiple distributions comparison\n";
    std::cout << "4. Interactive example\n";
    std::cout << "Enter choice (1-4): ";

    int choice;
    std::cin >> choice;
    std::cin.ignore(); // Игнорируем оставшийся newline

    switch (choice) {
    case 1:
        example_normal_distribution();
        break;
    case 2:
        example_temperature_data();
        break;
    case 3:
        example_multiple_distributions();
        break;
    case 4:
        interactive_example();
        break;
    default:
        std::cout << "Invalid choice, running normal distribution example.\n";
        example_normal_distribution();
    }

    return 0;
}