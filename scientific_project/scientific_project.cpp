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
    cout << "Hello" << endl;
    return 0;
}