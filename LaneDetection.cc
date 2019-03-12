#include "LaneDetection.hpp"

/**
 *@brief Apply the OpenVv gaussian filter to the input image in order to 
 *@brief reduce the noises. 
 *
 * */

cv::Mat LaneDetection::ReduceNoises(cv::Mat input_image)
{
    cv::Mat denoised_image;
    return cv::GaussianBlur(input_image, denoised_image, cv::Size(3, 3), 0, 0);
}

/**
 *@brief Apply the OpenCV Edge detection  in the blurred frame by filtering it.
 *
 * */

cv::Mat LaneDetection::EdgeDetection(cv::Mat denoised_image)
{
    cv::Mat edges;

    /* Applies the Grayscale transform. This will return an image with only 
            one color channel*/
    cv::cvtColor(denoised_image, edges, cv::COLOR_RGB2GRAY);
    // Binarize gray image
    cv::threshold(edges, edges, 140, 255, cv::THRESH_BINARY);

    return edges;
}

/**
 *@brief Defining the region of Interest by masking the image so that only  
 *@brief the edges that form part of the lane are detected.
 *
 * */

cv::Mat LaneDetection::ApplyMask(cv::Mat edges, int height, int width)
{

    cv::Mat mask = cv::Mat::zeros(edges.size(), img_edges.type());
    cv::Point pts[3] = {
        cv::Point(0, height),
        cv::Point(width / 2, height / 2),
        cv::Point(width, height),
        // cv::Point(1280, 720)};

    cv::fillConvexPoly(mask, pts, 3, cv::Scalar(255, 0, 0));

    cv::Mat masked_image;

    cv::bitwise_and(edges, mask, masked_image);

    return masked_image;
}