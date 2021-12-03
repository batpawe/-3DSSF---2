// Subtask nr 3
// Evaluation and comparison
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>

#define M_PI 3.14159265358979323846
// Bilateral Upsampling from subtask nr 2 --------------------------------------------------------------------------
cv::Mat gaussianKernel(int window, float sigma_g)
{
    cv::Mat gKernel(window, window, CV_32FC1);
    double k_sum = 0.0;
    for (int i = 0; i < window; i++) {
        for (int j = 0; j < window; j++) {
            double kp_val = std::exp(-(float)(i * i + j * j) / (2 * sigma_g * sigma_g))
                            / (2 * M_PI * sigma_g * sigma_g);
            gKernel.at<float>(i, j) = (float)kp_val;
            k_sum += kp_val;
        }
    }
#pragma omp parallel for
    for (int i = 0; i < window; i++) {
        for (int j = 0; j < window; j++) {
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

cv::Mat upsampleImage(cv::Mat& img, cv::Mat& depth_image, int window, float sigma_g, float sigma_f){
    int factor = std::log2(img.rows / depth_image.rows);
    cv::Mat upsampled_image = depth_image.clone();
    cv::Mat rgb_copy = img.clone();
    for (int f = 0; f < factor; ++f)
    {
        cv::resize(upsampled_image, upsampled_image, upsampled_image.size() * 2);
        cv::resize(rgb_copy, rgb_copy, upsampled_image.size());
        upsampled_image = filterBilateral(rgb_copy, upsampled_image, window, sigma_g, sigma_f);
    }
    cv::resize(upsampled_image, upsampled_image, img.size());
    upsampled_image = filterBilateral(img, upsampled_image, window, sigma_g, sigma_f);
    return upsampled_image;
}
// Bilateral Upsampling from subtask nr 2 --------------------------------------------------------------------------

// COMPARISON TECHNIQUES
// Root Mean square error
double RMSE(cv::Mat& img1, cv::Mat& img2)
{
    int size = img1.rows * img1.cols;
    double rmse = 0;
    double diff = 0;
    for (int r = 0; r < img1.rows; ++r) {
        for (int c = 0; c < img1.cols; ++c) {
            diff = img1.at<uchar>(r, c) - img2.at<uchar>(r, c);
            rmse += diff * diff;
        }
    }
    double mse = (double)(rmse / size);
    return sqrt(mse);
}

// Sum squared diff
double SSD(cv::Mat& img1, cv::Mat& img2)
{
    double ssd = 0;
    double diff = 0;
    for (int r = 0; r < img1.rows; ++r) {
        for (int c = 0; c < img1.cols; ++c) {
            diff = img1.at<uchar>(r, c) - img2.at<uchar>(r, c);
            ssd += diff * diff;
        }
    }
    return ssd;
}

// Peak signal to noise ratio
double PSNR(cv::Mat& img1, cv::Mat& img2)
{

    double max = 255;
    int size = img1.rows * img1.cols;
    double ssd = 0;
    double diff = 0;
    for (int r = 0; r < img1.rows; ++r) {
        for (int c = 0; c < img1.cols; ++c) {
            diff = img1.at<uchar>(r, c) - img2.at<uchar>(r, c);
            ssd += diff * diff;
        }
    }
    double mse = (double)(ssd / size);
    double psnr = 10 * log10((max * max) / mse);
    return psnr;
}

int main(){
    for(int i = 1; i <= 12; i++){
        cv::Mat rgb_img = cv::imread("data/set_" + std::to_string(i) + "/left.png", 0);
        cv::Mat depth_image = cv::imread("data/set_" + std::to_string(i) + "/disp.png", 0);
        cv::Mat processed_image;

        // downsample image 4 times
        cv::pyrDown(depth_image, processed_image);
        cv::pyrDown(processed_image, processed_image);
        cv::pyrDown(processed_image, processed_image);
        cv::pyrDown(processed_image, processed_image);
        cv::Mat noise_image(processed_image.size(), processed_image.type());
        cv::randn(noise_image, 10, 5);
        processed_image = processed_image + noise_image;
        cv::imwrite( "data/set_" + std::to_string(i) + "/res_0.png", processed_image);

        auto start = std::chrono::high_resolution_clock::now();
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);

        std::cout << "Set_"+ std::to_string(i) << std::endl;

        cv::Mat pyramids(depth_image.size(), depth_image.type());
        start = std::chrono::high_resolution_clock::now();
        cv::pyrUp(processed_image, pyramids, cv::Size(processed_image.cols*2, processed_image.rows*2));
        cv::pyrUp(pyramids, pyramids, cv::Size(pyramids.cols*2, pyramids.rows*2));
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
        std::cout << "Pyramids upsampling time: " << duration.count() << " microseconds" << std::endl;
        std::cout << "Metrics:" << std::endl;
        std::cout << "RMSE: " + std::to_string(RMSE(depth_image, pyramids)) << std::endl;
        std::cout << "SSD: " + std::to_string(SSD(depth_image, pyramids)) << std::endl;
        std::cout << "PSNR: " + std::to_string(PSNR(depth_image, pyramids)) << std::endl;
        cv::imwrite( "data/set_" + std::to_string(i) + "/res_1.png", pyramids);

        cv::Mat bicubic_interpolation(depth_image.size(), depth_image.type());
        start = std::chrono::high_resolution_clock::now();
        cv::resize(processed_image, bicubic_interpolation, depth_image.size(), 0, 0,cv::INTER_CUBIC);
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
        std::cout << "Bicubic Interpolation upsampling time: " << duration.count() << " microseconds" << std::endl;
        std::cout << "Metrics:" << std::endl;
        std::cout << "RMSE: " + std::to_string(RMSE(depth_image, bicubic_interpolation)) << std::endl;
        std::cout << "SSD: " + std::to_string(SSD(depth_image, bicubic_interpolation)) << std::endl;
        std::cout << "PSNR: " + std::to_string(PSNR(depth_image, bicubic_interpolation)) << std::endl;
        cv::imwrite( "data/set_" + std::to_string(i) + "/res_2.png", bicubic_interpolation);

        cv::Mat nearest_neighbor(depth_image.size(), depth_image.type());
        start = std::chrono::high_resolution_clock::now();
        cv::resize(processed_image, nearest_neighbor, depth_image.size(), 0, 0,cv::INTER_NEAREST);
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
        std::cout << "Nearest neighbor upsampling time: " << duration.count() << " microseconds" << std::endl;
        std::cout << "Metrics:" << std::endl;
        std::cout << "RMSE: " + std::to_string(RMSE(depth_image, nearest_neighbor)) << std::endl;
        std::cout << "SSD: " + std::to_string(SSD(depth_image, nearest_neighbor)) << std::endl;
        std::cout << "PSNR: " + std::to_string(PSNR(depth_image, nearest_neighbor)) << std::endl;
        cv::imwrite( "data/set_" + std::to_string(i) + "/res_3.png", nearest_neighbor);

        cv::Mat bilinear_interpolation(depth_image.size(), depth_image.type());
        start = std::chrono::high_resolution_clock::now();
        cv::resize(processed_image, bilinear_interpolation, depth_image.size(), 0, 0,cv::INTER_LINEAR);
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
        std::cout << "Bilinear interpolation upsampling time: " << duration.count() << " microseconds" << std::endl;
        std::cout << "Metrics:" << std::endl;
        std::cout << "RMSE: " + std::to_string(RMSE(depth_image, bilinear_interpolation)) << std::endl;
        std::cout << "SSD: " + std::to_string(SSD(depth_image, bilinear_interpolation)) << std::endl;
        std::cout << "PSNR: " + std::to_string(PSNR(depth_image, bilinear_interpolation)) << std::endl;
        cv::imwrite( "data/set_" + std::to_string(i) + "/res_4.png", bilinear_interpolation);

        cv::Mat filteredImage;
        start = std::chrono::high_resolution_clock::now();
        filteredImage = upsampleImage(processed_image, depth_image, 5, 3, 1);
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
        std::cout << "Guided bilateral filter upsampling time: " << duration.count() << " microseconds" << std::endl;
        std::cout << "Metrics:" << std::endl;
        std::cout << "RMSE: " + std::to_string(RMSE(depth_image, filteredImage)) << std::endl;
        std::cout << "SSD: " + std::to_string(SSD(depth_image, filteredImage)) << std::endl;
        std::cout << "PSNR: " + std::to_string(PSNR(depth_image, filteredImage)) << std::endl;
        cv::imwrite( "data/set_" + std::to_string(i) + "/res_5.png", filteredImage);

        std::cout << std::endl;
    }
    return 0;
}
