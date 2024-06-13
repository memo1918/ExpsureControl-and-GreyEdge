#include "GainController.h"
#include "GreyEdge.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>


int main(){
    float ST_ = 0.06;       //Threshold
    int cMax_ = 100;        //max contrast
    int OER_ =  10;         // 
    int NR_ = 40;           //
    float evMax = 24.0;     //Maximum rate to decrease gain
    float evDec = 0.6;
    float gain = 20.0; 
    float mink_norm_ = 5.0;
    float sigma_ = 5.0;

    GainController gainController;
    GreyEdge greyEdge;

    cv::Mat image = cv::imread("image.jpg");

    cv::Mat gray_image;
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
    int pixels = gray_image.rows * gray_image.cols;

    
    cv::Mat hist = gainController.calcHistogmra(gray_image);
    std::vector<float> cumhist = gainController.cumHist(hist, pixels);
    float imagebr = gainController.imageBr(hist, pixels);
    float imagecr = gainController.imageCr(gray_image, imagebr, pixels);
    float wsl = gainController.findWSL(cumhist, OER_);
    float bsl = gainController.findBSL(cumhist, NR_);
    float idwsl = cumhist[gainController.findIndex(cumhist,1-ST_)];
    
    if ( wsl < (1-ST_)){
        gain = gainController.increaseGain(cumhist, gain, idwsl, wsl);
    } else if (bsl > ST_ | imagecr < cMax_) {
        gain += gainController.decreaseGain(wsl, evMax, evDec, ST_);
    }
    std::cout<< "Gain: " << gain << std::endl;

    
    cv::Vec3d grey_edge_illum = greyEdge.grey_edge(image, mink_norm_, sigma_); 
    std::cout << "Grey-Edge illumination: " << grey_edge_illum << std::endl;

}