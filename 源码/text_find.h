#pragma once

#include <QDialog>
#include "ui_text_find.h"
#include"QtTool.h"

class text_find : public QDialog {
	Q_OBJECT
public:
	//���캯��
	text_find(QWidget *parent, Ui::QtToolClass& mainUi);
	//��������
	~text_find();
private:
	Ui::textFindDialog ui;
	Ui::QtToolClass mainUI;
};