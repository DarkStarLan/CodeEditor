#include "text_replace.h"

void flush(Ui::QtToolClass& mainUI) {
	//���������ʾ��
	QString Text = mainUI.MainTextEditor->toPlainText();
	QTextCursor cursor = mainUI.MainTextEditor->textCursor();
	//��ǰ
	mainUI.row_now->setText(QString::number(cursor.blockNumber() + 1));
	mainUI.col_now->setText(QString::number(cursor.columnNumber() + 1));
	//�ܼ�
	int row, nums;
	getRowsAndNums(Text, &row, &nums);
	mainUI.row_all->setText(QString::number(row));
	mainUI.font_nums->setText(QString::number(nums));
}

//���캯��
text_replace::text_replace(QWidget *parent, Ui::QtToolClass& mainUi) : QDialog(parent), mainUI(mainUi) {
	ui.setupUi(this);

	//�滻��һ�� ��ť
	connect(ui.replace_next, &QPushButton::clicked, this, [=]() {
		QString Data = mainUI.MainTextEditor->toPlainText();
		QString check_text = ui.checkText->text();
		QString target_text = ui.targetText->text();
		int index = Data.indexOf(check_text);
		if (index == -1) QMessageBox::about(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�滻��ɣ���0��"));
		else {
			Data.replace(check_text, target_text);
			mainUI.MainTextEditor->setText(Data);  //ˢ���ı�
			QMessageBox::about(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�滻��ɣ���1��"));
		}
		flush(this->mainUI);
	});
	//�滻ȫ�� ��ť
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
		mainUI.MainTextEditor->setText(Data);  //ˢ���ı�
		QMessageBox::about(this, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�滻��ɣ���") + QString::number(t) + QString::fromLocal8Bit("��"));
		flush(this->mainUI);
	});
}
//��������
text_replace::~text_replace() { }