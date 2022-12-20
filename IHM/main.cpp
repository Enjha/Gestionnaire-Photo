#include "widget.h"
#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QIcon::setThemeName("Visionneuse Photo"); //nom de la fenêtre principale
    Widget w;

    // Chargement de la feuille de style
    QFile stylesheet( ":/styles/resources/stylesheet.qss" );
    stylesheet.open( QFile::ReadOnly );

    // Application de la feuille de style
    QString style(stylesheet.readAll());
    a.setStyleSheet(style);

    w.show();
    return a.exec();
}
