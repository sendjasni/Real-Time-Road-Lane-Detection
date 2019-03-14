#pragma once

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

class LaneDetection
{

public:
  /* Apply Gaussian blurring to the input Image*/
  cv::Mat PreProcessing(cv::Mat input_image);

  /* Filter the image to obtain only edges*/
  cv::Mat EdgeDetection(cv::Mat img_noise);

  /* Mask the edges image to only care about region of Interest*/
  cv::Mat RegionOfInterestDetection(cv::Mat img_edges, int h, int w);

  /* Identify lines which indicate lane lines*/
  std::vector<cv::Vec4i> HoughLines(cv::Mat masked_image);

  /* Draw the dtected lines on the initial image */
  cv::Mat DrawLines(cv::Mat original_image, std::vector<cv::Vec4i> lines, cv::Mat masked_image);
};