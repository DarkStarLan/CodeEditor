#pragma once

#include <QDialog>
#include "ui_rename_file.h"
#include"QtTool.h"

class rename_file : public QDialog {
	Q_OBJECT
public:
	rename_file(QWidget* parent, QString* FileName);
	~rename_file();
private:
	Ui::renameFileDialog ui;
	QString* fileName;
};
