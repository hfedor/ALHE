#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <iostream>
#include <time.h>
#include <cstdlib>
#include <algorithm>
#include "Ware.h"
#include "individual.h"
#include "trianglewarehouse.h"
#include "rectanglewarehouse.h"
#include "rhombuswarehouse.h"

#include <vector>

std::vector<Individual> shuffle(std::vector<Individual> pop)
{
    for(auto & item : pop)
    {
        std::swap(item,pop[rand()%pop.size()]);
    }
    return pop;
}


std::vector<Individual>
calculate(int populationSize, int lambda, int generationCount, int mutateBySwapRate, int mutateByRotationRate,
          int warehouseWidth, int warehouseHeight, int hallwayWidth, Individual &p) {
    std::vector<Individual> population;
    //generowanie populacji
    for(int i =0; i< populationSize;i++)
    {
        p.shuffle();
        population.push_back(p);
    }

    population = shuffle(population);
    for (int j = 0; j < generationCount; ++j)
    {
        //reprodukcja
        for(int i =0;i<lambda - populationSize;i++)
        {
            Individual child =population[i%populationSize].crossover(population[populationSize-1-i%populationSize]);

            //mutacja przez zamiane
            while(rand()%100 <  mutateBySwapRate)
            {
                child.mutateBySwap();
            }
            while(rand()%100 <  mutateByRotationRate)
            {
                child.mutateByRotation();
            }

            population.push_back(child);
        }

        //obliczanie funkcji przystosowania
        for (Individual &item : population)
        {

            item.fillRectangleWarehouse(warehouseWidth,warehouseHeight,hallwayWidth);
            item.calculateWaresArea();
        }
        //sortowanie
        sort(population.begin(), population.end(), Individual::compare);
        population.erase(population.begin() + populationSize, population.end());

    }
    return population;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    QBrush greenBrush(Qt::green);
    QBrush blueBrush(Qt::blue);
    QPen outlinePen(Qt::black);
    outlinePen.setWidth(2);



    int populationSize = 40;
        int lambda = 70;
        int generationCount = 1000;
        int mutateBySwapRate = 10;
        int mutateByRotationRate = 10;

      Individual p;
      p.initRandom(60);
      std::vector<Individual> population;

      //RectangleWarehouse rectangleWarehouse (30,40,3);
      //population= rectangleWarehouse.calculate(populationSize,lambda,generationCount,mutateBySwapRate,mutateByRotationRate,p);


      RhombusWarehouse rhombusWarehouse (3,-1,40,2,32);
      population = rhombusWarehouse.calculate(populationSize,lambda,generationCount,mutateBySwapRate,mutateByRotationRate,p);



      //p.fillTriangleWarehouse(3,-1,20,2);
       // p.fillRectangleWarehouse(20,23,2);

     // p.fillRhombusWarehouse(3,-1,25,2,12);
      std::vector<Point> points;
  //    points = p.GetVerticles();
      points = population[0].GetVerticles();

    QPolygonF polygon;

    for(std::vector<Point>::iterator i = points.begin(); i != points.end(); i++)
        polygon << QPointF(float(20*(*i).GetX()), float(20*(*i).GetY()));

    magazine = scene->addPolygon(polygon, outlinePen, blueBrush);

    std::vector<Ware> wares = population[0].GetWares();

    for(std::vector<Ware>::iterator i = wares.begin(); i != wares.end(); i++)
    { if((*i).x >= 0)
        {
            scene->addRect(QRect(20*(*i).x, 20*(*i).y, 20*(*i).getActualWidth(), 20*(*i).getActualHeight()), outlinePen, greenBrush);
        }
    }


    std::cout<<"hello";
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

    QBrush greenBrush(Qt::green);
    QBrush blueBrush(Qt::blue);
    QPen outlinePen(Qt::black);
    outlinePen.setWidth(2);

    int A = std::atof(ui->LineEditA->text().toStdString().c_str());
    int B = std::atof(ui->LineEditB->text().toStdString().c_str());
    int C = std::atof(ui->LineEditC->text().toStdString().c_str());

      Individual p;
      //p.initTestSet1x1(100);
      p.initRandom(60);
      p.fillTriangleWarehouse(A,B,C,2);

      std::vector<Point> points;
      points = p.GetVerticles();

    for(std::vector<Point>::iterator i = points.begin(); i != points.end(); i++)
        polygon << QPointF(float(20*(*i).GetX()), float(20*(*i).GetY()));

    magazine = scene->addPolygon(polygon, outlinePen, blueBrush);

    std::vector<Ware> wares = p.GetWares();

    for(std::vector<Ware>::iterator i = wares.begin(); i != wares.end(); i++)
        if((*i).x >= 0)
        {
            QGraphicsRectItem *ware;
            ware = scene->addRect(QRect(20*(*i).x, 20*(*i).y, 20*(*i).getActualWidth(), 20*(*i).getActualHeight()), outlinePen, greenBrush);
            ware->setFlag(QGraphicsItem::ItemIsMovable);
        }
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->graphicsView->scene()->clear();

    QPolygonF polygon;

    QBrush greenBrush(Qt::green);
    QBrush blueBrush(Qt::blue);
    QPen outlinePen(Qt::black);
    outlinePen.setWidth(2);

      Individual p;
      p.initRandom(200);
      p.fillRectangleWarehouse(40,50,2);

      std::vector<Point> points;
      points = p.GetVerticles();

    for(std::vector<Point>::iterator i = points.begin(); i != points.end(); i++)
        polygon << QPointF(float(5*(*i).GetX()), float(5*(*i).GetY()));

    magazine = scene->addPolygon(polygon, outlinePen, blueBrush);

    std::vector<Ware> wares = p.GetWares();

    for(std::vector<Ware>::iterator i = wares.begin(); i != wares.end(); i++)
        if((*i).x >= 0)
                scene->addRect(QRect(5*(*i).x, 5*(*i).y, 5*(*i).getActualHeight(), 5*(*i).getActualWidth()), outlinePen, greenBrush);
}

