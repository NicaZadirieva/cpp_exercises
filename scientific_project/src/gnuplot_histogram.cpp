#include "../include/gnuplot_histogram.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void GnuplotHistogram::save_histogram_data(const Histogram& hist,
    const std::string& filename) {
    hist.save_to_csv(filename);
}

std::string GnuplotHistogram::generate_gnuplot_script(const Histogram& hist,
    const std::string& title,
    const std::string& data_file) {
    std::stringstream script;

    script << "set terminal pngcairo size 1200,800 enhanced font 'Arial,12'\n";
    script << "set output 'histogram_output.png'\n";
    script << "set title '" << title << "' font 'Arial,16'\n";
    script << "set xlabel 'Value' font 'Arial,14'\n";
    script << "set ylabel 'Frequency' font 'Arial,14'\n";
    script << "set style fill solid 0.7 border lt -1\n";
    script << "set style histogram rowstacked\n";
    script << "set style data histograms\n";
    script << "set boxwidth 0.9 relative\n";
    script << "set grid ytics lt 0 lw 1 lc rgb '#bbbbbb'\n";
    script << "set grid xtics lt 0 lw 1 lc rgb '#bbbbbb'\n";
    script << "set key off\n";
    script << "set palette defined (0 \"#4B8BBE\", 1 \"#306998\", 2 \"#FFE873\", 3 \"#FFD43B\", 4 \"#646464\")\n";

    // Автоматическое определение формата подписей
    const auto& bins = hist.bins();
    if (!bins.empty()) {
        double range = bins.back() - bins.front();

        if (range > 1000) {
            script << "set format x '%.0f'\n";
        }
        else if (range > 1) {
            script << "set format x '%.1f'\n";
        }
        else {
            script << "set format x '%.3f'\n";
        }
    }

    script << "plot '" << data_file << "' using 3:4:xtic(1) with boxes lc palette\n";

    // Добавляем второй график с плотностью распределения
    script << "\n# Density plot\n";
    script << "set terminal pngcairo size 1200,800 enhanced font 'Arial,12'\n";
    script << "set output 'density_output.png'\n";
    script << "set title 'Probability Density' font 'Arial,16'\n";
    script << "set ylabel 'Density' font 'Arial,14'\n";
    script << "plot '" << data_file << "' using 3:6 with boxes lc rgb '#FF6B6B' title 'PDF'\n";

    return script.str();
}

void GnuplotHistogram::plot(const Histogram& hist,
    const std::string& title,
    const std::string& output_file) {
    // Сохраняем данные гистограммы во временный файл
    std::string data_file = "histogram_data.csv";
    save_histogram_data(hist, data_file);

    // Генерируем скрипт gnuplot
    std::string script = generate_gnuplot_script(hist, title, data_file);

    // Сохраняем скрипт
    std::ofstream script_file("histogram_plot.gnu");
    script_file << script;
    script_file.close();

    // Запускаем gnuplot
    std::string command = "gnuplot histogram_plot.gnu";
    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "✅ Histogram plot saved as 'histogram_output.png'\n";
        std::cout << "✅ Density plot saved as 'density_output.png'\n";
    }
    else {
        std::cerr << "❌ Error: gnuplot not found or failed to execute\n";
        std::cerr << "   Install gnuplot: sudo apt-get install gnuplot (Linux)\n";
        std::cerr << "   or download from: http://www.gnuplot.info\n";
    }

    // Удаляем временные файлы
    std::remove(data_file.c_str());
    std::remove("histogram_plot.gnu");
}

void GnuplotHistogram::plot_interactive(const Histogram& hist,
    const std::string& title) {
    std::string data_file = "temp_histogram_data.csv";
    save_histogram_data(hist, data_file);

    std::stringstream command;
    command << "gnuplot -persist -e \"";
    command << "set title '" << title << "'; ";
    command << "set xlabel 'Value'; ";
    command << "set ylabel 'Frequency'; ";
    command << "set style fill solid border; ";
    command << "plot '" << data_file << "' using 3:4 with boxes title 'Histogram'\"";

    int result = std::system(command.str().c_str());

    if (result != 0) {
        std::cout << "Note: gnuplot not available, showing ASCII histogram instead.\n";
        hist.print_ascii();
    }

    std::remove(data_file.c_str());
}