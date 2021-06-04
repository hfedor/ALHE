#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <vector>
#include"Ware.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_LineEditA_textChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_add_wares_clicked(){};



    void on_pushButton_3_clicked();

    void on_generateButton_clicked();





private:
    Ui::MainWindow *ui;

    QGraphicsScene *scene;
    QGraphicsPolygonItem *magazine;
    std:: vector<Ware> waresFromUser;
};
#endif // MAINWINDOW_H
