#include "widget.h"
#include "ui_widget.h"

#include <QFileDialog>
#include <QTextOption>
#include <QLabel>
#include <QMessageBox>
#include <QAbstractItemView>
#include <QInputDialog>
#include <limits>
#include <string>
#include <iostream>
#include <istream>
#include <fstream>
#include <QCloseEvent>
#include <QDesktopWidget>

using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle(tr("Bibliothèque de Photos"));
    initializationWindow();
    ui->removeImageButton->setIcon(QIcon(":/icons/resources/remove.png"));
    ui->resizeImageButton->setIcon(QIcon(":/icons/resources/resize.png"));
    ui->cropImageButton->setIcon(QIcon(":/icons/resources/crop.png"));
    ui->addFileButton->setIcon(QIcon(":/icons/resources/file.png"));
    ui->saveLibraryButton->setIcon(QIcon(":/icons/resources/save.png"));
    ui->modifRemarkButton->setIcon(QIcon(":/icons/resources/remark.png"));
    ui->modifScoreButton->setIcon(QIcon(":/icons/resources/score.png"));
}

Widget::~Widget()
{
    delete ui;
}

//evenement lors de la fermeture de l'application
void Widget::closeEvent(QCloseEvent *event){
    event->ignore();
    QMessageBox quit;
    quit.setText("Voulez-vous sauvegardez vos modifications avant de quitter ?");
    QAbstractButton *BoutonOui = quit.addButton("Oui", QMessageBox::YesRole);
    QAbstractButton *BoutonNon = quit.addButton("Non", QMessageBox::NoRole);
    quit.setIcon(QMessageBox::Question);
    quit.exec();

    if (quit.clickedButton()==BoutonNon){
        event->accept();
    }
    else{
        Utils *uti = new Utils();
        event->accept();
    }
}

// Ajout de fichier(image)
void Widget::on_addFileButton_clicked(){

    QStringList filesPath = QFileDialog::getOpenFileNames(this, tr("Open Image"), "/home/jana", tr("Image Files (*.png *.jpg *.bmp)"));

    if(filesPath.size() == 0)return;

    for(QString s : filesPath){
        QPixmap *pixmap_img = new QPixmap(s);
        QImage imgTemp(pixmap_img->toImage());
        Image *img = new Image(s, imgTemp);

        if(!galerie.contains(img)){
            galerie.addImage(img);
            QListWidgetItem *item = new QListWidgetItem(ui->listGalerie);
            item->setSizeHint(ui->listGalerie->sizeHint()/2);
            ui->listGalerie->setResizeMode(QListView::Adjust);
            item->setText(img->getName());
            item->setData(Qt::DecorationRole, QPixmap::fromImage(img->getImage()).scaled(item->sizeHint().width(), item->sizeHint().height() ,Qt::IgnoreAspectRatio));
        }else{
            QMessageBox suppr;
            suppr.setText("Vous ne pouvez pas ajoutez deux fois le même fichier: \n"+img->getName()+".");
            suppr.exec();
        }
    }
    refreshGalerie();
}

void Widget::on_resizeImageButton_clicked(){

    bool ok1 = true;
    int largeur = QInputDialog::getInt(this, tr("Redimension"),tr("Largeur (en pixels):\n"
                                                                  "(l'application garde un ratio équilibré pour une bonne qualité d'image)"), currentImage->getWidth(), 16, 2024, 1, &ok1);
    if (ok1 && largeur >= 16 && largeur <= 2024){
        bool ok2 = true;
        int hauteur = QInputDialog::getInt(this, tr("Redimension"),tr("Hauteur (en pixels): \n"
                                                                      "(l'application garde un ratio équilibré pour une bonne qualité d'image)"), currentImage->getHeight(), 64, 2024, 1, &ok2);
        if (ok1 && hauteur >= 16 && hauteur <= 1800){
            currentImage->getImage().scaled(largeur, hauteur, Qt::KeepAspectRatio);
            QPixmap pix = QPixmap::fromImage(currentImage->getImage()).scaled(largeur, hauteur, Qt::KeepAspectRatio);
            QImage img = pix.toImage();
            currentImage->setImage(img);
            ui->selectedPhoto->setPixmap(pix);
            ui->imageDimensions->setText(QString::fromStdString(to_string(currentImage->getWidth())+"x"+to_string(currentImage->getHeight())));
        }
    }
}

void Widget::on_cropImageButton_clicked(){

    bool okd = true;
    int marge_droite = QInputDialog::getInt(this, tr("Rogner"),tr("Marge à droite (en pixels):"), 0, 0, currentImage->getWidth(), 1, &okd);
    if (okd && marge_droite >= 0 && marge_droite <= currentImage->getWidth()){
        bool okh = true;
        int marge_haut = QInputDialog::getInt(this, tr("Rogner"),tr("Marge en haut (en pixels):"), 0, 0, currentImage->getHeight(), 1, &okh);
        if (okh && marge_haut >= 0 && marge_haut <= currentImage->getHeight()){
            bool okl = true;
            int largeur = QInputDialog::getInt(this, tr("Redimension après Rognage"),tr("Largeur (en pixels):"), currentImage->getWidth(), 16 , currentImage->getWidth() - marge_droite , 1, &okl);
            if (okl && largeur >= 16 && largeur <= currentImage->getWidth() - marge_droite){
                bool okw = true;
                int hauteur = QInputDialog::getInt(this, tr("Redimension après Rognage"),tr("Hauteur (en pixels):"), currentImage->getHeight(), 64, currentImage->getHeight() - marge_haut, 1, &okw);
                if (okw && hauteur >= 64 && hauteur <= currentImage->getHeight() - marge_haut){
                    QImage copy = currentImage->getImage().copy(marge_droite,marge_haut,largeur, hauteur);
                    ui->selectedPhoto->setPixmap(QPixmap::fromImage(copy));
                    ui->imageDimensions->setText(QString::fromStdString(to_string(currentImage->getWidth())+"x"+to_string(currentImage->getHeight())));
                }
            }
        }
    }
}

void Widget::on_removeImageButton_clicked(){
    QMessageBox suppr;
    suppr.setText("Êtes-vous sur de bien vouloir supprimer cette image ?");
    QAbstractButton *BoutonOui = suppr.addButton("Oui", QMessageBox::YesRole);
    QAbstractButton *BoutonNon = suppr.addButton("Non", QMessageBox::NoRole);
    suppr.setIcon(QMessageBox::Question);
    suppr.exec();

   if (suppr.clickedButton() == BoutonOui) {
        galerie.removeImage(currentImage);
        QListWidgetItem *item = ui->listGalerie->takeItem(ui->listGalerie->currentRow()); //TODO
        delete item;
        setVisibleInformation(false);
    }
}

void Widget::on_listGalerie_itemClicked(QListWidgetItem *item){

    currentImage = galerie.getImageByName(item->text());
    initializeLayoutInfo(currentImage);

}

void Widget::on_alphabetSort_clicked(){
    refreshGalerie();
}


void Widget::on_scoreSort_clicked(){
    refreshGalerie();
}

void Widget::on_persoSort_clicked(){

}

void Widget::refreshGalerie(){
    if(ui->alphabetSort->isChecked()) galerie.sortByName();
    else if(ui->scoreSort->isChecked()) galerie.sortByScore();

    ui->listGalerie->clear();

    for(Image* i : galerie.getImages()){
        if(ui->searchBar->text().compare("") != 0)
            if(!(i->getName().contains(ui->searchBar->text()))) continue;

        QListWidgetItem *item = new QListWidgetItem(ui->listGalerie);
        item->setSizeHint(ui->listGalerie->sizeHint()/2);
        ui->listGalerie->setResizeMode(QListView::Adjust);
        item->setText(i->getName());
        item->setData(Qt::DecorationRole, QPixmap::fromImage(i->getImage()).scaled(item->sizeHint().width(), item->sizeHint().height() ,Qt::IgnoreAspectRatio));
    }
}

void Widget::setVisibleInformation(bool isVisible){
    for (int i = 0; i < ui->LayoutInformation->count(); ++i){

        QWidget *widget = ui->LayoutInformation->itemAt(i)->widget();
        if (widget != NULL)
        {
            widget->setVisible(isVisible);
        }
    }
    for (int i = 0; i < ui->LayoutOptions1->count(); ++i){

        QWidget *widget = ui->LayoutOptions1->itemAt(i)->widget();
        if (widget != NULL)
        {
            widget->setVisible(isVisible);
        }
    }
    for (int i = 0; i < ui->LayoutOptions2->count(); ++i){

        QWidget *widget = ui->LayoutOptions2->itemAt(i)->widget();
        if (widget != NULL)
        {
            widget->setVisible(isVisible);
        }
    }
    for (int i = 0; i < ui->LayoutPhoto->count(); ++i){

        QWidget *widget = ui->LayoutPhoto->itemAt(i)->widget();
        if (widget != NULL)
        {
            widget->setVisible(isVisible);
        }
    }

    isInfoVisible = isVisible;
}

void Widget::on_modifRemarkButton_clicked(){

    ui->saveRemarkButton->setVisible(true);
    ui->modifRemarkButton->setVisible(false);
    ui->remarkEdit->setVisible(true);
    ui->remark->setVisible(false);
}

void Widget::on_saveRemarkButton_clicked(){
    currentImage->setRemark(ui->remarkEdit->toPlainText());
    if(ui->remarkEdit->toPlainText().compare("") != 0){
        if(ui->modifRemarkButton->text().compare(" Ajouter commentaire") == 0)
            ui->modifRemarkButton->setText(" Modifier commentaire");
        ui->remark->setText(ui->remarkEdit->toPlainText());
        ui->remark->setStyleSheet("QLabel#remark{ color: black; }");
    }else{
        if(ui->modifRemarkButton->text().compare(" Ajouter commentaire") != 0)
            ui->modifRemarkButton->setText(" Ajouter commentaire");
        ui->remark->setText("Pas de commentaire.");
        ui->remark->setStyleSheet("QLabel#remark{ color: gray; }");
    }
    ui->remarkEdit->setVisible(false);
    ui->remark->setVisible(true);
    ui->saveRemarkButton->setVisible(false);
    ui->modifRemarkButton->setVisible(true);
}

void Widget::initializationWindow(){
    ui->listGalerie->setDragDropMode(QAbstractItemView::InternalMove);

    setFixedSize(ui->LayoutPrincipal->parentWidget()->size());
    setWindowTitle("Bibliothèque de photos");
    setVisibleInformation(false);
    ui->saveScoreButton->setVisible(false);
    ui->saveRemarkButton->setVisible(false);
    ui->remarkEdit->setVisible(false);
    ui->remarkEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->remarkEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void Widget::initializeScore(Image* currentImage){
    QPixmap voidStar (":/icons/resources/void_star.png");
    QPixmap fullStar (":/icons/resources/full_star.png");
    switch(currentImage->getScore()){
    case(0):
        ui->star1->setIcon(QIcon(voidStar));
        ui->star2->setIcon(QIcon(voidStar));
        ui->star3->setIcon(QIcon(voidStar));
        ui->star4->setIcon(QIcon(voidStar));
        ui->star5->setIcon(QIcon(voidStar));
        break;
    case(1):
        ui->star1->setIcon(QIcon(fullStar));
        ui->star2->setIcon(QIcon(voidStar));
        ui->star3->setIcon(QIcon(voidStar));
        ui->star4->setIcon(QIcon(voidStar));
        ui->star5->setIcon(QIcon(voidStar));
        break;
    case(2):
        ui->star1->setIcon(QIcon(fullStar));
        ui->star2->setIcon(QIcon(fullStar));
        ui->star3->setIcon(QIcon(voidStar));
        ui->star4->setIcon(QIcon(voidStar));
        ui->star5->setIcon(QIcon(voidStar));
        break;
    case(3):
        ui->star1->setIcon(QIcon(fullStar));
        ui->star2->setIcon(QIcon(fullStar));
        ui->star3->setIcon(QIcon(fullStar));
        ui->star4->setIcon(QIcon(voidStar));
        ui->star5->setIcon(QIcon(voidStar));
        break;
    case(4):
        ui->star1->setIcon(QIcon(fullStar));
        ui->star2->setIcon(QIcon(fullStar));
        ui->star3->setIcon(QIcon(fullStar));
        ui->star4->setIcon(QIcon(fullStar));
        ui->star5->setIcon(QIcon(voidStar));
        break;
    case(5):
        ui->star1->setIcon(QIcon(fullStar));
        ui->star2->setIcon(QIcon(fullStar));
        ui->star3->setIcon(QIcon(fullStar));
        ui->star4->setIcon(QIcon(fullStar));
        ui->star5->setIcon(QIcon(fullStar));
        break;
    default:
        break;
    }
    ui->star1->setFlat(true);
    ui->star1->setIconSize(fullStar.rect().size());
    ui->star1->setDisabled(true);
    ui->star1->setCursor(Qt::ArrowCursor);

    ui->star2->setFlat(true);
    ui->star2->setIconSize(fullStar.rect().size());
    ui->star2->setDisabled(true);
    ui->star2->setCursor(Qt::ArrowCursor);

    ui->star3->setFlat(true);
    ui->star3->setIconSize(fullStar.rect().size());
    ui->star3->setDisabled(true);
    ui->star3->setCursor(Qt::ArrowCursor);

    ui->star4->setFlat(true);
    ui->star4->setIconSize(fullStar.rect().size());
    ui->star4->setDisabled(true);
    ui->star4->setCursor(Qt::ArrowCursor);

    ui->star5->setFlat(true);
    ui->star5->setIconSize(fullStar.rect().size());
    ui->star5->setDisabled(true);
    ui->star5->setCursor(Qt::ArrowCursor);
}

void Widget::initializeLayoutInfo(Image* currentImage){
    if(!isInfoVisible){
        setVisibleInformation(true);
    }
    initializeRemark(currentImage);
    initializeScore(currentImage);

    QString dimensionsTmp = QString::fromStdString(to_string(currentImage->getWidth()) + "x" + to_string(currentImage->getHeight()));
    QString dimensions = dimensionsTmp;
    QPixmap pixmap_img = QPixmap::fromImage(currentImage->getImage());

    ui->imageName->setText(currentImage->getName());
    ui->imageDimensions->setText(dimensions);
    if(pixmap_img.width() > ui->selectedPhoto->width() && pixmap_img.height() > ui->selectedPhoto->height()){
        ui->selectedPhoto->setPixmap(pixmap_img.scaled(ui->selectedPhoto->width(), ui->selectedPhoto->height(), Qt::KeepAspectRatio));
    }else{
        ui->selectedPhoto->setPixmap(pixmap_img.scaled(currentImage->getWidth(), currentImage->getHeight(), Qt::KeepAspectRatio));
    }
    ui->selectedPhoto->setAlignment(Qt::AlignCenter);

    ui->saveScoreButton->setVisible(false);
    ui->modifScoreButton->setVisible(true);
    ui->saveRemarkButton->setVisible(false);
    ui->modifRemarkButton->setVisible(true);
    ui->remarkEdit->setVisible(false);
    ui->remark->setVisible(true);
}

void Widget::initializeRemark(Image* currentImage){
    if(currentImage->getRemark() == ""){
        ui->remark->setText("Pas de commentaire.");
        ui->remark->setStyleSheet("QLabel#remark{ color: gray; }");
        ui->modifRemarkButton->setText(" Ajouter commentaire");
    }else{
        ui->remark->setText(currentImage->getRemark());
    }
}

void Widget::on_modifScoreButton_clicked(){
    ui->star1->setDisabled(false);
    ui->star1->setCursor(Qt::PointingHandCursor);

    ui->star2->setDisabled(false);
    ui->star2->setCursor(Qt::PointingHandCursor);

    ui->star3->setDisabled(false);
    ui->star3->setCursor(Qt::PointingHandCursor);

    ui->star4->setDisabled(false);
    ui->star4->setCursor(Qt::PointingHandCursor);

    ui->star5->setDisabled(false);
    ui->star5->setCursor(Qt::PointingHandCursor);

    ui->modifScoreButton->setVisible(false);
    ui->saveScoreButton->setVisible(true);
}

void Widget::on_saveScoreButton_clicked(){
    ui->star1->setDisabled(true);
    ui->star1->setCursor(Qt::ArrowCursor);

    ui->star2->setDisabled(true);
    ui->star2->setCursor(Qt::ArrowCursor);

    ui->star3->setDisabled(true);
    ui->star3->setCursor(Qt::ArrowCursor);

    ui->star4->setDisabled(true);
    ui->star4->setCursor(Qt::ArrowCursor);

    ui->star5->setDisabled(true);
    ui->star5->setCursor(Qt::ArrowCursor);

    ui->saveScoreButton->setVisible(false);
    ui->modifScoreButton->setVisible(true);
    currentImage->setScore(currentScore);

    refreshGalerie();
}

void Widget::on_star1_clicked(){
    QPixmap voidStar (":/icons/resources/void_star.png");
    QPixmap fullStar (":/icons/resources/full_star.png");


    ui->star1->setIcon(QIcon(fullStar));
    ui->star2->setIcon(QIcon(voidStar));
    ui->star3->setIcon(QIcon(voidStar));
    ui->star4->setIcon(QIcon(voidStar));
    ui->star5->setIcon(QIcon(voidStar));
    currentScore = 1;
}


void Widget::on_star2_clicked(){
    QPixmap voidStar (":/icons/resources/void_star.png");
    QPixmap fullStar (":/icons/resources/full_star.png");

    ui->star1->setIcon(QIcon(fullStar));
    ui->star2->setIcon(QIcon(fullStar));
    ui->star3->setIcon(QIcon(voidStar));
    ui->star4->setIcon(QIcon(voidStar));
    ui->star5->setIcon(QIcon(voidStar));
    currentScore = 2;
}


void Widget::on_star3_clicked(){
    QPixmap voidStar (":/icons/resources/void_star.png");
    QPixmap fullStar (":/icons/resources/full_star.png");

    ui->star1->setIcon(QIcon(fullStar));
    ui->star2->setIcon(QIcon(fullStar));
    ui->star3->setIcon(QIcon(fullStar));
    ui->star4->setIcon(QIcon(voidStar));
    ui->star5->setIcon(QIcon(voidStar));
    currentScore = 3;
}

void Widget::on_star4_clicked(){
    QPixmap voidStar (":/icons/resources/void_star.png");
    QPixmap fullStar (":/icons/resources/full_star.png");

    ui->star1->setIcon(QIcon(fullStar));
    ui->star2->setIcon(QIcon(fullStar));
    ui->star3->setIcon(QIcon(fullStar));
    ui->star4->setIcon(QIcon(fullStar));
    ui->star5->setIcon(QIcon(voidStar));
    currentScore = 4;
}


void Widget::on_star5_clicked(){
    QPixmap fullStar (":/icons/resources/full_star.png");

    ui->star1->setIcon(QIcon(fullStar));
    ui->star2->setIcon(QIcon(fullStar));
    ui->star3->setIcon(QIcon(fullStar));
    ui->star4->setIcon(QIcon(fullStar));
    ui->star5->setIcon(QIcon(fullStar));
    currentScore = 5;
}

void Widget::on_searchBar_textChanged(){
    refreshGalerie();
}

void Widget::initializeLibrary(){

    QString pathname = QDir::current().absolutePath() + "/save.txt";
    ifstream file(pathname.toStdString(), ios::in);

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

            QString s = QDir::current().absolutePath() + imageName;
            QPixmap *pixmap_img = new QPixmap(s);
            QImage imgTemp(pixmap_img->toImage());

            Image *image = new Image(s, imgTemp, imageName, imageScore,imageRemark);
            galerie.addImage(image);
        }

    }else cerr << "Erreur d'ouverture de fichier" << endl;

}


void Widget::on_saveLibraryButton_clicked(){

    //test with QFile
    QFile qfile(QDir::current().absolutePath() + "/save.txt");

    qDebug() << QDir::current().absolutePath();

    QString core = QCoreApplication::applicationDirPath();

    qDebug() << core;

    if (!qfile.open(QIODevice::WriteOnly | QIODevice::Text)){
        cerr << "ERROR QFILE" << endl;
    }else{
        QTextStream out(&qfile);
        out << "Write the number : " << 75 << "\n";
        //char *hello = "hello";
        //qfile.write(hello, qstrlen(hello));
    }

    //test with stream
    QString pathname = "IHM:\\resources\\save.txt";
    ofstream file(pathname.toStdString(), ios::out | ios::trunc);
    if(file){
        file << galerie.getImages().size() << endl;
        for(Image *img: galerie.getImages()){
            QImage originalPixmap;
            QString str = ":/saves/resources/";
            QFile savedImage(str);
            originalPixmap.save(&savedImage, "JPG");

            file << img->getName().toStdString() << " " << img->getScore() << endl;
            file << img->getRemark().toStdString() << endl;

        }
        file.close();
    }else cerr << "Erreur d'ouverture de fichier" << endl;
}

