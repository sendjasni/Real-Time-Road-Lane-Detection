#pragma once

#include <iostream>
#include <cstdlib>

extern "C"
{
#include <ptask.h>
#include <tstat.h>
#include <time.h>
}

#define PER 20
#define ESC 27
#define PRIORITY 10

#define INITIAL_IMAGE_WINDOW_NAME "Original capture"
#define EDGE_IMAGE_WINDOW_NAME "Edge image"

#define MODE_OFF 0
#define MODE_ON 1

void CheckCapturingDevice();
tpars TaskCreate(int processor_id);
int CheckTaskCreation(int task_index);
int StartTask(tpars task_params, void (*task_body)(void));
void DisplayTasksInstances(int job_id);
ptask CapturingImageTask();
ptask DisplyingImageTask();
ptask LaneDetectionTask();
void TasksStatisticComputing(int nbr_tasks);

