#include "utils.h"

Utils::Utils()
{}

Utils::~Utils(){
}

QStringList Utils::split(QString s, char del){
    QStringList list = s.split(del);
    return list;
}

/*void writeImagesSaves(string pathname, Galerie galerie){
    ofstream file(pathname, ios::out | ios::trunc);
    if(file){
        file << galerie.getImages().size() << endl;
        for(Image *img: galerie.getImages()){
            QImage originalPixmap;
            QString str = ":\\saves\\saves\\";
            QFile savedImage(str);
            originalPixmap.save(&savedImage, "JPG");

            file << img->getName().toStdString() << " " << img->getScore() << endl;
            file << img->getRemark().toStdString() << endl;
            cout << " " << endl;

        }
        file.close();
    }else cerr << "ERROR with opening file" << endl;
}

void readImagesSaves(string pathname, Galerie galerie){

    ifstream file(pathname, ios::in);
    QStringList content;
    string remark = "";

        if(file){
            string param;
            getline(file, param);
            int numberOfImages = stoi(param);
            cout << "nombre d'image : " << numberOfImages << endl;
            for(int i = 0; i < numberOfImages ; i++){
                getline(file, param);
                cout << param << endl;
                Utils *util = new Utils();
                content = util->split(QString::fromStdString(param),' ');
                cout << "content : " << content.size() << endl;

                QString imageName = content.at(0);
                int imageScore = stoi(content.at(1).toStdString());

                getline(file, param);
                QString imageRemark = QString::fromStdString(param);

                Image *image = new Image(imageName, imageScore,imageRemark);
                galerie.addImage(image);
            }

        }else cerr << "ERROR with opening file" << endl;
}*/


