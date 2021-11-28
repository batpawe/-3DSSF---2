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
| Pyramids | 1196 |13.992360| 302078442 | 25.21 |
| Bicubic interpolation | 531  | 13.990716 | 302007448 | 25.21 |
| Nearest Neighbor | 204 | 14.620173  | 329794029 | 24.83 |
| Bilinear Interpolation | 210  | 13,856663 | 296247751 | 25.29 |
| Guided Joint bilateral filter | 996329  | 14.600095 | 328888849 | 24.84 |

![Set 1](1.png?raw=true "Title")

### Set 2
| Upsampling method | Microseconds | RMSE | SSD | PSNR |
|--|--|--|--|--|
| Pyramids | 98 |15.960355| 42986182 | 24.06 |
| Bicubic interpolation | 148  | 15.580437 | 40964066 | 24.27 |
| Nearest Neighbor | 70 | 16.560461  | 46279495 | 23.74 |
| Bilinear Interpolation | 85  | 15.674798 | 41461757 | 24.22 |
| Guided Joint bilateral filter | 996329  | 21.702905 | 79483965 | 21.40 |

![Set 2](2.png?raw=true "Title")


### Set 3
| Upsampling method | Microseconds | RMSE | SSD | PSNR |
|--|--|--|--|--|
| Pyramids | 95 |16.458340| 45710488 | 23.80 |
| Bicubic interpolation | 144  | 15.940810 | 42880965 | 24.08 |
| Nearest Neighbor | 70 | 17.113439  | 49421779 | 23.74 |
| Bilinear Interpolation | 76  | 16.088521 | 43679335 | 24.00 |
| Guided Joint bilateral filter | 112322  | 20.786934 | 72916305 | 21.77 |

![Set 3](3.png?raw=true "Title")


### Set 4
| Upsampling method | Microseconds | RMSE | SSD | PSNR |
|--|--|--|--|--|
| Pyramids | 629 |13.090580| 243853393 | 25.79 |
| Bicubic interpolation | 286  | 13.074575 | 243257447 | 25.80 |
| Nearest Neighbor | 151 | 13.733252  | 268384729 | 25.37 |
| Bilinear Interpolation | 237  | 12.913076 | 237285082 | 25.91|
| Guided Joint bilateral filter | 911235  | 9.174269 | 119771616 | 28.87 |

![Set 4](4.png?raw=true "Title")

### Set 5
| Upsampling method | Microseconds | RMSE | SSD | PSNR |
|--|--|--|--|--|
| Pyramids | 631 |12.369720| 212641032 | 26.28 |
| Bicubic interpolation | 354  | 12.740376 | 225575418 | 26.02 |
| Nearest Neighbor | 141 | 13.165571 | 240883311 | 25.74 |
| Bilinear Interpolation | 111  | 12.446151 | 215276902 | 26.23 |
| Guided Joint bilateral filter | 881840  | 12.653169 | 222497898 | 26.08 |

![Set 5](5.png?raw=true "Title")

### Set 6
| Upsampling method | Microseconds | RMSE | SSD | PSNR |
|--|--|--|--|--|
| Pyramids | 665 |14.049773| 287471921 | 25.17 |
| Bicubic interpolation | 290  | 14.455255 | 304304474 | 24.93 |
| Nearest Neighbor | 184 | 15.199287 | 336436627 | 24.49 |
| Bilinear Interpolation | 177  | 14.340984 | 299512323 | 24.99 |
| Guided Joint bilateral filter | 961519  | 12.711220 | 235305067 | 26.04 |

![Set 6](6.png?raw=true "Title")


### Set 7
| Upsampling method | Microseconds | RMSE | SSD | PSNR |
|--|--|--|--|--|
| Pyramids | 224 |10.758835| 54011983 | 27.49 |
| Bicubic interpolation | 291  | 10.999370 | 56454071 |27.30 |
| Nearest Neighbor | 108 | 11.322470 | 59819389 | 27.05 |
| Bilinear Interpolation | 90  | 10.592534 | 52355143 | 27.63 |
| Guided Joint bilateral filter | 297336  | 15.528339 | 112514798 | 24.30 |

![Set 7](7.png?raw=true "Title")

### Set 8
| Upsampling method | Microseconds | RMSE | SSD | PSNR |
|--|--|--|--|--|
| Pyramids | 215 |11.000845| 54896455 |27.30 |
| Bicubic interpolation | 292  | 11.481489 | 59798273 |26.93 |
| Nearest Neighbor | 106 | 12.100614 | 66421248 | 26.47 |
| Bilinear Interpolation | 104  | 11.201854 | 56920970 | 27.14 |
| Guided Joint bilateral filter | 285823  | 23.097404 | 242001765 | 20.85 |

![Set 8](8.png?raw=true "Title")

### Set 9
| Upsampling method | Microseconds | RMSE | SSD | PSNR |
|--|--|--|--|--|
| Pyramids | 207 |10.968375| 56032175 |27.32 |
| Bicubic interpolation | 308  | 11.147586 | 57878133 |27.18 |
| Nearest Neighbor | 98 | 11.454452 | 61108478 | 26.95 |
| Bilinear Interpolation | 98  | 10.790806 | 54232627 | 27.46 |
| Guided Joint bilateral filter | 299558  | 21.362396 | 212545917 | 21.53 |

![Set 9](9.png?raw=true "Title")

### Set 10
| Upsampling method | Microseconds | RMSE | SSD | PSNR |
|--|--|--|--|--|
| Pyramids | 226 |10.681547| 53238763 |27.55 |
| Bicubic interpolation | 294  | 10.906091 | 55500623 |27.37 |
| Nearest Neighbor | 130 | 11.272970 | 59297492 | 27.09 |
| Bilinear Interpolation | 99  | 10.485704 | 51304420 | 27.71 |
| Guided Joint bilateral filter | 293039  |16.593996 | 128487714 | 23.73 |

![Set 10](10.png?raw=true "Title")

### Set 11
| Upsampling method | Microseconds | RMSE | SSD | PSNR |
|--|--|--|--|--|
| Pyramids | 219 |10.805926| 52968319 |27.45 |
| Bicubic interpolation | 323  | 11.474994 | 59730635 |26.93 |
| Nearest Neighbor | 103 | 11.900194 | 64239224 |26.61 |
| Bilinear Interpolation | 99  | 11.110091 | 55992184 | 27.21 |
| Guided Joint bilateral filter | 288297  |22.606673 | 231827790 | 21.04 |

![Set 11](11.png?raw=true "Title")

### Set 12
| Upsampling method | Microseconds | RMSE | SSD | PSNR |
|--|--|--|--|--|
| Pyramids | 215 |10.829905| 54727917 |27.43 |
| Bicubic interpolation | 293  | 11.061950 | 57098273 |27.25 |
| Nearest Neighbor | 107 | 11.412921 | 60778959 |26.98 |
| Bilinear Interpolation | 92  | 10.656861 | 52992962 | 27.57 |
| Guided Joint bilateral filter | 319612  |15.207826 | 107918008 |24.48 |

![Set 12](12.png?raw=true "Title")

## Optional tasks
As optional tasks I implemented some OpenMP pragmas to make the code run faster. These pragmas can be visible in file with subtasks 1,2 and 3.

## Conclusion
The results of the assignment are really weird for me. I can clearly see that guided bilateral upsampling works better than other implementations proposed by OpenCV. Of course it is way slower but the depth image is much more clear than others. Maybe it is the case of sharpening the image? I think that after tweaking some parameters it would be better to achieve greater results. Especially in metrics because right now although guided bilateral upsampling looks great it has usually the worst score of all of the upsampling methods. 
