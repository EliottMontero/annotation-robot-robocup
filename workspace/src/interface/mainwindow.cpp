#include "mainwindow.h"

#include <QtWidgets>

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    model = new QStandardItemModel(3, 3, this);

    mainbar = new QMenuBar(this);
    m1 = new QMenu("Mode de Vue",this);
    mainbar->addMenu(m1);
    QGridLayout *layout = new QGridLayout();
    setLayout(layout);

    label1=new QLabel(this);
    label1->setStyleSheet("QLabel { background-color : red; color : blue; }");
    label1->setAlignment(Qt::AlignCenter);

    label2=new QLabel(this);
    label2->setStyleSheet("QLabel { background-color : green; color : blue; }");
    label2->setAlignment(Qt::AlignCenter);


    ouvrirAct=new QAction(tr("ouvrir"),this);
    connect(ouvrirAct, SIGNAL(triggered()), this, SLOT(afficher()));
    mainbar->addMenu(m1);
    m1->addAction(ouvrirAct);
    layout->addWidget(mainbar,0,0);
    layout->addWidget(label2,1,2);
    layout->addWidget(label1,1,1);



    setWindowTitle(tr("NAME"));
}

void Window::afficher(){
            // fichierchoisi=QFileDialog::getOpenFileName(this,
           //                                            tr("Open Image"), "",
            //                                           tr("Image Files (*.png *.ppm *.jpg *.jpeg *.bmp)"));
            //image = new QImageReader("a.jpg");
            //printf("lol");

            pixmap.load("../interface/a.jpeg");
            taille=pixmap.size();
            //taille=QPixmap::scaled((taille.width()/2),(taille.height()/2)).size();
            label1->setVisible(true);
            label1->setFixedSize(taille);
            label1->setPixmap(pixmap);
            label1->setMask(pixmap.mask());
            label1->show();

            label2->setVisible(true);
            label2->setFixedSize(taille);
            label2->setPixmap(pixmap);
            label2->setMask(pixmap.mask());
            label2->show();
}


