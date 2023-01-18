#include "premierefenetre.h"
#include "gestionbdd.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //gestionBDD g;
    PremiereFenetre w;

    return a.exec();
}
