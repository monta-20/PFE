#include "parsexml.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDomDocument>
#include <QMessageBox>

void parseXML(MainWindow *window) {
    QFile file(":/File/file/DID.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file";
        return;
    }

    QXmlStreamReader xml(&file);
    QString currentService;
    QString currentFunctionGroup;

    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();

        if (token == QXmlStreamReader::StartElement) {
            if (xml.name() == QLatin1String("FUNC-GROUP")) {
                currentFunctionGroup = xml.attributes().value("name").toString();
                window->ui->functionGroupComboBox->addItem(currentFunctionGroup);
                window->udsMap[currentFunctionGroup]; // Initialize the map for the current function group
            } else if (xml.name() == QLatin1String("SID") && !currentFunctionGroup.isEmpty()) {
                currentService = xml.attributes().value("DES").toString();
                QString serviceID = xml.attributes().value("value").toString();
                window->udsMap[currentFunctionGroup][currentService]; // Initialize the map for the current service
                window->serviceIDMap[currentService] = serviceID;
            } else if (xml.name() == QLatin1String("SUB-FUNC") && !currentService.isEmpty()) {
                QString subFuncName = xml.attributes().value("Name").toString();
                QString subFuncID = xml.attributes().value("VALUE").toString();
                window->udsMap[currentFunctionGroup][currentService].insert(subFuncName, subFuncID);
            }
        }
    }
    if (xml.hasError()) {
        qDebug() << "Error: " << xml.errorString();
    }xml.clear();
    file.close();
}

void onFunctionGroupChanged(MainWindow *window, int index) {
    QString selectedFunctionGroup = window->ui->functionGroupComboBox->currentText();
    window->ui->serviceComboBox->clear();
    window->ui->serviceIDLineEdit->clear();
    window->ui->subFuncComboBox->clear();
    window->ui->subFuncLineEdit->clear();
    if (window->udsMap.contains(selectedFunctionGroup)) {
        QMap<QString, QMap<QString, QString>> services = window->udsMap[selectedFunctionGroup];
        for (auto it = services.begin(); it != services.end(); ++it) {
            window->ui->serviceComboBox->addItem(it.key(), window->serviceIDMap[it.key()]);
        }
    }
}

void onServiceChanged(MainWindow *window, int index) {
    QString selectedFunctionGroup = window->ui->functionGroupComboBox->currentText();
    QString selectedService = window->ui->serviceComboBox->currentText();
    window->ui->serviceIDLineEdit->setText(window->serviceIDMap[selectedService]);
    window->ui->subFuncComboBox->clear();
    window->ui->subFuncLineEdit->clear();
    QString serviceID = window->serviceIDMap[selectedService];
    window->ui->serviceIDLineEdit->setText(serviceID);
    window->ui->SD0->clear();
    if (!serviceID.isEmpty()) {
        window->ui->SD1->setText(serviceID.right(2));        // Assuming serviceID is in the format "0x22"
        window->ui->SD2->clear();
        window->ui->SD3->clear();
    }
    if (window->udsMap[selectedFunctionGroup].contains(selectedService)) {
        QMap<QString, QString> subFuncs = window->udsMap[selectedFunctionGroup][selectedService];
        for (auto it = subFuncs.begin(); it != subFuncs.end(); ++it) {
            window->ui->subFuncComboBox->addItem(it.key(), it.value());
        }
    }
}

void onSubFuncChanged(MainWindow *window, int index) {
    if (index != -1) {
        window->ui->subFuncLineEdit->setText(window->ui->subFuncComboBox->currentData().toString());
        // Set data fields based on sub-function ID
        QString subFuncID = window->ui->subFuncComboBox->currentData().toString();
        QString didPart = subFuncID.mid(2); // Assuming subFuncID is in the format "0xF1XX"
        QString serviceTarget = window->ui->serviceComboBox->currentData().toString();
        if (!subFuncID.isEmpty()) {
            if(serviceTarget=="0x22"){

                window->ui->SD2->setText(didPart.left(2)); // D1 = "F1"
                window->ui->SD3->setText(didPart.right(2)); // D2 = "XX"
            }else if(serviceTarget=="0x10"||serviceTarget=="0x11" ){
                window->ui->SD2->setText(didPart.left(2));
                window->ui->SD3->clear();
            }
            else {
                window->ui->subFuncLineEdit->clear();
                window->ui->SD2->clear();
                window->ui->SD3->clear();
            }
        }
    }
}
