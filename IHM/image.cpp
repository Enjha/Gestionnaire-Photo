#include "image.h"


Image::Image(){}

Image::Image(QString path, QImage image){
    _name = u->split(path, '/').takeLast();
    _path = path;
    _image = image;
    _score = 0;
    _remark = "";
}

Image::Image(QString path, QImage image,QString name, int score, QString remark){
    _path = path;
    _image = image;
    _name = name;
    _score = score;
    _remark = remark;
}

/* Méthodes utiles */
bool Image::equals(Image* image){
    if(_name.compare(image->getName()) == 0 )return true;
    else return false;
}

/* Accesseurs */
QString Image::getName(){
    return _name;
}
QString Image::getPath(){
    return _path;
}
void Image::setImage(QImage t){
    _image = t;
}
QImage Image::getImage(){
    return _image;
}

void Image::setScore(int s){
    _score = s;
}
int Image::getScore(){
    return _score;
}

void Image::setRemark(QString r){
    _remark = r;
}
QString Image::getRemark(){
    return _remark;
}

void Image::setColorDominant(Color cd){
    _colorDominant = cd;
}

Color Image::getColorDominant(){
    return _colorDominant;
}

int Image::getHeight(){
    return _image.height();
}

int Image::getWidth(){
    return _image.width();
}

