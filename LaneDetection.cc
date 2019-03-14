#include "LaneDetection.hh"

/**
 *@brief Pre-process the input image using grayscale and gaussian blur in order to 
 *@brief reduce the noises. 
 *
 * */

cv::Mat LaneDetection::PreProcessing(cv::Mat input_image)
{
    /* Applies the Grayscale transform. This will return an image with only 
            one color channel*/
    cv::Mat gray_scaled;
    cv::cvtColor(input_image, gray_scaled, cv::COLOR_RGB2GRAY);

    cv::Mat smoothed_image;
    int kernel_size = 5;

    /* Applies a Gaussian smoothing function */
    cv::GaussianBlur(gray_scaled, smoothed_image, cv::Size(kernel_size, kernel_size), 0);

    return smoothed_image;
}

/**
 *@brief Apply the OpenCV Edge detection  in the blurred frame by filtering it.
 *
 * */

cv::Mat LaneDetection::EdgeDetection(cv::Mat smoothed_image)
{
    cv::Mat edges_detected_image;
    int min_threshold = 100;
    int max_threshold = 200;

    cv::Canny(smoothed_image, edges_detected_image, min_threshold, max_threshold);

    return edges_detected_image;
}

/**
 *@brief Defining the region of Interest by applying a mask to the endges image   
 *@brief so that only the edges that form part of the lane are detected.
 *
 * */

cv::Mat LaneDetection::RegionOfInterestDetection(cv::Mat edges, int height, int width)
{

    cv::Mat mask = cv::Mat::zeros(edges.size(), edges.type());
    cv::Point pts[3] = {
        cv::Point(0, height),
        cv::Point(width / 2, height / 2),
        cv::Point(width, height),
    };

    cv::fillConvexPoly(mask, pts, 3, cv::Scalar(255, 0, 0));

    cv::Mat masked_image;

    /*returning the image only where mask pixels are nonzero*/
    cv::bitwise_and(edges, mask, masked_image);

    return masked_image;
}

/**
 * 
 * 
 **/

std::vector<cv::Vec4i> LaneDetection::HoughLines(cv::Mat masked_image)
{
    double rho = 1;
    double theta = CV_PI / 180;
    int threshold = 30;
    double min_line_len = 20;
    double max_line_gap = 20;

    std::vector<cv::Vec4i> lines;

    cv::HoughLinesP(masked_image, lines, rho, theta, threshold, min_line_len, max_line_gap);

    return lines;
}

/**
 * 
 * 
 **/

cv::Mat LaneDetection::DrawLines(cv::Mat original_image, std::vector<cv::Vec4i> lines, cv::Mat masked_image)
{
    cv::Mat line_image;
    line_image = cv::Mat::zeros(original_image.size(), original_image.type());

    for (size_t i = 0; i < lines.size(); i++)
    {
        cv::Vec4i l = lines[i];
        cv::line(line_image, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(255, 0, 0), 2, CV_AA);
    }
    double alpha = 0.8;
    double beta = 1;
    double gamma = 0;

    cv::Mat out;
    cv::addWeighted(original_image, alpha, line_image, beta, gamma, out);

    return out;
}