#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QRegularExpression>
#include <QMessageBox>
#include <QDir>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    ui->customPlot->addGraph(); // blue line
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(1)->setPen(QPen(Qt::red));

    QDateTime currenTime = QDateTime::currentDateTime();
    QString time = currenTime.toString("hh:mm:ss");

    QSharedPointer<QCPAxisTickerDateTime> timeTicker(new QCPAxisTickerDateTime);
    timeTicker->setDateTimeFormat(time);
    ui->customPlot->xAxis->setTicker(timeTicker);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange(-1.2, 1.2);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}

void MainWindow::realtimeDataSlot()
{
    static QElapsedTimer time;
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
      // add data to lines:
      ui->customPlot->graph(0)->addData(key, qSin(key));
      ui->customPlot->graph(1)->addData(key, qCos(key));
      // rescale value (vertical) axis to fit the current data:
      //ui->customPlot->graph(0)->rescaleValueAxis();
      //ui->customPlot->graph(1)->rescaleValueAxis(true);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->customPlot->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->customPlot->setInteractions(QCP::iRangeDrag |QCP::iRangeZoom);
    ui->customPlot->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
      ui->statusBar->showMessage(
            QString("%1 FPS, Total Data points: %2")
            .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
            .arg(ui->customPlot->graph(0)->data()->size()+ui->customPlot->graph(1)->data()->size())
            , 0);
      lastFpsKey = key;
      frameCount = 0;
    }



}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
        QFile filename = QFileDialog::getOpenFileName(this, tr("Find Files"), QDir::currentPath(),tr("JSON Files (*.json)"));
        filename.open(QIODevice::ReadWrite|QIODevice::Text);
        QString data = filename.readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();

        //Motor 1 için struct atama
        QJsonObject jsonObjectMotor= jsonObject["M1"].toObject();
        motor1.KP = jsonObjectMotor["KP"].toDouble();
        motor1.KD = jsonObjectMotor["KD"].toDouble();
        motor1.KI = jsonObjectMotor["KI"].toDouble();
        motor1.pA_CL = jsonObjectMotor["pA_CL"].toDouble();
        motor1.pB_CL = jsonObjectMotor["pB_CL"].toDouble();
        motor1.pC_CL = jsonObjectMotor["pC_CL"].toDouble();

        //Motor 2 için struct atama
        jsonObjectMotor= jsonObject["M2"].toObject();

        motor2.KP = jsonObjectMotor["KP"].toDouble();
        motor2.KD = jsonObjectMotor["KD"].toDouble();
        motor2.KI = jsonObjectMotor["KI"].toDouble();
        motor2.pA_CL = jsonObjectMotor["pA_CL"].toDouble();
        motor2.pB_CL = jsonObjectMotor["pB_CL"].toDouble();
        motor2.pC_CL = jsonObjectMotor["pC_CL"].toDouble();

        //Motor 3 için struct atama
        jsonObjectMotor= jsonObject["M3"].toObject();

        motor3.KP = jsonObjectMotor["KP"].toDouble();
        motor3.KD = jsonObjectMotor["KD"].toDouble();
        motor3.KI = jsonObjectMotor["KI"].toDouble();
        motor3.pA_CL = jsonObjectMotor["pA_CL"].toDouble();
        motor3.pB_CL = jsonObjectMotor["pB_CL"].toDouble();
        motor3.pC_CL = jsonObjectMotor["pC_CL"].toDouble();

        //Motor 4 için struct atama
        jsonObjectMotor= jsonObject["M4"].toObject();

        motor4.KP = jsonObjectMotor["KP"].toDouble();
        motor4.KD = jsonObjectMotor["KD"].toDouble();
        motor4.KI = jsonObjectMotor["KI"].toDouble();
        motor4.pA_CL = jsonObjectMotor["pA_CL"].toDouble();
        motor4.pB_CL = jsonObjectMotor["pB_CL"].toDouble();
        motor4.pC_CL = jsonObjectMotor["pC_CL"].toDouble();


        //Version'u string formunda alma
        QJsonObject jsonObjectVersion= jsonObject["Version"].toObject();
        version.Soft = jsonObjectVersion["Soft"].toString();
        version.Par = jsonObjectVersion["Par"].toString();
        //String olarak alınan par'ı parçalayıp listeye atma
        QRegularExpression rx("[.]");// match a comma or a space
        QStringList ParList = version.Par.split(rx, Qt::SkipEmptyParts);
        //Her bir liste elemanını String'den Double a çevirme
        double ParTop = ParList[0].toDouble();
        double ParMid = ParList[1].toDouble();
        double ParBot = ParList[2].toDouble();


        ui->softValLabel->setText(version.Soft);
        ui->parTopSpinBox->setValue(ParTop);
        ui->parMidSpinBox->setValue(ParMid);
        ui->parBotSpinBox->setValue(ParBot);

        ui->m1kdSpinBox->setValue(motor1.KD);
        ui->m1kpSpinBox->setValue(motor1.KP);
        ui->m1kiSpinBox->setValue(motor1.KI);
        ui->m1paclSpinBox->setValue(motor1.pA_CL);
        ui->m1pbclSpinBox->setValue(motor1.pB_CL);
        ui->m1pcclSpinBox->setValue(motor1.pC_CL);


        ui->m2kdSpinBox->setValue(motor2.KD);
        ui->m2kpSpinBox->setValue(motor2.KP);
        ui->m2kiSpinBox->setValue(motor2.KI);
        ui->m2paclSpinBox->setValue(motor2.pA_CL);
        ui->m2pbclSpinBox->setValue(motor2.pB_CL);
        ui->m2pcclSpinBox->setValue(motor2.pC_CL);

        ui->m3kdSpinBox->setValue(motor3.KD);
        ui->m3kpSpinBox->setValue(motor3.KP);
        ui->m3kiSpinBox->setValue(motor3.KI);
        ui->m3paclSpinBox->setValue(motor3.pA_CL);
        ui->m3pbclSpinBox->setValue(motor3.pB_CL);
        ui->m3pcclSpinBox->setValue(motor3.pC_CL);

        ui->m4kdSpinBox->setValue(motor4.KD);
        ui->m4kpSpinBox->setValue(motor4.KP);
        ui->m4kiSpinBox->setValue(motor4.KI);
        ui->m4paclSpinBox->setValue(motor4.pA_CL);
        ui->m4pbclSpinBox->setValue(motor4.pB_CL);
        ui->m4pcclSpinBox->setValue(motor4.pC_CL);

}


void MainWindow::on_saveButton_clicked()
{

    //Motor 1 için double değerleri al
    double motor1kpdouble = ui->m1kpSpinBox->value();
    double motor1kddouble = ui->m1kdSpinBox->value();
    double motor1kidouble = ui->m1kiSpinBox->value();
    double motor1pacldouble = ui->m1paclSpinBox->value();
    double motor1pbcldouble = ui->m1pbclSpinBox->value();
    double motor1pccldouble = ui->m1pcclSpinBox->value();

    //Motor 2 için double değerleri al
    double motor2kpdouble = ui->m2kpSpinBox->value();
    double motor2kddouble = ui->m2kdSpinBox->value();
    double motor2kidouble = ui->m2kiSpinBox->value();
    double motor2pacldouble = ui->m2paclSpinBox->value();
    double motor2pbcldouble = ui->m2pbclSpinBox->value();
    double motor2pccldouble = ui->m2pcclSpinBox->value();

    //Motor 3 için double değerleri al
    double motor3kpdouble = ui->m3kpSpinBox->value();
    double motor3kddouble = ui->m3kdSpinBox->value();
    double motor3kidouble = ui->m3kiSpinBox->value();
    double motor3pacldouble = ui->m3paclSpinBox->value();
    double motor3pbcldouble = ui->m3pbclSpinBox->value();
    double motor3pccldouble = ui->m3pcclSpinBox->value();

    //Motor 4 için double değerleri al
    double motor4kpdouble = ui->m4kpSpinBox->value();
    double motor4kddouble = ui->m4kdSpinBox->value();
    double motor4kidouble = ui->m4kiSpinBox->value();
    double motor4pacldouble = ui->m4paclSpinBox->value();
    double motor4pbcldouble = ui->m4pbclSpinBox->value();
    double motor4pccldouble = ui->m4pcclSpinBox->value();

    //Version Soft u string formatında al
    QString VersionSoft = ui->softValLabel->text();
    //Version Par için String Listesi oluştur
    QString ParTop = ui->parTopSpinBox->text();
    QString ParMid = ui->parMidSpinBox->text();
    QString ParBot = ui->parBotSpinBox->text();
    QStringList VersionParList;
    VersionParList << ParTop << ParMid << ParBot;
    //Version Par için string listesini tek bir stringe çevir
    QString VersionPar = VersionParList.join("");
    VersionPar = VersionParList.join(".");

    QString fileName("C:/Users/HP/Desktop/New Folder/"+VersionSoft+" "+VersionPar+".json");
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly))
    {

        QJsonObject doccreated;

        QJsonObject VersionObject;
        VersionObject.insert("Soft",VersionSoft);
        VersionObject.insert("Par",VersionPar);
        doccreated.insert("Version",VersionObject);

        QJsonObject Motor1Object;
        Motor1Object.insert("KP",motor1kpdouble);
        Motor1Object.insert("KD",motor1kddouble);
        Motor1Object.insert("KI",motor1kidouble);
        Motor1Object.insert("pA_CL",motor1pacldouble);
        Motor1Object.insert("pB_CL",motor1pbcldouble);
        Motor1Object.insert("pC_CL",motor1pccldouble);
        doccreated.insert("M1",Motor1Object);

        QJsonObject Motor2Object;
        Motor2Object.insert("KP",motor2kpdouble);
        Motor2Object.insert("KD",motor2kddouble);
        Motor2Object.insert("KI",motor2kidouble);
        Motor2Object.insert("pA_CL",motor2pacldouble);
        Motor2Object.insert("pB_CL",motor2pbcldouble);
        Motor2Object.insert("pC_CL",motor2pccldouble);
        doccreated.insert("M2",Motor2Object);

        QJsonObject Motor3Object;
        Motor3Object.insert("KP",motor3kpdouble);
        Motor3Object.insert("KD",motor3kddouble);
        Motor3Object.insert("KI",motor3kidouble);
        Motor3Object.insert("pA_CL",motor3pacldouble);
        Motor3Object.insert("pB_CL",motor3pbcldouble);
        Motor3Object.insert("pC_CL",motor3pccldouble);
        doccreated.insert("M3",Motor3Object);

        QJsonObject Motor4Object;
        Motor4Object.insert("KP",motor4kpdouble);
        Motor4Object.insert("KD",motor4kddouble);
        Motor4Object.insert("KI",motor4kidouble);
        Motor4Object.insert("pA_CL",motor4pacldouble);
        Motor4Object.insert("pB_CL",motor4pbcldouble);
        Motor4Object.insert("pC_CL",motor4pccldouble);
        doccreated.insert("M4",Motor4Object);

        QJsonDocument doc(doccreated);
        file.write(doc.toJson());

        QMessageBox::information(
            this,
            tr("Save"),
            tr("JSON Dosyası kaydedildi!") );
    }

    qDebug()<< "DOCUMENT SAVED !" ;

}


