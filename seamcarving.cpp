#include <algorithm>
#include <cmath>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

#include "seamcarving.h"

using namespace cv;
using namespace std;


/**
 * Algorithme négatif
 * 
 * On soustrait à 255 la valeur du pixel.
 * 
 * @param image Image chargé dans le main
 * @param nomImage Nom de l'image avec l'extension
 * @param repertoire Répertoire d'export de l'image générée
 * @return Clone de la nouvelle image générée
 */
Mat negatif(Mat image, string nomImage, string repertoire){

    // Parcours de l'image
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {

            // Récupération du pixel de l'image pour le traiter
            uchar& pixel = image.at<uchar>(y, x);

            // Sauvegarde des valeurs des pixels pour inverser
            uchar pixel_temp = pixel;

            // Inversion des couleurs
            pixel = 255 - pixel_temp;   
  

        }
    }


    // Sauvegarde de l'image
    string fichier_modifie = repertoire+"/negatif-" + string(nomImage);

    imwrite(fichier_modifie.c_str(), image); // Sauvearde en fichier

    cout << "Image negatif et enregistrée!" << endl;

    return image.clone();
}// fin negatif




/** 
 * Algorithme gaussien
 * 
 * Lissage modéré dépendament de la fenêtre utilisé
 * On perd moins de détaille que la moyenne
 * 
 * On applique une fenêtre (kernel) sur chaque pixel de l'image.
 * Dans le gaussien on applique sur notre pixel, une fenêtre avec des valeurs qui peuvent être différentes.
 * Ici on fait une moyenne en divisant pas la somme des valeurs de la fenêtre.
 * 
 * @param image Image chargé dans le main
 * @param nomImage Nom de l'image avec l'extension
 * @param repertoire Répertoire d'export de l'image générée
 * @return Clone de la nouvelle image générée
 */
Mat gaussien(Mat image, string nomImage, string repertoire){

    int kernelSize = 3;
    int halfSize = kernelSize / 2;

    int k=0, l=0;

    int fenetre[3][3] = {
        {1,2,1},
        {2,4,2},
        {1,2,1}
    };

    Mat gaussien_img = image.clone();

    // Parcourir chaque pixel de l'image
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
    
            uchar& pixel = gaussien_img.at<uchar>(i, j);

            int som = 1;
            uchar mean_value = 0;

            // Appliquer le kernel
            k=0;
            for (int m = -halfSize; m <= halfSize; m++) {
                l=0;
                for (int n = -halfSize; n <= halfSize; n++) {
                    // Coordonnées pour l'image, adaptées celon la position de la fenêtre dans l'image
                    int x = j + n;
                    int y = i + m;

                    // Vérifier les limites de l'image
                    if (x >= 0 && x < image.cols && y >= 0 && y < image.rows) {
                        uchar pixel_value = image.at<uchar>(y, x);

                        som += fenetre[k][l] * pixel_value;
                    }
                    l++;
                }
                k++;
            }

            mean_value = som/16;

            // Stocker la valeur
            pixel = mean_value;
        }
    }



    string fichier_modifie = repertoire+"gaussien-" + string(nomImage);
    imwrite(fichier_modifie.c_str(), gaussien_img); 
    cout << "Image gaussianisée et enregistrée!" << endl;

    return gaussien_img;
}// fin gaussien


/**
    Fonction permettant de faire le gradient d'une image
    @param image L'image en question
    @param nomImage Nom de l'image
    @return Un clone de l'image ayant subie un un gradient 
*/ 
Mat filtreGradient(Mat image, string nomImage, string repertoire) { // Vérifier si l'image est en niveaux de gris 
    if (image.channels() != 1) { 
        cvtColor(image, image, COLOR_BGR2GRAY); 
    }

    // Matrices pour les gradients
    Mat grad_x = Mat::zeros(image.size(), CV_32F);
    Mat grad_y = Mat::zeros(image.size(), CV_32F);

    // Masques de dérivation
    int dx_mask[3] = {-1, 0, 1};
    int dy_mask[3] = {-1, 0, 1};

    // Calculer les gradients horizontaux et verticaux
    for (int y = 1; y < image.rows - 1; y++) {
    for (int x = 1; x < image.cols - 1; x++) {
    // Calculer le gradient horizontal
    float sum_x = 0;
    for (int j = -1; j <= 1; j++) {
        sum_x += dx_mask[j + 1] * (float)image.at<uchar>(y, x + j);
    }

    grad_x.at<float>(y, x) = sum_x;


    // Calculer le gradient vertical
    float sum_y = 0;
    for (int i = -1; i <= 1; i++) {
        sum_y += dy_mask[i + 1] * (float)image.at<uchar>(y + i, x);
    }
    grad_y.at<float>(y, x) = sum_y;
    }
    }

    // Combiner les gradients pour obtenir l'intensité des bords
    Mat edges = Mat::zeros(image.size(), CV_32F);
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            edges.at<float>(y, x) =
            sqrt(grad_x.at<float>(y, x) * grad_x.at<float>(y, x) +
            grad_y.at<float>(y, x) * grad_y.at<float>(y, x));
        }
    }

    // Normaliser les bords
    normalize(edges, edges, 0, 255, NORM_MINMAX);
    edges.convertTo(edges, CV_8U);


    // string fichier_modifie = repertoire+"gradient-" + string(nomImage);
    // imwrite(fichier_modifie.c_str(), edges); 
    // cout << "Image gradient et enregistrée!" << endl;

    return edges.clone();
} // fin filtreGradient


/**
 * Crée la matrice cumulative
 */
int** matrice_cumulative(Mat image){

    int** m_cumul = (int**)malloc(sizeof(int*) * image.rows);

    for(int i=0; i<image.rows; i++){
        m_cumul[i] = (int*)malloc(sizeof(int) * image.cols);
    }

    // initialise la 1ere ligne avec les valeurs de la 1ere ligne de l'image
    for(int i=0; i<image.cols; i++){
        uchar& pixel = image.at<uchar>(0, i);
        m_cumul[0][i] = pixel;
    }


    // Parcourir chaque pixel de l'image pour générer la matrice cumulative 
    for (int i = 1; i < image.rows; i++) { // on commence à la ligne 1
        for (int j = 0; j < image.cols; j++) {

            uchar& pixel_cur = image.at<uchar>(i, j); // Pixel courant

            uchar& pixel_m = image.at<uchar>(i-1, j);


            if(j == 0){ // Cas où on est sur le bord gauche
                uchar& pixel_d = image.at<uchar>(i-1, j+1);

                m_cumul[i][j] = pixel_cur + min(pixel_m, pixel_d);
            }
            else if(j == image.cols-1){ // Cas où on est sur le bord droit
                uchar& pixel_g = image.at<uchar>(i-1, j-1);

                m_cumul[i][j] = pixel_cur + min(pixel_m, pixel_g);
            }
            else{ // Cas générale
                uchar& pixel_d = image.at<uchar>(i-1, j+1);
                uchar& pixel_g = image.at<uchar>(i-1, j-1);

                m_cumul[i][j] = pixel_cur + min(pixel_m, min(pixel_d, pixel_g));
            }
        }
    }


    return m_cumul;
} // fin matrice_cumulative


/**
 * Recherche du chemin minium du bas vers le haut
 * @return true si on a un chemin unique
 */
bool find_way_cols(const Mat image, int** m_cumul, int**& m_way, const int i_way, bool**& m_bool){

    int minimum = INT_MAX;
    int i_minimum = 0;

    int* way = (int*)malloc(sizeof(int) * image.rows); // tableau temporaire du chemin qui sera sauvegardé dans la matrice si c'est un chemin unique


    // Etape 1 : trouver le minimum dans la dernière ligne
    for(int j=0; j<image.cols; j++){
        if(m_bool[image.rows-1][j] == false && m_cumul[image.rows-1][j] < minimum){
            i_minimum = j;
            minimum = m_cumul[image.rows-1][j];
        }
    }

    way[0] = i_minimum;


    int k = 1;
    // Etape 2 : trouver le minimum des 3 cases au dessus de l'indice précédent et vérifier qu'on ne passe pas par une case déjà occupée
    for(int i=image.rows-2; i>=0; i--){
        minimum = INT_MAX;
        i_minimum = -1;

        // Au milieu
        if(m_bool[i][way[k-1]] == false && m_cumul[i][way[k-1]] < minimum){
            minimum = m_cumul[i][way[k-1]];
            i_minimum = way[k-1];
        }

        if(way[k-1] > 0){ // Si on est pas sur le bord droit
            if(m_bool[i][way[k-1]-1] == false && m_cumul[i][way[k-1]-1] < minimum){
                minimum = m_cumul[i][way[k-1]-1];
                i_minimum = way[k-1]-1;
            }
        }

        if(way[k-1] < image.cols-1){ // Si on est pas sur le bord gauche
            if(m_bool[i][way[k-1]+1] == false && m_cumul[i][way[k-1]+1] < minimum){
                minimum = m_cumul[i][way[k-1]+1];
                i_minimum = way[k-1]+1;
            }
        }

        if(i_minimum != -1){
            m_bool[image.rows-1][way[0]] = true; // On bloque le chemin
            // free(way);
            way[k] = i_minimum;
            // return false;
        }
        else{
            way[k] = 0;
        }

        
        k++;
    }

    // Etape 3 : sauvegarder dans la matrice m_way le nouveau chemin et tracer le chemin occupé dans m_bool
    k=0;
    for(int i=image.rows-1; i>=0; i--){
        m_way[i_way][k] = way[k];
        m_bool[i][way[k]] = true;
        k++;
    }


    free(way);

    return true;

} // fin find_way


/**
 * Supprime un pixel d'une image et remplace à la fin de la ligne par un pixel blanc (255)
 */
void removePixelAndShiftGray(cv::Mat& image, int row, int col) {
    // Vérification des dimensions
    if (row < 0 || row >= image.rows || col < 0 || col >= image.cols) {
        std::cerr << col << " :Position hors limites!" << std::endl;
        return;
    }

    // Décalage des pixels vers la gauche pour la ligne donnée
    for (int c = col; c < image.cols - 1; ++c) {
        image.at<uchar>(row, c) = image.at<uchar>(row, c + 1);
    }

    // Optionnel : remplir le dernier pixel avec une valeur spécifique (par exemple, noir = 0)
    image.at<uchar>(row, image.cols - 1) = 255;
}


/**
 * Suppression du chemin sur l'image
 */
Mat suppression_seam(Mat image, bool** m_bool, int nb_tour, string nomImage, string repertoire, int seam_type){
    Mat reduce_img = image.clone();

    // int k=0;
    // for(int j=0; j<nb_tour; j++){
    //     if(seam_type == SEAM_COLS){
    //         for(int i=reduce_img.rows-1; i>=0; i--){
    //             removePixelAndShiftGray(reduce_img, i, m_way[j][k]);
    //             k++;
    //         }
    //     }
    // }

    for (int i = 0; i < image.rows; i++) { // on commence à la ligne 1
        for (int j = 0; j < image.cols; j++) {
            if(m_bool[i][j]){
                removePixelAndShiftGray(reduce_img, i, j);
            }
        }
    }
    

    return reduce_img;
} // fin suppression_colonne


/**
 * Traçage du chemin sur l'images
 */
Mat image_seamed(Mat image, bool** m_bool, string nomImage, string repertoire, int seam_type){
    Mat img_seamed = image.clone();


    if(seam_type == SEAM_COLS){
        for (int i = 0; i < img_seamed.rows; i++) { // on commence à la ligne 1
            for (int j = 0; j < img_seamed.cols; j++) {
                uchar& pixel_cur = img_seamed.at<uchar>(i, j); // Pixel courant

                if(m_bool[i][j]){
                    pixel_cur = 255;
                }
            }
        }
    }
    else if(seam_type == SEAM_ROWS){

    }


    // string fichier_modifie = repertoire+"seamed_img-" + string(nomImage);
    // imwrite(fichier_modifie.c_str(), seamed_img); 

    return img_seamed;
} // fin image_seamed


/**
 * Fonction principale qui contient tout l'algorithme du seam carving
 * @return Image redimensionnée
 */
Mat seamcarving_cols(Mat image, int NB_TOUR, string nomImage, string repertoire){

    int** m = nullptr; // matrice cumulative
    int** m_way = nullptr; // matrice de tableau qui contient le chemin du bas vers le haut de l'image
    bool** m_bool = nullptr; // matrice de boolean qui contient les pixels déjà occupés



    m_way = (int**)malloc(sizeof(int*) * NB_TOUR); 
    m_bool = (bool**)malloc(sizeof(bool*) * image.rows);
    for(int i=0; i<image.rows; i++){
        m_way[i] = (int*)malloc(sizeof(int) * image.rows);
        m_bool[i] = (bool*)malloc(sizeof(bool) * image.cols);
    }

    for(int i=0; i<image.rows; i++){
        for(int j=0; j<image.cols; j++){
            m_bool[i][j] = false;
        }
    }

    Mat image_reduce = image.clone();
    Mat img_seamed = image.clone();
    
    Mat image_gradient = filtreGradient(image_reduce.clone(), nomImage, repertoire);

    m = matrice_cumulative(image_gradient);

    /**
     * On crée une matrice cumulative à partir de l'image ayant subit un gradient.
     * Avec cette matrice on crée un tableau qui contient le chemin le plus "minimum" de bas en haut
     * On supprime ce chemin dans l'image gradient
     * On supprime ce chemin dans l'image originale
     * On trace sur l'image originale le chemin supprimé (optionel)
     */
    // for(int i=0; i<NB_TOUR; i++){
    //     find_way_cols(image_gradient, m, m_way, i, m_bool);
    //     // image_gradient = suppression_seam(image_gradient.clone(), way, nomImage, repertoire, SEAM_COLS);
    //     // image_reduce = suppression_seam(image_reduce.clone(), way, nomImage, repertoire, SEAM_COLS);
    //     // img_seamed = image_seamed(img_seamed.clone(), way, nomImage, repertoire, SEAM_COLS);
    // }
    int t=0;
    int t_stop=0;
    do{
        if(find_way_cols(image_gradient, m, m_way, t, m_bool) == true){
            t++;
        }
        
        t_stop++;
    }while(t_stop<100000 && t<NB_TOUR);

    cout << t << endl;
    cout << t_stop << endl;

    image_reduce = suppression_seam(image_reduce.clone(), m_bool, t, nomImage, repertoire, SEAM_COLS);

    img_seamed = image_seamed(img_seamed.clone(), m_bool, nomImage, repertoire, SEAM_COLS);



    for(int i=0; i<image.rows; i++){
        free(m[i]);
    }
    free(m);


    // Crop de l'image
    Mat resized_image;
    int new_height = image_reduce.rows;
    int new_width = image_reduce.cols - NB_TOUR;

    resized_image = image_reduce(cv::Rect(0, 0, new_width, new_height));

    // Enregistrement de l'image crop
    string fichier_modifie = repertoire+"resized_cols-" + string(nomImage);
    imwrite(fichier_modifie.c_str(), image_reduce); 
    cout << "Image resized et enregistrée!" << endl;

    // Enregistrement de l'image contenant les seams
    fichier_modifie = repertoire+"seamed_cols-" + string(nomImage);
    imwrite(fichier_modifie.c_str(), img_seamed); 
    cout << "Image seamed et enregistrée!" << endl;

    return image_gradient;

} // fin seamcarving_cols

