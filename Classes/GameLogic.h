#pragma once
#include "player.h"
#include "public.h"

#define CHUPAI_WAIT_TIME	30 //���Ƶȴ�ʱ��
#define JIAODIZHU_WAIT_TIME 3  //�е��� �����������ӱ� �ȴ�ʱ��

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

	CPlayer::enDir   m_BeginPlayerDir;//��һ��
	CPlayer::enDir   m_EndPlayerDir;
	CPlayer*  m_pCryPlayer;
	CPlayer*  m_pRobPlayer;
	CPlayer*  m_pChuPai;
	stResult m_Result;//��һ����ҳ�����
	int m_iCount;
	int m_iIndex;
	int m_iAppearCardDir;//���Ƶ���ҵ�λ��

	CGameLogic();
	~CGameLogic();

	//��ʼ��
	void Init();

	//��һ��
	void anew(vector<CPlayer*> vect);

	//��Ϸ����
	enResult Run(CPlayer* downPlayer,//�·������
			 CPlayer* rightPlayer,//�ҷ������
			 CPlayer* leftPlayer,//�󷽵����
			 vector<CPlayer::stCardSpr> vect,//����
			 bool ai);//�·�����Ƿ����ai

	//����˭��һ������ ˭���һ������
	void setFirstPalyer(CPlayer::enDir & begin,//��һ�����Ƶ���ҵ�λ��
					    CPlayer::enDir & end,  //���һ�����Ƶ���ҵ�λ��
						CPlayer::enDir Landlord);//������λ��

	//�ӱ�
	void Double(CPlayer* pNowPlayer,//��ǰ���
				CPlayer* pNextPlayer,//��һ�����
			    CPlayer* pLastPlayer);//��һ�����

	//����
	enResult appearCard(CPlayer* pNowPlayer,//��ǰ���
				    CPlayer* pNextPlayer,//��һ�����
				    CPlayer* pLastPlayer,//��һ�����
					CPlayer* pDownPlayer);//�·������

	//��Ҫ
	void noDenmand(CPlayer* pNowPlayer,//��ǰ���
				   CPlayer* pNextPlayer,//��һ�����
				   CPlayer* pLastPlayer);//��һ�����

	//������
	void RobLoadlord(bool rob,//�Ƿ�������
				 	 CPlayer* pNowPlayer,//��ǰ���
					 CPlayer* pNextPlayer,//��һ�����
					 CPlayer* pLastPlayer,//��һ�����
					 vector<CPlayer::stCardSpr> vect);//����	
	
	//�е���
	bool CryLoadlord(bool cry,//�Ƿ�е���
				 	 CPlayer* pNowPlayer,//��ǰ���
					 CPlayer* pNextPlayer,//��һ�����
					 CPlayer* pLastPlayer,//��һ�����
					 vector<CPlayer::stCardSpr> vect);//����		

};
