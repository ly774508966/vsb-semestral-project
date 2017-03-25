#include "hashing.h"
#include "../utils/utils.h"
#include <opencv2/rgbd.hpp>

cv::Vec3d Hashing::extractSurfaceNormal(cv::Mat &src, cv::Point c) {
    float dzdx = (src.at<float>(c.y, c.x + 1) - src.at<float>(c.y, c.x - 1)) / 2.0f;
    float dzdy = (src.at<float>(c.y + 1, c.x) - src.at<float>(c.y - 1, c.x)) / 2.0f;
    cv::Vec3f d(-dzdy, -dzdx, 1.0f);

    return cv::normalize(d);
}

char Hashing::quantizeSurfaceNormals(cv::Vec3f normal) {
    // For quantization of surface normals into 8 bins, we're using faces of Octahedron
    // where each face represents 1 bin
    static cv::Vec3f octahedronUnitNormals[8] = {
        cv::Vec3f(1.0f, 0, 1.0f),
        cv::Vec3f(0, 1.0f, 1.0f),
        cv::Vec3f(-1.0f, 0, 1.0f),
        cv::Vec3f(0, -1.0f, 1.0f),
        cv::Vec3f(1.0f, 0, -1.0f),
        cv::Vec3f(0, 1.0f, -1.0f),
        cv::Vec3f(-1.0f, 0, -1.0f),
        cv::Vec3f(0, -1.0f, -1.0f),
    };



    return 0;
}

void trainingSetGeneration(cv::Mat &train, cv::Mat &trainDepth) {
//    auto rgbdNormals = cv::rgbd::RgbdNormals(trainDepth.rows, trainDepth.cols, CV_32F, );

    cv::Mat normals = cv::Mat::zeros(trainDepth.size(), CV_32FC3);

//    for (int y = 0; y < trainDepth.rows; y++) {
//        for (int x = 0; x < trainDepth.cols; x++) {
//            if (trainDepth.at<float>(y, x) == 0) {
//                trainDepth.at<float>(y, x) = 1000000;
//            }
//        }
//    }


    for (int y = 1; y < trainDepth.rows - 1; y++) {
        for (int x = 1; x < trainDepth.cols - 1; x++) {
//            if (trainDepth.at<float>(y, x) <= 7500) continue;

            float dzdx = (trainDepth.at<float>(y, x + 1) - trainDepth.at<float>(y, x - 1)) / 2.0f;
            float dzdy = (trainDepth.at<float>(y + 1, x) - trainDepth.at<float>(y - 1, x)) / 2.0f;

            cv::Vec3f d(-dzdy, -dzdx, 1.0f);
            cv::Vec3f n = cv::normalize(d);
            normals.at<cv::Vec3f>(y, x) = n;
        }
    }

//    for (int y = 1; y < normals.rows - 1; y++) {
//        for (int x = 1; x < normals.cols - 1; x++) {
//            std::cout << "[" << x << ", " << y << "] ("
//                      <<  trainDepth.at<float>(y, x - 1) << ", "
//                     <<  trainDepth.at<float>(y, x) << ", "
//                        <<  trainDepth.at<float>(y, x + 1)
//                      << ") "
//                      << normals.at<cv::Vec3f>(y, x) << std::endl;
//        }
//    }

    cv::Mat norm_8uc3 = cv::Mat(normals.size(), CV_8UC3);
    for (int y = 1; y < normals.rows - 1; y++) {
        for (int x = 1; x < normals.cols - 1; x++) {
            uchar b, g, r;
            cv::Vec3f px = normals.at<cv::Vec3f>(y, x);
            b = static_cast<uchar>(px[0] * 255.0f);
            g = static_cast<uchar>(px[1] * 255.0f);
            r = static_cast<uchar>(px[2] * 255.0f);
            norm_8uc3.at<cv::Vec3b>(y, x) = cv::Vec3b(b, g, r);
        }
    }

    cv::normalize(normals, normals, 0.0f, 1.0f, CV_MINMAX, CV_32FC3);

    // Show results
    cv::imshow("test train - norm_8uc3", norm_8uc3);
    cv::imshow("test train depth", trainDepth);
    cv::imshow("test train depth - normals", normals);
    cv::waitKey(0);
}
