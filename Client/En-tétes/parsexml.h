#ifndef PARSEXML_H
#define PARSEXML_H

#include <QString>
#include <QDomElement>
#include <QtXml>
class MainWindow;
void parseXML(MainWindow *window);
void onFunctionGroupChanged(MainWindow *window, int index);
void onServiceChanged(MainWindow *window, int index);
void onSubFuncChanged(MainWindow *window, int index);
#endif // PARSEXML_H
