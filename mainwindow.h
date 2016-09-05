#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QVector>
#include "mynetwork.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    myNetwork *network;
    void setUpConnection();
    QVector<QVector<QString>> plots;

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // MAINWINDOW_H
