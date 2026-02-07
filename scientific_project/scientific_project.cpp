#include <iostream>
#include <Eigen/Dense>

int main() {
    std::cout << "=== Eigen Test ===\n\n";

    // 1. Создание матриц
    Eigen::MatrixXd A(2, 2);
    A << 1, 2,
        3, 4;

    Eigen::VectorXd b(2);
    b << 5, 6;

    std::cout << "Matrix A:\n" << A << "\n\n";
    std::cout << "Vector b:\n" << b << "\n\n";

    // 2. Решение системы уравнений
    Eigen::VectorXd x = A.colPivHouseholderQr().solve(b);
    std::cout << "Solution x = A\\b:\n" << x << "\n\n";

    // 3. Проверка
    std::cout << "Check A*x:\n" << A * x << "\n";
    std::cout << "Expected b:\n" << b << "\n\n";

    // 4. Другие операции
    std::cout << "A^T:\n" << A.transpose() << "\n\n";
    std::cout << "A*A:\n" << A * A << "\n\n";
    std::cout << "Determinant of A: " << A.determinant() << "\n";
    std::cout << "Inverse of A:\n" << A.inverse() << "\n";

    return 0;
}