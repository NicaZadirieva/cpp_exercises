// scientific_project.cpp: определяет точку входа для приложения.


#include "scientific_project.h"
#include "simple_gnuplot.h"

using namespace std;


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


    return 0;
}