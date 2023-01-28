#include "new_File.h"

new_file::new_file(QWidget* parent, QString* openFilePath, bool* IsOpenFile, QString defaultText)
    : QDialog(parent), OpenFilePath(openFilePath), isOpenFile(IsOpenFile) {
    ui.setupUi(this);
    setWindowTitle(QString::fromLocal8Bit("新建"));
    ui.fileNameLine->setText(defaultText);
    //选择路径按钮
    connect(ui.checkLoad, &QPushButton::clicked, this, [=]() {
        QString path = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("选择"));
        ui.savePathLine->setText(path);
    });
    //创建按钮
    connect(ui.okButton, &QPushButton::clicked, this, [=]() {
        QString fileName = ui.savePathLine->text();
        QString path = fileName +"/" + ui.fileNameLine->text();
        if (fileName.isEmpty()) {
            QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("文件名不能为空"), QMessageBox::Ok);
            this->show();
            return;
        }
        QFile file(path);  //设置路径
        bool exist = QFile::exists(path);
        if (exist) {
            QMessageBox::critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("文件已经存在"));
            this->show();
            return;
        }
        file.open(QIODevice::ReadWrite);  //创建文件
        *this->OpenFilePath = path;
        *this->isOpenFile = true;
        file.close();
    });
}
//析构函数
new_file::~new_file() { }