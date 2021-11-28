// Subtask nr 2
// Guided Bilateral Filter Upsampling

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>

#define M_PI 3.14159265358979323846

cv::Mat gaussianKernel(int window_size, float sigma)
{
    cv::Mat gKernel(window_size, window_size, CV_32FC1);
    double k_sum = 0.0;
    for (int i = 0; i < window_size; i++) {
        for (int j = 0; j < window_size; j++) {
            double kp_val = std::exp(-(float)(i * i + j * j) / (2 * sigma * sigma))
                            / (2 * M_PI * sigma * sigma);
            gKernel.at<float>(i, j) = (float)kp_val;
            k_sum += kp_val;
        }
    }
#pragma omp parallel for
    for (int i = 0; i < window_size; i++) {
        for (int j = 0; j < window_size; j++) {
            gKernel.at<float>(i, j) /= (float)k_sum;
        }
    }
    return gKernel;
}

cv::Mat filterBilateral(cv::Mat& img, cv::Mat& depth_image, int window, float sigma_g, float sigma_f) {
    cv::Mat result(img.rows, img.cols, img.type(), cv::Scalar(0));
    cv::Mat gKernel = gaussianKernel(window, sigma_g);

    for (int i = window / 2; i < img.rows - window / 2; ++i) {
        for (int j = window / 2; j < img.cols - window / 2; ++j) {
            float window_sum = 0;
            float total_sum = 0;

            for (int x = -window / 2; x <= window / 2; ++x) {
                for (int y = -window / 2; y <= window / 2; ++y) {
                    float distance = std::abs((float)img.at<uchar>(i, j) - (float)img.at<uchar>(i + x, j + y));

                    float normalized_val = (1 / std::sqrt(2 * (float)M_PI) * sigma_f) * std::exp(-distance / (2 * sigma_f * sigma_f));
                    float w = normalized_val * gKernel.at<float>(x + window / 2, y + window / 2);

                    total_sum += (float)depth_image.at<uchar>(i + x, j + y) * w;
                    window_sum += w;
                }
            }
            result.at<uchar>(i, j) = (int)(total_sum / window_sum);
        }
    }
    return result;
}

cv::Mat upsampleImage(cv::Mat& image, cv::Mat& depth_image, int window, float sigma_g, float sigma_f){
    int factor = std::log2(image.rows / depth_image.rows);
    cv::Mat upsampled_image = depth_image.clone();
    cv::Mat rgb_copy = image.clone();
    for (int f = 0; f < factor; ++f)
    {
        cv::resize(upsampled_image, upsampled_image, upsampled_image.size() * 2);
        cv::resize(rgb_copy, rgb_copy, upsampled_image.size());
        upsampled_image = filterBilateral(rgb_copy, upsampled_image, window, sigma_g, sigma_f);
    }
    cv::resize(upsampled_image, upsampled_image, image.size());
    upsampled_image = filterBilateral(image, upsampled_image, window, sigma_g, sigma_f);
    return upsampled_image;
}

int main(){
    cv::Mat image = cv::imread("data/set_2/normal_res.png", 0);
    cv::Mat depth_image = cv::imread("data/set_2/lowres_depth.png", 0);
    cv::Mat filteredImage;
    filteredImage = upsampleImage(image, depth_image, 5, 3, 1);
    cv::imwrite( "data/set_2/upsampled_image.png", filteredImage);
    cv::imshow("Upsampling", filteredImage);
    cv::waitKey();
    return 0;
}
