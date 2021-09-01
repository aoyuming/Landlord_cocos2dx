#pragma once 

#include "cocos2d.h"
USING_NS_CC;


class CTimer : public cocos2d::Layer
{
public:

	bool m_bFlags;
	int m_iTimeMax;//����ʱ
	int m_iDir;//����
	Label* m_LabelTime;//ʱ������
	Sprite* m_SpriteClock;//ʱ��ͼƬ

	
	CTimer(const char* fn,int dir,Node* praent);
	~CTimer();

	
	//��ʼ��ʱ
	void startTimer(int timeMax);

	//ֹͣ��ʱ
	void stopTimer();

	//�ص�����
	void TimeCallBackFun(float ft);

};
