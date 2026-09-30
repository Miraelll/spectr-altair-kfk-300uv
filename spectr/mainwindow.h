#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QMap>
#include <QChart>
#include <QLineSeries>
#include <QChartView>
#include <QValueAxis>
#include <QVBoxLayout>
#include <QSplineSeries>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    Ui::MainWindow *ui;
    QSerialPort *serialPort;
    QTimer *measureTimer;
    bool isMeasuring;
    bool recordToTable;
    QByteArray readBuffer;
    QList<double> darkCurrents;
    QMap<double, double> spectrumZeroValues;
    QMap<double, int> spectrumZeroGains;
    QMap<double, double> spectrumSampleValues;
    QTimer *portUpdateTimer;
    double spectrumGainMultiplier = 1.0;
    double lastCorrectedT = -1.0;
    int currentPhysicalGain = -1;
    bool hasHardwareJumped = false;
    int currentSampleGain = 1;


    enum ScanState {
        Idle,           // Режим ожидания
        ScanningBase,   // Прогон базовой линии (запись I0)
        ScanningSample  // Прогон образца (запись I)
    };
    ScanState currentState; // Текущее состояние программы

    double currentScanWl; // Волна, на которой мы находимся прямо сейчас в цикле
    double endScanWl;     // Конечная волна (чтобы знать, когда остановиться)
    double scanStep;      // Шаг изменения длины волны

    QChart *spectrumChart;
    QSplineSeries *spectrumSeries;
    QValueAxis *axisX;
    QValueAxis *axisY;



private slots:
    void readData();
    void on_btnConnect_clicked();
    void on_btnUpdateDark_clicked();
    void on_btnSetZero_clicked();
    void on_btnMeasure_clicked();
    void requestMeasurement();
    void on_btnScanBase_clicked();
    void on_btnScanSample_clicked();
    void nextScanStep();
    void on_btnExport_clicked();
    void on_btnDisconnect_clicked();
    void on_btnClearPhoto_clicked();
    void on_btnExportPhoto_clicked();
    void updatePortList();
    void on_btnCancelScan_clicked();
};

#endif // MAINWINDOW_H
