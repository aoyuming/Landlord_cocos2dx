#include "timer.h"


CTimer::CTimer(const char* fn,int dir,Node* parent)
	:
	m_iDir(dir),
	m_iTimeMax(-1),
	m_bFlags(false)
{
	Point p = Point::ZERO;
	Size size = Director::getInstance()->getWinSize();
	switch( dir )
	{
	case 0://下
		{
			p.x = size.width / 2;
			p.y = 300;
		}break;
	case 1://右
		{
			p.x = size.width - 250;
			p.y = size.height / 2 + 150;
		}break;
	case 2://左
		{
			p.x = 250;
			p.y = size.height / 2 + 150;
		}break;
	}
	m_SpriteClock = Sprite::create(fn);
	m_SpriteClock->setPosition(p);
	m_SpriteClock->setVisible(false);
	
	m_LabelTime = Label::create("0","",24);
	m_LabelTime->setPosition(m_SpriteClock->getPosition());
	m_LabelTime->setTextColor(ccc4(255,0,0,0));
	m_LabelTime->setVisible(false);

	addChild(m_SpriteClock);
	addChild(m_LabelTime);
	parent->addChild(this);

}
CTimer::~CTimer()
{
	this->removeFromParent();
}

//开始计时
void CTimer::startTimer(int timeMax)
{
	m_iTimeMax = timeMax;
	m_bFlags = false;
	char buf[20];
	_itoa(m_iTimeMax,buf,10);
	m_LabelTime->setString(buf);
	m_SpriteClock->setVisible(true);
	m_LabelTime->setVisible(true);
	
	this->schedule(schedule_selector(CTimer::TimeCallBackFun),1);
}
//停止计时
void CTimer::stopTimer()
{
	m_iTimeMax = -1;
	m_bFlags = false;
	m_SpriteClock->setVisible(false);
	m_LabelTime->setVisible(false);
	this->unschedule(schedule_selector(CTimer::TimeCallBackFun));
}
//每一秒执行一次
void CTimer::TimeCallBackFun(float ft)
{
	if( m_iTimeMax == 1)
	{
		m_iTimeMax = -1;
		m_bFlags = true;//(m_CallBackFun)();//回调函数
		m_SpriteClock->setVisible(false);
		m_LabelTime->setVisible(false);
		this->unschedule(schedule_selector(CTimer::TimeCallBackFun));
		return;
	}

	m_iTimeMax -= 1;
	char buf[20];
	_itoa(m_iTimeMax,buf,10);
	m_LabelTime->setString(buf);
}
