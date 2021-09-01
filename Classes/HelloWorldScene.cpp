#include "HelloWorldScene.h"
#include <string>
#include "logic.h"
#include <windows.h>

//#include <windows.h>

using namespace std;

USING_NS_CC;



Scene* scene;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

//加载背景图片
void HelloWorld::loadBack(const char* fn)
{
	auto sprite = Sprite::create(fn);
	sprite->setAnchorPoint(Point(0,0));
	sprite->setPosition(Point::ZERO);
	addChild(sprite);

}
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	srand((unsigned int)time(0));

	//加载背景图片
	//loadBack("PIC//board.png");
	loadBack("PIC//back.png");

	Size winsize = Director::getInstance()->getWinSize();
	Sprite* spreite = Sprite::create("PIC//liaotiankuang.png");
	spreite->setAnchorPoint(Point(0,0));
	spreite->setPosition(Point(winsize.width - 226,0));
	addChild(spreite);

	//加载音乐
	//LoadDirectory("sound");

	//设置牌的属性
	for( int i = 0; i < 54; ++i )
		m_AllCardVect.push_back((enCard)i);

	//设置底牌
	setBaseCard(m_AllCardVect);

	//发牌一秒给没个玩家发3张牌的回调函数
	this->schedule(schedule_selector(HelloWorld::deal),0.2f);

	CPlayer* player1 = new CPlayer("9744","nimabi22","王大锤",CPlayer::enDir::DOWN,CPlayer::enState::en_Wait,5000,50,true,this);
	CPlayer* player2 = new CPlayer("1233","nimabi22","王小妹",CPlayer::enDir::RIGTH,CPlayer::enState::en_Wait,5000,50,false,this);
	CPlayer* player3 = new CPlayer("233","nimabi22","王尼妹",CPlayer::enDir::LEFT,CPlayer::enState::en_Wait,5000,50,false,this);

	//player1->cardPosUpdate(m_Player[0]->m_Dir);
	//player2->cardPosUpdate(m_Player[1]->m_Dir);
	//player3->cardPosUpdate(m_Player[2]->m_Dir);

	m_vPlayer.push_back(player1);
	m_vPlayer.push_back(player2);
	m_vPlayer.push_back(player3);

	//3个 按钮 不要  出牌  提示 
	Button** button[3][3] = 
	{
		{&m_ButtonChuPai,&m_ButtonBuYao,&m_ButtonTiShi},
		{&m_ButtonCryLandlord,&m_ButtonNoCryLandlord,&m_ButtonCobLandlord},
		{&m_ButtonNoCobLandlord,&m_ButtonDouble,&m_ButtonNoDouble}
	};
	
	////按钮的回调函数指针数组
	//void (HelloWorld::*pFun[4][3])(Ref*) = 
	//{
	//	{&HelloWorld::ButtonClick_ChuPai,&HelloWorld::ButtonClick_BuYao,&HelloWorld::ButtonClick_TiShi},
	//	{&HelloWorld::ButtonClick_OneScore,&HelloWorld::ButtonClick_TwoScore,&HelloWorld::ButtonClick_ThreeScore},
	//	{&HelloWorld::ButtonClick_CryLandlord,&HelloWorld::ButtonClick_NoCryLandlord,&HelloWorld::ButtonClick_CobLandlord},
	//	{&HelloWorld::ButtonClick_NoCobLandlord,&HelloWorld::ButtonClick_Double,&HelloWorld::ButtonClick_NoDouble}
	//};

	//按钮上的字
	char* str[3][3] = 
	{ 
		{"出牌","不要","提示"},
		{"叫地主","不叫","抢地主"},
		{"不抢","加倍","不加倍"}
	};

	Size size = Director::getInstance()->getWinSize();

	for( int i = 0; i < 3; ++i)
	{
		for( int j = 0; j < 3; ++j )
		{
			string s = str[i][j];
			GBKToUTF8(s,"gb2312","utf-8");
			(*button[i][j]) = Button::create();
			(*button[i][j])->loadTextureNormal("PIC//button//putong.png");
			(*button[i][j])->loadTexturePressed("PIC//button//anxia.png");
			(*button[i][j])->loadTextureDisabled("PIC//button//jinyong.png");
			(*button[i][j])->setAnchorPoint(Point(0,0));
			(*button[i][j])->setScale9Enabled(true);
			(*button[i][j])->setContentSize(Size(65,30));
			(*button[i][j])->setTitleColor(ccc3(255,255,255));
			(*button[i][j])->setTitleText(s.c_str());
			(*button[i][j])->setTitleFontSize(15);
		//	button[i]->setTitleFontName("fonts//arial.ttf");
			(*button[i][j])->setPressedActionEnabled(true);
			(*button[i][j])->setZoomScale(0.1);
			//(*button[i][j])->addClickEventListener(CC_CALLBACK_1(pFun[i][j],this));
			setButtonCallBcak(*button[i][j],i,j);//设置回调函数
			setButtonPos((*button[i][j]),i,j);//设置位置
			addChild((*button[i][j]));
		}
	}

	string s = "新一局";
	GBKToUTF8(s,"gb2312","utf-8");
	m_ButtonAnew = Button::create();
	m_ButtonAnew->loadTextureNormal("PIC//button//buchu1.png");
	m_ButtonAnew->loadTexturePressed("PIC//button//buchu3.png");
	m_ButtonAnew->loadTextureDisabled("PIC//button//buchu2.png");
	m_ButtonAnew->setTitleText(s.c_str());
	m_ButtonAnew->setTitleColor(ccc3(255,255,255));
	m_ButtonAnew->setTitleFontSize(15);
	m_ButtonAnew->setContentSize(Size(100,40));
	m_ButtonAnew->setScale9Enabled(true);
	m_ButtonAnew->setPressedActionEnabled(true);
	m_ButtonAnew->setZoomScale(0.1);
	m_ButtonAnew->setPosition(Point(winsize.width / 2 ,winsize.height / 2 + 40));
	m_ButtonAnew->addClickEventListener(CC_CALLBACK_1(HelloWorld::ButtonClick_Anew,this));
	m_ButtonAnew->setVisible(false);
	addChild(m_ButtonAnew);


	//1：创建多点触屏事件监听者
	auto pListener = EventListenerTouchAllAtOnce::create();
	//2: 设置监听的4个调用函数
	pListener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan,this);
	pListener->onTouchesMoved = CC_CALLBACK_2(HelloWorld::onTouchesMoved,this);
	pListener->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded,this);
	//3: 让事件分发器装载上面已经创建好的多点触屏事件监听者
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener,this);

	//逻辑
	m_pLogic = new CGameLogic();

	//触摸
	m_bTouchPress = false;
	m_bTouchMove = false;
	m_bTouchEnd = false;
	m_bFlags = false;
	m_bDownAi = false;

	string s1 = "小键盘1：增加AI速度";
	string s2 = "小键盘2：重新开始";
	string s3 = "小键盘3：进入托管模式";
	string s4 = "小键盘4：全部明牌";
	string s5 = "小键盘4：为自己添加3张底牌";


	GBKToUTF8(s1, "gb2312", "utf-8");
	GBKToUTF8(s2, "gb2312", "utf-8");
	GBKToUTF8(s3, "gb2312", "utf-8");
	GBKToUTF8(s4, "gb2312", "utf-8");
	GBKToUTF8(s5, "gb2312", "utf-8");


	Label* l1 = Label::create(s1, "Marker Felt", 15);
	Label* l2 = Label::create(s2, "Marker Felt", 15);
	Label* l3 = Label::create(s3, "Marker Felt", 15);
	Label* l4 = Label::create(s4, "Marker Felt", 15);
	Label* l5 = Label::create(s5, "Marker Felt", 15);

	l1->setAnchorPoint(Point(0, 0));
	l2->setAnchorPoint(Point(0, 0));
	l3->setAnchorPoint(Point(0, 0));
	l4->setAnchorPoint(Point(0, 0));
	l5->setAnchorPoint(Point(0, 0));

	l1->setPosition(winsize.width - 200, 110);
	l2->setPosition(winsize.width - 200, 90);
	l3->setPosition(winsize.width - 200, 70);
	l4->setPosition(winsize.width - 200, 50);
	l5->setPosition(winsize.width - 200, 30);

	addChild(l1);
	addChild(l2);
	addChild(l3);
	addChild(l4);
	addChild(l5);

    return true;
}

//设置底牌
void HelloWorld::setBaseCard(vector<enCard>& vect)
{
	int idx = 0;
	Size size = Director::getInstance()->getWinSize();
	int interval = 30; //间隔
	int cardLenth = 3 * 83*0.7 + interval*2;//3张牌的长度
	int count = cardLenth / 3;

	Point pos;
	pos.x = size.width / 2 - cardLenth / 2 + 20;
	pos.y = size.height - 90;
	for( vector<CPlayer::stCardSpr>::iterator it = m_vBackCard.begin(); it != m_vBackCard.end(); ++it )
		it->sprite->removeFromParent();//移除精灵

	m_vBackCard.clear();//清空

	//重新选3张牌
	for( int i = 0; i < (int)vect.size(); ++i )
	{
		vector<enCard>::iterator ik = vect.begin();
		int temp = rand() % vect.size();
		CPlayer::stCardSpr sp;
		sp.card = vect[temp];
		sp.sprite = Sprite::create("PIC//card_back.png");
		sp.sprite->setScale(0.7,0.7);
		sp.sprite->setAnchorPoint(Point(0,0));
		sp.sprite->setPosition(ccp(idx*count + pos.x,pos.y));
		sp.sprite->setVisible(true);
		addChild(sp.sprite);
		m_vBackCard.push_back(sp);
		vect.erase(ik + temp);

		if( idx == 2 )
			break;
		else
			idx++;
	}

	m_bDeal = true;
}
//显示底牌
void HelloWorld::showBackCard(vector<CPlayer::stCardSpr>& vect,bool show)
{
	for( vector<CPlayer::stCardSpr>::iterator it = vect.begin(); it != vect.end(); ++it )
	{
		it->sprite->setTexture(show ? "PIC//cards.png" : "PIC//card_back.png");
		it->sprite->setTextureRect(show ? Rect((int)it->card * 82,0,82,114) : Rect(0,0,82,114));
	}
}
//发牌
void HelloWorld::deal(float dt)
{
	if( m_bDeal )//发牌？
	{
		if( m_AllCardVect.size() == 0 )
		{
			m_bDeal = false;
			m_pLogic->anew(m_vPlayer);
			//拆牌
			for( int i = 0; i < 3; ++i )
			{
				vector<enCard> vect;
				for( int j = 0; j < m_vPlayer[i]->m_vCardVect.size(); ++j )
					vect.push_back(m_vPlayer[i]->m_vCardVect[j]->card);
				m_vPlayer[i]->unseamCard(vect);
			}

			return;
		}

		for( int i = 0; i < 3; ++i )
		{
			int rd = rand() % m_AllCardVect.size();
			bool Flags = (m_vPlayer[i]->m_Dir == CPlayer::enDir::DOWN);
			vector<enCard>::iterator it = m_AllCardVect.begin();
			CPlayer::stCardSpr* cardSpr = new CPlayer::stCardSpr;
			cardSpr->layerColor = LayerColor::create(ccc4(0x00,0x00,0xff,0x88));
			cardSpr->layerColor->changeHeight(114);
			cardSpr->layerColor->changeWidth(82);
			cardSpr->card = m_AllCardVect[rd];
			cardSpr->opt  = false;
			cardSpr->popup = false;
			cardSpr->sprite = Sprite::create( Flags ? "PIC//cards.png" : "PIC//card_back.png");
			cardSpr->sprite->setAnchorPoint(Point(0,0));
			cardSpr->sprite->setTextureRect(Flags ? Rect((int)m_AllCardVect[rd] * 82,0,82,114) : Rect(0,0,82,114));
			m_vPlayer[i]->m_vCardVect.push_back(cardSpr);
			m_vPlayer[i]->m_vDeleteCardVect.push_back(cardSpr);
			m_AllCardVect.erase(it + rd);
			cardSpr->layerColor->setVisible(false);
			this->addChild(cardSpr->sprite,1);
			this->addChild(cardSpr->layerColor,1);

			if( !Flags )
			{
				cardSpr->layerColor->setScale(0.8f,0.8f);
				cardSpr->sprite->setScale(0.8f,0.8f);
			}
			//排列
			m_vPlayer[i]->cardPosUpdate(m_vPlayer[i]->m_Dir);
		}
		 //播放音效
		 SimpleAudioEngine::getInstance()->playEffect(FAPAI_MUSIC);
	}
}

//设置按钮的回调函数
void HelloWorld::setButtonCallBcak(Button* button,int i,int j)
{
	if( i == 0 )
	{
		if( j == 0)
			(button)->addClickEventListener(CC_CALLBACK_1(HelloWorld::ButtonClick_ChuPai,this));
		else if( j == 1)
			(button)->addClickEventListener(CC_CALLBACK_1(HelloWorld::ButtonClick_BuYao,this));
		else if( j == 2)
			(button)->addClickEventListener(CC_CALLBACK_1(HelloWorld::ButtonClick_TiShi,this));
	}
	else if( i == 1 )
	{
		if( j == 0)
			(button)->addClickEventListener(CC_CALLBACK_1(HelloWorld::ButtonClick_CryLandlord,this));
		else if( j == 1)
			(button)->addClickEventListener(CC_CALLBACK_1(HelloWorld::ButtonClick_NoCryLandlord,this));
		else if( j == 2)
			(button)->addClickEventListener(CC_CALLBACK_1(HelloWorld::ButtonClick_CobLandlord,this));
	}
	else if( i == 2 )
	{
		if( j == 0)
			(button)->addClickEventListener(CC_CALLBACK_1(HelloWorld::ButtonClick_NoCobLandlord,this));
		else if( j == 1)
			(button)->addClickEventListener(CC_CALLBACK_1(HelloWorld::ButtonClick_Double,this));
		else if( j == 2)
			(button)->addClickEventListener(CC_CALLBACK_1(HelloWorld::ButtonClick_NoDouble,this));
	}
}

//设置按钮位置
void HelloWorld::setButtonPos(Button* button,int i,int j)
{
	if( button == NULL )
		return ;

	Size size = Director::getInstance()->getVisibleSize();
	Point pos = Point(0,0);
	int y = size.height / 2 - 120;

	if( i == 0 )
	{
		if( j == 0)
			pos = Point(size.width / 2 - 80 - 80,y);
		else if( j == 1)
			pos = Point(size.width / 2 - 40,y);
		else if( j == 2)
			pos = Point(size.width / 2 + 80,y);
	}
	else if( i == 1 )
	{
		if( j == 0)
			pos = Point(size.width / 2 - 80 - 60,y);
		else if( j == 1)
			pos = Point(size.width / 2 + 60,y);
		else if( j == 2)
			pos = Point(size.width / 2 - 80 - 60,y);
	}
	else if( i == 2 )
	{
		if( j == 0)
			pos = Point(size.width / 2 + 60,y);
		else if( j == 1)
			pos = Point(size.width / 2 - 80 - 60,y);
		else if( j == 2)
			pos = Point(size.width / 2 + 80,y);
	}
	button->setPosition(pos);
}


//叫地主  
void HelloWorld::ButtonClick_CryLandlord(Ref* node)
{
	m_vPlayer[PLAYER_DOWN]->CryLoadlord(true);
	m_vPlayer[PLAYER_DOWN]->stopTimer();
	m_pLogic->CryLoadlord(true,m_vPlayer[PLAYER_DOWN],m_vPlayer[PLAYER_RIGHT],m_vPlayer[PLAYER_LEFT],m_vBackCard);

}
//不叫地主
void HelloWorld::ButtonClick_NoCryLandlord(Ref* node)
{
	m_vPlayer[PLAYER_DOWN]->CryLoadlord(false);
	m_vPlayer[PLAYER_DOWN]->stopTimer();
	if (m_pLogic->CryLoadlord(false,m_vPlayer[PLAYER_DOWN],m_vPlayer[PLAYER_RIGHT],m_vPlayer[PLAYER_LEFT],m_vBackCard))
		anewSendCard();
}
//抢地主
void HelloWorld::ButtonClick_CobLandlord(Ref* node)
{
	m_vPlayer[PLAYER_DOWN]->RobLoadlord(true);
	m_vPlayer[PLAYER_DOWN]->stopTimer();
	m_pLogic->RobLoadlord(true,m_vPlayer[PLAYER_DOWN],m_vPlayer[PLAYER_RIGHT],m_vPlayer[PLAYER_LEFT],m_vBackCard);

}
//不抢
void HelloWorld::ButtonClick_NoCobLandlord(Ref* node)
{
	m_vPlayer[PLAYER_DOWN]->RobLoadlord(false);
	m_vPlayer[PLAYER_DOWN]->stopTimer();
	m_pLogic->RobLoadlord(false,m_vPlayer[PLAYER_DOWN],m_vPlayer[PLAYER_RIGHT],m_vPlayer[PLAYER_LEFT],m_vBackCard);
}
//加倍
void HelloWorld::ButtonClick_Double(Ref* node)
{
	m_vPlayer[PLAYER_DOWN]->Double(true);
	m_vPlayer[PLAYER_DOWN]->stopTimer();
	m_pLogic->Double(m_vPlayer[PLAYER_DOWN],m_vPlayer[PLAYER_RIGHT],m_vPlayer[PLAYER_LEFT]);
	
}
//不加倍
void HelloWorld::ButtonClick_NoDouble(Ref* node)
{
	m_pLogic->Double(m_vPlayer[PLAYER_DOWN],m_vPlayer[PLAYER_RIGHT],m_vPlayer[PLAYER_LEFT]);
	m_vPlayer[PLAYER_DOWN]->stopTimer();
	m_vPlayer[PLAYER_DOWN]->Double(false);

}
//过
void HelloWorld::ButtonClick_BuYao(Ref* node)
{
	m_vPlayer[PLAYER_DOWN]->stopTimer();
	m_vPlayer[PLAYER_DOWN]->noDenmand();
	m_pLogic->noDenmand(m_vPlayer[PLAYER_DOWN],m_vPlayer[PLAYER_RIGHT],m_vPlayer[PLAYER_LEFT]);
}
//提示
void HelloWorld::ButtonClick_TiShi(Ref* node)
{
	stPlayerData nextPlayer,lastPlayer,nowPlayer;
	nextPlayer.cardCount = m_vPlayer[PLAYER_RIGHT]->m_vCardVect.size();
	nextPlayer.dir		 = m_vPlayer[PLAYER_RIGHT]->m_Dir;
	nextPlayer.landlord  = m_vPlayer[PLAYER_RIGHT]->m_bLandlord;

	lastPlayer.cardCount = m_vPlayer[PLAYER_LEFT]->m_vCardVect.size();
	lastPlayer.dir		 = m_vPlayer[PLAYER_LEFT]->m_Dir;
	lastPlayer.landlord  = m_vPlayer[PLAYER_LEFT]->m_bLandlord;

	nowPlayer.cardCount	 = m_vPlayer[PLAYER_DOWN]->m_vCardVect.size();
	nowPlayer.dir		 = m_vPlayer[PLAYER_DOWN]->m_Dir;
	nowPlayer.landlord   = m_vPlayer[PLAYER_DOWN]->m_bLandlord;

	if( !m_vPlayer[PLAYER_DOWN]->CPlayer::hint(m_pLogic->m_Result,lastPlayer,nowPlayer,nextPlayer,m_pLogic->m_iAppearCardDir,true) 
		&& m_vPlayer[0]->m_enState != CPlayer::enState::en_ChuPai  )
	{
		m_vPlayer[PLAYER_DOWN]->stopTimer();
		m_pLogic->noDenmand(m_vPlayer[PLAYER_DOWN],m_vPlayer[PLAYER_RIGHT],m_vPlayer[PLAYER_LEFT]);
	}
}
//出牌
void HelloWorld::ButtonClick_ChuPai(Ref* node)
{
	if( !m_vPlayer[PLAYER_DOWN]->appearCard(m_pLogic->m_Result) )
		return;

	m_vPlayer[PLAYER_DOWN]->stopTimer();
	CGameLogic::enResult en = m_pLogic->appearCard(m_vPlayer[PLAYER_DOWN],m_vPlayer[PLAYER_RIGHT],m_vPlayer[PLAYER_LEFT],m_vPlayer[PLAYER_DOWN]);
	if( en == CGameLogic::enResult::en_Win )
	{
		m_ButtonAnew->setVisible(true);
		SimpleAudioEngine::getInstance()->playBackgroundMusic(BGM_WIN_MUSICE,false);
	}
	else if(en == CGameLogic::enResult::en_Lose )
	{
		m_ButtonAnew->setVisible(true);
		SimpleAudioEngine::getInstance()->playBackgroundMusic(BGM_LOSE_MUSICE,false);
	}
	else if(en == CGameLogic::enResult::en_New )
		anewSendCard();

}
void HelloWorld::ButtonClick_Anew(Ref* node)
{
	anewSendCard();
}
//一键重新发牌
void HelloWorld::anewSendCard()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic(BGM_ZHENGCHANG_MUSICE,true);
	m_AllCardVect.clear(); 
	m_pLogic->Init();
	m_ButtonAnew->setVisible(false); 

	//设置牌的属性
	for( int i = 0; i <= 53; i += 1 )
		m_AllCardVect.push_back((enCard)i);

	for( int i = 0; i < 3; ++i )
		m_vPlayer[i]->stopTimer();

	setBaseCard(m_AllCardVect);//设置底牌
	m_vPlayer[PLAYER_DOWN]->clear();
	m_vPlayer[PLAYER_RIGHT]->clear();
	m_vPlayer[PLAYER_LEFT]->clear();
	m_bDeal = true;
	//SimpleAudioEngine::getInstance()->playBackgroundMusic(BGM_ZHADAN_MUSICE,true);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
void HelloWorld::onEnter()
{
	Layer::onEnter();
	scheduleUpdate();
  //播放音乐
	SimpleAudioEngine::getInstance()->playBackgroundMusic(BGM_WELCOME_MUSICE,true);

  //播放音效
  // SimpleAudioEngine::getInstance()->playEffect(EFFECT_MUSIC)
}

void HelloWorld::onExit()
{
	delete m_vPlayer[PLAYER_DOWN];
	delete m_vPlayer[PLAYER_RIGHT];
	delete m_vPlayer[PLAYER_LEFT];
	delete m_pLogic;

	unscheduleUpdate();
	Layer::onExit();
}

//设置按钮Visible
void HelloWorld::setButtonVisible(CPlayer::enState state)
{
	m_ButtonChuPai->setVisible(false);
	m_ButtonBuYao->setVisible(false);
	m_ButtonTiShi->setVisible(false);
	m_ButtonCryLandlord->setVisible(false);//叫地主
	m_ButtonNoCryLandlord->setVisible(false);//不叫
	m_ButtonCobLandlord->setVisible(false);//抢地主
	m_ButtonNoCobLandlord->setVisible(false);//不抢
	m_ButtonDouble->setVisible(false);//加倍
	m_ButtonNoDouble->setVisible(false);//不加倍

	switch( state )
	{
	case CPlayer::enState::en_Wait://等待
		{
		}break;
	case CPlayer::enState::en_JiaoDiZhu://叫地主
		{
			m_ButtonCryLandlord->setVisible(true);//叫地主
			m_ButtonNoCryLandlord->setVisible(true);//不叫

		}break;
	case CPlayer::enState::en_QiangDiZhu://抢地主:
		{
			m_ButtonCobLandlord->setVisible(true);//抢地主
			m_ButtonNoCobLandlord->setVisible(true);//不抢

		}break;
	case CPlayer::enState::en_JiaBei://加倍
		{
			m_ButtonDouble->setVisible(true);
			m_ButtonNoDouble->setVisible(true);
		}break;
	case CPlayer::enState::en_ChuPai://出任意牌
		{
			m_ButtonChuPai->setVisible(true);
			m_ButtonTiShi->setVisible(true);
			m_ButtonBuYao->setVisible(true);
			m_ButtonBuYao->setEnabled(false);
			m_ButtonBuYao->setBright(false);
			m_ButtonBuYao->setTouchEnabled(false); 
		}break;
	case CPlayer::enState::en_GenPai://必须出指定的类型牌
		{
			m_ButtonChuPai->setVisible(true);
			m_ButtonTiShi->setVisible(true);
			m_ButtonBuYao->setVisible(true);
			m_ButtonBuYao->setEnabled(true);
			m_ButtonBuYao->setBright(true);
			m_ButtonBuYao->setTouchEnabled(true); 
		}break;
	}

}
//替玩家换牌
void HelloWorld::tihuanCard(CPlayer* player)
{
	vector<enCard> vect;
	//vect.push_back(Spandes_3);
	//vect.push_back(RedStar_3);
	//vect.push_back(Club_3);
	//vect.push_back(Spandes_4);
	//vect.push_back(RedStar_4);
	//vect.push_back(Club_4);
	//vect.push_back(Spandes_6);
	vect.push_back(Little_King);
	vect.push_back(Big_King);
	//vect.push_back(Spandes_10);
	//vect.push_back(Spandes_J);
	//vect.push_back(Spandes_Q);
	//vect.push_back(Spandes_10);
	//vect.push_back(Spandes_10);
	//vect.push_back(Spandes_Q);
	//vect.push_back(Spandes_K);
	//vect.push_back(Spandes_2);
	//vect.push_back(Spandes_2);
	//vect.push_back(Spandes_2);
	for( vector<CPlayer::stCardSpr*>::iterator it = player->m_vDeleteCardVect.begin(); it != player->m_vDeleteCardVect.end(); ++it )
	{
		(*it)->sprite->removeFromParent();
		(*it)->layerColor->removeFromParent();
		delete *it;
	}

	player->m_vDeleteCardVect.clear();
	player->m_vMoveVect.clear();
	player->m_vCardVect.clear();
	player->m_vSendVect.clear();
	player->m_vShowSendVect.clear();
	player->m_Label->setVisible(false);
	player->m_LabelUpString->setTextColor(ccc4(255,193,100,0));

	for( int i = 0 ; i < vect.size(); )
	{
		int rd = rand() % vect.size();
		bool Flags = (player->m_Dir == CPlayer::enDir::DOWN);
		vector<enCard>::iterator it = vect.begin();
		CPlayer::stCardSpr* cardSpr = new CPlayer::stCardSpr;
		cardSpr->layerColor = LayerColor::create(ccc4(0x00,0x00,0xff,0x88));
		cardSpr->layerColor->changeHeight(114);
		cardSpr->layerColor->changeWidth(82);
		cardSpr->card = vect[rd];
		cardSpr->opt  = false;
		cardSpr->popup = false;
		cardSpr->sprite = Sprite::create( Flags ? "PIC//cards.png" : "PIC//card_back.png");
		cardSpr->sprite->setAnchorPoint(Point(0,0));
		cardSpr->sprite->setTextureRect(Flags ? Rect((int)vect[rd] * 82,0,82,114) : Rect(0,0,82,114));
		player->m_vCardVect.push_back(cardSpr);
		player->m_vDeleteCardVect.push_back(cardSpr);
		vect.erase(it + rd);
		cardSpr->layerColor->setVisible(false);
		this->addChild(cardSpr->sprite,1);
		this->addChild(cardSpr->layerColor,1);

		if( !Flags )
		{
			cardSpr->layerColor->setScale(0.8f,0.8f);
			cardSpr->sprite->setScale(0.8f,0.8f);
		}
		//排列
		player->cardPosUpdate(player->m_Dir);
	}


}
void HelloWorld::update(float fd)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	setButtonVisible(m_vPlayer[PLAYER_DOWN]->m_enState);

	static int a = 0;
	static int max = 50;
	if( a == max )
	{
		a = 0;
		if( !m_bDeal )
		{
			CGameLogic::enResult re = m_pLogic->Run(m_vPlayer[PLAYER_DOWN],m_vPlayer[PLAYER_RIGHT],m_vPlayer[PLAYER_LEFT],m_vBackCard,m_bDownAi);
			switch( re )
			{
			case CGameLogic::enResult::en_New:anewSendCard();						break;
			case CGameLogic::enResult::en_Win:
				{
					m_ButtonAnew->setVisible(true);
					SimpleAudioEngine::getInstance()->playBackgroundMusic(BGM_WIN_MUSICE,false);
				}break;
			case CGameLogic::enResult::en_Lose:
				{
					m_ButtonAnew->setVisible(true);
					SimpleAudioEngine::getInstance()->playBackgroundMusic(BGM_LOSE_MUSICE,false);
				}break;
			}
			
		}
	
	}
	else
		a++;

	
	for( int i = 0; i < 3; ++i )
	{
		if( m_vPlayer[i]->m_bLandlord )	//有地主 显示底牌
			this->showBackCard(m_vBackCard,true);
		
		switch ( m_vPlayer[i]->Run() )
		{
		case CPlayer::enAiResult::en_NoCryLoadlord://不叫地主
			{
				m_vPlayer[i]->CryLoadlord(false);
				if( m_pLogic->CryLoadlord(false,m_vPlayer[PLAYER_DOWN],m_vPlayer[PLAYER_RIGHT],m_vPlayer[PLAYER_LEFT],m_vBackCard) )
				{
					anewSendCard();
					return;
				}
			}break;
		case CPlayer::enAiResult::en_NoRobLaodlord://不抢地主
			{
				m_vPlayer[i]->RobLoadlord(false);
				m_pLogic->RobLoadlord(false,m_vPlayer[PLAYER_DOWN],m_vPlayer[PLAYER_RIGHT],m_vPlayer[PLAYER_LEFT],m_vBackCard);
			}break;
		case CPlayer::enAiResult::en_NoDouble://不加倍
			{
				m_vPlayer[i]->Double(false);
				m_pLogic->Double(m_vPlayer[PLAYER_DOWN],m_vPlayer[PLAYER_RIGHT],m_vPlayer[PLAYER_LEFT]);
			}break;
		case CPlayer::enAiResult::en_NoDenmand://要不起
			{
				m_pLogic->noDenmand(m_vPlayer[PLAYER_DOWN],m_vPlayer[PLAYER_RIGHT],m_vPlayer[PLAYER_LEFT]);
				m_vPlayer[i]->noDenmand();
			}break;
		case CPlayer::enAiResult::en_AppearCard://出牌
			{
				if( !m_vPlayer[i]->appearCard(m_pLogic->m_Result) )
					continue;
				CGameLogic::enResult re = m_pLogic->appearCard(m_vPlayer[PLAYER_DOWN],m_vPlayer[PLAYER_RIGHT],m_vPlayer[PLAYER_LEFT],m_vPlayer[PLAYER_DOWN]) ;
				if( re == CGameLogic::enResult::en_Win )
					SimpleAudioEngine::getInstance()->playBackgroundMusic(BGM_WIN_MUSICE,false);
				else if(re == CGameLogic::enResult::en_Lose)
					SimpleAudioEngine::getInstance()->playBackgroundMusic(BGM_LOSE_MUSICE,false);
			}break;
		}
	}


	if( GetAsyncKeyState(VK_NUMPAD1) & 1)
	{
		max = max == 10 ? 50 : 10;
		a = 0;
	}
	if( GetAsyncKeyState(VK_NUMPAD2) & 1)
		anewSendCard();
	if( GetAsyncKeyState(VK_NUMPAD3) & 1)
		m_bDownAi = m_bDownAi ? false : true;

	if( GetAsyncKeyState(VK_NUMPAD4) & 1)
	{
		m_vPlayer[PLAYER_RIGHT]->showCard(true);
		m_vPlayer[PLAYER_LEFT]->showCard(true);
	}
	if( GetAsyncKeyState(VK_NUMPAD5) & 1)
	{
		m_vPlayer[PLAYER_DOWN]->setLandlord(m_vBackCard);
	}


	if( m_bTouchPress )//单击
	{
		m_bTouchPress = false;
		m_vPlayer[PLAYER_DOWN]->addInTowPointCardLayer(m_PressPoint,m_PressPoint);
	}
	if( m_bTouchMove )//移动
	{
		m_bTouchMove = false; 
		m_vPlayer[PLAYER_DOWN]->addInTowPointCardLayer(m_PressPoint,m_MovePoint);
	}

	if( m_bFlags )
	{
		m_bFlags = false;
		m_vPlayer[PLAYER_DOWN]->setCardInit();
		return;
	}
	if( m_bTouchEnd )//抬起
	{
		m_bTouchPress = m_bTouchMove = m_bTouchEnd = false;
		m_vPlayer[PLAYER_DOWN]->moveOptCard();
	}
}

void HelloWorld::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	for( int i = 0; i < (int)touches.size(); ++i )
	{
		Vec2 v = touches[i]->getLocation();
		m_PressLeftPoint = v;
		m_PressRightPoint = v;
		m_vPlayer[0]->getCardPoint(v,m_PressLeftPoint,m_PressRightPoint);
	}

	m_PressPoint = m_PressLeftPoint;
	m_bTouchPress = true;
}

void HelloWorld::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
	for( int i = 0; i < (int)touches.size(); ++i )
	{
		m_MovePoint = touches[i]->getLocation();
	}

	if( m_PressLeftPoint.x < m_MovePoint.x )
		m_PressPoint = m_PressLeftPoint;
	else
		m_PressPoint = m_PressRightPoint;

	m_bTouchMove = true;
}

void HelloWorld::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{

	Size visibleSize = Director::getInstance()->getVisibleSize();

	for( int i = 0; i < (int)touches.size(); ++i )
	{
		m_EndPoint = touches[i]->getLocation();
		m_bFlags = !m_vPlayer[0]->getCardPoint(m_EndPoint,Point(),Point());
	}
	m_bTouchEnd = true;
}