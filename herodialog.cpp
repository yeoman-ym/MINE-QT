/*************************************************
Copyright:Call_Me_Yeoman
Author:Yeoman
Date:2021-2-4
Description:扫雷英雄榜实现
**************************************************/
#include "herodialog.h"

HeroDialog::HeroDialog(QWidget *parent) : QDialog(parent)
{
    //创建界面
    this->setWindowTitle("扫雷英雄榜");
    //创建标签名字组件
    QLabel *label_1 = new QLabel("初 级");
    QLabel *label_2 = new QLabel("中 级");
    QLabel *label_3 = new QLabel("高 级");

    //创建按钮组件
    QPushButton *ok_button = new QPushButton("关 闭");
    QPushButton *reset_button = new QPushButton("重 置");
    //创建按钮并连接信号到本类的相应槽
    this->connect(ok_button,SIGNAL(clicked(bool)),this,SLOT(close()));
    this->connect(reset_button,SIGNAL(clicked(bool)),this,SLOT(slot_reset()));

    //读取旧数据并显示
    lowrecordtimelabel = new QLabel("999");     //初级纪录时间
    middlerecordtimelabel = new QLabel("999");  //中级纪录时间
    heightrecordtimelabel = new QLabel("999");  //高级纪录时间
    lowrecordnamelabel = new QLabel("匿名");     //初级纪录姓名
    middlerecordnamelabel = new QLabel("匿名");  //中级纪录姓名
    heightrecordnamelabel = new QLabel("匿名");  //高级纪录姓名

    //创建网格布局
    QGridLayout *glayout = new QGridLayout;
    glayout->addWidget(label_1,0,0);
    glayout->addWidget(lowrecordtimelabel,0,1);
    glayout->addWidget(lowrecordnamelabel,0,2);
    glayout->addWidget(label_2,1,0);
    glayout->addWidget(middlerecordtimelabel,1,1);
    glayout->addWidget(middlerecordnamelabel,1,2);
    glayout->addWidget(label_3,2,0);
    glayout->addWidget(heightrecordtimelabel,2,1);
    glayout->addWidget(heightrecordnamelabel,2,2);

    //创建水平布局
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(reset_button);
    hlayout->addStretch();
    hlayout->addWidget(ok_button);

    //创建竖直布局（主布局）
    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->addLayout(glayout);
    vlayout->addLayout(hlayout);

    this->readsettings();
//    this->init_herorecord();
}

//函数功能：读英雄榜数据并设置到当前界面
void HeroDialog::readsettings()
{
    //从文件中读取排行榜数据
    QSettings *setting = new QSettings("YEOMAN","mine",this);
    setting->beginGroup("herorecord");
    QString lowertime = setting->value("record_lowertime").toString();
    QString middletime = setting->value("record_middletime").toString();
    QString heighttime = setting->value("record_heighttime").toString();

    QString lowername = setting->value("record_lowername","匿名").toString();
    QString middlename = setting->value("record_middlename","匿名").toString();
    QString heightname = setting->value("record_heightname","匿名").toString();
    setting->endGroup();

    lowrecordtimelabel->setText(lowertime);
    middlerecordtimelabel->setText(middletime);
    heightrecordtimelabel->setText(heighttime);

    lowrecordnamelabel->setText(lowername);
    middlerecordnamelabel->setText(middlename);
    heightrecordnamelabel->setText(heightname);

//    qDebug() << "lowertime = " << lowertime << Qt::endl;
//    qDebug() << "lowrecordtimelabel = " << lowrecordtimelabel << Qt::endl;
}

//函数功能：写英雄榜数据到注册表
void HeroDialog::init_herorecord()
{
    QSettings *setting = new QSettings("YEOMAN","mine",this);
    setting->beginGroup("herorecord");
    setting->setValue("record_lowertime",lowrecordtimelabel->text().toInt());
    setting->setValue("record_middletime",middlerecordtimelabel->text().toInt());
    setting->setValue("record_heighttime",heightrecordtimelabel->text().toInt());

//    setting->setValue("record_lowertime",888);
//    setting->setValue("record_lowertime",888);
//    setting->setValue("record_lowertime",888);

    setting->setValue("record_lowername",lowrecordnamelabel->text());
    setting->setValue("record_middlename",middlerecordnamelabel->text());
    setting->setValue("record_heightname",heightrecordnamelabel->text());
    setting->endGroup();
}

//重新设置英雄榜的槽
void HeroDialog::slot_reset()
{    
    qDebug() << "11111" << Qt::endl;
    lowrecordtimelabel->setText("888");
    middlerecordtimelabel->setText("888");
    heightrecordtimelabel->setText("888");

    lowrecordnamelabel->setText("状元");
    middlerecordnamelabel->setText("榜眼");
    heightrecordnamelabel->setText("探花");
    //初始化英雄榜
    this->init_herorecord();
}
