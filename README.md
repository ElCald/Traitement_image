### CHPS0703 - Imagerie 
TP traitement d'images avec des filtres

#### Informations
**Nécessite d'installer la bibliothèque OpenCV.**

À l'execution, le porgramme va créer un répertoire `/images` à la racine et à l'intérieur un répertoire avec le nom de l'image.
Supporte les extensions basiques : jpg, png

Si nécessaire sous linux, déplacer le répertoire `opencv2` qui se trouve dans `/usr/include/opencv4/`, vers le répertoire `/usr/include`, car il est possible que ce répertoire se trouve dans `/opencv4` et donc le compilateur ne trouve pas les fichiers d'en-tête.

---------------
#### Compilation
```sh
make
```

#### Execution
```sh
./main file/path/file.jpg
```
