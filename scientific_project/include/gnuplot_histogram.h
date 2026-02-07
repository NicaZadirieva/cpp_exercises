#ifndef GNUPLOT_HISTOGRAM_H
#define GNUPLOT_HISTOGRAM_H

#include "histogram.h"
#include <string>
#include <vector>

class GnuplotHistogram {
public:
    static void plot(const Histogram& hist,
        const std::string& title = "Histogram",
        const std::string& output_file = "");

    static void plot_interactive(const Histogram& hist,
        const std::string& title = "Histogram");

    static void plot_multiple(const std::vector<Histogram>& histograms,
        const std::vector<std::string>& names,
        const std::string& output_file = "histograms.png");

private:
    static std::string generate_gnuplot_script(const Histogram& hist,
        const std::string& title,
        const std::string& data_file);

    static void save_histogram_data(const Histogram& hist,
        const std::string& filename);
};

#endif // GNUPLOT_HISTOGRAM_H