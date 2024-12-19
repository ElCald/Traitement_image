#ifndef SEAMCARVING_H
#define SEAMCARVING_H

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#define SEAM_ROWS 0
#define SEAM_COLS 1

Mat gaussien(Mat image, string nomImage, string repertoire);
Mat filtreGradient(Mat image, string nomImage, string repertoire);
inline int** matrice_cumulative_cols(Mat image);
inline int** matrice_cumulative_rows(Mat image);
inline int* find_way_cols(Mat image, int** m_cumul);
inline int* find_way_rows(Mat image, int** m_cumul);
inline void removePixelAndShiftLeftGray(cv::Mat& image, int row, int col);
inline void removePixelAndShiftUpGray(cv::Mat& image, int row, int col);
inline Mat suppression_seam(Mat image, int* way, string nomImage, string repertoire, int seam_type);
inline Mat image_seamed(Mat image, int* way, string nomImage, string repertoire, int seam_type);

Mat seamcarving(Mat image, int NB_TOUR, string nomImage, string repertoire, int seam_type);

#endif