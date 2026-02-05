// scientific_project.cpp: определяет точку входа для приложения.
//

#include "scientific_project.h"
#include "include/scientific_data.h"
using namespace std;

int main()
{
	cout << "Hello CMake." << endl;
	ScientificData* scientificData = new ScientificData("power", "Vt");
	scientificData->addMeasurement(3.5);
	scientificData->addMeasurement(4.5);
	scientificData->addMeasurement(5.5);
	scientificData->addMeasurement(6.5);
	scientificData->addMeasurement(7.5);
	scientificData->addMeasurement(8.5);

	scientificData->saveToFile();
	return 0;
}
