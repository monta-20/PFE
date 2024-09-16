#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include "loadjson_sovd.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"



void loadJsonData(MainWindow *window) {
    QFile file(":/File/file/data.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
       window->ui->textEdit->setPlainText("Failed to open Data JSON file.");
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (!document.isObject()) {
        window->ui->textEdit->setPlainText("Invalid Data JSON file.");
        return;
    }

    window->jsonObject = document.object();
}

void loadDtcJsonData(MainWindow *window) {
    QFile file(":/File/file/fault.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        window->ui->codeTextEdit->setPlainText("Failed to open DTC JSON file.");
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(jsonData);
    if (!document.isObject()) {
        window->ui->codeTextEdit->setPlainText("Invalid DTC JSON file.");
        return;
    }

    window->dtcJsonObject = document.object();
}

void populateComboBox(MainWindow *window) {
    window->ui->comboBox->addItem("Request: HTTP GET " + window->baseUri + "/DataIdentifier");
    QJsonArray componentsArray = window->jsonObject["DataIdentifier"].toArray();
    for (const QJsonValue &value : componentsArray) {
        QJsonObject componentObject = value.toObject();
        window->ui->comboBox->addItem("Request: HTTP GET " + window->baseUri + "/DataIdentifier/" + componentObject["id"].toString());
    }
}

void populateCodeComboBox(MainWindow *window) {
    window->ui->codeComboBox->addItem("Request: HTTP GET " + window->baseUri + "/faults");
    QJsonArray itemsArray = window->dtcJsonObject["faults"].toArray();
    for (const QJsonValue &value : itemsArray) {
        QJsonObject itemObject = value.toObject();
        window->ui->codeComboBox->addItem("Request: HTTP GET " + window->baseUri + "/faults/" +itemObject["id"].toString());
    }
}

void displayAllItems(MainWindow *window) {
    QString url = window->baseUri + "/DataIdentifier";
    fetchData(url,window);
}

void displayItemById(const QString &id,MainWindow *window) {
    QString displayText ="HTTP OK 200\n";
    displayText += "{\n";
    displayText += "  \"items\": [\n";

    QJsonArray dataIdentifierArray = window->jsonObject["DataIdentifier"].toArray();
    for (const QJsonValue &val : dataIdentifierArray) {
        QJsonObject itemObject = val.toObject();
        if (itemObject["id"].toString() == id) {
            displayText += "    {\n";
            displayText += QString("      \"id\": \"%1\",\n").arg(itemObject["id"].toString());
            displayText += QString("      \"name\": \"%1\",\n").arg(itemObject["name"].toString());
            displayText += QString("      \"category\": \"%1\"\n").arg(itemObject["category"].toString());
            displayText += QString("      \"data\": \"%1\"\n").arg(itemObject["data"].toString());
            displayText += "    }";
            break;
        }
    }
    displayText += "\n  ]\n";
    displayText += "}";

    if (!displayText.isEmpty()) {
        window->ui->textEdit->setPlainText(displayText);
    } else {
        window->ui->textEdit->setPlainText("ID not found or no data available for ID: " + id);
    }
}

void displayItemByCode(const QString &id,MainWindow *window) {
    QString displayText ="HTTP OK 200\n";
    displayText += "{\n";
    displayText += "  \"items\": [\n";


    QJsonArray itemsArray = window->dtcJsonObject["faults"].toArray();
    for (const QJsonValue &val : itemsArray) {
        QJsonObject itemObject = val.toObject();
        if (itemObject["id"].toString() == id) {
            displayText += "    {\n";
            displayText += QString("      \"id\": \"%1\",\n").arg(itemObject["id"].toString());
            displayText += QString("      \"scope\": \"%1\",\n").arg(itemObject["scope"].toString());
            displayText += QString("      \"display_code\": \"%1\"\n").arg(itemObject["display_code"].toString());
            displayText += QString("      \"fault_translation_id\": \"%1\"\n").arg(itemObject["fault_translation_id"].toString());
            displayText += QString("      \"severity\": \"%1\"\n").arg(itemObject["severity"].toInt());
            displayText += QString("      \"status\":\n");
            displayText += QString("           {\n");
            displayText += QString("                \"mask\": \"%1\"\n").arg(itemObject["status"].toObject()["mask"].toString());
            displayText += QString("                \"aggregatedStatus\": \"%1\"\n").arg(itemObject["status"].toObject()["aggregatedStatus"].toString());
            displayText += QString("           }\n");

            displayText += "    }";
            break;
        }
    }
    displayText += "\n  ]\n";
    displayText += "}";
    if (!displayText.isEmpty()) {
        window->ui->codeTextEdit->setPlainText(displayText);
    } else {
        window->ui->codeTextEdit->setPlainText("Code not found or no data available for Code: " + id);
    }
}

void fetchData(const QString &url,MainWindow *window) {
    QUrl qurl(url);
    QNetworkRequest request(qurl);
    window->networkManager->get(request);
}

void onNetworkReply(QNetworkReply *reply,MainWindow *window) {
    if (reply->error() != QNetworkReply::NoError) {
        window->ui->textEdit->setPlainText("Failed to fetch data: " + reply->errorString());
        reply->deleteLater();
        return;
    }
    QByteArray responseData = reply->readAll();
    QJsonDocument document = QJsonDocument::fromJson(responseData);
    if (!document.isArray() && !document.isObject()) {
        window->ui->textEdit->setPlainText("Invalid JSON response.");
        reply->deleteLater();
        return;
    }
    QString displayText ="HTTP OK 200\n";
    displayText += "{\n";
    displayText += "  \"items\": [\n";
    // Si la réponse est un objet JSON
    if (document.isObject()) {
        QJsonObject dataObject = document.object();
        QJsonArray dataArray = dataObject["DataIdentifier"].toArray();
        for (const QJsonValue &value : dataArray) {
            QJsonObject itemObject = value.toObject();
            displayText += "    {\n";
            displayText += QString("      \"id\": \"%1\",\n").arg(itemObject["id"].toString());
            displayText += QString("      \"name\": \"%1\",\n").arg(itemObject["name"].toString());
            displayText += QString("      \"category\": \"%1\"\n").arg(itemObject["category"].toString());
            displayText += "    }";
        }

    } else if (document.isArray()) {
        // Si la réponse est un tableau JSON
        QJsonArray dataArray = document.array();
        for (const QJsonValue &value : dataArray) {
            QJsonObject itemObject = value.toObject();
            displayText += "    {\n";
            displayText += QString("      \"id\": \"%1\",\n").arg(itemObject["id"].toString());
            displayText += QString("      \"name\": \"%1\",\n").arg(itemObject["name"].toString());
            displayText += QString("      \"category\": \"%1\"\n").arg(itemObject["category"].toString());
            displayText += "    }";
        }
        displayText += "\n  ]\n";
        displayText += "}";
        window->ui->textEdit->setPlainText(displayText);reply->deleteLater();
    }
}
void onComboBoxChanged(int index,MainWindow *window) {
    if (index == 0) {
        displayAllItems(window);
    } else {
        QString id = window->ui->comboBox->itemText(index).split("/").last();
        displayItemById(id,window);
    }
}

void onCodeComboBoxChanged(int index,MainWindow *window) {
    if (index == 0) {
        window->ui->textEdit->setPlainText("Select a code to display details.");
    } else {
        QString code = window->ui->codeComboBox->itemText(index).split("/").last();
        displayItemByCode(code,window);
    }
}


