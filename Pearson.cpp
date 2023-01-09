#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
#include <functional>
#include <opencv2/core/types.hpp>

// Function to calculate the mean of a vector
double mean(const std::vector<int> &vector) {
    double sum = 0.0;
    for (int value: vector) {
        sum += static_cast<double>(value);
    }
    return sum / static_cast<double>(vector.size());
}

// Function to calculate the Pearson correlation coefficient
double correlation(const std::vector<int> &vector1, const std::vector<int> &vector2) {
    if (vector1.size() != vector2.size()) {
        std::cerr << "Error: Vectors must have the same size." << std::endl;
        return 0.0;
    }

    double mean1 = mean(vector1);
    double mean2 = mean(vector2);

    double sumOfProducts = 0.0;
    double sumOfSquares1 = 0.0;
    double sumOfSquares2 = 0.0;
    for (size_t i = 0U; i < vector1.size(); i++) {
        sumOfProducts += (static_cast<double>(vector1[i]) - mean1) * (static_cast<double>(vector2[i]) - mean2);
        sumOfSquares1 += (static_cast<double>(vector1[i]) - mean1) * (static_cast<double>(vector1[i]) - mean1);
        sumOfSquares2 += (static_cast<double>(vector2[i]) - mean2) * (static_cast<double>(vector2[i]) - mean2);
    }

    return sumOfProducts / sqrt(sumOfSquares1 * sumOfSquares2);
}

void sampleFunctionDependingN(unsigned int n) {
    for (size_t i = 0U; i < n; ++i) {
        std::cout << n << std::endl;
    }
}

std::vector<cv::Point2f> executionTimes(unsigned int iterations, const std::function<void(unsigned int)> &f) {
    std::vector<cv::Point2f> executionTimes;

    for (unsigned int i = 0U; i < iterations; ++i) {
        // Tu código
        auto start = std::chrono::high_resolution_clock::now();
        f(i);
        // Tu código a medir
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = (end - start).count();
        (void) executionTimes.emplace_back(i, elapsed);
    }
    return executionTimes;
}