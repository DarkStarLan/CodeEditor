#pragma once

#include <QDialog>
#include "ui_smart_tips_set.h"
#include"QtTool.h"

class smart_tips_set : public QDialog {
	Q_OBJECT
public:
	smart_tips_set(QWidget *parent, bool* IsOpenSmartTips);
	~smart_tips_set();
private:
	Ui::smartTipsSetDialog ui;
	bool* isOpenSmartTips;
};