#include "include/scientific_dataset.h"
#include <iostream>
#include <cmath>
#include <random>

// Создание тестового набора данных физического эксперимента
ScientificDataset create_physics_experiment() {
    // Столбцы: время, скорость, ускорение, энергия
    std::vector<std::string> columns = {
        "time_s",
        "velocity_ms",
        "acceleration_ms2",
        "kinetic_energy_j"
    };

    // Генерируем данные для 1000 измерений
    std::vector<double> data;
    data.reserve(1000 * 4);

    std::mt19937 gen(42); // Детерминированный генератор для воспроизводимости
    std::normal_distribution<> velocity_dist(10.0, 2.0); // Средняя скорость 10 м/с
    std::normal_distribution<> accel_dist(0.0, 1.0);    // Среднее ускорение 0 м/с²

    const double mass = 2.0; // Масса объекта в кг

    for (int i = 0; i < 1000; ++i) {
        double time = i * 0.01; // 0.01 секунды между измерениями
        double velocity = velocity_dist(gen);
        double acceleration = accel_dist(gen);
        double kinetic_energy = 0.5 * mass * velocity * velocity;

        data.push_back(time);
        data.push_back(velocity);
        data.push_back(acceleration);
        data.push_back(kinetic_energy);
    }

    return ScientificDataset(columns, data);
}

// Создание тестового набора данных химической реакции
ScientificDataset create_chemistry_experiment() {
    // Данные для кинетики реакции
    std::vector<std::string> columns = {
        "temperature_k",
        "pressure_pa",
        "concentration_mol_l",
        "reaction_rate",
        "ph_value"
    };

    std::vector<double> data;

    // Генерируем данные для разных температур
    for (double temp = 273.15; temp <= 373.15; temp += 10.0) {
        for (double pressure = 101325.0; pressure <= 202650.0; pressure += 20265.0) {
            double concentration = std::exp(-1000.0 / temp) * 1.0;
            double reaction_rate = concentration * std::exp(-5000.0 / temp);
            double ph = 7.0 + 0.1 * std::sin(temp / 50.0);

            data.push_back(temp);
            data.push_back(pressure);
            data.push_back(concentration);
            data.push_back(reaction_rate);
            data.push_back(ph);
        }
    }

    return ScientificDataset(columns, data);
}

// Создание простого набора для отладки
ScientificDataset create_simple_dataset() {
    std::vector<std::string> columns = { "x", "y", "z" };

    // Простые данные: синусоидальная волна
    std::vector<double> data;
    for (int i = 0; i < 10; ++i) {
        data.push_back(i * 0.1);           // x
        data.push_back(std::sin(i * 0.1)); // y = sin(x)
        data.push_back(std::cos(i * 0.1)); // z = cos(x)
    }

    return ScientificDataset(columns, data);
}

int main() {
    std::cout << "=== Creating test binary files for ScientificDataset ===\n\n";

    // 1. Создаем простой набор для отладки
    {
        ScientificDataset simple = create_simple_dataset();
        simple.print_info();

        if (simple.saveBinary("test_simple_dataset.bin")) {
            std::cout << "\n✅ Saved simple dataset to: test_simple_dataset.bin\n";

            // Проверяем загрузку
            ScientificDataset loaded;
            if (loaded.loadBinary("test_simple_dataset.bin")) {
                std::cout << "✅ Successfully loaded simple dataset\n";
                loaded.print_info();
            }
        }
    }

    std::cout << "\n" << std::string(50, '=') << "\n\n";

    // 2. Создаем физический эксперимент
    {
        ScientificDataset physics = create_physics_experiment();
        std::cout << "Physics experiment dataset:\n";
        std::cout << "Rows: " << physics.rows() << ", Columns: " << physics.cols() << "\n";

        if (physics.saveBinary("test_physics_experiment.bin")) {
            std::cout << "✅ Saved physics dataset to: test_physics_experiment.bin\n";

            // Проверка целостности
            ScientificDataset loaded;
            if (loaded.loadBinary("test_physics_experiment.bin")) {
                std::cout << "✅ Verified physics dataset integrity\n";
                std::cout << "Loaded rows: " << loaded.rows() << ", columns: " << loaded.cols() << "\n";

                // Сравнение первых нескольких значений
                auto original_col = physics.get_column(1); // velocity
                auto loaded_col = loaded.get_column(1);

                bool match = true;
                for (size_t i = 0; i < std::min(original_col.size(), loaded_col.size()); ++i) {
                    if (std::abs(original_col[i] - loaded_col[i]) > 1e-10) {
                        match = false;
                        break;
                    }
                }

                if (match) {
                    std::cout << "✅ Data matches perfectly!\n";
                }
            }
        }
    }

    std::cout << "\n" << std::string(50, '=') << "\n\n";

    // 3. Создаем химический эксперимент
    {
        ScientificDataset chemistry = create_chemistry_experiment();
        std::cout << "Chemistry experiment dataset:\n";
        std::cout << "Rows: " << chemistry.rows() << ", Columns: " << chemistry.cols() << "\n";

        if (chemistry.saveBinary("test_chemistry_experiment.bin")) {
            std::cout << "✅ Saved chemistry dataset to: test_chemistry_experiment.bin\n";
        }
    }

    // 4. Тестируем обработку ошибок
    {
        ScientificDataset empty;
        if (!empty.saveBinary("")) {
            std::cout << "\n✅ Correctly handled empty filename\n";
        }

        if (!empty.loadBinary("non_existent_file.bin")) {
            std::cout << "✅ Correctly handled non-existent file\n";
        }
    }

    std::cout << "\n=== All test files created successfully ===\n";
    std::cout << "Files created:\n";
    std::cout << "1. test_simple_dataset.bin\n";
    std::cout << "2. test_physics_experiment.bin\n";
    std::cout << "3. test_chemistry_experiment.bin\n";

    return 0;
}