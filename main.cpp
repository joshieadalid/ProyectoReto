#include <cmath>
#include <functional>
#include <opencv2/core/types.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "Pearson.h"

cv::Point2f bottomLeft(const std::vector<cv::Point2f> &samples) {
    cv::Point2f bottomLeft{};
    for (cv::Point2f sample: samples) {
        bottomLeft = {std::min(bottomLeft.x, sample.x), std::min(bottomLeft.y, sample.y)};
    }
    return bottomLeft;
}

cv::Point2f topRight(const std::vector<cv::Point2f> &samples) {
    cv::Point2f topRight{};
    for (cv::Point2f sample: samples) {
        topRight = {std::max(topRight.x, sample.x), std::max(topRight.y, sample.y)};
    }
    return topRight;
}

std::vector<cv::Point2f> generateSamples(const std::function<float(float)> &f, float from, float to, float step) {
    std::vector<cv::Point2f> samples;
    auto numSamples = static_cast<unsigned int>((to - from) / step);
    for (size_t i = 0U; i <= numSamples; ++i) {
        float x = from + static_cast<float>(i) * step;
        float y = f(x);
        (void) samples.emplace_back(x, y);
    }
    return samples;
}

std::vector<cv::Point> scaleSample(const std::vector<cv::Point2f> &samples, cv::Size imageSize) {
    cv::Point2f sampleBottomLeft = bottomLeft(samples);
    cv::Point2f sampleTopRight = topRight(samples);

    cv::Point2f dataOffset;
    dataOffset.x = sampleTopRight.x - sampleBottomLeft.x;
    dataOffset.y = 0.0F;

    // El ancho de x, es la escala para x y y.
    cv::Point2f sampleScale;
    float deltaX = static_cast<float>(imageSize.width) / (sampleTopRight.x - sampleBottomLeft.x);
    float deltaY = static_cast<float>(imageSize.height) / (sampleTopRight.y - sampleBottomLeft.y);
    sampleScale = {deltaX, deltaY};

    // Define la función de transformación
    auto transform = [&](cv::Point2f sample) {
        cv::Point scaledSample;
        scaledSample.x = static_cast<int>((sample.x - dataOffset.x) * sampleScale.x) +
                         static_cast<int>(static_cast<double>(imageSize.width));
        scaledSample.y = static_cast<int>((sample.y - dataOffset.y) * -sampleScale.y) + (imageSize.height);
        return scaledSample;
    };

    // Aplica la transformación a cada elemento del vector de muestras
    std::vector<cv::Point> scaledSamples;
    (void) std::transform(samples.begin(), samples.end(), std::back_inserter(scaledSamples), transform);
    return scaledSamples;
}

void draw(const std::vector<cv::Point> &points, cv::Mat &image) {
    cv::polylines(image, points, false, cv::Scalar(255.0, 255.0, 255.0), 1, 8, 0);

    //Eje horizontal
    cv::line(image, {0, image.size().height}, {image.size().width, image.size().height}, cv::Scalar(0.0, 0.0, 255.0), 3,
             8, 0);

    // Eje vertical
    cv::line(image, {0, 0}, {0, image.size().height}, cv::Scalar(0.0, 255.0, 0.0), 2, 8, 0);
}

void drawFunctions(float from, float to, float step, cv::Mat image) {
    std::function<float(float)> parabola = [](float x) { return  sin(x); };
    std::vector<cv::Point2f> execSamples = executionTimes(100U, sampleFunctionDependingN);
    std::vector<cv::Point2f> samples = generateSamples(parabola, from, to, step);
    draw(scaleSample(execSamples, image.size()), image);
    draw(scaleSample(samples, image.size()), image);

    cv::namedWindow("plot", cv::WINDOW_AUTOSIZE);
    cv::imshow("plot", image);
    (void) cv::imwrite("draw.png", image);
}

int main() {
    cv::Mat image(cv::Size{500, 500}, CV_8UC3);
    drawFunctions(0.F, 5.0F, 0.1F, image);

    (void) cv::waitKey(0);

    return 0;
}
