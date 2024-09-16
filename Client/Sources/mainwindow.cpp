#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "edit.h"
#include "parsexml.h"
#include<QHBoxLayout>
#include<QToolBar>
MainWindow::MainWindow(const QUrl &url ,const QUrl &url_ , bool debug, QWidget *parent)
    :QMainWindow(parent),
   ui(new Ui::MainWindow),
   m_url1(url),
   m_url2(url_),
   m_debug(debug),
   simul(new Simulation(this)),
   currentSeed(0)
   , currentSeedPart(0)
   , currentKey(0)
   , transmitter_handle(PCANTP_HANDLE_USBBUS1)
   , receiver_handle(PCANTP_HANDLE_USBBUS1)


{
    ui->setupUi(this);
    QFrame frame;

    // Set the frame style to have a raised border and a sunken title bar
    frame.setFrameStyle(QFrame::Sunken | QFrame::Raised);

    // Create a QLabel widget to display a title
    QLabel titleLabel("My Window");

    // Set the title label's font to bold
    titleLabel.setFont(QFont("Arial", 16, QFont::Bold));



  //  ui->toolBar->hide();
    ui->SelectLevel->addItem("Extended Level" , 0x01);
    ui->SelectLevel->addItem("Programming Level", 0x03);
    ui->SelectLevel->addItem("Development Level", 0x05);


    connect(ui->SelectLevel,SIGNAL(currentIndexChanged(int)),this,SLOT(ChoiceLevel(int)));



    srand(static_cast<unsigned int>(time(0))); // Seed the random number generator

    connect(ui->sendSA1,&QPushButton::clicked,this,&MainWindow::on_btnSendOne_clicked);
    connect(ui->sendSA2,&QPushButton::clicked,this,&MainWindow::on_btnSendTwo_clicked);

    QPixmap picNo(":/ResPicture/img/Not_connect.png");
    QPixmap picYes(":/ResPicture/img/connected.png");
    int h = ui->labelStatus->height();
    int w = ui->labelStatus->width();
    ui->labelStatus->setPixmap(picNo.scaled(w,h,Qt::KeepAspectRatio));

    this->setWindowTitle("Diagnostic & Download Manager");
    ui->groupBox_TR->setStyleSheet("background-color:#f2f2f2");
    ui->groupBox_SIM->setStyleSheet("background-color:#f2f2f2");
    ui->groupBox_SOVD->setStyleSheet("background-color:#f2f2f2");
    //ui->toolBar->setStyleSheet("QToolBar {border-left:1px solid rgb(180,180,180);background-color:#F4D03F;} ::separator{background:#E59866 ; height:2px; };");


    if(m_debug){

        connect(&m_webSocket1,&QWebSocket::connected,this,&MainWindow::onConnected1);
        connect(&m_webSocket1,&QWebSocket::disconnected,this,&MainWindow::closed);
        connect(&m_webSocket2,&QWebSocket::connected,this,&MainWindow::onConnected2);
        connect(&m_webSocket2,&QWebSocket::disconnected,this,&MainWindow::closed);

        m_webSocket1.open(QUrl(url));
        m_webSocket2.open(QUrl(url_));

        connect(ui->idField, &QLineEdit::textChanged, [this](const QString &text) { checkIdLength(this, text); });
       // connect(ui->dataField ,&QLineEdit::textChanged, [this](const QString &text) { updateFormattedText(this, text); });
        connect(ui->dataField2 ,&QLineEdit::textChanged, [this](const QString &text) { updateFormattedText(this, text); });
        connect(ui->dataField_sim ,&QLineEdit::textChanged, [this](const QString &text) { updateFormattedText(this, text); });
        connect(ui->functionGroupComboBox, &QComboBox::currentIndexChanged, [this](int index) { onFunctionGroupChanged(this, index); });
        connect(ui->serviceComboBox, &QComboBox::currentIndexChanged, [this](int index) { onServiceChanged(this, index); });
        connect(ui->subFuncComboBox, &QComboBox::currentIndexChanged, [this](int index) { onSubFuncChanged(this, index); });
        champSet(this);
        parseXML(this);

        baseUri = "http://localhost:4000";

        loadJsonData(this);

        loadDtcJsonData(this);
        populateComboBox(this);
        populateCodeComboBox(this);

        setupToolBar(this);
        toggleToolBar(this);

        connect(ui->comboBox, &QComboBox::currentIndexChanged,[this](int index) { onComboBoxChanged(index,this); });

        connect(ui->codeComboBox, &QComboBox::currentIndexChanged,[this](int index) { onCodeComboBoxChanged(index,this); });
        //connect(networkManager, &QNetworkAccessManager::finished,
        //  [this](QNetworkReply reply){ onNetworkReply(reply); });

        connect(ui->comboBox, &QComboBox::currentIndexChanged,[this](int index) { onComboBoxChanged(index,this); });


        // connect(simul, &Simulation::messageReceived, this, &MainWindow::onMessageReceived);
        connect(ui->DownloadBtn, &QPushButton::clicked, this, &MainWindow::StartDownload);
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MainWindow::updateProgressBar);

        connect(ui->LocationSelect,QOverload<int>::of(&QComboBox::currentIndexChanged),this,&MainWindow::onCodeComboBoxChangedLocation);
        //this code comment

        connect(simul, &Simulation::deviceListUpdated, this, &MainWindow::onDeviceListUpdated);
        connect(ui->checkSRV, &QCheckBox::stateChanged, this, &MainWindow::onCheckBoxStateChanged);


        connect(ui->actionTransmit_Recieve, &QAction::triggered, this, &MainWindow::handleFirstAction);
        connect(ui->actionCANTP, &QAction::triggered, this, &MainWindow::handleSecondAction);
        connect(ui->actionSOVD, &QAction::triggered, this, &MainWindow::handleThirdAction);
        connect(ui->actionSIMULATION, &QAction::triggered, this, &MainWindow::handleFourthAction);
        connect(ui->actionMANAGEMENT_OF_SECURITY_KEYS, &QAction::triggered, this, &MainWindow::handleFifthAction);
        connect(ui->sendButton_sim, &QPushButton::clicked, this, &MainWindow::processSimulation);

        connect(ui->checkFF,&QCheckBox::toggled,this,&MainWindow::checkFF);
        connect(ui->checkSF,&QCheckBox::toggled,this,&MainWindow::checkSF);
       // simul->initialize();
       /* QTimer *timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MainWindow::checkAvailableDevices);
        timer->start(1000*20);*/
        QRect Geometry = ui->groupBox_T->geometry();
        int x = Geometry.x();
        int y= Geometry.y();
        int width = Geometry.width();
        int height = Geometry.height();
        ui->groupBox_TR->setGeometry(x,y,width,height);
        ui->groupBox_TR->show();
        ui->groupBox_SIM->hide();
        ui->groupBox_SOVD->hide();
        ui->groupBox_Simulation->hide();
        ui->groupBox_SecretKey->hide();

        ui->groupBox_10->hide();
        ui->groupBox_11->hide();
        ui->groupBox_12->hide();
        ui->label_SF->hide();
        ui->label_FF->hide();
        ui->label_CF_1->hide();
        ui->label_CF_2->hide();
        ui->All_DataField->hide();
        ui->label_All_Data->hide();
        //Before simulation
        ui->Data_all_Length->hide();
        ui->groupBox_FF->hide();
        ui->groupBox_13->hide();

        ui->groupBox_T->hide();

        connect(ui->DSC0x01,&QRadioButton::clicked,this,&MainWindow::RadioDSC0x01);
        connect(ui->DSC0x02,&QRadioButton::clicked,this,&MainWindow::RadioDSC0x02);
        connect(ui->DSC0x03,&QRadioButton::clicked,this,&MainWindow::RadioDSC0x03);
        connect(ui->VIN,&QRadioButton::clicked,this,&MainWindow::RadioVIN);
        connect(ui->BSI,&QRadioButton::clicked,this,&MainWindow::RadioBSI);
        connect(ui->UDSv,&QRadioButton::clicked,this,&MainWindow::RadioUDSv);
        connect(ui->ActiveDS,&QRadioButton::clicked,this,&MainWindow::RadioActiveDS);
        connect(ui->DOWNLOAD,&QRadioButton::clicked,this,&MainWindow::RadioDOWNLOAD);
      /*  QBoxLayout *toolLayout = new QBoxLayout(QBoxLayout::LeftToRight, this);
        toolLayout->SetFixedSize()
        //set margins to zero so the toolbar touches the widget's edges
        toolLayout->setContentsMargins(0, 0, 0, 0);


        toolLayout->addWidget(ui->toolBar);

        ui->toolBar->setOrientation(Qt::Horizontal);
        toolLayout->setDirection(QBoxLayout::LeftToRight);
      */
      //  ui->actionMANAGEMENT_OF_SECURITY_KEYS->setSeparator(true);
        ui->actionTransmit_Recieve->setCheckable(true);
        ui->actionCANTP->setCheckable(true);
        ui->actionSOVD->setCheckable(true);
        ui->actionSIMULATION->setCheckable(true);
        ui->actionMANAGEMENT_OF_SECURITY_KEYS->setCheckable(true);

    }
}
void MainWindow::initializeCAN()
{
    int res = CANTP_Initialize_2016(channelValue, baudrateValue, (cantp_hwtype)0, 0, 0);
    printf("Initialize transmitter: %s\n", STATUS_OK_KO(res));
    canInitialized = (res == 0); // Assuming 0 means success
}

void MainWindow::RadioDSC0x01(){
    cantp_handle transmitter=PCANTP_HANDLE_USBBUS1 ;
    if(CANTP_Initialize_2016(transmitter,PCANTP_BAUDRATE_250K),(cantp_hwtype)0, 0, 0){
        ui->dataField_ResSimulateur->setText("06 50 01 00 FA 01 F4 FF ");
    }

}
void MainWindow::RadioDSC0x02(){
    cantp_handle transmitter=PCANTP_HANDLE_USBBUS1 ;
    if(CANTP_Initialize_2016(transmitter,PCANTP_BAUDRATE_250K),(cantp_hwtype)0, 0, 0){
        ui->dataField_ResSimulateur->setText("06 50 02 0F A0 01 F4 FF ");
    }
}
void MainWindow::RadioDSC0x03(){
    cantp_handle transmitter=PCANTP_HANDLE_USBBUS1 ;
    if(CANTP_Initialize_2016(transmitter,PCANTP_BAUDRATE_250K),(cantp_hwtype)0, 0, 0){
         ui->dataField_ResSimulateur->setText("06 50 03 00 FA 01 F4 FF ");
    }
}
void MainWindow::RadioVIN(){
    cantp_handle transmitter=PCANTP_HANDLE_USBBUS1 ;
    if(CANTP_Initialize_2016(transmitter,PCANTP_BAUDRATE_250K),(cantp_hwtype)0, 0, 0){
         ui->dataField_ResSimulateur->setText("10 14 62 F1 90 01 02 03");
    }
}
void MainWindow::RadioBSI(){
    cantp_handle transmitter=PCANTP_HANDLE_USBBUS1 ;
    if(CANTP_Initialize_2016(transmitter,PCANTP_BAUDRATE_250K),(cantp_hwtype)0, 0, 0){
        ui->dataField_ResSimulateur->setText("10 45 62 F1 80 42 00 03");
    }
}
void MainWindow::RadioUDSv(){
    cantp_handle transmitter=PCANTP_HANDLE_USBBUS1 ;
    if(CANTP_Initialize_2016(transmitter,PCANTP_BAUDRATE_250K),(cantp_hwtype)0, 0, 0){
         ui->dataField_ResSimulateur->setText("07 62 FF 00 02 00 00 00");
    }
}
void MainWindow::RadioActiveDS(){
    cantp_handle transmitter=PCANTP_HANDLE_USBBUS1 ;
    if(CANTP_Initialize_2016(transmitter,PCANTP_BAUDRATE_250K),(cantp_hwtype)0, 0, 0){
          ui->dataField_ResSimulateur->setText("04 62 FF 86 01 FF FF FF");
    }
}
void MainWindow::RadioDOWNLOAD(){
    cantp_handle transmitter=PCANTP_HANDLE_USBBUS1 ;
    if(CANTP_Initialize_2016(transmitter,PCANTP_BAUDRATE_250K),(cantp_hwtype)0, 0, 0){
             ui->dataField_ResSimulateur->setText("01 77 FF FF FF FF FF FF");
    }
}

void MainWindow::ChoiceLevel(int index){
    uint16_t value = ui->SelectLevel->currentData().toUInt();
    QString hexString = "0"+QString::number(value,16).toUpper();
    ui->SA2_L->setText(hexString);
}
void MainWindow::on_btnSendOne_clicked()
{
     if (!canInitialized) {
        QMessageBox::warning(this, "CAN Not Initialized", "The CAN is not initialized. Please initialize the CAN before sending messages.");
        return;
    }
    cantp_msg request;
    cantp_msg response;
    bool ok;
    CANTP_MsgDataAlloc_2016(&request, PCANTP_MSGTYPE_CAN);
    CANTP_MsgDataInit_2016(&request, 0x18DA60F9, PCANTP_CAN_MSGTYPE_STANDARD, 8, NULL, NULL);
    request.msgdata.can->data[0] = ui->SA0->text().toUInt(&ok, 16);
    request.msgdata.can->data[1] = ui->SA1->text().toUInt(&ok, 16);
    request.msgdata.can->data[2] = ui->SA2_L->text().toUInt(&ok, 16);
    request.msgdata.can->data[3] = ui->SA3->text().toUInt(&ok, 16);
    request.msgdata.can->data[4] = ui->SA4->text().toUInt(&ok, 16);
    request.msgdata.can->data[5] = ui->SA5->text().toUInt(&ok, 16);
    request.msgdata.can->data[6] = ui->SA6->text().toUInt(&ok, 16);
    request.msgdata.can->data[7] = ui->SA7->text().toUInt(&ok, 16);
    CANTP_MsgDataAlloc_2016(&response, PCANTP_MSGTYPE_CAN);
    CANTP_MsgDataInit_2016(&response, 0x18DAF960, PCANTP_CAN_MSGTYPE_STANDARD, 8, NULL, NULL);
    handleSecurityAccessRequest(request, response);
    CANTP_MsgDataFree_2016(&request);
    CANTP_MsgDataFree_2016(&response);
}
void MainWindow::on_btnSendTwo_clicked()
{
     if (!canInitialized) {
        QMessageBox::warning(this, "CAN Not Initialized", "The CAN is not initialized. Please initialize the CAN before sending messages.");
        return;
    }
    cantp_msg request;
    cantp_msg response;
    bool ok;
    CANTP_MsgDataAlloc_2016(&request, PCANTP_MSGTYPE_CAN);
    CANTP_MsgDataInit_2016(&request, 0x18DA60F9, PCANTP_CAN_MSGTYPE_STANDARD, 8, NULL, NULL);
    uint16_t key = generateKey(currentSeedPart);
    request.msgdata.can->data[0] = ui->SA00->text().toUInt(&ok, 16);
    request.msgdata.can->data[1] = ui->SA01->text().toUInt(&ok, 16);
    request.msgdata.can->data[2] = ui->SA02->text().toUInt(&ok, 16);
    request.msgdata.can->data[3] = ui->SA03_K1->text().toUInt(&ok, 16);
    request.msgdata.can->data[4] = ui->SA04_K2->text().toUInt(&ok, 16);
    request.msgdata.can->data[5] = ui->SA05->text().toUInt(&ok, 16);
    request.msgdata.can->data[6] = ui->SA06->text().toUInt(&ok, 16);
    request.msgdata.can->data[7] = ui->SA07->text().toUInt(&ok, 16);
    CANTP_MsgDataAlloc_2016(&response, PCANTP_MSGTYPE_CAN);
    CANTP_MsgDataInit_2016(&response, 0x18DAF960, PCANTP_CAN_MSGTYPE_STANDARD, 8, NULL, NULL);
    handleSecurityAccessRequest(request, response);
    CANTP_MsgDataFree_2016(&request);
    CANTP_MsgDataFree_2016(&response);
}




void MainWindow :: checkFF(bool checked){
    a="2";
    ui->groupBox_FF->show();
    ui->groupBox_13->hide();
    ui->SD0->clear();
    ui->SD1->clear();
    ui->SD2->clear();
    ui->SD3->clear();
    ui->SD4->clear();
    ui->SD5->clear();
    ui->SD6->clear();
    ui->SD7->clear();
    ui->groupBox_10->hide();
    ui->groupBox_11->hide();
    ui->groupBox_12->hide();
    ui->label_SF->hide();
    ui->label_FF->hide();
    ui->label_CF_1->hide();
    ui->label_CF_2->hide();
    ui->All_DataField->hide();
    ui->label_All_Data->hide();
    ui->Data_all_Length->hide();
}
void MainWindow::sendAndReceive(cantp_handle handle, const std::vector<uint8_t>& data, const std::string& description,
                                uint32_t can_id){
    cantp_status res;
    cantp_msg tx_msg;
    cantp_msg rx_msg;
    HANDLE receive_event = CreateEvent(NULL, FALSE, FALSE, NULL);
    // Allouer le message de transmission

    res = CANTP_MsgDataAlloc_2016(&tx_msg, PCANTP_MSGTYPE_CAN);
    printf("Allocate tx message (%s): %s\n", description.c_str(), STATUS_OK_KO(res));
    // Initialiser le message

    res = CANTP_MsgDataInit_2016(&tx_msg, can_id, PCANTP_CAN_MSGTYPE_EXTENDED, data.size(), NULL, NULL);
    printf("Initialize tx message (%s): %s\n", description.c_str(), STATUS_OK_KO(res));
    if (res != PCANTP_STATUS_OK) {
    printf("Initialize tx message failed (%s)\n", description.c_str());
    return;
    }
    // Remplir le message avec les données
    tx_msg.can_info.can_id = can_id; // Adresse CAN, à adapter selon votre configuration
    tx_msg.msgdata.can->length = data.size();
    std::copy(data.begin(), data.end(), tx_msg.msgdata.can->data);// Configurer l'événement de réception
    res = CANTP_SetValue_2016(handle, PCANTP_PARAMETER_RECEIVE_EVENT, &receive_event, sizeof(receive_event));
    printf("Set receive event: %s\n", STATUS_OK_KO(res));// Envoyer le message
    res = CANTP_Write_2016(handle, &tx_msg);
    printf("Write message (%s): %s\n", description.c_str(), STATUS_OK_KO(res));// Libérer le message de transmission
    res = CANTP_MsgDataFree_2016(&tx_msg);
    printf("Free tx message (%s): %s\n", description.c_str(), STATUS_OK_KO(res));// Attendre la réponse
    int wait_result = WaitForSingleObject(receive_event, 1000); // 1000 ms timeout
    if (wait_result == WAIT_OBJECT_0) {
    res = CANTP_Read_2016(handle, &rx_msg, NULL, PCANTP_MSGTYPE_NONE);
    printf("Read response (%s): %s\n", description.c_str(), STATUS_OK_KO(res));
     if (res == PCANTP_STATUS_OK) {
     printf("Response data (%s): ", description.c_str());
         for (int i = 0; i < rx_msg.msgdata.can->length; ++i) {
      printf("%02X ", rx_msg.msgdata.any->data[i]);
      }
     printf("\n");
     }
    res = CANTP_MsgDataFree_2016(&rx_msg);
    printf("Free rx message (%s): %s\n", description.c_str(), STATUS_OK_KO(res));
    }
    else {
      printf("Wait result (%s): Timeout\n", description.c_str());
    }
    CloseHandle(receive_event);
}
int MainWindow:: onCodeComboBoxChangedLocation(int index) {
    return index;
}
void MainWindow::StartDownload()
{
     if (!canInitialized) {
        QMessageBox::warning(this, "CAN Not Initialized", "The CAN is not initialized. Please initialize the CAN before sending messages.");
        return;
    }
    progress = 0;
    ui->progressBar->setValue(progress);
    timer->start(8); // Update every 100 milliseconds


    cantp_status res ;
    cantp_handle transmitter_handle = PCANTP_HANDLE_USBBUS1 ;
    cantp_mapping mapping ;

    //Initialisation de l'interface CAN
    res = CANTP_Initialize_2016(transmitter_handle,PCANTP_BAUDRATE_250K,(cantp_hwtype)0,0,0);
    printf("Initialize;%s\n",STATUS_OK_KO(res));
    mapping.can_id =0x18DA60F9;
    mapping.can_id_flow_ctrl = 0x18DAF960;
    int a;
    int index =onCodeComboBoxChangedLocation(a);
    if(index==0){

        sendAndReceive(transmitter_handle, { 0x02 ,0x10 ,0x03 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 }, "DiagnosticSessionControl(Extended Session)", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x02 ,0x50 ,0x03 ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF }, "SessionControlResponse", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x02 ,0x85 ,0x02 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 }, "DTC Setting", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x02 ,0xC5 ,0x02 ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF }, "DTC SettingResponse", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x03 ,0x28 ,0x03 ,0x03 ,0x55 ,0x55 ,0x55 ,0x55 }, "CommunicationControl", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x03 ,0x68 ,0x03 ,0x03 ,0xFF ,0xFF ,0xFF ,0xFF }, "CommunicationControlRequest", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x02, 0x10, 0x02, 0x55, 0x55, 0x55, 0x55, 0x55 }, "DiagnosticSessionControl", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x06, 0x50, 0x02, 0x0F, 0xA0, 0x01, 0xF4, 0xFF }, "SessionControlResponse", mapping.can_id_flow_ctrl);

        // Demande de déverrouillage
        sendAndReceive(transmitter_handle, { 0x02, 0x27, 0x03, 0x55, 0x55, 0x55, 0x55, 0x55 }, "SecurityAccessRequest", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x04, 0x67, 0x03, 0xB0, 0xCA, 0xFF, 0xFF, 0xFF }, "SecurityAccessSeed", mapping.can_id_flow_ctrl);
        // Envoyer la clé de déverrouillage
        sendAndReceive(transmitter_handle, { 0x04, 0x27, 0x04, 0xB0, 0xCA, 0x55, 0x55, 0x55 }, "SecurityAccessSendKey", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x02, 0x67, 0x04, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }, "SecurityAccessResponse", mapping.can_id_flow_ctrl);
        // Demande de téléchargement
        sendAndReceive(transmitter_handle, { 0x07, 0x34 ,0x00 ,0x13 ,0x34 ,0x00 ,0x02 ,0x23 }, "RequestDownload", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x03, 0x74 ,0x10 ,0x12, 0xFF,0xFF,0xFF,0xFF }, "RequestDownloadResponse", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x10, 0x23, 0x36, 0x01, 0x12, 0xAA, 0x60, 0xDD }, "FF", mapping.can_id);
       // sendAndReceive(transmitter_handle, { 0x30, 0x00, 0x00, 0xFF,0xFF,0xFF,0xFF,0xFF }, "FC", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x21, 0x36, 0x01, 0xAD, 0xCD, 0xDA, 0xFD, 0xF9 }, "CF1", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x22, 0x36, 0x01, 0x6C, 0xCC, 0x2A, 0xA6, 0xEF }, "CF2", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x23, 0x36, 0x01, 0xCC, 0xDA, 0xED, 0x9A,0xFF }, "CF3", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x02, 0x76, 0x01,0xFF,0xFF,0xFF,0xFF,0xFF }, "Request Transfer Data1", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x24, 0x36, 0x02, 0x02, 0x14, 0xDD, 0x47, 0x69 }, "CF4", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x25, 0x36, 0x02, 0xAA, 0x14, 0xDA, 0x3D, 0x8A }, "CF5", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x26, 0x36, 0x02, 0x6E, 0x4F, 0x1A, 0x2A ,0xFF }, "CF6", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x02, 0x76, 0x02,0xFF,0xFF,0xFF,0xFF,0xFF }, "Request Transfer Data2", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x01, 0x37 , 0xFF,0xFF, 0xFF, 0xFF, 0xFF, 0xFF }, "Request Transfer Exit", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x01, 0x77 , 0xFF,0xFF, 0xFF, 0xFF, 0xFF, 0xFF }, "Response Transfer Exit", mapping.can_id_flow_ctrl);

    }
    else if (index == 1){
        sendAndReceive(transmitter_handle, { 0x02 ,0x10 ,0x03 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 }, "DiagnosticSessionControl(Extended Session)", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x02 ,0x50 ,0x03 ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF }, "SessionControlResponse", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x02 ,0x85 ,0x02 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 }, "DTC Setting", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x02 ,0xC5 ,0x02 ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF }, "DTC SettingResponse", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x03 ,0x28 ,0x03 ,0x03 ,0x55 ,0x55 ,0x55 ,0x55 }, "CommunicationControl", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x03 ,0x68 ,0x03 ,0x03 ,0xFF ,0xFF ,0xFF ,0xFF }, "CommunicationControlRequest", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x02, 0x10, 0x02, 0x55, 0x55, 0x55, 0x55, 0x55 }, "DiagnosticSessionControl", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x06, 0x50, 0x02, 0x0F, 0xA0, 0x01, 0xF4, 0xFF }, "SessionControlResponse", mapping.can_id_flow_ctrl);
        // Demande de déverrouillage
        sendAndReceive(transmitter_handle, { 0x02, 0x27, 0x03, 0x55, 0x55, 0x55, 0x55, 0x55 }, "SecurityAccessRequest", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x04, 0x67, 0x03, 0xB0, 0xCA, 0xFF, 0xFF, 0xFF }, "SecurityAccessSeed", mapping.can_id_flow_ctrl);
        // Envoyer la clé de déverrouillage
        sendAndReceive(transmitter_handle, { 0x04, 0x27, 0x04, 0xB0, 0xCA, 0x55, 0x55, 0x55 }, "SecurityAccessSendKey", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x02, 0x67, 0x04, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }, "SecurityAccessResponse", mapping.can_id_flow_ctrl);
        // Demande de téléchargement
        sendAndReceive(transmitter_handle, { 0x07, 0x34 ,0x00 ,0x13 ,0x34 ,0x55 ,0x12 ,0x23 }, "RequestDownload", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x03, 0x74 ,0x10 ,0x12, 0xFF,0xFF,0xFF,0xFF }, "RequestDownloadResponse", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x10, 0x23, 0x36, 0x01, 0x12, 0xAA, 0x60, 0xDD }, "FF", mapping.can_id);
        //sendAndReceive(transmitter_handle, { 0x30, 0x00, 0x00, 0xFF,0xFF,0xFF,0xFF,0xFF }, "FC", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x21, 0x36, 0x01, 0xAD, 0xCD, 0xDA, 0xFD, 0xF9 }, "CF1", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x22, 0x36, 0x01, 0x6C, 0xCC, 0x2A, 0xA6, 0xEF }, "CF2", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x23, 0x36, 0x01, 0xCC, 0xDA, 0xED, 0x9A,0xFF }, "CF3", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x02, 0x76, 0x01,0xFF,0xFF,0xFF,0xFF,0xFF }, "Request Transfer Data1", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x24, 0x36, 0x02, 0x02, 0x14, 0xDD, 0x47, 0x69 }, "CF4", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x25, 0x36, 0x02, 0xAA, 0x14, 0xDA, 0x3D, 0x8A }, "CF5", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x26, 0x36, 0x02, 0x6E, 0x4F, 0x1A, 0x2A ,0xFF }, "CF6", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x02, 0x76, 0x02,0xFF,0xFF,0xFF,0xFF,0xFF }, "Request Transfer Data2", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x01, 0x37 , 0xFF,0xFF, 0xFF, 0xFF, 0xFF, 0xFF }, "Request Transfer Exit", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x01, 0x77 , 0xFF,0xFF, 0xFF, 0xFF, 0xFF, 0xFF }, "Response Transfer Exit", mapping.can_id_flow_ctrl);

    }
    else if (index == 2){
        sendAndReceive(transmitter_handle, { 0x02 ,0x10 ,0x03 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 }, "DiagnosticSessionControl(Extended Session)", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x02 ,0x50 ,0x03 ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF }, "SessionControlResponse", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x02 ,0x85 ,0x02 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 }, "DTC Setting", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x02 ,0xC5 ,0x02 ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF }, "DTC SettingResponse", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x03 ,0x28 ,0x03 ,0x03 ,0x55 ,0x55 ,0x55 ,0x55 }, "CommunicationControl", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x03 ,0x68 ,0x03 ,0x03 ,0xFF ,0xFF ,0xFF ,0xFF }, "CommunicationControlRequest", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x02, 0x10, 0x02, 0x55, 0x55, 0x55, 0x55, 0x55 }, "DiagnosticSessionControl", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x06, 0x50, 0x02, 0x0F, 0xA0, 0x01, 0xF4, 0xFF }, "SessionControlResponse", mapping.can_id_flow_ctrl);
        // Demande de déverrouillage
        sendAndReceive(transmitter_handle, { 0x02, 0x27, 0x03, 0x55, 0x55, 0x55, 0x55, 0x55 }, "SecurityAccessRequest", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x04, 0x67, 0x03, 0xB0, 0xCA, 0xFF, 0xFF, 0xFF }, "SecurityAccessSeed", mapping.can_id_flow_ctrl);
        // Envoyer la clé de déverrouillage
        sendAndReceive(transmitter_handle, { 0x04, 0x27, 0x04, 0xB0, 0xCA, 0x55, 0x55, 0x55 }, "SecurityAccessSendKey", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x02, 0x67, 0x04, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }, "SecurityAccessResponse", mapping.can_id_flow_ctrl);
        // Demande de téléchargement
        sendAndReceive(transmitter_handle, { 0x07, 0x34 ,0x00 ,0x13 ,0x34 ,0x89 ,0xDD ,0x23 }, "RequestDownload", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x03, 0x74 ,0x10 ,0x12, 0xFF,0xFF,0xFF,0xFF }, "RequestDownloadResponse", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x10, 0x23, 0x36, 0x01, 0x12, 0xAA, 0x60, 0xDD }, "FF", mapping.can_id);
       // sendAndReceive(transmitter_handle, { 0x30, 0x00, 0x00, 0xFF,0xFF,0xFF,0xFF,0xFF }, "FC", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x21, 0x36, 0x01, 0xAD, 0xCD, 0xDA, 0xFD, 0xF9 }, "CF1", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x22, 0x36, 0x01, 0x6C, 0xCC, 0x2A, 0xA6, 0xEF }, "CF2", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x23, 0x36, 0x01, 0xCC, 0xDA, 0xED, 0x9A,0xFF }, "CF3", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x02, 0x76, 0x01,0xFF,0xFF,0xFF,0xFF,0xFF }, "Request Transfer Data1", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x24, 0x36, 0x02, 0x02, 0x14, 0xDD, 0x47, 0x69 }, "CF4", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x25, 0x36, 0x02, 0xAA, 0x14, 0xDA, 0x3D, 0x8A }, "CF5", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x26, 0x36, 0x02, 0x6E, 0x4F, 0x1A, 0x2A ,0xFF }, "CF6", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x02, 0x76, 0x02,0xFF,0xFF,0xFF,0xFF,0xFF }, "Request Transfer Data2", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x01, 0x37 , 0xFF,0xFF, 0xFF, 0xFF, 0xFF, 0xFF }, "Request Transfer Exit", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x01, 0x77 , 0xFF,0xFF, 0xFF, 0xFF, 0xFF, 0xFF }, "Response Transfer Exit", mapping.can_id_flow_ctrl);

    }
    else if (index == 3){
        sendAndReceive(transmitter_handle, { 0x02 ,0x10 ,0x03 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 }, "DiagnosticSessionControl(Extended Session)", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x02 ,0x50 ,0x03 ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF }, "SessionControlResponse", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x02 ,0x85 ,0x02 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 }, "DTC Setting", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x02 ,0xC5 ,0x02 ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF }, "DTC SettingResponse", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x03 ,0x28 ,0x03 ,0x03 ,0x55 ,0x55 ,0x55 ,0x55 }, "CommunicationControl", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x03 ,0x68 ,0x03 ,0x03 ,0xFF ,0xFF ,0xFF ,0xFF }, "CommunicationControlRequest", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x02, 0x10, 0x02, 0x55, 0x55, 0x55, 0x55, 0x55 }, "DiagnosticSessionControl", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x06, 0x50, 0x02, 0x0F, 0xA0, 0x01, 0xF4, 0xFF }, "SessionControlResponse", mapping.can_id_flow_ctrl);
        // Demande de déverrouillage
        sendAndReceive(transmitter_handle, { 0x02, 0x27, 0x03, 0x55, 0x55, 0x55, 0x55, 0x55 }, "SecurityAccessRequest", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x04, 0x67, 0x03, 0xB0, 0xCA, 0xFF, 0xFF, 0xFF }, "SecurityAccessSeed", mapping.can_id_flow_ctrl);
        // Envoyer la clé de déverrouillage
        sendAndReceive(transmitter_handle, { 0x04, 0x27, 0x04, 0xB0, 0xCA, 0x55, 0x55, 0x55 }, "SecurityAccessSendKey", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x02, 0x67, 0x04, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }, "SecurityAccessResponse", mapping.can_id_flow_ctrl);
        // Demande de téléchargement
        sendAndReceive(transmitter_handle, { 0x07, 0x34 ,0x00 ,0x13 ,0x36 ,0x77 ,0xBE ,0x23 }, "RequestDownload", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x03, 0x74 ,0x10 ,0x12, 0xFF,0xFF,0xFF,0xFF }, "RequestDownloadResponse", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x10, 0x23, 0x36, 0x01, 0x12, 0xAA, 0x60, 0xDD }, "FF", mapping.can_id);
       // sendAndReceive(transmitter_handle, { 0x30, 0x00, 0x00, 0xFF,0xFF,0xFF,0xFF,0xFF }, "FC", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x21, 0x36, 0x01, 0xAD, 0xCD, 0xDA, 0xFD, 0xF9 }, "CF1", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x22, 0x36, 0x01, 0x6C, 0xCC, 0x2A, 0xA6, 0xEF }, "CF2", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x23, 0x36, 0x01, 0xCC, 0xDA, 0xED, 0x9A,0xFF }, "CF3", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x02, 0x76, 0x01,0xFF,0xFF,0xFF,0xFF,0xFF }, "Request Transfer Data1", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x24, 0x36, 0x02, 0x02, 0x14, 0xDD, 0x47, 0x69 }, "CF4", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x25, 0x36, 0x02, 0xAA, 0x14, 0xDA, 0x3D, 0x8A }, "CF5", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x26, 0x36, 0x02, 0x6E, 0x4F, 0x1A, 0x2A ,0xFF }, "CF6", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x02, 0x76, 0x02,0xFF,0xFF,0xFF,0xFF,0xFF }, "Request Transfer Data2", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x01, 0x37 , 0xFF,0xFF, 0xFF, 0xFF, 0xFF, 0xFF }, "Request Transfer Exit", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x01, 0x77 , 0xFF,0xFF, 0xFF, 0xFF, 0xFF, 0xFF }, "Response Transfer Exit", mapping.can_id_flow_ctrl);

    }
    else if (index == 4){
        sendAndReceive(transmitter_handle, { 0x02 ,0x10 ,0x03 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 }, "DiagnosticSessionControl(Extended Session)", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x02 ,0x50 ,0x03 ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF }, "SessionControlResponse", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x02 ,0x85 ,0x02 ,0x55 ,0x55 ,0x55 ,0x55 ,0x55 }, "DTC Setting", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x02 ,0xC5 ,0x02 ,0xFF ,0xFF ,0xFF ,0xFF ,0xFF }, "DTC SettingResponse", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x03 ,0x28 ,0x03 ,0x03 ,0x55 ,0x55 ,0x55 ,0x55 }, "CommunicationControl", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x03 ,0x68 ,0x03 ,0x03 ,0xFF ,0xFF ,0xFF ,0xFF }, "CommunicationControlRequest", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x02, 0x10, 0x02, 0x55, 0x55, 0x55, 0x55, 0x55 }, "DiagnosticSessionControl", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x06, 0x50, 0x02, 0x0F, 0xA0, 0x01, 0xF4, 0xFF }, "SessionControlResponse", mapping.can_id_flow_ctrl);
        // Demande de déverrouillage
        sendAndReceive(transmitter_handle, { 0x02, 0x27, 0x03, 0x55, 0x55, 0x55, 0x55, 0x55 }, "SecurityAccessRequest", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x04, 0x67, 0x03, 0xB0, 0xCA, 0xFF, 0xFF, 0xFF }, "SecurityAccessSeed", mapping.can_id_flow_ctrl);
        // Envoyer la clé de déverrouillage
        sendAndReceive(transmitter_handle, { 0x04, 0x27, 0x04, 0xB0, 0xCA, 0x55, 0x55, 0x55 }, "SecurityAccessSendKey", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x02, 0x67, 0x04, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }, "SecurityAccessResponse", mapping.can_id_flow_ctrl);
        // Demande de téléchargement
        sendAndReceive(transmitter_handle, { 0x07, 0x34 ,0x00 ,0x13 ,0x39 ,0x12 ,0xBE ,0x23 }, "RequestDownload", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x03, 0x74 ,0x10 ,0x12, 0xFF,0xFF,0xFF,0xFF }, "RequestDownloadResponse", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x10, 0x23, 0x36, 0x01, 0x12, 0xAA, 0x60, 0xDD }, "FF", mapping.can_id);
       // sendAndReceive(transmitter_handle, { 0x30, 0x00, 0x00, 0xFF,0xFF,0xFF,0xFF,0xFF }, "FC", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x21, 0x36, 0x01, 0xAD, 0xCD, 0xDA, 0xFD, 0xF9 }, "CF1", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x22, 0x36, 0x01, 0x6C, 0xCC, 0x2A, 0xA6, 0xEF }, "CF2", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x23, 0x36, 0x01, 0xCC, 0xDA, 0xED, 0x9A,0xFF }, "CF3", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x02, 0x76, 0x01,0xFF,0xFF,0xFF,0xFF,0xFF }, "Request Transfer Data1", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x24, 0x36, 0x02, 0x02, 0x14, 0xDD, 0x47, 0x69 }, "CF4", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x25, 0x36, 0x02, 0xAA, 0x14, 0xDA, 0x3D, 0x8A }, "CF5", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x26, 0x36, 0x02, 0x6E, 0x4F, 0x1A, 0x2A ,0xFF }, "CF6", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x02, 0x76, 0x02,0xFF,0xFF,0xFF,0xFF,0xFF }, "Request Transfer Data2", mapping.can_id_flow_ctrl);
        sendAndReceive(transmitter_handle, { 0x01, 0x37 , 0xFF,0xFF, 0xFF, 0xFF, 0xFF, 0xFF }, "Request Transfer Exit", mapping.can_id);
        sendAndReceive(transmitter_handle, { 0x01, 0x77 , 0xFF,0xFF, 0xFF, 0xFF, 0xFF, 0xFF }, "Response Transfer Exit", mapping.can_id_flow_ctrl);

    }
    res =CANTP_Uninitialize_2016(transmitter_handle);
    printf("Uninitialize:%s\n",STATUS_OK_KO(res));

}

void MainWindow::updateProgressBar()
{
    progress += 1;
    ui->progressBar->setValue(progress);
       if (progress >= 100) {
        timer->stop();
    }
}
void MainWindow :: checkSF(bool checked){
    a = "1";
    ui->groupBox_13->show();
    ui->groupBox_FF->hide();

    ui->dataField2->clear();
    ui->receivedDataField2->clear();

    ui->Data_all_Length->hide();
    ui->groupBox_FF->hide();

}
void MainWindow::handleFirstAction() {
    QRect Geometry = ui->groupBox_T->geometry();
    int x = Geometry.x();
    int y= Geometry.y();
    int width = Geometry.width();
    int height = Geometry.height();
    ui->groupBox_TR->setGeometry(x,y,width,height);
    ui->groupBox_TR->show();
    ui->groupBox_SIM->hide();
    ui->groupBox_T->hide();
    ui->groupBox_SOVD->hide();
    ui->groupBox_Simulation->hide();
    ui->groupBox_SecretKey->hide();
    ui->actionTransmit_Recieve->setCheckable(true);
    ui->actionCANTP->setCheckable(false);
    ui->actionSOVD->setCheckable(false);
    ui->actionSIMULATION->setCheckable(false);
    ui->actionMANAGEMENT_OF_SECURITY_KEYS->setCheckable(false);
    updateToolButtonState(this,ui->actionTransmit_Recieve);
}

void MainWindow::handleSecondAction() {
    QRect Geometry = ui->groupBox_T->geometry();
    int x = Geometry.x();
    int y= Geometry.y();
    QRect Geometry1 = ui->groupBox_SIM->geometry();

    int width = Geometry1.width();
    int height = Geometry1.height();
    ui->groupBox_SIM->setGeometry(x,y,width,height);
    ui->groupBox_TR->hide();
    ui->groupBox_SOVD->hide();
    ui->groupBox_T->hide();
    ui->groupBox_Simulation->hide();
    ui->groupBox_SecretKey->hide();
    ui->groupBox_SIM->show();
    ui->groupBox_8->setCheckable(true);
    ui->actionCANTP->setCheckable(true);
    ui->actionTransmit_Recieve->setCheckable(false);
    ui->actionSOVD->setCheckable(false);
    ui->actionSIMULATION->setCheckable(false);
    ui->actionMANAGEMENT_OF_SECURITY_KEYS->setCheckable(false);
    updateToolButtonState(this,ui->actionCANTP);
}

void MainWindow::handleThirdAction(){
    QRect Geometry = ui->groupBox_T->geometry();
    int x = Geometry.x();
    int y= Geometry.y();
    QRect Geometry1 = ui->groupBox_SOVD->geometry();

    int width = Geometry1.width();
    int height = Geometry1.height();
    ui->groupBox_SOVD->setGeometry(x,y,width,height);
    ui->groupBox_TR->hide();
    ui->groupBox_SOVD->show();
    ui->groupBox_SIM->hide();
    ui->groupBox_SecretKey->hide();
    ui->groupBox_T->hide();
    ui->groupBox_Simulation->hide();
   // ui->actionSOVD->setCheckable(false);
    ui->groupBox_SOVD->setCheckable(true);
    ui->actionTransmit_Recieve->setCheckable(false);
    ui->actionCANTP->setCheckable(false);
    ui->actionSOVD->setCheckable(true);
    ui->actionSIMULATION->setCheckable(false);
    ui->actionMANAGEMENT_OF_SECURITY_KEYS->setCheckable(false);
    updateToolButtonState(this,ui->actionSOVD);
}
void MainWindow::handleFourthAction(){
    QRect Geometry = ui->groupBox_T->geometry();
    int x = Geometry.x();
    int y= Geometry.y();
    //int width = Geometry.width();
   // int height = Geometry.height();

    QRect Geometry1 = ui->groupBox_Simulation->geometry();

    int width = Geometry1.width();
    int height = Geometry1.height();

    ui->groupBox_Simulation->setGeometry(x,y,width,height);
    ui->groupBox_TR->hide();
    ui->groupBox_SOVD->hide();
    ui->groupBox_SIM->hide();
    ui->groupBox_T->hide();
    ui->groupBox_SecretKey->hide();
    ui->groupBox_Simulation->show();
    ui->groupBox_Simulation->setCheckable(true);
    ui->groupBox_SOVD->setCheckable(true);
     ui->actionSIMULATION->setCheckable(true);
    ui->actionTransmit_Recieve->setCheckable(false);
    ui->actionCANTP->setCheckable(false);
    ui->actionSOVD->setCheckable(false);
    ui->actionMANAGEMENT_OF_SECURITY_KEYS->setCheckable(false);
    updateToolButtonState(this,ui->actionSIMULATION);
}
void MainWindow::handleFifthAction(){
    QRect Geometry = ui->groupBox_T->geometry();
    int x = Geometry.x();
    int y= Geometry.y();
    QRect Geometry1 = ui->groupBox_SecretKey->geometry();

    int width = Geometry1.width();
    int height = Geometry1.height();
    ui->groupBox_SecretKey->setGeometry(x,y,width,height);
    ui->groupBox_TR->hide();
    ui->groupBox_SOVD->hide();
    ui->groupBox_SIM->hide();
    ui->groupBox_T->hide();
    ui->groupBox_Simulation->hide();
    ui->groupBox_SecretKey->show();
    ui->groupBox_SecretKey->setCheckable(true);
    ui->actionMANAGEMENT_OF_SECURITY_KEYS->setCheckable(true);
    ui->actionTransmit_Recieve->setCheckable(false);
    ui->actionCANTP->setCheckable(false);
    ui->actionSOVD->setCheckable(false);
    ui->actionSIMULATION->setCheckable(false);

    updateToolButtonState(this,ui->actionSIMULATION);
}
void MainWindow::onConnected1(){
    if (m_debug)
    qDebug() << "WebSocket connected"<<m_url1;
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::sendData1);
    connect(ui->restButton, &QPushButton::clicked, this, &MainWindow::restData);
    //connect(ui->sendButtonTwo,&QPushButton::clicked, this, &MainWindow::SendData2);
    connect(&m_webSocket1, &QWebSocket::textMessageReceived,this, &MainWindow::onTextMessageReceived1);
}
void MainWindow::onConnected2(){
    if (m_debug)
    qDebug() << "WebSocket connected"<<m_url2;
    connect(ui->sendButtonTwo,&QPushButton::clicked, this, &MainWindow::SendData2);
    connect(&m_webSocket2, &QWebSocket::textMessageReceived,this, &MainWindow::onTextMessageReceived2);
}

void MainWindow::sendData1(){

    QString baudRate =ui->baudRateField->currentText();
    QString channel = ui->channelField->currentText();
    QString id = ui->idField->text();
    QString data =ui->dataField->text();
    QString dlc = ui->dlcField->text();
    QString ID = ui->receivedIdField->text();

    QJsonObject jsonObject;
    jsonObject["Baud Rate"] = baudRate;
    jsonObject["Channel"] = channel;
    jsonObject["ID"] = id;
    jsonObject["Data"] = data;
    jsonObject["DLC"] = dlc;

    QJsonDocument jsonDoc(jsonObject);
    QByteArray jsonData = jsonDoc.toJson();
    m_webSocket1.sendTextMessage(jsonData);
}
void MainWindow::restData(){
    ui->idField->clear();
    ui->dataField->clear();
}

QString MainWindow ::formatHexString(const QString &hexString){
    QString formattedString ;
    int length = hexString.length();

    for(int i = 0; i<length;i+=2){
        if(i>0){
            formattedString.append(" ");
        }
        formattedString.append(hexString.mid(i,2));
    }
    return formattedString.toUpper();
}
void MainWindow::onTextMessageReceived1(const QString& message)
{
    QJsonDocument jsonDocc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject  receivedObject=jsonDocc.object();

    if( receivedObject.contains("Id") &&  receivedObject.contains("Dlc") && receivedObject.contains("Data") ){
        int dlc =  receivedObject["Dlc"].toInt();
        ui->receivedDlcField->setText(QString::number(dlc));
        int id =  receivedObject["Id"].toInt();
        QString result = QString::number( id, 16 );
        QString data =  receivedObject["Data"].toString();
        qDebug()<<data;
        QPixmap picNo(":/ResPicture/img/Not_connect.png");
        QPixmap picYes(":/ResPicture/img/connected.png");
        int h = ui->labelStatus->height();
        int w = ui->labelStatus->width();
        if(data.mid(2,2)=="7f" || data.mid(8,2)=="00"){
            ui->labelStatus->setPixmap(picNo.scaled(w,h,Qt::KeepAspectRatio));
        }
        else{
            ui->labelStatus->setPixmap(picYes.scaled(w,h,Qt::KeepAspectRatio));
        }
        QString IdFormatted  = formatHexString(result);
        QString DataFormatted  = formatHexString(data);

        ui->receivedIdField->setText(IdFormatted);
        ui->receivedDataField->setText(DataFormatted);
        // for UDS-Service

        QString ServiceName =  receivedObject["SRVName"].toString();
        ui->ServiceNameField->setText(ServiceName);
        QString SBFName =  receivedObject["SBFName"].toString();
        ui->SBFNameField->setText(SBFName);
        QString SBFValue =  receivedObject["SBFValue"].toString();
        ui->SBFValueField->setText(SBFValue);

        QString DIDName =  receivedObject["DIDName"].toString();
        ui->DIDNameField->setText(DIDName);

        QString DIDValue =  receivedObject["DIDValue"].toString();
        ui->DIDValueField->setText(DIDValue);
        QString DIDDescription =  receivedObject["DIDDescription"].toString().trimmed();
        ui->DIDDescriptionField->setText(DIDDescription);


        QString SBFNameRes =  receivedObject["SBFNameRes"].toString();
        ui->SBFNameResField->setText(SBFNameRes);
        QString SBFValueRes =  receivedObject["SBFValueRes"].toString();
        ui->SBFValueResField->setText(SBFValueRes);
        QString SBFDescriptionRes =  receivedObject["SBFDescriptionRes"].toString().trimmed();
        ui->SBFDescriptionResField->setText(SBFDescriptionRes);
//This code is adding .
    }
    if(ui->DIDNameField->document()->isEmpty()){
        ui->DIDNameField->setText("It does not have any DID.");    }
}

void MainWindow::SendData2() {
    QString D0 =ui->SD0->text();
    QString D1 =ui->SD1->text();
    QString D2 =ui->SD2->text();
    QString D3 =ui->SD3->text();
    QString D4 =ui->SD4->text();
    QString D5 =ui->SD5->text();
    QString D6 =ui->SD6->text();
    QString D7 =ui->SD7->text();

    QString DataL = ui->dataField2->text();


    QJsonObject JSONObject;
    JSONObject["D0"] = D0;
    JSONObject["D1"] = D1;
    JSONObject["D2"] = D2;
    JSONObject["D3"] = D3;
    JSONObject["D4"] = D4;
    JSONObject["D5"] = D5;
    JSONObject["D6"] = D6;
    JSONObject["D7"] = D7;

    JSONObject["DataL"] = DataL;
    JSONObject["A"] = a;

    QJsonDocument JSONDoc(JSONObject);
    QByteArray JSONData = JSONDoc.toJson();
    m_webSocket2.sendTextMessage(JSONData);

    //comment simulation part

    /*Simulation S;
    QVector<cantp_handle> vec=  S.getAvailableDevices();
    qDebug()<<vec[0];
    JSONObject["Device"]=vec[0];*/
}

void MainWindow::LengthRecieveFct(QString& msg){
    if(msg.length()==1){
        msg="0"+msg;
    }
}

void MainWindow :: processSimulation(){
    QString inputData = ui->dataField_sim->text();
    QString outputData ;
    //bool isChecked = ui->checkSRV_2->isChecked();
    simul->processSimulation(inputData,outputData);
    ui->receivedDataField_sim->setText(outputData);
}
void MainWindow::onTextMessageReceived2(const QString& message)
{
    QJsonDocument jsonDocc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject  receivedObject=jsonDocc.object();

    //if(receivedObject.contains("D00") && receivedObject.contains("D11") && receivedObject.contains("D22") && receivedObject.contains("D33") &&
    //receivedObject.contains("D44") && receivedObject.contains("D55") && receivedObject.contains("D66") && receivedObject.contains("D77")){

        bool ok ;  
        int data0 =  receivedObject["Da"].toInt();
        QString result0 = QString::number( data0, 16 ).toUpper();
        this->LengthRecieveFct(result0);
        int data1 =  receivedObject["D00"].toInt();
        QString result1 = QString::number( data1, 16 ).toUpper();
        uint hex = result1.toUInt(&ok , 16);
        ui->Data_all_Length->setText(QString("%1 Bytes" ).arg(hex));
        this->LengthRecieveFct(result1);
        int data2 =  receivedObject["D11"].toInt();
        QString result2 = QString::number( data2, 16 ).toUpper();
        this->LengthRecieveFct(result2);
        int data3 =  receivedObject["D22"].toInt();
        QString result3 = QString::number( data3, 16 ).toUpper();
        this->LengthRecieveFct(result3);
        int data4 =  receivedObject["D33"].toInt();
        QString result4 = QString::number( data4, 16 ).toUpper();
        this->LengthRecieveFct(result4);
        int data5 =  receivedObject["D44"].toInt();
        QString result5 = QString::number( data5, 16 ).toUpper();
        this->LengthRecieveFct(result5);
        int data6 =  receivedObject["D55"].toInt();
        QString result6 = QString::number( data6, 16 ).toUpper();
        this->LengthRecieveFct(result6);
        int data7 =  receivedObject["D66"].toInt();
        QString result7 = QString::number( data7, 16 ).toUpper();
        this->LengthRecieveFct(result7);


        ui->SD0_1->setText(result0);
        ui->SD0_2->setText(result1);
        ui->SD0_3->setText(result2);
        ui->SD0_4->setText(result3);
        ui->SD0_5->setText(result4);
        ui->SD0_6->setText(result5);
        ui->SD0_7->setText(result6);
        ui->SD0_8->setText(result7);

        int data01 =  receivedObject["Db"].toInt();
        QString result01 = QString::number( data01, 16 ).toUpper();
        this->LengthRecieveFct(result01);
        int data8 =  receivedObject["D77"].toInt();
        QString result8 = QString::number( data8, 16 ).toUpper();
        this->LengthRecieveFct(result8);
        int data9 =  receivedObject["D88"].toInt();
        QString result9 = QString::number( data9, 16 ).toUpper();
        this->LengthRecieveFct(result9);
        int data10 =  receivedObject["D99"].toInt();
        QString result10 = QString::number( data10, 16 ).toUpper();
        this->LengthRecieveFct(result10);
        int data11 =  receivedObject["D100"].toInt();
        QString result11 = QString::number( data11, 16 ).toUpper();
        this->LengthRecieveFct(result11);
        int data12 =  receivedObject["D110"].toInt();
        QString result12 = QString::number( data12, 16 ).toUpper();
        this->LengthRecieveFct(result12);
        int data13 =  receivedObject["D120"].toInt();
        QString result13 = QString::number( data13, 16 ).toUpper();
        this->LengthRecieveFct(result13);
        int data14 =  receivedObject["D130"].toInt();
        QString result14 = QString::number( data14, 16 ).toUpper();
        this->LengthRecieveFct(result14);

        ui->SD0_9->setText(result01);
        ui->SD0_10->setText(result8);
        ui->SD0_11->setText(result9);
        ui->SD0_12->setText(result10);
        ui->SD0_13->setText(result11);
        ui->SD0_14->setText(result12);
        ui->SD0_15->setText(result13);
        ui->SD0_16->setText(result14);

        int data02 =  receivedObject["Dc"].toInt();
        QString result02 = QString::number( data02, 16 ).toUpper();
        this->LengthRecieveFct(result02);
        int data15 =  receivedObject["D140"].toInt();
        QString result15 = QString::number( data15, 16 ).toUpper();
        this->LengthRecieveFct(result15);
        int data16 =  receivedObject["D150"].toInt();
        QString result16 = QString::number( data16, 16 ).toUpper();
        this->LengthRecieveFct(result16);
        int data17 =  receivedObject["D160"].toInt();
        QString result17 = QString::number( data17, 16 ).toUpper();
        this->LengthRecieveFct(result17);
        int data18 =  receivedObject["D170"].toInt();
        QString result18 = QString::number( data18, 16 ).toUpper();
        this->LengthRecieveFct(result18);
        int data19 =  receivedObject["D180"].toInt();
        QString result19 = QString::number( data19, 16 ).toUpper();
        this->LengthRecieveFct(result19);
        int data20 =  receivedObject["D190"].toInt();
        QString result20 = QString::number( data20, 16 ).toUpper();
        this->LengthRecieveFct(result20);
        int data21 =  receivedObject["D200"].toInt();
        QString result21 = QString::number( data21, 16 ).toUpper();
        this->LengthRecieveFct(result21);

        ui->SD0_25->setText(result02);
        ui->SD0_26->setText(result15);
        ui->SD0_27->setText(result16);
        ui->SD0_28->setText(result17);
        ui->SD0_29->setText(result18);
        ui->SD0_30->setText(result19);
        ui->SD0_31->setText(result20);
        ui->SD0_32->setText(result21);

        QString dataAll =  receivedObject["RESULT"].toString().toUpper();
        QString DataAllformatted = formatHexString(dataAll);
        ui->All_DataField->setText(DataAllformatted);


        QString DataReceived =  receivedObject["RESULTT"].toString().toUpper();
        QString DataReceivedformatted = formatHexString(DataReceived);
        ui->receivedDataField2->setText(DataReceivedformatted);


       //0x10142EF1900102030405060708090A0B0C0D0E0F1011
       if(data0<=8 && data0!=0){
            ui->groupBox_10->show();
            ui->groupBox_11->hide();
            ui->groupBox_12->hide();
            ui->label_SF->show();
            ui->label_FF->hide();
            ui->label_CF_1->hide();
            ui->label_CF_2->hide();
            ui->Data_all_Length->hide();
       }
       else if(data0==0){
            ui->groupBox_10->hide();
            ui->label_SF->hide();
       }
       else{
            ui->groupBox_10->show();
            ui->groupBox_11->show();
            ui->groupBox_12->show();
            ui->label_SF->hide();
            ui->label_FF->show();
            ui->label_CF_1->show();
            ui->label_CF_2->show();
            ui->All_DataField->show();
            ui->label_All_Data->show();
            ui->Data_all_Length->show();
       }
   // }
}

/*void MainWindow::onMessageReceived(const QString &message){

    ui->HandleResField->append(message);
}*/
//This code comment

void MainWindow::checkAvailableDevices()
{
    simul->getAvailableDevices();
}
void MainWindow::onCheckBoxStateChanged(int state)
{
    if (state == Qt::Checked )
    {
        checkAvailableDevices();

    }
    else{
        qDebug()<<"UnChecked Box";
    }
}
void MainWindow::onDeviceListUpdated(const QString &deviceList)
{
    ui->HandleResField->append(deviceList);
}


MainWindow::~MainWindow()
{
    delete ui;
    delete simul;
}
