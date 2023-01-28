#include "text_find.h"

//���캯��
text_find::text_find(QWidget *parent, Ui::QtToolClass& mainUi) : QDialog(parent), mainUI(mainUi) {
	ui.setupUi(this);
	//��һ�� ��ť
	connect(ui.okButton, &QPushButton::clicked, this, [=]() {
		QString Data = mainUI.MainTextEditor->toPlainText();
		QString targetText = ui.lineEdit->text();  //��Ҫ���ҵ�����
		int index = Data.indexOf(targetText), t = 0;
		while(index != -1) {  //�����ֵĴ���
			++t;
			index = Data.indexOf(targetText, index + targetText.length());
		}
		QMessageBox::about(this, QString::fromLocal8Bit("�������"), QString::fromLocal8Bit("������") + QString::number(t) + QString::fromLocal8Bit("��"));
	});
}
//��������
text_find::~text_find() { }