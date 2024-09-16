#include "simulation.h"
#include <QDebug>

Simulation::Simulation(QObject *parent) : QObject(parent), m_handle(PCANTP_HANDLE_USBBUS1)
{

}

Simulation::~Simulation()
{
    uninitialize();
}

void Simulation::initialize()
{
    cantp_status status = CANTP_Initialize_2016(m_handle, PCANTP_BAUDRATE_250K);
    if (status != PCAN_ERROR_OK)
    {
        qDebug() << "Erreur d'initialisation:" << status;


    }
    else{
        qDebug()<<"PCAN-USB"<<status<< "was initialized";
    }


}

void Simulation::uninitialize()
{
    CANTP_Uninitialize_2016(m_handle);
}

/*void Simulation::readMessages()
{
    TPCANMsg msg;
    TPCANTimestamp timestamp;

    while (true)
    {
        TPCANStatus status = CAN_Read(m_handle, &msg, NULL);
        if (status != PCAN_ERROR_OK)
        {
            if (status != PCAN_ERROR_QRCVEMPTY)
            {
                qDebug() << "Erreur de lecture:" << status;
            }
            break;


            QString message = QString("ID: %1 LEN: %2 DATA: ")
                                  .arg(msg.ID, 0, 16)
                                  .arg(msg.LEN);
            for (int i = 0; i < msg.LEN; ++i)
            {
                message.append(QString("%1 ").arg(msg.DATA[i], 2, 16, QChar('0')));
            }
            emit messageReceived(message);
        }
    }
}*/
QString Simulation::getDeviceCondition(cantp_handle handle)
{
    DWORD condition;
    cantp_status status = CANTP_GetValue_2016(handle, PCANTP_PARAMETER_CHANNEL_CONDITION, &condition, sizeof(condition));
    if (status != PCAN_ERROR_OK)
    {
        return QString("Erreur: %1").arg(status);
    }

    switch (condition)
    {
    case PCANTP_CHANNEL_AVAILABLE:
        return "AVAILABLE";
    case PCANTP_CHANNEL_OCCUPIED:
        return "OCCUPIED";
    case PCANTP_CHANNEL_UNAVAILABLE:
        return "UNAVAILABLE"  ;
    case PCAN_CHANNEL_PCANVIEW:
        return "PCAN_VIEW";
    default:
        return "UNKOWN STATUS";
    }
}

QVector<cantp_handle> Simulation::getAvailableDevices()
{
    QVector<cantp_handle> devices;
    QStringList deviceInfoList;
    cantp_status status;
    for (int i = 0; i < 8; i++) {
        cantp_handle handle = (cantp_handle)(PCANTP_HANDLE_USBBUS1 + i);
        DWORD condition;
        status = CANTP_GetValue_2016(handle, PCANTP_PARAMETER_CHANNEL_CONDITION, &condition, sizeof(unsigned int));
        if (true)/*(condition & PCAN_CHANNEL_AVAILABLE) == PCAN_CHANNEL_AVAILABLE*/
        {
            //devices.append(handle);
            QString conditionStr = getDeviceCondition(handle);
            deviceInfoList.append(QString("PCAN Handle: %1 - %2").arg(handle%80).arg(conditionStr));
            if(conditionStr=="PCAN_VIEW") devices.append(handle);

        }   else{
                 qDebug()<<"No ERROR HAPPEN";
            }
    }
    qDebug()<<devices[0];
    emit deviceListUpdated(deviceInfoList.join("\n"));
    return devices;
}

void Simulation :: processSimulation(const QString& inputData,QString& outputData){

   // QVector<cantp_handle> device = getAvailableDevices();
    cantp_status res ;
    char buffer[500];
    cantp_msg tx_msg;
    cantp_msg rx_msg;
    int can_id;
    uint32_t STmin;
    cantp_mapping mapping;
    HANDLE receive_event;
    int wait_result;
    BOOL res_b;
    uint64_t null_handle = 0;
    cantp_handle transmitter_handle;
    cantp_handle receiver_handle;

    transmitter_handle =PCANTP_HANDLE_USBBUS1;
    receiver_handle = PCANTP_HANDLE_USBBUS1;
    memset(&rx_msg , 0 , sizeof(cantp_msg));
    memset(&tx_msg, 0 , sizeof(cantp_msg));

    // Initialize channels: CAN1.0 - 250Kbit/s
    res = CANTP_Initialize_2016(transmitter_handle, PCANTP_BAUDRATE_250K, (cantp_hwtype)0, 0, 0);
    printf("Initialize transmitter: %s\n", STATUS_OK_KO(res));
    res = CANTP_Initialize_2016(receiver_handle, PCANTP_BAUDRATE_250K, (cantp_hwtype)0, 0, 0);
    printf("Initialize receiver: %s\n", STATUS_OK_KO(res));

    // Create a receive event on receiver
    receive_event = CreateEvent(NULL, FALSE, FALSE, NULL);
    res = CANTP_SetValue_2016(receiver_handle, PCANTP_PARAMETER_RECEIVE_EVENT, &receive_event, sizeof(receive_event));
    printf("Create a receive event on receiver: %s\n", STATUS_OK_KO(res));

    // Change STmin value to 1500us
    STmin = STMIN_1500US;
    res = CANTP_SetValue_2016(transmitter_handle, PCANTP_PARAMETER_SEPARATION_TIME, &STmin, sizeof(STmin));
    printf("Set STMIN = 1500us on transmitter: %s\n", STATUS_OK_KO(res));
    res = CANTP_SetValue_2016(receiver_handle, PCANTP_PARAMETER_SEPARATION_TIME, &STmin, sizeof(STmin));
    printf("Set STMIN = 1500us on receiver: %s\n", STATUS_OK_KO(res));

    // Allocate tx message
    res = CANTP_MsgDataAlloc_2016(&tx_msg, PCANTP_MSGTYPE_CAN);
    printf("Allocate tx CAN message: %s\n", STATUS_OK_KO(res));

    // Allocate rx message
    res = CANTP_MsgDataAlloc_2016(&rx_msg, PCANTP_MSGTYPE_NONE);
    printf("Allocate rx message: %s\n", STATUS_OK_KO(res));

    // Create a simple physical mapping:
    can_id = 0x18DA60F9;
    mapping.can_id = can_id;
    mapping.can_id_flow_ctrl = 0x18DAF960;
    mapping.can_msgtype = PCANTP_CAN_MSGTYPE_EXTENDED;
    mapping.netaddrinfo.extension_addr = 0x00;
    mapping.netaddrinfo.format = PCANTP_ISOTP_FORMAT_NORMAL;
    mapping.netaddrinfo.msgtype = PCANTP_ISOTP_MSGTYPE_DIAGNOSTIC;
    mapping.netaddrinfo.source_addr = 0xF9;
    mapping.netaddrinfo.target_addr = 0x60;
    mapping.netaddrinfo.target_type = PCANTP_ISOTP_ADDRESSING_PHYSICAL;

    // Add mapping on channels
    res = CANTP_AddMapping_2016(transmitter_handle, &mapping);
    printf("Add a simple mapping on transmitter: %s\n", STATUS_OK_KO(res));
    res = CANTP_AddMapping_2016(receiver_handle, &mapping);
    printf("Add a simple mapping on receiver: %s\n", STATUS_OK_KO(res));

    std::vector<uint8_t> byteArray;
    bool ok;

    // Process the hex string two characters at a time
    for (int i = 0; i < inputData.size(); i += 2) {
        QString byteString = inputData.mid(i, 2);
        uint8_t byte = byteString.toUInt(&ok, 16);

        if (ok) {
            byteArray.push_back(byte);
        } else {
            qDebug() << "Invalid hex string:" << byteString;
            // Handle error appropriately, maybe clear the byteArray and break
            byteArray.clear();
            break;
        }
    }
    qDebug()<<byteArray.size() ;
    // Initialize Tx message
    res = CANTP_MsgDataInit_2016(&tx_msg, mapping.can_id, PCANTP_CAN_MSGTYPE_EXTENDED, 8, NULL, NULL);
    for(size_t j = 0 ; j<byteArray.size() ; ++j){
        tx_msg.msgdata.can->data[j] = byteArray[j];
    }

    // ui->dataField_sim->setText(bytes);

    // Write FF message
    res = CANTP_Write_2016(transmitter_handle, &tx_msg);
    printf("Write FF message on transmitter: %s\n", STATUS_OK_KO(res));

    wait_result = WaitForSingleObject(receive_event, 20);
    printf("Wait for FC frame on receiver: %s\n", OK_KO(wait_result == WAIT_OBJECT_0));

    if (wait_result == WAIT_OBJECT_0) {
        // Read FC frame
        res = CANTP_Read_2016(receiver_handle, &rx_msg, NULL, PCANTP_MSGTYPE_CAN);
        printf("Read FC frame on receiver: %s\n", STATUS_OK_KO(res));
        printf("FC frame data: ");
        for (int i = 0; i < 8; i++) {
            uint8_t ptr = rx_msg.msgdata.can->data[i];
            qDebug()<<ptr ;
            printf("%02x ", rx_msg.msgdata.can->data[i]);
        }

        //Diagnostic Session Control
        if(byteArray[1]== 0x10){
            if(byteArray[2]==0x02){
               outputData = "06 50 02 0F A0 01 F4 FF" ;
                res = CANTP_MsgDataInit_2016(&rx_msg, mapping.can_id_flow_ctrl, PCANTP_CAN_MSGTYPE_EXTENDED, 8, NULL, NULL);
                // rx_msg.msgdata.can->length = 0x08;
                rx_msg.msgdata.can->data[0]= 0x06 ;
                rx_msg.msgdata.can->data[1]= 0x50 ;
                rx_msg.msgdata.can->data[2]= 0x02 ;
                rx_msg.msgdata.can->data[3]= 0x0F ;
                rx_msg.msgdata.can->data[4]= 0xA0 ;
                rx_msg.msgdata.can->data[5]= 0x01 ;
                rx_msg.msgdata.can->data[6]= 0xF4 ;
                rx_msg.msgdata.can->data[7]= 0xFF ;
                res = CANTP_Write_2016(receiver_handle, &rx_msg);
            }
            for (int i = 0; i < 8; i++) {
                uint8_t ptr = rx_msg.msgdata.can->data[i];
                qDebug()<<ptr ;
                printf("%02x ", rx_msg.msgdata.can->data[i]);
            }
            if(byteArray[2]==0x03){
                outputData = "06 50 03 00 FA 01 F4 FF" ;

                res = CANTP_MsgDataInit_2016(&rx_msg, mapping.can_id_flow_ctrl, PCANTP_CAN_MSGTYPE_EXTENDED, 8, NULL, NULL);
                // rx_msg.msgdata.can->length = 0x08;
                rx_msg.msgdata.can->data[0]= 0x06 ;
                rx_msg.msgdata.can->data[1]= 0x50 ;
                rx_msg.msgdata.can->data[2]= 0x03 ;
                rx_msg.msgdata.can->data[3]= 0x00 ;
                rx_msg.msgdata.can->data[4]= 0xFA ;
                rx_msg.msgdata.can->data[5]= 0x01 ;
                rx_msg.msgdata.can->data[6]= 0xF4 ;
                rx_msg.msgdata.can->data[7]= 0xFF ;
                res = CANTP_Write_2016(receiver_handle, &rx_msg);
            }

            if(byteArray[2]==0x01){
                outputData ="06 50 01 00 FA 01 F4 FF" ;

                res = CANTP_MsgDataInit_2016(&rx_msg, mapping.can_id_flow_ctrl, PCANTP_CAN_MSGTYPE_EXTENDED, 8, NULL, NULL);
                // rx_msg.msgdata.can->length = 0x08;
                rx_msg.msgdata.can->data[0]= 0x06 ;
                rx_msg.msgdata.can->data[1]= 0x50 ;
                rx_msg.msgdata.can->data[2]= 0x01 ;
                rx_msg.msgdata.can->data[3]= 0x00 ;
                rx_msg.msgdata.can->data[4]= 0xFA ;
                rx_msg.msgdata.can->data[5]= 0x01 ;
                rx_msg.msgdata.can->data[6]= 0xF4 ;
                rx_msg.msgdata.can->data[7]= 0xFF ;
                res = CANTP_Write_2016(receiver_handle, &rx_msg);
            }

        }
        //RDBI
        if(byteArray[1]==0x22){
            //Boot Software Identification
            if(byteArray[2]==0xF1 && byteArray[3]==0x80){
                outputData ="10 45 62 F1 80 42 00 03" ;
                res = CANTP_MsgDataInit_2016(&rx_msg, mapping.can_id_flow_ctrl, PCANTP_CAN_MSGTYPE_EXTENDED, 8, NULL, NULL);
                // rx_msg.msgdata.can->length = 0x45;
                rx_msg.msgdata.can->data[0]= 0x10 ;
                rx_msg.msgdata.can->data[1]= 0x45 ;
                rx_msg.msgdata.can->data[2]= 0x62 ;
                rx_msg.msgdata.can->data[3]= 0xF1 ;
                rx_msg.msgdata.can->data[4]= 0x80 ;
                rx_msg.msgdata.can->data[5]= 0x42 ;
                rx_msg.msgdata.can->data[6]= 0x00 ;
                rx_msg.msgdata.can->data[7]= 0x03 ;
                res = CANTP_Write_2016(receiver_handle, &rx_msg);
            }
            //VIN
            if(byteArray[2]==0xF1 && byteArray[3]==0x90){
                outputData = "10 14 62 F1 90 01 02 03" ;
                res = CANTP_MsgDataInit_2016(&rx_msg, mapping.can_id_flow_ctrl, PCANTP_CAN_MSGTYPE_EXTENDED, 8, NULL, NULL);
                rx_msg.msgdata.can->length = 0x45;
                rx_msg.msgdata.can->data[0]= 0x10 ;
                rx_msg.msgdata.can->data[1]= 0x14 ;
                rx_msg.msgdata.can->data[2]= 0x62 ;
                rx_msg.msgdata.can->data[3]= 0xF1 ;
                rx_msg.msgdata.can->data[4]= 0x90 ;
                rx_msg.msgdata.can->data[5]= 0x01 ;
                rx_msg.msgdata.can->data[6]= 0x02 ;
                rx_msg.msgdata.can->data[7]= 0x03 ;
                res = CANTP_Write_2016(receiver_handle, &rx_msg);

                res = CANTP_MsgDataInit_2016(&rx_msg, mapping.can_id, PCANTP_CAN_MSGTYPE_EXTENDED, 8, NULL, NULL);
                // rx_msg.msgdata.can->length = 0x45;
                rx_msg.msgdata.can->data[0]= 0x30 ;
                rx_msg.msgdata.can->data[1]= 0x00 ;
                rx_msg.msgdata.can->data[2]= 0x00 ;
                rx_msg.msgdata.can->data[3]= 0xFF ;
                rx_msg.msgdata.can->data[4]= 0xFF ;
                rx_msg.msgdata.can->data[5]= 0xFF ;
                rx_msg.msgdata.can->data[6]= 0xFF ;
                rx_msg.msgdata.can->data[7]= 0xFF ;
                res = CANTP_Write_2016(receiver_handle, &rx_msg);
                res = CANTP_MsgDataInit_2016(&rx_msg, mapping.can_id_flow_ctrl, PCANTP_CAN_MSGTYPE_EXTENDED, 8, NULL, NULL);
                // rx_msg.msgdata.can->length = 0x45;
                rx_msg.msgdata.can->data[0]= 0x21 ;
                rx_msg.msgdata.can->data[1]= 0x04 ;
                rx_msg.msgdata.can->data[2]= 0x05 ;
                rx_msg.msgdata.can->data[3]= 0x06 ;
                rx_msg.msgdata.can->data[4]= 0x07 ;
                rx_msg.msgdata.can->data[5]= 0x08 ;
                rx_msg.msgdata.can->data[6]= 0x09 ;
                rx_msg.msgdata.can->data[7]= 0x0A ;
                res = CANTP_Write_2016(receiver_handle, &rx_msg);

                res = CANTP_MsgDataInit_2016(&rx_msg, mapping.can_id_flow_ctrl, PCANTP_CAN_MSGTYPE_EXTENDED, 8, NULL, NULL);
                // rx_msg.msgdata.can->length = 0x45;
                rx_msg.msgdata.can->data[0]= 0x22 ;
                rx_msg.msgdata.can->data[1]= 0x0B ;
                rx_msg.msgdata.can->data[2]= 0x0C ;
                rx_msg.msgdata.can->data[3]= 0x0D ;
                rx_msg.msgdata.can->data[4]= 0x0E ;
                rx_msg.msgdata.can->data[5]= 0x0F ;
                rx_msg.msgdata.can->data[6]= 0x10 ;
                rx_msg.msgdata.can->data[7]= 0x11 ;
                res = CANTP_Write_2016(receiver_handle, &rx_msg);


            }
            //UDS VERSION
            if(byteArray[2]==0xFF && byteArray[3]==0x00){
                outputData = "07 62 FF 00 02 00 00 00" ;

                res = CANTP_MsgDataInit_2016(&rx_msg, mapping.can_id_flow_ctrl, PCANTP_CAN_MSGTYPE_EXTENDED, 8, NULL, NULL);
                //rx_msg.msgdata.can->length = 0x45;
                rx_msg.msgdata.can->data[0]= 0x07 ;
                rx_msg.msgdata.can->data[1]= 0x62 ;
                rx_msg.msgdata.can->data[2]= 0xFF ;
                rx_msg.msgdata.can->data[3]= 0x00 ;
                rx_msg.msgdata.can->data[4]= 0x02 ;
                rx_msg.msgdata.can->data[5]= 0x00 ;
                rx_msg.msgdata.can->data[6]= 0x00 ;
                rx_msg.msgdata.can->data[7]= 0x00 ;
                res = CANTP_Write_2016(receiver_handle, &rx_msg);
            }
            //Active Diagnostic Session
            if(byteArray[2]==0xF1 && byteArray[3]==0x86){
               outputData = "04 62 FF 86 01 FF FF FF" ;

                res = CANTP_MsgDataInit_2016(&rx_msg, mapping.can_id_flow_ctrl, PCANTP_CAN_MSGTYPE_EXTENDED, 8, NULL, NULL);
                //rx_msg.msgdata.can->length = 0x45;
                rx_msg.msgdata.can->data[0]= 0x04 ;
                rx_msg.msgdata.can->data[1]= 0x62 ;
                rx_msg.msgdata.can->data[2]= 0xFF ;
                rx_msg.msgdata.can->data[3]= 0x86 ;
                rx_msg.msgdata.can->data[4]= 0x01 ;
                rx_msg.msgdata.can->data[5]= 0xFF ;
                rx_msg.msgdata.can->data[6]= 0xFF ;
                rx_msg.msgdata.can->data[7]= 0xFF ;
                res = CANTP_Write_2016(receiver_handle, &rx_msg);
            }
        }
    }
    // Free messages space
    res = CANTP_MsgDataFree_2016(&tx_msg);
    printf("Free tx message: %s\n", STATUS_OK_KO(res));
    res = CANTP_MsgDataFree_2016(&rx_msg);
    printf("Free rx message: %s\n", STATUS_OK_KO(res));

    // Close receive event
    res = CANTP_SetValue_2016(receiver_handle, PCANTP_PARAMETER_RECEIVE_EVENT, &null_handle, sizeof(HANDLE));
    printf("Stop receive event on receiver: %s\n", STATUS_OK_KO(res));
    res_b = CloseHandle(receive_event);
    printf("Close receive event: %s\n", OK_KO(res_b));

    // Uninitialize channels
    res = CANTP_Uninitialize_2016(transmitter_handle);
    printf("Uninitialize transmitter: %s\n", STATUS_OK_KO(res));
    res = CANTP_Uninitialize_2016(receiver_handle);
    printf("Uninitialize receiver: %s\n", STATUS_OK_KO(res));
}

