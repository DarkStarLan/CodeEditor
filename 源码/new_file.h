#pragma once

#include <QDialog>
#include "ui_new_file.h"
#include"QtTool.h"

class new_file : public QDialog {
    Q_OBJECT
public:
    //���캯��
    new_file(QWidget* parent, QString* openFilePath, bool* IsOpenFile, QString defaultText = "");
    //��������
    ~new_file();
private:
    Ui::newFileDialog ui;
    QString* OpenFilePath;
    bool* isOpenFile;
};
