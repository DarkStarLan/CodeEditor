#pragma once

#include <QDialog>
#include"ui_SmartTips.h"
#include"QtTool.h"
#include<qmouseeventtransition.h>

class SmartTips : public QDialog {
	Q_OBJECT
public:
	SmartTips(QWidget *parent, bool* Mutex);
	~SmartTips();
	Ui::SmartTipsDialog ui;
	size_t len = 1;  //�����ַ����ĳ���
protected:
	//��굥���¼�
	bool SmartTips::eventFilter(QObject* watched, QEvent* event);
	void* tool;
	bool* mutex;
};