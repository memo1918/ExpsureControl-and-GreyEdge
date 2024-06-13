
#include <opencv2/opencv.hpp>
#include <vector>

class GainController {
public:
    cv::Mat calcHistogmra(const cv::Mat& image);
    float imageBr (const cv::Mat& hist, int pixels);
    float imageCr (const cv::Mat& image, float imageBr, int pixels);
    std::vector<float> cumHist(const cv::Mat& hist, int pixels);
    float findWSL(std::vector<float>& cumHist, int oer);
    float findBSL(std::vector<float>& cumHist, int nr);
    int findIndex(const std::vector<float>& H, float c);
    float increaseGain(const std::vector<float>& cumhist, float gain, float idwsl,float wsl);
    float decreaseGain(float wsl,float evMax, float evDec, float ST_);
};
