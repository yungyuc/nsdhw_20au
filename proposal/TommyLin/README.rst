============================
Digital Image Focus Stacking
============================

Wiki: A digital image processing technique which combines multiple images taken
at different focus distances to give a resulting image with a greater depth of
field (DOF) than any of the individual source images.


Basic Information
=================

Git hub repository:
https://github.com/TommyLin/focus-stacking


Problem To Solve
================

When macro photography, the short depth of field (DOF) makes clear image only
in a small area because of very short distance to subject. To get a clear
(focused) image in a wide range, the photographer can shrink and use small
aperture size. But shrinking aperture increase diffraction effect that also
cause image blurry. So there are limit to get a clear photo with one shot.
<img src="./wiki/Dof_blocks_f1_4.jpg" width="200"><img src="./wiki/Dof_blocks_f4_0.jpg" width="200"><img src="./wiki/Dof_blocks_f22.jpg" width="200">
![]<(./wiki/Dof_blocks_f1_4.jpg =200x125)
Aperture size: f/1.4 - f/4.0 - f/22
source: https://en.wikipedia.org/wiki/Depth_of_field

Fortunately, photographer can take multiple "digital" photos and stack these
photos together to make a photo focused.
<img src="./wiki/750px-Focus_stacking_Tachinid_fly.jpg" width="600">
source: https://en.wikipedia.org/wiki/Circle_of_confusion

Definition of depth of field (DOF)
![DOF Equation](./DOF_equation.png  "DOF = 2 x u^2 x N x c / f^2")

+--------+----------------------+--------------------------------+
| Symbol | Definition           | Example                        |
+========+======================+================================+
| c      | circle of confusion  | Canon APS-C camera CoC=0.018mm |
+--------+----------------------+--------------------------------+
| f      |  focal length        | EF100mm f/2.8L MACRO IS USM    |
+--------+----------------------+--------------------------------+
| N      |  f-number (N=f/D)    | f/2.8, f/4, f/10, f/16         |
+--------+----------------------+--------------------------------+
| u      |  distance to subject | 5cm ~ 20cm for Camera          |
+--------+----------------------+--------------------------------+


Algorithm
=========
Steps to perform focus stacking
1. Perform a Gaussian blur on each image with following kernel:
   1 2 1
   2 4 2
   1 2 1
2. Perform a Laplacian Edge Detection kernel on each Gaussian Blurred image:
   -1 -1 -1
   -1  8 -1
   -1 -1 -1
3. Take absolute value of Laplacian of Gaussian (LoG) result. This will quantify
   the strength of edges with respect to the size and strength of kernel.
4. Create a blank image, loop through each pixel and fine the strongest edge in
   the LoG(i.e. the highest value in the image stack) and take the RGB value for
   that pixel from the corresponding image.


Prospective Users
=================

Macro photography and optical microscopy users.


System Architecture
===================

Input: Directory that store images with different focus distance.
Step 1: Read image from files
Step 2: Gaussian blur
Step 3: Laplacian Edge Detection
Step 4: Image stacking


API Description
===============

Show how your system can be programmed.  You are supposed to implement the
system using both C++ and Python.  Describe how a user writes a script in the
system.

Auto scan mode: Users select specific directory that contain photos. The system 
will scan directory and determinate the groups of photos.


Engineering Infrastructure
==========================

Describe how you plan to put together the build system, testing framework, and
documentation.  Show how you will do version control.

You may use continuous integration, but it is not required.  If you use it,
describe how it works in your code development.

Schedule
========

* Week 1: [Python] Browse directory and read image
* Week 2: [C++] Gaussian blur
* Week 3: [C++] Laplacian edge detection
* Week 4: [C++] Image stacking
* Week 5: [Python] Generate defocused photos
* Week 6: [C++] Compare stacked photo with source photo (SSIM)
* Week 7: Setup testing environment
* Week 8: CI integration


References
==========

1. https://en.wikipedia.org/wiki/Focus_stacking
2. https://en.wikipedia.org/wiki/Depth_of_field
3. https://en.wikipedia.org/wiki/Circle_of_confusion
4. https://en.wikipedia.org/wiki/Structural_similarity
