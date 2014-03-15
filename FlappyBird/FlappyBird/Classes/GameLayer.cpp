//
//  GameLayer.cpp
//  FlappyBird
//
//  Created by DoubleHH on 14-3-13.
//
//

#include "GameLayer.h"
#include "BBButton.h"

const int ZORDER_GROUND = 2;
const int ZORDER_COLLISION = 1;
const int ZORDER_ROLE = 10;
const int ZORDER_COUNTDOWN = 11;

const float INTERVAL_COLLISION = 150.0f;

GameLayer::~GameLayer() {
    
    CC_SAFE_RELEASE_NULL(mGrounds);
    CC_SAFE_RELEASE_NULL(mCollisionArray);
}

CCScene *GameLayer::scene() {
    
    CCScene *scene = CCScene::create();
    scene->addChild(GameLayer::create());
    return scene;
}

bool GameLayer::init() {
    
    bool bRet = false;
    do {
        CC_BREAK_IF(!CCLayer::init());
        
        extraInit();
        
        bRet = true;
    } while (0);
    return bRet;
}

void GameLayer::extraInit() {
    
    initRole();
    initGround();
    initCollisions();
    
    setRoleState(ROLE_STATE_JUMP_DOWN);
    setTouchEnabled(false);
    
    BBButton *button = BBButton::create(CCSizeMake(200, 60), this, bbButtonFunI_selector(GameLayer::restart), NULL, NULL, NULL, NULL, "Play Again");
    button->mLabel->setFontSize(30);
    button->setPosition(CCPoint(getContentSize().width - button->getContentSize().width, getContentSize().height - button->getContentSize().height));
    button->mDependedTarget = this;
    this->addChild(button, 10);
    
    setScore(0);
    
    beginCountdown(3);
}

void GameLayer::mainUpdate(float dt) {
    
    if (getIsPaused()) {
        return;
    }
    
    updateGround(dt);
    updateCollisions(dt);
    roleUpdate(dt);
    updateScore();
}

#pragma mark - Game Control
void GameLayer::gameOver() {
    
    this->unschedule(schedule_selector(GameLayer::mainUpdate));
    setIsPaused(true);
    CCMessageBox("Where is a will, There is a way!", "Game over");
}

void GameLayer::restart() {
    
    setScore(0);
    resetCollisions();
    resetRole();
    beginCountdown(3);
}

void GameLayer::resetRole() {
    mRole->setPosition(ccp(getContentSize().width * .5, getContentSize().height * .5));
    setRoleState(ROLE_STATE_JUMP_DOWN);
}

void GameLayer::resetCollisions() {
    
    float x = getStartCollsionX();
    float interval = BarPipe::getSize().width + INTERVAL_COLLISION;
    for (int i=0; i<mCollisionArray->count(); ++i) {
        BarPipe *pBar = (BarPipe *)mCollisionArray->objectAtIndex(i);
        pBar->setRandomY();
        pBar->setBarPositionX(x);
        pBar->setIsPassed(false);
        x += interval;
    }
}

float GameLayer::getStartCollsionX() {
    return (CCDirector::sharedDirector()->getWinSize().width * .7);
}

#pragma mark - Countdown
void GameLayer::preCountdown() {
    setTouchEnabled(false);
    setIsPaused(true);
}

#define TagCountdown 108474
void GameLayer::beginCountdown(int second) {
    
    preCountdown();
    setCountdownNum(second);
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCLabelTTF *label = CCLabelTTF::create(CCString::createWithFormat("%d", getCountdownNum())->getCString(), "ArialMT", 100);
    label->setColor(ccRED);
    label->setAnchorPoint(ccp(.5, .5));
    label->setPosition(ccp(size.width * .5, size.height * .5));
    this->addChild(label, ZORDER_COUNTDOWN);
    this->schedule(schedule_selector(GameLayer::countdownSeconds), 0.8);
    label->setTag(TagCountdown);
    
    doCountdownAnimation(label);
}

void GameLayer::countdownSeconds() {
    
    CCLabelTTF *label = (CCLabelTTF *)getChildByTag(TagCountdown);
    if (label) {
        
        setCountdownNum(getCountdownNum() - 1);
        if (getCountdownNum() < 1) {
            label->removeFromParent();
            label = NULL;
            this->unschedule(schedule_selector(GameLayer::countdownSeconds));
            countdonwDone();
            return;
        }
        
        label->setString(CCString::createWithFormat("%d", getCountdownNum())->getCString());
        doCountdownAnimation(label);
    }
}

void GameLayer::countdonwDone() {
    setTouchEnabled(true);
    setIsPaused(false);
    this->schedule(schedule_selector(GameLayer::mainUpdate), 0.016f);
}

void GameLayer::doCountdownAnimation(CCNode *node) {
    node->runAction(CCSequence::create(CCScaleTo::create(0.3, 3),
                                       CCScaleTo::create(0.15, 1.0),
                                       NULL));
}

#pragma mark - Score
void GameLayer::setScore(int var) {
    score = var;
    if (!mScoreLabel) {
        CCLabelTTF *label = CCLabelTTF::create(CCString::createWithFormat("%d", getScore())->getCString(), "ArialMT", 30);
        label->setColor(ccYELLOW);
        label->setAnchorPoint(ccp(0, .5));
        label->setPosition(ccp(50, getContentSize().height * 0.9));
        this->addChild(label, ZORDER_COUNTDOWN);
        
        mScoreLabel = label;
    }
    mScoreLabel->setString(CCString::createWithFormat("Score: %d", getScore())->getCString());
}

int GameLayer::getScore() {
    return score;
}

/**
 更新用户的分数
 **/
void GameLayer::updateScore() {
    
    for (int i=0; i<mCollisionArray->count(); ++i) {
        BarPipe *pBarPipe = (BarPipe *)mCollisionArray->objectAtIndex(i);
        if (!pBarPipe->getIsPassed() &&
            (pBarPipe->getBarPositionX() + pBarPipe->getSize().width) < mRole->getPositionX()) {
            
            pBarPipe->setIsPassed(true);
            setScore(getScore() + 1);
        }
    }
}

#pragma mark - Touch
void GameLayer::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, true);
}

void GameLayer::onExit() {
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool GameLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    
    setRoleState(ROLE_STATE_JUMP_UP);
    return true;
}

#pragma mark - Role
void GameLayer::initRole() {
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    mRole = CCSprite::create("bird.png");
    mRole->setAnchorPoint(ccp(0, 0));
    mRole->setPosition(ccp(size.width * .5, size.height * .5));
    this->addChild(mRole, ZORDER_ROLE);
}

void GameLayer::roleUpdate(float dt) {
    
    switch (getRoleState()) {
        case ROLE_STATE_JUMP_UP: {
            
            this->roleJumpUpLogic();
            break;
        }
        case ROLE_STATE_JUMP_DOWN: {
            
            this->roleJumpDownLogic();
            break;
        }
        default:
            break;
    }
}

void GameLayer::setRoleState(ROLE_STATE var) {
    
    switch (var) {
        case ROLE_STATE_JUMP_UP: {
            CCLOG("开始上升");
            setRoleYSpeed(getRoleConstYSpeed());
            break;
        }
        case ROLE_STATE_JUMP_DOWN: {
            CCLOG("开始下降");
            setRoleYSpeed(0);
            break;
        }
        default:
            break;
    }
    roleState = var;
}

void GameLayer::roleJumpDownLogic() {
    
    /**
     1, 先判断是否到地面下了
     2, 检测右边的碰撞
     3, 检测下面的碰撞
     **/
    
    float roleY = mRole->getPositionY();
    roleY -= getRoleYSpeed();
    if (roleY < getGroundHeight()) {
        gameOver();
        CCLOG("正在下降, 撞地了");
        return;
    }
    
    BarPipe *willCollisionBar = getWillCollisionBar();
    if (isCollisionWithBar(COLL_STATE_RIGHT, willCollisionBar)) {
        gameOver();
        CCLOG("正在下降, 右边发生碰撞");
        return;
    }
    
    if (isCollisionWithBar(COLL_STATE_BOTTOM, willCollisionBar)) {
        gameOver();
        CCLOG("正在下降, 下边发生碰撞");
        return;
    }
    
    mRole->setPositionY(roleY);
    setRoleYSpeed(getRoleYSpeed() + getRoleAccelerate());
}

void GameLayer::roleJumpUpLogic() {
    
    /**
     1, 检测是否碰到天花板了
     2, 判断速度是否小于等于零了
     3, 检测右边的碰撞
     4. 检测上边的碰撞
     **/
    float roleY = mRole->getPositionY();
    roleY += getRoleYSpeed();
    // 判断是否过了屏幕上方
    if (roleY + mRole->getContentSize().height > getContentSize().height) {
        gameOver();
        CCLOG("正在上升, 撞天花板了");
        return;
    }
    
    if (getRoleYSpeed() <= 0) {
        setRoleState(ROLE_STATE_JUMP_DOWN);
        return;
    }
    
    
    BarPipe *willCollisionBar = getWillCollisionBar();
    if (isCollisionWithBar(COLL_STATE_RIGHT, willCollisionBar)) {
        gameOver();
        CCLOG("正在上升, 右边发生碰撞");
        return;
    }
    
    if (isCollisionWithBar(COLL_STATE_TOP, willCollisionBar)) {
        gameOver();
        CCLOG("正在上升, 上边发生碰撞");
        return;
    }

    mRole->setPositionY(roleY);
    setRoleYSpeed(getRoleYSpeed() - getRoleAccelerate());
}

ROLE_STATE GameLayer::getRoleState() {
    return roleState;
}

#pragma mark - Ground
void GameLayer::initGround() {
    
    mGrounds = CCArray::create();
    CC_SAFE_RETAIN(mGrounds);
    for (int i=0; i<2; ++i) {
        CCSprite *groundSp = CCSprite::create("ground.png");
        groundSp->setAnchorPoint(ccp(0, 0));
        groundSp->setPosition(ccp(0, 0));
        mGrounds->addObject(groundSp);
        this->addChild(groundSp, ZORDER_GROUND);
        
        if (1 == i) {
            groundSp->setPositionX(groundSp->getContentSize().width);
            setGroundHeight(groundSp->getContentSize().height);
        }
    }
}

void GameLayer::updateGround(float dt) {
    CCSprite *groundSp1 = (CCSprite *)mGrounds->objectAtIndex(0);
    CCSprite *groundSp2 = (CCSprite *)mGrounds->objectAtIndex(1);
    
    groundSp1->setPositionX(groundSp1->getPositionX() - getGroundSpeed());
    groundSp2->setPositionX(groundSp2->getPositionX() - getGroundSpeed());
    
    if (groundSp1->getPositionX() < -groundSp1->getContentSize().width) {
        groundSp1->setPositionX(groundSp2->getPositionX() + groundSp2->getContentSize().width);
        mGrounds->exchangeObject(groundSp1, groundSp2);
    }
}

#pragma mark - Collisions
void GameLayer::initCollisions() {
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    mCollisionArray = CCArray::create();
    CC_SAFE_RETAIN(mCollisionArray);
    float barWidth = BarPipe::getSize().width;
    float length = INTERVAL_COLLISION + barWidth;
    int count = ceil(size.width / length) + 1;
    
    float x = getStartCollsionX();
    for (int i=0; i<count; ++i) {
        
        BarPipe *pBarPipe = BarPipe::randomCreate(this);
        pBarPipe->setBarPositionX(x);
        mCollisionArray->addObject(pBarPipe);
        
        x += length;
    }
}

void GameLayer::updateCollisions(float dt) {
    
    if (!mCollisionArray->count()) {
        return;
    }
    
    for (int i=0; i<mCollisionArray->count(); ++i) {
        BarPipe *pBarPipe = (BarPipe *)mCollisionArray->objectAtIndex(i);
        pBarPipe->setBarPositionX(pBarPipe->getBarPositionX() - getGroundSpeed());
    }
    
    /**
        检测第一个是否出界, 如是
        1.改变它的位置
        2.把第一个放到最后一个
     **/
    BarPipe *pFirstBarPipe = (BarPipe *)mCollisionArray->objectAtIndex(0);
    if (pFirstBarPipe->getBarPositionX() < -pFirstBarPipe->getSize().width) {
        
        BarPipe *pLastBarPipe = (BarPipe *)mCollisionArray->lastObject();
        float firstX = pLastBarPipe->getBarPositionX() + BarPipe::getSize().width + INTERVAL_COLLISION;
        pFirstBarPipe->setBarPositionX(firstX);
        pFirstBarPipe->setRandomY();
        pFirstBarPipe->setIsPassed(false);
        
        CC_SAFE_RETAIN(pFirstBarPipe);
        mCollisionArray->removeObjectAtIndex(0);
        mCollisionArray->addObject(pFirstBarPipe);
        CC_SAFE_RELEASE(pFirstBarPipe);
    }
    
}

#pragma mark - Check if have a Collision
bool GameLayer::isCollisionWithBar(COLL_STATE state, BarPipe *barPipe) {
    
    if (isCollisionArea(state, mRole->boundingBox(), barPipe->getUpBar()->boundingBox())) {
        return true;
    }
    if (isCollisionArea(state, mRole->boundingBox(), barPipe->getDownBar()->boundingBox())) {
        return true;
    }
    return false;
}


bool GameLayer::isCollisionArea(COLL_STATE state, CCRect roleBox, CCRect collisionBox) {
    bool bFlag = false;
    
    switch (state) {
        case COLL_STATE_LEFT: {
            bFlag = isCollisionLeft(roleBox, collisionBox);
            break;
        }
        case COLL_STATE_RIGHT: {
            bFlag = isCollisionRight(roleBox, collisionBox);
            break;
        }
        case COLL_STATE_TOP: {
            bFlag = isCollisionTop(roleBox, collisionBox);
            break;
        }
        case COLL_STATE_BOTTOM: {
            bFlag = isCollisionBottom(roleBox, collisionBox);
            break;
        }
        default:
            break;
    }
    
    if (bFlag) {
        fixCollision(state, mRole, collisionBox);
    }
    
    return bFlag;
}

bool GameLayer::isCollisionLeft(CCRect roleBox, CCRect collisionBox) {
    
    CCPoint targetPoint = ccp(roleBox.getMinX(), roleBox.getMidY());
    return collisionBox.containsPoint(targetPoint);
}

bool GameLayer::isCollisionRight(CCRect roleBox, CCRect collisionBox) {
    
    CCPoint targetPoint = ccp(roleBox.getMaxX(), roleBox.getMidY());
    return collisionBox.containsPoint(targetPoint);
}

bool GameLayer::isCollisionBottom(CCRect roleBox, CCRect collisionBox) {
    
    CCPoint targetPoint = ccp(roleBox.getMidX(), roleBox.getMinY());
    return collisionBox.containsPoint(targetPoint);
}

bool GameLayer::isCollisionTop(CCRect roleBox, CCRect collisionBox) {
    
    CCPoint targetPoint = ccp(roleBox.getMidX(), roleBox.getMaxY());
    return collisionBox.containsPoint(targetPoint);
}

void GameLayer::fixCollision(COLL_STATE state, CCNode *role, cocos2d::CCRect collisionBox) {
    CCRect roleBox = role->boundingBox();
    
    switch (state) {
        case COLL_STATE_LEFT: {
            float fixedX = role->getPositionX();
            fixedX += collisionBox.getMaxX() - roleBox.getMinX();
            role->setPositionX(fixedX);
            break;
        }
        case COLL_STATE_RIGHT: {
            float fixedX = role->getPositionX();
            fixedX -= roleBox.getMaxX() - collisionBox.getMinX();
            role->setPositionX(fixedX);
            break;
        }
        case COLL_STATE_TOP: {
            float fixedY = role->getPositionY();
            fixedY -= roleBox.getMaxY() - collisionBox.getMinY();
            role->setPositionY(fixedY);
            break;
        }
        case COLL_STATE_BOTTOM: {
            float fixedY = role->getPositionY();
            fixedY += collisionBox.getMaxY() - roleBox.getMinY();
            role->setPositionY(fixedY);
            break;
        }
        default:
            break;
    }
}

BarPipe *GameLayer::getWillCollisionBar() {
    
    for (int i=0; i<mCollisionArray->count(); ++i) {
        BarPipe *pBar = (BarPipe *)mCollisionArray->objectAtIndex(i);
        if (pBar->getBarPositionX() + BarPipe::getSize().width > mRole->getPositionX()) {
            return pBar;
        }
    }
    return NULL;
}

#pragma mark - BarPipe

BarPipe::~BarPipe() {
    
    CC_SAFE_RELEASE_NULL(upBar);
    CC_SAFE_RELEASE_NULL(downBar);
}

bool BarPipe::init() {
    
    upBar = CCSprite::create("up_bar.png");
    downBar = CCSprite::create("down_bar.png");
    CC_SAFE_RETAIN(upBar);
    CC_SAFE_RETAIN(downBar);
    
    upBar->setAnchorPoint(ccp(0, 0));
    downBar->setAnchorPoint(upBar->getAnchorPoint());
    
    upBar->setPositionX(0);
    downBar->setPositionX(upBar->getPositionX());
    
    return true;
}

void BarPipe::setRandomY() {
    if (!mParent) {
        return;
    }
    GameLayer *layer = (GameLayer *)mParent;
    float groundHeight = layer->getGroundHeight();
    // 计算出管道最多能上下的最大值
    float randomTotal = upBar->getContentSize().height + downBar->getContentSize().height + getBarInterval() - (mParent->getContentSize().height - groundHeight);
    float randomY = CCRANDOM_0_1() * randomTotal;
    downBar->setPositionY(-randomY + groundHeight);
    upBar->setPositionY(downBar->getPositionY() + getHeight(downBar) + getBarInterval());
}

BarPipe *BarPipe::randomCreate(CCNode *parent) {
    
    BarPipe *pBarPipe = new BarPipe();
    if (pBarPipe && pBarPipe->init()) {
        
        pBarPipe->setParentNode(parent);
        pBarPipe->setRandomY();
        pBarPipe->autorelease();
        return pBarPipe;
    }
    CC_SAFE_DELETE(pBarPipe);
    return NULL;
}

void BarPipe::setParentNode(cocos2d::CCNode *parent) {
    
    if (!parent) {
        return;
    }
    mParent = parent;
    if (upBar) {
        parent->addChild(upBar, ZORDER_COLLISION);
    }
    if (downBar) {
        parent->addChild(downBar, ZORDER_COLLISION);;
    }
}

void BarPipe::setBarPositionX(float var) {
    if (upBar) {
        upBar->setPositionX(var);
    }
    if (downBar) {
        downBar->setPositionX(var);
    }
}

float BarPipe::getBarPositionX() {
    if (upBar) {
        return upBar->getPositionX();
    }
    if (downBar) {
        return downBar->getPositionX();
    }
    return 0;
}

CCSize BarPipe::getSize() {
    return CCSize(52, 319);
}
