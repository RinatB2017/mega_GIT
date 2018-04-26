i-vizon trunk
=============
Code Area 89! 
A warm welcome to my code base :)

I'm learning how effectively we can use the available computing units on the embedded platform.

In my exploration phase, I have written some C++ wrappers for ease of use and also given some 
tips where ever possible under the tag @TIPS:

For newbies out of college, I hope this will be a good starting point.

I have tried to introduce as many tools/libraries I can, to make 
an end to end solutions.

Willing to contribute or any queries
reach me at mageswaran1989@gmail.com with subject "i-vizon:OpenCL"


Folder Structure:
=================
common	:	Common headers
utils	:	System utilities
docs	:	My notes
opencl	:	Wrappers and try outs 
gles	:	Wrappers and try outs
pcl	:	Wrappers and try outs
linux	:	Wrappers for linux APIs
c++11	:	For newbies
data	:	All data file needed for the exercises

General Structure:
==================
library_name/
	wrappers/
		inc/
		src/
		*.pri
	1.hello_world
		Qt/
			hello_world.pro
		hello_world.cpp
	2.exercise_two
	3.exercise_three
	4.exercise_four
	5.exercise_five
	
Naming Convention Followed:
===========================
Point Cloud C++ coding guide lines

FileNames:
==========
file_name_one.h/.cpp

Compiler and IDE used:
======================
gcc (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3
QtCreator 2.8.1

Software Libraries/SDKs Used:
=========================
1.OpenCL 1.1/1.2
2.OpenMP 3.0
3.OpenCV 2.4.8
4.Boost 1.54
5.Qt 4.8.0
6.PCL 1.7
I will leave the installation guide to Google.




Important:
==========
Code base is still in pre release stage, will take another few months to freeze the wrappers!
Right now it is upto you to explore the wrappers, till I generate Doxygen files.

ASSUMPTIONS:
============
*You are familiar with linux environment
*You know to use QtCreator for C++ projects
*Git is cloned in /opt/i-vizon/trunk

To Start:
=========
1.Open the *.pro file using QtCreator and configure the output folder location.
2.In qtcreator press Ctrl+5 and under "Build Steps" add CONFIG+=linuxPC in "Additional Arguments" section
3.If you have libraries already installed, its time to RUN and explore :)

