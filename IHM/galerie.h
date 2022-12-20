#ifndef GALERIE_H
#define GALERIE_H
#include "image.h"

class Galerie
{
private:
    int size;//nombre d'image de la galerie
    list<Image*> images;//Liste contenant toutes les images de la galerie

public:
    Galerie();
    Galerie(list<Image*>);

    void addImage(Image* image);//permet d'ajouter une image à la galerie
    void addAllImages(list<Image*> images);//permet d'ajouter plusieurs images à la galerie
    void removeImage(Image* image);//permet de supprimer une image de la galerie
    void removeAllImages();//permet de supprimer plusieurs images de la galerie
    bool contains(Image* image);//renvoie true si l'image en paramètre est contenu dans la galerie
    void sortByName();//trier par nom
    void sortByScore();//trier par score
    list<Image*> getListe() {return images;};
    Image* getImageByName(QString name);//retourne l'image associée à son id (nom)

    /* Accesseurs */
    int getSize();
    list<Image*> getImages();
};

#endif // GALERIE_H
