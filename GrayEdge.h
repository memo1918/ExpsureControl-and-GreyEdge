#include <opencv2/opencv.hpp>

class GreyEdge {
    public:
        cv::Vec3d grey_edge(cv::Mat& im, double mink_norm = 1, double sigma = 1);

};