/*************************************************
Copyright:Call_Me_Yeoman
Author:Yeoman
Date:2020-12-10
Description:扫雷鼠标操作相关头文件
**************************************************/
#ifndef MINEITEM_H
#define MINEITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QMessageBox>

class MineItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit MineItem(int t_x,int t_y,QPixmap pixmap,QGraphicsItem *parent = nullptr);

    int m_X;                                                        //在容器中的行数
    int m_Y;                                                        //在容器中的列数
    bool m_isMine;                                                  //是否为雷
    bool m_isOpened;                                                //是否打开
    int m_aroundMineNum;                                            //周围雷数
    int m_rMouseKeyNum;                                             //右键点击的次数

    void mousePressEvent(QGraphicsSceneMouseEvent *event);          //鼠标点击事件(右键)
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);        //鼠标释放事件(左键)

signals:
    void sig_restartGame();                                         //重新开始游戏信号
    void sig_resetMineNum(int t_signedMineNum);                     //重设雷数信号
};

#endif // MINEITEM_H
