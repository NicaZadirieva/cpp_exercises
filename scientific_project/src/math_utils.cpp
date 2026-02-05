// 1. Создайте map<string, double> experiment_params
// 2. Добавьте: {"temperature", 298.15}, {"pressure", 1.013}, {"time", 3600.0}
// 3. Напишите функцию, которая выводит все параметры в формате "key = value units"
// 4. Проверьте наличие ключа "pH", если нет - добавьте со значением 7.0
#include "../include/math_utils.h"

using namespace std;

void print_experiment_params(map<string, double>& experiment_params) {
	for (const auto& [param_name, param_value] : experiment_params) {
		cout << param_name << " = " << param_value << " units" << endl;
	}
}
void entrypoint() {
	map<string, double> experiment_params = {};
	experiment_params["temperature"] = 298.15;
	experiment_params["pressure"] = 1.013;
	experiment_params["time"] = 3600.0;
	print_experiment_params(experiment_params);
	if (experiment_params["pH"] == NULL) {
		experiment_params["pH"] = 7.0;
		cout << "After adding pH: " << endl;
		print_experiment_params(experiment_params);
	}

}