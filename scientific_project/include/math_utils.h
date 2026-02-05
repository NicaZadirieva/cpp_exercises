#pragma once

#include <iostream>
#include <cmath>

// Константа для размера массива
const int ARRAY_LENGTH = 10;

// Функция для создания и заполнения массива
double* create_array();

// Функция для вычисления среднего значения массива
double calc_average(double* arr);

// Главная функция - точка входа
void entrypoint();