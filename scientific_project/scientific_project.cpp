#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <functional>

// Golden section search method for finding minimum of function f(x) on interval [a, b]
// Returns the minimum point and function value at that point
std::pair<double, double> goldenSectionSearch(
    std::function<double(double)> f, // function to minimize
    double a,                        // left boundary
    double b,                        // right boundary
    double tol = 1e-6,               // tolerance
    int maxIter = 1000               // maximum iterations
) {
    // Golden ratio φ = (1+√5)/2
    const double phi = (1.0 + std::sqrt(5.0)) / 2.0; // ≈1.618
    const double invPhi = 1.0 / phi;                 // ≈0.618
    const double invPhi2 = 1.0 / (phi * phi);        // ≈0.382

    double x1, x2; // division points
    double f1, f2; // function values at division points

    // Initialize division points
    x1 = b - invPhi * (b - a);
    x2 = a + invPhi * (b - a);

    // First function evaluations
    f1 = f(x1);
    f2 = f(x2);

    int iter = 0;

    // Main iteration loop
    while (std::abs(b - a) > tol && iter < maxIter) {
        if (f1 > f2) {
            // Minimum is in the right part [x1, b]
            a = x1;
            x1 = x2;
            f1 = f2;
            x2 = a + invPhi * (b - a);
            f2 = f(x2);
        }
        else {
            // Minimum is in the left part [a, x2]
            b = x2;
            x2 = x1;
            f2 = f1;
            x1 = b - invPhi * (b - a);
            f1 = f(x1);
        }
        iter++;
    }

    // Return midpoint of final interval and function value
    double xMin = (a + b) / 2.0;
    double fMin = f(xMin);

    return std::make_pair(xMin, fMin);
}

// Alternative implementation with intermediate results output
double goldenSectionMinimize(
    std::function<double(double)> f,
    double a,
    double b,
    double eps = 1e-6,
    bool verbose = false
) {
    const double phi = (1.0 + std::sqrt(5.0)) / 2.0;
    const double invPhi = 1.0 / phi;

    // Initial points
    double x1 = b - invPhi * (b - a);
    double x2 = a + invPhi * (b - a);

    double f1 = f(x1);
    double f2 = f(x2);

    if (verbose) {
        std::cout << "Starting golden section search:" << std::endl;
        std::cout << "Interval: [" << a << ", " << b << "]" << std::endl;
        std::cout << "Tolerance: " << eps << std::endl << std::endl;
    }

    int iteration = 0;

    while (std::abs(b - a) > eps) {
        iteration++;

        if (verbose) {
            std::cout << "Iteration " << iteration << ":" << std::endl;
            std::cout << "  Interval: [" << a << ", " << b
                << "], length: " << b - a << std::endl;
            std::cout << "  x1 = " << x1 << ", f(x1) = " << f1 << std::endl;
            std::cout << "  x2 = " << x2 << ", f(x2) = " << f2 << std::endl;
        }

        if (f1 > f2) {
            // Minimum in right part
            a = x1;
            x1 = x2;
            f1 = f2;
            x2 = a + invPhi * (b - a);
            f2 = f(x2);
            if (verbose) std::cout << "  Minimum in right part" << std::endl;
        }
        else {
            // Minimum in left part
            b = x2;
            x2 = x1;
            f2 = f1;
            x1 = b - invPhi * (b - a);
            f1 = f(x1);
            if (verbose) std::cout << "  Minimum in left part" << std::endl;
        }

        if (verbose) std::cout << std::endl;
    }

    double result = (a + b) / 2.0;

    if (verbose) {
        std::cout << "Result:" << std::endl;
        std::cout << "  Found minimum at x = " << result << std::endl;
        std::cout << "  f(x) = " << f(result) << std::endl;
        std::cout << "  Iterations required: " << iteration << std::endl;
    }

    return result;
}

// Example usage
int main() {
    // Example 1: Minimize quadratic function f(x) = x^2 - 4x + 5
    auto f1 = [](double x) { return x * x - 4 * x + 5; };

    std::cout << "Example 1: Minimizing f(x) = x^2 - 4x + 5" << std::endl;
    auto result1 = goldenSectionSearch(f1, 0, 5, 1e-6);
    std::cout << "Minimum at x = " << result1.first
        << ", f(x) = " << result1.second << std::endl;
    std::cout << "Expected minimum: x = 2, f(x) = 1" << std::endl << std::endl;

    // Example 2: Minimize function f(x) = cos(x) on [0, 2π]
    auto f2 = [](double x) { return std::cos(x); };

    std::cout << "Example 2: Minimizing f(x) = cos(x) on [0, 2π]" << std::endl;
    double min2 = goldenSectionMinimize(f2, 0, 2 * M_PI, 1e-6, true);
    std::cout << "Found minimum: x = " << min2
        << ", f(x) = " << f2(min2) << std::endl;
    std::cout << "Expected minimum: x = π, f(x) = -1" << std::endl << std::endl;

    // Example 3: Minimize function with multiple extrema
    auto f3 = [](double x) { return std::sin(x) + 0.5 * x; };

    std::cout << "Example 3: Minimizing f(x) = sin(x) + 0.5*x on [-5, 5]" << std::endl;
    auto result3 = goldenSectionSearch(f3, -5, 5, 1e-8);
    std::cout << "Minimum at x = " << result3.first
        << ", f(x) = " << result3.second << std::endl;

    return 0;
}