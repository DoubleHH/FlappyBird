//
//  BBButton.h
//  BBLoveStudy1
//
//  Created by huanghui on 13-11-5.
//
//

#ifndef __BBLoveStudy1__BBButton__
#define __BBLoveStudy1__BBButton__

#include "cocos2d.h"

typedef void(cocos2d::CCObject::*SEL_BBButtonHandle)(int);
#define bbButtonFunI_selector(_SELECTOR) (SEL_BBButtonHandle)(&_SELECTOR)

class BBButton : public cocos2d::CCLayer {
public:
    
    BBButton():mEnable(false), mListener(NULL), mTouchDownFunc(NULL), mTouchUpInsideFunc(NULL), mBgImageSprite(NULL), mImageSprite(NULL), mLabel(NULL), mSelected(false), mImageLocationFixedX(-1), mPriority(-2), mIsLoadImageFromLocal(true) {}
    
    virtual bool initWithContentSize(cocos2d::CCSize contentSize,
                                     cocos2d::CCObject *target,
                                     SEL_BBButtonHandle touchUpInsideCallback,
                                     const char *pBgNormalFileName,
                                     const char *pBgHighlightFileName,
                                     const char *pImageNormalFileName,
                                     const char *pImageHighlightFileName,
                                     const char *pTitleText,
                                     bool isLoadImageFromLocal = true,
                                     int pPriority = -2
                                     );
    /*
     cocos2d::CCSize contentSize,       占的大小
     cocos2d::CCObject *target,         点击button时回调的对象
     SEL_BBButtonHandle handle,         回调函数
     const char *pBgNormalFileName,     button的背景, 未点击时的图片
     const char *pBgHighlightFileName,  button的背景, 点击时的图片
     const char *pImageNormalFileName,  button的图片, 未点击时的图片
     const char *pImageHighlightFileName,button的图片, 点击时的图片
     const char *pTitleText             button的文字
     */
    static BBButton *create(cocos2d::CCSize contentSize,
                            cocos2d::CCObject *target,
                            SEL_BBButtonHandle touchUpInsideCallback,
                            const char *pBgNormalFileName,
                            const char *pBgHighlightFileName,
                            const char *pImageNormalFileName,
                            const char *pImageHighlightFileName,
                            const char *pTitleText,
                            bool isLoadImageFromLocal = true,  // 是否从本地加载或者从plist文件(TexturePacker)加载
                            int pPriority = -2
                            );
    
    // 注册点击事件等
    virtual void registerWithTouchDispatcher();
    virtual void onExit();
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    CC_PROPERTY(bool, mEnable, ButtonEnable);
    CC_PROPERTY(bool, mSelected, Selected);
    
    cocos2d::CCSprite *mBgImageSprite;
    cocos2d::CCSprite *mImageSprite;
    cocos2d::CCLabelTTF *mLabel;
    
    
    // 设置背景
    void setBackgroundImage(const char *pBgNormalFileName,
                            const char *pBgHighlightFileName);
    // 设置图片
    void setImage(const char *pImageNormalFileName,
                  const char *pImageHighlightFileName);
    
    // 设置图片和标签间隔, 默认都在中心点
    void setImageLabelInterval(float pInterval);
    // 设置图片的位置不变
    void setImageFixedLocation(float x);
    
    // 设置label的颜色, 正常,选中,不可点击
    void setNormalColor(cocos2d::ccColor3B normalColor);
    void setSelectedColor(cocos2d::ccColor3B selectedColor);
    void setDisabledColor(cocos2d::ccColor3B disableColor);
    
    void setTouchDownHandle(SEL_BBButtonHandle touchDownCallback);
    void setTouchUpInsideHandle(SEL_BBButtonHandle touchUpInsideCallback);
    
public:
    // 依赖的目标, 与目标的是否能touch保持一致
    CCLayer *mDependedTarget;
    
protected:
    // 回调函数
    SEL_BBButtonHandle mTouchDownFunc;      //点下的回调
    SEL_BBButtonHandle mTouchUpInsideFunc;  //点下并在button范围内抬起的回调
    // 回调对象
    CCObject *mListener;
    cocos2d::ccColor3B mNormalColor;
    cocos2d::ccColor3B mSelectedColor;
    cocos2d::ccColor3B mDisableColor;
    
    // 加载图片的方式, 默认在本地, 否则在CCFrameCache内
    bool mIsLoadImageFromLocal;
    
protected:
    std::string mBgNormalFileName;
    std::string mBgHighlightFileName;
    std::string mImageNormalFileName;
    std::string mImageHighlightFileName;
    
    float mImageLocationFixedX;
    
    int mPriority;
};

#endif /* defined(__BBLoveStudy1__BBButton__) */
