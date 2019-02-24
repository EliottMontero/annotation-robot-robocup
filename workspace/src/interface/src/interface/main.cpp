#include <QApplication>
#include "mainwindow.h"

#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QLayout>
#include <QtWidgets>
#include <QPushButton>


#include <QLineEdit>
#include <QFormLayout>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;

    window.show();


    return app.exec();
}
