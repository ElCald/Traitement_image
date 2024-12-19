#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <filesystem>
#include <cmath>

#include "traitement.h"

using namespace std;
using namespace cv;



/**
 * Algorithme de binarisation
 * 
 * L'image n'est plus qu'en noir (0) et blanc (255).
 * 
 * @param image Image chargé dans le main
 * @param nomImage Nom de l'image avec l'extension
 * @param repertoire Répertoire d'export de l'image générée
 * @return Clone de la nouvelle image générée
 */
Mat binarisation(Mat image, string nomImage, string repertoire){
    
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            
            uchar& pixel = image.at<uchar>(y, x);

            // Si la valeur est supérieur à 128, elle passe à 255 sinon 0
            pixel = pixel > 128?255:0;

        }
    }

    string fichier_modifie = repertoire+"binarise-" + string(nomImage);
    imwrite(fichier_modifie.c_str(), image); 
    cout << "Image binarisée et enregistrée!" << endl;

    return image.clone();
}// fin binarisation



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
 * Algorithme de quantification
 * 
 * On réduit le nombre de teinte utilisée sur l'image.
 * 
 * @param image Image chargé dans le main
 * @param nomImage Nom de l'image avec l'extension
 * @param repertoire Répertoire d'export de l'image générée
 * @return Clone de la nouvelle image générée
 */
Mat quantification(Mat image, string nomImage, string repertoire){


    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            
            uchar& pixel = image.at<uchar>(y, x);

            // Modifier les valeurs RGB 
            pixel = (pixel/64) * 64;   

        }
    }


    string fichier_modifie = repertoire+"quantification-" + string(nomImage);
    imwrite(fichier_modifie.c_str(), image); 
    cout << "Image quantifiée et enregistrée!" << endl;

    return image.clone();
}// fin quantification


/** 
 * Algorithme de réhaussement
 * 
 * Augmentation du contraste
 * 
 * On prend des valeur v1 et v2 dans l'histogramme. 
 * Pour v1 on prend la 1ère valeur après les zéro à partir de 0.
 * Pour v2 on prend la dernière valeur avant les zéros entre v2 et 255.
 * On génère un histograme dans lequel, les valeurs avant v1 sont 0 et après v2 sont 255.
 * Entre v1 et v2 on crée une courbe linéaire entre 0 et 255.
 * Ainsi pour chaque valeur entre 0 et 255 est associé à une valeur de l'histogramme.
 * 
 * @param image Image chargé dans le main
 * @param nomImage Nom de l'image avec l'extension
 * @param repertoire Répertoire d'export de l'image générée
 * @return Clone de la nouvelle image générée
 */
Mat rehaussement(Mat image, string nomImage, string repertoire){


    int histo[256] = {0};

    int k=0, i, a, b, v1, v2;

    // Création de l'histogramme
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            
            uchar& pixel = image.at<uchar>(y, x);
            histo[pixel] ++;

        }
    }

    // Récup des valeurs de v1 et v2
    while(histo[k] == 0){
        k++;
    }

    v1 = k-1;

    while(histo[k] != 0){
        k++;
    }

    v2 = k;


    // Mise à 0 des valeurs avant v1
    for(i=0; i<v1; i++){
        histo[i] = 0;
    }

    // Mise à 255 des valeurs après v2
    for(i=v2; i<256; i++){
        histo[i] = 255;
    }


    // Génération de la courbe entre v1 et v2
    a = 255/(v2-v1);
    b = -((255*v1)/(v2-v1));

    for(i=v1; i<v2; i++){
        histo[i] = a*i+b;
    }


    // Application du filtre à l'image
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            
            uchar& pixel = image.at<uchar>(y, x);

            pixel = histo[pixel];
        }
    }


    string fichier_modifie = repertoire+"rehausse-" + string(nomImage);
    imwrite(fichier_modifie.c_str(), image);
    cout << "Image rehaussée et enregistrée!" << endl;

    return image.clone();
}// fin quantification



/** 
 * Algorithme d'égalisation
 * 
 * Augmentation de la plage d'intensité de couleur, on passe d'une plage réduite à l'utilisation complète allant de 0 à 255
 * Ainsi les valeurs sombres deviendront plus sombre et vont tendres vers 0 et les valeurs claires vont être plus claire et tendres vers 255
 * On applatit donc la courbe afin d'améliorer le contraste
 * 
 * On crée un histogramme de l'image, qu'on va pondérer.
 * 
 * @param image Image chargé dans le main
 * @param nomImage Nom de l'image avec l'extension
 * @param repertoire Répertoire d'export de l'image générée
 * @return Clone de la nouvelle image générée
 */
Mat egalisation(Mat image, string nomImage, string repertoire){

    int k=0;
    int histo[256] = {0};


    // Création de l'histogramme de chaque couleur
    for(int i=0; i<256; i++){
        for (int y = 0; y < image.rows; y++) {
            for (int x = 0; x < image.cols; x++) {

                uchar& pixel = image.at<uchar>(y, x);
                
                if(pixel == i){
                    k++;
                }
            
            }
        }
        histo[i] = k;
    }


    // Pondération de l'histogramme
    for(int i=0; i<256; i++){
        histo[i] = (255.0/(image.rows * image.cols)) * (double)histo[i];
    }


    // Application du filtre à l'image
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            
            uchar& pixel = image.at<uchar>(y, x);

            // Modifier la valeur
            pixel = histo[pixel];
        }
    }


    string fichier_modifie = repertoire+"egalisation-" + string(nomImage);
    imwrite(fichier_modifie.c_str(), image); 
    cout << "Image egalisée et enregistrée!" << endl;

    return image.clone();
}// fin egalisation 



/** 
 * Algorithme d'érosion
 * 
 * Retire de l'informations sur le bord des formes en grattant des pixels
 * 
 * On applique une fenêtre (kernel) sur chaque pixel de l'image.
 * Dans l'érosion on applique sur notre pixel, la valeur minimum qui se trouve dans la fenêtre, sachant que l'image doit être binarisée
 * 
 * @param image Image binarisée chargé dans le main
 * @param nomImage Nom de l'image avec l'extension
 * @param repertoire Répertoire d'export de l'image générée
 * @return Clone de la nouvelle image générée
 */
Mat erosion(Mat image, string nomImage, string repertoire){

    // Définir la taille du kernel
    int kernelSize = 3;
    int halfSize = kernelSize / 2;


    Mat eroded_img = image.clone();

    // Parcourir chaque pixel de l'image
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
    
            uchar& pixel = eroded_img.at<uchar>(i, j);
           
            uchar min_value = 255;

            // Faire parcourir le kernel sur l'image
            for (int m = -halfSize; m <= halfSize; m++) {
                for (int n = -halfSize; n <= halfSize; n++) {
                    int x = j + n;
                    int y = i + m;

                    // Vérification pour ne pas faire sortir la fenêtre de l'image, si c'est le cas on applique pas le filtre
                    if (x >= 0 && x < image.cols && y >= 0 && y < image.rows) {
                        uchar pixel_value = image.at<uchar>(y, x);
                        min_value = min(min_value, pixel_value);
                    }
                }
            }

            pixel = min_value;
        }
    }


    string fichier_modifie = repertoire+"erosion-" + string(nomImage);
    imwrite(fichier_modifie.c_str(), eroded_img); 
    cout << "Image érodée et enregistrée!" << endl;

    return eroded_img;
}// fin erosion


/** 
 * Algorithme de dilatation
 * 
 * Ajoute de l'information sur le bord des formes
 * 
 * On applique une fenêtre (kernel) sur chaque pixel de l'image.
 * Dans la dilatation on applique sur notre pixel, la valeur maximum qui se trouve dans la fenêtre, sachant que l'image doit être binarisée
 * 
 * @param image Image binarisée chargé dans le main
 * @param nomImage Nom de l'image avec l'extension
 * @param repertoire Répertoire d'export de l'image générée
 * @return Clone de la nouvelle image générée
 */
Mat dilatation(Mat image, string nomImage, string repertoire){

    int kernelSize = 3;
    int halfSize = kernelSize / 2;

    Mat dilated_img = image.clone();
    

    // Parcourir chaque pixel de l'image
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
    
            uchar& pixel = dilated_img.at<uchar>(i, j);

            // Initialiser la valeur min
            uchar max_value = 0; // Valeur maximale possible pour un pixel en niveaux de gris

            // Appliquer le kernel
            for (int m = -halfSize; m <= halfSize; m++) {
                for (int n = -halfSize; n <= halfSize; n++) {
                    int x = j + n;
                    int y = i + m;

                    // Vérifier les limites de l'image
                    if (x >= 0 && x < image.cols && y >= 0 && y < image.rows) {
                        uchar pixel_value = image.at<uchar>(y, x);
                        max_value = max(max_value, pixel_value);
                    }
                }
            }

            // Stocker la valeur min dans l'image érodée
        }
    }

    string fichier_modifie = repertoire+"dilate-" + string(nomImage);
    imwrite(fichier_modifie.c_str(), dilated_img); 
    cout << "Image dilatée et enregistrée!" << endl;

    return dilated_img;
}// fin dilatation


/** 
 * Algorithme d'ouverture
 * 
 * Elimine les imperfections
 * 
 * Application de l'algorithme d'érosion puis de dilatation à notre image.
 * 
 * @param image Image chargé dans le main
 * @param nomImage Nom de l'image avec l'extension
 * @param repertoire Répertoire d'export de l'image générée
 * @return Clone de la nouvelle image générée
 */
Mat ouverture(Mat image, string nomImage, string repertoire){

    Mat ouvert_img = image.clone();

    ouvert_img = erosion(ouvert_img, nomImage, repertoire).clone();
    ouvert_img = dilatation(ouvert_img, nomImage, repertoire).clone();


    string fichier_modifie = repertoire+"ouverture-" + string(nomImage);
    imwrite(fichier_modifie.c_str(), ouvert_img); 
    cout << "Image ouvert et enregistrée!" << endl;

    return ouvert_img;

}//fin ouverture


/** 
 * Algorithme de fermeture
 * 
 * Comble les trous et relie les formes qui sont proches
 * 
 * Application de l'algorithme de dilatation puis d'érosion à notre image.
 * 
 * @param image Image chargé dans le main
 * @param nomImage Nom de l'image avec l'extension
 * @param repertoire Répertoire d'export de l'image générée
 * @return Clone de la nouvelle image générée
 */
Mat fermeture(Mat image, string nomImage, string repertoire){

    Mat fermer_img = image.clone();

    fermer_img = dilatation(fermer_img, nomImage, repertoire).clone();
    fermer_img = erosion(fermer_img, nomImage, repertoire).clone();
    

    string fichier_modifie = repertoire+"fermeture-" + string(nomImage);
    imwrite(fichier_modifie.c_str(), fermer_img); 
    cout << "Image fermée et enregistrée!" << endl;

    return fermer_img;

}//fin fermeture





/** 
 * Algorithme de moyenne
 * 
 * Lissage des textures mais rend flou
 * 
 * On applique une fenêtre (kernel) sur chaque pixel de l'image.
 * Dans la moyenne on applique sur notre pixel, la valeur moyenne qui se trouve dans la fenêtre.
 * Donc on fait la somme de tous les pixels qu'on divise par la taille de notre fenêtre.
 * 
 * @param image Image chargé dans le main
 * @param nomImage Nom de l'image avec l'extension
 * @param repertoire Répertoire d'export de l'image générée
 * @return Clone de la nouvelle image générée
 */
Mat moyenne(Mat image, string nomImage, string repertoire){

    int kernelSize = 21;
    int halfSize = kernelSize / 2;

    Mat moyenne_img = image.clone();

    // Parcourir chaque pixel de l'image
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
    
            uchar& pixel = moyenne_img.at<uchar>(i, j);

            int som = 1;
            uchar mean_value = 0;

            // Appliquer le kernel
            for (int m = -halfSize; m <= halfSize; m++) {
                for (int n = -halfSize; n <= halfSize; n++) {
                    int x = j + n;
                    int y = i + m;

                    // Vérifier les limites de l'image
                    if (x >= 0 && x < image.cols && y >= 0 && y < image.rows) {
                        uchar pixel_value = image.at<uchar>(y, x);
                        som += pixel_value;
                    }
                }
            }

            mean_value = som/(kernelSize*kernelSize);

            // Stocker la valeur
            pixel = mean_value;
        }
    }



    string fichier_modifie = repertoire+"moyenne-" + string(nomImage);
    imwrite(fichier_modifie.c_str(), moyenne_img); 
    cout << "Image moyennée et enregistrée!" << endl;

    return moyenne_img;
}// fin moyenne



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
 * Algorithme minimum
 * 
 * Contracter les formes lumineuses, permet la détection des ombres et élimine les bruits brillants
 * 
 * On applique une fenêtre (kernel) sur chaque pixel de l'image.
 * Dans le minimum on applique sur notre pixel, la valeur minimum qui se trouve dans la fenêtre
 * 
 * @param image Image chargé dans le main
 * @param nomImage Nom de l'image avec l'extension
 * @param repertoire Répertoire d'export de l'image générée
 * @return Clone de la nouvelle image générée
 */
Mat minimum(Mat image, string nomImage, string repertoire){

    int kernelSize = 3;
    int halfSize = kernelSize / 2;

    Mat minimum_img = image.clone();

    // Parcourir chaque pixel de l'image
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
    
            uchar& pixel = minimum_img.at<uchar>(i, j);

            int min = 255;
            uchar mean_value = 0;

            // Appliquer le kernel
            for (int m = -halfSize; m <= halfSize; m++) {
                for (int n = -halfSize; n <= halfSize; n++) {
                    int x = j + n;
                    int y = i + m;

                    // Vérifier les limites de l'image
                    if (x >= 0 && x < image.cols && y >= 0 && y < image.rows) {
                        uchar pixel_value = image.at<uchar>(y, x);
                        min = pixel_value<min?pixel_value:min;
                    }
                }
            }

            // Stocker la valeur
            pixel = min;
        }
    }


    string fichier_modifie = repertoire+"minimum-" + string(nomImage);
    imwrite(fichier_modifie.c_str(), minimum_img); 
    cout << "Image minimum et enregistrée!" << endl;

    return minimum_img;
}// fin minimum



/** 
 * Algorithme maximum
 * 
 * Attenue les taches d'ombre et renforce les points lumineux
 * 
 * On applique une fenêtre (kernel) sur chaque pixel de l'image.
 * Dans le maximum on applique sur notre pixel, la valeur maximum qui se trouve dans la fenêtre
 * 
 * @param image Image chargé dans le main
 * @param nomImage Nom de l'image avec l'extension
 * @param repertoire Répertoire d'export de l'image générée
 * @return Clone de la nouvelle image générée
 */
Mat maximum(Mat image, string nomImage, string repertoire){

    int kernelSize = 3;
    int halfSize = kernelSize / 2;

    Mat maximum_img = image.clone();

    // Parcourir chaque pixel de l'image
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
    
            uchar& pixel = maximum_img.at<uchar>(i, j);

            int max = 0;
            uchar mean_value = 0;

            // Appliquer le kernel
            for (int m = -halfSize; m <= halfSize; m++) {
                for (int n = -halfSize; n <= halfSize; n++) {
                    int x = j + n;
                    int y = i + m;

                    // Vérifier les limites de l'image
                    if (x >= 0 && x < image.cols && y >= 0 && y < image.rows) {
                        uchar pixel_value = image.at<uchar>(y, x);
                        max = pixel_value>max?pixel_value:max;
                    }
                }
            }

            // Stocker la valeur
            pixel = max;
        }
    }


    string fichier_modifie = repertoire+"maximum-" + string(nomImage);
    imwrite(fichier_modifie.c_str(), maximum_img); 
    cout << "Image maximum et enregistrée!" << endl;

    return maximum_img;
}// fin maximum



/** 
 * Algorithme de médianne
 * 
 * Elimine le bruit poivre et sel
 * 
 * On applique une fenêtre (kernel) sur chaque pixel de l'image.
 * Dans la medianne on applique sur notre pixel, la valeur medianne qui se trouve dans la fenêtre.
 * Pour ce faire on stock les valeurs de la fenêtre dans un tableau, on tri et on prend au milieu.
 * 
 * @param image Image chargé dans le main
 * @param nomImage Nom de l'image avec l'extension
 * @param repertoire Répertoire d'export de l'image générée
 * @return Clone de la nouvelle image générée
 */
Mat mediane(Mat image, string nomImage, string repertoire){

    int kernelSize = 3;
    int halfSize = kernelSize / 2;

    int k=0;

    int tab_med[9] = {0};


    Mat mediane_img = image.clone();

    // Parcourir chaque pixel de l'image
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
    
            uchar& pixel = mediane_img.at<uchar>(i, j);

            int med = 0;
            uchar mean_value = 0;
            k=0;

            // Appliquer le kernel
            for (int m = -halfSize; m <= halfSize; m++) {
                for (int n = -halfSize; n <= halfSize; n++) {
                    int x = j + n;
                    int y = i + m;

                    // Vérifier les limites de l'image
                    if (x >= 0 && x < image.cols && y >= 0 && y < image.rows) {
                        uchar pixel_value = image.at<uchar>(y, x);
                        tab_med[k] = pixel_value;
                        k++;
                    }
                }
            }

            sort(tab_med, tab_med + 9);

            med = tab_med[5];

            // Stocker la valeur
            pixel = med;
        }
    }


    string fichier_modifie = repertoire+"mediane-" + string(nomImage);
    imwrite(fichier_modifie.c_str(), mediane_img); 
    cout << "Image mediane et enregistrée!" << endl;

    return mediane_img;
}// fin mediane



/** 
 * Algorithme de Sobel
 * 
 * Détection des contours
 * 
 * On applique une fenêtre (kernel) sur chaque pixel de l'image.
 * Dans Sobel on applique sur notre pixel, 2 fenêtres pour traiter les lignes et les colonnes.
 * Ici pas besoin de diviser car la somme de des fenêtres est égale à zéro.
 * 
 * @param image Image chargé dans le main
 * @param nomImage Nom de l'image avec l'extension
 * @param repertoire Répertoire d'export de l'image générée
 * @return Clone de la nouvelle image générée
 */
Mat sobel(Mat image, string nomImage, string repertoire){

    int kernelSize = 3;
    int halfSize = kernelSize / 2;


    int k=0, l=0;

    int fenetre_x[3][3] = {
        {-1,0,1},
        {-2,0,2},
        {-1,0,1}
    };


    int fenetre_y[3][3] = {
        {-1,-2,-1},
        {0,0,0},
        {1,2,1}
    };

    Mat sobel_img = image.clone();

    // Parcourir chaque pixel de l'image
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
    
            uchar& pixel = sobel_img.at<uchar>(i, j);

            int som = 1;
            uchar mean_value = 0;

            // Appliquer le kernel
            k=0;
            for (int m = -halfSize; m <= halfSize; m++) {
                l=0;
                for (int n = -halfSize; n <= halfSize; n++) {
                    int x = j + n;
                    int y = i + m;

                    // Vérifier les limites de l'image
                    if (x >= 0 && x < image.cols && y >= 0 && y < image.rows) {
                        uchar pixel_value = image.at<uchar>(y, x);

                        som += fenetre_x[k][l] * pixel_value;
                    }
                    l++;
                }
                k++;
            }

            
            som = (som < 0) ? 0 : som;
            som = (som > 255) ? 255 : som;
            mean_value = som;

            // Stocker la valeur
            pixel = mean_value;


            pixel = sobel_img.at<uchar>(i, j);

            som = 1;
            mean_value = 0;

            // Appliquer le kernel
            k=0;
            for (int m = -halfSize; m <= halfSize; m++) {
                l=0;
                for (int n = -halfSize; n <= halfSize; n++) {
                    int x = j + n;
                    int y = i + m;

                    // Vérifier les limites de l'image
                    if (x >= 0 && x < image.cols && y >= 0 && y < image.rows) {
                        uchar pixel_value = image.at<uchar>(y, x);

                        som += fenetre_y[k][l] * pixel_value;
                    }
                    l++;
                }
                k++;
            }

            som = (som < 0) ? 0 : som;
            som = (som > 255) ? 255 : som;
            mean_value = som;

            // Stocker la valeur
            pixel = mean_value;
        }
    }



    string fichier_modifie = repertoire+"sobel-" + string(nomImage);
    imwrite(fichier_modifie.c_str(), sobel_img); 
    cout << "Image sobélisée et enregistrée!" << endl;

    return sobel_img;
}// fin sobel



Mat sharr(Mat image, string nomImage, string repertoire){

    int kernelSize = 3;
    int halfSize = kernelSize / 2;


    int k=0, l=0;

    int fenetre_x[3][3] = {
        {-3,0,3},
        {-10,0,10},
        {-3,0,3}
    };


    int fenetre_y[3][3] = {
        {-3,-10,-3},
        {0,0,0},
        {3,10,3}
    };

    Mat sharr_img = image.clone();

    // Parcourir chaque pixel de l'image
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
    
            uchar& pixel = sharr_img.at<uchar>(i, j);

            int som = 1;
            uchar mean_value = 0;

            // Appliquer le kernel
            k=0;
            for (int m = -halfSize; m <= halfSize; m++) {
                l=0;
                for (int n = -halfSize; n <= halfSize; n++) {
                    int x = j + n;
                    int y = i + m;

                    // Vérifier les limites de l'image
                    if (x >= 0 && x < image.cols && y >= 0 && y < image.rows) {
                        uchar pixel_value = image.at<uchar>(y, x);

                        som += fenetre_x[k][l] * pixel_value;
                    }
                    l++;
                }
                k++;
            }

            
            som = (som < 0) ? 0 : som;
            som = (som > 255) ? 255 : som;
            mean_value = som;

            // Stocker la valeur
            pixel = mean_value;


            pixel = sharr_img.at<uchar>(i, j);

            som = 1;
            mean_value = 0;

            // Appliquer le kernel
            k=0;
            for (int m = -halfSize; m <= halfSize; m++) {
                l=0;
                for (int n = -halfSize; n <= halfSize; n++) {
                    int x = j + n;
                    int y = i + m;

                    // Vérifier les limites de l'image
                    if (x >= 0 && x < image.cols && y >= 0 && y < image.rows) {
                        uchar pixel_value = image.at<uchar>(y, x);

                        som += fenetre_y[k][l] * pixel_value;
                    }
                    l++;
                }
                k++;
            }

            som = (som < 0) ? 0 : som;
            som = (som > 255) ? 255 : som;
            mean_value = som;

            // Stocker la valeur
            pixel = mean_value;
        }
    }



    string fichier_modifie = repertoire+"sharr-" + string(nomImage);
    imwrite(fichier_modifie.c_str(), sharr_img); 
    cout << "Image sobélisée et enregistrée!" << endl;

    return sharr_img;
}// fin sharr



/** 
 * Algorithme de Laplacien 1
 * 
 * Détection des contours
 * 
 * On applique une fenêtre (kernel) sur chaque pixel de l'image.
 * Dans Laplacien 1 on applique sur notre pixel, 1 fenêtre de Laplacien.
 * 
 * @param image Image chargé dans le main
 * @param nomImage Nom de l'image avec l'extension
 * @param repertoire Répertoire d'export de l'image générée
 * @return Clone de la nouvelle image générée
 */
Mat laplacien_1(Mat image, string nomImage, string repertoire){

    int kernelSize = 3;
    int halfSize = kernelSize / 2;

    int k=0, l=0;

    int fenetre[3][3] = {
        {0,-1,0},
        {-1,4,-1},
        {0,-1,0}
    };

    Mat laplacien_1_img = image.clone();

    // Parcourir chaque pixel de l'image
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
    
            uchar& pixel = laplacien_1_img.at<uchar>(i, j);

            int som = 1;
            uchar mean_value = 0;

            // Appliquer le kernel
            k=0;
            for (int m = -halfSize; m <= halfSize; m++) {
                l=0;
                for (int n = -halfSize; n <= halfSize; n++) {
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

            som = (som < 0) ? 0 : som;
            som = (som > 255) ? 255 : som;
            mean_value = som;

            // Stocker la valeur
            pixel = mean_value;
        }
    }



    string fichier_modifie = repertoire+"laplacien_1-" + string(nomImage);
    imwrite(fichier_modifie.c_str(), laplacien_1_img); 
    cout << "Image laplacienisée 1 et enregistrée!" << endl;

    return laplacien_1_img;
}// fin laplacien_1



/** 
 * Algorithme de Laplacien 2
 * 
 * Détection des contours
 * 
 * On applique une fenêtre (kernel) sur chaque pixel de l'image.
 * Dans Laplacien 2 on applique sur notre pixel, 1 fenêtres de Laplacien.
 * 
 * @param image Image chargé dans le main
 * @param nomImage Nom de l'image avec l'extension
 * @param repertoire Répertoire d'export de l'image générée
 * @return Clone de la nouvelle image générée
 */
Mat laplacien_2(Mat image, string nomImage, string repertoire){

    int kernelSize = 3;
    int halfSize = kernelSize / 2;

    int k=0, l=0;

    int fenetre[3][3] = {
        {-1,-1,-1},
        {-1,8,-1},
        {-1,-1,-1}
    };

    Mat laplacien_2_img = image.clone();

    // Parcourir chaque pixel de l'image
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
    
            uchar& pixel = laplacien_2_img.at<uchar>(i, j);

            int som = 1;
            uchar mean_value = 0;

            // Appliquer le kernel
            k=0;
            for (int m = -halfSize; m <= halfSize; m++) {
                l=0;
                for (int n = -halfSize; n <= halfSize; n++) {
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

            som = (som < 0) ? 0 : som;
            som = (som > 255) ? 255 : som;
            mean_value = som;

            // Stocker la valeur
            pixel = mean_value;
        }
    }



    string fichier_modifie = repertoire+"laplacien_2-" + string(nomImage);
    imwrite(fichier_modifie.c_str(), laplacien_2_img); 
    cout << "Image laplacienisée 2 et enregistrée!" << endl;

    return laplacien_2_img;
}// fin laplacien_2



/** 
 * Algorithme bilatéral
 * 
 * Réduction de bruit
 * 
 * @param image Image chargé dans le main
 * @param nomImage Nom de l'image avec l'extension
 * @param repertoire Répertoire d'export de l'image générée
 * @return Clone de la nouvelle image générée
 */
Mat bilateral(Mat image, string nomImage, string repertoire){


    /*
    Des artéfactes se trouvent sur le haut de l'image 
    */

    int kernelSize = 3;
    int halfSize = kernelSize / 2;
    int k = 200; // Constante : Echelle d'influence des voisins (valeur élevé > forte influence donc lissage), val faible < 50 < moyenne < 200 < élevé
    int c_spatial = 2000; // Constante : Distance entre les pixels (pixels loins on une grande influence), val faible < 50 < moyenne < 1250 < élevé
    int c_spectral = 2000; // Constante : Intensité des pixel (pixels avec une grande intensité de différence on une grande influence), val faible < 50 < moyenne < 1250 < élevé
    int x, y; // Coordonnées dans l'image avec adapté à la fenêtre
    double spatial;
    double spectral;

    int** matrice_image = (int**)malloc(sizeof(int*)*image.rows); // matrice de poids

    for(int i=0; i<image.rows; i++){
        matrice_image[i] = (int*)malloc(sizeof(int)*image.cols);
    }

    Mat bilateral_img = image.clone();

    // Parcourir chaque pixel de l'image pour générer la matrice 
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
    
            uchar& pixel = bilateral_img.at<uchar>(i, j);


            for (int m = -halfSize; m <= halfSize; m++) {
                for (int n = -halfSize; n <= halfSize; n++) {
                    x = j + n;
                    y = i + m;

                    // Vérifier les limites de l'image
                    if (m != n && x >= 0 && x < bilateral_img.cols && y >= 0 && y < bilateral_img.rows) {
                        uchar pixel_value = bilateral_img.at<uchar>(y, x);

                        // Application de la formule sur chaque case de ma matrice
                        spatial = sqrt(pow(i-x, 2) + pow(j-y, 2));
                        spectral = sqrt(pow(pixel_value - pixel, 2));

                        matrice_image[i][j] = k * exp((-spatial)/c_spatial * exp((-spectral)/c_spectral));
                    }
                }
            }

        }
    }


    // Application de le fenêtre gaussienne
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            
            double som = 1;
            double diviseur = 1;
    
            uchar& pixel = bilateral_img.at<uchar>(i, j);

            for (int m = -halfSize; m <= halfSize; m++) {
                for (int n = -halfSize; n <= halfSize; n++) {
                    x = j + n;
                    y = i + m;

                    // Vérifier les limites de l'image
                    if (x >= 0 && x < bilateral_img.cols && y >= 0 && y < bilateral_img.rows) {
                        uchar pixel_value = bilateral_img.at<uchar>(y, x);

                        som += matrice_image[y][x]*pixel_value;
                        diviseur += matrice_image[y][x];
                    }
                }
            }

            // vérifie que le diviseur n'est pas inferieur ou égale à zéro pour permettre de faire la division 
            pixel = (diviseur > 0) ? (som / diviseur) : pixel;

        }
    }


    string fichier_modifie = repertoire+"bilateral-" + string(nomImage);
    imwrite(fichier_modifie.c_str(), bilateral_img); 
    cout << "Image bilateralisée et enregistrée!" << endl;

    return bilateral_img;
}// fin bilateral






Mat energymap(Mat image, string nomImage, string repertoire){

    int kernelSize = 3;
    int halfSize = kernelSize / 2;

    int k=0;


    Mat moy_img = gaussien(image.clone(), nomImage, repertoire);
    Mat energymap_img = laplacien_2(moy_img, nomImage, repertoire);


    // Parcourir chaque pixel de l'image

    
    
        for (int i = 2; i < energymap_img.rows-2; i++) {
            for (int j = 2; j < energymap_img.cols-2; j++) {

            uchar& pixel = energymap_img.at<uchar>(i, j);
            int min = 255;

            // Appliquer le kernel
            for (int m = -halfSize; m <= halfSize; m++) {
                
                int x = j + m;
                int y = i - 1;

                uchar pixel_value = energymap_img.at<uchar>(y, x);

                min = pixel_value < min ? pixel_value : min;
            
            }
 
            // Stocker la valeur
            // pixel = (pixel+min) > 255 ? 255 : (pixel+min);
            pixel += min;
            pixel = pixel > 255 ? 255 : pixel;
            min = 255;
        }
    }

    Mat coloredImage;
    applyColorMap(negatif(energymap_img.clone(), nomImage, repertoire), coloredImage, COLORMAP_JET);


    string fichier_modifie = repertoire+"energymap-" + string(nomImage);
    imwrite(fichier_modifie.c_str(), energymap_img); 

    string fichier_modifie_color = repertoire+"colored_energymap-" + string(nomImage);
    imwrite(fichier_modifie_color.c_str(), coloredImage); 

    cout << "Image energymap et enregistrée!" << endl;

    return coloredImage;
}// fin energymap