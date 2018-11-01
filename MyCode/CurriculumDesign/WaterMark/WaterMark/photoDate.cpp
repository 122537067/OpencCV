//
//  photoDate.cpp
//  WaterMark
//
//  Created by one on 2018/11/1.
//  Copyright © 2018 one. All rights reserved.
//

#include "photoDate.hpp"

//
//  photoDate.cpp
//  WaterMark
//
//  Created by one on 2018/11/1.
//  Copyright © 2018 one. All rights reserved.
//

#include <stdio.h>
int photoDate::getDateSize(void)
{
    return dateSize;
}
int photoDate::getDateBold(void)
{
    return dateBold;
}
int photoDate::getDateCols(void)
{
    return dateCols;
}
int photoDate::getDateRows(void)
{
    return dateRows;
}
void photoDate::setDateSize(int size)
{
    dateSize = size;
}
void photoDate::setDateBold(int bold)
{
    dateBold = bold;
}
void photoDate::setDateCols(int cols)
{
    dateCols = cols;
}
void photoDate::setDateRows(int rows)
{
    dateRows = rows;
}

