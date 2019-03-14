# Real_time_lane_detection

OpenCV implementation of a real time lane detection using the [PTask]((https://github.com/glipari/ptask#compiling)) library.

## Contents

- Introduction
- Dependencies
- Build

## Introduction

**Real_time_lane_detection** is a real time lane detection for  autonomous vehicle, using OpenCv as the computer vision processing library, and [**PTask**](https://github.com/glipari/ptask) as the task-related real-times properties manager.  

## Dependencies

- cmake >= 3.5
  - All OSes : [click here for installation instructions](https://cmake.org/install/)
- make >= 3.1
  - Linux : make is installed by default on most Linux distributions
- PTask
  - Periodic Real-Time Task interface facility to pthreads, [click here for installation instructions](https://github.com/glipari/ptask#compiling)
- OpenCv
  - Linux :  [click here for installation instructions](https://docs.opencv.org/3.4/d7/d9f/tutorial_linux_install.html)

## Basic Build Instructions

CMake is used as its build system for generating makefiles.

```shell
git clone https://github.com/sendjasni/Real-Time-Road-Lane-Detection.git
mkdir build && cd build
cmake .. && make
sudo ./lanedetection
```