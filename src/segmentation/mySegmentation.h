#pragma once

#include <QtGlobal>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

template<typename IntType>
cv::Mat createGrayScaleMat(
		const quint32 width,
		const quint32 height,
		const IntType *buffer
) {

	cv::Mat matrix(cv::Size(width, height), CV_64FC1);

	for (quint32 x = 0; x < width; x++) {
		for (quint32 y = 0; y < height; y++) {
			matrix.at<qreal>(x, y) = ((qreal) buffer[x * width + y]) / 4000;
		}
	}

	return matrix;
}

void mySegmentation(cv::Mat &mat) {

	// nie dział !!!
	//	cv::Mat edges(mat.rows, mat.cols, CV_64FC1);
	//	cv::Canny(mat, edges, 100, 200);
	//	cv::imshow("edges", edges * 10);

	cv::Mat part1(mat.rows, mat.cols, CV_64FC1);
	cv::Mat part2_x(mat.rows, mat.cols, CV_64FC1);
	cv::Mat part2_y(mat.rows, mat.cols, CV_64FC1);
	cv::Mat part3(mat.rows, mat.cols, CV_64FC1);

	cv::GaussianBlur(mat, part1, cv::Size(5, 5), 5, 5);

	cv::Sobel(part1, part2_x, -1, 2, 0, 5);
	cv::Sobel(part1, part2_y, -1, 0, 2, 5);

	part3 = part2_x + part2_y;

	cv::imshow("mat", mat * 10);
	cv::imshow("part1", part1 * 10);
	cv::imshow("part2", part2_x * 10);
	cv::imshow("part2_y", part2_y * 10);
	cv::imshow("part3", part3 * 10);

}