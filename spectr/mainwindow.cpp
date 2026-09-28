#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <cmath>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QToolTip>
#include <QCursor>
#include <QShortcut>
#include <QWheelEvent>

class NumericTableWidgetItem : public QTableWidgetItem {
public:
    NumericTableWidgetItem(const QString &text) : QTableWidgetItem(text) {}

    bool operator<(const QTableWidgetItem &other) const override {
        bool ok1, ok2;
        double val1 = this->text().toDouble(&ok1);
        double val2 = other.text().toDouble(&ok2);

        if (ok1 && ok2) {
            return val1 < val2;
        }
        return QTableWidgetItem::operator<(other);
    }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    this->setWindowTitle("Анализ спектра АЛЬТАИР");
    this->setStyleSheet(R"(
        QStatusBar {
            color: black;
        }
        QMainWindow {
            background-color: #f4f6f9;
        }

        QPushButton {
            background-color: #009688; /* Бирюзовый цвет */
            color: white;
            border: none;
            border-radius: 5px; /* Скругленные углы */
            padding: 8px 15px;
            font-size: 13px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #00796b;
        }

        QPushButton:pressed {
            background-color: #004d40;
        }

        QTableWidget {
            background-color: white;
            alternate-background-color: #f9fbfe; /* Цвет для зебры (четных/нечетных строк) */
            gridline-color: #e0e0e0;
            selection-background-color: #b2dfdb; /* Цвет выделенной строки */
            selection-color: black;
            border: 1px solid #cfd8dc;
            border-radius: 4px;
        }

        QHeaderView::section {
            background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #4db6ac, stop:1 #009688);
            color: white;
            font-weight: bold;
            padding: 5px;
            border: 1px solid #00796b;
        }

        QTabBar::tab {
            background-color: #e0e0e0;
            color: #333333;
            padding: 8px 20px;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
            margin-right: 2px;
        }

        QTabBar::tab:selected {
            background-color: #009688;
            color: white;
            font-weight: bold;
        }

        QDoubleSpinBox, QComboBox {
            padding: 2px;
            border: 1px solid #b0bec5;
            border-radius: 3px;
            background-color: white;
        }
    )");

    ui->tablePhotometric->setAlternatingRowColors(true);
    ui->tableSpectral->setAlternatingRowColors(true);

    ui->tablePhotometric->horizontalHeader()->setSortIndicatorShown(true);
    ui->tablePhotometric->horizontalHeader()->setSectionsClickable(true);

    ui->tableSpectral->horizontalHeader()->setSortIndicatorShown(true);
    ui->tableSpectral->horizontalHeader()->setSectionsClickable(true);

    connect(ui->tablePhotometric->horizontalHeader(), &QHeaderView::sectionClicked, this, [=](int logicalIndex) {
        if (logicalIndex == 4) return;

        Qt::SortOrder order = ui->tablePhotometric->horizontalHeader()->sortIndicatorOrder();
        ui->tablePhotometric->sortByColumn(logicalIndex, order);
    });

    connect(ui->tableSpectral->horizontalHeader(), &QHeaderView::sectionClicked, this, [=](int logicalIndex) {
        Qt::SortOrder order = ui->tableSpectral->horizontalHeader()->sortIndicatorOrder();
        ui->tableSpectral->sortByColumn(logicalIndex, order);
    });

    currentState = Idle;
    recordToTable = false;

    spectrumSeries = new QSplineSeries();
    spectrumChart = new QChart();
    spectrumChart->addSeries(spectrumSeries);
    spectrumChart->legend()->hide();
    spectrumChart->setTitle("Спектр поглощения");

    axisX = new QValueAxis();
    axisX->setTitleText("Длина волны, нм");
    axisX->setLabelFormat("%d");
    spectrumChart->addAxis(axisX, Qt::AlignBottom);
    spectrumSeries->attachAxis(axisX);

    axisY = new QValueAxis();
    axisY->setTitleText("Оптическая плотность (D)");
    spectrumChart->addAxis(axisY, Qt::AlignLeft);
    spectrumSeries->attachAxis(axisY);

    QChartView *chartView = new QChartView(spectrumChart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *graphLayout = new QVBoxLayout(ui->widgetGraph);
    graphLayout->setContentsMargins(0, 0, 0, 0);
    graphLayout->addWidget(chartView);

    ui->spinStartWl->setSuffix(" нм");
    ui->spinEndWl->setSuffix(" нм");
    ui->spinStepZero->setSuffix(" нм");
    ui->spinStepMeasure->setSuffix(" нм");
    ui->spinWavelength->setSuffix(" нм");

    QShortcut *zoomResetShortcut = new QShortcut(QKeySequence("Ctrl+Z"), this);
    connect(zoomResetShortcut, &QShortcut::activated, this, [=]() {
        if (spectrumChart) {
            spectrumChart->zoomReset(); // Возвращает график к исходному масштабу
            statusBar()->showMessage("Масштаб графика сброшен", 2000);
        }
    });

    ui->btnSetZero->setEnabled(false);
    ui->btnMeasure->setEnabled(false);
    ui->btnScanBase->setEnabled(false);
    ui->btnScanSample->setEnabled(false);
    ui->btnExport->setEnabled(false);
    ui->btnClearPhoto->setEnabled(false);
    ui->btnExportPhoto->setEnabled(false);
    ui->btnUpdateDark->setEnabled(false);
    ui->btnDisconnect->setEnabled(false);
    statusBar()->showMessage("Ожидание подключения прибора...");

    chartView->setRubberBand(QChartView::RectangleRubberBand);
    chartView->viewport()->installEventFilter(this);

    connect(spectrumSeries, &QSplineSeries::hovered, this, [=](const QPointF &point, bool state) {
        if (state) {
            QString tooltipText = QString("Длина волны: %1 нм\nОпт. плотность: %2")
                                      .arg(point.x(), 0, 'f', 0)
                                      .arg(point.y(), 0, 'f', 3);
            QToolTip::showText(QCursor::pos(), tooltipText);
        } else {
            QToolTip::hideText();
        }
    });

    serialPort = new QSerialPort(this);
    serialPort->setBaudRate(QSerialPort::Baud115200);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::readData);
    connect(serialPort, &QSerialPort::errorOccurred, this, [=](QSerialPort::SerialPortError error) {
        if (error == QSerialPort::ResourceError) {
            QMessageBox::critical(this, "Обрыв связи", "Устройство было физически отключено.");
            on_btnDisconnect_clicked();
        }
    });

    portUpdateTimer = new QTimer(this);
    connect(portUpdateTimer, &QTimer::timeout, this, &MainWindow::updatePortList);
    portUpdateTimer->start(1000);

    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        ui->cbPorts->addItem(info.portName());
        qDebug() << "The port has been found:" << info.portName();
    }
    ui->tablePhotometric->setColumnCount(5);
    ui->tablePhotometric->setHorizontalHeaderLabels({"№", "Длина волны", "Коэф. пропускания %", "Оптическая плотность", "Название образца"});
    ui->tablePhotometric->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed | QAbstractItemView::AnyKeyPressed);
    ui->tablePhotometric->horizontalHeader()->setStretchLastSection(true); // Последняя колонка растянется на всю ширину

    ui->tableSpectral->setColumnCount(4);
    ui->tableSpectral->setHorizontalHeaderLabels({"№", "Длина волны", "Коэф. пропускания %", "Оптическая плотность"});
    ui->tableSpectral->horizontalHeader()->setStretchLastSection(true);

    isMeasuring = false;
    measureTimer = new QTimer(this);
    connect(measureTimer, &QTimer::timeout, this, &MainWindow::requestMeasurement);
    connect(ui->spinStepZero, &QDoubleSpinBox::valueChanged, this, [=](double val){
        ui->spinStepMeasure->setMinimum(val);
    });
    ui->spinStepMeasure->setMinimum(ui->spinStepZero->value());
}

MainWindow::~MainWindow()
{
    if (serialPort->isOpen()) {
        serialPort->close();
    }
    delete ui;


}

void MainWindow::on_btnConnect_clicked()
{
    if (serialPort->isOpen()) {
        serialPort->close();
    }

    QString selectedPort = ui->cbPorts->currentText();
    serialPort->setPortName(selectedPort);
    if (serialPort->open(QIODevice::ReadWrite)) {
        ui->tabWidget->setEnabled(true);
        ui->btnUpdateDark->setEnabled(true);
        ui->btnSetZero->setEnabled(true);
        ui->btnMeasure->setEnabled(true);
        ui->btnScanBase->setEnabled(true);
        ui->btnScanSample->setEnabled(true);
        ui->btnExport->setEnabled(true);
        ui->btnClearPhoto->setEnabled(true);
        ui->btnExportPhoto->setEnabled(true);

        statusBar()->showMessage("Подключено к порту " + selectedPort + ". Идет инициализация...");

        ui->btnDisconnect->setEnabled(true);
        ui->btnConnect->setEnabled(false);
        ui->cbPorts->setEnabled(false);

        QMessageBox::information(this, "Успех", "Успешно подключено к прибору АЛЬТАИР на порту " + selectedPort);

        serialPort->write("company\r\n");
        serialPort->waitForBytesWritten(1000);
        qDebug() << "Sent: company";

        serialPort->write("connect\r\n");
        serialPort->waitForBytesWritten(100);
        qDebug() << "Sent: connect";

        QTimer::singleShot(500, this, [=]() {
            if (serialPort && serialPort->isOpen()) {
                serialPort->write("gettype\r\n");
                serialPort->waitForBytesWritten(50);
                qDebug() << "Sent: gettype";

                serialPort->write("getsoftver\r\n");
                serialPort->waitForBytesWritten(50);
                qDebug() << "Sent: getsoftver";

                serialPort->write("gethardver\r\n");
                serialPort->waitForBytesWritten(50);
                qDebug() << "Sent: gethardver";

                QTimer::singleShot(300, this, [=]() {
                    if (serialPort && serialPort->isOpen()) {
                        serialPort->write("resetdark\r\n");
                        serialPort->waitForBytesWritten(150);
                        qDebug() << "Sent: resetdark";

                        serialPort->write("getwl\r\n");
                        serialPort->waitForBytesWritten(150);
                        qDebug() << "Sent: getwl";
                    }
                });
            }
        });

    } else {
        qDebug() << "Error port:" << serialPort->errorString();
        QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к порту: " + serialPort->errorString());
    }
}

void MainWindow::on_btnUpdateDark_clicked()
{
    if (serialPort != nullptr && serialPort->isOpen()) {
        serialPort->write("resetdark\r\n");
        qDebug() << "The resetdark command has been sent to the device.";
    } else {
        qDebug() << "Error: Port is not connected!";
    }
}

void MainWindow::readData()
{
    readBuffer.append(serialPort->readAll());

    if (readBuffer.contains('>')) {
        QString response = QString::fromLatin1(readBuffer).replace("\r", "");
        qDebug() << "The device’s response:" << response;

        if (response.contains("getwl")) {
            QStringList parts = response.split("\n", Qt::SkipEmptyParts);

            if (parts.size() >= 2) {
                bool ok;
                double wl = parts[1].trimmed().toDouble(&ok);

                if (ok) {
                    ui->spinWavelength->setValue(wl);
                    qDebug() << "The initial wavelength has been successfully set:" << wl;
                }
            }
        }

        else if (response.contains("ge 10")) {
            QStringList parts = response.split("\n", Qt::SkipEmptyParts);
            QList<double> values;

            for (const QString &part : std::as_const(parts)) {
                bool ok;
                double val = part.trimmed().toDouble(&ok);
                if (ok) values.append(val);
            }

            if (!values.isEmpty()) {
                double sum = 0;
                for (double v : values) sum += v;
                double averageI = sum / values.size();

                if (currentState == Idle) {
                    double currentWl = ui->spinWavelength->value();
                    double I0 = spectrumZeroValues.value(currentWl, -1);
                    int gain = spectrumZeroGains.value(currentWl, 1);

                    double Idark = 0;
                    if (!darkCurrents.isEmpty()) {
                        int darkIndex = gain - 1;
                        if (darkIndex >= 0 && darkIndex < darkCurrents.size()) {
                            Idark = darkCurrents[darkIndex];
                        } else {
                            Idark = darkCurrents.last();
                        }
                    }

                    if (I0 != -1) {
                        double T = ((averageI - Idark) / (I0 - Idark)) * 100.0;
                        double D = 0;
                        if (T > 0) {
                            D = -log10(T / 100.0);
                            if (D < 0) D = 0;
                        } else {
                            D = 3.0;
                        }

                        if (recordToTable) {
                            int row = ui->tablePhotometric->rowCount();
                            ui->tablePhotometric->insertRow(row);

                            NumericTableWidgetItem *itemNum = new NumericTableWidgetItem(QString::number(row + 1));
                            NumericTableWidgetItem *itemWl = new NumericTableWidgetItem(QString::number(currentWl));
                            NumericTableWidgetItem *itemT = new NumericTableWidgetItem(QString::number(T, 'f', 2));
                            NumericTableWidgetItem *itemD = new NumericTableWidgetItem(QString::number(D, 'f', 3));
                            QTableWidgetItem *itemName = new QTableWidgetItem("");

                            itemNum->setFlags(itemNum->flags() & ~Qt::ItemIsEditable);
                            itemWl->setFlags(itemWl->flags() & ~Qt::ItemIsEditable);
                            itemT->setFlags(itemT->flags() & ~Qt::ItemIsEditable);
                            itemD->setFlags(itemD->flags() & ~Qt::ItemIsEditable);
                            itemName->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);

                            ui->tablePhotometric->setItem(row, 0, itemNum);
                            ui->tablePhotometric->setItem(row, 1, itemWl);
                            ui->tablePhotometric->setItem(row, 2, itemT);
                            ui->tablePhotometric->setItem(row, 3, itemD);
                            ui->tablePhotometric->setItem(row, 4, itemName);

                            recordToTable = false;
                            qDebug() << "The entry has been made.";
                        }
                    } else {
                        if (recordToTable) {
                            QMessageBox::warning(this, "Нет базовой линии",
                                                 "Буфер нулей пуст для волны " + QString::number(currentWl) +
                                                     " нм.\nВероятно, он был очищен спектральным сканированием.\nНажмите «Зануление» перед измерением.");
                            recordToTable = false;
                        }
                    }
                }
                else if (currentState == ScanningSample) {

                    double I0 = -1;
                    int gain = 1;

                    if (!spectrumZeroValues.isEmpty()) {
                        if (spectrumZeroValues.contains(currentScanWl)) {
                            I0 = spectrumZeroValues.value(currentScanWl);
                            gain = spectrumZeroGains.value(currentScanWl, 1);
                        }
                        else {
                            auto upper = spectrumZeroValues.lowerBound(currentScanWl);
                            if (upper != spectrumZeroValues.begin() && upper != spectrumZeroValues.end()) {
                                auto lower = std::prev(upper);
                                double x1 = lower.key(), y1 = lower.value();
                                double x2 = upper.key(), y2 = upper.value();

                                I0 = y1 + (currentScanWl - x1) * (y2 - y1) / (x2 - x1);

                                gain = spectrumZeroGains.value(x1, 1);
                            }
                        }
                    }


                    if (I0 != -1) {
                        int gain_base = gain;

                        // 1. Вычисляем чистый базовый свет (без темнового тока)
                        double Idark_base = (gain_base - 1 >= 0 && gain_base - 1 < darkCurrents.size()) ? darkCurrents[gain_base - 1] : 0;
                        double I0_true = I0 - Idark_base;
                        if (I0_true <= 0.01) I0_true = 0.01; // Защита от деления на ноль

                        // 2. Считаем сигнал образца (с темновым током от ЕГО физического Gain)
                        double Idark_sample = (currentSampleGain - 1 >= 0 && currentSampleGain - 1 < darkCurrents.size()) ? darkCurrents[currentSampleGain - 1] : 0;
                        double I_signal = averageI - Idark_sample;

                        // 3. Вычисляем аппаратный множитель (разница уровней Gain)
                        double multiplier = pow(2.0, currentSampleGain - gain_base);

                        // 4. Приводим всё к одному масштабу и считаем честное T
                        double T = (I_signal / multiplier / I0_true) * 100.0;

                        // Считаем оптическую плотность
                        double D = 0;
                        if (T > 0) {
                            D = -log10(T / 100.0);
                            if (D < 0) D = 0;
                        } else {
                            D = 3.0;
                        }
                        spectrumSeries->append(currentScanWl, D);

                        if (D > axisY->max()) axisY->setMax(D + 0.3);
                        if (D < axisY->min()) axisY->setMin(D - 0.2);

                        int row = ui->tableSpectral->rowCount();
                        ui->tableSpectral->insertRow(row);

                        NumericTableWidgetItem *itemNum = new NumericTableWidgetItem(QString::number(row + 1));
                        NumericTableWidgetItem *itemWl = new NumericTableWidgetItem(QString::number(currentScanWl));
                        NumericTableWidgetItem *itemT = new NumericTableWidgetItem(QString::number(T, 'f', 2));
                        NumericTableWidgetItem *itemD = new NumericTableWidgetItem(QString::number(D, 'f', 3));

                        itemNum->setFlags(itemNum->flags() & ~Qt::ItemIsEditable);
                        itemWl->setFlags(itemWl->flags() & ~Qt::ItemIsEditable);
                        itemT->setFlags(itemT->flags() & ~Qt::ItemIsEditable);
                        itemD->setFlags(itemD->flags() & ~Qt::ItemIsEditable);

                        ui->tableSpectral->setItem(row, 0, itemNum);
                        ui->tableSpectral->setItem(row, 1, itemWl);
                        ui->tableSpectral->setItem(row, 2, itemT);
                        ui->tableSpectral->setItem(row, 3, itemD);

                        currentScanWl += scanStep;
                        nextScanStep();
                    } else {
                        qDebug() << "Scanning error: No I0 for the wave" << currentScanWl;
                        currentState = Idle;
                        ui->btnScanBase->setEnabled(true);
                        ui->btnScanSample->setEnabled(true);
                        ui->btnUpdateDark->setEnabled(true);
                        ui->tabWidget->setTabEnabled(0, true);
                    }
                }
            }
        }

        else if (response.contains("resetdark")) {
            QStringList parts = response.split("\n", Qt::SkipEmptyParts);

            darkCurrents.clear();

            QString message = "Получены значения темнового тока:\n";
            int aIndex = 1;

            for (int i = 1; i < parts.size(); ++i) {
                bool ok;
                double val = parts[i].trimmed().toDouble(&ok);

                if (ok) {
                    darkCurrents.append(val);
                    message += QString("A%1: %2\n").arg(aIndex).arg(val);
                    aIndex++;
                }
            }

            if (!darkCurrents.isEmpty()) {
                QMessageBox::information(this, "Информация", message);
                qDebug() << "Dark currents are preserved:" << darkCurrents;
            }
            statusBar()->showMessage("Прибор готов к работе", 5000);
        }
        else if (response.contains("rezero")) {
            QStringList parts = response.split("\n", Qt::SkipEmptyParts);

            if (parts.size() >= 3) {
                bool ok1, ok2;
                double val = parts[1].trimmed().toDouble(&ok1);
                int gain = parts[2].trimmed().toInt(&ok2);

                if (ok1 && ok2) {
                    double wl = (currentState == ScanningBase) ? currentScanWl : ui->spinWavelength->value();

                    if (currentState == ScanningBase) {
                        // Сохраняем базу
                        spectrumZeroValues[wl] = val;
                        spectrumZeroGains[wl] = gain;

                        currentScanWl += scanStep;
                        nextScanStep();
                    }
                    else if (currentState == ScanningSample) {
                        currentSampleGain = gain;
                        qDebug() << "Gain образца на" << currentScanWl << "нм равен" << currentSampleGain;

                        serialPort->write("ge 10\r\n");
                    }
                }
            }
        }

        readBuffer.clear();
    }
}

void MainWindow::on_btnSetZero_clicked()
{
    if (serialPort && serialPort->isOpen()) {
        QString wl = QString::number(ui->spinWavelength->value(), 'f', 1);

        serialPort->write(QString("swl %1\r\n").arg(wl).toUtf8());
        qDebug() << "Setting the wavelength:" << wl;

        QTimer::singleShot(1000, this, [=]() {
            if (serialPort->isOpen()) {
                serialPort->write("rezero\r\n");
                qDebug() << "The rezero command has been sent.";
            }
        });
    } else {
        qDebug() << "Error: Port is not connected!";
    }
}

void MainWindow::on_btnMeasure_clicked()
{
    recordToTable = true;


    if (measureTimer && !measureTimer->isActive() && serialPort && serialPort->isOpen()) {
        serialPort->write("ge 10\r\n");
    }
}

void MainWindow::requestMeasurement()
{
    if (serialPort && serialPort->isOpen()) {
        serialPort->write("ge 10\r\n");
    }
}

void MainWindow::on_btnScanBase_clicked()
{
    if (!serialPort || !serialPort->isOpen()) {
        qDebug() << "Порт не открыт!";
        return;
    }

    currentScanWl = ui->spinStartWl->value();
    endScanWl = ui->spinEndWl->value();
    scanStep = ui->spinStepZero->value();

    spectrumZeroValues.clear();
    spectrumZeroGains.clear();

    ui->btnScanBase->setEnabled(false);
    ui->btnScanSample->setEnabled(false);
    ui->btnUpdateDark->setEnabled(false);
    ui->tabWidget->setTabEnabled(0, false);

    currentState = ScanningBase;
    qDebug() << "Launching the spectral baseline scan...";
    statusBar()->showMessage(QString("Идет сканирование базовой линии (зануление) от %1 до %2 нм...")
                                 .arg(currentScanWl).arg(endScanWl));
    nextScanStep();
}

void MainWindow::on_btnScanSample_clicked()
{
    if (!serialPort || !serialPort->isOpen()) {
        qDebug() << "The port is not open!";
        return;
    }

    currentScanWl = ui->spinStartWl->value();
    endScanWl = ui->spinEndWl->value();
    scanStep = ui->spinStepMeasure->value();

    spectrumSampleValues.clear();
    ui->tableSpectral->setRowCount(0);
    spectrumSeries->clear();
    axisX->setRange(currentScanWl, endScanWl);
    axisY->setRange(0, 1.0);

    spectrumGainMultiplier = 1.0;
    lastCorrectedT = -1.0;
    currentPhysicalGain = -1;
    hasHardwareJumped = false;

    ui->btnScanBase->setEnabled(false);
    ui->btnScanSample->setEnabled(false);
    ui->btnUpdateDark->setEnabled(false);
    ui->tabWidget->setTabEnabled(0, false);

    currentState = ScanningSample;
    qDebug() << "Starting the spectral scan of the sample...";
    statusBar()->showMessage(QString("Идет сканирование спектра образца от %1 до %2 нм...")
                                 .arg(currentScanWl).arg(endScanWl));
    nextScanStep();
}

void MainWindow::nextScanStep()
{
    if (currentScanWl > endScanWl) {
        statusBar()->showMessage("Сканирование успешно завершено!", 10000);
        currentState = Idle;
        ui->btnScanBase->setEnabled(true);
        ui->btnScanSample->setEnabled(true);
        ui->btnUpdateDark->setEnabled(true);
        ui->tabWidget->setTabEnabled(0, true);
        qDebug() << "The scan is complete!";

        QMessageBox::information(this, "Готово", "Сканирование диапазона завершено!");
        return;
    }

    QString wlStr = QString::number(currentScanWl, 'f', 1);
    serialPort->write(QString("swl %1\r\n").arg(wlStr).toUtf8());
    statusBar()->showMessage(QString("Измерение на %1 нм...").arg(wlStr));

    QTimer::singleShot(1500, this, [=]() {
        if (serialPort && serialPort->isOpen()) {
           serialPort->write("rezero\r\n");
        }
    });
}

void MainWindow::on_btnExport_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Сохранить спектр",
                                                    "",
                                                    "Таблицы CSV (*.csv);;Все файлы (*)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось создать или открыть файл для записи.");
        return;
    }

    QTextStream out(&file);

    out.setEncoding(QStringConverter::Utf8);
    out << "\xEF\xBB\xBF";

    out << "Number;Wavelength;Transmittance coefficient, %;Optical density\n";

    int rowCount = ui->tableSpectral->rowCount();
    int colCount = ui->tableSpectral->columnCount();

    for (int row = 0; row < rowCount; ++row) {
        QStringList rowData;

        for (int col = 0; col < colCount; ++col) {
            QTableWidgetItem *item = ui->tableSpectral->item(row, col);
            if (item) {
                QString text = item->text();
                text.replace(".", ",");
                rowData << text;
            } else {
                rowData << "";
            }
        }

        out << rowData.join(";") << "\n";
    }

    file.close();
    QMessageBox::information(this, "Успех", "Данные успешно сохранены в файл!");
}

void MainWindow::on_btnDisconnect_clicked()
{
    if (serialPort && serialPort->isOpen()) {
        serialPort->write("quit\r\n");
        serialPort->waitForBytesWritten(200);
        serialPort->close();
        qDebug() << "The quit command has been sent. The device is disconnected.";
    }

    if (measureTimer && measureTimer->isActive()) {
        measureTimer->stop();
        isMeasuring = false;
        ui->btnMeasure->setText("Измерение");
    }

    currentState = Idle;

    ui->btnDisconnect->setEnabled(false);
    ui->btnConnect->setEnabled(true);
    ui->cbPorts->setEnabled(true);
    ui->tabWidget->setEnabled(false);
    ui->btnUpdateDark->setEnabled(false);
    statusBar()->showMessage("Прибор отключен. Ожидание подключения...");

    ui->btnScanBase->setEnabled(true);
    ui->btnScanSample->setEnabled(true);
    ui->btnUpdateDark->setEnabled(true);
    ui->tabWidget->setTabEnabled(0, true);
}

void MainWindow::on_btnClearPhoto_clicked()
{
    ui->tablePhotometric->setRowCount(0);
}

void MainWindow::on_btnExportPhoto_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Сохранить фотометрию",
                                                    "",
                                                    "Таблицы CSV (*.csv);;Все файлы (*)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка", "Не удалось создать файл для записи.");
        return;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    out << "\xEF\xBB\xBF";
    out << "Number;Wavelength;Transmittance coefficient, %;Optical density;Sample name\n";

    int rowCount = ui->tablePhotometric->rowCount();
    int colCount = ui->tablePhotometric->columnCount();

    for (int row = 0; row < rowCount; ++row) {
        QStringList rowData;
        for (int col = 0; col < colCount; ++col) {
            QTableWidgetItem *item = ui->tablePhotometric->item(row, col);
            if (item) {
                QString text = item->text();
                text.replace(".", ",");
                rowData << text;
            } else {
                rowData << "";
            }
        }
        out << rowData.join(";") << "\n";
    }

    file.close();
    QMessageBox::information(this, "Успех", "Данные фотометрии успешно сохранены!");
}

void MainWindow::updatePortList()
{
    if (serialPort && serialPort->isOpen()) {
        return;
    }

    const auto infos = QSerialPortInfo::availablePorts();
    QStringList currentPorts;
    for (const QSerialPortInfo &info : infos) {
        currentPorts << info.portName();
    }

    QStringList existingPorts;
    for (int i = 0; i < ui->cbPorts->count(); ++i) {
        existingPorts << ui->cbPorts->itemText(i);
    }

    if (currentPorts == existingPorts) {
        return;
    }

    QString currentSelection = ui->cbPorts->currentText();

    ui->cbPorts->clear();
    ui->cbPorts->addItems(currentPorts);


    int index = ui->cbPorts->findText(currentSelection);
    if (index != -1) {
        ui->cbPorts->setCurrentIndex(index);
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::Wheel) {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);

        if (spectrumChart) {
            if (wheelEvent->angleDelta().y() > 0) {
                spectrumChart->zoomIn();
            }
            else {
                spectrumChart->zoomOut();
            }
        }
        return true;
    }

    return QMainWindow::eventFilter(watched, event);
}
