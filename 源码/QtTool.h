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
    //构造函数
    QtTool(QWidget *parent = nullptr);
    //读取文件
    QByteArray GetFileByteArray(const QString path);
    //另存为
    bool saveAs();
    //保存
    bool save();
    //设置文本
    void setMainEditorText(QString data);
    //设置字体和颜色
    void setFontAndColor();
    //读取保存的字体数据
    void readAndSetFontConfig(QString path);
    //读取关键字对应的颜色
    void readKeysAndColor(QString path);
    //刷新关键字颜色
    void flushKeysColor(QString& data);
    //读取语言支持
    void readAndSetLanguageSupport(const QString path);
    //刷新语言支持
    void flushLanguageSupport();
    //只编译
    bool onlyCompile();
    //只运行
    void onlyRun();
    //编译并运行
    void CompileAndRun();
    //智能提示
    void SmartTipWindow(const QString& data);
    //刷新一次颜色
    void flushTextColor();
    //刷新上一个单词
    void flushBackTextColor(QString word);
    //读取并设置智能提示
    void readAndSetSmartTipsConfig(QString path);
    //析构函数
    ~QtTool();
protected:
    //鼠标双击事件
    void mouseDoubleClickEvent(QMouseEvent* e);
    //鼠标单击事件
    void mousePressEvent(QMouseEvent* e);
    //检测
    bool eventFilter(QObject* watched, QEvent* event);
private:
    Ui::QtToolClass ui;
    QString OpenFilePath = "NullPath";  //打开的文件的路径
    bool isOpenFile = false;  //是否打开了文件
    QQueue<QString> Temp;
    QFont mainFont;
    QColor mainColor;
    size_t lastLength = 0;
    bool checked = false;
    bool quoteO = false;  //单引号
    bool quoteD = false;  //双引号
    QString tab = "    ";  //默认tab为四个空格
    QTextCursor lastCursor;  //记录checked为true时的光标位置
    QHash<QString, QColor> keysMap;  //关键字对应的颜色
    QHash<QString, Command> LanguageSupport;  //语言支持
    void* smWindow = nullptr;
    bool mutex = false;  //锁定检测输入
    bool InputFlushLock;  //强制刷新锁
};
//获取行数和列数
void getRowsAndNums(const QString& data, int* row, int* nums);