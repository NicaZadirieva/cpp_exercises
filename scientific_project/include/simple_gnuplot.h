#ifndef SIMPLE_GNUPLOT_H
#define SIMPLE_GNUPLOT_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>

class SimpleGnuplot {
public:
    static bool plot_histogram(const std::vector<double>& bins_start,
        const std::vector<double>& bins_end,
        const std::vector<int>& frequencies,
        const std::string& title = "Histogram") {

        if (bins_start.empty() || frequencies.empty()) {
            std::cerr << "No data to plot\n";
            return false;
        }

        // 1. Сохраняем данные в простой формат
        std::ofstream data_file("plot_data.txt");
        if (!data_file.is_open()) {
            std::cerr << "Cannot create data file\n";
            return false;
        }

        for (size_t i = 0; i < frequencies.size(); ++i) {
            double midpoint = (bins_start[i] + bins_end[i]) / 2.0;
            data_file << midpoint << " " << frequencies[i] << "\n";
        }
        data_file.close();

        // 2. Создаем простой скрипт gnuplot
        std::ofstream script("plot_script.gnu");
        script << "set terminal png\n";
        script << "set output 'histogram.png'\n";
        script << "set title '" << title << "'\n";
        script << "set xlabel 'Value'\n";
        script << "set ylabel 'Frequency'\n";
        script << "set style fill solid\n";
        script << "set boxwidth 0.9 relative\n";
        script << "plot 'plot_data.txt' using 1:2 with boxes\n";
        script.close();

        // 3. Запускаем gnuplot
        int result = std::system("gnuplot plot_script.gnu");

        // 4. Удаляем временные файлы
        std::remove("plot_data.txt");
        std::remove("plot_script.gnu");

        if (result == 0) {
            std::cout << "Histogram saved as 'histogram.png'\n";
            return true;
        }
        else {
            std::cerr << "Failed to run gnuplot\n";
            return false;
        }
    }

    static bool plot_data(const std::vector<double>& x,
        const std::vector<double>& y,
        const std::string& title = "Plot",
        const std::string& xlabel = "X",
        const std::string& ylabel = "Y") {

        if (x.size() != y.size() || x.empty()) {
            return false;
        }

        std::ofstream data("data.txt");
        for (size_t i = 0; i < x.size(); ++i) {
            data << x[i] << " " << y[i] << "\n";
        }
        data.close();

        std::ofstream script("script.gnu");
        script << "set terminal png\n";
        script << "set output '" << title << ".png'\n";
        script << "set title '" << title << "'\n";
        script << "set xlabel '" << xlabel << "'\n";
        script << "set ylabel '" << ylabel << "'\n";
        script << "set grid\n";
        script << "plot 'data.txt' with lines\n";
        script.close();

        system("gnuplot script.gnu");

        std::remove("data.txt");
        std::remove("script.gnu");

        return true;
    }
};

#endif // SIMPLE_GNUPLOT_H