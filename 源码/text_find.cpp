#include "text_find.h"

//构造函数
text_find::text_find(QWidget *parent, Ui::QtToolClass& mainUi) : QDialog(parent), mainUI(mainUi) {
	ui.setupUi(this);
	//下一个 按钮
	connect(ui.okButton, &QPushButton::clicked, this, [=]() {
		QString Data = mainUI.MainTextEditor->toPlainText();
		QString targetText = ui.lineEdit->text();  //需要查找的内容
		int index = Data.indexOf(targetText), t = 0;
		while(index != -1) {  //检测出现的次数
			++t;
			index = Data.indexOf(targetText, index + targetText.length());
		}
		QMessageBox::about(this, QString::fromLocal8Bit("查找完成"), QString::fromLocal8Bit("共出现") + QString::number(t) + QString::fromLocal8Bit("次"));
	});
}
//析构函数
text_find::~text_find() { }