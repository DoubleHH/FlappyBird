//
//  FrameUtil.h
//  BabyReadBook3
//
//  Created by huanghui on 13-9-23.
//
//

#ifndef __BabyReadBook3__FrameUtil__
#define __BabyReadBook3__FrameUtil__

#include "cocos2d.h"

cocos2d::CCRect getFrame(cocos2d::CCNode *node);
cocos2d::CCPoint getCenter(cocos2d::CCNode *node);
float getWidth(cocos2d::CCNode *node);
float getHeight(cocos2d::CCNode *node);
float getLeft(cocos2d::CCNode *node);
float getRight(cocos2d::CCNode *node);
float getTop(cocos2d::CCNode *node);
float getBottom(cocos2d::CCNode *node);

void hhPrintRect(cocos2d::CCRect rect, const char *a);
void hhPrintSize(cocos2d::CCSize size, const char *a);
void hhPrintPoint(cocos2d::CCPoint point, const char *a);
void hhPrintRect(cocos2d::CCRect rect);
void hhPrintSize(cocos2d::CCSize size);
void hhPrintPoint(cocos2d::CCPoint point);

#endif /* defined(__BabyReadBook3__FrameUtil__) */
