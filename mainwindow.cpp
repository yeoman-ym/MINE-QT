/*************************************************
Copyright:Call_Me_Yeoman
Author:Yeoman
Date:2021-2-4
Description:扫雷菜单
**************************************************/
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),centralget(0),mineView(0),minescene(0),minenumLcd(0),timeLcd(0),smilebtn(0),m_time(0),m_timer(0)
{
    this->setWindowTitle("扫雷");
    //应用图标设置，在关于里面才显示
    this->setWindowIcon(QIcon("://images/steam.icns"));

    this->create_action();
    this->create_menu();
    this->slot_newgame();
}

void MainWindow::create_action()       //创建行为
{
    //新游戏行为
    newaction = new QAction(tr("开 战(&F)"),this);
    this->connect(newaction,SIGNAL(triggered()),this,SLOT(slot_newgame()));

    //低中高自定义级行为
    lowleveaction = new QAction(tr("初级(&L)"),this);
    lowleveaction->setCheckable(true);
    lowleveaction->setChecked(true);
    middleleveaction = new QAction(tr("中级(&M)"),this);
    middleleveaction->setCheckable(true);
    heightleveaction = new QAction(tr("高级(&H)"),this);
    heightleveaction->setCheckable(true);
    customaction = new QAction(tr("自定义(&P)"),this);
    customaction->setCheckable(true);

    //创建行为组
    QActionGroup *actionGroup = new QActionGroup(this);
    actionGroup->addAction(lowleveaction);
    actionGroup->addAction(middleleveaction);
    actionGroup->addAction(heightleveaction);
    actionGroup->addAction(customaction);
    this->connect(actionGroup,SIGNAL(triggered(QAction*)),this,SLOT(slot_newgamebyleve(QAction*)));

    coloraction = new QAction(tr("颜 色(&C)"),this);
    coloraction->setCheckable(true);
    coloraction->setChecked(true);
    this->connect(coloraction,SIGNAL(triggered(bool)),this,SLOT(slot_colorchanged()));

    soundaction = new QAction(tr("声 音(&S)"),this);
    soundaction->setCheckable(true);
    soundaction->setChecked(true);
    this->connect(soundaction,SIGNAL(triggered(bool)),this,SLOT(slot_soundchanged()));

    heroaction = new QAction(tr("英雄榜(&R)"),this);
    heroaction->setCheckable(true);
    this->connect(heroaction,SIGNAL(triggered(bool)),this,SLOT(slot_herochecked()));

    exitaction = new QAction(tr("退 出(&X)"),this);
    this->connect(exitaction,SIGNAL(triggered(bool)),this,SLOT(close()));

    aboutaction = new QAction(tr("关 于(&A)"),this);
    this->connect(aboutaction,SIGNAL(triggered(bool)),this,SLOT(slot_about()));
}

void MainWindow::create_menu()         //创建菜单
{
    QMenuBar *menu_bar = this->menuBar();
    QMenu *file_menu = menu_bar->addMenu(tr("菜 单"));
    QMenu *help_menu = menu_bar->addMenu(tr("帮 助"));

    //菜单添加行为
    file_menu->addAction(newaction);
    file_menu->addSeparator();

    file_menu->addAction(lowleveaction);
    file_menu->addAction(middleleveaction);
    file_menu->addAction(heightleveaction);
    file_menu->addAction(customaction);
    file_menu->addSeparator();

    file_menu->addAction(coloraction);
    file_menu->addAction(soundaction);
    file_menu->addAction(heroaction);
    file_menu->addSeparator();

    file_menu->addAction(exitaction);
    help_menu->addAction(aboutaction);
}

//函数功能：读当前游戏设置，并设置到场景的行，列，雷数，级别
void MainWindow::readsettings()        //读当前游戏设置
{
    QSettings *setting = new QSettings("YEOMAN","mine",this);
    setting->beginGroup("currentleve");
    minescene->m_crrentleve = setting->value("currentleve").toInt();
    minescene->m_scenerow = setting->value("currentrownum").toInt();
    minescene->m_scenecol = setting->value("currentcolnum").toInt();
    minescene->m_minesum = setting->value("currentminenum").toInt();
    setting->endGroup();
}

//函数功能：写当前游戏设置，将当前场景的行，列，雷数，级别写入到注册表
void MainWindow::writesettings()       //写当前游戏设置
{
    QSettings *setting = new QSettings("YEOMAN","mine",this);
    setting->beginGroup("currentleve");
    setting->setValue("currentleve",minescene->m_crrentleve);
    setting->setValue("currentrownum",minescene->m_scenerow);
    setting->setValue("currentcolnum",minescene->m_scenecol);
    setting->setValue("currentminenum",minescene->m_minesum);
    setting->endGroup();
}

void MainWindow::slot_newgame()        //开始新游戏槽
{
    //1) 清理旧对象 deleteLater();
    centralget->deleteLater();
    mineView->deleteLater();
    minescene->deleteLater();
    minenumLcd->deleteLater();
    timeLcd->deleteLater();
    smilebtn->deleteLater();
    m_timer->deleteLater();

    //2) 创建主界面对象
    centralget = new QWidget(this);

    //3) 创建调色板，雷数 LCD ，时间 LCD ，并设置 LCD 上的颜色
    QPalette textPalette;
    textPalette.setColor(QPalette::Normal, QPalette::WindowText, Qt::red);
    textPalette.setColor(QPalette::Normal, QPalette::Window, Qt::blue);
    minenumLcd = new QLCDNumber(centralget);
    minenumLcd->setPalette(textPalette);
    minenumLcd->setDigitCount(3);

    timeLcd = new QLCDNumber(centralget);
    timeLcd->setPalette(textPalette);
    timeLcd->setDigitCount(3);

    //4) 创建笑脸按钮，并设置图片，并将信 号连接到开始新游戏槽
    smilebtn = new QPushButton(centralget);
    smilebtn->setIcon(QPixmap("://images/smile.png"));
    this->connect(smilebtn,SIGNAL(clicked(bool)),this,SLOT(slot_newgame()));

    //5) 创建水平布局，将雷数LCD,笑脸按钮,时间LCD,加入到水平布局
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(minenumLcd);
    hlayout->addStretch();
    hlayout->addWidget(smilebtn);
    hlayout->addStretch();
    hlayout->addWidget(timeLcd);

    //6) 创建主视图，主场景，将场景加入到视图，并读取游戏当前设置
    mineView = new QGraphicsView(centralget);    //视图
    minescene = new MineScene(mineView);         //场景
    mineView->setScene(minescene);
    this->readsettings();

    //7) 将场景的开始新游戏信号连接到开始新游戏槽，将场景的显示雷数信号连接到显示雷数的槽，将场景成功过关信号连接到更新英雄榜的槽
    this->connect(minescene,SIGNAL(sig_scenenewGame()),this,SLOT(slot_newgame()));
    this->connect(minescene,SIGNAL(sig_scenedisplayMineNum(int)),this,SLOT(slot_displayMineNum(int)));
    this->connect(minescene,SIGNAL(sig_successPassGame()),this,SLOT(slot_updatehero()));

    //8) 创建垂直布局，并将水平布局和主视图加入到垂直布局，将主界面设置应用程序主界面
    QVBoxLayout *vlayout = new QVBoxLayout(centralget);
    vlayout->addLayout(hlayout);
    vlayout->addWidget(mineView);
    this->setCentralWidget(centralget);

    //9) 根据图片大小和行，列数据设置场景，主视图，主窗口大小
    minescene->setSceneRect(0,0,minescene->m_scenecol * COMMON::MAPWIDTH, minescene->m_scenerow * COMMON::MAPHEIGHT);
    mineView->setFixedSize(minescene->m_scenecol * COMMON::MAPWIDTH + 5,minescene->m_scenerow * COMMON::MAPHEIGHT + 5);
    this->setFixedSize(minescene->m_scenecol * COMMON::MAPWIDTH + 30,minescene->m_scenerow * COMMON::MAPHEIGHT + 80);

    //10) 根据当前场景的游戏级别设置相应的级别行为被选中
    if(minescene->m_crrentleve == COMMON::LOWLEVE)
        lowleveaction->setChecked(true);
    else if(minescene->m_crrentleve == COMMON::MIDDLELEVE)
        middleleveaction->setChecked(true);
    else if(minescene->m_crrentleve == COMMON::HEIGHTLEVE)
        heightleveaction->setChecked(true);
    else if(minescene->m_crrentleve == COMMON::CUSOMLEVE)
        customaction->setChecked(true);

    //11) LCD 显示雷数和时间（初始为 0
    m_time = 0;
    minenumLcd->display(minescene->m_minesum);
    timeLcd->display(m_time);

    //12) 创建定时器，将超时信号连接到显 示时间槽
    m_timer = new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(slot_displayTime()));
    m_timer->start(1000);

    //13) 初始化场景 启动定时器
    minescene->initscene();
}

void MainWindow::slot_newgamebyleve(QAction *act)                //点击游戏级别行为开始新游戏槽
{
    //1) 如果是初级行为
    if(act == lowleveaction)
    {
        //2) 设置场景的行，列，雷数，级别为初级
        this->minescene->m_scenerow = COMMON::LOWROWANDCOL;
        this->minescene->m_scenecol = COMMON::LOWROWANDCOL;
        this->minescene->m_minesum = COMMON::LOWMINENUM;
        this->minescene->m_crrentleve = COMMON::LOWLEVE;
    }
    //3) 如果是中级行为
    else if(act == middleleveaction)
    {
        //4) 设置场景的行，列，雷数，级别为中级
        this->minescene->m_scenerow = COMMON::MIDDLEROWANDCOL;
        this->minescene->m_scenecol = COMMON::MIDDLEROWANDCOL;
        this->minescene->m_minesum = COMMON::MIDDLEMINENUM;
        this->minescene->m_crrentleve = COMMON::MIDDLELEVE;
    }
    //5) 如果是高级级行为
    else if(act == heightleveaction)
    {
        //6) 设置场景的行，列，雷数，级别为高级，
        this->minescene->m_scenerow = COMMON::HEIGHTROW;
        this->minescene->m_scenecol = COMMON::HEIGHTCOL;
        this->minescene->m_minesum = COMMON::HEIGHTMINENUM;
        this->minescene->m_crrentleve = COMMON::HEIGHTLEVE;
    }
    //7) 如果是自定义行为
    else if(act == customaction)
    {
        //8) 创建自定义游戏框对象，并运行，并将自定义对话框的发送数据信号连接到接受自定义游戏数据槽
        CustomGameDialog *custom = new CustomGameDialog(this);
        this->connect(custom,SIGNAL(signal_sendCustomset(int,int,int)),this,SLOT(slot_acceptCutsomvale(int,int,int)));
        custom->exec();
    }
    //9) 并写入到注册表，开始新游戏
    this->writesettings();
    this->slot_newgame();
}

void MainWindow::slot_acceptCutsomvale(int row,int col, int count)
{
    //将自定义数据设置到场景的行，列，雷数，级别，将数据写入到注册表的当前游戏设置中
    minescene->m_crrentleve = COMMON::CUSOMLEVE;
    minescene->m_scenecol = col;
    minescene->m_scenerow = row;
    minescene->m_minesum = count;
    this->writesettings();

    //开始新游戏
    this->slot_newgame();
}

void MainWindow::slot_colorchanged()                       //设置颜色的槽
{
    QColor color = QColorDialog::getColor(Qt::blue,this,"选择颜色");
    if(color.isValid())
    {
        QPalette textPalette = minenumLcd->palette();       //获取调色板
        textPalette.setColor(QPalette::Normal, QPalette::WindowText, color);
        minenumLcd->setPalette(textPalette);
        textPalette= timeLcd->palette();
        textPalette.setColor(QPalette::Normal, QPalette::WindowText, color);
        timeLcd->setPalette(textPalette);

    }
}

void MainWindow::slot_soundchanged()                       //设置声音的槽
{
    //如果声音行为可选，则将声音行为设置为不可选，未选中，关闭场景声音
    if(soundaction->isCheckable())
    {
        soundaction->setCheckable(false);
        soundaction->setChecked(false);
        minescene->m_soundOpen = false;
    }
    //否则将声音行为设置为可选，已选中，打开场景声音
    else
    {
        soundaction->setCheckable(true);
        soundaction->setChecked(true);
        minescene->m_soundOpen = true;
    }
    this->update();
}

void MainWindow::slot_herochecked()                        //显示英雄榜的槽
{
    //创建英雄榜对象，并显示
    HeroDialog heroshow(this);
    heroshow.exec();
}

void MainWindow::slot_about()                              //显示about扫雷的槽
{
    //使用消息框中的about函数创建消息框
    QString str = tr("游戏名称: 扫雷\n游戏版本: 1.0\n游戏作者:YEOMAN\n发布日期:2021-3-3");
    QMessageBox::about(this,tr("关于扫雷"),str);
}

void MainWindow::slot_updatehero()                         //更新英雄榜的槽
{
    QSettings *setting = new QSettings("YEOMAN","mine",this);
    int recordtime = 0;
    QString newname;
    switch(minescene->m_crrentleve)
    {
    //1) 如果是初级游戏
    case COMMON::LOWLEVE:
        //    2) 从注册表中读取初级游戏纪录时间
        setting->beginGroup("herorecord");
        recordtime = setting->value("record_lowertime").toInt();
        setting->endGroup();
        qDebug() << "recodtime = " << recordtime << Qt::endl;
        //    3) 如果当前游戏用时比纪录时间少
        if(m_time < recordtime)
        {
            bool ok;
            //    4) 打开标准输入框的文本输入框
            newname = QInputDialog::getText(this,tr("新纪录"),"小伙子很棒嘛，请输入你的大名:",QLineEdit::Normal,"姓 名",&ok);
            if(ok && !newname.trimmed().isEmpty())
            {
                //    5) 将新纪录时间和姓 名写入到注册表 ，跳出
                setting->beginGroup("herorecord");
                setting->setValue("record_lowertime",m_time);
                setting->setValue("record_lowername",newname);
                setting->endGroup();
            }
        }
        break;
        //1) 如果是中级游戏
    case COMMON::MIDDLELEVE:
        //    2) 从注册表中读取初级游戏纪录时间
        setting->beginGroup("herorecord");
        recordtime = setting->value("record_middletime").toInt();
        setting->endGroup();
        //    3) 如果当前游戏用时比纪录时间少
        if(m_time < recordtime)
        {
            bool ok;
            //    4) 打开标准输入框的文本输入框
            newname = QInputDialog::getText(this,tr("新纪录"),"小伙子很棒嘛，请输入你的大名:",QLineEdit::Normal,"姓 名",&ok);
            if(ok && !newname.trimmed().isEmpty())
            {
                //    5) 将新纪录时间和姓 名写入到注册表 ，跳出
                setting->beginGroup("herorecord");
                setting->setValue("record_middletime",m_time);
                setting->setValue("record_middlename",newname);
                setting->endGroup();
            }
        }
        break;
    case COMMON::HEIGHTLEVE:
        //    2) 从注册表中读取初级游戏纪录时间
        setting->beginGroup("herorecord");
        recordtime = setting->value("record_heighttime").toInt();
        setting->endGroup();
        //    3) 如果当前游戏用时比纪录时间少
        if(m_time < recordtime)
        {
            bool ok;
            //    4) 打开标准输入框的文本输入框
            newname = QInputDialog::getText(this,tr("新纪录"),"小伙子很棒嘛，请输入你的大名:",QLineEdit::Normal,"姓 名",&ok);
            if(ok && !newname.trimmed().isEmpty())
            {
                //    5) 将新纪录时间和姓 名写入到注册表 ，跳出
                setting->beginGroup("herorecord");
                setting->setValue("record_heighttime",m_time);
                setting->setValue("record_heightname",newname);
                setting->endGroup();
            }
        }
        break;
    }
     //    16) 显示信息，游戏结束，是否继续，如果点继续则重新开始游戏，如果点取消则关闭游戏
    int button = QMessageBox::information(this,tr("游戏结束"),tr("胜败乃兵家常事，再战否？"),QMessageBox::Ok | QMessageBox::Cancel);
    if(button == QMessageBox::Ok)
        slot_newgame();
    else
        this->close();
}

void MainWindow::slot_displayMineNum(int flagminenum)                  //显示雷数的槽
{
    minenumLcd->display(minescene->m_minesum - flagminenum);
}

void MainWindow::slot_displayTime()                        //显示时间的槽
{
    //如果游戏未结束
    if(!minescene->m_isGameOver)
    {
        //当前游戏用时加1,并显示，如果场景为声音打开和声音行为被选中，则播放声音
        m_time++;
        timeLcd->display(m_time);
        if(minescene->m_soundOpen && soundaction->isChecked())
            QSound::play("://sounds/time.wav");
    }
    else
    {
        //如果游戏结束，则停止计时器
        m_timer->stop();
    }
}

MainWindow::~MainWindow()
{
}

