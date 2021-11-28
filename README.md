# 3DSSF 2nd assignment


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
