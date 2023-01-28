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
	//���캯��
	insertKeys(QWidget *parent, QHash<QString, QColor>* keys);
	//��ʾ��ɫ
	void setColorShow(const short r, const short g, const short b);
	//��������
	~insertKeys();
private:
	Ui::insertKeysDialog ui;
	QHash<QString, QColor>* KeysMap;
};