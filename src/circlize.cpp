#include "circlize.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <opencv2/highgui/highgui.hpp>


namespace kg {


Circlize::Circlize(const cv::Mat& source)
    :originalImage_(source)
    ,width_(source.cols)
    ,height_(source.rows)
{
    std::random_device rnd;
    mt_ = std::mt19937(rnd());
}


Circlize::~Circlize()
{
}


auto Circlize::calcDistanceOfColor(const RGB& color1, const RGB& color2) -> int const
{
    int diffRed   = color1.r - color2.r;
    int diffGreen = color1.g - color2.g;
    int diffBlue  = color1.b - color2.b;

    return std::pow(diffRed, 2.0)
         + std::pow(diffGreen, 2.0)
         + std::pow(diffBlue, 2.0);
}


auto Circlize::calcEvaluationValue(const cv::Mat& source, const Circle::Circle& circle) -> double const
{
    auto pixels = 0;
    auto totalDiff = 0;
    for ( auto y = std::max(0, circle.y - circle.r); y < std::min(height_, circle.y + circle.r); y++ ) {
        for ( auto x = std::max(0, circle.x - circle.r); x < std::min(width_, circle.x + circle.r); x++ ) {
            auto baseIndex = source.step*y + COLOR_CHANNEL*x;
            auto r = source.data[baseIndex + RED_INDEX];
            auto g = source.data[baseIndex + GREEN_INDEX];
            auto b = source.data[baseIndex + BLUE_INDEX];

            RGB color(r, g, b);
            totalDiff += calcDistanceOfColor(color, circle.color);
            ++pixels;
        }
    }

    return static_cast<double>(totalDiff)/static_cast<double>(pixels);
}


auto Circlize::generateNormalRandomValue(double median, double variance) -> double
{
    std::normal_distribution<> norm(median, variance);

    return norm(mt_);
}

auto Circlize::generateUniformRandomValue(double min, double max) -> double
{
    std::uniform_real_distribution<> uniform(min, max);

    return uniform(mt_);
}

auto Circlize::convertValueInRange(double value, double min, double max) -> double const
{
    auto result = value;
    if ( value < min ) {
        result = min;
    } else if ( value > max ) {
        result = max;
    }

    return result;
}


auto Circlize::execute() -> void
{
    constexpr auto numCircle  = 10;
    const auto circleVariance = std::min(width_, height_)/8;
    const auto colorVariance  = 30;
    const auto maxR           = std::min(width_, height_)/20;
    const std::string windowName("Circlize");

    std::vector<int> defaultColor(COLOR_CHANNEL);
    for ( auto& color : defaultColor ) {
        color = static_cast<int>(generateUniformRandomValue(0, 255));
    }
    RGB color(defaultColor[0], defaultColor[1], defaultColor[2]);
    Circle lastCircle(width_/2, height_/2, generateNormalRandomValue(maxR/2, circleVariance), color);

    auto workingImage = cv::Mat(cv::Size(width_, height_), CV_8UC3, cv::Scalar::all(255));
    while ( 1 ) {
        auto duplicatedImage = workingImage.clone();

        auto minEvaluationValue = 3*255.0;
        auto bestCircleIndex = 0;
        std::vector<Circle> circles;
        for ( auto i = 0; i < numCircle; i++ ) {
            auto x = static_cast<int>(convertValueInRange(generateNormalRandomValue(lastCircle.x, circleVariance), 0, width_ - 1));
            auto y = static_cast<int>(convertValueInRange(generateNormalRandomValue(lastCircle.y, circleVariance), 0, height_ - 1));
            auto r = static_cast<int>(convertValueInRange(generateNormalRandomValue(lastCircle.r, circleVariance), 1, maxR));

            RGB& lastColor = lastCircle.color;
            int red   = static_cast<int>(convertValueInRange(generateNormalRandomValue(lastColor.r, colorVariance), 0, 255));
            int green = static_cast<int>(convertValueInRange(generateNormalRandomValue(lastColor.g, colorVariance), 0, 255));
            int blue  = static_cast<int>(convertValueInRange(generateNormalRandomValue(lastColor.b, colorVariance), 0, 255));
            RGB color(red, green, blue);

            Circle circle(r, x, y, color);
            circles.push_back(circle);
            cv::circle(duplicatedImage, cv::Point(x, y), r, cv::Scalar(blue, green, red), -1, CV_AA);
            cv::imshow(windowName, duplicatedImage);
            cv::waitKey(0);

            auto evaluationValue = calcEvaluationValue(originalImage_, circle);
            if ( evaluationValue < minEvaluationValue ) {
                minEvaluationValue = evaluationValue;
                bestCircleIndex = i;
            }
        }

        Circle& circle = circles[bestCircleIndex];
        RGB& color = circle.color;
        cv::circle(workingImage, cv::Point(circle.x, circle.y), circle.r, cv::Scalar(color.b, color.g, color.r), -1, CV_AA);
        cv::imshow(windowName, workingImage);
        cv::waitKey(1);

        lastCircle = circle;
    }
}


}

