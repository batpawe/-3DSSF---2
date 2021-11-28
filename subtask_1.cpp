// Subtask nr 1
// Bilateral Filter

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>

#define M_PI 3.14159265358979323846

cv::Mat gaussianKernel(int window_size, float sigma = 1)
{
    cv::Mat gKernel(window_size, window_size, CV_32FC1);
    double k_sum = 0.0;
#pragma omp parallel for
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

cv::Mat filterBilateral(cv::Mat& img, int window, float sigma_g, float sigma_f) {
    cv::Mat result(img.rows, img.cols, img.type(), cv::Scalar(0));
    cv::Mat gKernel = gaussianKernel(window, sigma_g);

#pragma omp parallel for
    for (int i = window / 2; i < img.rows - window / 2; ++i) {
        for (int j = window / 2; j < img.cols - window / 2; ++j) {
            float window_sum = 0;
            float total_sum = 0;

            for (int x = -window / 2; x <= window / 2; ++x) {
                for (int y = -window / 2; y <= window / 2; ++y) {
                    float distance = std::abs((float)img.at<uchar>(i, j) - (float)img.at<uchar>(i + x, j + y));

                    float normalized_val = (1 / std::sqrt(2 * (float)M_PI) * sigma_g) * std::exp(-distance / (2 * sigma_g * sigma_g));
                    float w = normalized_val * gKernel.at<float>(x + window / 2, y + window / 2);

                    total_sum += (float)img.at<uchar>(i + x, j + y) * w;
                    window_sum += w;
                }
            }
            result.at<uchar>(i, j) = (int)(total_sum / window_sum);
        }
    }
    return result;
}

int main(){
    float sigma_g[4] = {0.01, 1, 10, 25};
    float sigma_f[4] = {0.01, 1, 10, 25};

    cv::Mat image = cv::imread("data/set_3/right.png", 0);
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            cv::Mat filteredImage;
            bilateralFilter(image, filteredImage, 20, sigma_g[i], sigma_f[j]);
            cv::imwrite( "data/bilateral/bilateral_filter_sg_" + std::to_string(i) + "_sf_" + std::to_string(j) + ".png", filteredImage);
        }
    }
    return 0;
}
