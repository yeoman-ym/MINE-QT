/*************************************************
Copyright:Call_Me_Yeoman
Author:Yeoman
Date:2020-12-10
Description:扫雷公共数据头文件
**************************************************/
#ifndef COMMON_H
#define COMMON_H

class COMMON
{
public:
                                    //游戏级别
    enum gameLeve
    {
        LOWLEVE = 0,                //低级
        MIDDLELEVE = 1,             //中级
        HEIGHTLEVE = 2,             //高级
        CUSOMLEVE = 3               //自定义
    };
                                    //初级行\列\雷设置
    enum lowset
    {
        LOWROWANDCOL = 9,           //初级行列数
        LOWMINENUM = 10             //初级雷数
    };
                                    //中级行\列\雷设置
    enum middleset
    {
        MIDDLEROWANDCOL = 16,       //中级行列数
        MIDDLEMINENUM = 40          //中级雷数
    };
                                    //高级行\列\雷设置
    enum heightset
    {
        HEIGHTROW = 16,             //高级行数
        HEIGHTCOL = 30,             //高级行数
        HEIGHTMINENUM = 99          //高级雷数
    };

    enum mapset
    {
        MAPWIDTH = 25,              //地图宽度
        MAPHEIGHT = 25              //地图高度
    };
};

#endif // COMMON_H
