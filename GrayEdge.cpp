#include "GreyEdge.h"
#include <iostream>
#include <opencv2/opencv.hpp>


cv::Vec3d GreyEdge::grey_edge(cv::Mat& im, double mink_norm, double sigma){
    cv::Mat image = im.clone();
    cv::Mat gaussImage;
    cv::GaussianBlur(image, gaussImage, cv::Size(2 * sigma + 1, 2 * sigma + 1),1);

    //Derivative with laplance
    std::vector<cv::Mat> deriv_image(3);

    std::vector<cv::Mat> cahn(3);
    cv::split(gaussImage, cahn);

    for(int i = 0; i < 3; ++i){
        cv::Mat laplance_image;

        cv::Laplacian(cahn[i], laplance_image, CV_64F, 3);            
        laplance_image = cv::abs(laplance_image);
        deriv_image[i] = laplance_image;
        // deriv_image[i] = cv::abs(laplance_image);
    }


    // Saturated pixels
    for (int channel = 0; channel < 3; ++channel) {
        cv::Mat deriv_channel = deriv_image[channel].clone();
        cv::Mat image_channel;
        cv::extractChannel(image, image_channel, channel);

        for (int row = 0; row < image_channel.rows; ++row) {
            for (int col = 0; col < image_channel.cols; ++col) {
                if (image_channel.at<uchar>(row, col) >= 255) {
                    deriv_channel.at<double>(row, col) = 0.0;
                }
            }
        }

        deriv_image[channel] = deriv_channel;
    }


    //Estimation
    cv::Vec3d illum;
    for (int channel = 0; channel < 3; ++channel) {

        cv::Mat powered;
        cv::pow(deriv_image[channel], mink_norm, powered);

        long sum = cv::sum(powered)[0];
        sum = std::pow(sum, 1.0/mink_norm);
        illum[channel] = sum;
    }

    cv::Mat powered;
    cv::pow(illum, 2, powered);
    double sum = std::sqrt(cv::sum(powered)[0]);
    illum = illum / sum; 

    return illum;
}