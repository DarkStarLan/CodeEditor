#pragma once

#include <QDialog>
#include"ui_insertKeys.h"
#include"QtTool.h"
#include<qspinbox.h>
#include<qcolordialog.h>
#include<qinputdialog.h>

class insertKeys : public QDialog {
	Q_OBJECT
public:
	//构造函数
	insertKeys(QWidget *parent, QHash<QString, QColor>* keys);
	//显示颜色
	void setColorShow(const short r, const short g, const short b);
	//析构函数
	~insertKeys();
private:
	Ui::insertKeysDialog ui;
	QHash<QString, QColor>* KeysMap;
};