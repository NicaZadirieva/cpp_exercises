// scientific_project.cpp: определяет точку входа для приложения.


#include "scientific_project.h"


using namespace std;


// Проверка и удаление BOM
string removeBOM(const string& str) {
    if (str.size() >= 3 &&
        static_cast<unsigned char>(str[0]) == 0xEF &&
        static_cast<unsigned char>(str[1]) == 0xBB &&
        static_cast<unsigned char>(str[2]) == 0xBF) {
        return str.substr(3);
    }
    return str;
}

int main() {
    ScientificFile file("data/test.csv", ios::in);
    vector<double> numbers = {};
    bool firstLine = true;
    while (!file.eof()) {
        string line = file.read_line();
        if (firstLine) {
            line = removeBOM(line);
            firstLine = false;
        }
        stringstream s(line);

        string num;

        while (getline(s, num, ',')) {
            numbers.push_back(stod(num));
        }
    }
    copy(numbers.begin(), numbers.end(), ostream_iterator<double>(cout, " "));
    return 0;
}