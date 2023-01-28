#include "text_font.h"

//构造函数
text_font::text_font(QWidget* parent, Ui::QtToolClass& mainUi, QFont* font, QColor* color)
	: QDialog(parent), mainUI(mainUi), Font(font), Color(color) {
	ui.setupUi(this);

	ui.font_name->setText(this->Font->family());
	ui.font_size->setText(QString::number(this->Font->pointSize()));
	if (this->Font->bold()) ui.font_style->setText(QString::fromLocal8Bit("[粗体]"));
	if (this->Font->italic()) ui.font_style->setText(ui.font_style->text() + QString::fromLocal8Bit("[斜体]"));
	int r = 0, g = 0, b = 0;
	this->Color->getRgb(&r, &g, &b);
	ui.font_color->setText("#" + QString::number(r, 16) + QString::number(g, 16) + QString::number(b, 16));
	//选择字体 按钮
	connect(ui.check_button, &QPushButton::clicked, this, [this]() {
		bool ok;
		QFont font = QFontDialog::getFont(&ok, QFont("Consolas", 18), this, QString::fromLocal8Bit("选择字体"));
		ui.font_name->setText(font.family());
		ui.font_size->setText(QString::number(font.pointSize()));
		if (ok) ui.font_style->setText("");  //清空样式属性
		if (font.bold()) ui.font_style->setText(QString::fromLocal8Bit("[粗体]"));
		if (font.italic()) ui.font_style->setText(ui.font_style->text() + QString::fromLocal8Bit("[斜体]"));
		*this->Font = font;  //刷新字体
	});
	//选择颜色 按钮
	connect(ui.color_button, &QPushButton::clicked, this, [this]() {
		QColor color = QColorDialog::getColor(*this->Color, this, QString::fromLocal8Bit("选择颜色"));
		int r = 0, g = 0, b = 0;
		color.getRgb(&r, &g, &b);
		ui.font_color->setText("#" + QString::number(r, 16) + QString::number(g, 16) + QString::number(b, 16));
		*this->Color = color;
	});
	//确定 按钮
	connect(ui.okButton, &QPushButton::clicked, this, [this]() {
		mainUI.MainTextEditor->setFont(*this->Font);
		mainUI.MainTextEditor->setTextColor(*this->Color);
		mainUI.MainTextEditor->setText(mainUI.MainTextEditor->toPlainText());
		QFile file("font.config");
		file.open(QIODevice::WriteOnly | QIODevice::Truncate);
		QDataStream ofs(&file);
		ofs.setVersion(QDataStream::Qt_5_14);
		ofs << *Font;
		ofs << *Color;
		file.close();
	});
}
//析构函数
text_font::~text_font() { }