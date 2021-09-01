#include "player.h"

int		CPlayer::m_iMultiple          = 15;//����
int		CPlayer::m_iRobLoadlordLevel  = 1;//�������������ȼ�
int		CPlayer::m_iFollowCardLevel   = 1;//���Ƶ������ȼ�
int		CPlayer::m_iNoAskLevel        = 1;//Ҫ���������ȼ�
int		CPlayer::m_iCardGreaterLevel  = 0;//���������ȼ�
Sprite* CPlayer::m_SpriteMultiple	  = NULL;//����ͼƬ
Label*  CPlayer::m_LabelMultiple	  = NULL;//��������
enType  CPlayer::m_LandlordNoCryCard  = TYPE_DANPAI; //������Ҫ����


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

	//ͷ�������λ��
	upPos.y  = figPos.y + 210;
	upPos.x  = figPos.x + 40;

	//�������ͼ��
	m_SpriteFigure = Sprite::create( sex ? "PIC//figure//man.png" : "PIC//figure//woman.png");
	m_SpriteFigure->setAnchorPoint(Point(0,0));
	m_SpriteFigure->setPosition(figPos);

	string str = "";
	m_Label = Label::create(str.c_str(),"MoShou.ttf",24);
	m_Label->setVisible(false);
	m_Label->setAnchorPoint(Point(0,0));
	m_Label->setTextColor(ccc4(255,242,0,0));

	//������������
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

	//��ӻ�ʣ�����Ƶ�����
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

	//�����·���
	if( m_Dir != DOWN )
		return;

	//��ӱ���ͼƬ
	m_SpriteMultiple = Sprite::create("PIC//multiple.png");
	m_SpriteMultiple->setAnchorPoint(Point(0,0));
	m_SpriteMultiple->setPosition(Point(winSize.width/2 + 80,0));
	m_SpriteMultiple->setTextureRect(Rect(0,0,22,22));

	//����
	int x = m_SpriteMultiple->getPosition().x + m_SpriteMultiple->getTextureRect().size.width + 10;
	_itoa(m_iMultiple,labelBuf,10);
	m_LabelMultiple = Label::create(labelBuf,"Marker Felt",24);
	m_LabelMultiple->setAnchorPoint(Point(0,0));
	m_LabelMultiple->setPosition(Point(x,0));
	m_LabelMultiple->setTextColor(ccc4(255,242,0,0));

	//��ӻ��ֶ�
	m_SpriteJoyBean = Sprite::create("PIC//joyBean.png");
	int joyBeanW = m_SpriteJoyBean->getTextureRect().size.width;
	m_SpriteJoyBean->setAnchorPoint(Point(0,0));
	m_SpriteJoyBean->setPosition(Point(winSize.width/2 - 80 - joyBeanW,0));

	//����
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

//����
CPlayer::enAiResult CPlayer::Run()
{
	if( m_Timer->m_bFlags )
	{
		m_Timer->m_bFlags = false;

		if( m_enState == en_ChuPai )
			m_vSendVect.push_back(m_vCardVect[0]);

		switch( m_enState )
		{
		case en_Wait:		return en_AiWait;		//�ȴ�
		case en_JiaoDiZhu:	return en_NoCryLoadlord;//�е���
		case en_QiangDiZhu:	return en_NoRobLaodlord;//������
		case en_JiaBei:		return en_NoDouble;		//�ӱ�
		case en_GenPai:		return en_NoDenmand;	//�����ָ����������
		case en_ChuPai:		return en_AppearCard;	//��������
		}
	}

	return en_AiWait;
}

//����
void CPlayer::showCard(bool sound)
{
	if( sound )
	{
		//������Ч
		string soundFilePath = MINGPAI_MUSIC;
		manTurnWomanSound(soundFilePath);//����תŮ��
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
//����ȫ��������ʾ���
void CPlayer::setAllLabelHide(bool show)
{
	m_Label->setVisible(show);
	//m_LabelCardCount->setVisible(show);
	m_LabelUpString->setVisible(show);
}
//��ʱ������֮��Ļص�����
void CPlayer::TimerCallBackFun()
{

}
//��ʼ��ʱ
void CPlayer::startTimer(int timeMax)
{
	m_Timer->startTimer(timeMax);
	m_Label->setVisible(false);
	setVectorCardHide(m_vShowSendVect,false);
}
//ֹͣ��ʱ
void CPlayer::stopTimer()
{
	m_Timer->stopTimer();
}
 //�����ȼ���ʼ��
void CPlayer::SoundLevelInit()
{
	CPlayer::m_iRobLoadlordLevel = 1;//�������������ȼ�
	CPlayer::m_iFollowCardLevel  = 1;//���Ƶ������ȼ�
	CPlayer::m_iNoAskLevel       = 1;//Ҫ���������ȼ�
	CPlayer::m_iCardGreaterLevel = 0;//���������ȼ�
}
//��ȡ���ƵĽ��
stResult CPlayer::getResult()
{
	return m_Result;
}
//�����Լ�ͷ�ϵ��֣��ǵ�������ũ��
void CPlayer::setUpString(string s)
{
	GBKToUTF8(s,"gb2312","utf-8");
	m_LabelUpString->setString(s.c_str());
	m_LabelUpString->setVisible(true);
}
//���
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

	CPlayer::m_iMultiple		 = 15;//����
	CPlayer::m_iRobLoadlordLevel = 1;//�������������ȼ�
	CPlayer::m_iFollowCardLevel  = 1;//���Ƶ������ȼ�
	CPlayer::m_iNoAskLevel       = 1;//Ҫ���������ȼ�
	CPlayer::m_iCardGreaterLevel = 0;//���������ȼ�
	CPlayer::m_LandlordNoCryCard = TYPE_DANPAI; //������Ҫ����

	m_enState = en_Wait;
	m_bShowCard = false;
	m_bMoveCard = false;
	m_bLandlord = false;
	m_bCardUpdate = false;
	m_bNoCryLoadlord = false;
}
//��ʾ
bool CPlayer::hint(stResult result,stPlayerData lastPlayer,stPlayerData nowPlayer,stPlayerData nextPlayer,int dir,bool flag)
{
	vector<enCard> enCardVect;
	for( int i = 0; i < m_vCardVect.size(); ++i )
		enCardVect.push_back(m_vCardVect[i]->card);

	if( (enCardVect = logic::hint(enCardVect,result,CPlayer::m_LandlordNoCryCard,lastPlayer,nowPlayer,nextPlayer,dir,flag)).size() == 0 )//Ҫ����
	{
		if( m_enState != en_ChuPai )
			noDenmand();
		return false;
	} 

	//����ʾ�õ��Ƶ�����
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
	//�ƶ���ʾ�õ���
	moveOptCard();	

	return true;
}
//����Ϊ����
void CPlayer::setLandlord(const vector<stCardSpr>& vect)
{
	m_LabelUpString->setTextColor(ccc4(255,0,0,0));
	m_bLandlord = true;//����Ϊ����
	for( int i = 0; i < (int)vect.size(); ++i )
	{
		//��ӵ���
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

		if( m_bShowCard )//����
		{
			sp->sprite->setTexture("PIC//cards.png");
			sp->sprite->setTextureRect(Rect((int)sp->card * 82,0,82,114));
		}
	}
	cardPosUpdate(m_Dir);//��������λ��
}
//��Ҫ
void CPlayer::noDenmand()
{
	string soundFilePath = BUYAO_MUSIC;
	char buf[32];
	_itoa(m_iNoAskLevel++,buf,10);
	soundFilePath.replace(soundFilePath.find('1'),strlen(buf),buf);
	m_iNoAskLevel = (m_iNoAskLevel == 5) ? 1 : m_iNoAskLevel;
	manTurnWomanSound(soundFilePath);//����תŮ��
	SimpleAudioEngine::getInstance()->playEffect(soundFilePath.c_str());

	//����lebel�ַ���
	string str = "��Ҫ";
	GBKToUTF8(str,"gb2312","utf-8");
	m_Label->setString(str.c_str());
	setTextPos(m_Label,m_Dir);//��������λ��
	setVectorCardHide(m_vShowSendVect,false);//���ô�����Ʋ���ʾ
	setCardInit();//�����Ƶ���Ϣ��ʼ��	
}
//�ӱ�
void CPlayer::Double(bool dou)
{
	if( dou )
		CPlayer::m_iMultiple *= 2;//����

	//��Ч�ļ�·��
	string soundFilePath = dou ? JIABEI_MUSIC : BUJIABEI_MUSIC;
	manTurnWomanSound(soundFilePath);//����תŮ��
	SimpleAudioEngine::getInstance()->playEffect(soundFilePath.c_str());

	//����dou����lebel�ַ���
	string str = dou ? "�ӱ�" : "���ӱ�";
	GBKToUTF8(str,"gb2312","utf-8");
	m_Label->setString(str.c_str());
	setTextPos(m_Label,m_Dir);//��������λ��
	setVectorCardHide(m_vShowSendVect,false);//���ô�����Ʋ���ʾ
	setCardInit();//�����Ƶ���Ϣ��ʼ��	
	cardPosUpdate(m_Dir);//��������λ��
}
//������
void CPlayer::RobLoadlord(bool rob)
{
	if( rob )//������
		CPlayer::m_iMultiple *= 2;//����

	string soundFilePath;//��Ч�ļ���λ��

	//������
	if(rob)
	{
		//�ж��м������������� �Ų�ͬ����Ч
		soundFilePath = QIANGDIZHU_MUSIC;
		int i = soundFilePath.find('1');
		char buf[32];
		_itoa(m_iRobLoadlordLevel++,buf,10);
		soundFilePath.replace(i,strlen(buf),buf);
		m_iRobLoadlordLevel = (m_iRobLoadlordLevel == 4) ? 1 : m_iRobLoadlordLevel;
	}
	else
		soundFilePath = BUQIANG_MUSIC;

	manTurnWomanSound(soundFilePath);//����תŮ��
	SimpleAudioEngine::getInstance()->playEffect(soundFilePath.c_str());

	//����rob����lebel�ַ���
	string str = rob ? "������" : "����";
	GBKToUTF8(str,"gb2312","utf-8");
	m_Label->setString(str.c_str());
	setTextPos(m_Label,m_Dir);//��������λ��
	setVectorCardHide(m_vShowSendVect,false);//���ô�����Ʋ���ʾ
	setCardInit();//�����Ƶ���Ϣ��ʼ��	
	cardPosUpdate(m_Dir);//��������λ��
}
//�е���
void CPlayer::CryLoadlord(bool cry)
{
	if( cry )
		CPlayer::m_iMultiple *= 2;//����

	//������Ч
	string soundFilePath = cry ? JIAODIZHU_MUSIC : BUJIAO_MUSIC;
	manTurnWomanSound(soundFilePath);//����תŮ��
	SimpleAudioEngine::getInstance()->playEffect(soundFilePath.c_str());

	//����cry����lebel�ַ���
	string str = cry ? "�е���" : "����";
	GBKToUTF8(str,"gb2312","utf-8");//����ת��
	m_Label->setString(str.c_str());
	setTextPos(m_Label,m_Dir);//��������λ��
	setVectorCardHide(m_vShowSendVect,false);//���ô�����Ʋ���ʾ
	setCardInit();//�����Ƶ���Ϣ��ʼ��	
	cardPosUpdate(m_Dir);//��������λ��
}
//��������λ��
void CPlayer::setTextPos(Label* label,enDir dir)
{
	//��ȡ��Ļ��� �� ������
	Size WinSize = Director::getInstance()->getWinSize();
	Size Text = label->getContentSize();
	Point pos;

	//������������ĸ���������λ��
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
//�е�����תŮ������
void CPlayer::manTurnWomanSound(string& sound)
{
	/*
		�е������ļ���ʽΪ��Man_xx.mp3
		Ů�������ļ���ʽΪ��Woman_xx.mp3
		��תŮǰ�����Wo,M����Сд
	*/

	if( m_bSex )//�������Ҫ����
		return;

	int idx = sound.find('M');
	sound.replace(idx,1,"m");
	sound.insert(idx,"Wo");
}
//����
bool CPlayer::appearCard(stResult result)
{
	if( m_vSendVect.size() == 0 )
		return false;

	m_Result = ifVectCardType(m_vSendVect);
	if( m_Result == stResult(TYPE_NULL,-1,0) )
		return false;

	if( result == stResult(TYPE_NULL,-1,0))//�Լ���һ������
	{
		
	}
	else
	{
		//�Ƿ������˳�����
		if( (m_Result.type != result.type || m_Result.conut != result.conut || m_Result.value <= result.value)
			&& m_Result.type != TYPE_WANGZHA && m_Result.type != TYPE_ZHADAN  )
			return false;
	}

	//��Ч�ļ�·��
	string soundFilePath = "";

	switch( m_Result.type )
	{
	case TYPE_3DAI1://3��1
		{
			soundFilePath = SANDAIYI_MUSIC;
			chupai(soundFilePath);
	
		}break;
	case TYPE_3BUDAI://3����
		{
			int i = (m_Result.value);//ת��Ϊint
			if( compareCard((enCard)(i*4),Spandes_2) )
				i = 2;
			else if( compareCard((enCard)(i*4),Spandes_A) )
				i = 1;
			else
				i = i + 3;
			soundFilePath = SANBUDAI_MUSIC;
			chupai(soundFilePath,i);

		}break;
	case TYPE_3DAI2://3��2
		{
			soundFilePath = SANDAIYIDUI_MUSIC;
			chupai(soundFilePath);

		}break;
	case TYPE_DUIZI://һ��
		{
			int i = (m_Result.value);//ת��Ϊint
			if( compareCard((enCard)(i*4),Spandes_2) )
				i = 2;
			else if( compareCard((enCard)(i*4),Spandes_A) )
				i = 1;
			else
				i = i + 3;
			soundFilePath = YIDUI_MUSIC;
			chupai(soundFilePath,i);

		}break;
	case TYPE_DANPAI://����
		{
			int i = (m_Result.value);//ת��Ϊint
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
	case TYPE_LIANDUI://����
		{
			soundFilePath = LIANDUI_MUSIC;
			chupai(soundFilePath);

		}break;
	case TYPE_WANGZHA://��ը
		{
			m_iMultiple *= 2;//��������
			soundFilePath = WANGZHA_MUSIC;
			chupai(soundFilePath);
			//��������
			SimpleAudioEngine::getInstance()->playBackgroundMusic(BGM_ZHADAN_MUSICE,true);

		}break;
	case TYPE_ZHADAN://ը��
		{
			m_iMultiple *= 2;//��������
			soundFilePath = ZHADAN_MUSIC;
			chupai(soundFilePath);
			//��������
			SimpleAudioEngine::getInstance()->playBackgroundMusic(BGM_ZHADAN_MUSICE,true);

		}break;
	case TYPE_SHUNZI://˳��
		{
			soundFilePath = SHUNZI_MUSIC;
			chupai(soundFilePath);

		}break;
	case TYPE_FEIJI://�ɻ�
		{
			soundFilePath = FEIJI_MUSIC;
			chupai(soundFilePath);

		}break;
	case TYPE_SIDAI2://4��2
		{
			soundFilePath = SIDAIER_MUSIC;
			chupai(soundFilePath);

		}break;
	}

	for( int i = 0; i < m_vDeleteCardVect.size(); ++i )
		m_vDeleteCardVect[i]->layerColor->setVisible(false);

	return true;
}
//����
void CPlayer::chupai(string sound,int SoundIndex)
{
	m_Label->setVisible(false);

	//�����ļ��±�
	if( SoundIndex == 0)
	{
		if( m_iCardGreaterLevel == 0 || sound == ZHADAN_MUSIC || sound == WANGZHA_MUSIC )//�Լ���һ������ ������ը��
		{
			manTurnWomanSound(sound);//�����Ƿ�ת��
			SimpleAudioEngine::getInstance()->playEffect(sound.c_str());
			if( sound == WANGZHA_MUSIC || sound == ZHADAN_MUSIC  )
				m_iCardGreaterLevel = 0;
			else
				m_iCardGreaterLevel++;
		}
		else//ѹ�Ʊ��˳��Ƶ�����
		{
			char buf[10];
			sound = DANI1_MUSICE;
			_itoa(m_iCardGreaterLevel++,buf,10);
			int index = sound.find('1');
			sound.erase(index);
			sound = sound + buf + ".mp3";
			m_iCardGreaterLevel = (m_iCardGreaterLevel == 4) ? 1 : m_iCardGreaterLevel;
			manTurnWomanSound(sound);//�����Ƿ�ת��
			SimpleAudioEngine::getInstance()->playEffect(sound.c_str());
		}
	}
	else//�̶����������������
	{
		char buf[10];
		_itoa(SoundIndex,buf,10);
		int index = sound.find('1');
		sound.erase(index);
		sound = sound + buf + ".mp3";
		m_iCardGreaterLevel = 0;
		manTurnWomanSound(sound);//�����Ƿ�ת��
		SimpleAudioEngine::getInstance()->playEffect(sound.c_str());
	}

	setVectorCardHide(m_vShowSendVect,false);//���ô�����Ʋ���ʾ
	m_vShowSendVect.clear();//�����һ�δ����

	//���Ҫ��ʾ���ȥ����
	for( int idx = 0; idx < (int)m_vSendVect.size(); ++idx )
		m_vShowSendVect.push_back( m_vSendVect[idx] );

	////����vectɾ�������ȥ����
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

	//����
	my_Sort(m_vCardVect);

	if( m_vCardVect.size() == 2 )//2���Ʊ���
	{
		string s = BAOJING2_MUSICE;
		manTurnWomanSound(s);//�����Ƿ�ת��
		SimpleAudioEngine::getInstance()->playEffect(s.c_str());
	}
	else if( m_vCardVect.size() == 1 )//1���Ʊ���
	{
		string s = BAOJING1_MUSICE;
		manTurnWomanSound(s);//�����Ƿ�ת��
		SimpleAudioEngine::getInstance()->playEffect(s.c_str());
	}

	m_vSendVect.clear();//���Ҫ���ȥ����
	setShowCardPos(m_vShowSendVect,m_Dir);//���ô��ȥ�Ƶ�λ��
	cardPosUpdate(m_Dir);//�����Ƶ�λ��
	setCardInit();//����ȫ�����Գ�ʼ��
}
//�����Ʋ���ʾ
void CPlayer::setVectorCardHide(vector<stCardSpr*>& vect,bool hide)
{
	for( vector<stCardSpr*>::iterator it = vect.begin(); it != vect.end(); ++it )
		(*it)->sprite->setVisible(hide);
}
//�ж�vector�������ʲô���͵���
//����type_null�������ʹ���
stResult CPlayer::ifVectCardType(vector<stCardSpr*>& vect )
{
	vector<enCard> CardVect;//ֽ�Ʊ�
	for( int i = 0; i < vect.size(); ++i )
		CardVect.push_back( vect[i]->card );

	stResult result;
	
	//�ж��Ƿ��ǵ���
	if( (result = judgeSingle(CardVect)).type != TYPE_NULL )
		return result;
	//�ж��ǲ�����ը
	if( (result = judgeWangBomb(CardVect)).type != TYPE_NULL )
		return result;
	//�ж��Ƿ�˳��
	if( (result = judgeStraight(CardVect)).type != TYPE_NULL )
		return result;
	//�ж��Ƿ��Ƕ���
	if( (result = judgeTwain(CardVect)).type != TYPE_NULL )
		return result;
	//�ж��Ƿ�3��1
	if( (result = judgeThreeBandOne(CardVect)).type != TYPE_NULL )
		return result;
	//�ж��Ƿ�3��2
	if( (result = judgeThreeBandTwo(CardVect)).type != TYPE_NULL )
		return result;
	//�ж��Ƿ�3����
	if( (result = judgeThreeBandZero(CardVect)).type != TYPE_NULL )
		return result;
	//�ж��Ƿ�����
	if( (result = judgeContinuousTwain(CardVect)).type != TYPE_NULL )
		return result;
	//�ж��Ƿ���ը��
	if( (result = judgeBomb(CardVect)).type != TYPE_NULL )
		return result;
	//�ж��Ƿ��Ƿɻ�
	if( (result = judgeAircraft(CardVect)).type != TYPE_NULL )
		return result;
	//�ж���4��2
	if( (result = judgeFourBandTwo(CardVect)).type != TYPE_NULL )
		return result;

	return result;
}
//Ϊ2��֮��������͸����
void CPlayer::addInTowPointCardLayer(Point begin,Point end)
{
	if( m_vCardVect.size() == 0 )
		return;

	int endW = m_vCardVect[0]->sprite->getTextureRect().size.width;
	int endH = m_vCardVect[0]->sprite->getTextureRect().size.height;
	int endX = m_vCardVect[0]->sprite->getPositionX();
	int endY = m_vCardVect[0]->sprite->getPositionY();
	int CardbeginX = m_vCardVect[m_vCardVect.size() - 1]->sprite->getPositionX();

	//�����Χ�������Ƶķ�Χ�Ͳ�����
	if( begin.x < CardbeginX && begin.x < end.x )
		return ;
	//�����Χ�������Ƶķ�Χ�Ͳ�����
	if( begin.x > endX + endW && begin.x > end.x )
		return ;

	for( int i = 0; i < (int)m_vCardVect.size(); ++i )
		m_vCardVect[i]->layerColor->setVisible(m_vCardVect[i]->popup = false);

	for( int i = 0; i < (int)m_vCardVect.size(); ++i )
	{
		if( end == begin )//����
		{
			ME_Rect rect,that;
			//������?
			int w = ( begin.x > end.x ) ? 20 : m_vCardVect[i]->sprite->getTextureRect().size.width;
			rect.bottom = m_vCardVect[i]->sprite->getPositionY();
			rect.left = m_vCardVect[i]->sprite->getPositionX();
			rect.top = m_vCardVect[i]->sprite->getPositionY() + m_vCardVect[i]->sprite->getTextureRect().size.height;
			rect.right = m_vCardVect[i]->sprite->getPositionX() + w;

			//��һ�ε��µĵ� �� �����ƶ��ĵ� ���һ������
			that.bottom = begin.y;
			that.top = begin.y;
			that.left = MIN(begin.x,end.x);
			that.right = MAX(end.x,begin.x);
			//�жϵ��Ƿ����Ƶķ�Χ��
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

		if( begin.x > end.x )//������
			cardX += cardW;
		else if( i == m_vCardVect.size() - 1)//��ߵ�һ��
		{
			if( PressBeginX >= cardX && PressBeginX <= cardX + cardW && PressY >= cardY && PressY <= cardY + cardH ) 
			{
				m_vCardVect[i]->layerColor->setPosition(m_vCardVect[i]->sprite->getPosition());//����λ��
				m_vCardVect[i]->layerColor->setVisible(m_vCardVect[i]->popup = true);//����Ϊѡ�У���Ӱ�͸����
				continue;
			}
		}
		//��2��֮���������Ϊѡ��
		if( cardX >= PressBeginX && cardX <= PressEndX && PressY >= cardY && PressY <= cardY + cardH ) 
		{
			m_vCardVect[i]->layerColor->setPosition(m_vCardVect[i]->sprite->getPosition());//����λ��
			m_vCardVect[i]->layerColor->setVisible(m_vCardVect[i]->popup = true);//����Ϊѡ�У���Ӱ�͸����
		}
	}
}
//������ȫ�����Գ�ʼ��
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
//�Ƶİ�͸�����ʼ��
void CPlayer::setCardLayerInit()
{
	for( int i = (int)m_vCardVect.size() - 1; i >= 0 ; --i )
	{
		m_vCardVect[i]->popup = false;
		m_vCardVect[i]->layerColor->setVisible(false);
	}
}
//���Ҫ������Ƶ�m_SendVect
void CPlayer::addSendCardVect(vector<stCardSpr*>& vect )
{
	m_vSendVect.clear(); 
	for( int i = (int)m_vCardVect.size() - 1; i >= 0 ; --i )
	{
		if( m_vCardVect[i]->opt )
			vect.push_back(m_vCardVect[i]);
	}
}
//�ƶ���ѡ�е���
void CPlayer::moveOptCard()
{
	vector<enCard> cardVect;

	vector<stSaveCardData> returnVect;
	for( int i = (int)m_vCardVect.size() - 1; i >= 0 ; --i )
	{
		if( m_vCardVect[i]->popup  )//�Ƿ�ѡ��
		{
			m_vCardVect[i]->popup = false;
			m_vMoveVect.push_back(m_vCardVect[i]);
			cardVect.push_back(m_vCardVect[i]->card);
		}
	}
	
	vector<enCard> tempVect = cardVect;

	if( cardVect.size() >= 5 )
	{
		FindContinuousTwain(cardVect,returnVect);	//�ж��Ƿ�������
		FindStraight(tempVect,returnVect);//�ж��Ƿ���˳��
	}

	//��˳�ӻ�������
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

		if( m_vMoveVect[i]->opt )//�Ƿ��Ѿ�����
			y = DOWN_Y;
		else
			y += 30;

		//�ƶ�
		CCActionInterval *forward = CCMoveTo::create(0.15f,ccp(x, y));  
		m_vMoveVect[i]->sprite->runAction(forward);
		m_vMoveVect[i]->opt   = m_vMoveVect[i]->opt ? false : true;
	}
	addSendCardVect(m_vSendVect);//���Ҫ������Ƶ�M_SendVect
	setCardLayerInit();//������λ�ó�ʼ��
	m_vMoveVect.clear();
}

//����һ�������Ƶ����X��X+W
bool CPlayer::getCardPoint(Point p,Point& left,Point& rigth)
{
	for( int i = 0; i < (int)m_vCardVect.size() ; ++i )
	{
		ME_Rect rect;
		rect.bottom = m_vCardVect[i]->sprite->getPositionY();
		rect.left = m_vCardVect[i]->sprite->getPositionX();
		rect.top = m_vCardVect[i]->sprite->getPositionY() + m_vCardVect[i]->sprite->getTextureRect().size.height;
		rect.right = m_vCardVect[i]->sprite->getPositionX() + m_vCardVect[i]->sprite->getTextureRect().size.width;

		if( MyPtInRect(&rect,p.x,p.y)) //�жϵ��Ƿ����Ƶķ�Χ
		{
			int w = (i == 0) ? 82 : 20;
			left = m_vCardVect[i]->sprite->getPosition();
			rigth = left + Point(w,0);
			return true;
		}
	}

	return false;
}
//ѡ������
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
	//���þ������ɫ����������
	for( int i = (int)vect.size() - 1; i >= 0; --i )
	{
		vect[i]->sprite->setZOrder( (int)vect.size() - i );
		vect[i]->layerColor->setZOrder((int)vect.size() - i );
	}
}
//���ô��ȥ���Ƶ�λ��
void CPlayer::setShowCardPos(vector<stCardSpr*>& vect,enDir dir)
{
	Size size = Director::getInstance()->getVisibleSize();
	float ScaleW = 0.7f;
	float ScaleH = 0.7f;
	int count = 0;
	int picW = 82 * ScaleW;
	int picH = 114 * ScaleH;
	int interval = 20;//������֮��ļ��
	Vec2 p = Vec2::ZERO;

	switch( dir )
	{
	case DOWN:
		{
			int length = vect.size() * interval + picW - interval;	//�ܹ��Ƶĳ���
			p = Vec2(size.width / 2 - length / 2 ,SHOWCARD_Y);
		}break;
	case RIGTH:
		{
			int length = vect.size() * 20;//�ܹ��Ƶĳ���
			p = Vec2(size.width - picW - 220 - length ,size.height / 2 + 60);
		}break;
	case LEFT:
		{
			int length = vect.size() * 20;//�ܹ��Ƶĳ���
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
//�Ű�
void CPlayer::arrange(vector<stCardSpr*>& vect,enDir dir)
{
	Size size = Director::getInstance()->getVisibleSize();//��Ļ��С
	int count = 0;
	switch( dir )
	{
	case DOWN:
		{
			//�ܹ��Ƶĳ���
			int length = vect.size() * 25 + 82 - 23;
			Vec2 p = Vec2(size.width / 2 - length / 2 ,DOWN_Y);
			
			//����
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

			//����
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

			//����
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

	//��ʾ��ʣ��������
	string str = "ʣ";
	_itoa(vect.size(),buf,10);
	str = str + buf + "��";
	GBKToUTF8(str,"gb2312","utf-8");//����ת��
	m_LabelCardCount->setString(str.c_str());

	Point pos = vect[0]->sprite->getPosition();
	Size  cardSize = vect[0]->sprite->getTextureRect().size;
	Size  strSize  = m_LabelCardCount->getContentSize();
	m_LabelCardCount->setPosition(ccp(pos.x + cardSize.width/2 - strSize.width/2,pos.y - strSize.height - 5));
	m_LabelCardCount->setVisible(true);	
}
//�Ƶ�λ�ø���
void CPlayer::cardPosUpdate(enDir dir)
{
	arrange(m_vCardVect,dir);
}
//Ai
CPlayer::enAiResult CPlayer::Ai(stPlayerData lastPlayer,stPlayerData nowPlayer,stPlayerData nextPlayer,int dir,stResult result)//��һ����ҳ����ƣ�Ĭ��û��		
{
	vector<enCard> myCardVect;//�Լ�����ʣ�µ���
	vector<enCard> returnCardVect;//Ai���س�������

	for ( int i = 0; i < (int)m_vCardVect.size(); ++i )
		myCardVect.push_back(m_vCardVect[i]->card);

	//���ø���AI
	enAiResult aiResult = logic::Ai(m_enState,myCardVect,returnCardVect,CPlayer::m_LandlordNoCryCard,
									lastPlayer,nowPlayer,nextPlayer,dir,result);
	switch ( aiResult )
	{
	case en_AiWait://�ȴ�
		{
		}break;
	case en_CryLoadlord://�е���
		{
			CryLoadlord(true);
		}break;
	case en_NoCryLoadlord://���е���
		{
			CryLoadlord(false);
		}break;
	case en_RobLaodlord://������
		{
			RobLoadlord(true);
		}break;
	case en_NoRobLaodlord://��������
		{
			RobLoadlord(false);
		}break;
	case en_Double://�ӱ�
		{
			Double(true);
		}break;
	case en_NoDouble://���ӱ�
		{
			Double(false);
		}break;
	case en_NoDenmand://Ҫ����
		{
			noDenmand();
		}break;
	case en_AppearCard:	 //����
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

			bool go = appearCard(result);//����
			if( !go )//Ҫ����
				noDenmand();

		}break;
	}

	m_enState = en_Wait;

	return aiResult;
}