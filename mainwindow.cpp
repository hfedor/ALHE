#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <iostream>
#include <time.h>
#include <cstdlib>
#include <algorithm>
#include <string>

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
    scene->setBackgroundBrush(QColor(26, 26, 26, 255));
    ui->graphicsView->setScene(scene);

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
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->graphicsView->scene()->clear();

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

void MainWindow::on_pushButton_3_clicked()
{
    double count = std::atof(ui->waresCountForm->text().toStdString().c_str());
    double width = std::atof(ui->wareWidthForm->text().toStdString().c_str());
    double height = std::atof(ui->wareHeightForm->text().toStdString().c_str());

    if(count <1 || width <1 || height <1) return;

    for (int i = 0 ;i < count ; i++)
    {
        Ware ware(waresFromUser.size(),width,height);

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

    scene->addText("Przetwarzanie...", QFont("Arial", 20) );

    QPolygonF polygon;

    QBrush blueBrush(QColor(84, 84, 84, 255));
    QPen outlinePen(Qt::black);
    outlinePen.setWidth(2);

    double rectangleWarehouseWidthForm = std::atof(ui-> rectangleWarehouseWidthForm->text().toStdString().c_str());
    double rectangleWarehouseHeightForm = std::atof(ui-> rectangleWarehouseHeightForm->text().toStdString().c_str());
    double triangleWarehouseLeftLegAForm = std::atof(ui-> triangleWarehouseLeftLegAForm->text().toStdString().c_str());
    double triangleWarehouseRightLegAForm = std::atof(ui-> triangleWarehouseRightLegAForm->text().toStdString().c_str());
    double triangleWarehouseRightLegBForm = std::atof(ui-> triangleWarehouseRightLegBForm->text().toStdString().c_str());
    double trapezeWarehouseLeftLegAForm = std::atof(ui-> rhombusWarehouseLeftLegAForm->text().toStdString().c_str());
    double trapezeWarehouseRightLegAForm = std::atof(ui-> rhombusWarehouseRightLegAForm->text().toStdString().c_str());
    double trapezeWarehouseRightLegBForm = std::atof(ui-> rhombusWarehouseRightLegBForm->text().toStdString().c_str());
    double trapezeWarehouseheightForm = std::atof(ui-> rhombusWarehouseheightForm->text().toStdString().c_str());
    double warehouseHallwayWidthForm = std::atof(ui-> warehouseHallwayWidthForm->text().toStdString().c_str());
    int generationCountForm = std::atoi(ui-> generationCountForm->text().toStdString().c_str());
    int populationSizeForm = std::atoi(ui-> populationSizeForm->text().toStdString().c_str());
    int lambdaForm = std::atoi(ui-> lambdaForm->text().toStdString().c_str());
    int mutateBySwapRateForm = std::atoi(ui-> mutateBySwapRateForm->text().toStdString().c_str());
    int mutateByRotationRateForm = std::atoi(ui-> mutateByRotationRateForm->text().toStdString().c_str());
    double wareHeightForm = std::atof(ui-> wareHeightForm->text().toStdString().c_str());
    int waresCountForm = std::atoi(ui-> waresCountForm->text().toStdString().c_str());
    double wareWidthForm = std::atof(ui-> wareWidthForm->text().toStdString().c_str());
    int randomWaresSetCount = std::atoi(ui-> randomWaresSetCount->text().toStdString().c_str());
    bool isRandomWaresSetForm = ui->isRandomWaresSetForm->isChecked();

    /*
     * rectangle - 0
     * triangle -1;
     * trapeze -2
     *
    */
    int selectedWarehouseShape = ui->tabWidget->currentIndex();
    int algorithmSelectionIndex  = ui->algorithmSelectionComboBox->currentIndex();

    if(selectedWarehouseShape == 0)
    {
        RectangleWarehouse rectangleWarehouse(
                    rectangleWarehouseWidthForm,
                    rectangleWarehouseHeightForm,
                    warehouseHallwayWidthForm);

        if(isRandomWaresSetForm && randomWaresSetCount>0)
            rectangleWarehouse.initRandom(randomWaresSetCount);
        else if(waresFromUser.size()>0)
        {
            rectangleWarehouse.setWares(waresFromUser);
            rectangleWarehouse.randomize();
        }
        else
            return;

        typedef  Population<RectangleWarehouse> RectangleWarehousePopulation;

        RectangleWarehousePopulation population(populationSizeForm,lambdaForm,generationCountForm,mutateBySwapRateForm,mutateByRotationRateForm,rectangleWarehouse);

        population.calculateMiPlusLambda();

        std::vector<Point> points;
        points = rectangleWarehouse.GetVerticles();

        QPolygonF polygon;

        for(std::vector<Point>::iterator i = points.begin(); i != points.end(); i++)
        polygon << QPointF(float(20*(*i).GetX()), float(20*(*i).GetY()));

        magazine = scene->addPolygon(polygon, outlinePen, blueBrush);

        std::vector<Ware> wares = population.GetBest().GetWares();
        QPolygonF hallway;

        for(auto i : population.GetBest().GetHallwayVerticles())
            hallway << QPointF(float(20*(i.GetX())), float(20*(i.GetY())));

        scene->addPolygon(hallway, outlinePen, QBrush(QColor(54, 54, 54, 255)));

        for(std::vector<Ware>::iterator i = wares.begin(); i != wares.end(); i++)
            if((*i).x >= 0)
            {
                QBrush wareBrush(RandWareColor());
                QGraphicsRectItem *ware;
                ware = scene->addRect(QRect(20*(*i).x, 20*(*i).y, 20*(*i).getActualWidth(), 20*(*i).getActualHeight()), outlinePen, wareBrush);

                ware->setFlag(QGraphicsItem::ItemIsMovable);
            }

        // wypisywanie wyniku
        double warehouseCoverage = population.GetBest().GetWaresArea()  / population.GetBest().GetWarehouseArea() * 100.0;
        double hallwayCoverage = population.GetBest().GetHallwayArea() / population.GetBest().GetWarehouseArea() * 100.0;
        std::string s = to_string(warehouseCoverage);
        std::string s2 = to_string(hallwayCoverage);
        ui->resultLabel->setText(QString::fromStdString("Wynik:  "+ s + "% zajętej powierzchni\nPowierzchnia korytarza: " + s2 + "%"));

    }
    else if(selectedWarehouseShape == 1)
    {
        TriangleWarehouse triangleWarehouse(
                    triangleWarehouseLeftLegAForm,
                    triangleWarehouseRightLegAForm,
                    triangleWarehouseRightLegBForm,
                    warehouseHallwayWidthForm);

        if(isRandomWaresSetForm && randomWaresSetCount>0)
            triangleWarehouse.initRandom(randomWaresSetCount);
        else if(waresFromUser.size()>0)
        {
            triangleWarehouse.setWares(waresFromUser);
            triangleWarehouse.randomize();
        }
        else
            return;

        typedef  Population<TriangleWarehouse> TriangleWarehousePopulation;

        TriangleWarehousePopulation population(populationSizeForm,lambdaForm,generationCountForm,mutateBySwapRateForm,mutateByRotationRateForm,triangleWarehouse);

        population.calculateMiPlusLambda();

        std::vector<Point> points;
        points = triangleWarehouse.GetVerticles();

        QPolygonF polygon;

        for(std::vector<Point>::iterator i = points.begin(); i != points.end(); i++)
        polygon << QPointF(float(20*(*i).GetX()), float(20*(*i).GetY()));

        ui->graphicsView->scene()->clear();

        magazine = scene->addPolygon(polygon, outlinePen, blueBrush);

        std::vector<Ware> wares = population.GetBest().GetWares();

        QPolygonF hallway;

        for(auto i : population.GetBest().GetHallwayVerticles())
            hallway << QPointF(float(20*(i.GetX())), float(20*(i.GetY())));

        scene->addPolygon(hallway, outlinePen, QBrush(QColor(54, 54, 54, 255)));

        for(std::vector<Ware>::iterator i = wares.begin(); i != wares.end(); i++)
            if((*i).x >= 0)
            {
                QBrush wareBrush(RandWareColor());
                QGraphicsRectItem *ware;
                ware = scene->addRect(QRect(20*(*i).x, 20*(*i).y, 20*(*i).getActualWidth(), 20*(*i).getActualHeight()), outlinePen, wareBrush);

                ware->setFlag(QGraphicsItem::ItemIsMovable);
            }

        // wypisywanie wyniku
        double warehouseCoverage = population.GetBest().GetWaresArea()  / population.GetBest().GetWarehouseArea() * 100.0;
        double hallwayCoverage = population.GetBest().GetHallwayArea() / population.GetBest().GetWarehouseArea() * 100.0;
        std::string s = to_string(warehouseCoverage);
        std::string s2 = to_string(hallwayCoverage);
        ui->resultLabel->setText(QString::fromStdString("Wynik:  "+ s + "% zajętej powierzchni\nPowierzchnia korytarza: " + s2 + "%"));

    }
    else if(selectedWarehouseShape == 2)
    {
        TrapezeWarehouse trapezeWarehouse(
                    trapezeWarehouseLeftLegAForm,
                    trapezeWarehouseRightLegAForm,
                    trapezeWarehouseRightLegBForm,
                    warehouseHallwayWidthForm,
                    trapezeWarehouseheightForm);

        if(isRandomWaresSetForm && randomWaresSetCount>0)
            trapezeWarehouse.initRandom(randomWaresSetCount);
        else if(waresFromUser.size()>0)
        {
            trapezeWarehouse.setWares(waresFromUser);
            trapezeWarehouse.randomize();
        }
        else
            return;

        typedef  Population<TrapezeWarehouse> TrapezeWarehousePopulation;

        TrapezeWarehousePopulation population(populationSizeForm,lambdaForm,generationCountForm,mutateBySwapRateForm,mutateByRotationRateForm,trapezeWarehouse);

        population.calculateMiPlusLambda();

        std::vector<Point> points;
        points = trapezeWarehouse.GetVerticles();

        QPolygonF polygon;

        for(std::vector<Point>::iterator i = points.begin(); i != points.end(); i++)
            polygon << QPointF(float(20*(*i).GetX()), float(20*(*i).GetY()));

        magazine = scene->addPolygon(polygon, outlinePen, blueBrush);

        std::vector<Ware> wares = population.GetBest().GetWares();

        QPolygonF hallway;

        for(auto i : population.GetBest().GetHallwayVerticles())
            hallway << QPointF(float(20*(i.GetX())), float(20*(i.GetY())));

        scene->addPolygon(hallway, outlinePen, QBrush(QColor(54, 54, 54, 255)));

        for(std::vector<Ware>::iterator i = wares.begin(); i != wares.end(); i++)
            if((*i).x >= 0)
            {
                QBrush wareBrush(RandWareColor());
                QGraphicsRectItem *ware;
                ware = scene->addRect(QRect(20*(*i).x, 20*(*i).y, 20*(*i).getActualWidth(), 20*(*i).getActualHeight()), outlinePen, wareBrush);

                ware->setFlag(QGraphicsItem::ItemIsMovable);
            }

        // wypisywanie wyniku
        double warehouseCoverage = population.GetBest().GetWaresArea()  / population.GetBest().GetWarehouseArea() * 100.0;
        double hallwayCoverage = population.GetBest().GetHallwayArea() / population.GetBest().GetWarehouseArea() * 100.0;
        std::string s = to_string(warehouseCoverage);
        std::string s2 = to_string(hallwayCoverage);
        ui->resultLabel->setText(QString::fromStdString("Wynik:  "+ s + "% zajętej powierzchni\nPowierzchnia korytarza: " + s2 + "%"));
    }
}

