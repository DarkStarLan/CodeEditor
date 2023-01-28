#include "SmartTips.h"

//构造函数
SmartTips::SmartTips(QWidget *parent, bool* Mutex)
	: QDialog(parent), tool(static_cast<QtTool*>(parent)), mutex(Mutex) {
	ui.setupUi(this);
	this->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::Popup);
	this->setFixedSize(241, 161);
	installEventFilter(this);
}
//事件
bool SmartTips::eventFilter(QObject* watched, QEvent* event) {
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
		if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
			*this->mutex = true;
			QtTool* QT = (QtTool*)(this->tool);
			QString txt = ui.listWidget->currentItem()->text();
			QTextEdit* TextEditor = QT->ui.MainTextEditor;
			//删除多余字符
			for (int i = 0; i < this->len; ++i) TextEditor->textCursor().deletePreviousChar();
			//调整颜色
			QTextCharFormat temp;
			temp.setForeground(QT->mainColor);
			temp.setFont(QT->mainFont);
			QTextCharFormat fmt;
			fmt.setForeground(QT->keysMap.value(txt));
			fmt.setFont(QT->mainFont);
			QT->ui.MainTextEditor->setCurrentCharFormat(fmt);
			TextEditor->insertPlainText(txt);
			QT->ui.MainTextEditor->setCurrentCharFormat(temp);
			//其他
			this->hide();
			this->clearFocus();
			QT->activateWindow();
			QT->ui.MainTextEditor->setFocus();
			*this->mutex = false;
			//QT->lastCursor = QT->ui.MainTextEditor->textCursor();  //刷新光标
		}
	}
	return QWidget::eventFilter(watched, event);
}
//析构函数
SmartTips::~SmartTips() { }