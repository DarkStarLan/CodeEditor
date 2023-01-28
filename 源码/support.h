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
	//构造函数
	support(QWidget *parent, QHash<QString, Command>* lspt);
	//析构函数
	~support();
private:
	Ui::languageSupportDialog ui;
	QHash<QString, Command>* LSpt;  //语言支持
};