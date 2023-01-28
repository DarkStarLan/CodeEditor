#include "rename_file.h"

rename_file::rename_file(QWidget *parent, QString* FileName)
	: QDialog(parent), fileName(FileName) {
	ui.setupUi(this);
	QString name = *this->fileName;
	ui.file_name->setText(name.mid(name.lastIndexOf("/") + 1, name.length() - name.lastIndexOf("/") - 1));
	//ok¼ü
	connect(ui.okButton, &QPushButton::clicked, this, [=]() {
		QFile file(*this->fileName);
		QString target = name.mid(0, name.lastIndexOf("/")) + "/" + ui.file_name->text();
		file.rename(target);
		*this->fileName = target;
	});
}
//Îö¹¹º¯Êý
rename_file::~rename_file() { }