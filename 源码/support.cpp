#include "support.h"

support::support(QWidget *parent, QHash<QString, Command>* lspt)
	: QDialog(parent), LSpt(lspt) {
	ui.setupUi(this);
	for (auto i : this->LSpt->keys()) {
		QListWidgetItem* item = new QListWidgetItem(i);
		item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		ui.listWidget->addItem(item);
	}
	ui.listWidget->sortItems();
	ui.listWidget->setItemSelected(ui.listWidget->itemAt(0, 0), true);
	//����һ�ݹ�ϣ��
	QHash<QString, Command> LSPT = *(this->LSpt);
	//��Ӱ�ť
	connect(ui.addButton, &QPushButton::clicked, this, [&]() {
		bool ok = false;
		QString name = QInputDialog::getText(this, QString::fromLocal8Bit("���"), QString::fromLocal8Bit("����:"), QLineEdit::Normal, "new", &ok);
		if (!ok) return;
		if (name.isEmpty() || LSPT.count(name)) {
			QMessageBox::critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�ؼ����ظ���Ϊ��"));
			return;
		}
		QListWidgetItem* item = new QListWidgetItem(name);
		item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		ui.listWidget->addItem(item);
		ui.listWidget->setItemSelected(item, true);
		LSPT.insert(name, Command("start ", "", "support/" + name + ".bat"));
	});
	//ɾ����ť
	connect(ui.deleteButton, &QPushButton::clicked, this, [&]() {
		LSPT.remove(ui.listWidget->currentItem()->text());
		delete ui.listWidget->currentItem();
	});
	//�ı�ѡ��
	connect(ui.listWidget, &QListWidget::currentItemChanged, this, [&]() {
		Command cmd = LSPT.value(ui.listWidget->currentItem()->text());
		ui.loadEditor->setText(cmd.Path);
		ui.batLeft->setText(cmd.Left);
		ui.batRight->setText(cmd.Right);
	});
	//ѡ��·����ť
	connect(ui.checkLoadButton, &QPushButton::clicked, this, [this]() {
		QString Load = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("ѡ���ļ�"));
		ui.loadEditor->setText(Load);
	});
	//���水ť
	connect(ui.saveButton, &QPushButton::clicked, this, [&]() {
		Command cmd(ui.batLeft->text(), ui.batRight->text(), ui.loadEditor->text());
		LSPT.insert(ui.listWidget->currentItem()->text(), cmd);
	});
	//�½������ť
	connect(ui.newBatButton, &QPushButton::clicked, this, [this]() {
		QString Load;
		bool isOpen = false;
		new_file window(this, &Load, &isOpen, "new.bat");
		window.exec();
		if (!isOpen) return;
		ui.loadEditor->setText(Load);
	});
	//ȷ����ť
	connect(ui.okButton, &QPushButton::clicked, this, [&]() {
		qDebug() << LSPT;
		*this->LSpt = LSPT;
		QFile file(LANGUAGEPATH);
		file.open(QIODevice::WriteOnly | QIODevice::Truncate);
		QDataStream fo(&file);
		fo << (*this->LSpt);
		file.close();
	});
}
//��������
support::~support() { }