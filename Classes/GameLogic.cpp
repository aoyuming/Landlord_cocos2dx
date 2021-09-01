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
	m_iAppearCardDir(-1),//��û����
	m_Result(stResult(TYPE_NULL,-1,0))
{}
CGameLogic::~CGameLogic()
{

}

//��ʼ��
void CGameLogic::Init()
{
	m_Result = stResult(TYPE_NULL,-1,0);
	m_pCryPlayer = NULL;
	m_pRobPlayer = NULL;
	m_pChuPai = NULL;
}
//��һ��
void CGameLogic::anew(vector<CPlayer*> vect)
{
	Init();//��ʼ��
	setFirstPalyer(m_BeginPlayerDir,m_EndPlayerDir,(CPlayer::enDir)m_iCount++);
	m_iCount = m_iCount == 3 ? 0 : m_iCount;

	for( int i = 0; i < (int)vect.size(); ++i )
	{
		if( vect[i]->m_Dir == m_BeginPlayerDir )
		{
			vect[i]->m_enState = CPlayer::enState::en_JiaoDiZhu;
			vect[i]->startTimer(JIAODIZHU_WAIT_TIME);//���ü�ʱ��
			break;
		}
	}
}
//�ӱ�
void CGameLogic::Double(CPlayer* pNowPlayer,//��ǰ���
			CPlayer* pNextPlayer,//��һ�����
			CPlayer* pLastPlayer)//��һ�����
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
		if( player[i]->m_bLandlord )//�ҵ�����
		{
			tempPlayer = player[i];
			break;
		}
	}

	//���õ�������
	tempPlayer->m_enState = CPlayer::enState::en_ChuPai;
	tempPlayer->startTimer(CHUPAI_WAIT_TIME);//���ü�ʱ��
	setFirstPalyer(m_BeginPlayerDir,m_EndPlayerDir,tempPlayer->m_Dir);
}
//����˭��һ������ ˭���һ������
void CGameLogic::setFirstPalyer(CPlayer::enDir & begin,//��һ�����Ƶ���ҵ�λ��
								CPlayer::enDir & end,  //���һ�����Ƶ���ҵ�λ��
						        CPlayer::enDir Landlord)//������λ��
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
//����
CGameLogic::enResult CGameLogic::appearCard(CPlayer* pNowPlayer,//��ǰ���
							CPlayer* pNextPlayer,//��һ�����
							CPlayer* pLastPlayer,//��һ�����
							CPlayer* pDownPlayer)//�·������
{
	if( pNowPlayer->m_vCardVect.size() == 0 )//û����
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
	pNextPlayer->startTimer(CHUPAI_WAIT_TIME);//���ü�ʱ��
	m_pChuPai = pNowPlayer;//��ǰ���Ƶ����
	m_Result = pNowPlayer->getResult();//��ǰ��ҳ����Ƶ���Ϣ
	m_iAppearCardDir = pNowPlayer->m_Dir;
	setFirstPalyer(m_BeginPlayerDir,m_EndPlayerDir,m_pChuPai->m_Dir);
	return en_Wait;
}
//��Ҫ
void CGameLogic::noDenmand(CPlayer* pNowPlayer,//��ǰ���
						   CPlayer* pNextPlayer,//��һ�����
						   CPlayer* pLastPlayer)//��һ�����
{
	if( pNowPlayer->m_Dir == m_EndPlayerDir )
	{
		pNextPlayer->m_enState = CPlayer::enState::en_ChuPai;
		CPlayer::SoundLevelInit();//�����ȼ���ʼ��
		m_Result = stResult(TYPE_NULL,-1,0);
	}
	else
		pNextPlayer->m_enState = CPlayer::enState::en_GenPai;

	pNextPlayer->startTimer(CHUPAI_WAIT_TIME);//���ü�ʱ��
	pLastPlayer->m_enState = CPlayer::enState::en_Wait;
	pNowPlayer->m_enState = CPlayer::enState::en_Wait;

	if( pNowPlayer->m_bLandlord )//���õ�����Ҫ����
		CPlayer::m_LandlordNoCryCard = m_Result.type;

}
//������
void CGameLogic::RobLoadlord(bool rob,//�Ƿ�������
							CPlayer* pNowPlayer,//��ǰ���
							CPlayer* pNextPlayer,//��һ�����
							CPlayer* pLastPlayer,//��һ�����
							vector<CPlayer::stCardSpr> vect)//����	
{

	CPlayer* player[] = { pNowPlayer,pNextPlayer,pLastPlayer};
	if( rob )//������
	{
		if( pNowPlayer->m_Dir == m_BeginPlayerDir )//���һ��������
		{
			pNowPlayer->setLandlord(vect);//����Ϊ����
			pNowPlayer->setUpString("����");
			pNextPlayer->setUpString("ũ��");
			pLastPlayer->setUpString("ũ��");
			setFirstPalyer(m_BeginPlayerDir,m_EndPlayerDir,pNowPlayer->m_Dir);
			for( int j = 0; j < 3; ++j )//ȫ�������Ƿ�ӱ�
			{
				player[j]->startTimer(JIAODIZHU_WAIT_TIME);
				player[j]->m_enState = CPlayer::enState::en_JiaBei;
			}
		}
		else if( pNextPlayer->m_bNoCryLoadlord )//��һ����û�е���
		{
			m_pRobPlayer = pNowPlayer;
			pNowPlayer->m_enState = CPlayer::enState::en_Wait;
			pNextPlayer->m_enState = CPlayer::enState::en_Wait;
			pLastPlayer->m_enState = CPlayer::enState::en_QiangDiZhu;
			pLastPlayer->startTimer(JIAODIZHU_WAIT_TIME);//���ü�ʱ��
		}
		else//�������һ��
		{
			m_pRobPlayer = pNowPlayer;
			pLastPlayer->m_enState = CPlayer::enState::en_Wait;
			pNowPlayer->m_enState = CPlayer::enState::en_Wait;
			pNextPlayer->m_enState = CPlayer::enState::en_QiangDiZhu;
			pNextPlayer->startTimer(JIAODIZHU_WAIT_TIME);//���ü�ʱ��
		}
	}
	else//����
	{
		if( m_pRobPlayer == NULL && pNowPlayer->m_Dir == m_EndPlayerDir )//û��������
		{
			m_pCryPlayer->setLandlord(vect);//����Ϊ����
			m_pCryPlayer->setUpString("����");

			for( int k = 0; k < 3;++k )
			{
				if( player[k]->m_Dir != m_pCryPlayer->m_Dir )
				{
					player[k]->setUpString("ũ��");
				}
			}
	
			setFirstPalyer(m_BeginPlayerDir,m_EndPlayerDir,m_pCryPlayer->m_Dir);
			for( int j = 0; j < 3; ++j )//ȫ�������Ƿ�ӱ�
			{
				player[j]->startTimer(JIAODIZHU_WAIT_TIME);
				player[j]->m_enState = CPlayer::enState::en_JiaBei;
			}
		}
		else if( pNowPlayer->m_Dir == m_BeginPlayerDir ) //���һ��
		{                
			CPlayer* pyr = ( m_pRobPlayer == NULL  ) ? m_pCryPlayer : m_pRobPlayer;//û��������
			pyr->setLandlord(vect);//����Ϊ����
			pyr->setUpString("����");
			for( int k = 0; k < 3;++k )
			{
				if( player[k]->m_Dir != pyr->m_Dir )
				{
					player[k]->setUpString("ũ��");
				}
			}

			setFirstPalyer(m_BeginPlayerDir,m_EndPlayerDir,pyr->m_Dir);
			for( int j = 0; j < 3; ++j )//ȫ�������Ƿ�ӱ�
			{
				player[j]->startTimer(JIAODIZHU_WAIT_TIME);
				player[j]->m_enState = CPlayer::enState::en_JiaBei;
			}

		}
		else if( pNextPlayer->m_bNoCryLoadlord )//��һ����û�е���
		{
			//û��������
			pLastPlayer->setLandlord(vect);//����Ϊ����
			pLastPlayer->setUpString("����");
			for( int k = 0; k < 3;++k )
			{
				if( player[k]->m_Dir != pLastPlayer->m_Dir )
				{
					player[k]->setUpString("ũ��");
				}
			}

			setFirstPalyer(m_BeginPlayerDir,m_EndPlayerDir,pLastPlayer->m_Dir);
			for( int j = 0; j < 3; ++j )//ȫ�������Ƿ�ӱ�
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
			pNextPlayer->startTimer(JIAODIZHU_WAIT_TIME);//���ü�ʱ��
		}
	}
}
//�е���
bool CGameLogic::CryLoadlord(bool cry,//�Ƿ�е���
							CPlayer* pNowPlayer,//��ǰ���
							CPlayer* pNextPlayer,//��һ�����
							CPlayer* pLastPlayer,//��һ�����
							vector<CPlayer::stCardSpr> vect)//����	
{ 
	if( cry )//�е���
	{
		if( m_EndPlayerDir == pNowPlayer->m_Dir && m_pCryPlayer == NULL )//��һ�˽е���
		{
			pNowPlayer->setLandlord(vect);//����Ϊ����
			pNowPlayer->setUpString("����");
			pLastPlayer->setUpString("ũ��");
			pNextPlayer->setUpString("ũ��");
			setFirstPalyer(m_BeginPlayerDir,m_EndPlayerDir,pNowPlayer->m_Dir);

			CPlayer* player[] = {pNowPlayer,pNextPlayer,pLastPlayer};
			for( int j = 0; j < 3; ++j )//ȫ�������Ƿ�ӱ�
			{
				player[j]->startTimer(JIAODIZHU_WAIT_TIME);
				player[j]->m_enState = CPlayer::enState::en_JiaBei;
			}

		}
		else
		{
			if( pLastPlayer->m_bNoCryLoadlord ) //��һ����  ���е���
				setFirstPalyer(m_BeginPlayerDir,m_EndPlayerDir,pNowPlayer->m_Dir);
			
			m_pCryPlayer = pNowPlayer;
			pLastPlayer->m_enState = CPlayer::enState::en_Wait;
			pNowPlayer->m_enState = CPlayer::enState::en_Wait;
			pNextPlayer->m_enState = CPlayer::enState::en_QiangDiZhu;
			pNextPlayer->startTimer(JIAODIZHU_WAIT_TIME);//���ü�ʱ��
		}
	}
	else//����
	{
		if( m_pCryPlayer == NULL && pNowPlayer->m_Dir == m_EndPlayerDir )//û�˽е���
		{
			//���·���
			return true;
		}
		else
		{
			if( pNowPlayer->m_Dir == m_BeginPlayerDir )//��һ���� ���е���
				pNowPlayer->m_bNoCryLoadlord = true;
			
			pLastPlayer->m_enState = CPlayer::enState::en_Wait;
			pNowPlayer->m_enState = CPlayer::enState::en_Wait;
			pNextPlayer->m_enState = CPlayer::enState::en_JiaoDiZhu;
			pNextPlayer->startTimer(JIAODIZHU_WAIT_TIME);//���ü�ʱ��
		}
	}
	
	return false;
}

//��Ϸ����
CGameLogic::enResult CGameLogic::Run(CPlayer* downPlayer,//�·������
					 CPlayer* rightPlayer,//�ҷ������
					 CPlayer* leftPlayer,//�󷽵����
					 vector<CPlayer::stCardSpr> vect,//����
					 bool ai)//�·�����Ƿ����ai
{
	CPlayer* player[] = { downPlayer,rightPlayer,leftPlayer };

	vector<CPlayer*> vectPlayer;
	vectPlayer.push_back(downPlayer);
	vectPlayer.push_back(rightPlayer);
	vectPlayer.push_back(leftPlayer);

	//�·�������AI
	if( !ai && player[m_iIndex]->m_Dir == CPlayer::enDir::DOWN )
	{
		if( m_iIndex == 2 )
			m_iIndex = 0;
		else
			m_iIndex++;
		return en_Wait;
	}
 	
	int next = (m_iIndex == 2) ? 0 : m_iIndex + 1;//��һλ
	int last = (m_iIndex == 0) ? 2 : m_iIndex - 1;//��һλ
	player[m_iIndex]->stopTimer();//ֹͣ��ʱ��

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

	CPlayer::enAiResult re = player[m_iIndex]->Ai(lastPlayer,nowPlayer,nextPlayer,m_iAppearCardDir,m_Result);//Ai ����һ�����

	switch ( re )
	{
	case CPlayer::enAiResult::en_AiWait://�ȴ�
		{

		}break;
	case CPlayer::enAiResult::en_CryLoadlord://�е���
		{
			CryLoadlord(true,player[m_iIndex],player[next],player[last],vect);

		}break;
	case CPlayer::enAiResult::en_NoCryLoadlord://���е���
		{
			if( CryLoadlord(false,player[m_iIndex],player[next],player[last],vect) )
				return en_New;
		}break;
	case CPlayer::enAiResult::en_RobLaodlord://������
		{
			RobLoadlord(true,player[m_iIndex],player[next],player[last],vect);

		}break;
	case CPlayer::enAiResult::en_NoRobLaodlord://��������
		{
			RobLoadlord(false,player[m_iIndex],player[next],player[last],vect);
				
		}break;
	case CPlayer::enAiResult::en_Double://�ӱ�
	case CPlayer::enAiResult::en_NoDouble://���ӱ�
		{
			Double(player[m_iIndex],player[next],player[last]);

		}break;
	case CPlayer::enAiResult::en_NoDenmand://Ҫ����
		{
			noDenmand(player[m_iIndex],player[next],player[last]);

		}break;
	case CPlayer::enAiResult::en_AppearCard://����
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


