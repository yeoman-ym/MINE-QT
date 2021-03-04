#include "customgamedialog.h"

CustomGameDialog::CustomGameDialog(QWidget *parent) : QDialog(parent)
{
    //创建界面
    this->setWindowTitle("自定义游戏");
    QLabel *label_1 = new QLabel("行 数");
    QLabel *label_2 = new QLabel("列 数");
    QLabel *label_3 = new QLabel("雷 数");

    //行列数和雷数分量框设置
    colspinbox = new QSpinBox;
    colspinbox->setRange(10,100);
    colspinbox->setSingleStep(1);
    colspinbox->setValue(10);

    rowspinbox = new QSpinBox;
    rowspinbox->setRange(10,100);
    rowspinbox->setSingleStep(1);
    rowspinbox->setValue(10);

    minenumspinbox = new QSpinBox;
    minenumspinbox->setRange(10,100);
    minenumspinbox->setSingleStep(1);
    minenumspinbox->setValue(20);

    //创建确认和取消按钮组件
    QPushButton *ok_button = new QPushButton("确定");
    QPushButton *cancel_button = new QPushButton("取消");

    QGridLayout *glayout = new QGridLayout(this);
    glayout->addWidget(label_1,0,0);
    glayout->addWidget(colspinbox,0,1);
    glayout->addWidget(label_2,1,0);
    glayout->addWidget(rowspinbox,1,1);
    glayout->addWidget(label_3,2,0);
    glayout->addWidget(minenumspinbox,2,1);

    glayout->addWidget(ok_button,3,0);
    glayout->addWidget(cancel_button,3,1);

    //按钮连接信号到本类的槽函数
    this->connect(ok_button,SIGNAL(clicked(bool)),this,SLOT(slot_accpetok()));
    this->connect(cancel_button,SIGNAL(clicked(bool)),this,SLOT(close()));
}

void CustomGameDialog::readsettings()
{
    //读取自定义游戏设置
    QSettings *setting = new QSettings("YEOMAN","mine",this);
    //自定义游戏数据
    setting->beginGroup("customGame");
    int row = setting->value("cusom_row",10).toInt();
    int col = setting->value("custom_col",10).toInt();
    int mine = setting->value("custom_mine",11).toInt();
    setting->endGroup();

    //将读取到的数据设置到相应分量框
    rowspinbox->setValue(row);
    colspinbox->setValue(col);
    minenumspinbox->setValue(mine);
}

void CustomGameDialog::writesettings()
{
    //将数据写入自定义的文件中
    QSettings *setting = new QSettings("YEOMAN","mine",this);
    setting->beginGroup("customGame");
    setting->setValue("cusom_row",rowspinbox->value());
    setting->setValue("custom_col",colspinbox->value());
    setting->setValue("custom_mine",minenumspinbox->value());
    setting->endGroup();
}

    //确定按钮连接的槽函数
void CustomGameDialog::slot_accpetok()
{
    //获取分量框中的数据
    int row = rowspinbox->value();
    int col = colspinbox->value();
    int count = minenumspinbox->value();

    //发送自定义游戏数据信号，将当前设置保存到注册表，关闭对话框
    emit this->signal_sendCustomset(row,col,count);
    this->writesettings();
    this->close();
}
