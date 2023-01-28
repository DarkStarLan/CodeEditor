#include "QtTool.h"

//���캯��
QtTool::QtTool(QWidget *parent) : QMainWindow(parent) {
    ui.setupUi(this);
    ui.MainTextEditor->setEnabled(false);
    this->smWindow = new SmartTips(this, &this->mutex);
    installEventFilter(this);
    //��Ӳ����
    addToolBar(Qt::LeftToolBarArea, ui.mainToolBar);
    ui.mainToolBar->addWidget(ui.CommandWidget);
    //��ȡ������ʾ����
    this->readAndSetSmartTipsConfig(SMARTPATH);
    //��ȡ����֧��
    this->readAndSetLanguageSupport(LANGUAGEPATH);
    ui.LanguageCheck->setCurrentText("C++");
    //��ȡ�������������
    this->readAndSetFontConfig(FONTPATH);
    //��ȡ�ؼ��ֶ�Ӧ����ɫ
    this->readKeysAndColor(KEYSPATH);
    //�˵��� �� ��ť
    ui.actionopen->setShortcut(QKeySequence::Open);  //���ÿ�ݼ�
    connect(ui.actionopen, &QAction::triggered, this, [=]() {
        QString path = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("ѡ���ļ�"));
        QByteArray arr = this->GetFileByteArray(path);
        ui.MainTextEditor->setText(arr);
        this->lastLength = arr.length();
        this->OpenFilePath = path;
        this->isOpenFile = true;
        ui.MainTextEditor->setEnabled(true);
        this->flushTextColor();
    });
    //�˵��� �½� ��ť
    ui.actionnew->setShortcut(QKeySequence::New);  //���ÿ�ݼ�
    connect(ui.actionnew, &QAction::triggered, this, [=]() {
        //������ģ̬����
        new_file window(this, &this->OpenFilePath, &this->isOpenFile);
        //window->setAttribute(Qt::WA_DeleteOnClose);
        window.exec();
        if (this->isOpenFile) {
            ui.MainTextEditor->setEnabled(true);
            ui.MainTextEditor->setText("");
        }
    });
    //�˵��� ���� ��ť
    ui.actionsave->setShortcut(QKeySequence::Save);  //���ÿ�ݼ�
    connect(ui.actionsave, &QAction::triggered, this, [=]() { this->save(); });
    //�˵��� ���Ϊ ��ť
    ui.actionsave_as->setShortcut(QKeySequence::SaveAs);  //���ÿ�ݼ�
    connect(ui.actionsave_as, &QAction::triggered, this, [=]() { this->saveAs(); });
    //�˵��� �˳� ��ť
    ui.actionexit->setShortcut(QKeySequence::Quit);  //���ÿ�ݼ�
    connect(ui.actionexit, &QAction::triggered, this, [=]() {
        this->close();  //�˳�
        ui.MainTextEditor->setText("");
    });
    //�˵��� ���� ��ť
    ui.actionfind->setShortcut(QKeySequence::Find);  //���ÿ�ݼ�
    connect(ui.actionfind, &QAction::triggered, this, [=]() {
        text_find* window = new text_find(this, ui);
        window->setAttribute(Qt::WA_DeleteOnClose);
        window->show();
    });
    //�˵��� �滻 ��ť
    ui.actionreplace->setShortcut(QKeySequence::Replace);  //���ÿ�ݼ�
    connect(ui.actionreplace, &QAction::triggered, this, [=]() {
        text_replace* window = new text_replace(this, ui);
        window->setAttribute(Qt::WA_DeleteOnClose);
        window->show();
    });
    //�˵��� ���� ��ť
    ui.actionfont->setShortcut(QKeySequence("Ctrl+Shift+F"));  //���ÿ�ݼ�
    connect(ui.actionfont, &QAction::triggered, this, [=]() {
        text_font* window = new text_font(this, ui, &this->mainFont, &this->mainColor);
        //window->setAttribute(Qt::WA_DeleteOnClose);
        window->exec();
    });
    //�˵��� �ر� ��ť
    ui.actionclose->setShortcut(QKeySequence("Ctrl+Shift+C"));  //���ÿ�ݼ�
    connect(ui.actionclose, &QAction::triggered, this, [=]() {
        this->isOpenFile = false;
        this->OpenFilePath = "NullPath";
        ui.MainTextEditor->setText("");
        ui.MainTextEditor->setEnabled(false);
    });
    //�˵��� ������ ��ť
    ui.actionrename->setShortcut(QKeySequence("Ctrl+Shift+R"));  //���ÿ�ݼ�
    connect(ui.actionrename, &QAction::triggered, this, [=]() {
        if (this->OpenFilePath == "NullPath") return;
        rename_file file(this, &this->OpenFilePath);
        file.exec();
    });
    //�������
    connect(ui.MainTextEditor, &QTextEdit::textChanged, this, [=]() {
        if (this->mutex) return;
        QString Text = ui.MainTextEditor->toPlainText();
        if (this->Temp.size() <= 5) this->Temp.push_back(Text);
        QTextCursor cursor = ui.MainTextEditor->textCursor();
        //���������ʾ��
        //��ǰ
        ui.row_now->setText(QString::number(cursor.blockNumber() + 1));
        ui.col_now->setText(QString::number(cursor.columnNumber() + 1));
        //�ܼ�
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
        //��ȡ�ı���ַ�
        if (cursor != this->lastCursor) this->checked = false;  //���ı�
        QString lineText = ui.MainTextEditor->document()->findBlockByLineNumber(cursor.blockNumber()).text();
        QString ch;
        if (cursor.columnNumber() > 0) ch = lineText[cursor.columnNumber() - 1];
        else ch = "\n";
        //��ȡ{}����
        int fmt = 0, lin = 0;  //fmt��ʾ���˼���{}
        for (QString c : Text) {
            if (lin == cursor.blockNumber()) break;
            if (c == "{") ++fmt;
            else if (c == "}") --fmt;
            if (c == "\n") ++lin;
        }
        if (ch == "\n") {
            for (int i = 0; i < fmt; ++i) ui.MainTextEditor->insertPlainText(this->tab);
        }
        //��ȡ�ı�ĵ���
        QString word = lineText.mid(0, cursor.columnNumber());
        //word = word.mid(word.lastIndexOf(" ") + 1, word.length() - word.lastIndexOf(" "));
        //�����ַ���
        QString QSs[17] = { " ", ";", ",", "\n", "#", "&", "^", "@", "$", "(", ")", "[", "]", "{", "}", "<", ">" };
        for (int i = 0; i < 17; ++i) {
            int index = word.lastIndexOf(QSs[i]);
            if (index != -1) word = word.mid(index + 1, word.length() - index);
        }
        //������ʾ
        this->SmartTipWindow(word);  //������
        //ˢ����һ�����ʵ���ɫ
        this->flushBackTextColor(word);
        //���ܲ�ȫ
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
            return;  //���ж�����������������
        }
        if (ch == "(") {
            ui.MainTextEditor->insertPlainText(")");
            changed = true;  //��ʱ�������ʽ��
        }
        else if (ch == "[") {
            ui.MainTextEditor->insertPlainText("]");
            changed = true;  //��ʱ�������ʽ��
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
    //����
    ui.actionreturn->setShortcut(QKeySequence::Back);
    connect(ui.actionreturn, &QAction::triggered, this, [=]() {
        ui.MainTextEditor->setText(this->Temp.front());
        this->Temp.pop_front();
    });
    //ֻ����
    ui.actioncompile->setShortcut(QKeySequence("F6"));  //���ÿ�ݼ�
    connect(ui.actioncompile, &QAction::triggered, this, [this]() { this->onlyCompile(); });
    connect(ui.onlyCompile, &QPushButton::clicked, this, [this]() { this->onlyCompile(); });
    //ֻ����
    ui.actionrun->setShortcut(QKeySequence("F10"));  //���ÿ�ݼ�
    connect(ui.actionrun, &QAction::triggered, this, [this]() { this->onlyRun(); });
    connect(ui.onlyRun, &QPushButton::clicked, this, [this]() { this->onlyRun(); });
    //���벢����
    ui.actioncompileAndRun->setShortcut(QKeySequence("F5"));  //���ÿ�ݼ�
    connect(ui.actioncompileAndRun, &QAction::triggered, this, [this]() { this->CompileAndRun(); });
    connect(ui.compileAndRun, &QPushButton::clicked, this, [this]() { this->CompileAndRun(); });
    //�˵��� ���ùؼ��� ��ť
    ui.actionset_keys->setShortcut(QKeySequence("Ctrl+Shift+K"));  //���ÿ�ݼ�
    connect(ui.actionset_keys, &QAction::triggered, this, [this]() {
        insertKeys window(this, &this->keysMap);
        window.exec();
    });
    //�˵��� ����֧���� ��ť
    ui.actionset_support->setShortcut(QKeySequence("Ctrl+Shift+L"));  //���ÿ�ݼ�
    connect(ui.actionset_support, &QAction::triggered, this, [this]() {
        support window(this, &this->LanguageSupport);
        window.exec();
        this->flushLanguageSupport();
    });
    //�˵��� ������ʾ ��ť
    ui.actionsmart_tips->setShortcut(QKeySequence("Ctrl+Shift+S"));  //���ÿ�ݼ�
    connect(ui.actionsmart_tips, &QAction::triggered, this, [this]() {
        smart_tips_set window(this, &this->InputFlushLock);
        window.exec();
    });
}
//��ȡ�ļ�
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
//���Ϊ
bool QtTool::saveAs() {
    this->flushTextColor();  //ˢ��һ����ɫ
    QString Load = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("ѡ��"));
    if (Load.isEmpty()) return false;
    QString Path = Load + "/" + "Untitled.txt";
    QFile file(Path);
    file.open(QIODevice::ReadWrite | QIODevice::Text);  //���ļ�
    file.write(ui.MainTextEditor->toPlainText().toUtf8());  //д������
    file.close();  //�ر��ļ�
    this->OpenFilePath = Path;
    return true;
}
//����
bool QtTool::save() {
    if (this->OpenFilePath == "NullPath") {  //δ���ù�·��
        return this->saveAs();
    }
    this->flushTextColor();  //ˢ��һ����ɫ
    QFile file(this->OpenFilePath);
    file.open(QIODevice::WriteOnly | QIODevice::Text);  //���ļ�
    file.write(ui.MainTextEditor->toPlainText().toUtf8());  //д������
    file.close();  //�ر��ļ�
    if (this->OpenFilePath == "NullPath") return false;
    return true;
}
//�����ı�
void QtTool::setMainEditorText(QString data) {
    ui.MainTextEditor->setText(data);
    QFont mainFont;
    QColor mainColor;
}
//�����������ɫ
void QtTool::setFontAndColor() {
    ui.MainTextEditor->setFont(this->mainFont);
    ui.MainTextEditor->setTextColor(this->mainColor);
}
//��ȡ�������������
void QtTool::readAndSetFontConfig(QString path) {
    QFileInfo fi(path);
    if (fi.isFile()) {  //����ļ�����
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        QDataStream ifs(&file);
        ifs >> this->mainFont;
        ifs >> this->mainColor;
        file.close();
        this->setFontAndColor();
    }
}
//��ȡ�ؼ��ֶ�Ӧ����ɫ
void QtTool::readKeysAndColor(QString path) {
    QFileInfo fi(path);
    if (fi.isFile()) {  //����ļ�����
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        QDataStream ifs(&file);
        ifs >> this->keysMap;
        file.close();
    }
}
//ˢ�¹ؼ�����ɫ
void QtTool::flushKeysColor(QString& data) {
    //ui.MainTextEditor->setTextColor(QColor(255, 0, 0));
}
//��ȡ����������
void getRowsAndNums(const QString& data, int* row, int* nums) {
    size_t len = data.length();
    int i = 0, j = 0;
    for (; i < len; ++i) {
        if (data[i] == '\n') ++j;
    }
    *row = j + 1;
    *nums = len - j;
}
//��ȡ����֧��
void QtTool::readAndSetLanguageSupport(const QString path) {
    QFileInfo fi(path);
    if (fi.isFile()) {
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        QDataStream fin(&file);
        fin >> this->LanguageSupport;  //��ȡ��ϣ��
        file.close();
    }
    for (auto it = this->LanguageSupport.constKeyValueBegin(), end = this->LanguageSupport.constKeyValueEnd(); it != end; ++it) {
        ui.LanguageCheck->addItem((*it).first);
    }
}
//ˢ������֧��
void QtTool::flushLanguageSupport() {
    ui.LanguageCheck->clear();  //���
    for (auto it = this->LanguageSupport.constKeyValueBegin(), end = this->LanguageSupport.constKeyValueEnd(); it != end; ++it) {
        ui.LanguageCheck->addItem((*it).first);
    }
}
//ֻ����
bool QtTool::onlyCompile() {
    Command CMD = this->LanguageSupport[ui.LanguageCheck->currentText()];
    QString path = CMD.Path, leftCMD = CMD.Left, rightCMD = CMD.Right;
    //�����ļ�
    if (!this->save()) return false;
    //����·��
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
    //�޸�bat�ļ�
    ofs << data;
    file.close();
    //����bat
    QString command = "\"" + cmd + "\"";
    system(command.toUtf8().constData());
    return true;
    //QProcess process(nullptr);
    //process.start("\"E:/QT01/QtWidgetsApplication 01/Debug/support/cpp.bat\"");
    //qDebug() << "\"" + cmd + "\"";
    //process.waitForFinished();
}
//ֻ����
void QtTool::onlyRun() {
    if (!this->save()) return;
    if (ui.LanguageCheck->currentText() == "Python") this->onlyCompile();
    else if (ui.LanguageCheck->currentText() == "Java") this->onlyCompile();
    else system("start main.exe");
}
//���벢����
void QtTool::CompileAndRun() {
    bool b = false;
    if (ui.LanguageCheck->currentText() != "Python" && ui.LanguageCheck->currentText() != "Java") b = this->onlyCompile();
    if (b) this->onlyRun();
}
//������ʾ
void QtTool::SmartTipWindow(const QString& data) {
    SmartTips* SP = (SmartTips*)(this->smWindow);
    if (data == "") {
        SP->ui.listWidget->clear();  //���
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
    SP->ui.listWidget->clear();  //���
    ui.listWidget->clear();
    QList<QString> Keys;
    for (auto i : this->keysMap.keys()) {
        if (i.indexOf(data) != -1) {
            Keys.append(i);
            b = true;
        }
    }
    Keys.sort();  //����
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
//��������
QtTool::~QtTool() { }
//���˫���¼�
void QtTool::mouseDoubleClickEvent(QMouseEvent* e) {
    this->isOpenFile = true;
    ui.MainTextEditor->setEnabled(true);
}
//��굥���¼�
void QtTool::mousePressEvent(QMouseEvent* e) {
    ((SmartTips*)(this->smWindow))->hide();
}
//ˢ��һ����ɫ
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
            //��ӹؼ���
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
//ˢ����һ������
void QtTool::flushBackTextColor(QString word) {
    bool b = this->keysMap.contains(word);
    this->mutex = true;
    int len = word.length();
    QTextCursor cursor = ui.MainTextEditor->textCursor();
    while (len--) cursor.deletePreviousChar();  //ɾ���������
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
//��ȡ������������ʾ
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
//���
bool QtTool::eventFilter(QObject* watched, QEvent* event) {
    if (event->type() == QEvent::KeyRelease) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        SmartTips* SP = (SmartTips*)(this->smWindow);
        if (keyEvent->key() == Qt::Key_Escape) return QWidget::eventFilter(watched, event);
        if (!SP->isHidden() && keyEvent->key() == Qt::Key_Down) {
            ui.MainTextEditor->clearFocus();
            SP->activateWindow();
            SP->ui.listWidget->setFocus();
            //�����ƶ����
            //tmpCursor = ui.MainTextEditor->textCursor();
            //tmpCursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, 1);
            //ui.MainTextEditor->setTextCursor(tmpCursor);
        }
    }
    return QWidget::eventFilter(watched, event);
}