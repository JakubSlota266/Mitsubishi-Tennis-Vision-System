# Distributed Vision System
## The goal of this project is to set up a distributed data acquisition system for two basler cameras, using Raspberry PI, OpenCV, Pylon and Deep Learning Toolbox from Matlab.
### The algorithm of current approach consist following steps:  
-> Setup a camera object  
-> Grab a photo and perform HSV transformation  
-> Select the area of interest  
-> Extract x,y coordinates of center of rectangle  
-> Send x,y via TCP to Matlab to use neural network to transform 2D coordinates into 3D model.  
