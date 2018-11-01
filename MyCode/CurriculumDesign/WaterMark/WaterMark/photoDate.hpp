//
//  photoDate.hpp
//  WaterMark
//
//  Created by one on 2018/11/1.
//  Copyright © 2018 one. All rights reserved.
//

#ifndef photoDate_hpp
#define photoDate_hpp

#include <stdio.h>

//日期类
class photoDate
{
public:
    int dateSize;   //日期大小
    int dateBold;   //日期字体粗细
    int dateCols; //位置
    int dateRows;
    
    // 获取类值
    int getDateSize(void);
    int getDateBold(void);
    int getDateCols(void);
    int getDateRows(void);
    // 设置类值
    void setDateSize(int size);
    void setDateBold(int bold);
    void setDateCols(int cols);
    void setDateRows(int rows);
};

#endif /* photoDate_hpp */
