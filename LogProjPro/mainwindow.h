#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

struct degerler{
    double KD;
    double KP;
    double KI;
    double pA_CL;
    double pB_CL;
    double pC_CL;
    QString Soft;
    QString Par;
};

degerler motor1;
degerler motor2;
degerler motor3;
degerler motor4;
degerler version;

private slots:
    void on_pushButton_clicked();

    void on_saveButton_clicked();

    void realtimeDataSlot();

private:
    Ui::MainWindow *ui;
    QCustomPlot *mPlot;
    QPointer<QCPGraph> mGraph1;
    QPointer<QCPGraph> mGraph2;
    QTimer dataTimer;
};
#endif // MAINWINDOW_H
