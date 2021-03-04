/*************************************************
Copyright:Call_Me_Yeoman
Author:Yeoman
Date:2020-12-10
Description:扫雷程序主菜单头文件
**************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGraphicsView>
#include <QLCDNumber>
#include "minescene.h"
#include <QAction>
#include <QActionGroup>
#include <QMenuBar>
#include <QMenu>
#include <QTimer>
#include "Common.h"
#include "customgamedialog.h"
#include "herodialog.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QSound>
#include <QDebug>
#include <QColorDialog>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QAction *newaction;                             //新建行为
    QAction *lowleveaction;                         //低级行为
    QAction *middleleveaction;                      //中级行为
    QAction *heightleveaction;                      //高级行为
    QAction *customaction;                          //自定义行为
    QAction *coloraction;                           //颜色行为
    QAction *soundaction;                           //声音行为
    QAction *heroaction;                            //英雄榜行为
    QAction *exitaction;                            //退出行为
    QAction *aboutaction;                           //关于行为

    QWidget *centralget;                            //主程序界面mainwindow->setcellwidget(centralget);
    QGraphicsView *mineView;                        //主程序视图
    MineScene *minescene;                           //主程序场景
    QLCDNumber *minenumLcd;                         //雷数LCD
    QLCDNumber *timeLcd;                            //时间LCD
    QPushButton *smilebtn;                          //笑脸按钮
    int m_time;                                     //当前用时数
    QTimer *m_timer;                                //定时器

    void create_action();                           //创建行为
    void create_menu();                             //创建菜单
    void readsettings();                            //读当前游戏设置
    void writesettings();                           //写当前游戏设置

public slots:
    void slot_newgame();                            //开始新游戏槽
    void slot_newgamebyleve(QAction *);             //点击游戏级别行为开始新游戏槽
    void slot_acceptCutsomvale(int,int,int);        //接收自定义游戏设置槽
    void slot_colorchanged();                       //设置颜色的槽
    void slot_soundchanged();                       //设置声音的槽
    void slot_herochecked();                        //显示英雄榜的槽
    void slot_about();                              //显示about扫雷的槽
    void slot_updatehero();                         //更新英雄榜的槽
    void slot_displayMineNum(int);                  //显示雷数的槽
    void slot_displayTime();                        //显示时间的槽
};
#endif // MAINWINDOW_H
