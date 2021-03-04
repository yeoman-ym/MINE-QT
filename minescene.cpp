/*************************************************
Copyright:Call_Me_Yeoman
Author:Yeoman
Date:2021-2-4
Description:扫雷场景
**************************************************/
#include "minescene.h"

QPoint t_around[8] =
{
    QPoint(-1,-1),
    QPoint(-1,0),
    QPoint(-1,1),
    QPoint(0,-1),
    QPoint(0,1),
    QPoint(1,-1),
    QPoint(1,0),
    QPoint(1,1)
};

MineScene::MineScene(QObject *parent) : QGraphicsScene(parent)
{   
    m_isGameOver = false;                           //设置游戏未结束
    m_signedMineNum = 0;                            //右键标记的雷数
    m_rightMineNum = 0;                             //右键标记正确的雷数
    m_soundOpen = true;                             //声音开关: true: 打开声音 false: 关闭声音
}

void MineScene::initscene()                         //初始化游戏场景
{
    //1) 初始化非雷数＝方块总数-总雷数
    this->m_remainNoMines = m_scenerow * m_scenecol - m_minesum;

    //2) 根据当前行，列循环创建方块并加入行 vector 和场景中，并将行vector加入到总vector 中
    for(int i = 0; i < m_scenerow; i++)
    {
        QVector <MineItem*> t_vec;
        for(int j = 0; j < m_scenecol; j++)
        {
            MineItem *t_item = new MineItem(i, j, QPixmap("://images/block.png"));
            t_item->setPos(j * COMMON::MAPWIDTH, i * COMMON::MAPHEIGHT);
            this->addItem(t_item);
            t_vec.push_back(t_item);
        }
        this->m_itemVec.push_back(t_vec);
    }

    //3) 根据雷数循环随机布雷 srand(time(NULL)); qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));___老方法
    for(int i = 0; i < m_minesum; i++)
    {
        //4) 随机产生一个坐标
        //        int t_rows = qrand() % m_scenerow;
        int t_rows = QRandomGenerator::global()->bounded(m_scenerow);   //生成一个0到m_scenerow之间的数
        //        int t_cols = qrand() % m_scenecol;
        int t_cols = QRandomGenerator::global()->bounded(m_scenecol);

        //5) 如果该方块不是雷，则设置其为雷，并将周围八个方块上的周围雷数加 1
        if(!this->m_itemVec[t_rows][t_cols]->m_isMine)
        {
            this->m_itemVec[t_rows][t_cols]->m_isMine = true;
            this->countAroundMines(this->m_itemVec[t_rows][t_cols]);
        }
        else
            i--;
    }

    //6) 将所有方块的重新开始游戏信号连接到场景的重新开始游戏信号
    for(int i = 0; i < m_scenerow; i++)
    {
        for(int j = 0; j < m_scenecol; j++)
        {
            this->connect(this->m_itemVec[i][j],SIGNAL(sig_restartGame()),this,SIGNAL(sig_scenenewGame()));
            //7) 将所有方块的重新设置雷数信号连接到场景的重新显示雷数信号
            this->connect(this->m_itemVec[i][j],SIGNAL(sig_resetMineNum(int)),this,SIGNAL(sig_scenedisplayMineNum(int)));
        }
    }
}

void MineScene::countAroundMines(MineItem *t_item) //统计方块周围的雷数
{
    //1) 如果为空或者不是雷，则返回
    if(NULL == t_item || !t_item->m_isMine)
        return;

    //2) 循环查找 8 个方位
    for(int i = 0; i < 8; i++)
    {
        //3) 计算所找方块在容器中的行，列
        int t_x = t_item->m_X + t_around[i].x();
        int t_y = t_item->m_Y + t_around[i].y();
        //4) 如果越界 则重新下一次循环
        if(t_x < 0 || t_x >= this->m_scenerow || t_y < 0 || t_y >= this->m_scenecol)
            continue;

        //5) 如果所找方块是雷，则重新下一次循环
        if(m_itemVec[t_x][t_y]->m_isMine)
            continue;

        //6) 所找方块的周围雷数加 1
        m_itemVec[t_x][t_y]->m_aroundMineNum++;
    }
}

void MineScene::openAllItems()                      //打开所有的方块
{
    if(this->m_isGameOver)
        return;

    //1) 循环遍历所有方块
    for(int i = 0; i < m_scenerow; i++)
    {
        for(int j = 0; j < m_scenecol; j++)
        {
            //2) 将方块设置为打 开
            m_itemVec[i][j]->m_isOpened = true;
            if(m_itemVec[i][j]->m_isMine)   //3) 如果是雷则将方块图片设置为雷图片
                m_itemVec[i][j]->setPixmap(QPixmap("://images/mine.png"));
            else
            {
                //4) 如果不是雷则将方块图片设置为方块周围雷数所对应图片
                QString path = tr("://images/") + QString::number(m_itemVec[i][j]->m_aroundMineNum) + tr(".png");
                m_itemVec[i][j]->setPixmap(path);
            }
        }
    }
}

void MineScene::expendWater(MineItem *t_item)       //扩散函数
{
//    1) 如果为空，如果是雷则返回
      if(NULL == t_item || t_item->m_isMine)
          return;

//    2) 循环查找 8 个方位
      for(int i = 0; i < 8; i++)
      {
          int t_x = t_item->m_X + t_around[i].x();  //3) 计算所找方块在容器中的行，列
          int t_y = t_item->m_Y + t_around[i].y();

          //    4) 如果越界如果越界,,则重新下一次循环则重新下一次循环
          if(t_x < 0 || t_x >= this->m_scenerow || t_y < 0 || t_y >= m_scenecol)
              continue;

          //    5) 如果所找方块是雷或者已打开，则重新下一次循环如果所找方块是雷或者已打开，则重新下一次循环
          else if(m_itemVec[t_x][t_y]->m_isMine || m_itemVec[t_x][t_y]->m_isOpened)
              continue;

          //    6) 如果所找方块的已经右键点击如果所找方块的已经右键点击
          else if(m_itemVec[t_x][t_y]->m_rMouseKeyNum > 0)
              continue;

          //    7) 设置所找方块已打开，当前剩余非雷数减剩余非雷数减1
          m_itemVec[t_x][t_y]->m_isOpened = true;
          this->m_remainNoMines--;

          //    8) 如果所找方块周围雷数为如果所找方块周围雷数为00，则设置相应图片，并递归查找，则设置相应图片，并递归查找
          int itemnum = m_itemVec[t_x][t_y]->m_aroundMineNum;
          if(0 == itemnum)
              expendWater(m_itemVec[t_x][t_y]);
          //    9) 如果所找方块周围雷数不为如果所找方块周围雷数不为0,0,则设置所找方块的图片为所找方块的周围雷数对应图片则设置所找方块的图片为所找方块的周围雷
          QString path = tr("://images/") + QString::number(m_itemVec[t_x][t_y]->m_aroundMineNum) + tr(".png");
          m_itemVec[t_x][t_y]->setPixmap(path);
      }

      //    10) 如果当前剩余非雷数为如果当前剩余非雷数为0
      if(0 == m_remainNoMines)
      {
          m_isGameOver = true;
          //    11) 设置当前游戏结束，如果声音打开，则播放声音设置当前游戏结束，如果声音打开，则播放声音
          if(m_soundOpen)
              QSound::play(":/sounds/win.wav");
          this->openAllItems();         //    12) 打开所有雷打开所有雷
          emit sig_successPassGame();   //    13) 发送成功过关的信号发送成功过关的信号;
      }
}
