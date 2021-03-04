/*************************************************
Copyright:Call_Me_Yeoman
Author:Yeoman
Date:2021-2-4
Description:扫雷游戏自定义数据头文件
**************************************************/
#ifndef CUSTOMGAMEDIALOG_H
#define CUSTOMGAMEDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QSpinBox>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QSettings>

class CustomGameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CustomGameDialog(QWidget *parent = nullptr);

    QSpinBox *colspinbox;                                           //列分量框
    QSpinBox *rowspinbox;                                           //行分量框
    QSpinBox *minenumspinbox;                                       //雷数分量框

    void readsettings();                                            //读自定义游戏设置文件
    void writesettings();                                           //写自定义游戏设置文件

signals:
    void signal_sendCustomset(int,int,int);                         //发送自定义游戏行、列、雷数的信号

public slots:
    void slot_accpetok();                                           //接收确定按钮的槽
};

#endif // CUSTOMGAMEDIALOG_H
