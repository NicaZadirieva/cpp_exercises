// scientific_project.cpp: определяет точку входа для приложения.


#include "scientific_project.h"
#include "simple_gnuplot.h"

using namespace std;

double calculate_delta_x(vector<double> &xs,
    vector<double> &ys) {
    double delta_x = xs[1] - xs[0];
    double delta_y = ys[1] - ys[0];
    double epsilon = 0.1;
    bool found = false;
    while (!found) {
        for (int i = 1; i < xs.size() - 1; i++) {
            if (abs((ys[i + 1] - ys[i]) / delta_x - delta_y) > epsilon) {
                delta_x = xs[i + 1] - xs[i];
                break;
            }
        }
        found = true;
    }
    return delta_x;
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

    auto delta_x = calculate_delta_x(xs, ys);
    vector<double> dxs = vector<double>(xs.size());
    vector<double> dys = vector<double>(ys.size());
    for (int i = 0; i < xs.size() - 1; i++) {
        dxs.push_back(xs[i + 1]);
        dys.push_back((ys[i + 1] - ys[i]) / delta_x);
    }
    cout << endl << "dys: " << endl;
    copy(dys.begin(), dys.end(), ostream_iterator<double>(cout, " "));


    SimpleGnuplot::plot_data(dxs, dys, "dfx");
    return 0;
}