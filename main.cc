#include "LaneDetection.hh"

int main(int argc, char const *argv[])
{
    // cv::VideoCapture capture("../Data/test.jpg");

    // if (!capture.isOpened())
    // {
    //     std::cout << "Error opening video stream or file" << std::endl;
    //     return EXIT_FAILURE;
    // }
    cv::Mat input_frame;

    input_frame = cv::imread("../Data/solidWhiteCurve.jpg");

    if (!input_frame.empty())
    {
        LaneDetection lanedetection;

        cv::Mat blured_image;
        blured_image = lanedetection.PreProcessing(input_frame);

        cv::Mat edges;
        edges = lanedetection.EdgeDetection(blured_image);

        cv::Mat masked_image;
        int frame_width = input_frame.cols;
        int frame_height = input_frame.rows;

        masked_image = lanedetection.RegionOfInterestDetection(edges, frame_height, frame_width);

        
        std::vector<cv::Vec4i> lines;
        lines = lanedetection.HoughLines(masked_image);

        cv::Mat line_image;
        line_image = lanedetection.DrawLines(input_frame, lines, masked_image);

        cv::namedWindow("Original_image", cv::WINDOW_AUTOSIZE);
        cv::imshow("Original_image", input_frame);

        cv::namedWindow("Lines", cv::WINDOW_AUTOSIZE);
        cv::imshow("Lines", line_image);

        cv::waitKey();
        cv::destroyAllWindows();
    }
    else
    {
        std::cout << "Error loading frame!" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
