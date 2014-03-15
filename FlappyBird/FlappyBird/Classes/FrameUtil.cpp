//
//  FrameUtil.cpp
//  BabyReadBook3
//
//  Created by huanghui on 13-9-23.
//
//

#include "FrameUtil.h"

using namespace cocos2d;

CCRect getFrame(CCNode *node) {
    CCRect frame = CCRectMake(0, 0, node->getContentSize().width, node->getContentSize().height);
    frame.origin.x = node->getPosition().x - node->getContentSize().width * node->getAnchorPoint().x;
    frame.origin.y = node->getPosition().y - node->getContentSize().height * node->getAnchorPoint().y;
    return frame;
}


CCPoint getCenter(CCNode *node) {
    CCRect frame = getFrame(node);
    return CCPointMake(frame.origin.x + frame.size.width * 0.5, frame.origin.y + frame.size.height * 0.5);
}

float getWidth(CCNode *node) {
    return node->getContentSize().width;
}

float getHeight(CCNode *node) {
    return node->getContentSize().height;
}

float getLeft(CCNode *node) {
    CCRect frame = getFrame(node);
    return frame.origin.x;
}

float getRight(CCNode *node) {
    CCRect frame = getFrame(node);
    return frame.origin.x + frame.size.width;
}

float getTop(CCNode *node) {
    CCRect frame = getFrame(node);
    return frame.origin.y + frame.size.height;
}

float getBottom(CCNode *node) {
    CCRect frame = getFrame(node);
    return frame.origin.y;
}

void hhPrintRect(CCRect rect, const char *a) {
    CCLOG("%s: Rect(x=%f, y=%f, w=%f, h=%f)", a, rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
}

void hhPrintSize(CCSize size, const char *a) {
    CCLOG("%s: Size(w=%f, h=%f)", a, size.width, size.height);
}

void hhPrintPoint(CCPoint point, const char *a) {
    CCLOG("%s: Point(x=%f, y=%f)", a, point.x, point.y);
}

void hhPrintRect(CCRect rect) {
    hhPrintRect(rect, "");
}

void hhPrintSize(CCSize size) {
    hhPrintSize(size, "");
}

void hhPrintPoint(CCPoint point) {
    hhPrintPoint(point, "");
}

