#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iomanip>

class Histogram {
private:
    std::vector<double> data_;
    std::vector<double> bins_;
    std::vector<int> frequencies_;
    double min_val_, max_val_;
    size_t num_bins_;

public:
    Histogram(const std::vector<double>& data, size_t num_bins = 10);

    void compute();
    void print_ascii(size_t max_height = 20) const;
    void print_statistics() const;
    void save_to_csv(const std::string& filename) const;

    // Геттеры
    const std::vector<double>& bins() const { return bins_; }
    const std::vector<int>& frequencies() const { return frequencies_; }
    double mean() const;
    double variance() const;
    double stddev() const;

private:
    void compute_bins();
    void compute_frequencies();
    void print_horizontal(size_t max_height) const;
    void print_vertical() const;
};

#endif // HISTOGRAM_H