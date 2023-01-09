//
// Created by joshieadalid on 2/01/23.
//

#include <vector>
#include <chrono>
#include <functional>

#ifndef GRAPHIC_PEARSON_H
#define GRAPHIC_PEARSON_H

#endif //GRAPHIC_PEARSON_H

double correlation(const std::vector<int>& vector1, const std::vector<int>& vector2);
std::vector<cv::Point2f> executionTimes(unsigned int iterations, const std::function<void (unsigned int)>& f);
void sampleFunctionDependingN(unsigned int n);