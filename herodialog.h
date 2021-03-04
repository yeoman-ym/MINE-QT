/*************************************************
Copyright:Call_Me_Yeoman
Author:Yeoman
Date:2021-2-4
Description:扫雷英雄排行榜头文件
**************************************************/
#ifndef HERODIALOG_H
#define HERODIALOG_H
#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QSettings>
#include <QPoint>
#include <QSize>
#include <QDebug>

class HeroDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HeroDialog(QWidget *parent = nullptr);

    QLabel *lowrecordtimelabel;     //初级纪录时间
    QLabel *middlerecordtimelabel;  //中级纪录时间
    QLabel *heightrecordtimelabel;  //高级纪录时间
    QLabel *lowrecordnamelabel;     //初级纪录姓名
    QLabel *middlerecordnamelabel;  //中级纪录姓名
    QLabel *heightrecordnamelabel;  //高级纪录姓名

    void readsettings();            //读英雄榜数据
    void init_herorecord();         //写英雄榜数据

public slots:
    void slot_reset();              //重置英雄榜数据
};

#endif // HERODIALOG_H
