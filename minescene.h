/*************************************************
Copyright:Call_Me_Yeoman
Author:Yeoman
Date:2021-2-4
Description:扫雷公共数据头文件
**************************************************/
#ifndef MINESCENE_H
#define MINESCENE_H

#include <QObject>
#include <QGraphicsScene>
#include "mineitem.h"
#include "Common.h"
#include <QtGlobal>
#include <QRandomGenerator>
#include <QSound>

class MineScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit MineScene(QObject *parent = nullptr);

    QVector< QVector< MineItem *> > m_itemVec;  //存储方块的容器
    int m_scenerow;                             //当前场景行数
    int m_scenecol;                             //当前场景列数
    int m_minesum;                              //当前场景雷数
    int m_crrentleve;                           //当前场景难度级别
    int m_signedMineNum;                        //右键标记的雷数
    int m_rightMineNum;                         //右键标记正确的雷数
    bool m_isGameOver;                          //游戏是否结束
    int m_remainNoMines;                        //剩余非雷数
    bool m_soundOpen;                           //声音开关: true: 打开声音 false: 关闭声音

    void initscene();                           //初始化游戏场景
    void countAroundMines(MineItem *t_item);    //统计方块周围的雷数
    void openAllItems();                        //打开所有的方块
    void expendWater(MineItem *t_item);         //扩散函数

signals:
    void sig_successPassGame();                 //游戏成功过关信号
    void sig_scenenewGame();                    //重新开始游戏信号
    void sig_scenedisplayMineNum(int);          //显示雷数信号
};

#endif // MINESCENE_H
