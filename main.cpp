#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <filesystem>

#include "seamcarving.h"

#define NB_PARAM 1

using namespace std;
using namespace cv;


int main(int argc, char* argv[]) {

    if(argc != NB_PARAM +1){
        cerr << "Paramètre : chemin vers l'image" << endl;
        exit(EXIT_FAILURE);
    }


    string nom_image; // Nom de l'image original avec son extension (ex : nom_image.png)
    string nom_image_noExt; // Nom de l'image sans son extension (ex : nom_image)
    string dir_path; // Chemin d'accès vers l'enregistrement des images générées (ex : images/nom_image/)



    // On récupère un chemin d'accès vers l'image et on ne garde que le nom de l'image avec son extension
    filesystem::path chemin_abs_img(argv[1]);
    nom_image = chemin_abs_img.filename().string();

    // On ne garde que le nom de l'image sans l'extension afin de pouvoir créer un répertoire avec son nom
    filesystem::path chemin_img_ext(nom_image);
    nom_image_noExt = chemin_img_ext.stem().string();

    dir_path = string("images/")+nom_image_noExt+string("/");



    // Création du répertoire "images"
    if (!filesystem::exists("images")) {
        if (!filesystem::create_directory("images")) {
            cerr << "Échec de la création du répertoire 'images'." << endl;
            exit(EXIT_FAILURE);
        } 
    } 


    // Création du répertoire pourtant le nom de l'image traité
    if (!filesystem::exists(dir_path)) {
        if (!filesystem::create_directory(dir_path)) {
            cerr << "Échec de la création du répertoire d'accueil." << endl;
            exit(EXIT_FAILURE);
        } 
    }


<<<<<<< HEAD
=======


>>>>>>> 86af1b99e936edb8df232f7063d1f343402af134
    // Lecture de l'image avec le paramètre "IMREAD_GRAYSCALE" pour uniquement l'avoir en degrés de gris
    Mat image = imread(argv[1], IMREAD_GRAYSCALE);

    if (image.empty()) { // Vérification si l'image existe
        cerr << "Erreur de lecture de l'image (main)!" << endl;
        exit(EXIT_FAILURE);
    }

    // ----------------------- Projet -----------------------

    cout << "Rows: " << image.rows << endl;
    cout << "Cols: " << image.cols << endl;

    const int NB_TOUR = 300; // nombre de seams


<<<<<<< HEAD
    Mat img_gausse = gaussien(image.clone(), nom_image, dir_path);
    seamcarving_cols(img_gausse.clone(), NB_TOUR, nom_image, dir_path);
=======
    // ----------------------- TP02 -----------------------
    moyenne(image.clone(), nom_image, dir_path);
    gaussien(image.clone(), nom_image, dir_path);
    minimum(image.clone(), nom_image, dir_path);
    maximum(image.clone(), nom_image, dir_path);
    mediane(image.clone(), nom_image, dir_path);
    sobel(image.clone(), nom_image, dir_path);
    sharr(image.clone(), nom_image, dir_path);
    laplacien_1(image.clone(), nom_image, dir_path);
    laplacien_2(image.clone(), nom_image, dir_path);
    bilateral(image.clone(), nom_image, dir_path);


    // ----------------------- TP03 -----------------------
    // watershed(image.clone(), nom_image, dir_path);
>>>>>>> 86af1b99e936edb8df232f7063d1f343402af134


    return EXIT_SUCCESS;
} //fin main
