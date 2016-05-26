#ifndef KG_CIRCLIZE_H
#define KG_CIRCLIZE_H


#include <random>
#include <opencv2/core/core.hpp>

namespace kg {

class Circlize {
private:
    enum {
        COLOR_CHANNEL = 3,
        RED_INDEX = 2, GREEN_INDEX = 1, BLUE_INDEX = 0,
    };

    typedef struct RGB {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        RGB(unsigned char r_=0, unsigned char g_=0, unsigned char b_=0) : r(r_), g(g_), b(b_) { }
    } RGB;

    typedef struct Circle {
        int r;
        int x;
        int y;
        RGB color;
        Circle(int r_=0, int x_=0, int y_=0, RGB color_=RGB(0, 0, 0)) : r(r_), x(x_), y(y_), color(color_) { }
    } Circle;

    const cv::Mat originalImage_;
    const int width_;
    const int height_;

    std::mt19937 mt_;

private:
    auto calcDistanceOfColor(const RGB& color1, const RGB& color2) -> int const;
    auto calcEvaluationValue(const cv::Mat& source, const Circle& circle) -> double const;
    auto generateNormalRandomValue(double median, double variance) -> double;
    auto generateUniformRandomValue(double min, double max) -> double;
    auto convertValueInRange(double value, double min, double max) -> double const;


public:
    Circlize(const cv::Mat& source);
    Circlize(const Circlize& rhs) = delete;
    ~Circlize();
    auto operator=(const Circlize& rhs) -> Circlize& = delete;

    auto execute() -> void;
};



}

#endif

