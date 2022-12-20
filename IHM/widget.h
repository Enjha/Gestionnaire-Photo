#ifndef WIDGET_H
#define WIDGET_H

#include "image.h"

#include <QWidget>
#include <QStringList>
#include <QListWidgetItem>
#include <galerie.h>
#include <list>
#include <iostream>
#include <QtGui>
#include <QGridLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:

    void on_addFileButton_clicked(); //ajoute une image à la bibliotèque
    void on_resizeImageButton_clicked(); //redimensionne un image selectionnée
    void on_cropImageButton_clicked(); //rogne une image selectionnée
    void on_modifScoreButton_clicked(); //permet de moddifier les note d'une image selectionnée
    void on_modifRemarkButton_clicked(); //permet de moddifier le commentaire d'une image selectionnée
    void on_removeImageButton_clicked(); // permet de supprimer de la bibiliotèque l'image selectionnée
    void on_saveRemarkButton_clicked(); //sauvegarde le commentaire
    void on_saveScoreButton_clicked(); //sauvegarde la note
    void on_listGalerie_itemClicked(QListWidgetItem *item);//permet de selectionner un image dans la bibliothèque
    void on_star1_clicked();
    void on_star2_clicked();
    void on_star3_clicked();
    void on_star4_clicked();
    void on_star5_clicked();
    void on_alphabetSort_clicked(); //trier par ordre alphabétique les images
    void on_scoreSort_clicked(); //trier par note les images
    void on_searchBar_textChanged(); //chercher une image par caractères
    void on_saveLibraryButton_clicked(); //sauvegarder l'état actuel de la bibliotèque
    void on_persoSort_clicked(); //tier les images par odre personnel (drag and drop)

private:
    Ui::Widget *ui;
    QStringList galeriePhotos;
    Galerie galerie;
    bool isInfoVisible; //visibilité d'un composant de l'ui
    Image* currentImage;//image selectionnée
    int currentScore;

    void closeEvent(QCloseEvent *event); //evenement lors de la fermeture de l'application
    void initializationWindow();//initialisation de l'application
    void initializeLayoutInfo(Image* currentImage);//initialisation de l'ui
    void initializeRemark(Image* currentImage); //initialisation du commentaire
    void initializeScore(Image* currentImage);//initialisation de la note
    void setVisibleInformation(bool isVisible); //afficher une élément de l'ui
    void refreshGalerie();//actualiser les images présentes dans la bibliotèque
    void initializeLibrary();//initialiser la bibliotèque avec les images sauvegardées
};
#endif // WIDGET_H
