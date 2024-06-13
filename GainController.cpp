#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include "GainController.h"


cv::Mat GainController::calcHistogmra(const cv::Mat& image){
    int histSize = 256; // Number of bins
    float range[] = { 0, 256 }; 
    const float* histRange = { range };
    bool uniform = true, accumulate = false;
    cv::Mat hist;
    cv::calcHist(&image, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
    return hist;
}

float GainController::imageBr (const cv::Mat& hist, int pixels){
    float sum;
    for(int i = 0; i<256; i++){
        float pixelValue = hist.at<uchar>(0,i);
        sum += i*pixelValue;
    }
    return sum/pixels;
}

float GainController::imageCr (const cv::Mat& image, float imageBr, int pixels){
    cv::Size image_size = image.size();
    float sum;
    for(int r = 0; r < image_size.width; r++){
        for(int c = 0; c < image_size.height; c++){
            int pixelValue = image.at<uchar>(c,r);
            float deviation = pixelValue - imageBr;
            sum += deviation * deviation; 
        }
    }
    float variance = sum / pixels; 
    return sqrt(variance);
}

std::vector<float> GainController::cumHist(const cv::Mat& hist, int pixels){
    std::vector<float> cumulativeHist(hist.rows);
    float sum = 0.0f;
    for (int i = 0; i < hist.rows; ++i) {
        sum += hist.at<float>(i);
        cumulativeHist[i] = sum / pixels; // Normalize to range [0, 1]
    }
    return cumulativeHist;
}

float GainController::findWSL(std::vector<float>& cumHist, int oer){
    return cumHist[255-oer];
}

float GainController::findBSL(std::vector<float>& cumHist, int nr){
    return cumHist[nr];
}

int GainController::findIndex(const std::vector<float>& H, float c) {
    //Binary search
    int low = 0;
    int high = H.size() - 1;
    int result = -1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (H[mid] >= c) {
            result = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return result;
}

float GainController::increaseGain(const std::vector<float>& cumhist, float gain, float idwsl,float wsl){
    float n_idwsl = cumhist[GainController::findIndex(cumhist,wsl)];
    return (n_idwsl*gain)/idwsl;
}

float GainController::decreaseGain(float wsl,float evMax, float evDec, float ST_){
    return 1/evMax + (wsl * (((1-ST_)*evDec)-evMax)/(evMax*(((1-ST_)*evDec))));
}

