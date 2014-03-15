//
//  BBButton.cpp
//  BBLoveStudy1
//
//  Created by huanghui on 13-11-5.
//
//

#include "BBButton.h"

USING_NS_CC;

#define NULL_TO_EMPTY(__s) (NULL == __s ? "" : __s)

#pragma mark - BBHeadButton
BBButton *BBButton::create(cocos2d::CCSize contentSize,
                           cocos2d::CCObject *target,
                           SEL_BBButtonHandle touchUpInsideCallback,
                           const char *pBgNormalFileName,
                           const char *pBgHighlightFileName,
                           const char *pImageNormalFileName,
                           const char *pImageHighlightFileName,
                           const char *pTitleText,
                           bool isLoadImageFromLocal,
                           int pPriority) {
    
    BBButton *pTarget = new BBButton();
    if (pTarget && pTarget->initWithContentSize(contentSize,
                                                target,
                                                touchUpInsideCallback,
                                                pBgNormalFileName,
                                                pBgHighlightFileName,
                                                pImageNormalFileName,
                                                pImageHighlightFileName,
                                                pTitleText,
                                                isLoadImageFromLocal,
                                                pPriority)) {
        pTarget->autorelease();
        return pTarget;
    }
    CC_SAFE_DELETE(pTarget);
    return NULL;
}

bool BBButton::initWithContentSize(cocos2d::CCSize contentSize,
                                   cocos2d::CCObject *target,
                                   SEL_BBButtonHandle touchUpInsideCallback,
                                   const char *pBgNormalFileName,
                                   const char *pBgHighlightFileName,
                                   const char *pImageNormalFileName,
                                   const char *pImageHighlightFileName,
                                   const char *pTitleText,
                                   bool isLoadImageFromLocal,
                                   int pPriority) {
    
    if (!CCLayer::init()) {
        return false;
    }
    
    mDependedTarget = NULL;
    mBgImageSprite = NULL;
    mImageSprite = NULL;
    mLabel = NULL;
    mIsLoadImageFromLocal = isLoadImageFromLocal;
    mPriority = pPriority;
    
    mBgNormalFileName = "";
    mBgHighlightFileName = "";
    mImageNormalFileName = "";
    mImageHighlightFileName = "";
    
    mImageLocationFixedX = -1;
    
    this->ignoreAnchorPointForPosition(false);
    this->setContentSize(contentSize);
    
    this->mListener = target;
    setTouchUpInsideHandle(touchUpInsideCallback);
    
    mNormalColor = ccWHITE;
    mSelectedColor = ccGRAY;
    mDisableColor = mNormalColor;
    
    setBackgroundImage(pBgNormalFileName, pBgHighlightFileName);
    setImage(pImageNormalFileName, pImageHighlightFileName);
    
    mLabel = CCLabelTTF::create(NULL == pTitleText ? "" : pTitleText, "Marker Felt", 50, contentSize, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    mLabel->setPosition(ccp(contentSize.width * 0.5, contentSize.height * 0.5));
    this->addChild(mLabel, 2);
    
    //默认可以点击
    this->setButtonEnable(true);
    this->setSelected(false);
    
    if (mImageSprite) {
        setImageLabelInterval(10);
    }
    
    return true;
}

void BBButton::setTouchDownHandle(SEL_BBButtonHandle touchDownCallback) {
    this->mTouchDownFunc = touchDownCallback;
}

void BBButton::setTouchUpInsideHandle(SEL_BBButtonHandle touchUpInsideCallback) {
    this->mTouchUpInsideFunc = touchUpInsideCallback;
}

void BBButton::setBackgroundImage(const char *pBgNormalFileName,
                                  const char *pBgHighlightFileName) {
    
    if (mBgImageSprite && mBgImageSprite->getParent()) {
        mBgImageSprite->removeFromParent();
        mBgImageSprite = NULL;
    }
    if (pBgNormalFileName && strlen(pBgNormalFileName)) {
        
        mBgNormalFileName = NULL_TO_EMPTY(pBgNormalFileName);
        mBgHighlightFileName = NULL_TO_EMPTY(pBgHighlightFileName);
        
        if (mIsLoadImageFromLocal) {
            mBgImageSprite = CCSprite::create(pBgNormalFileName);
        } else {
            mBgImageSprite = CCSprite::createWithSpriteFrameName(pBgNormalFileName);
        }
        
        mBgImageSprite->setPosition(ccp(getContentSize().width * 0.5, getContentSize().height * 0.5));
        this->addChild(mBgImageSprite, 0);
    }
}

void BBButton::setImage(const char *pImageNormalFileName,
                        const char *pImageHighlightFileName) {
    
    CCPoint preLoc = CCPoint(INT16_MIN, getContentSize().height * 0.5);
    if (mImageSprite && mImageSprite->getParent()) {
        preLoc = mImageSprite->getPosition();
        mImageSprite->removeFromParent();
        mImageSprite = NULL;
    }
    if (pImageNormalFileName && strlen(pImageNormalFileName)) {
        
        if (INT16_MIN == preLoc.x) {
            preLoc.x = mImageLocationFixedX >= 0 ? mImageLocationFixedX : getContentSize().width * 0.5;
        }
        
        mImageNormalFileName = NULL_TO_EMPTY(pImageNormalFileName);
        mImageHighlightFileName = NULL_TO_EMPTY(pImageHighlightFileName);
        
        if (mIsLoadImageFromLocal) {
            mImageSprite = CCSprite::create(pImageNormalFileName);
        } else {
            mImageSprite = CCSprite::createWithSpriteFrameName(pImageNormalFileName);
        }
        mImageSprite->setPosition(preLoc);
        this->addChild(mImageSprite, 1);
    }
}

void BBButton::setImageLabelInterval(float pInterval) {
    
    if (mImageSprite) {
        
        CCLabelTTF *widthLabel = CCLabelTTF::create(mLabel->getString(), mLabel->getFontName(), mLabel->getFontSize());
        float allWidth = widthLabel->getContentSize().width;
        allWidth += mImageSprite->getContentSize().width + pInterval;
        
        float imageX = (getContentSize().width - allWidth) * 0.5 + mImageSprite->getContentSize().width * 0.5;
        float labelX = imageX + mImageSprite->getContentSize().width * 0.5 + pInterval + widthLabel->getContentSize().width * 0.5;
        mLabel->setPosition(ccp(labelX, getContentSize().height * 0.5));
        mImageSprite->setPosition(ccp(imageX, getContentSize().height * 0.5));
    }
}

void BBButton::setNormalColor(ccColor3B normalColor) {
    
    mNormalColor = normalColor;
    if (mEnable && !mSelected) {
        mLabel->setColor(mNormalColor);
    }
}

void BBButton::setSelectedColor(ccColor3B selectedColor) {
    
    mSelectedColor = selectedColor;
    if (mEnable && mSelected) {
        mLabel->setColor(mSelectedColor);
    }
}

void BBButton::setDisabledColor(ccColor3B disableColor) {
    
    mDisableColor = disableColor;
    if (!mEnable) {
        mLabel->setColor(mDisableColor);
    }
}

#pragma mark - Touch

void BBButton::registerWithTouchDispatcher() {
    (CCDirector::sharedDirector()->getTouchDispatcher())->addTargetedDelegate(this, mPriority, true);
}

void BBButton::onExit() {
    CCLayer::onExit();
    (CCDirector::sharedDirector()->getTouchDispatcher())->removeDelegate(this);
}

bool BBButton::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
    if (mEnable) {
        
        if (mDependedTarget && !mDependedTarget->isTouchEnabled()) {
            return false;
        }
        CCPoint touchedPoint = convertTouchToNodeSpace(pTouch);
        CCRect rect = CCRectMake(0, 0, getContentSize().width, getContentSize().height);
        if (rect.containsPoint(touchedPoint)) {
            this->setSelected(true);
            if (mListener && mTouchDownFunc) {
                (this->mListener->*mTouchDownFunc)(this->getTag());
            }
            return true;
        }
    }
    return false;
}

void BBButton::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
    CCLOG("BBButton::ccTouchEnded, tag: %d", getTag());
    if (mEnable && this->mSelected) {
        this->setSelected(false);
        
        CCPoint touchedPoint = convertTouchToNodeSpace(pTouch);
        CCRect rect = CCRectMake(0, 0, getContentSize().width, getContentSize().height);
        
        if (rect.containsPoint(touchedPoint)) {
            CCLOG("ccTouchEnded touch validate, tag : %d", this->getTag());
            if (mListener && mTouchUpInsideFunc) {
                (this->mListener->*mTouchUpInsideFunc)(this->getTag());
            }
        }
    }
}

#pragma mark - Enable
void BBButton::setButtonEnable(bool enable) {
    mEnable = enable;
    this->setTouchEnabled(enable);
    mLabel->setColor(enable ?  mNormalColor : mDisableColor);
}

bool BBButton::getButtonEnable() {
    return mEnable;
}

#pragma mark - Selected
bool BBButton::getSelected() {
    return mSelected;
}

void BBButton::setSelected(bool isSelected) {
    
    if (mSelected == isSelected) {
        return;
    }
    
    mSelected = isSelected;
    
    if (mSelected) {
        mLabel->setColor(mSelectedColor);
        if (mBgImageSprite && !mBgHighlightFileName.empty()) {
            if (mIsLoadImageFromLocal) {
                mBgImageSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage(mBgHighlightFileName.c_str()));
            } else {
                CCLOG("mBgHighlightFileName : %s", mBgHighlightFileName.c_str());
                CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(mBgHighlightFileName.c_str());
                if (frame) {
                    mBgImageSprite->setDisplayFrame(frame);
                }
            }
        }
        if (mImageSprite && !mImageHighlightFileName.empty()) {
            if (mIsLoadImageFromLocal) {
                mImageSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage(mImageHighlightFileName.c_str()));
            } else {
                CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(mImageHighlightFileName.c_str());
                if (frame) {
                    mImageSprite->setDisplayFrame(frame);
                }
            }
        }
    }
    else {
        mLabel->setColor(mNormalColor);
        if (mBgImageSprite && mBgNormalFileName.c_str()) {
            if (mIsLoadImageFromLocal) {
                mBgImageSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage(mBgNormalFileName.c_str()));
            } else {
                CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(mBgNormalFileName.c_str());
                if (frame) {
                    mBgImageSprite->setDisplayFrame(frame);
                }
            }
        }
        if (mImageSprite && mImageNormalFileName.c_str()) {
            if (mIsLoadImageFromLocal) {
                mImageSprite->setTexture(CCTextureCache::sharedTextureCache()->addImage(mImageNormalFileName.c_str()));
            } else {
                CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(mImageNormalFileName.c_str());
                if (frame) {
                    mImageSprite->setDisplayFrame(frame);
                }
            }
        }
    }
}

#pragma mark - ImageLocation Fixed
void BBButton::setImageFixedLocation(float x) {
    
    mImageLocationFixedX = x;
    mImageSprite->setPositionX(x);
    
    float sideOffset = 10;
    float width = 0;
    float labelX = 0;
    if (x > getContentSize().width * 0.5) {
        
        x -= mImageSprite->getContentSize().width * 0.5 + sideOffset;
        width = x - sideOffset;
        labelX = sideOffset + width * 0.5;
        mLabel->setHorizontalAlignment(kCCTextAlignmentRight);
    }
    else {
        x += mImageSprite->getContentSize().width * 0.5 + sideOffset;
        width = getContentSize().width - x - sideOffset;
        labelX = x + width * 0.5;
        mLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
    }
    mLabel->setPositionX(labelX);
    mLabel->setDimensions(CCSize(width, mLabel->getContentSize().height));
}

