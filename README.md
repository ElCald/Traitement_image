# Traitement d'images par filtres
CHPS0703 - Imagerie 

### Informations
**Nécessite d'installer la bibliothèque OpenCV.**

À l'execution, le porgramme va créer un répertoire `/images` à la racine et à l'intérieur un répertoire avec le nom de l'image. <br>
Supporte les extensions basiques : jpg, png.

Si nécessaire sous Linux, déplacer le répertoire `opencv2` qui se trouve dans `/usr/include/opencv4/`, vers le répertoire `/usr/include`, car il est possible que ce répertoire se trouve dans `/opencv4` et donc le compilateur ne trouve pas les fichiers d'en-tête.

---------------
#### Compilation
```sh
make
```

#### Execution
```sh
./main file/path/file.jpg
```

## Filtres
- binarisation
- négatif
- quantification
- réhaussement
- égalisation
- érosion
- dilatation
- ouverture
- fermeture
- moyenne
- gaussien
- minimum
- maximum
- médiane
- sobel
- sharr
- laplacien v1
- laplacien v2
- bilatéralisation
- energymap

## Seam carving
Algorithme permattant d'éliminer les zones "inutiles" dans une image afin d'en réduire sa taille.

### Pré-traitement
Application d'un filtre gaussien afin de lisser l'image et d'un filtre gradient afin de faire ressortir les bords des objets.

### Matrice cumulative
Création d'une matrice cumulative aux dimensions de l'image.

#### Algorithme
- Initialisation de la 1ère ligne de la matrice avec la 1ère ligne de l'image (ligne 0).
- À partir de la ligne 1 et en boucle jusqu'en bas de l'image. Dans la case courante de la matrice, placer la somme du pixel courant et du pixel minimum des 3 pixels au dessus du pixel courant de l'image(int).
- Faire attention aux cas sur les bords droit et gauche, afin de ne pas sortir de l'image pour chercher le pixel minimum.

### Recherche du chemin minimum
Avec la matrice cumulative rechercher le chemin minimum de l'image.

#### Algorithme
- Parcourir la dernière ligne pour trouver l'indice de la valeur minimum et l'ajouter à un tableau `path` de la taille de la hauteur de l'image.
- À partir de cet indice rechercher parmis les 3 pixels au dessus, le pixel minimum, l'ajouter au tableau "path" et prendre ce pixel comme référence d'indice de recherche.
- Répéter jusqu'en haut de l'image.

### Suppression du chemin minimum
Avec le tableau `path`, supprimer le chemin de l'image.

#### Algorithme
- Parcourir le tableau `path` et supprimer le pixel à l'indice i et à la valeur de la case i deu tableau `path`
- Décaler tous les pixels de l'image vers la droite ou la gauche.
- Répéter ceci jusqu'en haut de l'image.

### Algorithme générale
Répéter n fois dans l'ordre.
- Création d'une matrice cumulative de l'image.
- Rechercher le chemin minimum.
- Supprimer le chemin minimum.

Il faut toujours utiliser l'image qui est modifiée afin de pouvoir trouver un nouveau chemin minimum.

## Example
![screenshot](Examples/oiseaux.jpg) 

Original <br>

![screenshot](Examples/seamed_cols-oiseaux.jpg)

Paths <br>

![screenshot](Examples/resized_cols-oiseaux.jpg)

Resized
