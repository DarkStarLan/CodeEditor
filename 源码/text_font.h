#pragma once

#include <QDialog>
#include "ui_text_font.h"
#include"QtTool.h"
#include<qfontdialog.h>
#include<qcolordialog.h>

class text_font : public QDialog {
	Q_OBJECT
public:
	//���캯��
	text_font(QWidget *parent, Ui::QtToolClass& mainUi, QFont* mainFont, QColor* mainColor);
	//��������
	~text_font();
private:
	Ui::textFontDialog ui;
	Ui::QtToolClass mainUI;
	QFont* Font;
	QColor* Color;
};