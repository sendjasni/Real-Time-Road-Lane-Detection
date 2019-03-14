#include "LaneDetection.hh"
#include "realtime_task.hh"

int main(int argc, char const *argv[])
{

    ptask_init(SCHED_FIFO, PARTITIONED, NO_PROTOCOL);

    cv::namedWindow(INITIAL_IMAGE_WINDOW_NAME, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(EDGE_IMAGE_WINDOW_NAME, cv::WINDOW_AUTOSIZE);

    CheckCapturingDevice();

    int processor_id = 0;
    tpars params;

    params = TaskCreate(processor_id);
    CheckTaskCreation(StartTask(params, CapturingImageTask));
    processor_id++;

    params = TaskCreate(processor_id);
    CheckTaskCreation(StartTask(params, DisplyingImageTask));
    processor_id++;

    params = TaskCreate(processor_id);
    CheckTaskCreation(StartTask(params, LaneDetectionTask));

    ptask_activate(0);
    ptask_activate_at(1, PER, MILLI);
    ptask_activate_at(2, PER, MILLI);

    // int frame_count = int(capture.get(cv::CAP_PROP_FRAME_COUNT));
    // std::cout << " Frame count : " << frame_count << std::endl;

    int exit_char = getchar();
    int instance = 0;
    while (exit_char != ESC)
    {
        exit_char = getchar();
    }

    cv::destroyAllWindows();
    
    TasksStatisticComputing(processor_id);


    // cv::Mat input_frame;

    // input_frame = cv::imread("../Data/solidWhiteCurve.jpg");

    // if (!input_frame.empty())
    // {
    //     LaneDetection lanedetection;

    //     cv::Mat blured_image;
    //     blured_image = lanedetection.PreProcessing(input_frame);

    //     cv::Mat edges;
    //     edges = lanedetection.EdgeDetection(blured_image);

    //     cv::Mat masked_image;
    //     int frame_width = input_frame.cols;
    //     int frame_height = input_frame.rows;

    //     masked_image = lanedetection.RegionOfInterestDetection(edges, frame_height, frame_width);

    //     std::vector<cv::Vec4i> lines;
    //     lines = lanedetection.HoughLines(masked_image);

    //     cv::Mat line_image;
    //     line_image = lanedetection.DrawLines(input_frame, lines, masked_image);

    //     cv::namedWindow("Original_image", cv::WINDOW_AUTOSIZE);
    //     cv::imshow("Original_image", input_frame);

    //     cv::namedWindow("Lines", cv::WINDOW_AUTOSIZE);
    //     cv::imshow("Lines", line_image);

    //     cv::waitKey();
    //     cv::destroyAllWindows();
    // }
    // else
    // {
    //     std::cout << "Error loading frame!" << std::endl;
    //     return EXIT_FAILURE;
    // }

    return EXIT_SUCCESS;
}
