//
//  GameLayer.h
//  FlappyBird
//
//  Created by DoubleHH on 14-3-13.
//
//

#ifndef __FlappyBird__GameLayer__
#define __FlappyBird__GameLayer__

// 主角状态
typedef enum {
    ROLE_STATE_INVALID = 0,
    ROLE_STATE_STANDBY, // 静止站立
    ROLE_STATE_RUN,     // 直线跑动
    ROLE_STATE_JUMP_UP, // 跳起
    ROLE_STATE_JUMP_DOWN,//跳起后下落抛物线
    ROLE_STATE_FREE_FALL,//自由落体
}ROLE_STATE;

typedef enum {
    COLL_STATE_INVALID = 0,
    COLL_STATE_LEFT,
    COLL_STATE_RIGHT,
    COLL_STATE_TOP,
    COLL_STATE_BOTTOM,
}COLL_STATE;

class BarPipe;

#include "FBHeaders.h"

class GameLayer : public CCLayer {
public:
    GameLayer():groundSpeed(3.5), roleConstYSpeed(10), roleYSpeed(0), roleAccelerate(0.6), groundHeight(0), isPaused(true), mScoreLabel(NULL), score(0) {};
    ~GameLayer();
    
    virtual bool init();
    void extraInit();
    static CCScene *scene();
    CREATE_FUNC(GameLayer);
    void mainUpdate(float dt);
    
    virtual void registerWithTouchDispatcher(void);
    virtual void onExit();
    
    // default implements are used to call script callback if exist
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    void gameOver();
    void restart();
    
private:
    void resetRole();
    void resetCollisions();
    float getStartCollsionX();
    
private:
    CC_PROPERTY(int , score, Score);
    CCLabelTTF *mScoreLabel;
    void updateScore();
    
private:
    CC_SYNTHESIZE(int, countdownNum, CountdownNum);
    void preCountdown();
    void beginCountdown(int second);
    void countdownSeconds();
    void countdonwDone();
    void doCountdownAnimation(CCNode *node);
    
public:
    CC_SYNTHESIZE(float, groundHeight, GroundHeight);
    CC_SYNTHESIZE(bool, isPaused, IsPaused);
    
private:
    void initRole();
    CC_PROPERTY(ROLE_STATE, roleState, RoleState);
//    CC_SYNTHESIZE(float, constXSpeed, ConstXSpeed);
    CC_SYNTHESIZE(float, roleConstYSpeed, RoleConstYSpeed);
//    CC_SYNTHESIZE(float, xSpeed, XSpeed);
    CC_SYNTHESIZE(float, roleYSpeed, RoleYSpeed);
    CC_SYNTHESIZE(float, roleAccelerate, RoleAccelerate);
    
private:
    CCSprite *mRole;
    CCArray *mGrounds;
    
    CC_SYNTHESIZE(float, groundSpeed, GroundSpeed);
    
private:
    void roleUpdate(float dt);
    
//    void roleRunLogic();
//    void roleStandbyLogic();
    void roleJumpUpLogic();
    void roleJumpDownLogic();
//    void roleFreeFallLogic();
    
private:
    void initGround();
    void updateGround(float dt);
    
private:
    // 碰撞检测
    bool isCollisionArea(COLL_STATE state, CCRect roleBox, CCRect collisionBox);
    bool isCollisionLeft(CCRect roleBox, CCRect collisionBox);
    bool isCollisionRight(CCRect roleBox, CCRect collisionBox);
    bool isCollisionTop(CCRect roleBox, CCRect collisionBox);
    bool isCollisionBottom(CCRect roleBox, CCRect collisionBox);
    // 碰撞修复
    void fixCollision(COLL_STATE state, CCNode *role, cocos2d::CCRect collisionBox);

    
    void initCollisions();
    CCArray *mCollisionArray;
    void updateCollisions(float dt);
    
    bool isCollisionWithBar(COLL_STATE state, BarPipe *barPipe);
    BarPipe *getWillCollisionBar();
};

#endif /* defined(__FlappyBird__GameLayer__) */

class BarPipe : public CCObject {
public:
    BarPipe():upBar(NULL), downBar(NULL), barPositionX(0), mParent(NULL), barInterval(150), isPassed(false) {};
    ~BarPipe();
    
    virtual bool init();
    static BarPipe *randomCreate(CCNode *parent);
    void setParentNode(CCNode *parent);
    void setRandomY();
    
    static CCSize getSize();
    
public:
    CC_SYNTHESIZE_RETAIN(CCSprite *, upBar, UpBar);
    CC_SYNTHESIZE_RETAIN(CCSprite *, downBar, DownBar);
    CC_PROPERTY(float, barPositionX, BarPositionX);
    CC_SYNTHESIZE(float, barInterval, BarInterval);
    
    /**
     小鸟是否已经经过了这个柱子, 用于计算分数
     **/
    CC_SYNTHESIZE(bool, isPassed, IsPassed);
    
    CCNode *mParent;
   
};
