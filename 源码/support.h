#pragma once

#include <QDialog>
#include"ui_support.h"
#include"QtTool.h"
#include<qinputdialog.h>
#include"new_file.h"
#include"Command.h"

class support : public QDialog {
	Q_OBJECT
public:
	//���캯��
	support(QWidget *parent, QHash<QString, Command>* lspt);
	//��������
	~support();
private:
	Ui::languageSupportDialog ui;
	QHash<QString, Command>* LSpt;  //����֧��
};