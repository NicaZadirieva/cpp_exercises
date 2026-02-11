// scientific_project.cpp: определяет точку входа для приложения.


#include "scientific_project.h"
#include "simple_gnuplot.h"
#include <cassert>
using namespace std;



#include <vector>
#include <cassert>
#include <algorithm> // для lower_bound

/**
 * @brief Равномерная дискретизация ломаной по x с использованием промежуточных точек.
 *
 * Исходные точки (x, y) отсортированы по возрастанию x.
 * Весь интервал [x[0], x.back()] делится на N равных частей.
 * Для середины каждой части выполняется линейная интерполяция y
 * между двумя ближайшими исходными точками.
 *
 * @tparam T Числовой тип (должен поддерживать арифметику)
 * @param x Вектор x-координат, строго возрастающих
 * @param y Вектор y-координат, соответствующий x
 * @param N Требуемое количество точек (N >= 1)
 * @return std::pair<std::vector<T>, std::vector<T>> (x_resampled, y_resampled)
 */
template<typename T>
std::pair<std::vector<T>, std::vector<T>> resample_uniform_x(
    const std::vector<T>& x,
    const std::vector<T>& y,
    std::size_t N
) {
    // --- Проверки ---
    assert(x.size() == y.size() && "x and y must have the same length");
    assert(x.size() >= 2 && "At least two points are needed for interpolation");
    assert(std::is_sorted(x.begin(), x.end()) && "x must be sorted in ascending order");
    assert(N >= 1 && "N must be at least 1");

    T x0 = x.front();
    T x1 = x.back();
    T dx = (x1 - x0) / static_cast<T>(N);   // ширина одного интервала
    T half_dx = dx / T(2);                 // половина интервала

    std::vector<T> x_res, y_res;
    x_res.reserve(N);
    y_res.reserve(N);

    for (std::size_t k = 0; k < N; ++k) {
        // Середина k-го интервала
        T x_mid = x0 + static_cast<T>(k) * dx + half_dx;

        // Поиск отрезка [x[i], x[i+1]], содержащего x_mid
        // Используем std::lower_bound: первый индекс, где x[i] >= x_mid
        auto it = std::lower_bound(x.begin(), x.end(), x_mid);
        std::size_t idx = it - x.begin();

        if (idx == 0) {
            // x_mid левее первой точки (возможно только при x_mid == x0)
            // тогда берём y первой точки
            x_res.push_back(x_mid);
            y_res.push_back(y.front());
        }
        else if (idx >= x.size()) {
            // x_mid правее последней точки (только при x_mid == x1)
            x_res.push_back(x_mid);
            y_res.push_back(y.back());
        }
        else {
            // Интерполяция между точками idx-1 и idx
            T x_left = x[idx - 1];
            T x_right = x[idx];
            T y_left = y[idx - 1];
            T y_right = y[idx];

            // Линейная интерполяция
            T t = (x_mid - x_left) / (x_right - x_left);
            T y_mid = y_left + t * (y_right - y_left);

            x_res.push_back(x_mid);
            y_res.push_back(y_mid);
        }
    }

    return { std::move(x_res), std::move(y_res) };
}
vector<double> calculate_derivative(const vector<double>& xs, const vector<double>& ys) {
    size_t n = xs.size();
    vector<double> derivative(n);

    if (n < 2) return derivative;

    // Первая точка: правосторонняя разность
    derivative[0] = (ys[1] - ys[0]) / (xs[1] - xs[0]);

    // Внутренние точки: центральная разность (более точная)
    for (size_t i = 1; i < n - 1; i++) {
        derivative[i] = (ys[i + 1] - ys[i - 1]) / (xs[i + 1] - xs[i - 1]);
    }

    // Последняя точка: левосторонняя разность
    derivative[n - 1] = (ys[n - 1] - ys[n - 2]) / (xs[n - 1] - xs[n - 2]);

    return derivative;
}

string replace(string s,
    char c1, char c2)
{
    int l = s.length();

    // loop to traverse in the string
    for (int i = 0; i < l; i++)
    {
        // check for c1 and replace
        if (s[i] == c1)
            s[i] = c2;

        // check for c2 and replace
        else if (s[i] == c2)
            s[i] = c1;
    }
    return s;
}


pair<vector<double>, vector<double>> calculate_middle_points(const vector<double>& xs, const vector<double>& ys, int window_size = 3) {
    if (xs.empty() || ys.empty() || xs.size() != ys.size()) {
        return make_pair(vector<double>(), vector<double>());
    }

    vector<double> new_xs;
    vector<double> new_ys;
    int n = xs.size();

    for (int i = 0; i < n; i++) {
        int left, right;
        if (i - window_size >= 0) {
            left = i - window_size;
        }
        else {
            left = 0;
        }
        if (i + window_size < n) {
            right = i + window_size;
        }
        else {
            right = n - 1;
        }
        double sumx = 0.0, sumy = 0.0;
        for (int j = left; j <= right; j++) {
            sumx += xs[j];
            sumy += ys[j];
        }
        new_xs.push_back(sumx / (right - left));
        new_ys.push_back(sumy / (right - left));
    }

    return make_pair(new_xs, new_ys);
}

pair<vector<double>, vector<double>> calculate_sorted_middle_points(const vector<double>& xs, const vector<double>& ys, int window_size = 3) {
    if (xs.empty() || ys.empty() || xs.size() != ys.size()) {
        return make_pair(vector<double>(), vector<double>());
    }
    // Создаем вектор пар
    std::vector<std::pair<double, double>> pairs;
    pairs.reserve(xs.size());

    for (int i = 0; i < xs.size(); ++i) {
        pairs.emplace_back(xs[i], ys[i]);
    }
    // Сортируем по первому элементу (x)
    std::sort(pairs.begin(), pairs.end(),
        [](const auto& a, const auto& b) { return a.first < b.first; });
    int n = xs.size();
    // Распаковываем
    std::vector<double> x_sorted;
    std::vector<double> y_sorted;
    x_sorted.reserve(pairs.size());
    y_sorted.reserve(pairs.size());
    for (const auto& p : pairs) {
        x_sorted.push_back(p.first);
        y_sorted.push_back(p.second);
    }

    vector<double> new_ys = vector<double>();

    for (int i = 0; i < n; i++) {
        int left, right;
        if (i - window_size >= 0) {
            left = i - window_size;
        }
        else {
            left = 0;
        }
        if (i + window_size < n) {
            right = i + window_size;
        }
        else {
            right = n - 1;
        }
        double sumx = 0.0, sumy = 0.0;
        for (int j = left; j <= right; j++) {
            sumy += y_sorted[j];
        }
        new_ys.push_back(sumy / (right - left));
    }

    return make_pair(x_sorted, new_ys);
}

int main() {
    // raw data
    ScientificFile file("data/raw.csv", ios::in);
    vector<double> xs = {};
    vector<double> ys = {};
    file.read_line();
    file.read_line();
    while (!file.eof()) {
        string line = file.read_line();

        stringstream s(line);

        string x, y;
        getline(s, x, ';');
        getline(s, y, ';');
        x = replace(x, ',', '.');
        y = replace(y, ',', '.');
        xs.push_back(stod(x));
        ys.push_back(stod(y));

    }

    SimpleGnuplot::plot_data(xs, ys, "fx");


    vector<double> dys = calculate_derivative(xs, ys);
    SimpleGnuplot::plot_data(xs, dys, "dfx");

    // after deleting points
    auto [temp_x_del_points, temp_y_del_points] = calculate_sorted_middle_points(xs, ys, 600);
    auto [temp_x_del_points2, temp_y_del_points2] = resample_uniform_x(temp_x_del_points, temp_y_del_points, 80);


    SimpleGnuplot::plot_data(temp_x_del_points2, temp_y_del_points2, "delfx");
    vector<double> del_dys = calculate_derivative(temp_x_del_points2, temp_y_del_points2);
    SimpleGnuplot::plot_data(temp_x_del_points2, del_dys, "deldfx");
    // processed file via origin
    vector<double> processed_xs = {};
    vector<double> processed_ys = {};
    vector<double> processed_dys = {};
    ScientificFile processed_file("data/processed.csv", ios::in);
    processed_file.read_line();
    while (!processed_file.eof()) {
        string line = processed_file.read_line();

        stringstream s(line);

        string x, y, dys;
        getline(s, x, ';');
        getline(s, y, ';');
        getline(s, dys, ';');
        x = replace(x, ',', '.');
        y = replace(y, ',', '.');
        dys = replace(dys, ',', '.');

        processed_xs.push_back(stod(x));
        processed_ys.push_back(stod(y));
        processed_dys.push_back(stod(dys));

    }
    SimpleGnuplot::plot_data(processed_xs, processed_ys, "fx2");
    SimpleGnuplot::plot_data(processed_xs, processed_dys, "dfx2");


    return 0;
}