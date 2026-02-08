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




int main() {
    ScientificFile file("data/function_points.csv", ios::in);
    vector<double> xs = {};
    vector<double> ys = {};
    file.read_line();
    while (!file.eof()) {
        string line = file.read_line();

        stringstream s(line);

        string x, y;
        getline(s, x, ',');
        getline(s, y, ',');

        xs.push_back(stod(x));
        ys.push_back(stod(y));
    
    }
    cout << endl << endl;
    cout << "xs: " << endl;
    copy(xs.begin(), xs.end(), ostream_iterator<double>(cout, " "));
    cout << endl << "ys: " << endl;
    copy(ys.begin(), ys.end(), ostream_iterator<double>(cout, " "));
    cout << endl << endl;

    SimpleGnuplot::plot_data(xs, ys, "fx");


    vector<double> dys = calculate_derivative(xs, ys);

    copy(dys.begin(), dys.end(), ostream_iterator<double>(cout, " "));


    SimpleGnuplot::plot_data(xs, dys, "dfx");
    return 0;
}