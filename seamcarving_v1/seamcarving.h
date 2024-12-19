#ifndef SEAMCARVING_H
#define SEAMCARVING_H

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#define SEAM_ROWS 0
#define SEAM_COLS 1

Mat gaussien(Mat image, string nomImage, string repertoire);
Mat filtreGradient(Mat image, string nomImage, string repertoire);
int** matrice_cumulative(Mat image);
int* find_way_cols(Mat image, int** m_cumul);
void removePixelAndShiftGray(cv::Mat& image, int row, int col) ;
Mat suppression_seam(Mat image, int* way, string nomImage, string repertoire, int seam_type);
Mat image_seamed(Mat image, int* way, string nomImage, string repertoire, int seam_type);

Mat seamcarving_cols(Mat image, int NB_TOUR, string nomImage, string repertoire);

#endif