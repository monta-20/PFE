#include "edit.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

void checkIdLength(MainWindow *window, const QString &text) {
  int minLength = 5;
  int maxLength = 10;
  if (text.length() < minLength) {
     window->ui->idField->setStyleSheet("QLineEdit{ background-color: #FF0000; }");
  } else if (text.length() > maxLength) {
  QMessageBox::warning(window, "Warning", QString("Please enter a value between %1 and %2 characters.").arg(minLength).arg(maxLength));
  window->ui->idField->setText(text.left(maxLength));
  } else {
    window->ui->idField->setStyleSheet("");
  }
}

void champSet(MainWindow *window) {

   window->ui->dataField->setPlaceholderText("64 bits");
   window->ui->dataField->setFont(QFont("Times New Roman", 14));
   window->ui->dataField->setText("0x0210020000000000");
   window->ui->dataField->setMaxLength(4095);
   /*QString sty = "QLineEdit{"
    "border-radius: 4px;"
    "color:rgb(0, 0, 0);"
    "background-color: rgb(255, 255, 255);"
"}"

"QLineEdit:focus {"
    "border-style:outset;"
    "border-width:4px;"
    "border-radius: 4px;"
    "border-color: #2EECEC;"
    "color:rgb(0, 0, 0);"
    "background-color: rgb(255,255,255);"
"}";
   window->ui->receivedIdField->setStyleSheet(sty);*/
   window->ui->idField->setPlaceholderText("11/29 bits");
   window->ui->idField->setText("0x18DA60F9");
   window->ui->idField->setFont(QFont("Times New Roman", 14));
   window->ui->dlcField->setFont(QFont("Times New Roman", 14));
   window->ui->dlcField->setText("8");
   window->ui->receivedDataField->setReadOnly(true);
   window->ui->receivedDataField->setFont(QFont("Times New Roman", 14));
   window->ui->receivedIdField->setReadOnly(true);
   window->ui->receivedIdField->setFont(QFont("Times New Roman", 14));
   window->ui->receivedDlcField->setReadOnly(true);
   window->ui->receivedDlcField->setFont(QFont("Times New Roman", 14));
   window->ui->ServiceNameField->setReadOnly(true);
   window->ui->DIDNameField->setReadOnly(true);
   window->ui->DIDValueField->setReadOnly(true);
   window->ui->SBFNameField->setReadOnly(true);
   window->ui->SBFValueField->setReadOnly(true);
   window->ui->SBFNameResField->setReadOnly(true);
   window->ui->SBFValueResField->setReadOnly(true);
   window->ui->SBFDescriptionResField->setReadOnly(true);
}

void updateFormattedText(MainWindow *window, const QString &text){
    QString cleanText = text.simplified().remove(' '); // Remove all spaces

    QString formattedText ;

    for(int i = 0 ; i<cleanText.length();i+=2){
        QString pair = cleanText.mid(i,2);
        if(!pair.isEmpty()){
            formattedText += pair + "";
        }
    }
    window->ui->dataField2->blockSignals(true);
    window->ui->dataField2->setText(formattedText.trimmed());
    window->ui->dataField2->blockSignals(false);

    window->ui->dataField_sim->blockSignals(true);
    window->ui->dataField_sim->setText(formattedText.trimmed());
    window->ui->dataField_sim->blockSignals(false);

    updateCharCount(window);

}

void updateCharCount(MainWindow * window){
    QString text = window->ui->dataField2->text().simplified().remove(' '); // Remove all spaces
    int totalCharCountExclude = qMax(0, text.length() - 2); // Exclude the first two characters
    int totalCharCount = qMax(0, text.length());
    int halfCharCountExclude = totalCharCountExclude / 2;
    int halfCharCount = totalCharCount / 2 ;
    window->ui->ByteCount->setText(QString("%1").arg(halfCharCountExclude));
    window->ui->ByteCount_sim->setText(QString("%1").arg(halfCharCount));
}
void setupToolBar(MainWindow *window){

}

QPropertyAnimation* createButtonAnimation(MainWindow *window, QAction *button, const QString &startStyle, const QString &endStyle)
{
    QPropertyAnimation *animation = new QPropertyAnimation(window->findChild<QToolButton*>(button->text()), "styleSheet");
    animation->setDuration(500);
    animation->setStartValue(startStyle);
    animation->setEndValue(endStyle);
    return animation;
}

void updateToolButtonState( MainWindow *window,QAction *button)
{
    // Uncheck all buttons
    window->ui->actionTransmit_Recieve->setChecked(false);
    window->ui->actionCANTP->setChecked(false);
    window->ui->actionSOVD->setChecked(false);

    // Check the clicked button
    button->setChecked(true);

    // Create and configure the animation
    QPropertyAnimation *animation = createButtonAnimation(
        window,
        button,
        "QToolButton { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #c0392b, stop:1 #a93226); color: white; border-radius: 15px; padding: 10px; font-size: 14px; border: 2px solid #b03a2e; }",
        "QToolButton { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #e74c3c, stop:1 #c0392b); color: white; border-radius: 15px; padding: 10px; font-size: 14px; border: 2px solid #b03a2e; }"
        );
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void toggleToolBar(MainWindow *window)
{
   // bool isVisible = window->ui->toolBar->isVisible();
   //window->ui->toolBar->setVisible(!isVisible);
   //toggleButton->setIcon(QIcon(isVisible ? ":/icons/close.png" : ":/icons/open.png"));
}
