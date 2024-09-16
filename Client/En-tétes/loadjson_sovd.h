#ifndef LOADJSON_SOVD_H
#define LOADJSON_SOVD_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class MainWindow;

void onComboBoxChanged(int index,MainWindow *window);
void onCodeComboBoxChanged(int index,MainWindow *window);
void onNetworkReply(QNetworkReply *reply,MainWindow *window);

void loadJsonData(MainWindow *window,QString str);
void loadDtcJsonData(MainWindow *window);
void populateComboBox(MainWindow *window);
void populateCodeComboBox(MainWindow *window);
void displayAllItems(MainWindow *window);
void displayItemById(const QString &id,MainWindow *window);
void displayItemByCode(const QString &code,MainWindow *window);
void fetchData(const QString &url,MainWindow *window);



#endif // LOADJSON_SOVD_H
