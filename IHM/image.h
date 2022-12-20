#ifndef IMAGE_H
#define IMAGE_H
#include "stdio.h"
#include "utils.h"
#include <iostream>
#include <QImage>
#include <QString>
#include <QSize>

enum class Color { Red, Green, Blue };

class Image
{
private:
    QString _name;//nom de l'image (unique)
    QString _path;//chemin de l'image (unique)
    QImage _image; //texture de l'image
    int _score; //note de l'image (sur 5 étoiles)
    QString _remark; //commentaire de l'image
    Color _colorDominant;//la couleur qui domine le plus sur la texture de l'image
    Utils* u = new Utils();

public:
    /*Constructeurs*/
    Image();
    Image(QString _path, QImage _image);
    Image(QString _path, QImage _image, QString _name, int _score, QString _remark);


    /* Méthodes utiles */
    bool equals(Image* image); //comparaison d'égalité entre deux images
    friend bool operator<(Image& i1, Image& i2){
        return i1.getName() < i2.getName();
    }

    /* Accesseurs */
    QString getName();
    QString getPath();
    void setImage(QImage image);
    QImage getImage();
    void setScore(int score);
    int getScore();
    void setRemark(QString remark);
    QString getRemark();
    void setColorDominant(Color colorDominant);
    Color getColorDominant();
    int getHeight();
    int getWidth();
};

#endif // IMAGE_H
