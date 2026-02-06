#include "../include/matrix.h"
#include <cassert>
#include <iostream>
#include <chrono>
#include <vector>
#include <random>

// Упрощенная версия тестов без Unicode символов
void test_basic_functionality() {
    std::cout << "=== Basic Matrix Tests ===\n";

    // Test 1: Constructor
    Matrix m1(3, 4);
    assert(m1.rows() == 3);
    assert(m1.cols() == 4);
    std::cout << "  [PASS] Constructor test\n";

    // Test 2: Element access
    m1(0, 0) = 1.0;
    m1(1, 1) = 2.0;
    m1(2, 2) = 3.0;

    assert(m1(0, 0) == 1.0);
    assert(m1(1, 1) == 2.0);
    assert(m1(2, 2) == 3.0);
    std::cout << "  [PASS] Element access test\n";

    // Test 3: Copy constructor
    Matrix m2 = m1;
    assert(m2.rows() == 3);
    assert(m2.cols() == 4);
    assert(m2(0, 0) == 1.0);
    std::cout << "  [PASS] Copy constructor test\n";

    // Test 4: Move constructor
    Matrix m3(std::move(m2));
    assert(m3.rows() == 3);
    assert(m3.cols() == 4);
    assert(m3(0, 0) == 1.0);
    assert(m2.rows() == 0); // m2 should be empty after move
    std::cout << "  [PASS] Move constructor test\n";

    // Test 5: Copy assignment
    Matrix m4(1, 1);
    m4 = m3;
    assert(m4.rows() == 3);
    assert(m4.cols() == 4);
    assert(m4(0, 0) == 1.0);
    std::cout << "  [PASS] Copy assignment test\n";

    // Test 6: Move assignment
    Matrix m5(2, 2);
    m5 = std::move(m4);
    assert(m5.rows() == 3);
    assert(m5.cols() == 4);
    assert(m5(0, 0) == 1.0);
    assert(m4.rows() == 0); // m4 should be empty after move
    std::cout << "  [PASS] Move assignment test\n";

    // Test 7: Exception handling
    try {
        m5(10, 10); // Should throw
        assert(false && "Should have thrown exception");
    }
    catch (const std::out_of_range& e) {
        std::cout << "  [PASS] Exception test: " << e.what() << "\n";
    }

    std::cout << "=== All tests passed! ===\n\n";
}

void test_advanced_scenarios() {
    std::cout << "=== Advanced Matrix Tests ===\n";

    // Test with larger matrix
    Matrix large(100, 100);
    for (size_t i = 0; i < 100; ++i) {
        for (size_t j = 0; j < 100; ++j) {
            large(i, j) = static_cast<double>(i * 100 + j);
        }
    }
    std::cout << "  [PASS] Large matrix creation\n";

    // Test deep copy
    Matrix copy = large;
    copy(0, 0) = 999.0;
    assert(large(0, 0) != 999.0); // Original should not change
    std::cout << "  [PASS] Deep copy test\n";

    // Test performance of move vs copy
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10; ++i) {
        Matrix temp = large; // Copy
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto copy_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10; ++i) {
        Matrix temp = std::move(large); // Move
        large = std::move(temp); // Move back
    }
    end = std::chrono::high_resolution_clock::now();
    auto move_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "  Copy time: " << copy_time.count() << "ms, Move time: " << move_time.count() << "ms\n";
    std::cout << "  [PASS] Performance test\n";

    std::cout << "=== Advanced tests passed! ===\n\n";
}

void test_vector_of_matrices() {
    std::cout << "=== Vector of Matrices Test ===\n";

    std::vector<Matrix> matrices;
    matrices.reserve(5);

    for (int i = 0; i < 5; ++i) {
        Matrix temp(50, 50);
        for (size_t r = 0; r < 50; ++r) {
            for (size_t c = 0; c < 50; ++c) {
                temp(r, c) = static_cast<double>(i * r * c);
            }
        }
        matrices.push_back(std::move(temp));
    }

    assert(matrices.size() == 5);
    assert(matrices[2].rows() == 50);
    std::cout << "  [PASS] Vector of matrices test\n";
    std::cout << "=== Vector test passed! ===\n\n";
}

int main() {
    try {
        std::cout << "========================================\n";
        std::cout << "        MATRIX CLASS TESTS\n";
        std::cout << "========================================\n\n";

        test_basic_functionality();
        test_advanced_scenarios();
        test_vector_of_matrices();

        std::cout << "========================================\n";
        std::cout << "        ALL TESTS PASSED SUCCESSFULLY!\n";
        std::cout << "========================================\n";

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "\n[FAIL] Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "\n[FAIL] Test failed with unknown exception" << std::endl;
        return 1;
    }
}