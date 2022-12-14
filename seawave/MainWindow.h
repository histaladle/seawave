#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "RenderWidget.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    RenderWidget *renderWidget=nullptr;
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
