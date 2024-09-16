#ifndef SIMULATION_H
#define SIMULATION_H
#include <WinSock2.h>
#include <windows.h>
#include <PCAN-ISO-TP_2016.h>
#include <QObject>
#include <QVector>
#include "ui_mainwindow.h"

#define STMIN_1500US 0xFF
#define OK_KO(test) (test)?"OK":"KO"
#define STATUS_OK_KO(test) OK_KO(CANTP_StatusIsOk_2016(test, PCANTP_STATUS_OK, false))
class Simulation : public QObject
{
    Q_OBJECT

public:
    explicit Simulation(QObject *parent = nullptr);
    ~Simulation();

    void initialize();
    void uninitialize();
    //void readMessages();
    QVector<cantp_handle> getAvailableDevices();
    void processSimulation(const QString& inputData,QString& outputData) ;


signals:
   // void messageReceived(const QString &message);
    void deviceListUpdated(const QString &deviceList);

private:
    cantp_handle m_handle;
    QString getDeviceCondition(cantp_handle handle);
    Ui::MainWindow *m_ui;
};

#endif // PCANUSB_H
