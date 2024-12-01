#ifndef TRAITEMENT_H
#define TRAITEMENT_H

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


Mat binarisation(Mat image, string nomImage, string repertoire);
Mat negatif(Mat image, string nomImage, string repertoire);
Mat quantification(Mat image, string nomImage, string repertoire);
Mat rehaussement(Mat image, string nomImage, string repertoire);
Mat egalisation(Mat image, string nomImage, string repertoire);
Mat erosion(Mat image, string nomImage, string repertoire);
Mat dilatation(Mat image, string nomImage, string repertoire);
Mat ouverture(Mat image, string nomImage, string repertoire);
Mat fermeture(Mat image, string nomImage, string repertoire);
Mat moyenne(Mat image, string nomImage, string repertoire);
Mat gaussien(Mat image, string nomImage, string repertoire);
Mat minimum(Mat image, string nomImage, string repertoire);
Mat maximum(Mat image, string nomImage, string repertoire);
Mat mediane(Mat image, string nomImage, string repertoire);
Mat sobel(Mat image, string nomImage, string repertoire);
Mat sharr(Mat image, string nomImage, string repertoire);
Mat laplacien_1(Mat image, string nomImage, string repertoire);
Mat laplacien_2(Mat image, string nomImage, string repertoire);
Mat bilateral(Mat image, string nomImage, string repertoire);
Mat watershed(Mat image, string nomImage, string repertoire);



#endif