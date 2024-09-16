#ifndef SECURITY_MANAGER_H
#define SECURITY_MANAGER_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

Ui::MainWindow *ui ;
void handleSecurityAccessRequest(const cantp_msg &request, cantp_msg &response);
void generateNewSeed();
uint32_t generateKey(uint16_t seed_part);
uint16_t usMain_eSecKey_Calcul(uint16_t FusSeed);
bool verifyKey(uint16_t key);

#endif // SECURITY_MANAGER_H
