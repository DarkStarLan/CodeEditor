#pragma once

#include <QDialog>
#include "ui_text_replace.h"
#include"QtTool.h"

class text_replace : public QDialog {
	Q_OBJECT
public:
	//���캯��
	text_replace(QWidget* parent, Ui::QtToolClass& mainUi);
	//��������
	~text_replace();
private:
	Ui::textReplaceDialog ui;
	Ui::QtToolClass mainUI;
};
