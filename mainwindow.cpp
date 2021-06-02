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

    ui->tableWidget->setColumnCount(3);

    QTableWidgetItem *header1 = new QTableWidgetItem();
    header1->setText("Liczba");
     ui->tableWidget->setHorizontalHeaderItem(0,header1);

     QTableWidgetItem *header2 = new QTableWidgetItem();
     header2->setText("Wysokość");
      ui->tableWidget->setHorizontalHeaderItem(1,header2);

      QTableWidgetItem *header3 = new QTableWidgetItem();
      header3->setText("Szerokość");
       ui->tableWidget->setHorizontalHeaderItem(2,header3);



    int populationSize = 40;
        int lambda = 70;
        int generationCount = 1000;
        int mutateBySwapRate = 10;
        int mutateByRotationRate = 10;

      Individual p;
      p.initRandom(120);
      std::vector<Individual> population;

     // RectangleWarehouse rectangleWarehouse (30,40,3);
     // population= rectangleWarehouse.calculateMiLambda(populationSize,lambda,generationCount,mutateBySwapRate,mutateByRotationRate,p);


    // RhombusWarehouse rhombusWarehouse (3,-1,40,4,22);
     // population = rhombusWarehouse.calculateMiPlusLambda(populationSize,lambda,generationCount,mutateBySwapRate,mutateByRotationRate,p);

        TriangleWarehouse triangleWarehouse (3,-1,35,2);
        population = triangleWarehouse.calculateMiPlusLambda(populationSize,lambda,generationCount,mutateBySwapRate,mutateByRotationRate,p);




      //p.fillTriangleWarehouse(3,-1,35,2);
       // p.fillRectangleWarehouse(20,23,2);

     // p.fillRhombusWarehouse(3,-1,40,2,28);
      std::vector<Point> points;
      //population.push_back(p);
  //    points = p.GetVerticles();
      points = population[0].GetVerticles();

    QPolygonF polygon;

    for(std::vector<Point>::iterator i = points.begin(); i != points.end(); i++)
        polygon << QPointF(float(20*(*i).GetX()), float(-20*(*i).GetY()));

    magazine = scene->addPolygon(polygon, outlinePen, blueBrush);

    std::vector<Ware> wares = population[0].GetWares();

    for(std::vector<Ware>::iterator i = wares.begin(); i != wares.end(); i++)
    { if((*i).x >= 0)
        {
            scene->addRect(QRect(20*(*i).x, -20*(*i).y, 20*(*i).getActualWidth(), -20*(*i).getActualHeight()), outlinePen, greenBrush);
            std::cout<<"{id:"<< (*i).id<<" x:"<<(*i).x<<" y:"<<(*i).y<<" width:"<<(*i).getActualWidth()<<" height:"<<(*i).getActualHeight()<<"}"<<std::endl;

        }
    }
   /* for(int i =0; i<wares.size();i++)
    {
        std::cout<<wares[i].getActualHeight()<<",";
    }
    std::cout<<std::endl;
    for(int i =0; i<wares.size();i++)
    {
        std::cout<<wares[i].getActualWidth()<<",";
    }
    std::cout<<std::endl;

    std::cout<<"hello"<<std::endl;
    //while()*/
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

   /* int A = std::atof(ui->LineEdit->text().toStdString().c_str());
    int B = std::atof(ui->LineEdit->text().toStdString().c_str());
    int C = std::atof(ui->LineEdit->text().toStdString().c_str());

      Individual p;
      p.initTestSet1x1(100);
      p.fillTriangleWarehouse(A,B,C,2);

      std::vector<Point> points;
      points = p.GetVerticles();

    for(std::vector<Point>::iterator i = points.begin(); i != points.end(); i++)
        polygon << QPointF(float(20*(*i).GetX()), float(20*(*i).GetY()));

    magazine = scene->addPolygon(polygon, outlinePen, blueBrush);

    std::vector<Ware> wares = p.GetWares();

    for(std::vector<Ware>::iterator i = wares.begin(); i != wares.end(); i++)
        if((*i).x >= 0)
                scene->addRect(QRect(20*(*i).x, 20*(*i).y, 20*(*i).height, 20*(*i).width), outlinePen, greenBrush);*/
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
                scene->addRect(QRect(5*(*i).x, 5*(*i).y, 5*(*i).height, 5*(*i).width), outlinePen, greenBrush);
}








void MainWindow::on_pushButton_3_clicked()
{
    int count = std::atof(ui->waresCountForm->text().toStdString().c_str());
    int width = std::atof(ui->wareWidthForm->text().toStdString().c_str());
    int height = std::atof(ui->wareHeightForm->text().toStdString().c_str());

    if(count <1 || width <1 || height <1) return;

    for (int i =0;i< count ; i++)
    {
        Ware ware(waresFromUser.size(),(double)width,(double)height);

        waresFromUser.push_back(ware);
    }
    this->ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    QTableWidgetItem *newItem = new QTableWidgetItem(QString::number(count),0);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, newItem);
    QTableWidgetItem *newItem2 = new QTableWidgetItem(QString::number(height),0);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1, newItem2);
    QTableWidgetItem *newItem3 = new QTableWidgetItem(QString::number(width),0);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 2, newItem3);
}


void MainWindow::on_generateButton_clicked()
{
    ui->graphicsView->scene()->clear();

    QPolygonF polygon;

    QBrush greenBrush(Qt::green);
    QBrush blueBrush(Qt::blue);
    QPen outlinePen(Qt::black);
    outlinePen.setWidth(2);

    int rectangleWarehouseWidthForm = std::atof(ui-> rectangleWarehouseWidthForm->text().toStdString().c_str());
    int rectangleWarehouseHeightForm = std::atof(ui-> rectangleWarehouseHeightForm->text().toStdString().c_str());
    int triangleWarehouseLeftLegAForm = std::atof(ui-> triangleWarehouseLeftLegAForm->text().toStdString().c_str());
    int triangleWarehouseRightLegAForm = std::atof(ui-> triangleWarehouseRightLegAForm->text().toStdString().c_str());
    int triangleWarehouseRightLegBForm = std::atof(ui-> triangleWarehouseRightLegBForm->text().toStdString().c_str());
    int rhombusWarehouseLeftLegAForm = std::atof(ui-> rhombusWarehouseLeftLegAForm->text().toStdString().c_str());
    int rhombusWarehouseRightLegAForm = std::atof(ui-> rhombusWarehouseRightLegAForm->text().toStdString().c_str());
    int rhombusWarehouseRightLegBForm = std::atof(ui-> rhombusWarehouseRightLegBForm->text().toStdString().c_str());
    int rhombusWarehouseheightForm = std::atof(ui-> rhombusWarehouseheightForm->text().toStdString().c_str());
    int warehouseHallwayWidthForm = std::atof(ui-> warehouseHallwayWidthForm->text().toStdString().c_str());
    int generationCountForm = std::atof(ui-> generationCountForm->text().toStdString().c_str());
    int populationSizeForm = std::atof(ui-> populationSizeForm->text().toStdString().c_str());
    int lambdaForm = std::atof(ui-> lambdaForm->text().toStdString().c_str());
    int mutateBySwapRateForm = std::atof(ui-> mutateBySwapRateForm->text().toStdString().c_str());
    int mutateByRotationRateForm = std::atof(ui-> mutateByRotationRateForm->text().toStdString().c_str());
    int wareHeightForm = std::atof(ui-> wareHeightForm->text().toStdString().c_str());
    int waresCountForm = std::atof(ui-> waresCountForm->text().toStdString().c_str());
    int wareWidthForm = std::atof(ui-> wareWidthForm->text().toStdString().c_str());
    bool isRandomWaresSetForm = ui->isRandomWaresSetForm->isChecked();




}

