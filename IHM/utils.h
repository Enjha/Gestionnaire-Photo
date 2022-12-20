#ifndef UTILS_H
#define UTILS_H
#include "stdio.h"
#include <iostream>
#include <list>
#include <QString>
#include <QStringList>

using namespace std;

class Utils
{

public:
    Utils();
    ~Utils();
    int len(QString str);
    QStringList split(QString str, char seperator);//produire une liste de QString avec une QString séparé par un char
    //void writeImagesSaves(string pathname, Galerie galerie);
    //void readImagesSaves(string pathname, Galerie galerie);
};

#endif // UTILS_H
