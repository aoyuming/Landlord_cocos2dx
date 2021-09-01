#include "GameLogic.h"


CGameLogic::CGameLogic()
	:
	m_BeginPlayerDir(CPlayer::enDir::DOWN),
	m_EndPlayerDir(CPlayer::enDir::LEFT),
	m_pCryPlayer(NULL),
	m_pRobPlayer(NULL),
	m_iCount(0),
	m_iIndex(0),
	m_pChuPai(NULL),
	m_iAppearCardDir(-1),//还没出牌
	m_Result(stResult(TYPE_NULL,-1,0))
{}
CGameLogic::~CGameLogic()
{

}

//初始化
void CGameLogic::Init()
{
	m_Result = stResult(TYPE_NULL,-1,0);
	m_pCryPlayer = NULL;
	m_pRobPlayer = NULL;
	m_pChuPai = NULL;
}
//新一局
void CGameLogic::anew(vector<CPlayer*> vect)
{
	Init();//初始化
	setFirstPalyer(m_BeginPlayerDir,m_EndPlayerDir,(CPlayer::enDir)m_iCount++);
	m_iCount = m_iCount == 3 ? 0 : m_iCount;

	for( int i = 0; i < (int)vect.size(); ++i )
	{
		if( vect[i]->m_Dir == m_BeginPlayerDir )
		{
			vect[i]->m_enState = CPlayer::enState::en_JiaoDiZhu;
			vect[i]->startTimer(JIAODIZHU_WAIT_TIME);//设置计时器
			break;
		}
	}
}
//加倍
void CGameLogic::Double(CPlayer* pNowPlayer,//当前玩家
			CPlayer* pNextPlayer,//下一个玩家
			CPlayer* pLastPlayer)//上一个玩家
{

	pNowPlayer->m_enState = CPlayer::enState::en_Wait;
	
	CPlayer* player[] = {pNextPlayer,pLastPlayer,pNowPlayer};
	CPlayer* tempPlayer = NULL;
	for( int i = 0; i < 3; ++i )
	{
		if( player[i]->m_enState == CPlayer::enState::en_JiaBei )
			return;
	}
	for( int i = 0; i < 3; ++i )
	{
		if( player[i]->m_bLandlord )//找到地主
		{
			tempPlayer = player[i];
			break;
		}
	}

	//设置地主出牌
	tempPlayer->m_enState = CPlayer::enState::en_ChuPai;
	tempPlayer->startTimer(CHUPAI_WAIT_TIME);//设置计时器
	setFirstPalyer(m_BeginPlayerDir,m_EndPlayerDir,tempPlayer->m_Dir);
}
//设置谁第一个出牌 谁最后一个出牌
void CGameLogic::setFirstPalyer(CPlayer::enDir & begin,//第一个出牌的玩家的位置
								CPlayer::enDir & end,  //最后一个出牌的玩家的位置
						        CPlayer::enDir Landlord)//地主的位置
{
	if( Landlord == CPlayer::enDir::DOWN )
	{
		begin = CPlayer::enDir::DOWN;
		end   = CPlayer::enDir::LEFT;
	}
	else if ( Landlord == CPlayer::enDir::RIGTH )
	{
		begin = CPlayer::enDir::RIGTH;
		end   = CPlayer::enDir::DOWN;
	}
	else if( Landlord == CPlayer::enDir::LEFT )
	{
		begin = CPlayer::enDir::LEFT;
		end   = CPlayer::enDir::RIGTH;
	}
}
//出牌
CGameLogic::enResult CGameLogic::appearCard(CPlayer* pNowPlayer,//当前玩家
							CPlayer* pNextPlayer,//下一个玩家
							CPlayer* pLastPlayer,//上一个玩家
							CPlayer* pDownPlayer)//下方的玩家
{
	if( pNowPlayer->m_vCardVect.size() == 0 )//没牌了
	{
		CPlayer* player[] = { pNowPlayer,pNextPlayer,pLastPlayer };
		for( int i = 0; i < 3; ++i )
		{
			player[i]->stopTimer();
			player[i]->showCard(false);
			player[i]->setAllLabelHide(false);
			player[i]->m_enState = CPlayer::enState::en_Wait;
		}

		if( pNowPlayer->m_bLandlord  && pNowPlayer != pDownPlayer && !pNowPlayer->m_bLandlord )
			return en_Lose;
		if( !pNowPlayer->m_bLandlord && pNowPlayer != pDownPlayer && !pDownPlayer->m_bLandlord )
			return en_Win;
		if( pDownPlayer->m_bLandlord && pNowPlayer == pDownPlayer )
			return en_Win;

		return en_Win;

	}

	pNextPlayer->m_enState = CPlayer::enState::en_GenPai;
	pLastPlayer->m_enState = CPlayer::enState::en_Wait;
	pNowPlayer->m_enState = CPlayer::enState::en_Wait;
	pNextPlayer->startTimer(CHUPAI_WAIT_TIME);//设置计时器
	m_pChuPai = pNowPlayer;//当前出牌的玩家
	m_Result = pNowPlayer->getResult();//当前玩家出的牌的信息
	m_iAppearCardDir = pNowPlayer->m_Dir;
	setFirstPalyer(m_BeginPlayerDir,m_EndPlayerDir,m_pChuPai->m_Dir);
	return en_Wait;
}
//不要
void CGameLogic::noDenmand(CPlayer* pNowPlayer,//当前玩家
						   CPlayer* pNextPlayer,//下一个玩家
						   CPlayer* pLastPlayer)//上一个玩家
{
	if( pNowPlayer->m_Dir == m_EndPlayerDir )
	{
		pNextPlayer->m_enState = CPlayer::enState::en_ChuPai;
		CPlayer::SoundLevelInit();//声音等级初始化
		m_Result = stResult(TYPE_NULL,-1,0);
	}
	else
		pNextPlayer->m_enState = CPlayer::enState::en_GenPai;

	pNextPlayer->startTimer(CHUPAI_WAIT_TIME);//设置计时器
	pLastPlayer->m_enState = CPlayer::enState::en_Wait;
	pNowPlayer->m_enState = CPlayer::enState::en_Wait;

	if( pNowPlayer->m_bLandlord )//设置地主不要的牌
		CPlayer::m_LandlordNoCryCard = m_Result.type;

}
//抢地主
void CGameLogic::RobLoadlord(bool rob,//是否抢地主
							CPlayer* pNowPlayer,//当前玩家
							CPlayer* pNextPlayer,//下一个玩家
							CPlayer* pLastPlayer,//上一个玩家
							vector<CPlayer::stCardSpr> vect)//底牌	
{

	CPlayer* player[] = { pNowPlayer,pNextPlayer,pLastPlayer};
	if( rob )//抢地主
	{
		if( pNowPlayer->m_Dir == m_BeginPlayerDir )//最后一个抢地主
		{
			pNowPlayer->setLandlord(vect);//设置为地主
			pNowPlayer->setUpString("地主");
			pNextPlayer->setUpString("农民");
			pLastPlayer->setUpString("农民");
			setFirstPalyer(m_BeginPlayerDir,m_EndPlayerDir,pNowPlayer->m_Dir);
			for( int j = 0; j < 3; ++j )//全部设置是否加倍
			{
				player[j]->startTimer(JIAODIZHU_WAIT_TIME);
				player[j]->m_enState = CPlayer::enState::en_JiaBei;
			}
		}
		else if( pNextPlayer->m_bNoCryLoadlord )//第一个人没叫地主
		{
			m_pRobPlayer = pNowPlayer;
			pNowPlayer->m_enState = CPlayer::enState::en_Wait;
			pNextPlayer->m_enState = CPlayer::enState::en_Wait;
			pLastPlayer->m_enState = CPlayer::enState::en_QiangDiZhu;
			pLastPlayer->startTimer(JIAODIZHU_WAIT_TIME);//设置计时器
		}
		else//不是最后一个
		{
			m_pRobPlayer = pNowPlayer;
			pLastPlayer->m_enState = CPlayer::enState::en_Wait;
			pNowPlayer->m_enState = CPlayer::enState::en_Wait;
			pNextPlayer->m_enState = CPlayer::enState::en_QiangDiZhu;
			pNextPlayer->startTimer(JIAODIZHU_WAIT_TIME);//设置计时器
		}
	}
	else//不抢
	{
		if( m_pRobPlayer == NULL && pNowPlayer->m_Dir == m_EndPlayerDir )//没人抢地主
		{
			m_pCryPlayer->setLandlord(vect);//设置为地主
			m_pCryPlayer->setUpString("地主");

			for( int k = 0; k < 3;++k )
			{
				if( player[k]->m_Dir != m_pCryPlayer->m_Dir )
				{
					player[k]->setUpString("农民");
				}
			}
	
			setFirstPalyer(m_BeginPlayerDir,m_EndPlayerDir,m_pCryPlayer->m_Dir);
			for( int j = 0; j < 3; ++j )//全部设置是否加倍
			{
				player[j]->startTimer(JIAODIZHU_WAIT_TIME);
				player[j]->m_enState = CPlayer::enState::en_JiaBei;
			}
		}
		else if( pNowPlayer->m_Dir == m_BeginPlayerDir ) //最后一个
		{                
			CPlayer* pyr = ( m_pRobPlayer == NULL  ) ? m_pCryPlayer : m_pRobPlayer;//没人抢地主
			pyr->setLandlord(vect);//设置为地主
			pyr->setUpString("地主");
			for( int k = 0; k < 3;++k )
			{
				if( player[k]->m_Dir != pyr->m_Dir )
				{
					player[k]->setUpString("农民");
				}
			}

			setFirstPalyer(m_BeginPlayerDir,m_EndPlayerDir,pyr->m_Dir);
			for( int j = 0; j < 3; ++j )//全部设置是否加倍
			{
				player[j]->startTimer(JIAODIZHU_WAIT_TIME);
				player[j]->m_enState = CPlayer::enState::en_JiaBei;
			}

		}
		else if( pNextPlayer->m_bNoCryLoadlord )//第一个人没叫地主
		{
			//没人抢地主
			pLastPlayer->setLandlord(vect);//设置为地主
			pLastPlayer->setUpString("地主");
			for( int k = 0; k < 3;++k )
			{
				if( player[k]->m_Dir != pLastPlayer->m_Dir )
				{
					player[k]->setUpString("农民");
				}
			}

			setFirstPalyer(m_BeginPlayerDir,m_EndPlayerDir,pLastPlayer->m_Dir);
			for( int j = 0; j < 3; ++j )//全部设置是否加倍
			{
				player[j]->startTimer(JIAODIZHU_WAIT_TIME);
				player[j]->m_enState = CPlayer::enState::en_JiaBei;
			}
		}
		else
		{
			pLastPlayer->m_enState = CPlayer::enState::en_Wait;
			pNowPlayer->m_enState = CPlayer::enState::en_Wait;
			pNextPlayer->m_enState = CPlayer::enState::en_QiangDiZhu;
			pNextPlayer->startTimer(JIAODIZHU_WAIT_TIME);//设置计时器
		}
	}
}
//叫地主
bool CGameLogic::CryLoadlord(bool cry,//是否叫地主
							CPlayer* pNowPlayer,//当前玩家
							CPlayer* pNextPlayer,//下一个玩家
							CPlayer* pLastPlayer,//上一个玩家
							vector<CPlayer::stCardSpr> vect)//底牌	
{ 
	if( cry )//叫地主
	{
		if( m_EndPlayerDir == pNowPlayer->m_Dir && m_pCryPlayer == NULL )//就一人叫地主
		{
			pNowPlayer->setLandlord(vect);//设置为地主
			pNowPlayer->setUpString("地主");
			pLastPlayer->setUpString("农民");
			pNextPlayer->setUpString("农民");
			setFirstPalyer(m_BeginPlayerDir,m_EndPlayerDir,pNowPlayer->m_Dir);

			CPlayer* player[] = {pNowPlayer,pNextPlayer,pLastPlayer};
			for( int j = 0; j < 3; ++j )//全部设置是否加倍
			{
				player[j]->startTimer(JIAODIZHU_WAIT_TIME);
				player[j]->m_enState = CPlayer::enState::en_JiaBei;
			}

		}
		else
		{
			if( pLastPlayer->m_bNoCryLoadlord ) //上一个人  不叫地主
				setFirstPalyer(m_BeginPlayerDir,m_EndPlayerDir,pNowPlayer->m_Dir);
			
			m_pCryPlayer = pNowPlayer;
			pLastPlayer->m_enState = CPlayer::enState::en_Wait;
			pNowPlayer->m_enState = CPlayer::enState::en_Wait;
			pNextPlayer->m_enState = CPlayer::enState::en_QiangDiZhu;
			pNextPlayer->startTimer(JIAODIZHU_WAIT_TIME);//设置计时器
		}
	}
	else//不叫
	{
		if( m_pCryPlayer == NULL && pNowPlayer->m_Dir == m_EndPlayerDir )//没人叫地主
		{
			//重新发牌
			return true;
		}
		else
		{
			if( pNowPlayer->m_Dir == m_BeginPlayerDir )//第一个人 不叫地主
				pNowPlayer->m_bNoCryLoadlord = true;
			
			pLastPlayer->m_enState = CPlayer::enState::en_Wait;
			pNowPlayer->m_enState = CPlayer::enState::en_Wait;
			pNextPlayer->m_enState = CPlayer::enState::en_JiaoDiZhu;
			pNextPlayer->startTimer(JIAODIZHU_WAIT_TIME);//设置计时器
		}
	}
	
	return false;
}

//游戏运行
CGameLogic::enResult CGameLogic::Run(CPlayer* downPlayer,//下方的玩家
					 CPlayer* rightPlayer,//右方的玩家
					 CPlayer* leftPlayer,//左方的玩家
					 vector<CPlayer::stCardSpr> vect,//底牌
					 bool ai)//下方玩家是否调用ai
{
	CPlayer* player[] = { downPlayer,rightPlayer,leftPlayer };

	vector<CPlayer*> vectPlayer;
	vectPlayer.push_back(downPlayer);
	vectPlayer.push_back(rightPlayer);
	vectPlayer.push_back(leftPlayer);

	//下方不调用AI
	if( !ai && player[m_iIndex]->m_Dir == CPlayer::enDir::DOWN )
	{
		if( m_iIndex == 2 )
			m_iIndex = 0;
		else
			m_iIndex++;
		return en_Wait;
	}
 	
	int next = (m_iIndex == 2) ? 0 : m_iIndex + 1;//下一位
	int last = (m_iIndex == 0) ? 2 : m_iIndex - 1;//上一位
	player[m_iIndex]->stopTimer();//停止计时器

	stPlayerData nextPlayer,lastPlayer,nowPlayer;
	nextPlayer.cardCount	 = player[next]->m_vCardVect.size();
	nextPlayer.dir			 = player[next]->m_Dir;
	nextPlayer.landlord		 = player[next]->m_bLandlord;

	lastPlayer.cardCount	 = player[last]->m_vCardVect.size();
	lastPlayer.dir			 = player[last]->m_Dir;
	lastPlayer.landlord		 = player[last]->m_bLandlord;

	nowPlayer.cardCount		 = player[m_iIndex]->m_vCardVect.size();
	nowPlayer.dir			 = player[m_iIndex]->m_Dir;
	nowPlayer.landlord		 = player[m_iIndex]->m_bLandlord;

	CPlayer::enAiResult re = player[m_iIndex]->Ai(lastPlayer,nowPlayer,nextPlayer,m_iAppearCardDir,m_Result);//Ai 返回一个结果

	switch ( re )
	{
	case CPlayer::enAiResult::en_AiWait://等待
		{

		}break;
	case CPlayer::enAiResult::en_CryLoadlord://叫地主
		{
			CryLoadlord(true,player[m_iIndex],player[next],player[last],vect);

		}break;
	case CPlayer::enAiResult::en_NoCryLoadlord://不叫地主
		{
			if( CryLoadlord(false,player[m_iIndex],player[next],player[last],vect) )
				return en_New;
		}break;
	case CPlayer::enAiResult::en_RobLaodlord://抢地主
		{
			RobLoadlord(true,player[m_iIndex],player[next],player[last],vect);

		}break;
	case CPlayer::enAiResult::en_NoRobLaodlord://不抢地主
		{
			RobLoadlord(false,player[m_iIndex],player[next],player[last],vect);
				
		}break;
	case CPlayer::enAiResult::en_Double://加倍
	case CPlayer::enAiResult::en_NoDouble://不加倍
		{
			Double(player[m_iIndex],player[next],player[last]);

		}break;
	case CPlayer::enAiResult::en_NoDenmand://要不起
		{
			noDenmand(player[m_iIndex],player[next],player[last]);

		}break;
	case CPlayer::enAiResult::en_AppearCard://出牌
		{
			enResult en = appearCard(player[m_iIndex],player[next],player[last],downPlayer);
			return en;
		}break;
	}

	if( m_iIndex == 2 )
		m_iIndex = 0;
	else
		m_iIndex++;

	return en_Wait;
}


