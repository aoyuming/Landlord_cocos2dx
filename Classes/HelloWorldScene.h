#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include "PopupLayer.h"
#include "cocos2d.h"
#include "player.h"
#include "public.h"
#include <time.h>
#include "ui\CocosGUI.h"
#include "GameLogic.h"

#define PLAYER_DOWN  0 //下方玩家在数组的下标
#define PLAYER_RIGHT 1 //右方玩家在数组的下标
#define PLAYER_LEFT  2 //左方玩家在数组的下标

//using namespace CocosDenshion;
using namespace std;
using namespace cocos2d::ui;




USING_NS_CC;
class HelloWorld : public cocos2d::Layer
{

public:

	vector<CPlayer*> m_vPlayer;
	vector<Point> m_PressVector;

	//总共的牌
	vector<enCard> m_AllCardVect;
	//底牌
	vector<CPlayer::stCardSpr> m_vBackCard;

	CGameLogic* m_pLogic;//逻辑
	stResult m_TempResult;//结果

	CPlayer::enDir m_CryDir;//记录当前叫地主的是哪个方向的
	CPlayer::enDir m_RobDir;//记录当前抢地主的是哪个方向的

	//是否发牌
	bool m_bDeal;
	int m_iQiangDiZhu; //抢地主的人数

	//按下的点 和移动的点
	Point m_PressLeftPoint;
	Point m_PressRightPoint;
	Point m_PressPoint;
	Point m_MovePoint;
	Point m_EndPoint;
	bool  m_bDownAi;
	bool  m_bTouchPress;
	bool  m_bTouchMove;
	bool  m_bTouchEnd;
	bool  m_bFlags;


	Button* m_ButtonChuPai;//出牌
	Button* m_ButtonBuYao;//不要
	Button* m_ButtonTiShi;//提示
	Button* m_ButtonOneScore;//1分
	Button* m_ButtonTwoScore;//2分
	Button* m_ButtonThreeScore;//3分
	Button* m_ButtonCryLandlord;//叫地主
	Button* m_ButtonNoCryLandlord;//不叫
	Button* m_ButtonCobLandlord;//抢地主
	Button* m_ButtonNoCobLandlord;//不抢
	Button* m_ButtonDouble;//加倍
	Button* m_ButtonNoDouble;//不加倍
	Button* m_ButtonAnew;//从玩


	
	//玩家替换牌
	void tihuanCard(CPlayer* player);

	//显示底牌
	void showBackCard(vector<CPlayer::stCardSpr>& vect,bool show);

	//一键重新发牌
	void anewSendCard();

	//设置底牌
	void setBaseCard(vector<enCard>& vect);

	//发牌
	void deal(float dt);

	//加载背景图片
	void loadBack(const char* fn);

	//按钮 出牌 不要 提示 的回调函数
	void ButtonClick_ChuPai(Ref* node);
	void ButtonClick_BuYao(Ref* node);
	void ButtonClick_TiShi(Ref* node);

	//按钮 叫地主 不叫 抢地主 的回调函数
	void ButtonClick_CryLandlord(Ref* node);
	void ButtonClick_NoCryLandlord(Ref* node);
	void ButtonClick_CobLandlord(Ref* node);

	//按钮 不抢 加倍 不加倍  的回调函数
	void ButtonClick_NoCobLandlord(Ref* node);
	void ButtonClick_Double(Ref* node);
	void ButtonClick_NoDouble(Ref* node);

	void ButtonClick_Anew(Ref* node);

	//设置按钮位置
	void setButtonPos(Button* button,int i,int j);

	//设置按钮的回调函数
	void setButtonCallBcak(Button* button,int i,int j);

	//设置按钮Visible
	void setButtonVisible(CPlayer::enState state);

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	virtual void onEnter();
	virtual void onExit();
	virtual void update(float fd);

    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
