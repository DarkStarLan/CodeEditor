#include "insertKeys.h"

//构造函数
insertKeys::insertKeys(QWidget *parent, QHash<QString, QColor>* keys)
	: QDialog(parent), KeysMap(keys) {
	ui.setupUi(this);
	for (auto i : this->KeysMap->keys()) {
		QListWidgetItem* item = new QListWidgetItem(i);
		item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable);
		ui.listWidget->addItem(item);
	}
	ui.listWidget->sortItems();
	ui.listWidget->setItemSelected(ui.listWidget->itemAt(0, 0), true);
	//颜色
	ui.spinR->setMinimum(0);
	ui.spinG->setMinimum(0);
	ui.spinB->setMinimum(0);
	ui.spinR->setMaximum(255);
	ui.spinG->setMaximum(255);
	ui.spinB->setMaximum(255);
	ui.spinR->setValue(0);
	ui.spinG->setValue(0);
	ui.spinB->setValue(0);
	//拷贝一份哈希表
	QHash<QString, QColor> keysMap = *(this->KeysMap);
	//添加按钮
	connect(ui.addButton, &QPushButton::clicked, this, [&]() {
		bool ok = false;
		QString name = QInputDialog::getText(this, QString::fromLocal8Bit("添加"), QString::fromLocal8Bit("关键字:"), QLineEdit::Normal, "newKey", &ok);
		if (!ok) return;
		if (name.isEmpty() || keysMap.count(name)) {
			QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("关键字重复或为空"));
			return;
		}
		QListWidgetItem* item = new QListWidgetItem(name);
		item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		ui.listWidget->addItem(item);
		ui.listWidget->setItemSelected(item, true);
		keysMap.insert(name, QColor(0, 0, 0));
		this->setColorShow(0, 0, 0);
	});
	//删除按钮
	connect(ui.deleteButton, &QPushButton::clicked, this, [&]() {
		if (keysMap.count(ui.listWidget->currentItem()->text()) == 0) return;
		keysMap.remove(ui.listWidget->currentItem()->text());
		delete ui.listWidget->currentItem();
	});
	//改变选择
	connect(ui.listWidget, &QListWidget::currentItemChanged, this, [&]() {
		int r, g, b;
		keysMap.value(ui.listWidget->currentItem()->text()).getRgb(&r, &g, &b);
		ui.spinR->setValue(r);
		ui.spinG->setValue(g);
		ui.spinB->setValue(b);
		//设置背景颜色
		this->setColorShow(r, g, b);
	});
	//值改变
	connect(ui.spinR, &QSpinBox::textChanged, this, [&]() {
		this->setColorShow(ui.spinR->value(), ui.spinG->value(), ui.spinB->value());
	});
	connect(ui.spinG, &QSpinBox::textChanged, this, [&]() {
		this->setColorShow(ui.spinR->value(), ui.spinG->value(), ui.spinB->value());
	});
	connect(ui.spinB, &QSpinBox::textChanged, this, [&]() {
		this->setColorShow(ui.spinR->value(), ui.spinG->value(), ui.spinB->value());
	});
	//选择颜色
	connect(ui.checkColorButton, &QPushButton::clicked, this, [&]() {
		QColor color = QColorDialog::getColor(QColor(ui.spinR->value(), ui.spinG->value(),
			ui.spinB->value()), this, QString::fromLocal8Bit("选择"));
		int r, b, g;
		color.getRgb(&r, &g, &b);
		this->setColorShow(r, g, b);
		ui.spinR->setValue(r);
		ui.spinG->setValue(g);
		ui.spinB->setValue(b);
	});
	//保存颜色
	connect(ui.saveColorButton, &QPushButton::clicked, this, [&]() {
		int r = ui.spinR->value();
		int g = ui.spinG->value();
		int b = ui.spinB->value();
		keysMap[ui.listWidget->currentItem()->text()].setRgb(r, g, b);
		this->setColorShow(r, g, b);
	});
	//确认按钮
	connect(ui.okButton, &QPushButton::clicked, this, [&]() {
		*this->KeysMap = keysMap;
		QFile file(KEYSPATH);
		file.open(QIODevice::WriteOnly | QIODevice::Truncate);
		QDataStream fo(&file);
		fo << (*this->KeysMap);
		file.close();
	});
}
//显示颜色
void insertKeys::setColorShow(const short r, const short g, const short b) {
	QPalette pal(QColor(r, g, b));
	ui.ColorShow->setAutoFillBackground(true);
	ui.ColorShow->setPalette(pal);
}
//析构函数
insertKeys::~insertKeys() { }