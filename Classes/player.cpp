#include "player.h"

int		CPlayer::m_iMultiple          = 15;//倍数
int		CPlayer::m_iRobLoadlordLevel  = 1;//抢地主的声音等级
int		CPlayer::m_iFollowCardLevel   = 1;//跟牌的声音等级
int		CPlayer::m_iNoAskLevel        = 1;//要不起声音等级
int		CPlayer::m_iCardGreaterLevel  = 0;//出牌声音等级
Sprite* CPlayer::m_SpriteMultiple	  = NULL;//倍数图片
Label*  CPlayer::m_LabelMultiple	  = NULL;//倍数字体
enType  CPlayer::m_LandlordNoCryCard  = TYPE_DANPAI; //地主不要的牌


CPlayer::CPlayer(string userId,string passWord,string name,enDir dir,enState state,
				 int integral,int multiple,bool sex,Node* parent)
	:
	m_sUserId(userId),
	m_sPassWord(passWord),
	m_sName(name),
	m_Dir(dir),
	m_iJoyBean(integral),
	m_bSex(sex),
	m_bCardUpdate(false),
	m_bMoveCard(false),
	m_Praent(parent),
	m_fTime(3),
	m_bLandlord(false),
	m_bNoCryLoadlord(false),
	m_bShowCard(false),
	m_Result(stResult(TYPE_NULL,-1,0)),
	logic(state),
	m_Timer(new CTimer("PIC//time.png",(int)m_Dir,parent))
{
	Point figPos,upPos;
	Size winSize = Director::getInstance()->getWinSize();
	switch( dir )
	{
	case DOWN:
		{
			figPos.x = 10;
			figPos.y = 30;

		}break;
	case RIGTH:
		{
			figPos.x = winSize.width - 96;
			figPos.y = winSize.height - 200 - 70;

		}break;
	case LEFT:
		{
			figPos.x = 10;
			figPos.y = winSize.height - 200 - 70;

		}break;
	}

	//头上字体的位置
	upPos.y  = figPos.y + 210;
	upPos.x  = figPos.x + 40;

	//添加人物图像
	m_SpriteFigure = Sprite::create( sex ? "PIC//figure//man.png" : "PIC//figure//woman.png");
	m_SpriteFigure->setAnchorPoint(Point(0,0));
	m_SpriteFigure->setPosition(figPos);

	string str = "";
	m_Label = Label::create(str.c_str(),"MoShou.ttf",24);
	m_Label->setVisible(false);
	m_Label->setAnchorPoint(Point(0,0));
	m_Label->setTextColor(ccc4(255,242,0,0));

	//绘制人物名字
	GBKToUTF8(m_sName,"gb2312","utf-8");
	m_LabelName = Label::create(str.c_str(),"MoShou.ttf",14);
	m_LabelName->setAnchorPoint(Point(0,0));
	m_LabelName->setTextColor(ccc4(255,255,255,0));
	m_LabelName->setString(m_sName.c_str());

	int playerW = m_SpriteFigure->getTextureRect().size.width / 2;
	int playerH = m_SpriteFigure->getTextureRect().size.height;
	int nameW   = m_LabelName->getContentSize().width / 2;
	int nameH   = m_LabelName->getContentSize().height;
	m_LabelName->setPosition(figPos.x + playerW - nameW,figPos.y - nameH + 5);

	m_LabelUpString = Label::create("123","MoShou.ttf",24);
	m_LabelUpString->setAnchorPoint(Point(0,0));
	m_LabelUpString->setTextColor(ccc4(255,193,100,0));
	m_LabelUpString->setPosition(upPos);
	m_LabelUpString->setString("");
	m_LabelUpString->setVisible(false);

	//添加还剩多少牌的字体
	int count = 0;
	char labelBuf[32];
	_itoa(count,labelBuf,10);
	m_LabelCardCount = Label::create(labelBuf,"MoShou.ttf",15);
	m_LabelCardCount->setAnchorPoint(Point(0,0));
	m_LabelCardCount->setPosition(Vect::ZERO);
	m_LabelCardCount->setVisible(false);
	m_LabelCardCount->setTextColor(ccc4(255,242,0,0));

	parent->addChild(m_LabelUpString);
	parent->addChild(m_SpriteFigure);
	parent->addChild(m_LabelName);
	parent->addChild(m_Label);
	parent->addChild(m_LabelCardCount);

	//不是下方的
	if( m_Dir != DOWN )
		return;

	//添加倍数图片
	m_SpriteMultiple = Sprite::create("PIC//multiple.png");
	m_SpriteMultiple->setAnchorPoint(Point(0,0));
	m_SpriteMultiple->setPosition(Point(winSize.width/2 + 80,0));
	m_SpriteMultiple->setTextureRect(Rect(0,0,22,22));

	//字体
	int x = m_SpriteMultiple->getPosition().x + m_SpriteMultiple->getTextureRect().size.width + 10;
	_itoa(m_iMultiple,labelBuf,10);
	m_LabelMultiple = Label::create(labelBuf,"Marker Felt",24);
	m_LabelMultiple->setAnchorPoint(Point(0,0));
	m_LabelMultiple->setPosition(Point(x,0));
	m_LabelMultiple->setTextColor(ccc4(255,242,0,0));

	//添加欢乐豆
	m_SpriteJoyBean = Sprite::create("PIC//joyBean.png");
	int joyBeanW = m_SpriteJoyBean->getTextureRect().size.width;
	m_SpriteJoyBean->setAnchorPoint(Point(0,0));
	m_SpriteJoyBean->setPosition(Point(winSize.width/2 - 80 - joyBeanW,0));

	//字体
	x = m_SpriteJoyBean->getPosition().x + m_SpriteJoyBean->getTextureRect().size.width + 10;
	_itoa(m_iJoyBean,labelBuf,10);
	m_LabelJoyBean = Label::create(labelBuf,"MoShou.ttf",24);
	m_LabelJoyBean->setAnchorPoint(Point(0,0));
	m_LabelJoyBean->setPosition(Point(x,0));
	m_LabelJoyBean->setTextColor(ccc4(255,242,0,0));

	parent->addChild(m_SpriteJoyBean);
	parent->addChild(m_LabelJoyBean);
	parent->addChild(m_SpriteMultiple);
	parent->addChild(m_LabelMultiple);
}

CPlayer::~CPlayer()
{
	clear();
	delete m_Timer;
}

//运行
CPlayer::enAiResult CPlayer::Run()
{
	if( m_Timer->m_bFlags )
	{
		m_Timer->m_bFlags = false;

		if( m_enState == en_ChuPai )
			m_vSendVect.push_back(m_vCardVect[0]);

		switch( m_enState )
		{
		case en_Wait:		return en_AiWait;		//等待
		case en_JiaoDiZhu:	return en_NoCryLoadlord;//叫地主
		case en_QiangDiZhu:	return en_NoRobLaodlord;//抢地主
		case en_JiaBei:		return en_NoDouble;		//加倍
		case en_GenPai:		return en_NoDenmand;	//必须出指定的类型牌
		case en_ChuPai:		return en_AppearCard;	//出任意牌
		}
	}

	return en_AiWait;
}

//明牌
void CPlayer::showCard(bool sound)
{
	if( sound )
	{
		//播放音效
		string soundFilePath = MINGPAI_MUSIC;
		manTurnWomanSound(soundFilePath);//男声转女声
		SimpleAudioEngine::getInstance()->playEffect(soundFilePath.c_str());
	}
	m_bShowCard = true;
	m_iMultiple *= 3;

	if( m_Dir == DOWN )
		return;

	for( int i = 0; i < (int)m_vCardVect.size(); ++i )
	{
		enCard card = m_vCardVect[i]->card;
		m_vCardVect[i]->sprite->setTexture("PIC//cards.png");
		m_vCardVect[i]->sprite->setTextureRect(Rect(card * 82,0,82,114));
	}
}
//设置全部字体显示与否
void CPlayer::setAllLabelHide(bool show)
{
	m_Label->setVisible(show);
	//m_LabelCardCount->setVisible(show);
	m_LabelUpString->setVisible(show);
}
//计时器完了之后的回调函数
void CPlayer::TimerCallBackFun()
{

}
//开始计时
void CPlayer::startTimer(int timeMax)
{
	m_Timer->startTimer(timeMax);
	m_Label->setVisible(false);
	setVectorCardHide(m_vShowSendVect,false);
}
//停止计时
void CPlayer::stopTimer()
{
	m_Timer->stopTimer();
}
 //声音等级初始化
void CPlayer::SoundLevelInit()
{
	CPlayer::m_iRobLoadlordLevel = 1;//抢地主的声音等级
	CPlayer::m_iFollowCardLevel  = 1;//跟牌的声音等级
	CPlayer::m_iNoAskLevel       = 1;//要不起声音等级
	CPlayer::m_iCardGreaterLevel = 0;//出牌声音等级
}
//获取出牌的结果
stResult CPlayer::getResult()
{
	return m_Result;
}
//设置自己头上的字，是地主还是农民
void CPlayer::setUpString(string s)
{
	GBKToUTF8(s,"gb2312","utf-8");
	m_LabelUpString->setString(s.c_str());
	m_LabelUpString->setVisible(true);
}
//清空
void CPlayer::clear()
{
	for( vector<stCardSpr*>::iterator it = m_vDeleteCardVect.begin(); it != m_vDeleteCardVect.end(); ++it )
	{
		(*it)->sprite->removeFromParent();
		(*it)->layerColor->removeFromParent();
		delete *it;
	}

	m_vDeleteCardVect.clear();
	m_vMoveVect.clear();
	m_vCardVect.clear();
	m_vSendVect.clear();
	m_vShowSendVect.clear();
	m_Label->setVisible(false);
	m_LabelUpString->setVisible(false);
	m_LabelUpString->setTextColor(ccc4(255,193,100,0));

	CPlayer::m_iMultiple		 = 15;//倍数
	CPlayer::m_iRobLoadlordLevel = 1;//抢地主的声音等级
	CPlayer::m_iFollowCardLevel  = 1;//跟牌的声音等级
	CPlayer::m_iNoAskLevel       = 1;//要不起声音等级
	CPlayer::m_iCardGreaterLevel = 0;//出牌声音等级
	CPlayer::m_LandlordNoCryCard = TYPE_DANPAI; //地主不要的牌

	m_enState = en_Wait;
	m_bShowCard = false;
	m_bMoveCard = false;
	m_bLandlord = false;
	m_bCardUpdate = false;
	m_bNoCryLoadlord = false;
}
//提示
bool CPlayer::hint(stResult result,stPlayerData lastPlayer,stPlayerData nowPlayer,stPlayerData nextPlayer,int dir,bool flag)
{
	vector<enCard> enCardVect;
	for( int i = 0; i < m_vCardVect.size(); ++i )
		enCardVect.push_back(m_vCardVect[i]->card);

	if( (enCardVect = logic::hint(enCardVect,result,CPlayer::m_LandlordNoCryCard,lastPlayer,nowPlayer,nextPlayer,dir,flag)).size() == 0 )//要不起
	{
		if( m_enState != en_ChuPai )
			noDenmand();
		return false;
	} 

	//把提示好的牌弹出来
	for( int i = 0; i < (int)m_vCardVect.size(); ++i )
	{
		for( int j = 0; j < (int)enCardVect.size(); ++j )
		{
			if( m_vCardVect[i]->card == enCardVect[j] )
			{
				enCardVect.erase(enCardVect.begin() + j);
				m_vCardVect[i]->popup = true;
				break;
			}
		}
	}
	//移动提示好的牌
	moveOptCard();	

	return true;
}
//设置为地主
void CPlayer::setLandlord(const vector<stCardSpr>& vect)
{
	m_LabelUpString->setTextColor(ccc4(255,0,0,0));
	m_bLandlord = true;//设置为地主
	for( int i = 0; i < (int)vect.size(); ++i )
	{
		//添加底牌
		stCardSpr* sp = new stCardSpr;
		sp->card = vect[i].card;
		sp->opt = false;
		sp->popup = false;
		sp->layerColor = LayerColor::create(ccc4(0x00,0x00,0xFF,0x88));
		sp->layerColor->changeHeight(114);
		sp->layerColor->changeWidth(82);
		sp->layerColor->setVisible(false);
		sp->sprite = Sprite::create( m_Dir == DOWN ? "PIC//cards.png" : "PIC//card_back.png"  );
		sp->sprite->setAnchorPoint(Point(0,0));
		sp->sprite->setTextureRect( m_Dir == DOWN ? Rect((int)sp->card * 82,0,82,114) : Rect(0,0,82,114) );
		sp->sprite->setScale( m_Dir == DOWN ? (1,1) : (0.8f,0.8f) );
		m_vCardVect.push_back(sp);
		m_vDeleteCardVect.push_back(sp);
		m_Praent->addChild(sp->sprite);
		m_Praent->addChild(sp->layerColor);

		if( m_bShowCard )//明牌
		{
			sp->sprite->setTexture("PIC//cards.png");
			sp->sprite->setTextureRect(Rect((int)sp->card * 82,0,82,114));
		}
	}
	cardPosUpdate(m_Dir);//重新设置位置
}
//不要
void CPlayer::noDenmand()
{
	string soundFilePath = BUYAO_MUSIC;
	char buf[32];
	_itoa(m_iNoAskLevel++,buf,10);
	soundFilePath.replace(soundFilePath.find('1'),strlen(buf),buf);
	m_iNoAskLevel = (m_iNoAskLevel == 5) ? 1 : m_iNoAskLevel;
	manTurnWomanSound(soundFilePath);//男声转女声
	SimpleAudioEngine::getInstance()->playEffect(soundFilePath.c_str());

	//设置lebel字符串
	string str = "不要";
	GBKToUTF8(str,"gb2312","utf-8");
	m_Label->setString(str.c_str());
	setTextPos(m_Label,m_Dir);//设置字体位置
	setVectorCardHide(m_vShowSendVect,false);//设置打出的牌不显示
	setCardInit();//设置牌的信息初始化	
}
//加倍
void CPlayer::Double(bool dou)
{
	if( dou )
		CPlayer::m_iMultiple *= 2;//翻倍

	//音效文件路径
	string soundFilePath = dou ? JIABEI_MUSIC : BUJIABEI_MUSIC;
	manTurnWomanSound(soundFilePath);//男声转女声
	SimpleAudioEngine::getInstance()->playEffect(soundFilePath.c_str());

	//根据dou设置lebel字符串
	string str = dou ? "加倍" : "不加倍";
	GBKToUTF8(str,"gb2312","utf-8");
	m_Label->setString(str.c_str());
	setTextPos(m_Label,m_Dir);//设置字体位置
	setVectorCardHide(m_vShowSendVect,false);//设置打出的牌不显示
	setCardInit();//设置牌的信息初始化	
	cardPosUpdate(m_Dir);//重新设置位置
}
//抢地主
void CPlayer::RobLoadlord(bool rob)
{
	if( rob )//抢地主
		CPlayer::m_iMultiple *= 2;//翻倍

	string soundFilePath;//音效文件的位置

	//抢地主
	if(rob)
	{
		//判断有几个人抢地主了 放不同的音效
		soundFilePath = QIANGDIZHU_MUSIC;
		int i = soundFilePath.find('1');
		char buf[32];
		_itoa(m_iRobLoadlordLevel++,buf,10);
		soundFilePath.replace(i,strlen(buf),buf);
		m_iRobLoadlordLevel = (m_iRobLoadlordLevel == 4) ? 1 : m_iRobLoadlordLevel;
	}
	else
		soundFilePath = BUQIANG_MUSIC;

	manTurnWomanSound(soundFilePath);//男声转女声
	SimpleAudioEngine::getInstance()->playEffect(soundFilePath.c_str());

	//根据rob设置lebel字符串
	string str = rob ? "抢地主" : "不抢";
	GBKToUTF8(str,"gb2312","utf-8");
	m_Label->setString(str.c_str());
	setTextPos(m_Label,m_Dir);//设置字体位置
	setVectorCardHide(m_vShowSendVect,false);//设置打出的牌不显示
	setCardInit();//设置牌的信息初始化	
	cardPosUpdate(m_Dir);//重新设置位置
}
//叫地主
void CPlayer::CryLoadlord(bool cry)
{
	if( cry )
		CPlayer::m_iMultiple *= 2;//翻倍

	//播放音效
	string soundFilePath = cry ? JIAODIZHU_MUSIC : BUJIAO_MUSIC;
	manTurnWomanSound(soundFilePath);//男声转女声
	SimpleAudioEngine::getInstance()->playEffect(soundFilePath.c_str());

	//根据cry设置lebel字符串
	string str = cry ? "叫地主" : "不叫";
	GBKToUTF8(str,"gb2312","utf-8");//中文转换
	m_Label->setString(str.c_str());
	setTextPos(m_Label,m_Dir);//设置字体位置
	setVectorCardHide(m_vShowSendVect,false);//设置打出的牌不显示
	setCardInit();//设置牌的信息初始化	
	cardPosUpdate(m_Dir);//重新设置位置
}
//设置字体位置
void CPlayer::setTextPos(Label* label,enDir dir)
{
	//获取屏幕宽高 和 字体宽高
	Size WinSize = Director::getInstance()->getWinSize();
	Size Text = label->getContentSize();
	Point pos;

	//根据玩家所在哪个方向设置位置
	switch( dir )
	{
	case enDir::DOWN:
		{
			pos = Point(WinSize.width/2 - Text.width/2,250); 
		}break;
	case enDir::LEFT:
		{
			pos = Point(130+82*0.8f + Text.width/2,WinSize.height/2 + Text.height/2 + 100);
		}break;
	case enDir::RIGTH:
		{
			pos = Point(WinSize.width-82*0.8f-130-20-Text.width,WinSize.height/2 + Text.height/2 + 100);
		}break;
	}
	
	label->setPosition(pos);
	label->setVisible(true);
}
//男的声音转女的声音
void CPlayer::manTurnWomanSound(string& sound)
{
	/*
		男的声音文件格式为：Man_xx.mp3
		女的声音文件格式为：Woman_xx.mp3
		男转女前面添加Wo,M换成小写
	*/

	if( m_bSex )//本身就是要男声
		return;

	int idx = sound.find('M');
	sound.replace(idx,1,"m");
	sound.insert(idx,"Wo");
}
//出牌
bool CPlayer::appearCard(stResult result)
{
	if( m_vSendVect.size() == 0 )
		return false;

	m_Result = ifVectCardType(m_vSendVect);
	if( m_Result == stResult(TYPE_NULL,-1,0) )
		return false;

	if( result == stResult(TYPE_NULL,-1,0))//自己第一个出牌
	{
		
	}
	else
	{
		//是否大过别人出的牌
		if( (m_Result.type != result.type || m_Result.conut != result.conut || m_Result.value <= result.value)
			&& m_Result.type != TYPE_WANGZHA && m_Result.type != TYPE_ZHADAN  )
			return false;
	}

	//音效文件路径
	string soundFilePath = "";

	switch( m_Result.type )
	{
	case TYPE_3DAI1://3带1
		{
			soundFilePath = SANDAIYI_MUSIC;
			chupai(soundFilePath);
	
		}break;
	case TYPE_3BUDAI://3不带
		{
			int i = (m_Result.value);//转换为int
			if( compareCard((enCard)(i*4),Spandes_2) )
				i = 2;
			else if( compareCard((enCard)(i*4),Spandes_A) )
				i = 1;
			else
				i = i + 3;
			soundFilePath = SANBUDAI_MUSIC;
			chupai(soundFilePath,i);

		}break;
	case TYPE_3DAI2://3带2
		{
			soundFilePath = SANDAIYIDUI_MUSIC;
			chupai(soundFilePath);

		}break;
	case TYPE_DUIZI://一对
		{
			int i = (m_Result.value);//转换为int
			if( compareCard((enCard)(i*4),Spandes_2) )
				i = 2;
			else if( compareCard((enCard)(i*4),Spandes_A) )
				i = 1;
			else
				i = i + 3;
			soundFilePath = YIDUI_MUSIC;
			chupai(soundFilePath,i);

		}break;
	case TYPE_DANPAI://单牌
		{
			int i = (m_Result.value);//转换为int
			if( (enCard)(m_Result.value) == Big_King )
				i = 15;
			else if( (enCard)(m_Result.value) == Little_King )
				i = 14;
			else if( compareCard((enCard)(i*4),Spandes_2) )
				i = 2;
			else if( compareCard((enCard)(i*4),Spandes_A) )
				i = 1;
			else
				i = i + 3;
			soundFilePath = DANPAI_MUSIC;
			chupai(soundFilePath,i);

		}break;
	case TYPE_LIANDUI://连对
		{
			soundFilePath = LIANDUI_MUSIC;
			chupai(soundFilePath);

		}break;
	case TYPE_WANGZHA://王炸
		{
			m_iMultiple *= 2;//倍数翻倍
			soundFilePath = WANGZHA_MUSIC;
			chupai(soundFilePath);
			//更换音乐
			SimpleAudioEngine::getInstance()->playBackgroundMusic(BGM_ZHADAN_MUSICE,true);

		}break;
	case TYPE_ZHADAN://炸弹
		{
			m_iMultiple *= 2;//倍数翻倍
			soundFilePath = ZHADAN_MUSIC;
			chupai(soundFilePath);
			//更换音乐
			SimpleAudioEngine::getInstance()->playBackgroundMusic(BGM_ZHADAN_MUSICE,true);

		}break;
	case TYPE_SHUNZI://顺子
		{
			soundFilePath = SHUNZI_MUSIC;
			chupai(soundFilePath);

		}break;
	case TYPE_FEIJI://飞机
		{
			soundFilePath = FEIJI_MUSIC;
			chupai(soundFilePath);

		}break;
	case TYPE_SIDAI2://4带2
		{
			soundFilePath = SIDAIER_MUSIC;
			chupai(soundFilePath);

		}break;
	}

	for( int i = 0; i < m_vDeleteCardVect.size(); ++i )
		m_vDeleteCardVect[i]->layerColor->setVisible(false);

	return true;
}
//出牌
void CPlayer::chupai(string sound,int SoundIndex)
{
	m_Label->setVisible(false);

	//音乐文件下标
	if( SoundIndex == 0)
	{
		if( m_iCardGreaterLevel == 0 || sound == ZHADAN_MUSIC || sound == WANGZHA_MUSIC )//自己第一个出牌 或者是炸弹
		{
			manTurnWomanSound(sound);//声音是否转换
			SimpleAudioEngine::getInstance()->playEffect(sound.c_str());
			if( sound == WANGZHA_MUSIC || sound == ZHADAN_MUSIC  )
				m_iCardGreaterLevel = 0;
			else
				m_iCardGreaterLevel++;
		}
		else//压制别人出牌的声音
		{
			char buf[10];
			sound = DANI1_MUSICE;
			_itoa(m_iCardGreaterLevel++,buf,10);
			int index = sound.find('1');
			sound.erase(index);
			sound = sound + buf + ".mp3";
			m_iCardGreaterLevel = (m_iCardGreaterLevel == 4) ? 1 : m_iCardGreaterLevel;
			manTurnWomanSound(sound);//声音是否转换
			SimpleAudioEngine::getInstance()->playEffect(sound.c_str());
		}
	}
	else//固定声音的牌组的声音
	{
		char buf[10];
		_itoa(SoundIndex,buf,10);
		int index = sound.find('1');
		sound.erase(index);
		sound = sound + buf + ".mp3";
		m_iCardGreaterLevel = 0;
		manTurnWomanSound(sound);//声音是否转换
		SimpleAudioEngine::getInstance()->playEffect(sound.c_str());
	}

	setVectorCardHide(m_vShowSendVect,false);//设置打出的牌不显示
	m_vShowSendVect.clear();//清空上一次打的牌

	//添加要显示打出去的牌
	for( int idx = 0; idx < (int)m_vSendVect.size(); ++idx )
		m_vShowSendVect.push_back( m_vSendVect[idx] );

	////从主vect删除掉打出去的牌
	for( vector<stCardSpr*>::iterator it = m_vCardVect.begin(); it != m_vCardVect.end();  )
	{
		bool Flags = false;
		for( int j = 0; j < m_vShowSendVect.size(); ++j )
		{
			if( *it == m_vShowSendVect[j] )
			{
				it = m_vCardVect.erase(it);
				Flags = true;
				break;
			}
		}
		if( Flags )
			continue;
		else
			++it;
	}

	//排序
	my_Sort(m_vCardVect);

	if( m_vCardVect.size() == 2 )//2张牌报警
	{
		string s = BAOJING2_MUSICE;
		manTurnWomanSound(s);//声音是否转换
		SimpleAudioEngine::getInstance()->playEffect(s.c_str());
	}
	else if( m_vCardVect.size() == 1 )//1张牌报警
	{
		string s = BAOJING1_MUSICE;
		manTurnWomanSound(s);//声音是否转换
		SimpleAudioEngine::getInstance()->playEffect(s.c_str());
	}

	m_vSendVect.clear();//清空要打出去的牌
	setShowCardPos(m_vShowSendVect,m_Dir);//设置打出去牌的位置
	cardPosUpdate(m_Dir);//更新牌的位置
	setCardInit();//设置全部属性初始化
}
//设置牌不显示
void CPlayer::setVectorCardHide(vector<stCardSpr*>& vect,bool hide)
{
	for( vector<stCardSpr*>::iterator it = vect.begin(); it != vect.end(); ++it )
		(*it)->sprite->setVisible(hide);
}
//判断vector里的牌是什么类型的牌
//返回type_null代表类型错误
stResult CPlayer::ifVectCardType(vector<stCardSpr*>& vect )
{
	vector<enCard> CardVect;//纸牌表
	for( int i = 0; i < vect.size(); ++i )
		CardVect.push_back( vect[i]->card );

	stResult result;
	
	//判断是否是单牌
	if( (result = judgeSingle(CardVect)).type != TYPE_NULL )
		return result;
	//判断是不是王炸
	if( (result = judgeWangBomb(CardVect)).type != TYPE_NULL )
		return result;
	//判断是否顺子
	if( (result = judgeStraight(CardVect)).type != TYPE_NULL )
		return result;
	//判断是否是对子
	if( (result = judgeTwain(CardVect)).type != TYPE_NULL )
		return result;
	//判断是否3带1
	if( (result = judgeThreeBandOne(CardVect)).type != TYPE_NULL )
		return result;
	//判断是否3带2
	if( (result = judgeThreeBandTwo(CardVect)).type != TYPE_NULL )
		return result;
	//判断是否3不带
	if( (result = judgeThreeBandZero(CardVect)).type != TYPE_NULL )
		return result;
	//判断是否连对
	if( (result = judgeContinuousTwain(CardVect)).type != TYPE_NULL )
		return result;
	//判断是否是炸弹
	if( (result = judgeBomb(CardVect)).type != TYPE_NULL )
		return result;
	//判断是否是飞机
	if( (result = judgeAircraft(CardVect)).type != TYPE_NULL )
		return result;
	//判断是4带2
	if( (result = judgeFourBandTwo(CardVect)).type != TYPE_NULL )
		return result;

	return result;
}
//为2点之间的牌添加透明层
void CPlayer::addInTowPointCardLayer(Point begin,Point end)
{
	if( m_vCardVect.size() == 0 )
		return;

	int endW = m_vCardVect[0]->sprite->getTextureRect().size.width;
	int endH = m_vCardVect[0]->sprite->getTextureRect().size.height;
	int endX = m_vCardVect[0]->sprite->getPositionX();
	int endY = m_vCardVect[0]->sprite->getPositionY();
	int CardbeginX = m_vCardVect[m_vCardVect.size() - 1]->sprite->getPositionX();

	//如果范围超过了牌的范围就不处理
	if( begin.x < CardbeginX && begin.x < end.x )
		return ;
	//如果范围超过了牌的范围就不处理
	if( begin.x > endX + endW && begin.x > end.x )
		return ;

	for( int i = 0; i < (int)m_vCardVect.size(); ++i )
		m_vCardVect[i]->layerColor->setVisible(m_vCardVect[i]->popup = false);

	for( int i = 0; i < (int)m_vCardVect.size(); ++i )
	{
		if( end == begin )//单击
		{
			ME_Rect rect,that;
			//从左到右?
			int w = ( begin.x > end.x ) ? 20 : m_vCardVect[i]->sprite->getTextureRect().size.width;
			rect.bottom = m_vCardVect[i]->sprite->getPositionY();
			rect.left = m_vCardVect[i]->sprite->getPositionX();
			rect.top = m_vCardVect[i]->sprite->getPositionY() + m_vCardVect[i]->sprite->getTextureRect().size.height;
			rect.right = m_vCardVect[i]->sprite->getPositionX() + w;

			//第一次点下的点 和 现在移动的点 组成一个矩形
			that.bottom = begin.y;
			that.top = begin.y;
			that.left = MIN(begin.x,end.x);
			that.right = MAX(end.x,begin.x);
			//判断点是否在牌的范围内
			if( MyIntersectRect(&rect,&that) ) 
			{
				m_vCardVect[i]->popup = true;
				return;
			}
		}

		int PressBeginX = MIN(begin.x,end.x);
		int PressEndX   = MAX(end.x,begin.x);
		int PressY		= end.y;
		int cardX = m_vCardVect[i]->sprite->getPositionX();
	    int cardY = m_vCardVect[i]->sprite->getPositionY();
		int cardH = m_vCardVect[i]->sprite->getTextureRect().size.height + 30;
		int cardW = (i == 0) ? 82 : 20;

		if( m_vCardVect[i]->opt )
			cardY -= 30;

		if( begin.x > end.x )//从左到右
			cardX += cardW;
		else if( i == m_vCardVect.size() - 1)//左边第一个
		{
			if( PressBeginX >= cardX && PressBeginX <= cardX + cardW && PressY >= cardY && PressY <= cardY + cardH ) 
			{
				m_vCardVect[i]->layerColor->setPosition(m_vCardVect[i]->sprite->getPosition());//设置位置
				m_vCardVect[i]->layerColor->setVisible(m_vCardVect[i]->popup = true);//设置为选中，添加半透明层
				continue;
			}
		}
		//在2点之间的牌设置为选中
		if( cardX >= PressBeginX && cardX <= PressEndX && PressY >= cardY && PressY <= cardY + cardH ) 
		{
			m_vCardVect[i]->layerColor->setPosition(m_vCardVect[i]->sprite->getPosition());//设置位置
			m_vCardVect[i]->layerColor->setVisible(m_vCardVect[i]->popup = true);//设置为选中，添加半透明层
		}
	}
}
//设置牌全部属性初始化
void CPlayer::setCardInit()
{
	m_vSendVect.clear();
	m_vMoveVect.clear();

	if( m_Dir != DOWN )
		return;

	for( int i = (int)m_vCardVect.size() - 1; i >= 0 ; --i )
	{
		int x = m_vCardVect[i]->sprite->getPositionX();
		int y = DOWN_Y;

		CCActionInterval *forward = CCMoveTo::create(0.15,ccp(x, y));  
		m_vCardVect[i]->sprite->runAction(forward);
		m_vCardVect[i]->opt = false;
		m_vCardVect[i]->popup = false;
		m_vCardVect[i]->layerColor->setVisible(false);
	}
}
//牌的半透明层初始化
void CPlayer::setCardLayerInit()
{
	for( int i = (int)m_vCardVect.size() - 1; i >= 0 ; --i )
	{
		m_vCardVect[i]->popup = false;
		m_vCardVect[i]->layerColor->setVisible(false);
	}
}
//添加要打出的牌到m_SendVect
void CPlayer::addSendCardVect(vector<stCardSpr*>& vect )
{
	m_vSendVect.clear(); 
	for( int i = (int)m_vCardVect.size() - 1; i >= 0 ; --i )
	{
		if( m_vCardVect[i]->opt )
			vect.push_back(m_vCardVect[i]);
	}
}
//移动被选中的牌
void CPlayer::moveOptCard()
{
	vector<enCard> cardVect;

	vector<stSaveCardData> returnVect;
	for( int i = (int)m_vCardVect.size() - 1; i >= 0 ; --i )
	{
		if( m_vCardVect[i]->popup  )//是否被选中
		{
			m_vCardVect[i]->popup = false;
			m_vMoveVect.push_back(m_vCardVect[i]);
			cardVect.push_back(m_vCardVect[i]->card);
		}
	}
	
	vector<enCard> tempVect = cardVect;

	if( cardVect.size() >= 5 )
	{
		FindContinuousTwain(cardVect,returnVect);	//判断是否是连对
		FindStraight(tempVect,returnVect);//判断是否是顺子
	}

	//是顺子或者连对
	if( returnVect.size() != 0 )
	{
		for( int i = 0; i < (int)m_vMoveVect.size(); )
		{
			bool Flags = true;
			for( int j = 0; j < returnVect[0].result.conut; ++j )
			{
				if( m_vMoveVect[i]->card == returnVect[0].vect[j] )
				{
					Flags = false;
					break;
				}
			}

			if( Flags )
				m_vMoveVect.erase(m_vMoveVect.begin() + i);
			else
				i += 1;
		}
	}

	for( int i = 0; i < (int)m_vMoveVect.size(); ++i )
	{
		int x = m_vMoveVect[i]->sprite->getPositionX();
		int y = m_vMoveVect[i]->sprite->getPositionY();

		if( m_vMoveVect[i]->opt )//是否已经弹出
			y = DOWN_Y;
		else
			y += 30;

		//移动
		CCActionInterval *forward = CCMoveTo::create(0.15f,ccp(x, y));  
		m_vMoveVect[i]->sprite->runAction(forward);
		m_vMoveVect[i]->opt   = m_vMoveVect[i]->opt ? false : true;
	}
	addSendCardVect(m_vSendVect);//添加要打出的牌到M_SendVect
	setCardLayerInit();//设置牌位置初始化
	m_vMoveVect.clear();
}

//返回一个点上牌的起点X与X+W
bool CPlayer::getCardPoint(Point p,Point& left,Point& rigth)
{
	for( int i = 0; i < (int)m_vCardVect.size() ; ++i )
	{
		ME_Rect rect;
		rect.bottom = m_vCardVect[i]->sprite->getPositionY();
		rect.left = m_vCardVect[i]->sprite->getPositionX();
		rect.top = m_vCardVect[i]->sprite->getPositionY() + m_vCardVect[i]->sprite->getTextureRect().size.height;
		rect.right = m_vCardVect[i]->sprite->getPositionX() + m_vCardVect[i]->sprite->getTextureRect().size.width;

		if( MyPtInRect(&rect,p.x,p.y)) //判断点是否在牌的范围
		{
			int w = (i == 0) ? 82 : 20;
			left = m_vCardVect[i]->sprite->getPosition();
			rigth = left + Point(w,0);
			return true;
		}
	}

	return false;
}
//选择排序
void CPlayer::my_Sort(vector<stCardSpr*>& vect)
{
	for( int i = 0; i < (int)vect.size(); ++i )
	{
		int j = i;
		for( int k = j + 1; k < (int)vect.size(); ++k )
		{
			if( vect[j]->card > vect[k]->card )
				j = k;
		}
		if( j != i )
		{
			stCardSpr t = *vect[j];
			*vect[j] = *vect[i];
			*vect[i] = t;
		}
	}
	//设置精灵和颜色层在最上面
	for( int i = (int)vect.size() - 1; i >= 0; --i )
	{
		vect[i]->sprite->setZOrder( (int)vect.size() - i );
		vect[i]->layerColor->setZOrder((int)vect.size() - i );
	}
}
//设置打出去的牌的位置
void CPlayer::setShowCardPos(vector<stCardSpr*>& vect,enDir dir)
{
	Size size = Director::getInstance()->getVisibleSize();
	float ScaleW = 0.7f;
	float ScaleH = 0.7f;
	int count = 0;
	int picW = 82 * ScaleW;
	int picH = 114 * ScaleH;
	int interval = 20;//牌与牌之间的间隔
	Vec2 p = Vec2::ZERO;

	switch( dir )
	{
	case DOWN:
		{
			int length = vect.size() * interval + picW - interval;	//总共牌的长度
			p = Vec2(size.width / 2 - length / 2 ,SHOWCARD_Y);
		}break;
	case RIGTH:
		{
			int length = vect.size() * 20;//总共牌的长度
			p = Vec2(size.width - picW - 220 - length ,size.height / 2 + 60);
		}break;
	case LEFT:
		{
			int length = vect.size() * 20;//总共牌的长度
			p = Vec2(vect[0]->sprite->getPositionX() + picW + 60,size.height / 2 + 60);
		}break;
	}

	for( int i = 0; i < (int)vect.size(); ++i )
	{
		vect[i]->layerColor->setScale(ScaleW,ScaleH);
		vect[i]->sprite->setScale(ScaleW,ScaleH);
		vect[i]->sprite->setPosition(ccp(p.x + i * interval,p.y));
		vect[i]->layerColor->setPosition(ccp(p.x + i * interval,p.y));
		vect[i]->sprite->setZOrder(i);
		vect[i]->layerColor->setZOrder(i);
		vect[i]->sprite->setTexture("PIC//cards.png");
		vect[i]->sprite->setTextureRect(Rect((int)vect[i]->card * 82,0,82,114));
	}
}
//排版
void CPlayer::arrange(vector<stCardSpr*>& vect,enDir dir)
{
	Size size = Director::getInstance()->getVisibleSize();//屏幕大小
	int count = 0;
	switch( dir )
	{
	case DOWN:
		{
			//总共牌的长度
			int length = vect.size() * 25 + 82 - 23;
			Vec2 p = Vec2(size.width / 2 - length / 2 ,DOWN_Y);
			
			//排序
			my_Sort(vect);
			
			for( int i = (int)vect.size()-1; i >= 0; --i )
			{
				vect[i]->sprite->setPosition(23 * count + p.x,p.y);
				vect[i]->layerColor->setPosition(23 * count + p.x,p.y);
				count++;
			}

		}break;
	case RIGTH:
		{
			int length = vect.size() * 20;
			Vec2 p = Vec2(size.width - 82*0.8 - 130,size.height / 2 - length / 2 + 30);

			//排序
			my_Sort(vect);
			
			for( vector<stCardSpr*>::iterator it = vect.begin(); it != vect.end(); ++it )
			{
				(*it)->sprite->setPosition(p.x,p.y + count * 20);
				(*it)->layerColor->setPosition(20 * count + p.x,p.y);
				count++;
			}

			if( vect.size() == 0 )
				return;
		}break;
	case LEFT:
		{
			int length = vect.size() * 20;
			Vec2 p = Vec2(130,size.height / 2 - length / 2 + 30);

			//排序
			my_Sort(vect);
			
			for( vector<stCardSpr*>::iterator it = vect.begin(); it != vect.end(); ++it )
			{
				(*it)->sprite->setPosition(p.x,p.y + count * 20);
				(*it)->layerColor->setPosition(20 * count + p.x,p.y);
				count++;
			}

			if( vect.size() == 0 )
				return;

		}break;
	}

	char buf[32];
	_itoa(m_iMultiple,buf,10);
	m_LabelMultiple->setString(buf);
	
	if( dir == DOWN )
	{
		itoa(m_iJoyBean,buf,10);
		m_LabelJoyBean->setString(buf);
		return ;
	}

	if( vect.size() == 0 )
		m_LabelCardCount->setVisible(false); 

	//显示还剩多少张牌
	string str = "剩";
	_itoa(vect.size(),buf,10);
	str = str + buf + "张";
	GBKToUTF8(str,"gb2312","utf-8");//中文转换
	m_LabelCardCount->setString(str.c_str());

	Point pos = vect[0]->sprite->getPosition();
	Size  cardSize = vect[0]->sprite->getTextureRect().size;
	Size  strSize  = m_LabelCardCount->getContentSize();
	m_LabelCardCount->setPosition(ccp(pos.x + cardSize.width/2 - strSize.width/2,pos.y - strSize.height - 5));
	m_LabelCardCount->setVisible(true);	
}
//牌的位置更新
void CPlayer::cardPosUpdate(enDir dir)
{
	arrange(m_vCardVect,dir);
}
//Ai
CPlayer::enAiResult CPlayer::Ai(stPlayerData lastPlayer,stPlayerData nowPlayer,stPlayerData nextPlayer,int dir,stResult result)//上一个玩家出的牌，默认没出		
{
	vector<enCard> myCardVect;//自己手里剩下的牌
	vector<enCard> returnCardVect;//Ai返回出来的牌

	for ( int i = 0; i < (int)m_vCardVect.size(); ++i )
		myCardVect.push_back(m_vCardVect[i]->card);

	//调用父类AI
	enAiResult aiResult = logic::Ai(m_enState,myCardVect,returnCardVect,CPlayer::m_LandlordNoCryCard,
									lastPlayer,nowPlayer,nextPlayer,dir,result);
	switch ( aiResult )
	{
	case en_AiWait://等待
		{
		}break;
	case en_CryLoadlord://叫地主
		{
			CryLoadlord(true);
		}break;
	case en_NoCryLoadlord://不叫地主
		{
			CryLoadlord(false);
		}break;
	case en_RobLaodlord://抢地主
		{
			RobLoadlord(true);
		}break;
	case en_NoRobLaodlord://不抢地主
		{
			RobLoadlord(false);
		}break;
	case en_Double://加倍
		{
			Double(true);
		}break;
	case en_NoDouble://不加倍
		{
			Double(false);
		}break;
	case en_NoDenmand://要不起
		{
			noDenmand();
		}break;
	case en_AppearCard:	 //出牌
		{
			m_vSendVect.clear();
			for( int i = 0; i < m_vCardVect.size(); ++i)
			{
				for( int j = 0; j < (int)returnCardVect.size(); ++j )
				{
					if( m_vCardVect[i]->card == returnCardVect[j] )
					{
						returnCardVect.erase(returnCardVect.begin() + j);
						m_vSendVect.push_back(m_vCardVect[i]);
						break;
					}
				}
			}

			bool go = appearCard(result);//出牌
			if( !go )//要不起
				noDenmand();

		}break;
	}

	m_enState = en_Wait;

	return aiResult;
}