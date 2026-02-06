#include "../include/matrix.h"
#include <iostream>

int main() {
    std::cout << "=== Matrix Demo ===\n\n";

    // Create a 3x3 matrix
    Matrix A(3, 3);

    // Fill with values
    int counter = 1;
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            A(i, j) = counter++;
        }
    }

    std::cout << "Matrix A (3x3):\n";
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            std::cout << A(i, j) << " ";
        }
        std::cout << "\n";
    }

    // Test copy
    Matrix B = A;
    std::cout << "\nMatrix B (copy of A):\n";
    for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
            std::cout << B(i, j) << " ";
        }
        std::cout << "\n";
    }

    // Modify B
    B(0, 0) = 100;
    std::cout << "\nAfter modifying B(0,0) to 100:\n";
    std::cout << "A(0,0) = " << A(0, 0) << " (should still be 1)\n";
    std::cout << "B(0,0) = " << B(0, 0) << " (should be 100)\n";

    // Test move
    Matrix C = std::move(B);
    std::cout << "\nAfter moving B to C:\n";
    std::cout << "C rows: " << C.rows() << " (should be 3)\n";
    std::cout << "B rows: " << B.rows() << " (should be 0 after move)\n";

    return 0;
}



