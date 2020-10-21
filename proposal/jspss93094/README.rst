===========================
Proposal for Video Compression
===========================

Basic information
=================

`My GitHub repository (URL) <https://github.com/jspss93094/nsd_TermProject>`

Use C++ with Python to implement the video compression, and the file type can be various.

Problem to solve
================

Frequently, the videos are taken up much space if we wanted to save it, download it, 
or do something. It must took a long time to deal with a video. Even its size will be 
larger when the resolution is higher.

Some problems I met when I want to compress the video:

1. It often needed to install the software.
2. It took a long time to compress, but the result was not better than before.
3. The resolution became too low to watch.

Perspective users
=================

The person who wanted to compress the large video.

System architecture
===================

It also have many modules can use, like FFMPEG. But I want to realize the 
process and to implement it.
If some process in Python is better than in C++, I will embed some Python.

I just use C++ to create the function to complie and execute, or the file 
which can be imported in Python.

The function to implement : 

1. Get the input.
2. Decode the input video.
3. Do the calculation of compression.
4. Encode it to be the video type.
5. Output the result and save.

API description
===============

If users want to use my program in the program by themself, it can be easy to 
include the file in C++. Just download my file and include it in your header.

I will also provide the file which can be imported in Python. It can use by 
just call the functions.

Maybe I will make the graphical user interface which is easier to use by the general users.

Engineering infrastructure
==========================

Describe how you plan to put together the build system, testing framework, and
documentation.  Show how you will do version control.

You may use continuous integration, but it is not required.  If you use it,
describe how it works in your code development.

Schedule
========

Itemize the work to do and list the work items in a timeline.  Estimate the
efforts of each item.

References
==========

`An Introduction to Video Compression in C/C++ 
<https://www.yumpu.com/en/document/read/19876586/an-introduction-to-video-compression-in-c-c->`
