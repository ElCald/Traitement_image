#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <filesystem>

#include "traitement.h"

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




    // Lecture de l'image avec le paramètre "IMREAD_GRAYSCALE" pour uniquement l'avoir en degrés de gris
    Mat image = imread(argv[1], IMREAD_GRAYSCALE);

    if (image.empty()) { // Vérification si l'image existe
        cerr << "Erreur de lecture de l'image (main)!" << endl;
        exit(EXIT_FAILURE);
    }


    // ----------------------- TP01 -----------------------
    binarisation(image.clone(), nom_image, dir_path);
    negatif(image.clone(), nom_image, dir_path);
    quantification(image.clone(), nom_image, dir_path);
    rehaussement(image.clone(), nom_image, dir_path);
    egalisation(image.clone(), nom_image, dir_path);
    ouverture(binarisation(image.clone(), nom_image, dir_path), nom_image, dir_path);
    fermeture(binarisation(image.clone(), nom_image, dir_path), nom_image, dir_path);


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


    return EXIT_SUCCESS;
} //fin main
