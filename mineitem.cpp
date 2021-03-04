#include "mineitem.h"
#include "minescene.h"

MineItem::MineItem(int t_x,int t_y,QPixmap pixmap,QGraphicsItem *parent):
    QGraphicsPixmapItem(pixmap,parent),m_X(t_x),m_Y(t_y)                //列表初始化
{
    m_isMine = 0;                                                       //是否为雷
    m_isOpened = 0;                                                     //是否打开
    m_aroundMineNum = 0;                                                //周围雷数
    m_rMouseKeyNum = 0;                                                 //右键点击的次数
}

void MineItem::mousePressEvent(QGraphicsSceneMouseEvent *event)         //鼠标点击事件(右键)
{
//    1) 获取所在场景，
    MineScene *minescene = (MineScene *)this->scene();

//    2) 如果游戏已经结束或方块已经打开则返回
    if(minescene->m_isGameOver || this->m_isOpened)
        return ;

//    3) 如果是左键则设置图片
    if(event->button() == Qt::LeftButton)
        this->setPixmap(QPixmap(":/images/press.png"));

//    4) 如果是右键
    else if(event->button() == Qt::RightButton)
    {
        //    5) 右键点击次数加 1
        m_rMouseKeyNum++;
        //    6) 如果右键点击次数为 1

        if(1 == m_rMouseKeyNum)
        {
            //    7) 右键标记雷数加 1 ，设置图片为已标记，并发送重设雷数信号
            minescene->m_signedMineNum++;
            this->setPixmap(QPixmap(":/images/flag.png"));
            emit sig_resetMineNum(minescene->m_signedMineNum);

            //    8) 如果被标识方块是雷，则标记正确雷数加 1
            if(this->m_isMine)
                minescene->m_rightMineNum++;
        }
        //    9) 如果右键点 击次数为 2
        else if(2 == m_rMouseKeyNum)
        {
            //    10) 右键标记雷数减 1 ，设置图片为问号标记，并发送重设雷数信号
            minescene->m_signedMineNum--;
            this->setPixmap(QPixmap(":/images/question.png"));
            emit sig_resetMineNum(minescene->m_signedMineNum);

            //    11) 如果被标识方块是雷，则标记正确雷数减 1
            if(this->m_isMine)
                minescene->m_rightMineNum--;
        }
        //    12) 如果右键点击次数为 3
        else if(3 == m_rMouseKeyNum)
        {
            //    13) 设置方块图片为初始图片，将右键点击次数设为 0
            this->setPixmap(QPixmap(":/images/block.png"));
            m_rMouseKeyNum = 0;
        }
    }
}

void MineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)        //鼠标释放事件（左键）
{
//    1) 获取所在场景
    MineScene *minescene = (MineScene*)this->scene();

//    2) 如果游戏已经结束或方块已经打开则返回
    if(minescene->m_isGameOver || this->m_isOpened)
        return ;

//    3) 如果是左键
    if(event->button() == Qt::LeftButton)
    {
        if(1 == this->m_rMouseKeyNum)                                   //    4) 如果此方块已经被右键点击 1 次 ，则返回
            return ;

        //    5) 设置此方块已打开
        m_isOpened = true;
        if(m_isMine)                                                    //    6) 如果是雷
        {
            this->setPixmap(QPixmap(":/images/bomb.png"));              //    7) 设置为雷的图片
            if(minescene->m_soundOpen)                                  //    8) 如果 声音打开，则播放声音
                QSound::play(":/sounds/faile.wav");

            //    9) 打开所有方块，并设置游戏结束
            minescene->openAllItems();
            minescene->m_isGameOver = true;
            //    10) 弹出扫雷失败消息
            QMessageBox::information((QWidget*)minescene->parent(),tr("你输了！"),tr("下次玩简单一点的"));

            emit this->sig_restartGame();                               //    11) 发送重新游戏信号，并返回
            return ;
        }
        //    12) 如果不是雷
        else
        {
            //    13) 如果周围雷数为 0, 则设置图片，并扩散
            if(!m_aroundMineNum)
            {
                this->setPixmap(QPixmap(":/images/0.png"));
                minescene->expendWater(this);
            }
            //    14) 如果周围雷数不为 0 则设置方块图片为方块中周围雷数所对应图片
            else
            {
                QString path =  tr("://images/") + QString::number(m_aroundMineNum) + tr(".png");
                this->setPixmap(QPixmap(path));
            }
        }

        //    15) 将场景中非雷数减 1
        minescene->m_remainNoMines--;
        if(!minescene->m_remainNoMines)                 //    16) 如果场景中非雷数为 0
        {
            minescene->m_isGameOver = true;
            if(minescene->m_soundOpen)
                QSound::play(":/sounds/win.wav");       //    17) 设置游戏结束，如果声音打开，则播放声音

            minescene->openAllItems();
            minescene->sig_successPassGame();           //    18) 打开所有方块，发送场景的成功过关信号
        }
    }
}
