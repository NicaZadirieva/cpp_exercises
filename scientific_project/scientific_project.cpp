// scientific_project.cpp: определяет точку входа для приложения.


#include "scientific_project.h"
#include "simple_gnuplot.h"

using namespace std;


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
    auto [temp_x_del_points, temp_y_del_points] = calculate_middle_points(xs, ys, 600);
    auto [temp_x_del_points2, temp_y_del_points2] = calculate_middle_points(temp_x_del_points, temp_y_del_points, 600);
    auto [x_del_points, y_del_points] = calculate_middle_points(temp_x_del_points2, temp_y_del_points2, 600);

    SimpleGnuplot::plot_data(x_del_points, y_del_points, "delfx");
    vector<double> del_dys = calculate_derivative(x_del_points, y_del_points);
    SimpleGnuplot::plot_data(x_del_points, del_dys, "deldfx");
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