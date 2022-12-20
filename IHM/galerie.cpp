#include "galerie.h"

Galerie::Galerie()
{}

Galerie::Galerie(list<Image*> i){
    images = i;
    size = i.size();
}

struct ComparImageName{
    bool operator()(Image * im1, Image * im2){
        return im1->getName().toLower() < im2->getName().toLower();
    }
};
struct ComparImageScore{
    bool operator()(Image * im1, Image * im2){
        return im1->getScore() > im2->getScore();
    }
};

int Galerie::getSize(){
    return images.size();
}

void Galerie::addImage(Image* image){
    if(contains(image)) return;
    images.push_back(image);
    size++;
}
void Galerie::addAllImages(list<Image*> allImages){
    if(allImages.size() == 0) return;
    for(Image* i : allImages){
        addImage(i);
        size++;
    }
}
void Galerie::removeImage(Image* image){
    if(contains(image)){
        images.remove(image);
        size--;
    }
}
void Galerie::removeAllImages(){
    if(size == 0) return;
    for(Image* i : images){
        removeImage(i);
        size--;
    }
}
bool Galerie::contains(Image* image){
    for(Image* i : images){
        if(i->equals(image))
            return true;
    }
    return false;
}
list<Image*> Galerie::getImages(){
    return images;
}

Image* Galerie::getImageByName(QString name){
    if(size == 0) return NULL;
    for(Image* i : images){
        if(name.compare(i->getName()) == 0){
            return i;
        }
    }
    return NULL;
}
void Galerie::sortByScore(){
    images.sort(ComparImageScore());
}
void Galerie::sortByName(){
   images.sort(ComparImageName());
}
