#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <cmath>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QVector3D n(1,2,3);
    n.normalize();
    qDebug() << n;

    renderWidget=new RenderWidget(this);
    renderWidget->setFocus();
    ui->gridLayout_main->addWidget(renderWidget);
    int fou_len=1024,type=-1;
    double omega=2*M_PI/16,delta_t=-1;
    double *seq=new double[fou_len];
    FT::Complex *fou=new FT::Complex[fou_len];
    FT::Complex *ifou=new FT::Complex[fou_len];
    for(int i=0;i<fou_len;i++) {
        seq[i]=sin(omega*i*delta_t);
        fou[i].re=sin(omega*i*delta_t);
    }
    FT::fast_iterate(fou,fou_len,type);
    for(int i=0;i<fou_len;i++) {
        ifou[i]=fou[i]/fou_len;
    }
    FT::fast_iterate(ifou,fou_len,-type);
//    QByteArray ba;
//    QFile file("a.csv");
//    file.open(QFile::WriteOnly);
//    for(int i=0;i<fou_len;i++) {
//        ba.clear();
//        ba.append(QString::number(i));
//        file.write(ba);
//        file.write(",");
//        ba.clear();
//        ba.append(QString::number(seq[i]));
//        file.write(ba);
//        file.write(",");
//        ba.clear();
//        ba.append(QString::number(fou[i].mod()));
//        file.write(ba);
//        file.write(",");
//        ba.clear();
//        ba.append(QString::number(ifou[i].re));
//        file.write(ba);
//        file.write(",");
//        ba.clear();
//        ba.append(QString::number(ifou[i].im));
//        file.write(ba);
//        file.write("\n");
//    }
//    file.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}
