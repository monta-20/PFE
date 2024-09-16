#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWebSockets/QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include<QComboBox>
#include <QByteArray>
#include <QDebug>
#include <QDomElement>
#include<QPixmap>
#include <WinSock2.h>
#include <windows.h>
#include <PCANBasic.h>
#include "simulation.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include<QRadioButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
class QLineEdit;
class QPushButton;
class MainWindow :public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QUrl &url,const QUrl &url_,bool debug=false,QWidget *parent = nullptr);
    ~MainWindow();
    Ui::MainWindow *ui;
    QMap<QString, QStringList> sidSubFuncMap;
    QDomDocument xmlDoc;
    QMap<QString, QMap<QString, QMap<QString, QString>>> udsMap; // Map to store services, function groups, sub-functions, and IDs
    QMap<QString, QString> serviceIDMap; // Map to store service IDs
    QMap<QString, QString> subFuncIDMap;
    TPCANMsg msg;
    TPCANTimestamp timestamp;
    QNetworkAccessManager *networkManager;
    QJsonObject jsonObject;
    QJsonObject dtcJsonObject;
    QString baseUri;
    TPCANStatus status;
    char strMsg[256];
    int progress ;
    QTimer *timer ;
Q_SIGNALS:
    void closed();
private Q_SLOTS:

    void onConnected1();
    void onConnected2();
    void onTextMessageReceived1(const QString& message);
    void onTextMessageReceived2(const QString& message);
    void sendData1();
    void restData();
    void SendData2();
    void processSimulation();
    void StartDownload();
    void sendAndReceive(cantp_handle handle, const std::vector<uint8_t>& data, const std::string& description,uint32_t can_id);
    int onCodeComboBoxChangedLocation(int index);
    void updateProgressBar();

    void on_btnSendOne_clicked();
    void on_btnSendTwo_clicked();
    void ChoiceLevel(int index);

    //Simulation
    //void onMessageReceived(const QString &message);

    //this code comment
    void checkAvailableDevices();
    void onCheckBoxStateChanged(int state);
    void onDeviceListUpdated(const QString &deviceList);
    //
    void handleFirstAction();
    void handleSecondAction();
    void handleThirdAction();
    void handleFourthAction();
    void handleFifthAction();

    void LengthRecieveFct(QString& msg);
    QString formatHexString(const QString &hexString);
    void checkFF(bool check);
    void checkSF(bool check);

    void RadioDSC0x01();
    void RadioDSC0x02();
    void RadioDSC0x03();
    void RadioVIN();
    void RadioBSI();
    void RadioUDSv();
    void RadioActiveDS();
    void RadioDOWNLOAD();


private:
    QWebSocket m_webSocket1 ;
    QWebSocket m_webSocket2 ;
    QUrl m_url1 ;
    QUrl m_url2 ;
    bool m_debug;
    Simulation *simul ;
    QString a ;
    QByteArray bytes ;

    uint32_t currentSeed;
    uint16_t currentSeedPart;
    uint16_t currentKey;
    cantp_handle transmitter_handle;
    cantp_handle receiver_handle;

    void handleSecurityAccessRequest(const cantp_msg &request, cantp_msg &response);
    void generateNewSeed();
    uint32_t generateKey(uint16_t seed_part);
    uint16_t usMain_eSecKey_Calcul(uint16_t FusSeed);
    bool verifyKey(uint16_t key);

    QToolButton *toggleButton ;

    void initializeCAN() ;
    bool canInitialized;

};
void checkIdLength(MainWindow *window, const QString &text);
void champSet(MainWindow *window);
void updateFormattedText(MainWindow *window, const QString &text);
void parseXML(MainWindow *window);
void onFunctionGroupChanged(MainWindow *window, int index);
void onServiceChanged(MainWindow *window, int index);
void onSubFuncChanged(MainWindow *window, int index);
void onSendData(MainWindow *window);

void setupToolBar(MainWindow *window);
void updateToolButtonState(MainWindow *window,QAction *button);
void toggleToolBar(MainWindow *window) ;

void onComboBoxChanged(int index,MainWindow *window);
void onCodeComboBoxChanged(int index,MainWindow *window);
void onNetworkReply(QNetworkReply *reply,MainWindow *window);

void loadJsonData(MainWindow *window);
void loadDtcJsonData(MainWindow *window);
void populateComboBox(MainWindow *window);
void populateCodeComboBox(MainWindow *window);
void displayAllItems(MainWindow *window);
void displayItemById(const QString &id,MainWindow *window);
void displayItemByCode(const QString &code,MainWindow *window);
void fetchData(const QString &url,MainWindow *window);

#endif // MAINWINDOW_H
