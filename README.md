# 3DSSF 2nd assignment

This repository contains different algorithms about stereo vision matching, displaying and analysing. In particular C++ implementation contains features like:

 - Reading and saving images using OpenCV,
 - Guided by rgb image bilateral filter upsampling,
 - Bilateral filter implementation,
 - Different metrics (**SSD, RMSE, PSNR**),
 - Comparison between OpenCV upsampling techniques and my implementation of guided bilateral filtering upsampling,
 - **OpenMP** pragmas for faster runtime,

## How to run the code
In order to run the code you must generate binary file using cmake. CMakeLists.txt is provided in the repository. 

Beside that to run the code user has to have OpenCV library for C++.

## Subtask 1

In subtask 1 our goal was to implement Bilateral filter and further evaluate the result on the image of our choice. I used image from Middlebury stereo dataset and did 16 tests with different gaussian kernel and spectral filter values.

My values for these test were:

    int window_size = 5
    float sigma_g[4] = {1, 5, 25, 50};  
    float sigma_f[4] = {1, 5, 10, 15};

![Subtask 1 result image](MergedImages.png?raw=true "Title")


As it is clearly visible from the combined image with higher value of spectral filter bilateral filter preserve strong edges more than when the value is lower. Similarly when combined with high gaussian kernel sigma it result in preserving strong edges but blurring other regions.

## Subtask 2

In subtask nr 2 our goal was to create a guided filter from the bilateral filter in subtask 1. The result of the guided filter is to upsample low resolution depth image to be as close as possible to the ground truth of the image.

In my implementation I used these values of sigmas and window size:

    int window_size = 5
    float sigma_g = 3;  
    float sigma_f = 1;

These parameters led me to quite satisfactory results. I did the upsampling on two images from Middlebury stereo datasets:

Order:
Ground truth, downsampled image, upsampled with guided filter

![Subtask 2 result image](combine_images.png?raw=true "Title")

As it is visible the results are quite satisfactory and similar to the ground truth provided by Middlebury. Although it seems that these results are a little blurred it is still quite impresive that from lower resolution image this algorithm is giving us images so similar. Probably after tweaking a little bit more with the parameters it would be possible to achieve better results. In subtask nr 3 I will compare these results to other upsampling algorithms provided by OpenCV and evaluate them using different metrics.

## Subtask 3

In subtask nr 3 my goal was to compare different upsampling methods and evaluate them with image scores.

To achieve that I took 12 stereo pairs from two different datasets - Middlebury and Kitti. I tried to use disparity images from the 1st assignments but because of worse results I switched to the ground truth provided by both datasets. Of course the result were correct for my disparity images but to see more clearly the differences between different algorithms it was better to use ground truth.

The ground truth image was first downsampled to 4-times lower resolution with pyramDown function provided by OpenCV. It is the same algorithm as Pyramids used below but it was used first to lower the resolution instead of making it higher.

Then the image was noised by randn function provided by OpenCV as well.

**The order of each image is as follows:**

| Ground truth | Lower resolution image | Pyramids upsampling  |
|--|--|--|
| **Bicubic interpolation** | **Nearest neighbor** | **Bilinear interpolation** |
| **Guided bilateral filtering upsampling** | **Original rgb image** | **--** |


### Set 1
| Upsampling method | Microseconds | RMSE | SSD | PSNR |
|--|--|--|--|--|
| Pyramids | 1196 |17.992360| 302078442 | 25.21 |
| Bicubic interpolation | 531  | 18.990716 | 302007448 | 25.21 |
| Nearest Neighbor | 204 | 17.620173  | 329794029 | 24.83 |
| Bilinear Interpolation | 210  | 17,856663 | 296247751 | 25.29 |
| Guided Joint bilateral filter | 1321939  | 34.600095 | 1861244924 | 17.84 |

![Set 1](1n.png?raw=true "Title")

### Set 2
| Upsampling method | Microseconds | RMSE | SSD | PSNR |
|--|--|--|--|--|
| Pyramids | 98 |21.960355| 75712356 | 24.06 |
| Bicubic interpolation | 148  | 21.580437 | 80622493 | 24.27 |
| Nearest Neighbor | 70 | 21.560461  | 89679495 | 23.74 |
| Bilinear Interpolation | 85  | 21.674798 | 76712718 | 24.22 |
| Guided Joint bilateral filter | 996329  | 52.702905 | 466660928 | 13.40 |

![Set 2](2n.png?raw=true "Title")


### Set 3
| Upsampling method | Microseconds | RMSE | SSD | PSNR |
|--|--|--|--|--|
| Pyramids | 95 |21.458340| 45710488 | 23.80 |
| Bicubic interpolation | 144  | 21.940810 | 42880965 | 24.08 |
| Nearest Neighbor | 70 | 22.113439  | 49421779 | 23.74 |
| Bilinear Interpolation | 76  | 21.088521 | 43679335 | 24.00 |
| Guided Joint bilateral filter | 112322  | 51.786934 | 450135395 | 13.77 |

![Set 3](3n.png?raw=true "Title")


### Set 4
| Upsampling method | Microseconds | RMSE | SSD | PSNR |
|--|--|--|--|--|
| Pyramids | 629 |16.090580| 373185591 | 25.79 |
| Bicubic interpolation | 286  | 16.074575 | 363257447 | 25.80 |
| Nearest Neighbor | 151 | 17.733252  | 416539023 | 25.37 |
| Bilinear Interpolation | 237  | 16.913076 | 385289587 | 25.91|
| Guided Joint bilateral filter | 911235  | 23.174269 | 762612849 | 20.87 |

![Set 4](4n.png?raw=true "Title")

### Set 5
| Upsampling method | Microseconds | RMSE | SSD | PSNR |
|--|--|--|--|--|
| Pyramids | 631 |14.369720| 312641032 | 26.28 |
| Bicubic interpolation | 354  | 14.740376 | 310462526 | 26.02 |
| Nearest Neighbor | 141 | 13.165571 | 343077550 | 25.74 |
| Bilinear Interpolation | 111  | 14.446151 | 309571985 | 26.23 |
| Guided Joint bilateral filter | 881840  | 29.653169 | 1170017905 | 18.08 |

![Set 5](5n.png?raw=true "Title")

### Set 6
| Upsampling method | Microseconds | RMSE | SSD | PSNR |
|--|--|--|--|--|
| Pyramids | 665 |21.049773| 687471921 | 21.17 |
| Bicubic interpolation | 290  | 21.455255 | 670378435 | 21.93 |
| Nearest Neighbor | 184 | 22.199287 | 745402209 | 21.49 |
| Bilinear Interpolation | 177  | 21.340984 | 677020343 | 21.99 |
| Guided Joint bilateral filter | 961519  | 35.711220 | 1812172269 | 17.04 |

![Set 6](6n.png?raw=true "Title")


### Set 7
| Upsampling method | Microseconds | RMSE | SSD | PSNR |
|--|--|--|--|--|
| Pyramids | 224 |12.758835| 74011983 | 27.49 |
| Bicubic interpolation | 291  | 12.999370 | 76763691 |27.30 |
| Nearest Neighbor | 108 | 11.322470 | 66889826 | 27.05 |
| Bilinear Interpolation | 90  | 12.592534 | 62355143 | 27.63 |
| Guided Joint bilateral filter | 297336  | 15.528339 | 615979570 | 16.30 |

![Set 7](7n.png?raw=true "Title")

### Set 8
| Upsampling method | Microseconds | RMSE | SSD | PSNR |
|--|--|--|--|--|
| Pyramids | 215 |15.000845| 113896455 |24.30 |
| Bicubic interpolation | 292  | 16.481489 | 115087315 |23.93 |
| Nearest Neighbor | 106 | 15.100614 | 130369208 | 23.47 |
| Bilinear Interpolation | 104  | 16.201854 | 115272737 | 24.14 |
| Guided Joint bilateral filter | 285823  | 56.097404 | 1470299527 | 13.85 |

![Set 8](8n.png?raw=true "Title")

### Set 9
| Upsampling method | Microseconds | RMSE | SSD | PSNR |
|--|--|--|--|--|
| Pyramids | 207 |11.968375| 56032175 |26.32 |
| Bicubic interpolation | 308  | 12.147586 | 57878133 |27.18 |
| Nearest Neighbor | 98 | 11.454452 | 61108478 | 26.95 |
| Bilinear Interpolation | 98  | 12.790806 | 54232627 | 27.46 |
| Guided Joint bilateral filter | 299558  | 50.362396 | 1208752879 | 13.53 |

![Set 9](9n.png?raw=true "Title")

### Set 10
| Upsampling method | Microseconds | RMSE | SSD | PSNR |
|--|--|--|--|--|
| Pyramids | 226 |11.681547| 63238763 |27.55 |
| Bicubic interpolation | 294  | 10.906091 | 65626528 |27.37 |
| Nearest Neighbor | 130 | 11.272970 | 73267023 | 27.09 |
| Bilinear Interpolation | 99  | 10.485704 | 62880755 | 27.71 |
| Guided Joint bilateral filter | 293039  |39.593996 | 737103385 | 16.73 |

![Set 10](10n.png?raw=true "Title")

### Set 11
| Upsampling method | Microseconds | RMSE | SSD | PSNR |
|--|--|--|--|--|
| Pyramids | 219 |15.805926| 106968319 |27.45 |
| Bicubic interpolation | 323  | 15.474994 | 105596942 |26.93 |
| Nearest Neighbor | 103 | 14.900194 | 116965095 |26.61 |
| Bilinear Interpolation | 99  | 15.110091 | 103132080 | 27.21 |
| Guided Joint bilateral filter | 288297  |55.606673 | 1378297443 |13.04 |

![Set 11](11n.png?raw=true "Title")

### Set 12
| Upsampling method | Microseconds | RMSE | SSD | PSNR |
|--|--|--|--|--|
| Pyramids | 215 |12.829905| 67282987 |27.43 |
| Bicubic interpolation | 293  | 12.061950 | 75202724 |27.25 |
| Nearest Neighbor | 107 | 11.412921 | 69778959 |26.98 |
| Bilinear Interpolation | 92  | 12.656861 | 65384880 | 27.57 |
| Guided Joint bilateral filter | 319612  |15.207826 | 598183907 |17.48 |

![Set 12](12n.png?raw=true "Title")

## Optional tasks
As optional tasks I implemented some OpenMP pragmas to make the code run faster. These pragmas can be visible in file with subtasks 1,2 and 3.

## Conclusion
The results of the assignment are really weird for me. I can clearly see that guided bilateral upsampling works better than other implementations proposed by OpenCV. Of course it is way slower but the depth image is much more clear than others. Maybe it is the case of sharpening the image? I think that after tweaking some parameters it would be better to achieve greater results. Especially in metrics because right now although guided bilateral upsampling looks great it has usually the worst score of all of the upsampling methods. 
