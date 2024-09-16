#include "securitymanager.h"

void MainWindow:: handleSecurityAccessRequest(const cantp_msg &request, cantp_msg &response)
{
    bool ok ;
    uint8_t serviceId = request.msgdata.can->data[1];
    uint8_t subFunction = request.msgdata.can->data[2];



if (serviceId == 0x27) {
    if(subFunction == 0x03){
  generateNewSeed();


  response.msgdata.can->data[0] = 0x04;
  ui->SAR0->setText("04");
  response.msgdata.can->data[1] = 0x67;
  ui->SAR1->setText("67");
  response.msgdata.can->data[2] = 0x03;
  ui->SARL2->setText("03");
  response.msgdata.can->data[3] = (currentSeedPart >> 8) & 0xFF;
  uint16_t b = static_cast<int>((currentSeedPart >> 8) & 0xFF);
  qDebug()<< b;
  QString d3 = QString::number(b,16);
  qDebug()<<d3<<"valus now";
  ui->SARS3->setText(d3);
  (response.msgdata.can->data[4]) = currentSeedPart & 0xFF;
  uint16_t c = static_cast<int>(currentSeedPart & 0xFF) ;
  qDebug()<< c ;
  QString d4 = QString::number(c,16);
  ui->SARS4->setText(d4);
  qDebug()<<d4<<"valus now";
  response.msgdata.can->data[5] = 0xFF;
  ui->SAR5->setText("FF");
  response.msgdata.can->data[6] = 0xFF;
  ui->SAR6->setText("FF");
  response.msgdata.can->data[7] = 0xFF;
  ui->SAR7->setText("FF");


  std::cout << "Generated seed: " << std::hex <<currentSeedPart << std::endl;
  // Display the generated key
  std::cout << "Generated key based on seed: " << std::hex << currentKey << std::endl;
    }
    else if(subFunction == 0x01){
        generateNewSeed();
        response.msgdata.can->data[0] = 0x04;
        ui->SAR0->setText("04");
        response.msgdata.can->data[1] = 0x67;
        ui->SAR1->setText("67");
        response.msgdata.can->data[2] = 0x01;
        ui->SARL2->setText("02");
        response.msgdata.can->data[3] = (currentSeedPart >> 8) & 0xFF;
        uint16_t b = static_cast<int>((currentSeedPart >> 8) & 0xFF);
        qDebug()<< b;
        QString d3 = QString::number(b,16);
        qDebug()<<d3<<"valus now";
        ui->SARS3->setText(d3);
        (response.msgdata.can->data[4]) = currentSeedPart & 0xFF;
        uint16_t c = static_cast<int>(currentSeedPart & 0xFF) ;
        qDebug()<< c ;
        QString d4 = QString::number(c,16);
        ui->SARS4->setText(d4);
        qDebug()<<d4<<"valus now";
        response.msgdata.can->data[5] = 0xFF;
        ui->SAR5->setText("FF");
        response.msgdata.can->data[6] = 0xFF;
        ui->SAR6->setText("FF");
        response.msgdata.can->data[7] = 0xFF;
        ui->SAR7->setText("FF");
        std::cout << "Generated seed: " << std::hex <<currentSeedPart << std::endl;
        // Display the generated key
        std::cout << "Generated key based on seed: " << std::hex << currentKey << std::endl;
    }
    else if(subFunction == 0x05){
        generateNewSeed();
        response.msgdata.can->data[0] = 0x04;
        ui->SAR0->setText("04");
        response.msgdata.can->data[1] = 0x67;
        ui->SAR1->setText("67");
        response.msgdata.can->data[2] = 0x06;
        ui->SARL2->setText("06");
        response.msgdata.can->data[3] = (currentSeedPart >> 8) & 0xFF;
        uint16_t b = static_cast<int>((currentSeedPart >> 8) & 0xFF);
        qDebug()<< b;
        QString d3 = QString::number(b,16);
        qDebug()<<d3<<"valus now";
        ui->SARS3->setText(d3);
        (response.msgdata.can->data[4]) = currentSeedPart & 0xFF;
        uint16_t c = static_cast<int>(currentSeedPart & 0xFF) ;
        qDebug()<< c ;
        QString d4 = QString::number(c,16);
        ui->SARS4->setText(d4);
        qDebug()<<d4<<"valus now";
        response.msgdata.can->data[5] = 0xFF;
        ui->SAR5->setText("FF");
        response.msgdata.can->data[6] = 0xFF;
        ui->SAR6->setText("FF");
        response.msgdata.can->data[7] = 0xFF;
        ui->SAR7->setText("FF");
        std::cout << "Generated seed: " << std::hex <<currentSeedPart << std::endl;
        // Display the generated key
        std::cout << "Generated key based on seed: " << std::hex << currentKey << std::endl;
    }
    else if(subFunction==0x02) {
        // Send Key
        uint16_t receivedKey = generateKey(currentSeedPart);
        std::cout << "Generated key based on seed: " << std::hex << currentKey << std::endl;

        uint8_t key1 = (receivedKey >> 8) & 0xFF;
        uint8_t key2 = receivedKey & 0xFF ;
        if (verifyKey(receivedKey) && ui->SA03_K1->text().toUInt(&ok,16)==key1 && ui->SA04_K2->text().toUInt(&ok,16)==key2) {
            response.msgdata.can->data[0] = 0x02;
            ui->SAR00->setText("02");
            response.msgdata.can->data[1] = 0x67;
            ui->SAR01->setText("67");
            response.msgdata.can->data[2] = 0x02;
            ui->SAR02->setText("02");
            response.msgdata.can->data[3] = 0xFF;
            ui->SAR03->setText("FF");
            response.msgdata.can->data[4] = 0xFF;
            ui->SAR04->setText("FF");
            response.msgdata.can->data[5] = 0xFF;
            ui->SAR05->setText("FF");
            response.msgdata.can->data[6] = 0xFF;
            ui->SAR06->setText("FF");
            response.msgdata.can->data[7] = 0xFF;
            ui->SAR07->setText("FF");
            std::cout << "Key verified successfully" << std::endl;
        } else {
            response.msgdata.can->data[0] = 0x02;
            ui->SAR00->setText("02");
            response.msgdata.can->data[1] = 0x7F;
            ui->SAR01->setText("7F");
            response.msgdata.can->data[2] = 0x27;
            ui->SAR02->setText("27");         // Invalid Key error code
            response.msgdata.can->data[3] = 0x35;
            ui->SAR03->setText("35");
            response.msgdata.can->data[4] = 0xFF;
            ui->SAR04->setText("FF");
            response.msgdata.can->data[5] = 0xFF;
            ui->SAR05->setText("FF");
            response.msgdata.can->data[6] = 0xFF;
            ui->SAR06->setText("FF");
            response.msgdata.can->data[7] = 0xFF;
            ui->SAR07->setText("FF");
            std::cout << "Key verification failed" << std::endl;
        }
    }
    else if(subFunction==0x04) {
        // Send Key
   uint16_t receivedKey = generateKey(currentSeedPart);
   std::cout << "Generated key based on seed: " << std::hex << currentKey << std::endl;

   uint8_t key1 = (receivedKey >> 8) & 0xFF;
   uint8_t key2 = receivedKey & 0xFF ;
   if (verifyKey(receivedKey) && ui->SA03_K1->text().toUInt(&ok,16)==key1 && ui->SA04_K2->text().toUInt(&ok,16)==key2) {
             response.msgdata.can->data[0] = 0x02;
             ui->SAR00->setText("02");
             response.msgdata.can->data[1] = 0x67;
             ui->SAR01->setText("67");
             response.msgdata.can->data[2] = 0x04;
             ui->SAR02->setText("04");
             response.msgdata.can->data[3] = 0xFF;
             ui->SAR03->setText("FF");
             response.msgdata.can->data[4] = 0xFF;
             ui->SAR04->setText("FF");
             response.msgdata.can->data[5] = 0xFF;
             ui->SAR05->setText("FF");
               response.msgdata.can->data[6] = 0xFF;
             ui->SAR06->setText("FF");
               response.msgdata.can->data[7] = 0xFF;
             ui->SAR07->setText("FF");
               std::cout << "Key verified successfully" << std::endl;
    } else {
            response.msgdata.can->data[0] = 0x02;
             ui->SAR00->setText("02");
            response.msgdata.can->data[1] = 0x7F;
             ui->SAR01->setText("7F");
            response.msgdata.can->data[2] = 0x27;
             ui->SAR02->setText("27");             // Invalid Key error code
            response.msgdata.can->data[3] = 0xFF;
             ui->SAR03->setText("FF");
            response.msgdata.can->data[4] = 0xFF;
             ui->SAR04->setText("FF");
            response.msgdata.can->data[5] = 0xFF;
             ui->SAR05->setText("FF");
            response.msgdata.can->data[6] = 0xFF;
             ui->SAR06->setText("FF");
            response.msgdata.can->data[7] = 0xFF;
             ui->SAR07->setText("FF");
            std::cout << "Key verification failed" << std::endl;
        }
    }
    else if(subFunction==0x06) {
        // Send Key
        uint16_t receivedKey = generateKey(currentSeedPart);
        std::cout << "Generated key based on seed: " << std::hex << currentKey << std::endl;

        uint8_t key1 = (receivedKey >> 8) & 0xFF;
        uint8_t key2 = receivedKey & 0xFF ;
        if (verifyKey(receivedKey) && ui->SA03_K1->text().toUInt(&ok,16)==key1 && ui->SA04_K2->text().toUInt(&ok,16)==key2) {
            response.msgdata.can->data[0] = 0x02;
            ui->SAR00->setText("02");
            response.msgdata.can->data[1] = 0x67;
            ui->SAR01->setText("67");
            response.msgdata.can->data[2] = 0x06;
            ui->SAR02->setText("04");
            response.msgdata.can->data[3] = 0xFF;
            ui->SAR03->setText("FF");
            response.msgdata.can->data[4] = 0xFF;
            ui->SAR04->setText("FF");
            response.msgdata.can->data[5] = 0xFF;
            ui->SAR05->setText("FF");
            response.msgdata.can->data[6] = 0xFF;
            ui->SAR06->setText("FF");
            response.msgdata.can->data[7] = 0xFF;
            ui->SAR07->setText("FF");
            std::cout << "Key verified successfully" << std::endl;
        } else {
            response.msgdata.can->data[0] = 0x02;
            ui->SAR00->setText("02");
            response.msgdata.can->data[1] = 0x7F;
            ui->SAR01->setText("7F");
            response.msgdata.can->data[2] = 0x27;
            ui->SAR02->setText("27");             // Invalid Key error code
            response.msgdata.can->data[3] = 0xFF;
            ui->SAR03->setText("FF");
            response.msgdata.can->data[4] = 0xFF;
            ui->SAR04->setText("FF");
            response.msgdata.can->data[5] = 0xFF;
            ui->SAR05->setText("FF");
            response.msgdata.can->data[6] = 0xFF;
            ui->SAR06->setText("FF");
            response.msgdata.can->data[7] = 0xFF;
            ui->SAR07->setText("FF");
            std::cout << "Key verification failed" << std::endl;
        }

 }

}
}

void MainWindow:: generateNewSeed()
{

  currentSeed = rand() % 0xFFFF;
  currentSeedPart = static_cast<uint16_t>(currentSeed);
  currentKey = generateKey(currentSeedPart);
}

uint32_t MainWindow:: generateKey(uint16_t seed_part)
{

   uint16_t seed = static_cast<uint16_t>(seed_part & 0xFFFF);
   uint16_t key = usMain_eSecKey_Calcul(seed);
   return key;
}

uint16_t MainWindow::  usMain_eSecKey_Calcul(uint16_t FusSeed)
{
  uint16_t u16Key = 0;
  uint16_t u16Seed = FusSeed;
  uint16_t u16Count = 0;

  for (u16Count = 0; u16Count < 15; u16Count++) {
    u16Key ^= u16Seed;
    u16Seed = (uint16_t)((u16Seed << (1 + 0x03)) | (u16Seed >> (15 - 0x03)));
  }
  return u16Key;
}

bool MainWindow::verifyKey(uint16_t key)
{
  return key == currentKey;
}
