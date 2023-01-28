#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtTool.h"
#include"Command.h"
#include<qstring.h>
#include<qmessagebox.h>
#include<qpushbutton.h>
#include<qfiledialog.h>
#include<qdialog.h>
#include<qtextcursor.h>
#include<qdebug.h>
#include<qtextdocument.h>
#include<QTextBlock>
#include<qfile.h>
#include<qdatastream.h>
#include<qhash.h>
#include<qprocess.h>
#include<qtextstream.h>
#include<qqueue.h>
#include"new_file.h"
#include"text_find.h"
#include"text_replace.h"
#include"text_font.h"
#include"rename_file.h"
#include"SmartTips.h"
#include"insertKeys.h"
#include"support.h"
#include"smart_tips_set.h"

const QString FONTPATH = "font.config";
const QString KEYSPATH = "keys.config";
const QString LANGUAGEPATH = "languages.config";
const QString SMARTPATH = "smart.config";

class QtTool : public QMainWindow {
    Q_OBJECT
    friend class SmartTips;
public:
    //���캯��
    QtTool(QWidget *parent = nullptr);
    //��ȡ�ļ�
    QByteArray GetFileByteArray(const QString path);
    //���Ϊ
    bool saveAs();
    //����
    bool save();
    //�����ı�
    void setMainEditorText(QString data);
    //�����������ɫ
    void setFontAndColor();
    //��ȡ�������������
    void readAndSetFontConfig(QString path);
    //��ȡ�ؼ��ֶ�Ӧ����ɫ
    void readKeysAndColor(QString path);
    //ˢ�¹ؼ�����ɫ
    void flushKeysColor(QString& data);
    //��ȡ����֧��
    void readAndSetLanguageSupport(const QString path);
    //ˢ������֧��
    void flushLanguageSupport();
    //ֻ����
    bool onlyCompile();
    //ֻ����
    void onlyRun();
    //���벢����
    void CompileAndRun();
    //������ʾ
    void SmartTipWindow(const QString& data);
    //ˢ��һ����ɫ
    void flushTextColor();
    //ˢ����һ������
    void flushBackTextColor(QString word);
    //��ȡ������������ʾ
    void readAndSetSmartTipsConfig(QString path);
    //��������
    ~QtTool();
protected:
    //���˫���¼�
    void mouseDoubleClickEvent(QMouseEvent* e);
    //��굥���¼�
    void mousePressEvent(QMouseEvent* e);
    //���
    bool eventFilter(QObject* watched, QEvent* event);
private:
    Ui::QtToolClass ui;
    QString OpenFilePath = "NullPath";  //�򿪵��ļ���·��
    bool isOpenFile = false;  //�Ƿ�����ļ�
    QQueue<QString> Temp;
    QFont mainFont;
    QColor mainColor;
    size_t lastLength = 0;
    bool checked = false;
    bool quoteO = false;  //������
    bool quoteD = false;  //˫����
    QString tab = "    ";  //Ĭ��tabΪ�ĸ��ո�
    QTextCursor lastCursor;  //��¼checkedΪtrueʱ�Ĺ��λ��
    QHash<QString, QColor> keysMap;  //�ؼ��ֶ�Ӧ����ɫ
    QHash<QString, Command> LanguageSupport;  //����֧��
    void* smWindow = nullptr;
    bool mutex = false;  //�����������
    bool InputFlushLock;  //ǿ��ˢ����
};
//��ȡ����������
void getRowsAndNums(const QString& data, int* row, int* nums);