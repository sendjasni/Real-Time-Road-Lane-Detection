#include "realtime_task.hh"
#include "LaneDetection.hh"

cv::VideoCapture capture("../Data/solidYellowLeft.mp4"); /* open the input
                                                                     file */
int frame_index_write = 0;
int frame_index_read = 1;
int frame_index_read_ = 2;

pthread_mutex_t frame_rw = PTHREAD_MUTEX_INITIALIZER;              /*  thread mutex 
                                                            variable */
pthread_cond_t synch_condition = PTHREAD_COND_INITIALIZER;         /*thread condition 
                                                            variable*/
LaneDetection lanedetection;
cv::Mat frames[3];
cv::Mat blured_image;
cv::Mat edges;
cv::Mat masked_image;
int frame_width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
int frame_height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
int frame_count = capture.get(CV_CAP_PROP_FRAME_COUNT);
std::vector<cv::Vec4i> lines;
cv::Mat line_image;

int SwapIndex(int *fi)
{
    return *fi = (*fi + 1) % 3;
}

void CheckCapturingDevice()
{
    if (!capture.isOpened()) /* check if the camera device is open */
    {
        std::cout << "Error opening video stream or file" << std::endl;
        exit(EXIT_FAILURE);
    }
}

tpars TaskCreate(int processor_id)
{
    tpars params;

    params = TASK_SPEC_DFL;
    params.period = tspec_from(PER, MILLI);
    params.priority = PRIORITY;
    params.measure_flag = 1;
    params.act_flag = DEFERRED;
    params.processor = processor_id;

    return params;
}

/*
  Checking the tasks creation status
    */
int CheckTaskCreation(int task_index)
{
    if (task_index != -1)
    {
        std::cout << "The task " << task_index << " was created successfully"
                  << std::endl;
        return EXIT_SUCCESS;
    }
    else
    {
        std::cout << " ERROR! task not created !!!" << std::endl;
        exit(EXIT_FAILURE);
    }
}

int StartTask(tpars task_params, void (*task_body)(void))
{
    return ptask_create_param(task_body, &task_params);
}

/*
    Display runing tasks information
    */

void DisplayTasksInstances(int job_id)
{
    std::cout << "The job " << job_id << " of Task T" << ptask_get_index()
              << " is running on core " << sched_getcpu() << " at time : "
              << ptask_gettime(MILLI) << std::endl;
}

/* 
    The capturing task' body
    */
ptask CapturingImageTask()
{

    int task_job = 0;

    while (task_job <= frame_count)
    {
        DisplayTasksInstances(task_job);
        pthread_mutex_lock(&frame_rw);

        capture >> frames[frame_index_write];
        SwapIndex(&frame_index_write);

        pthread_cond_broadcast(&synch_condition); /* After capturing the frame
                                                    signal the displaying task*/
        pthread_mutex_unlock(&frame_rw);

        ptask_wait_for_period();
        task_job++;
    }
}

/*
    The displaying task' body
    */

ptask DisplyingImageTask()
{

    int task_job = 0;

    while (task_job <= frame_count)
    {
        DisplayTasksInstances(task_job);
        pthread_mutex_lock(&frame_rw);
        pthread_cond_wait(&synch_condition, &frame_rw); /*wait for the capturing
                                                          func to send a signal*/
        if (!frames[frame_index_read].empty())
        {
            cv::imshow(INITIAL_IMAGE_WINDOW_NAME, frames[frame_index_read]);
            SwapIndex(&frame_index_read);

            cv::waitKey(1);
        }
        else
        {
            std::cout << "Frame reading error" << std::endl;
        }

        pthread_mutex_unlock(&frame_rw);

        ptask_wait_for_period();
        task_job++;
    }
}

ptask LaneDetectionTask()
{
    int task_job = 0;

    while (task_job <= frame_count)
    {
        DisplayTasksInstances(task_job);
        pthread_mutex_lock(&frame_rw);
        pthread_cond_wait(&synch_condition, &frame_rw); /*wait for the capturing
                                                          func to send a signal*/
        if (!frames[frame_index_read_].empty())
        {

            blured_image = lanedetection.PreProcessing(frames[frame_index_read_]);

            edges = lanedetection.EdgeDetection(blured_image);

            masked_image = lanedetection.RegionOfInterestDetection(edges, frame_height, frame_width);

            lines = lanedetection.HoughLines(masked_image);

            line_image = lanedetection.DrawLines(frames[frame_index_read_], lines, masked_image);

            cv::imshow(EDGE_IMAGE_WINDOW_NAME, line_image);
            SwapIndex(&frame_index_read_);

            cv::waitKey(1);

        }
        else
        {
            std::cout << "Error loading frame!" << std::endl;
        }
        pthread_mutex_unlock(&frame_rw);

        ptask_wait_for_period();
        task_job++;
    }
}

/* 
   Compute the worst case execution and average time
   for the created tasks
*/

void TasksStatisticComputing(int nbr_tasks)
{
    for (int task_index = 0; task_index <= nbr_tasks; task_index++)
    {
        tspec wcet;
        wcet = ptask_get_wcet(task_index);

        tspec avg;
        avg = ptask_get_avg(task_index);

        std::cout << "Task " << task_index << " : WCET = " << tspec_to(&wcet, MILLI)
                  << " AVG = " << tspec_to(&avg, MILLI) << " NINST = "
                  << ptask_get_numinstances(task_index) << std::endl;
    }
}