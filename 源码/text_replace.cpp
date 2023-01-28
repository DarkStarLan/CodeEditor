#include "text_replace.h"

void flush(Ui::QtToolClass& mainUI) {
	//设置左侧显示框
	QString Text = mainUI.MainTextEditor->toPlainText();
	QTextCursor cursor = mainUI.MainTextEditor->textCursor();
	//当前
	mainUI.row_now->setText(QString::number(cursor.blockNumber() + 1));
	mainUI.col_now->setText(QString::number(cursor.columnNumber() + 1));
	//总计
	int row, nums;
	getRowsAndNums(Text, &row, &nums);
	mainUI.row_all->setText(QString::number(row));
	mainUI.font_nums->setText(QString::number(nums));
}

//构造函数
text_replace::text_replace(QWidget *parent, Ui::QtToolClass& mainUi) : QDialog(parent), mainUI(mainUi) {
	ui.setupUi(this);

	//替换下一个 按钮
	connect(ui.replace_next, &QPushButton::clicked, this, [=]() {
		QString Data = mainUI.MainTextEditor->toPlainText();
		QString check_text = ui.checkText->text();
		QString target_text = ui.targetText->text();
		int index = Data.indexOf(check_text);
		if (index == -1) QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("替换完成，共0处"));
		else {
			Data.replace(check_text, target_text);
			mainUI.MainTextEditor->setText(Data);  //刷新文本
			QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("替换完成，共1处"));
		}
		flush(this->mainUI);
	});
	//替换全部 按钮
	connect(ui.replace_all, &QPushButton::clicked, this, [=]() {
		QString Data = mainUI.MainTextEditor->toPlainText();
		QString check_text = ui.checkText->text();
		QString target_text = ui.targetText->text();
		int index = Data.indexOf(check_text), t = 0;
		while (index != -1) {
			++t;
			index = Data.indexOf(check_text, index + check_text.length());
		}
		Data.replace(check_text, target_text);
		mainUI.MainTextEditor->setText(Data);  //刷新文本
		QMessageBox::about(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("替换完成，共") + QString::number(t) + QString::fromLocal8Bit("处"));
		flush(this->mainUI);
	});
}
//析构函数
text_replace::~text_replace() { }