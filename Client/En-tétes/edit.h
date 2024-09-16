#ifndef EDIT_H
#define EDIT_H
#include <QToolButton>
#include <QPropertyAnimation>
#include <QString>

class MainWindow;

void checkIdLength(MainWindow *window, const QString &text);
void champSet(MainWindow *window);
void updateFormattedText(MainWindow *window, const QString &text);
void updateCharCount(MainWindow *window);
void resizeLineEdit(MainWindow *window);
void setupToolBar(MainWindow *window);
QPropertyAnimation* createButtonAnimation(MainWindow *window,QAction *button, const QString &startStyle, const QString &endStyle);
void updateToolButtonState(MainWindow *window,QAction *button);
void toggleToolBar(MainWindow *window);


#endif // EDIT_H
