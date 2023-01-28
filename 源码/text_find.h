#pragma once

#include <QDialog>
#include "ui_text_find.h"
#include"QtTool.h"

class text_find : public QDialog {
	Q_OBJECT
public:
	//构造函数
	text_find(QWidget *parent, Ui::QtToolClass& mainUi);
	//析构函数
	~text_find();
private:
	Ui::textFindDialog ui;
	Ui::QtToolClass mainUI;
};