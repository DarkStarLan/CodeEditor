#include "smart_tips_set.h"

smart_tips_set::smart_tips_set(QWidget *parent, bool* IsOpenSmartTips)
	: QDialog(parent), isOpenSmartTips(IsOpenSmartTips) {
	ui.setupUi(this);
	ui.openSmartTips->setChecked(!(*this->isOpenSmartTips));
	connect(ui.okButton, &QPushButton::clicked, this, [this]() {
		QFile file(SMARTPATH);
		file.open(QIODevice::WriteOnly);
		QDataStream fo(&file);
		fo << (!ui.openSmartTips->isChecked());  //ѡ�оͲ�����
		*isOpenSmartTips = !ui.openSmartTips->isChecked();
		file.close();
	});
}
//��������
smart_tips_set::~smart_tips_set() { }