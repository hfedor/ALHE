#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <iostream>
#include <time.h>
#include <cstdlib>
#include <algorithm>
#include "Ware.h"
#include "trianglewarehouse.h"
#include "rectanglewarehouse.h"
#include "trapezewarehouse.h"
#include "population.h"

#include <vector>

using namespace std;

QColor RandWareColor()
{
    int R,G,B;
    R = 97 + (rand() % 34);
    G = 61 + (rand() % 37);
    B = 19 + (rand() % 41);
    return QColor(R,G,B,255);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    QBrush blueBrush(QColor(84, 84, 84, 255));
    QPen outlinePen(Qt::black);
    outlinePen.setWidth(2);

    int populationSize = 40;
    int lambda = 70;
    int generationCount = 1000;
    int mutateBySwapRate = 10;
    int mutateByRotationRate = 10;

    TrapezeWarehouse trapezeWarehouse(3,-1,40,2,32);
    trapezeWarehouse.initRandom(60);

    typedef  Population<TrapezeWarehouse> TrapezeWarehousePopulation;

    TrapezeWarehousePopulation population(populationSize,lambda,generationCount,mutateBySwapRate,mutateByRotationRate,trapezeWarehouse);

    population.calculateMiPlusLambda();

    std::vector<Point> points;
    points = trapezeWarehouse.GetVerticles();

    QPolygonF polygon;

    for(std::vector<Point>::iterator i = points.begin(); i != points.end(); i++)
    polygon << QPointF(float(20*(*i).GetX()), float(20*(*i).GetY()));

    magazine = scene->addPolygon(polygon, outlinePen, blueBrush);

    std::vector<Ware> wares = population.GetBest().GetWares();

    for(std::vector<Ware>::iterator i = wares.begin(); i != wares.end(); i++)
        if((*i).x >= 0)
        {
            QBrush wareBrush(RandWareColor());
            scene->addRect(QRect(20*(*i).x, 20*(*i).y, 20*(*i).getActualWidth(), 20*(*i).getActualHeight()), outlinePen, wareBrush);
        }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_LineEditA_textChanged(const QString &arg1)
{
}


void MainWindow::on_pushButton_clicked()
{
    ui->graphicsView->scene()->clear();

    QPolygonF polygon;

    QBrush wareBrush(Qt::green);
    QBrush blueBrush(Qt::blue);
    QPen outlinePen(Qt::black);
    outlinePen.setWidth(2);

    int populationSize = 40;
        int lambda = 70;
        int generationCount = 1000;
        int mutateBySwapRate = 10;
        int mutateByRotationRate = 10;

    int A = std::atof(ui->LineEditA->text().toStdString().c_str());
    int B = std::atof(ui->LineEditB->text().toStdString().c_str());
    int C = std::atof(ui->LineEditC->text().toStdString().c_str());


    TriangleWarehouse triangleWarehouse(A,B,C,2);
    triangleWarehouse.initRandom(60);

    Population population(populationSize,lambda,generationCount,mutateBySwapRate,mutateByRotationRate,triangleWarehouse);

    population.calculateMiPlusLambda();

    std::vector<Point> points;
    points = triangleWarehouse.GetVerticles();

    for(std::vector<Point>::iterator i = points.begin(); i != points.end(); i++)
        polygon << QPointF(float(20*(*i).GetX()), float(20*(*i).GetY()));

    magazine = scene->addPolygon(polygon, outlinePen, blueBrush);

    std::vector<Ware> wares = population.GetBest().GetWares();

    for(std::vector<Ware>::iterator i = wares.begin(); i != wares.end(); i++)
        if((*i).x >= 0)
        {
            QGraphicsRectItem *ware;
            ware = scene->addRect(QRect(20*(*i).x, 20*(*i).y, 20*(*i).getActualWidth(), 20*(*i).getActualHeight()), outlinePen, wareBrush);
            ware->setFlag(QGraphicsItem::ItemIsMovable);
        }
}


void MainWindow::on_pushButton_2_clicked()
{
}

