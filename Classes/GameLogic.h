#pragma once
#include "player.h"
#include "public.h"

#define CHUPAI_WAIT_TIME	30 //出牌等待时间
#define JIAODIZHU_WAIT_TIME 3  //叫地主 ，抢地主，加倍 等待时间

class CGameLogic
{
public:

	enum enResult
	{
		en_New,
		en_Win,
		en_Lose,
		en_Wait
	};

	CPlayer::enDir   m_BeginPlayerDir;//第一个
	CPlayer::enDir   m_EndPlayerDir;
	CPlayer*  m_pCryPlayer;
	CPlayer*  m_pRobPlayer;
	CPlayer*  m_pChuPai;
	stResult m_Result;//上一个玩家出的牌
	int m_iCount;
	int m_iIndex;
	int m_iAppearCardDir;//出牌的玩家的位置

	CGameLogic();
	~CGameLogic();

	//初始化
	void Init();

	//新一局
	void anew(vector<CPlayer*> vect);

	//游戏运行
	enResult Run(CPlayer* downPlayer,//下方的玩家
			 CPlayer* rightPlayer,//右方的玩家
			 CPlayer* leftPlayer,//左方的玩家
			 vector<CPlayer::stCardSpr> vect,//底牌
			 bool ai);//下方玩家是否调用ai

	//设置谁第一个出牌 谁最后一个出牌
	void setFirstPalyer(CPlayer::enDir & begin,//第一个出牌的玩家的位置
					    CPlayer::enDir & end,  //最后一个出牌的玩家的位置
						CPlayer::enDir Landlord);//地主的位置

	//加倍
	void Double(CPlayer* pNowPlayer,//当前玩家
				CPlayer* pNextPlayer,//下一个玩家
			    CPlayer* pLastPlayer);//上一个玩家

	//出牌
	enResult appearCard(CPlayer* pNowPlayer,//当前玩家
				    CPlayer* pNextPlayer,//下一个玩家
				    CPlayer* pLastPlayer,//上一个玩家
					CPlayer* pDownPlayer);//下方的玩家

	//不要
	void noDenmand(CPlayer* pNowPlayer,//当前玩家
				   CPlayer* pNextPlayer,//下一个玩家
				   CPlayer* pLastPlayer);//上一个玩家

	//抢地主
	void RobLoadlord(bool rob,//是否抢地主
				 	 CPlayer* pNowPlayer,//当前玩家
					 CPlayer* pNextPlayer,//下一个玩家
					 CPlayer* pLastPlayer,//上一个玩家
					 vector<CPlayer::stCardSpr> vect);//底牌	
	
	//叫地主
	bool CryLoadlord(bool cry,//是否叫地主
				 	 CPlayer* pNowPlayer,//当前玩家
					 CPlayer* pNextPlayer,//下一个玩家
					 CPlayer* pLastPlayer,//上一个玩家
					 vector<CPlayer::stCardSpr> vect);//底牌		

};
