#include "QtTool.h"

//构造函数
QtTool::QtTool(QWidget *parent) : QMainWindow(parent) {
    ui.setupUi(this);
    ui.MainTextEditor->setEnabled(false);
    this->smWindow = new SmartTips(this, &this->mutex);
    installEventFilter(this);
    //添加侧边栏
    addToolBar(Qt::LeftToolBarArea, ui.mainToolBar);
    ui.mainToolBar->addWidget(ui.CommandWidget);
    //读取智能提示设置
    this->readAndSetSmartTipsConfig(SMARTPATH);
    //读取语言支持
    this->readAndSetLanguageSupport(LANGUAGEPATH);
    ui.LanguageCheck->setCurrentText("C++");
    //读取保存的字体数据
    this->readAndSetFontConfig(FONTPATH);
    //读取关键字对应的颜色
    this->readKeysAndColor(KEYSPATH);
    //菜单栏 打开 按钮
    ui.actionopen->setShortcut(QKeySequence::Open);  //设置快捷键
    connect(ui.actionopen, &QAction::triggered, this, [=]() {
        QString path = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择文件"));
        QByteArray arr = this->GetFileByteArray(path);
        ui.MainTextEditor->setText(arr);
        this->lastLength = arr.length();
        this->OpenFilePath = path;
        this->isOpenFile = true;
        ui.MainTextEditor->setEnabled(true);
        this->flushTextColor();
    });
    //菜单栏 新建 按钮
    ui.actionnew->setShortcut(QKeySequence::New);  //设置快捷键
    connect(ui.actionnew, &QAction::triggered, this, [=]() {
        //弹出新模态窗口
        new_file window(this, &this->OpenFilePath, &this->isOpenFile);
        //window->setAttribute(Qt::WA_DeleteOnClose);
        window.exec();
        if (this->isOpenFile) {
            ui.MainTextEditor->setEnabled(true);
            ui.MainTextEditor->setText("");
        }
    });
    //菜单栏 保存 按钮
    ui.actionsave->setShortcut(QKeySequence::Save);  //设置快捷键
    connect(ui.actionsave, &QAction::triggered, this, [=]() { this->save(); });
    //菜单栏 另存为 按钮
    ui.actionsave_as->setShortcut(QKeySequence::SaveAs);  //设置快捷键
    connect(ui.actionsave_as, &QAction::triggered, this, [=]() { this->saveAs(); });
    //菜单栏 退出 按钮
    ui.actionexit->setShortcut(QKeySequence::Quit);  //设置快捷键
    connect(ui.actionexit, &QAction::triggered, this, [=]() {
        this->close();  //退出
        ui.MainTextEditor->setText("");
    });
    //菜单栏 查找 按钮
    ui.actionfind->setShortcut(QKeySequence::Find);  //设置快捷键
    connect(ui.actionfind, &QAction::triggered, this, [=]() {
        text_find* window = new text_find(this, ui);
        window->setAttribute(Qt::WA_DeleteOnClose);
        window->show();
    });
    //菜单栏 替换 按钮
    ui.actionreplace->setShortcut(QKeySequence::Replace);  //设置快捷键
    connect(ui.actionreplace, &QAction::triggered, this, [=]() {
        text_replace* window = new text_replace(this, ui);
        window->setAttribute(Qt::WA_DeleteOnClose);
        window->show();
    });
    //菜单栏 字体 按钮
    ui.actionfont->setShortcut(QKeySequence("Ctrl+Shift+F"));  //设置快捷键
    connect(ui.actionfont, &QAction::triggered, this, [=]() {
        text_font* window = new text_font(this, ui, &this->mainFont, &this->mainColor);
        //window->setAttribute(Qt::WA_DeleteOnClose);
        window->exec();
    });
    //菜单栏 关闭 按钮
    ui.actionclose->setShortcut(QKeySequence("Ctrl+Shift+C"));  //设置快捷键
    connect(ui.actionclose, &QAction::triggered, this, [=]() {
        this->isOpenFile = false;
        this->OpenFilePath = "NullPath";
        ui.MainTextEditor->setText("");
        ui.MainTextEditor->setEnabled(false);
    });
    //菜单栏 重命名 按钮
    ui.actionrename->setShortcut(QKeySequence("Ctrl+Shift+R"));  //设置快捷键
    connect(ui.actionrename, &QAction::triggered, this, [=]() {
        if (this->OpenFilePath == "NullPath") return;
        rename_file file(this, &this->OpenFilePath);
        file.exec();
    });
    //检测输入
    connect(ui.MainTextEditor, &QTextEdit::textChanged, this, [=]() {
        if (this->mutex) return;
        QString Text = ui.MainTextEditor->toPlainText();
        if (this->Temp.size() <= 5) this->Temp.push_back(Text);
        QTextCursor cursor = ui.MainTextEditor->textCursor();
        //设置左侧显示框
        //当前
        ui.row_now->setText(QString::number(cursor.blockNumber() + 1));
        ui.col_now->setText(QString::number(cursor.columnNumber() + 1));
        //总计
        int row, nums;
        getRowsAndNums(Text, &row, &nums);
        ui.row_all->setText(QString::number(row));
        ui.font_nums->setText(QString::number(nums));
        if (Text.isEmpty()) {
            this->setFontAndColor();
            return;
        }
        /*if (Text.length() < this->lastLength) {
            this->lastLength = Text.length();
            return;
        }*/
        this->lastLength = Text.length();
        bool changed = false;
        //获取改变的字符
        if (cursor != this->lastCursor) this->checked = false;  //光标改变
        QString lineText = ui.MainTextEditor->document()->findBlockByLineNumber(cursor.blockNumber()).text();
        QString ch;
        if (cursor.columnNumber() > 0) ch = lineText[cursor.columnNumber() - 1];
        else ch = "\n";
        //获取{}层数
        int fmt = 0, lin = 0;  //fmt表示套了几层{}
        for (QString c : Text) {
            if (lin == cursor.blockNumber()) break;
            if (c == "{") ++fmt;
            else if (c == "}") --fmt;
            if (c == "\n") ++lin;
        }
        if (ch == "\n") {
            for (int i = 0; i < fmt; ++i) ui.MainTextEditor->insertPlainText(this->tab);
        }
        //获取改变的单词
        QString word = lineText.mid(0, cursor.columnNumber());
        //word = word.mid(word.lastIndexOf(" ") + 1, word.length() - word.lastIndexOf(" "));
        //过滤字符串
        QString QSs[17] = { " ", ";", ",", "\n", "#", "&", "^", "@", "$", "(", ")", "[", "]", "{", "}", "<", ">" };
        for (int i = 0; i < 17; ++i) {
            int index = word.lastIndexOf(QSs[i]);
            if (index != -1) word = word.mid(index + 1, word.length() - index);
        }
        //智能提示
        this->SmartTipWindow(word);  //会检测锁
        //刷新上一个单词的颜色
        this->flushBackTextColor(word);
        //智能补全
        if (this->checked && ch == "\n") {
            this->checked = false;
            QTextCursor cursor = ui.MainTextEditor->textCursor();
            QString c = ui.MainTextEditor->document()->findBlockByLineNumber(cursor.blockNumber())
                .text()[cursor.columnNumber()];
            //for (int i = 1; i < fmt; ++i) ui.MainTextEditor->insertPlainText(this->tab);
            this->mutex = true;
            ui.MainTextEditor->insertPlainText("\n");
            this->mutex = false;
            for (int i = 1; i < fmt; ++i) ui.MainTextEditor->insertPlainText(this->tab);
            cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1 + (--fmt * 4));
            ui.MainTextEditor->setTextCursor(cursor);
            return;  //不判断连续输入左右括号
        }
        if (ch == "(") {
            ui.MainTextEditor->insertPlainText(")");
            changed = true;  //暂时不允许格式化
        }
        else if (ch == "[") {
            ui.MainTextEditor->insertPlainText("]");
            changed = true;  //暂时不允许格式化
        }
        else if (ch == "{") {
            ui.MainTextEditor->insertPlainText("}");
            changed = true;
            this->checked = true;
        }
        /*
        if (!this->quoteD && ch == "\"") {
            ui.MainTextEditor->insertPlainText("\"");
            this->quoteD = true;
        }
        else this->quoteD = false;
        if (!this->quoteO && ch == "\'") {
            ui.MainTextEditor->insertPlainText("\'");
            this->quoteO = true;
        }
        else this->quoteO = false;
        */
        if (changed) {
            QTextCursor tmpCursor = ui.MainTextEditor->textCursor();
            tmpCursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, 1);
            ui.MainTextEditor->setTextCursor(tmpCursor);
            this->lastCursor = tmpCursor;
        }
    });
    //撤销
    ui.actionreturn->setShortcut(QKeySequence::Back);
    connect(ui.actionreturn, &QAction::triggered, this, [=]() {
        ui.MainTextEditor->setText(this->Temp.front());
        this->Temp.pop_front();
    });
    //只编译
    ui.actioncompile->setShortcut(QKeySequence("F6"));  //设置快捷键
    connect(ui.actioncompile, &QAction::triggered, this, [this]() { this->onlyCompile(); });
    connect(ui.onlyCompile, &QPushButton::clicked, this, [this]() { this->onlyCompile(); });
    //只运行
    ui.actionrun->setShortcut(QKeySequence("F10"));  //设置快捷键
    connect(ui.actionrun, &QAction::triggered, this, [this]() { this->onlyRun(); });
    connect(ui.onlyRun, &QPushButton::clicked, this, [this]() { this->onlyRun(); });
    //编译并运行
    ui.actioncompileAndRun->setShortcut(QKeySequence("F5"));  //设置快捷键
    connect(ui.actioncompileAndRun, &QAction::triggered, this, [this]() { this->CompileAndRun(); });
    connect(ui.compileAndRun, &QPushButton::clicked, this, [this]() { this->CompileAndRun(); });
    //菜单栏 设置关键字 按钮
    ui.actionset_keys->setShortcut(QKeySequence("Ctrl+Shift+K"));  //设置快捷键
    connect(ui.actionset_keys, &QAction::triggered, this, [this]() {
        insertKeys window(this, &this->keysMap);
        window.exec();
    });
    //菜单栏 管理支持项 按钮
    ui.actionset_support->setShortcut(QKeySequence("Ctrl+Shift+L"));  //设置快捷键
    connect(ui.actionset_support, &QAction::triggered, this, [this]() {
        support window(this, &this->LanguageSupport);
        window.exec();
        this->flushLanguageSupport();
    });
    //菜单栏 智能提示 按钮
    ui.actionsmart_tips->setShortcut(QKeySequence("Ctrl+Shift+S"));  //设置快捷键
    connect(ui.actionsmart_tips, &QAction::triggered, this, [this]() {
        smart_tips_set window(this, &this->InputFlushLock);
        window.exec();
    });
}
//读取文件
QByteArray QtTool::GetFileByteArray(const QString path) {
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray data;
    while (!file.atEnd()) {
        data += file.readLine();
    }
    file.close();
    return data;
}
//另存为
bool QtTool::saveAs() {
    this->flushTextColor();  //刷新一遍颜色
    QString Load = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("选择"));
    if (Load.isEmpty()) return false;
    QString Path = Load + "/" + "Untitled.txt";
    QFile file(Path);
    file.open(QIODevice::ReadWrite | QIODevice::Text);  //打开文件
    file.write(ui.MainTextEditor->toPlainText().toUtf8());  //写入数据
    file.close();  //关闭文件
    this->OpenFilePath = Path;
    return true;
}
//保存
bool QtTool::save() {
    if (this->OpenFilePath == "NullPath") {  //未设置过路径
        return this->saveAs();
    }
    this->flushTextColor();  //刷新一遍颜色
    QFile file(this->OpenFilePath);
    file.open(QIODevice::WriteOnly | QIODevice::Text);  //打开文件
    file.write(ui.MainTextEditor->toPlainText().toUtf8());  //写入数据
    file.close();  //关闭文件
    if (this->OpenFilePath == "NullPath") return false;
    return true;
}
//设置文本
void QtTool::setMainEditorText(QString data) {
    ui.MainTextEditor->setText(data);
    QFont mainFont;
    QColor mainColor;
}
//设置字体和颜色
void QtTool::setFontAndColor() {
    ui.MainTextEditor->setFont(this->mainFont);
    ui.MainTextEditor->setTextColor(this->mainColor);
}
//读取保存的字体数据
void QtTool::readAndSetFontConfig(QString path) {
    QFileInfo fi(path);
    if (fi.isFile()) {  //如果文件存在
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        QDataStream ifs(&file);
        ifs >> this->mainFont;
        ifs >> this->mainColor;
        file.close();
        this->setFontAndColor();
    }
}
//读取关键字对应的颜色
void QtTool::readKeysAndColor(QString path) {
    QFileInfo fi(path);
    if (fi.isFile()) {  //如果文件存在
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        QDataStream ifs(&file);
        ifs >> this->keysMap;
        file.close();
    }
}
//刷新关键字颜色
void QtTool::flushKeysColor(QString& data) {
    //ui.MainTextEditor->setTextColor(QColor(255, 0, 0));
}
//获取行数和列数
void getRowsAndNums(const QString& data, int* row, int* nums) {
    size_t len = data.length();
    int i = 0, j = 0;
    for (; i < len; ++i) {
        if (data[i] == '\n') ++j;
    }
    *row = j + 1;
    *nums = len - j;
}
//读取语言支持
void QtTool::readAndSetLanguageSupport(const QString path) {
    QFileInfo fi(path);
    if (fi.isFile()) {
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        QDataStream fin(&file);
        fin >> this->LanguageSupport;  //读取哈希表
        file.close();
    }
    for (auto it = this->LanguageSupport.constKeyValueBegin(), end = this->LanguageSupport.constKeyValueEnd(); it != end; ++it) {
        ui.LanguageCheck->addItem((*it).first);
    }
}
//刷新语言支持
void QtTool::flushLanguageSupport() {
    ui.LanguageCheck->clear();  //清空
    for (auto it = this->LanguageSupport.constKeyValueBegin(), end = this->LanguageSupport.constKeyValueEnd(); it != end; ++it) {
        ui.LanguageCheck->addItem((*it).first);
    }
}
//只编译
bool QtTool::onlyCompile() {
    Command CMD = this->LanguageSupport[ui.LanguageCheck->currentText()];
    QString path = CMD.Path, leftCMD = CMD.Left, rightCMD = CMD.Right;
    //保存文件
    if (!this->save()) return false;
    //设置路径
    QString cmd = qApp->applicationDirPath() + "/" + path;
    QFile file(cmd);
    file.open(QIODevice::ReadWrite | QIODevice::Truncate);
    QTextStream ofs(&file);
    ofs.setCodec("ASCII");
    QString data = leftCMD + this->OpenFilePath + rightCMD;
    if (ui.LanguageCheck->currentText() == "Java") {
        data = "javac " + this->OpenFilePath;
        ofs << data;
        file.close();
        QString command = "\"" + cmd + "\"";
        system(command.toUtf8().constData());
        command = "java " + this->OpenFilePath.mid(0, this->OpenFilePath.lastIndexOf(".class"));
        system(command.toUtf8().constData());
        return true;
    }
    //修改bat文件
    ofs << data;
    file.close();
    //运行bat
    QString command = "\"" + cmd + "\"";
    system(command.toUtf8().constData());
    return true;
    //QProcess process(nullptr);
    //process.start("\"E:/QT01/QtWidgetsApplication 01/Debug/support/cpp.bat\"");
    //qDebug() << "\"" + cmd + "\"";
    //process.waitForFinished();
}
//只运行
void QtTool::onlyRun() {
    if (!this->save()) return;
    if (ui.LanguageCheck->currentText() == "Python") this->onlyCompile();
    else if (ui.LanguageCheck->currentText() == "Java") this->onlyCompile();
    else system("start main.exe");
}
//编译并运行
void QtTool::CompileAndRun() {
    bool b = false;
    if (ui.LanguageCheck->currentText() != "Python" && ui.LanguageCheck->currentText() != "Java") b = this->onlyCompile();
    if (b) this->onlyRun();
}
//智能提示
void QtTool::SmartTipWindow(const QString& data) {
    SmartTips* SP = (SmartTips*)(this->smWindow);
    if (data == "") {
        SP->ui.listWidget->clear();  //清空
        ui.listWidget->clear();
        SP->hide();
        return;
    }
    bool b = false;
    SP->len = data.length();
    //SmartTips* smartWindow = new SmartTips(this);
    QPoint here = mapToGlobal(ui.MainTextEditor->pos());
    QFontMetrics fm(ui.MainTextEditor->font());
    SP->move(here.x() + (ui.col_now->text().toInt() - 1) * fm.width('0'), here.y() + (ui.row_now->text().toInt() + 1) * fm.height());
    SP->setAttribute(Qt::WA_DeleteOnClose);
    SP->ui.listWidget->clear();  //清空
    ui.listWidget->clear();
    QList<QString> Keys;
    for (auto i : this->keysMap.keys()) {
        if (i.indexOf(data) != -1) {
            Keys.append(i);
            b = true;
        }
    }
    Keys.sort();  //排序
    int flag = true;
    for (auto i : Keys) {
        QListWidgetItem* item_in = new QListWidgetItem(i);
        ui.listWidget->addItem(item_in);
        item_in->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        if (!this->InputFlushLock) {
            QListWidgetItem* item = new QListWidgetItem(i);
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            SP->ui.listWidget->addItem(item);
            if (flag) {
                SP->ui.listWidget->setCurrentItem(item);
                flag = false;
            }
        }
    }
    if (!this->InputFlushLock) {
        if (b) SP->show();
        else SP->hide();
    }
}
//析构函数
QtTool::~QtTool() { }
//鼠标双击事件
void QtTool::mouseDoubleClickEvent(QMouseEvent* e) {
    this->isOpenFile = true;
    ui.MainTextEditor->setEnabled(true);
}
//鼠标单击事件
void QtTool::mousePressEvent(QMouseEvent* e) {
    ((SmartTips*)(this->smWindow))->hide();
}
//刷新一次颜色
void QtTool::flushTextColor() {
    this->mutex = true;
    QString Data = ui.MainTextEditor->toPlainText();
    int posL = 0, posR = 0, len = Data.length(), t = 0, last = 0;
    ui.MainTextEditor->setText("");
    QString QSs[17] = { " ", "\n", ";", ",", "#", "&", "^", "@", "$", "(", ")", "[", "]", "{", "}", "<", ">" };
    int pos = Data.indexOf(' ');
    for (int i = 0; i < 17; ++i) {
        int index = Data.indexOf(QSs[i]);
        if ((pos == -1 || pos > index) && index != -1) pos = index;
    }
    posR = pos;
    while (posR != -1) {
        QString text = Data.mid(posL, posR - posL);
        if (text != "") {
            ui.MainTextEditor->insertPlainText(Data.mid(last, posL - last));
            last = posL + text.length();
            //添加关键字
            if (this->keysMap.contains(text)) {
                QTextCharFormat temp;
                temp.setForeground(this->mainColor);
                temp.setFont(this->mainFont);
                QTextCharFormat fmt;
                fmt.setForeground(this->keysMap.value(text));
                fmt.setFont(this->mainFont);
                ui.MainTextEditor->setCurrentCharFormat(fmt);
                ui.MainTextEditor->insertPlainText(text);
                ui.MainTextEditor->setCurrentCharFormat(temp);
            }
            else ui.MainTextEditor->insertPlainText(text);
        }
        posL = posR + 1;
        int pos = Data.indexOf(' ', posR + 1);
        for (int i = 0; i < 17; ++i) {
            int index = Data.indexOf(QSs[i], posR + 1);
            if ((pos == -1 || pos > index) && index != -1) pos = index;
        }
        posR = pos;
    }
    ui.MainTextEditor->insertPlainText(Data.mid(last, len - last));
    this->mutex = false;
}
//刷新上一个单词
void QtTool::flushBackTextColor(QString word) {
    bool b = this->keysMap.contains(word);
    this->mutex = true;
    int len = word.length();
    QTextCursor cursor = ui.MainTextEditor->textCursor();
    while (len--) cursor.deletePreviousChar();  //删除这个单词
    QTextCharFormat temp;
    temp.setForeground(this->mainColor);
    temp.setFont(this->mainFont);
    if (b) {
        QTextCharFormat fmt;
        fmt.setForeground(this->keysMap.value(word));
        fmt.setFont(this->mainFont);
        ui.MainTextEditor->setCurrentCharFormat(fmt);
    }
    ui.MainTextEditor->insertPlainText(word);
    if (b) ui.MainTextEditor->setCurrentCharFormat(temp);
    this->mutex = false;
}
//读取并设置智能提示
void QtTool::readAndSetSmartTipsConfig(QString path) {
    QFileInfo fi(path);
    if (fi.isFile()) {
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        QDataStream fin(&file);
        fin >> this->InputFlushLock;
        file.close();
    }
    else {
        QFile file(path);
        file.open(QIODevice::ReadWrite);
        file.close();
    }
}
//检测
bool QtTool::eventFilter(QObject* watched, QEvent* event) {
    if (event->type() == QEvent::KeyRelease) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        SmartTips* SP = (SmartTips*)(this->smWindow);
        if (keyEvent->key() == Qt::Key_Escape) return QWidget::eventFilter(watched, event);
        if (!SP->isHidden() && keyEvent->key() == Qt::Key_Down) {
            ui.MainTextEditor->clearFocus();
            SP->activateWindow();
            SP->ui.listWidget->setFocus();
            //向上移动光标
            //tmpCursor = ui.MainTextEditor->textCursor();
            //tmpCursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, 1);
            //ui.MainTextEditor->setTextCursor(tmpCursor);
        }
    }
    return QWidget::eventFilter(watched, event);
}