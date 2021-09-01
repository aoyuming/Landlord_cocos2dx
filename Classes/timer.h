#pragma once 

#include "cocos2d.h"
USING_NS_CC;


class CTimer : public cocos2d::Layer
{
public:

	bool m_bFlags;
	int m_iTimeMax;//倒计时
	int m_iDir;//方向
	Label* m_LabelTime;//时间数字
	Sprite* m_SpriteClock;//时钟图片

	
	CTimer(const char* fn,int dir,Node* praent);
	~CTimer();

	
	//开始计时
	void startTimer(int timeMax);

	//停止计时
	void stopTimer();

	//回调函数
	void TimeCallBackFun(float ft);

};
