#include <iostream>
#include <string>
#include <memory>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "circlize.h"

auto main() -> int
{
    const std::string imageFilePath("sample.jpg");
    cv::Mat image = cv::imread(imageFilePath);
    if ( image.empty() ) {
        std::cerr << imageFilePath << " cannot be opened." << std::endl;
        std::exit(1);
    }

    auto circlize = std::make_unique<kg::Circlize>(image);
    circlize->execute();

    return 0;
}

