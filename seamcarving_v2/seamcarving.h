#ifndef SEAMCARVING_H
#define SEAMCARVING_H

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#define SEAM_ROWS 0
#define SEAM_COLS 1

Mat negatif(Mat image, string nomImage, string repertoire);
Mat gaussien(Mat image, string nomImage, string repertoire);
Mat filtreGradient(Mat image, string nomImage, string repertoire);
int** matrice_cumulative(Mat image);
bool find_way_cols(const Mat image, int** m_cumul, int**& m_way, const int i_way, bool**& m_bool);
void removePixelAndShiftGray(cv::Mat& image, int row, int col);
Mat suppression_seam(Mat image, bool** m_bool, int nb_tour, string nomImage, string repertoire, int seam_type);
Mat image_seamed(Mat image, bool** m_bool, string nomImage, string repertoire, int seam_type);

Mat seamcarving_cols(Mat image, int NB_TOUR, string nomImage, string repertoire);

#endif