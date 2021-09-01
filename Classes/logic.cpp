#include "logic.h"

logic::logic(enState state)
	:
	m_enState(state),
	m_iCountHandCard(0)
{
	m_AppCardPlayer.cardCount = 0;
	m_AppCardPlayer.dir = 0;
	m_AppCardPlayer.landlord = false;
}


logic::~logic(void)
{
}
//��ʼ��
void logic::Init()
{
	m_vSingle.clear();		    //��ŵ���
	m_vTwain.clear();		    //��Ŷ���
	m_vThreeBandOne.clear();	//�������һ
	m_vThreeBandTwo.clear();	//���������
	m_vThreeBandZero.clear();	//���������
	m_vContinuousTwain.clear(); //�������
	m_vBomb.clear();			//���ը��
	m_vStraight.clear();		//���˳��
	m_vAircraft.clear();		//��ŷɻ�
	m_vFourBandTwo.clear();		//����Ĵ���
	m_vWangBomb.clear();		//�����ը
}
//������Ҫ�����Ʋ��ܳ���
int logic::countHandCard()
{
	int count = 0;
	count += m_vStraight.size();//˳��
	count += m_vContinuousTwain.size();//����
	count += m_vAircraft.size();//�ɻ�
	count += m_vThreeBandZero.size();
	count += m_vTwain.size();
	count += m_vSingle.size();
	count -= m_vBomb.size();
	count -= m_vWangBomb.size();

	int _3TiaoConut = m_vThreeBandZero.size();
	for( int i = 0; i < m_vAircraft.size(); ++i )
		_3TiaoConut += m_vAircraft[i].result.conut / 3;

	count -= _3TiaoConut;

	return count;
}
//����
void logic::unseamCard(vector<enCard>& CardVector)
{
	Init();//���
	FindWangBomb(CardVector,m_vWangBomb);//������ը
	FindBomb(CardVector,m_vBomb);//����ը��
	FindThreeBandZero(CardVector,m_vThreeBandZero);//����3����
	FindAircraft(CardVector,m_vAircraft);//���ҷɻ�
	FindStraight(CardVector,m_vStraight);//����˳��
	FindContinuousTwain(CardVector,m_vContinuousTwain);//��������
	FindTwain(CardVector,m_vTwain);//���Ҷ���
	FindSingle(CardVector,m_vSingle);//���ҵ���
}
//����
//��xx���޷�����ʱ�򣬿���ѡ���vectSaveCard���������
vector<enCard> logic::unseamCard(stResult result,//����
								 int addNum,//��Ҫ����ӵ�Vect���Ƶ�����
								 int cardCount,//vectSaveCard����Ʊ������cardCount
								 bool compare,//��������ͬ����
								 bool end,//�������
								 int jumpIndex,//�±���Ծ
								 int addJumpIndex,//��ӵ�ʱ����Ծ�±�
								 vector<stSaveCardData>& vectSaveCard)//��
{
	vector<enCard> vect;

	for( int i = 0; i < (int)vectSaveCard.size(); ++i )
	{
		if( compare )//����������ͬ
		{
			if( vectSaveCard[i].result.conut != cardCount )
				continue;
		}
		else//�������ָ��������
		{
			if( vectSaveCard[i].result.conut < cardCount )
				continue;
		}

		if( end )//�������
		{
			int beginIndex = 0;
			int endIndex   = vectSaveCard[i].vect.size() - 1;
			if( vectSaveCard[i].vect[beginIndex]/4 > result.value )
			{
				for( int j = 0; j < addNum; ++j )//Ҫ��ӵ�������
					vect.push_back(vectSaveCard[i].vect[beginIndex + j]);
				return vect;
			}

			if( vectSaveCard[i].vect[endIndex]/4 > result.value )
			{
				for( int j = 0; j < addNum; ++j )//Ҫ��ӵ�������
					vect.push_back(vectSaveCard[i].vect[endIndex - j]);
				return vect;
			}
		}
		else
		{
			//�������Է�����
			for( int k = 0; k < vectSaveCard[i].vect.size() ; k += jumpIndex )
			{
				bool Flags = true;
				if( this->compareCard(vectSaveCard[i].vect[k],Little_King) )
					Flags = false;

				if( Flags ? vectSaveCard[i].vect[k]/4 > result.value : vectSaveCard[i].vect[k] > result.value )
				{
					for( int j = 0; j < addNum / addJumpIndex ; ++j )//Ҫ��ӵ�����
					{
						for( int q = 0; q < addJumpIndex; ++q )
						{
							//Խ���ж�
							if( vect.size() != addNum && k + (j * addJumpIndex) + q >= vectSaveCard[i].vect.size() )
							{
								vect.clear();
								return vect;
							}

							//�Ƿ��㹻
							if( vect.size() == addNum )
								return vect;

							vect.push_back(vectSaveCard[i].vect[k + (j * addJumpIndex) + q]);
						}
					}

					return vect;
				}
			}
		}
	}
	return vect;
}
//������
vector<enCard> logic::appearSingle(stResult result,bool team,bool genpai)
{
	vector<enCard> Vect;
	if( genpai)
	{
		//��һ��ƫ�Ҵ�����ǵ���ʱ��һ��������ܸ��͸���������б����2�����ϵ���ʱ��ѡ��PASS�� 
		if( result.value >= Spandes_A / 4 && team  && m_iCountHandCard > 2 )
			return Vect;
		else if( team  && m_iCountHandCard > 2 )//����
		{
			//����
			if( (Vect = unseamCard(result,1,1,false,false,1,1,m_vSingle)).size() <= 0 )
				return Vect;
			//���ܴ���K
			if( Vect[0] / 4 >= Spandes_A / 4 )
				Vect.clear();

			return Vect;
		}

		/*
		���Ƶĸ���ԭ����������е��ƣ����֮,�������Ƹ�֮�������6�����ϵĵ�˳���Ÿ�֮��
		�����������֮���������˳��֮�������5����˳��֮�������˫˳��֮������ը֮������PASS�� 
		*/

		//����
		if( (Vect = unseamCard(result,1,1,false,false,1,1,m_vSingle)).size() > 0 )
			return Vect;
		//�����
		if( (Vect = unseamCard(result,1,1,false,false,1,1,m_vTwain)).size() > 0 )
			return Vect;
		//��6�����ϵ�˳��
		if( (Vect = unseamCard(result,1,6,false,true,1,1,m_vStraight)).size() > 0 )
			return Vect;
		//��3��
		if( (Vect = unseamCard(result,1,1,false,false,1,1,m_vThreeBandZero)).size() > 0 )
			return Vect;
		//��ɻ�
		if( (Vect = unseamCard(result,1,1,false,false,3,1,m_vAircraft)).size() > 0 )
			return Vect;
		//��5�����ϵ�˳��
		if( (Vect = unseamCard(result,1,5,false,false,1,1,m_vStraight)).size() > 0 )
			return Vect;
		//������
		if( (Vect = unseamCard(result,1,1,false,true,2,1,m_vContinuousTwain)).size() > 0 )
			return Vect;
	}
	else
	{
		/*
		���Ƶĳ���ԭ����Ϊ���ƿ�������������˳�ȴ����������ڳ�����ʱ��
		Ӧ���ȼ��һ������ + ��˳����������������������������� <= ���� + ���������ܺ� - 2ʱ
		�����ƣ����������1�ȵȡ�
		*/
		int _3TiaoConut = m_vThreeBandZero.size();
		for( int i = 0; i < m_vAircraft.size(); ++i )
			_3TiaoConut += m_vAircraft[i].result.conut / 3;

		//������
		if( _3TiaoConut < m_vTwain.size() + m_vSingle.size() )
		{
			//����
			if( (Vect = unseamCard(result,1,1,false,false,1,1,m_vSingle)).size() > 0 )
				return Vect;
		}
		else
		{
			//��3��1
			if(( Vect = appearThreeBandOne(result,false,false)).size() > 0)
				return Vect;
			//��3��2
			if(( Vect = appearThreeBandTwo(result,false,false)).size() > 0)
				return Vect;
		}
	}
	return Vect;
}
//������
vector<enCard> logic::appearTwain(stResult result,bool team,bool genpai)		
{
	vector<enCard> Vect;
	if( genpai )
	{
		//��һ��ƫ�Ҵ�����Ƕ���ʱ��һ��������ܸ��͸���������б����AA�����ϵ���ʱ��ѡ��PASS�� 
		if( result.value >= Spandes_K / 4 && team  && m_iCountHandCard > 2 )
			return Vect;
		else if( team  && m_iCountHandCard > 2 )//����
		{
			//�����
			if( (Vect = unseamCard(result,2,2,false,false,1,1,m_vTwain)).size() <= 0 )
				return Vect;
			//���ܴ���K
			if( Vect[0] / 4 >= Spandes_A / 4 )
				Vect.clear();

			return Vect;
		}

		/*
		���Ƶĸ���ԭ����������ж��ӣ����֮�������4�����ϵ�˫˳���Ÿ�֮�������������֮��
		�����˫˳��֮���������˳��֮������ը֮������PASS�� 
		*/

		//�����
		if( (Vect = unseamCard(result,2,2,false,false,1,1,m_vTwain)).size() > 0 )
			return Vect;
		//��4�����ϵĵ�˫˳
		if( (Vect = unseamCard(result,2,8,false,true,2,1,m_vContinuousTwain)).size() > 0 )
			return Vect;
		//��3��
		if( (Vect = unseamCard(result,2,2,false,false,1,1,m_vThreeBandZero)).size() > 0 )
			return Vect;
		//������
		if( (Vect = unseamCard(result,2,2,false,false,2,1,m_vContinuousTwain)).size() > 0 )
			return Vect;
		//��ɻ�
		if( (Vect = unseamCard(result,2,2,false,false,3,1,m_vAircraft)).size() > 0 )
			return Vect;
	}
	else
	{
		/*
		���ӵĳ���ԭ����Ϊ���ӿ�������������˳�ȴ����������ڳ�����ʱ��Ӧ���ȼ��һ����������˳������������������
		��������������� <= ���ӣ����������ܺͣ�2ʱ�������ӣ����������2�ȵȡ� 
		*/
		int _3TiaoConut = m_vThreeBandZero.size();
		for( int i = 0; i < m_vAircraft.size(); ++i )
			_3TiaoConut += m_vAircraft[i].result.conut / 3;

		if( _3TiaoConut <= m_vSingle.size() + m_vTwain.size() )//������
		{
			if( m_vTwain.size() > 0 )
				Vect = m_vTwain[0].vect;
		}
		else//������2
		{
			if( m_vThreeBandZero.size() > 0)
			{
				Vect = m_vThreeBandZero[0].vect;
				if( m_vSingle.size() > 0 )
				{
					Vect.push_back(m_vSingle[0].vect[0]);
					return Vect;
				}
				if( m_vTwain.size() > 0 )
				{
					Vect.push_back(m_vTwain[0].vect[0]);
					Vect.push_back(m_vTwain[0].vect[1]);
					return Vect;
				} 
			}
		}
	}
	return Vect;
}
//��3��1
vector<enCard> logic::appearThreeBandOne(stResult result,bool team,bool genpai)		
{
	vector<enCard> Vect;
	vector<enCard> temp;
	if( genpai )
	{
		//���һ��ƫ�Ҵ�������ǳ��˵��Ƽ�������������ͣ���ѡ��PASS�� 
		if( team && m_iCountHandCard > 2 )
			return Vect;

		/*
		����������1������2���Ƶĸ���ԭ�������������ͬ���͵������֮���������˳��֮������ը֮������PASS��
		ע��,ֻ�����������ڳ����Ժ�ʣһ����ʱ����ֱ�ӳ��������£������������2�� 
		*/

		//���3��
		Vect = unseamCard(result,3,3,false,false,1,1,m_vThreeBandZero);

		//û������
		if( Vect.size() == 0 )
		{
			//��ɻ�
			if( (Vect = unseamCard(result,3,3,false,false,3,1,m_vAircraft)).size() == 0 )
				return Vect;
		}

		//������2���һ���3�������� ��Ȼ�ͳ�2
		if( this->compareCard(Vect[0],Spandes_2) && m_AppCardPlayer.cardCount > 3 )
		{
			Vect.clear();
			return Vect;
		}

		//�����С�ĵ���
		if( (temp = unseamCard(stResult(TYPE_NULL,-1,0),1,1,false,false,1,1,m_vSingle)).size() > 0 )
		{
			if( (this->compareCard(temp[0],Spandes_2) || this->compareCard(temp[0],Little_King)) && m_AppCardPlayer.cardCount > 3 )
				;
			else
			{
				Vect.push_back(temp[0]);
				return Vect;
			}
		}
		//��6�����ϵ�˳��
		if( (temp = unseamCard(stResult(TYPE_NULL,-1,0),1,6,false,true,1,1,m_vStraight)).size() > 0 )
		{
			Vect.push_back(temp[0]);
			return Vect;
		}
		//�����
		if( (temp = unseamCard(stResult(TYPE_NULL,-1,0),1,1,false,false,1,1,m_vTwain)).size() > 0 )
		{
			Vect.push_back(temp[0]);
			return Vect;
		}

		//û�д��� ��Ҫ
		Vect.clear();
		return Vect;
	}
	else
	{
		/*
		�����ĳ���ԭ����Ϊ�������ƿ��Դ�һ�ŵ��ƻ�һ�����ӣ������ڳ�����ʱ��Ҫ����Ƿ��е��ƣ�
		����У����һ����С�ĵ��ƣ����û�У����ټ���Ƿ���ڶ��ӣ�����У����һ����С�Ķ��ӣ�
		������ƺͶ��Ӷ�û�У���������� 
		*/

		//��3��
		if((Vect = unseamCard(stResult(TYPE_NULL,-1,0),3,3,false,false,1,1,m_vThreeBandZero)).size() == 0)
			return Vect;

		//�����С�ĵ���
		if( (temp = unseamCard(stResult(TYPE_NULL,-1,0),1,1,false,false,1,1,m_vSingle)).size() > 0 )
		{
			if( (this->compareCard(temp[0],Spandes_2) || this->compareCard(temp[0],Little_King)) && m_AppCardPlayer.cardCount > 3 )
				;
			else
			{
				Vect.push_back(temp[0]);
				return Vect;
			}
		}
		//�����
		if( (temp = this->unseamCard(stResult(TYPE_NULL,-1,0),2,2,false,false,1,1,m_vTwain)).size() > 0 )
		{
			Vect.push_back(temp[0]);
			return Vect;
		}

		//��������
		return Vect;
	}
	return Vect;
}
//��3��2
vector<enCard> logic::appearThreeBandTwo(stResult result,bool team,bool genpai)	
{
	vector<enCard> Vect;
	vector<enCard> temp;
	if( genpai )
	{
		//���һ��ƫ�Ҵ�������ǳ��˵��Ƽ�������������ͣ���ѡ��PASS�� 
		if( team && m_iCountHandCard > 2 )
			return Vect;

		/*
		����������1������2���Ƶĸ���ԭ�������������ͬ���͵������֮���������˳��֮������ը֮������PASS��
		ע��,ֻ�����������ڳ����Ժ�ʣһ����ʱ����ֱ�ӳ��������£������������2�� 
		*/

		//���3��
		Vect = unseamCard(result,3,3,false,false,1,1,m_vThreeBandZero);

		//û������
		if( Vect.size() == 0 )
		{
			//��ɻ�
			if( (Vect = unseamCard(result,3,3,false,false,3,1,m_vAircraft)).size() == 0 )
				return Vect;
		}
		//������2���һ���3�������� ��Ȼ�ͳ�2
		if( this->compareCard(Vect[0],Spandes_2) && m_AppCardPlayer.cardCount > 3 )
		{
			Vect.clear();
			return Vect;
		}
		//�����С�Ķ���
		if( (temp = this->unseamCard(stResult(TYPE_NULL,-1,0),2,2,false,false,1,1,m_vTwain)).size() > 0 )
		{
			Vect.push_back(temp[0]);
			Vect.push_back(temp[1]);
			return Vect;
		}
		//��4�����ϵ�����
		if( (temp = unseamCard(stResult(TYPE_NULL,-1,0),2,8,false,true,2,1,m_vContinuousTwain)).size() > 0 )
		{
			Vect.push_back(temp[0]);
			Vect.push_back(temp[1]);
			return Vect;
		}

		Vect.clear();
		return Vect;
	}
	else
	{
		/*
		�����ĳ���ԭ����Ϊ�������ƿ��Դ�һ�ŵ��ƻ�һ�����ӣ������ڳ�����ʱ��Ҫ����Ƿ��е��ƣ�
		����У����һ����С�ĵ��ƣ����û�У����ټ���Ƿ���ڶ��ӣ�����У����һ����С�Ķ��ӣ�
		������ƺͶ��Ӷ�û�У���������� 
		*/

		//��3��
		if((Vect = unseamCard(stResult(TYPE_NULL,-1,0),3,3,false,false,1,1,m_vThreeBandZero)).size() == 0)
			return Vect;

		//�����С�ĵ���
		if( (temp = this->unseamCard(stResult(TYPE_NULL,-1,0),1,1,false,false,1,1,m_vSingle)).size() > 0 )
		{
			Vect.push_back(temp[0]);
			return Vect;
		}
		//�����
		if( (temp = this->unseamCard(stResult(TYPE_NULL,-1,0),2,2,false,false,1,1,m_vTwain)).size() > 0 )
		{
			Vect.push_back(temp[0]);
			Vect.push_back(temp[1]);
			return Vect;
		}

		//��������
		return Vect;
	}

	return Vect;
}
//��3����
vector<enCard> logic::appearThreeBandZero(stResult result,bool team,bool genpai)	
{
	vector<enCard> Vect;
	vector<enCard> temp;
	if( genpai )
	{
		//���һ��ƫ�Ҵ�������ǳ��˵��Ƽ�������������ͣ���ѡ��PASS�� 
		if( team  && m_iCountHandCard > 2 )
			return Vect;

		/*
		����������1������2���Ƶĸ���ԭ�������������ͬ���͵������֮���������˳��֮������ը֮������PASS��
		ע��,ֻ�����������ڳ����Ժ�ʣһ����ʱ����ֱ�ӳ��������£������������2�� 
		*/

		//���3��
		Vect = unseamCard(result,3,3,false,false,1,1,m_vThreeBandZero);

		//û������
		if( Vect.size() == 0 )
		{
			//��ɻ�
			if( (Vect = unseamCard(result,3,3,false,true,3,1,m_vAircraft)).size() == 0 )
				return Vect;
		}

		//������2���һ���3�������� ��Ȼ�ͳ�2
		if( this->compareCard(Vect[0],Spandes_2) && m_AppCardPlayer.cardCount > 3 )
		{
			Vect.clear();
			return Vect;
		}

		//��3����
		return Vect;
	}
	else
	{
		/*
		�����ĳ���ԭ����Ϊ�������ƿ��Դ�һ�ŵ��ƻ�һ�����ӣ������ڳ�����ʱ��Ҫ����Ƿ��е��ƣ�
		����У����һ����С�ĵ��ƣ����û�У����ټ���Ƿ���ڶ��ӣ�����У����һ����С�Ķ��ӣ�
		������ƺͶ��Ӷ�û�У���������� 
		*/

		//��3��
		if((Vect = unseamCard(stResult(TYPE_NULL,-1,0),3,3,false,false,1,1,m_vThreeBandZero)).size() == 0)
			return Vect;

		//�����С�ĵ���
		if( (temp = this->unseamCard(stResult(TYPE_NULL,-1,0),1,1,false,false,1,1,m_vSingle)).size() > 0 )
		{
			Vect.push_back(temp[0]);
			return Vect;
		}
		//�����
		if( (temp = this->unseamCard(stResult(TYPE_NULL,-1,0),2,2,false,false,1,1,m_vTwain)).size() > 0 )
		{
			Vect.push_back(temp[0]);
			Vect.push_back(temp[1]);
			return Vect;
		}

		//��������
		return Vect;
	}

	return Vect;
}
//������
vector<enCard> logic::appearContinuousTwain(stResult result,bool team,bool genpai)
{
	vector<enCard> vect;
	if( genpai )
	{
		//���һ��ƫ�Ҵ�������ǳ��˵��Ƽ�������������ͣ���ѡ��PASS�� 
		if( team  && m_iCountHandCard > 2 )
			return vect;

		/*
		˫˳�ĸ���ԭ������ͬ���͵��ƣ����֮�������ͬ������˫˳���������ͬ��������˳��
		�����ͬ��������˳,����ը֮������PASS�� 
		*/

		//����ͬ������˫˳
		if((vect = this->unseamCard(result,result.conut,result.conut,true,false,2,2,m_vContinuousTwain)).size() > 0 )
			return vect;
		//��ͬ������˫˳
		if((vect = this->unseamCard(result,result.conut,result.conut,false,false,2,2,m_vContinuousTwain)).size() > 0 )
			return vect;
		//����ͬ��������˳
		if((vect = this->unseamCard(result,result.conut,result.conut,true,false,3,2,m_vAircraft)).size() > 0 )
			return vect;
		//��ͬ��������˳
		if((vect = this->unseamCard(result,result.conut,result.conut,false,false,3,2,m_vAircraft)).size() > 0 )
			return vect;
	}
	else
	{
		/*
		˫˳�ĳ���ԭ��ֱ�ӳ�
		*/
		if( m_vContinuousTwain.size() > 0)
			vect = m_vContinuousTwain[0].vect;

	}
	return vect;
}
//��ը��
vector<enCard> logic::appearBomb(stResult result,bool team,bool genpai)			
{
	vector<enCard> Vect;
	for( int i = 0; i < m_vBomb.size(); ++i )
	{
		if( m_vBomb[i].result.value > result.value )
		{
			Vect = m_vBomb[i].vect;
			return Vect;
		}
	}
	return Vect;
}
//����ը
vector<enCard> logic::appearWangBomb(stResult result,bool team,bool genpai)		
{
	vector<enCard> Vect;
	if( m_vBomb.size() > 0 )
		return m_vBomb[0].vect;

	return Vect;
}
//��˳��
vector<enCard> logic::appearStraight(stResult result,bool team,bool genpai)			
{
	vector<enCard> vect;
	if( genpai )
	{
		//���һ��ƫ�Ҵ�������ǳ��˵��Ƽ�������������ͣ���ѡ��PASS�� 
		if( team  && m_iCountHandCard > 2 )
			return vect;

		/*
		���Ƶĸ���ԭ���������Ӧ�����ͣ����֮���������ͬ������˫˳���������ͬ��������˳��
		�����ͬ���������ƣ������ͬ������˫˳�������ͬ��������˳������ը֮������PASS�� 
		*/

		//����ͬ������˳��
		if( (vect = unseamCard(result,result.conut,result.conut,true,false,1,1,m_vStraight)).size() > 0 )
			return vect;
		//����ͬ������˫˳
		if( (vect = unseamCard(result,result.conut,result.conut * 2,true,false,2,2,m_vContinuousTwain)).size() > 0 )
			return vect;
		//����ͬ��������˳
		if( (vect = unseamCard(result,result.conut,result.conut * 3,true,false,3,3,m_vAircraft)).size() > 0 )
			return vect;
		//����ͬ������˳��
		if( (vect = unseamCard(result,result.conut,result.conut,false,false,1,1,m_vStraight)).size() > 0 )
			return vect;
		//����ͬ������˫˳
		if( (vect = unseamCard(result,result.conut,result.conut * 2,false,false,2,2,m_vContinuousTwain)).size() > 0 )
			return vect;
		//����ͬ��������˳
		if( (vect = unseamCard(result,result.conut,result.conut * 3,false,false,3,3,m_vAircraft)).size() > 0 )
			return vect;
	}
	else
	{
		//���Ƶĳ���ԭ��ֱ�ӳ���
		if( m_vStraight.size() > 0 )
			vect = m_vStraight[0].vect;
		return vect;
	}

	return vect;
}
//���ɻ�
vector<enCard> logic::appearAircraft(stResult result,bool team,bool genpai)			
{
	vector<enCard> vect;
	if( genpai )
	{
		//���һ��ƫ�Ҵ�������ǳ��˵��Ƽ�������������ͣ���ѡ��PASS�� 
		if( team  && m_iCountHandCard > 2 )
			return vect;

		/*
		��˳����˳���Ƶĸ���ԭ���������Ӧ�����ͣ����֮��������Խ������˳���С����˳��֮��
		����ը֮������PASS��ע�⣬ֻ�����������ڳ����Ժ�ʣһ����ʱ����ֱ�ӳ��������£������������2�� 
		*/
		//����ͬ��������˳
		if( (vect = unseamCard(result,result.conut,result.conut,true,false,3,3,m_vAircraft)).size() <= 0 )
		{
			//����ͬ��������˳
			if( (vect = unseamCard(result,result.conut,result.conut,false,false,3,3,m_vAircraft)).size() <= 0 )
				return vect;
		}

		//���
		vector<enCard> chibangVect;
		for( int i = 0; i < m_vSingle.size(); ++i )
		{
			vector<enCard> temp;
			temp = unseamCard(stResult(TYPE_NULL,-1,0),1,1,false,false,1,1,m_vSingle);
			chibangVect.push_back(temp[0]);

			//����
			if( chibangVect.size() >= result.conut/3 )
			{
				for( int j = 0; j < chibangVect.size(); ++j )
					vect.push_back(chibangVect[j]);
				return vect;
			}
		}

		//��6�����ϵ�˳��
		for( int i = 0; i < m_vStraight.size(); ++i )
		{
			if( m_vStraight[i].vect.size() >= 6 )
			{
				for( int j = 0; j < m_vStraight[i].vect.size() - 5; ++j )
				{
					chibangVect.push_back(m_vStraight[i].vect[j]);

					//����
					if( chibangVect.size() >= result.conut/3 )
					{
						for( int j = 0; j < chibangVect.size(); ++j )
							vect.push_back(chibangVect[j]);
						return vect;
					}
				}
			}
		}

		//�����
		for( int i = 0; i < m_vTwain.size(); ++i )
		{
			chibangVect.push_back(m_vStraight[i].vect[0]);

			//����
			if( chibangVect.size() >= result.conut/3 )
			{
				for( int j = 0; j < chibangVect.size(); ++j )
					vect.push_back(chibangVect[j]);
				return vect;
			}
		}
	}
	else
	{
		/*
		��˳�ĳ���ԭ����Ϊ��˳���ƿ��Դ����ţ�����ࣩ���ƻ�����������ࣩ���ӣ������������һ����
		��Ҫ����Ƿ��е��ƻ���ӡ�������㹻��ĵ��ƻ���ӣ� �������������е��ƣ���û���㹻��ĵ��ƣ�
		�����Ƿ���6�����ϵ����ƣ�����н����Ƶ���С�����������ƴ���������ж��ӣ���û���㹻��Ķ��ӣ�
		��� ���Ƿ���4�����ϵ�˫˳������н�˫˳����С���ӵ������Ӵ���
		*/

		if( m_vAircraft.size() > 0)
			vect = m_vAircraft[0].vect;
		else
			return vect;

		int count = m_vAircraft[0].result.conut;

		//�����
		vector<enCard> chibangVect;
		stResult rs = stResult(TYPE_NULL,-1,0);
		for( int i = 0; i < m_vSingle.size(); ++i )
		{
			vector<enCard> temp;
			temp = unseamCard(rs,1,1,false,false,1,1,m_vSingle);
			chibangVect.push_back(temp[0]);
			rs.value = temp[0] / 4; 

			//����
			if( chibangVect.size() >= count/3 )
			{
				for( int j = 0; j < chibangVect.size(); ++j )
					vect.push_back(chibangVect[j]);
				return vect;
			}
		}

		//��6�����ϵ�˳��
		for( int i = 0; i < m_vStraight.size(); ++i )
		{
			if( m_vStraight[i].vect.size() >= 6 )
			{
				for( int j = 0; j < m_vStraight[i].vect.size() - 5; ++j )
				{
					chibangVect.push_back(m_vStraight[i].vect[j]);

					//����
					if( chibangVect.size() >= count/3 )
					{
						for( int j = 0; j < chibangVect.size(); ++j )
							vect.push_back(chibangVect[j]);
						return vect;
					}
				}
			}
		}

		//�����
		for( int i = 0; i < m_vTwain.size(); ++i )
		{
			chibangVect.push_back(m_vTwain[i].vect[0]);

			//����
			if( chibangVect.size() >= count/3 )
			{
				for( int j = 0; j < chibangVect.size(); ++j )
					vect.push_back(chibangVect[j]);
				return vect;
			}
		}

		//��3����
		if( vect.size() >= 3 )
		{
			vector<enCard> temp;
			temp.push_back(vect[0]);
			temp.push_back(vect[1]);
			temp.push_back(vect[2]);
			return temp;
		}

	}
	return vect;
}
//��4��2
vector<enCard> logic::appearFourBandTwo(stResult result,bool team,bool genpai)
{
	vector<enCard> Vect;
	return Vect;
}
//�ҳ���ȵ��� 
bool logic::FindCompareCard(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect,enType type)
{
	//��������
	sort_quick(CardVector,0,CardVector.size() - 1);

	int count = (int)type;
	if( type == TYPE_WANGZHA )
		count = 2;

	for( int i = 0; i < (int)CardVector.size(); )
	{
		vector<enCard> temp;//size ���ڵ���4 ����ը�� ����3 ���� 3���� ����2 �������� ������ը
		if( type != TYPE_WANGZHA)
			temp.push_back(CardVector[i]);

		for( int j = i + 1; j < (int)CardVector.size(); ++j )
		{
			if( temp.size() == count )
				break;
			if( type == TYPE_WANGZHA)
			{
				if( this->compareCard(CardVector[j],Little_King) )
					temp.push_back(CardVector[j]);
			}
			else
			{
				if( this->compareCard(CardVector[j],CardVector[i]) && !this->compareCard(CardVector[j],Little_King) )
					temp.push_back(CardVector[j]);
			}
		}
		//��ͬ����
		if( temp.size() == count )
		{
			//�������͵���Ϣ
			stSaveCardData tempCardData;
			tempCardData.result.conut = count;
			tempCardData.result.type = type;
			tempCardData.result.value = temp[0] / 4;

			//��ӵ�tempCardData
			for(int k = 0; k < (int)temp.size(); ++k )
				tempCardData.vect.push_back(temp[k]);

			//��ӵ�saveCardVect
			saveCardVect.push_back(tempCardData);

			//��CardVector��ɾ��
			for( int j = 0; j < (int)CardVector.size(); )
			{
				bool Flags = true;
				for(int k = 0; k < (int)temp.size();  )
				{
					if( this->compareCard(CardVector[j],temp[k]) )
					{
						Flags = false;
						temp.erase(temp.begin() + k);
						CardVector.erase(CardVector.begin() + j);
						break;
					}
					else
						++k;
				}

				if(Flags)
					j++;
			}
		}
		else
			i += 1;
	}

	return saveCardVect.size() > 0;
}
//���ҵ���
bool logic::FindSingle(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect)
{
	for( int i = 0; i < (int)CardVector.size();)
	{
		stSaveCardData temp;
		temp.result.conut = 1;
		temp.result.type = TYPE_DANPAI;
		temp.result.value = CardVector[i] / 4;
		temp.vect.push_back(CardVector[i]);
		if( this->compareCard(CardVector[i],Little_King))
			temp.result.value = CardVector[i];

		saveCardVect.push_back(temp);
		CardVector.erase(CardVector.begin() + i);
	}

	return saveCardVect.size() > 0;
}
//��������
bool logic::FindContinuousTwain(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect)
{
	//���Ҷ���
	FindTwain(CardVector,m_vTwain);

	for( int i = 0; i < (int)m_vTwain.size(); )
	{
		stSaveCardData temp;
		vector<stSaveCardData> tempVect;
		temp.result = m_vTwain[i].result;
		temp.vect	= m_vTwain[i].vect;
		for( int j = 0; j < (int)m_vTwain.size(); )
		{
			bool Flags = true;
			if( this->compareCard(m_vTwain[j].vect[0],Spandes_2) ) //������2
				Flags = false;
			if( this->compareCard(m_vTwain[j].vect[0],Little_King)) //��������
				Flags = false;

			int NowEndIndex = temp.vect.size() - 1;
			int NextEndIndex = m_vTwain[j].vect.size() - 1;

			if( m_vTwain[j].vect[NextEndIndex]/4 - temp.vect[NowEndIndex]/4 == 1 && Flags )//������
			{
				temp.result.conut += 2;
				temp.result.type = TYPE_LIANDUI;
				temp.vect.push_back(m_vTwain[j].vect[0]);
				temp.vect.push_back(m_vTwain[j].vect[1]);
				tempVect.push_back(m_vTwain[j]);
				m_vTwain.erase(m_vTwain.begin() + j);
			}
			else
				j += 1;
		}

		//������
		//��m_vTwain��ɾ��
		if( temp.vect.size() >= 6 )
		{
			m_vTwain.erase(m_vTwain.begin() + i);
			saveCardVect.push_back(temp);
		}
		else//������� ��ɾ���Ļ���ȥ
		{
			for( vector<stSaveCardData>::iterator it = tempVect.begin(); it != tempVect.end(); ++it )
				m_vTwain.push_back(*it);

			i += 1;
		}
	}

	return saveCardVect.size() > 0;
}
//����˳��
bool logic::FindStraight(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect)
{
	//��������
	sort_quick(CardVector,0,CardVector.size() - 1);

	//�ҳ�5��˳��
	for( int i = 0; i < (int)CardVector.size(); )
	{
		vector<enCard> tempCardVect;//size����5������˳��
		tempCardVect.push_back(CardVector[i]);//��ʱ�洢��
		bool spill = false;
		for( int j = 0; tempCardVect.size() != 5 ; ++j )
		{
			int NowIndex = 0;
			for( int k = 0; k < (int)CardVector.size(); ++k )
			{
				//�ҳ�����CardVector����±�
				int index = tempCardVect.size() - 1;
				if( CardVector[k] == tempCardVect[index] )
				{
					NowIndex = k;
					break;
				}
			}

			//��һ���жϵ��±�
			int NextIndex = i + j + 1;

			//�Ƿ�Խ��
			if( NextIndex >= (int)CardVector.size() )
			{
				spill = true;
				break;
			}

			//����������2
			if( compareCard(CardVector[NextIndex],Little_King) || compareCard(CardVector[NextIndex],Spandes_2))
				break;

			//�ظ���
			if( CardVector[NextIndex]/4 - CardVector[NowIndex]/4 == 0 )
				continue;

			//��������
			if( CardVector[NextIndex]/4 - CardVector[NowIndex]/4 == 1 )
				tempCardVect.push_back(CardVector[NextIndex]);
		}

		//Խ��
		if( spill )
			break;

		if( tempCardVect.size() == 5 )//������˳�� 
		{
			//�Ѻ���5���Ƽ��뵽temp��
			stSaveCardData temp;
			for(int k = 0; k < tempCardVect.size(); ++k )
				temp.vect.push_back(tempCardVect[k]);

			//��CardVector��ɾ���ҳ�������
			for( int k = 0; k < tempCardVect.size(); ++k  )
			{
				for( vector<enCard>::iterator it = CardVector.begin(); it != CardVector.end(); )
				{
					if( *it == tempCardVect[k] )
					{
						it = CardVector.erase(it);
						break;
					}
					else
						++it;
				}
			}

			//����˳�ӵ���Ϣ
			temp.result.value = temp.vect[0] / 4;
			temp.result.type = TYPE_SHUNZI;
			temp.result.conut = 5;
			saveCardVect.push_back(temp);
		}
		else
			i += 1;
	}

	//û�ҵ�
	if( saveCardVect.size() == 0 )
		return false;

	//��չ��˳����ʣ��������ң����ĳ������
	//����֪��˳������ɸ�������ƣ��ͺϲ���һ��
	for( int i = 0; i < (int)CardVector.size(); )
	{
		bool Flags = false;
		for( int j = 0; j < saveCardVect.size(); ++j )
		{
			//ĩβ�±�
			int endIndex = saveCardVect[j].vect.size() - 1;

			//�������
			if( CardVector[i]/4 - saveCardVect[j].vect[endIndex]/4 != 1 )
				continue;

			//���ܸ�����2���
			if( compareCard(CardVector[i],Little_King) || compareCard(CardVector[i],Spandes_2) )
				continue;

			//�����
			Flags = true;
			saveCardVect[j].vect.push_back(CardVector[i]);
			saveCardVect[j].result.conut += 1;
			CardVector.erase(CardVector.begin() + i);
			break;
		}

		i = ( Flags ) ? i : i + 1;
	}

	//�ϲ���˳
	//���ĳ���������޷����ӳɸ�������ƣ�����ϲ���һ��
	for( int i = 0; i < saveCardVect.size(); i++ )
	{
		//ĩβ�±�
		int endIndex = saveCardVect[i].vect.size() - 1;

		for( int k = 0; k < saveCardVect.size(); ++k )
		{
			if( i == k )
				continue;

			//���������ģ��������
			if( saveCardVect[k].vect[0]/4 - saveCardVect[i].vect[endIndex]/4 != 1 )
				continue;

			//�ϲ�
			for( int j = 0; j < saveCardVect[k].vect.size(); ++j )
				saveCardVect[i].vect.push_back(saveCardVect[k].vect[j]);

			//ɾ��
			saveCardVect.erase(saveCardVect.begin() + k);
			break;
		}
	}

	return true;
}
//���ҷɻ�
bool logic::FindAircraft(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect)
{
	if( m_vThreeBandZero.size() == 0 )
		FindThreeBandZero(CardVector,m_vThreeBandZero);

	for( int i = 0; i < (int)m_vThreeBandZero.size(); )
	{
		//Ĭ�ϵ�ǰ��3������ɷɻ�
		//��ӵ�Temp��
		stSaveCardData temp;
		temp.result = m_vThreeBandZero[i].result;
		temp.vect   = m_vThreeBandZero[i].vect;
		for( int j = 0; j < (int)m_vThreeBandZero.size(); )
		{
			bool Flags = true;
			if( this->compareCard(m_vThreeBandZero[j].vect[0],Spandes_2) )//������2
				Flags = false;
			if( this->compareCard(m_vThreeBandZero[j].vect[0],Little_King))//��������
				Flags = false;

			int NowEndIndex = temp.vect.size() - 1;//����±�
			int NextEndIndex = m_vThreeBandZero[j].vect.size() - 1;//����±�
			if( m_vThreeBandZero[j].vect[NextEndIndex]/4 - temp.vect[NowEndIndex]/4 == 1 && Flags )
			{
				temp.result.conut += 3;
				temp.result.type = TYPE_FEIJI;
				temp.vect.push_back(m_vThreeBandZero[j].vect[0]);
				temp.vect.push_back(m_vThreeBandZero[j].vect[1]);
				temp.vect.push_back(m_vThreeBandZero[j].vect[2]);
				m_vThreeBandZero.erase(m_vThreeBandZero.begin() + j);
			}
			else
				j += 1;
		}

		//�ҵ��˷ɻ� 
		if( temp.vect.size() != 3 )
		{
			m_vThreeBandZero.erase(m_vThreeBandZero.begin() + i);
			saveCardVect.push_back(temp);
		}
		else 
			i += 1;
	}

	return saveCardVect.size() > 0;
}
//����4��2
bool logic::FindFourBandTwo(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect)
{
	return false;
}
//���Ҷ���
bool logic::FindTwain(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect)
{
	return FindCompareCard(CardVector,saveCardVect,TYPE_DUIZI);
}
//����3��1
bool logic::FindThreeBandOne(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect)
{
	return false;
}
//����3��2
bool logic::FindThreeBandTwo(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect)
{
	return false;
}
//����3����
bool logic::FindThreeBandZero(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect)
{
	return FindCompareCard(CardVector,saveCardVect,TYPE_3BUDAI);
}
//����ը��
bool logic::FindBomb(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect)
{ 
	return FindCompareCard(CardVector,saveCardVect,TYPE_ZHADAN);
}
//������ը
bool logic::FindWangBomb(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect)
{
	return FindCompareCard(CardVector,saveCardVect,TYPE_WANGZHA);
}
//��������
vector<enCard> logic::ChuRenYiPai(bool myLandlord,enType type,stPlayerData last,stPlayerData next,stResult result)
{
	vector<enCard> vect;

	int _3TiaoConut = m_vThreeBandZero.size();
	for( int i = 0; i < m_vAircraft.size(); ++i )
		_3TiaoConut += m_vAircraft[i].result.conut / 3;

	//����Լ��ǵ���
	if( myLandlord )
	{
		//�ڳ���ʱ�����ƫ����һ����ֻʣһ����ʱ�������������ƣ��������ɴ�С���� 
		if( last.cardCount == 1 || next.cardCount == 1 )
		{
			//������
			if(( vect = appearTwain(result,false,false)).size() > 0 && _3TiaoConut < m_vTwain.size() ) 
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//���ɻ�
			if(( vect = appearAircraft(result,false,false)).size() > 0)
				return vect;
			//��3��2
			if(( vect = appearThreeBandTwo(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//��3��1
			if(( vect = appearThreeBandOne(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//��3����
			if(( vect = appearThreeBandZero(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//��˳��
			if(( vect = appearStraight(result,false,false)).size() > 0)
				return vect;
			//������
			if(( vect = appearContinuousTwain(result,false,false)).size() > 0)
				return vect;
			//������ �ɴ�С��
			if( m_vSingle.size() > 0 )
			{
				int maxValue = m_vSingle[0].result.value;
				int index = 0;
				for( int i = 0 ; i < m_vSingle.size(); ++i )
				{
					if( m_vSingle[i].result.value > maxValue )
					{
						maxValue = m_vSingle[i].result.value;
						index = i;
					}
				}

				return m_vSingle[index].vect;
			}
			//��ը��
			if(( vect = appearBomb(result,false,false)).size() > 0)
				return vect;
			//����ը
			if(( vect = appearWangBomb(result,false,false)).size() > 0)
				return vect;
		}
		else if( last.cardCount == 2 || next.cardCount == 2 )//����ʣ��2����
		{
			//���ɻ�
			if(( vect = appearAircraft(result,false,false)).size() > 0)
				return vect;
			//��3��2
			if(( vect = appearThreeBandTwo(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//��3��1
			if(( vect = appearThreeBandOne(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//��3����
			if(( vect = appearThreeBandZero(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//��˳��
			if(( vect = appearStraight(result,false,false)).size() > 0)
				return vect;
			//������
			if(( vect = appearContinuousTwain(result,false,false)).size() > 0)
				return vect;
			//������
			if(( vect = appearSingle(result,false,false)).size() > 0  )
				return vect;
			//������
			//�Ӵ�С��
			if( m_vTwain.size() > 0 )
			{
				int maxValue = m_vTwain[0].result.value;
				int index = 0;
				for( int i = 0 ; i < m_vTwain.size(); ++i )
				{
					if( m_vTwain[i].result.value > maxValue )
					{
						maxValue = m_vTwain[i].result.value;
						index = i;
					}
				}

				return m_vTwain[index].vect;
			}
			//��ը��
			if(( vect = appearBomb(result,false,false)).size() > 0)
				return vect;
			//����ը
			if(( vect = appearWangBomb(result,false,false)).size() > 0)
				return vect;

		}
		else//��������
		{
			//�㵥�Ƶ�������2��������
			int singleSize = m_vSingle.size();
			for( int i = 0; i < m_vSingle.size(); ++i )
			{
				if( this->compareCard(m_vSingle[i].vect[0],Little_King) || this->compareCard(m_vSingle[i].vect[0],Spandes_2) ) 
					singleSize -= 1;
			}

			singleSize -= _3TiaoConut;

			if( singleSize > 2 )
			{
				//������
				if(( vect = appearSingle(result,false,false)).size() > 0 && !this->compareCard(vect[0],Spandes_2 ) 
					&& !this->compareCard(vect[0],Little_King)  )
					return vect;
			}
			//������
			if(( vect = appearTwain(result,false,false)).size() > 0 && _3TiaoConut < m_vTwain.size() ) 
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//���ɻ�
			if(( vect = appearAircraft(result,false,false)).size() > 0)
				return vect;
			//��3��2
			if(( vect = appearThreeBandTwo(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//��3��1
			if(( vect = appearThreeBandOne(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//��3����
			if(( vect = appearThreeBandZero(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//��˳��
			if(( vect = appearStraight(result,false,false)).size() > 0)
				return vect;
			//������
			if(( vect = appearContinuousTwain(result,false,false)).size() > 0)
				return vect;
			if( singleSize <= 2 )
			{
				//������
				if(( vect = appearSingle(result,false,false)).size() > 0 )
					return vect;
			}
			//��ը��
			if(( vect = appearBomb(result,false,false)).size() > 0)
				return vect;
			//����ը
			if(( vect = appearWangBomb(result,false,false)).size() > 0)
				return vect;
		}
	}
	else // ���ǵ���
	{

		// 1) �ڳ���ʱ������¼���ׯ�ң���ׯ��ֻʣһ����ʱ�������������ƣ��������ɴ�С����
		if( next.landlord && next.cardCount == 1 || last.landlord && last.cardCount == 1 )
		{
			//������
			if(( vect = appearTwain(result,false,false)).size() > 0 && _3TiaoConut < m_vTwain.size() ) 
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//���ɻ�
			if(( vect = appearAircraft(result,false,false)).size() > 0)
				return vect;
			//��3��2
			if(( vect = appearThreeBandTwo(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//��3��1
			if(( vect = appearThreeBandOne(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//��3����
			if(( vect = appearThreeBandZero(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//��˳��
			if(( vect = appearStraight(result,false,false)).size() > 0)
				return vect;
			//������
			if(( vect = appearContinuousTwain(result,false,false)).size() > 0)
				return vect;
			//������ �ɴ�С��
			if( m_vSingle.size() > 0 )
			{
				int maxValue = m_vSingle[0].result.value;
				int index = 0;
				for( int i = 0 ; i < m_vSingle.size(); ++i )
				{
					if( m_vSingle[i].result.value > maxValue )
					{
						maxValue = m_vSingle[i].result.value;
						index = i;
					}
				}

				return m_vSingle[index].vect;
			}
			//��ը��
			if(( vect = appearBomb(result,false,false)).size() > 0)
				return vect;
			//����ը
			if(( vect = appearWangBomb(result,false,false)).size() > 0)
				return vect;
		}

		if( next.landlord && next.cardCount == 2 || last.landlord && last.cardCount == 2 )
		{
			//���ɻ�
			if(( vect = appearAircraft(result,false,false)).size() > 0)
				return vect;
			//��3��2
			if(( vect = appearThreeBandTwo(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//��3��1
			if(( vect = appearThreeBandOne(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//��3����
			if(( vect = appearThreeBandZero(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//��˳��
			if(( vect = appearStraight(result,false,false)).size() > 0)
				return vect;
			//������
			if(( vect = appearContinuousTwain(result,false,false)).size() > 0)
				return vect;
			//������
			if(( vect = appearSingle(result,false,false)).size() > 0  )
				return vect;
			//������
			//�Ӵ�С��
			if( m_vTwain.size() > 0 )
			{
				int maxValue = m_vTwain[0].result.value;
				int index = 0;
				for( int i = 0 ; i < m_vTwain.size(); ++i )
				{
					if( m_vTwain[i].result.value > maxValue )
					{
						maxValue = m_vTwain[i].result.value;
						index = i;
					}
				}

				return m_vTwain[index].vect;
			}
			//��ը��
			if(( vect = appearBomb(result,false,false)).size() > 0)
				return vect;
			//����ը
			if(( vect = appearWangBomb(result,false,false)).size() > 0)
				return vect;
		}


		// 2) ��������¼ҵ�ƫ��ֻʣһ����ʱ���ڳ���ʱ��������С���ơ����ƻ�����һ���ԭ�� 
		if( next.landlord == false && next.cardCount == 1 ||  last.landlord == false && last.cardCount == 1 )
		{
			if( m_TempCard.size() <= 0 )
				return vect;

			this->sort_quick(m_TempCard,0,m_TempCard.size() - 1 );
			vect.push_back(m_TempCard[0]);
			return vect;
		}
		if( next.landlord == false && next.cardCount == 2 ||  last.landlord == false && last.cardCount == 2 )
		{
			if( m_TempCard.size() <= 0 )
				return vect;

			vector<stSaveCardData> temp;
			FindTwain(m_TempCard,temp);

			if( temp.size() > 0 )
			{
				int maxValue = temp[0].result.value;
				int index = 0;
				for( int i = 0 ; i < temp.size(); ++i )
				{
					if( temp[i].result.value > maxValue )
					{
						maxValue = temp[i].result.value;
						index = i;
					}
				}

				return temp[index].vect;
			}
		}

		//�㵥�Ƶ�������2��������
		int singleSize = m_vSingle.size();
		for( int i = 0; i < m_vSingle.size(); ++i )
		{
			if( this->compareCard(m_vSingle[i].vect[0],Little_King) || this->compareCard(m_vSingle[i].vect[0],Spandes_2) ) 
				singleSize -= 1;
		}

		singleSize -= _3TiaoConut;

		if( singleSize > 2 )
		{
			//������
			if(( vect = appearSingle(result,false,false)).size() > 0 && !this->compareCard(vect[0],Spandes_2) 
				&& !this->compareCard(vect[0],Little_King)  )
				return vect;
		}
		//������
		if(( vect = appearTwain(result,false,false)).size() > 0 && _3TiaoConut < m_vTwain.size() ) 
		{
			if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
				vect.clear();
			else
				return vect;
		}
		//���ɻ�
		if(( vect = appearAircraft(result,false,false)).size() > 0)
			return vect;
		//��3��2
		if(( vect = appearThreeBandTwo(result,false,false)).size() > 0 )
		{
			if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
				vect.clear();
			else
				return vect;
		}
		//��3��1
		if(( vect = appearThreeBandOne(result,false,false)).size() > 0 )
		{
			if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
				vect.clear();
			else
				return vect;
		}
		//��3����
		if(( vect = appearThreeBandZero(result,false,false)).size() > 0 )
		{
			if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
				vect.clear();
			else
				return vect;
		}
		//��˳��
		if(( vect = appearStraight(result,false,false)).size() > 0)
			return vect;
		//������
		if(( vect = appearContinuousTwain(result,false,false)).size() > 0)
			return vect;
		if( singleSize <= 2 )
		{
			//������
			if(( vect = appearSingle(result,false,false)).size() > 0)
				return vect;
		}
		//��ը��
		if(( vect = appearBomb(result,false,false)).size() > 0)
			return vect;
		//����ը
		if(( vect = appearWangBomb(result,false,false)).size() > 0)
			return vect;
	}

	return vect;
}
//Ai
logic::enAiResult logic::Ai(enState state,//����ʲô״̬
							vector<enCard> cardVect,//�����е���
							vector<enCard>& ReturnVect,//Ҫ���س�ȥ����
							enType LandlordNoCard,//������Ҫ������
							stPlayerData lastPlayer,//��һ����ҵ���Ϣ
							stPlayerData nowPlayer,//��ǰ��ҵ���Ϣ
							stPlayerData nextPlayer,//��һ����ҵ���Ϣ
							int dir,//������ҵķ���
							stResult result)//��һ����ҳ���ʲô�ƣ�Ĭ��û����
{

	bool myLandlord			= (!lastPlayer.landlord && !nextPlayer.landlord);//�Լ��ǲ��ǵ���
	vector<enCard> tempVect = cardVect;
	m_TempCard				= cardVect;
	m_AppCardPlayer			= nowPlayer ;

	if( lastPlayer.dir == dir )
		m_AppCardPlayer = lastPlayer;
	if( nextPlayer.dir == dir )
		m_AppCardPlayer = nextPlayer;
	if( nowPlayer.dir == dir )
		m_AppCardPlayer = nextPlayer;

	switch( state )
	{
	case en_Wait://�ȴ�
		{
			return en_AiWait;
		}break;
	case en_JiaoDiZhu://�е���
		{
			bool flags = ifCallCard(cardVect);
			return flags ? en_CryLoadlord : en_NoCryLoadlord;
		}break;
	case en_QiangDiZhu://������
		{
			bool flags = ifCallCard(cardVect);
			return flags ? en_RobLaodlord : en_NoRobLaodlord;
		}break;
	case en_JiaBei://�ӱ�
		{
			bool flags = ifCallCard(cardVect);
			return flags ? en_Double : en_NoDouble;
		}break;
	case en_ChuPai://��������
		{
			//����
			unseamCard(cardVect);

			//������Ҫ�����ֳ�����
			m_iCountHandCard = this->countHandCard();

			//����
			if((ReturnVect = ChuRenYiPai(myLandlord,LandlordNoCard,lastPlayer,nextPlayer,result)).size() == 0 )
				ReturnVect.push_back(tempVect[0]); 

			return en_AppearCard;

		}break;
	case en_GenPai://�����ָ����������
		{
			ReturnVect = hint(cardVect,result,LandlordNoCard,lastPlayer,nowPlayer,nextPlayer,dir,false);//��ʾ
			if( ReturnVect.size() != 0 )
				return en_AppearCard;
			else
				return en_NoDenmand;
		}break;
	}

	return en_AiWait;
}
//��ʾ ���ص�vect��size���Ϊ0 �ͱ�ʾҪ����
vector<enCard> logic::hint(vector<enCard> CardVector,stResult result,enType type,
						   stPlayerData lastPlayer,stPlayerData nowPlayer,stPlayerData nextPlayer,int dir,bool flag)//�Ƿ��������ʾ)
{
	vector<enCard> hintVect;			//���س�ȥ�Ƶı�
	vector<enCard> tempVect = CardVector;//�ҳ�3��һ

	//����
	unseamCard(CardVector);

	//������Ҫ�����ֳ�����
	m_iCountHandCard = this->countHandCard();

	bool myLandlord,team;
	if( dir == -1 )//��û����
	{
		myLandlord = true;
		team = false;
	}
	else
	{
		myLandlord = nowPlayer.landlord;//�Լ��ǲ��ǵ���
		team = ( !m_AppCardPlayer.landlord && !myLandlord );//�ǲ��Ƕ���
	}

	switch( result.type )
	{
	case TYPE_NULL://����������
		{
			vector<enCard> vect = ChuRenYiPai(myLandlord,type,lastPlayer,nextPlayer,result);
			if( vect.size() != 0 )
				return vect;
			else
			{
				vect.push_back(tempVect[0]);
				return vect;
			}

		}break;
	case TYPE_3DAI1://3��1
		{
			hintVect = appearThreeBandOne(result,team,true);
			if( hintVect.size() == 0 && flag  )
			{
				FindThreeBandZero(tempVect,m_vThreeBandZero);
				FindSingle(tempVect,m_vSingle);
				hintVect = appearThreeBandOne(result,false,true);
			}

			if( hintVect.size() != 0 )
				return hintVect;

		}break;
	case TYPE_3BUDAI://3����
		{
			hintVect = appearThreeBandZero(result,team,true);
			if( hintVect.size() == 0 && flag  )
			{
				FindThreeBandZero(tempVect,m_vThreeBandZero);
				hintVect = appearThreeBandZero(result,false,true);
			}

			if( hintVect.size() != 0 )
				return hintVect;
		}break;
	case TYPE_3DAI2://3��2
		{
			hintVect = appearThreeBandTwo(result,team,true);
			if( hintVect.size() == 0 && flag  )
			{
				FindThreeBandZero(tempVect,m_vThreeBandZero);
				FindTwain(tempVect,m_vTwain);
				hintVect = appearThreeBandTwo(result,false,true);
			}

			if( hintVect.size() != 0 )
				return hintVect;
		}break;
	case TYPE_DUIZI://����
		{	
			hintVect = appearTwain(result,team,true);
			if( hintVect.size() == 0 && flag  )
			{
				FindTwain(tempVect,m_vTwain);
				hintVect = appearTwain(result,false,true);
			}

			if( hintVect.size() != 0)
				return hintVect;
		}break;
	case TYPE_DANPAI://����
		{
			hintVect = appearSingle(result,team,true);
			if( hintVect.size() == 0 && flag  )
			{
				FindSingle(tempVect,m_vSingle);
				hintVect = appearSingle(result,false,true);
			}

			if( hintVect.size() != 0)
				return hintVect;
		}break;
	case TYPE_LIANDUI://����
		{
			hintVect = appearContinuousTwain(result,team,true);
			if( hintVect.size() == 0 && flag  )
			{
				FindContinuousTwain(tempVect,m_vContinuousTwain);
				hintVect = appearContinuousTwain(result,false,true);
			}

			if( hintVect.size() != 0)
				return hintVect;
		}break;
	case TYPE_SHUNZI://˳��
		{
			hintVect = appearStraight(result,team,true);
			if( hintVect.size() == 0 && flag  )
			{
				FindStraight(tempVect,m_vStraight);
				hintVect = appearStraight(result,false,true);
			}

			if( hintVect.size() != 0)
				return hintVect;

		}break;
	case TYPE_FEIJI://�ɻ�
		{
			hintVect = appearAircraft(result,team,true);
			if( hintVect.size() == 0 && flag  )
			{
				FindAircraft(tempVect,m_vAircraft);
				hintVect = appearAircraft(result,false,true);
			}

			if( hintVect.size() != 0)
				return hintVect;

		}break;
	}

	if( m_vBomb.size() > 0 && result.type != TYPE_WANGZHA && !team  )
	{
		for( int i = 0; i < m_vBomb.size(); ++i)
		{
			if(result.type == TYPE_ZHADAN)
			{
				if(  m_vBomb[i].result.value > result.value)
				{
					hintVect = m_vBomb[i].vect;
					return hintVect;
				}
			}
			else
			{
				hintVect = m_vBomb[i].vect;
				return hintVect;
			}
		}
	}
	else if( m_vWangBomb.size() > 0 && !team )
	{
		hintVect = m_vWangBomb[0].vect;
		return hintVect;
	}

	return hintVect;
}

//�Ƿ�е���
//��������д��� ������2���� ������Ʋ��ǵ��ƾͽе���
bool logic::ifCallCard(vector<enCard>& CardVector)
{
	bool big_king = false;//����
	bool lit_king = false;//С��
	int _2 = 0;//2
	int _A = 0;//A
	int _K = 0;//K
	int bomb = 0;//ը��
	int aircraft = 0;//�ɻ�
	int Straight = 0;//˳��
	int sandaiyi = 0;//����һ
	int liandui = 0;//����

	vector<int> continueVect;

	for( int i = 0; i < (int)CardVector.size(); ++i  )
	{
		if( IsIndexInVector(i,continueVect) )
			continue;

		if( CardVector[i] == Big_King )//����
		{
			big_king = true;
			continueVect.push_back(i);
		}
		if( CardVector[i] == Little_King )//С��
		{
			lit_king = true;
			continueVect.push_back(i);
		}
		if( compareCard(CardVector[i],Spandes_2) )//2
		{
			_2 += 1;
			continueVect.push_back(i);
		}
		if( compareCard(CardVector[i],Spandes_A) )
		{
			_A += 1;
			continueVect.push_back(i);
		}
		if( compareCard(CardVector[i],Spandes_K) )
		{
			_K += 1;
			continueVect.push_back(i);
		}
	}

	//�ж��м���ը����3��1
	for( int i = 0; i < (int)CardVector.size(); ++i )
	{
		if( IsIndexInVector(i,continueVect) )
			continue;

		enCard temp =  CardVector[i];
		int compareCount = 1;
		for( int j = 0; j < (int)CardVector.size(); ++j )
		{
			if( j == i || IsIndexInVector(j,continueVect) )
				continue;

			if( compareCard(CardVector[i],temp) )
			{
				compareCount += 1;
				continueVect.push_back(i);
				continueVect.push_back(j);
			}
		}

		if( compareCount == 3 )//3��1
			sandaiyi += 1;
		else if( compareCount == 4 )//ը��
			bomb += 1;
	}

	if( big_king || lit_king && /*bomb >= 1 &&*/ _2 > 1 && _A >= 1 )
		return true;

	return false;
}
//�Ƿ�������
bool logic::ifRobLandlord(vector<enCard>& CardVector)
{
	return true;
}
//�Ƿ�ӱ�
int logic::ifDouble(vector<enCard>& CardVector)
{
	return true;
}
//��ת��Ϊint
int logic::cardTransformInt(enCard card)
{
	return (int)(card) / 4;
}
//�ж����Ƿ����
bool logic::compareCard(enCard card1,enCard card2)
{
	if( card1 == card2 )
		return true;

	int index1 = cardTransformInt(card1);
	int index2 = cardTransformInt(card2);

	return ( index1 == index2 );
}
//�ж�һ��int�Ƿ���һ��������
bool logic::IsIndexInVector(int index,vector<int>& vect)
{
	//�ж��Ƿ��ظ�
	for( vector<int>::iterator it = vect.begin(); it != vect.end(); ++it)
	{
		if( index == *it )
		{
			return true;
		}
	}

	return false;
}
//�ж��Ƿ��ǵ���
stResult logic::judgeSingle( vector<enCard>& CardVector)
{
	if( CardVector[0] == Big_King && CardVector.size() == 1 )
		return stResult(TYPE_DANPAI,(int)CardVector[0],CardVector.size());

	if( CardVector[0] == Little_King && CardVector.size() == 1 )
		return stResult(TYPE_DANPAI,(int)CardVector[0],CardVector.size());

	if( CardVector.size() == 1 )
		return stResult(TYPE_DANPAI,(int)CardVector[0]/4,CardVector.size());
	else
		return stResult(TYPE_NULL,-1,0);
}
//�ж��Ƿ��Ƕ���
stResult logic::judgeTwain(vector<enCard>& CardVector)
{
	if( CardVector.size() != 2 )
		return stResult(TYPE_NULL,-1,0);

	enCard card1 = CardVector[0];
	enCard card2 = CardVector[1];

	if( compareCard(card1,card2) )
		return stResult(TYPE_DUIZI,card1/4,CardVector.size());
	else
		return stResult(TYPE_NULL,-1,0);

}
//�ж��Ƿ�3��1
stResult logic::judgeThreeBandOne(vector<enCard>& CardVector)
{
	if( CardVector.size() != 4 )
		return stResult(TYPE_NULL,-1,0);

	//�ж�һ�����Ƿ������3��
	for( int i = 0; i < (int)CardVector.size(); ++i)
	{
		enCard tempCard = CardVector[i];
		int equalityCount = 1;//��ȵĴ���

		for( int j = 0; j < (int)CardVector.size(); ++j )
		{
			if( i == j )
				continue;

			if( compareCard(tempCard,CardVector[j]) )
				equalityCount++;
		}

		if( equalityCount == 3 )//3��1
		{
			int value = tempCard/4;
			return stResult(TYPE_3DAI1,value,CardVector.size());
		}
	}

	return stResult(TYPE_NULL,-1,0);
}
//�ж��Ƿ�3��2
stResult logic::judgeThreeBandTwo(vector<enCard>& CardVector)
{
	if( CardVector.size() != 5 )
		return stResult(TYPE_NULL,-1,0);

	enCard tempCard;
	//�ж�һ�����Ƿ������3��
	for( int i = 0; i < (int)CardVector.size(); ++i)
	{
		tempCard = CardVector[i];
		int equalityCount = 1;//��ȵĴ���

		for( int j = 0; j < (int)CardVector.size(); ++j )
		{
			if( i == j )
				continue;

			if( compareCard(tempCard,CardVector[j]) )
				equalityCount++;
		}

		if( equalityCount == 3 )//3��1
			break;
	}

	//������¼ʣ�µ���
	enCard card[2];
	int index = 0;

	for( int i = 0; i < (int)CardVector.size(); ++i )
	{
		if( !compareCard(tempCard,CardVector[i]) )
			card[index++] = CardVector[i];
	}

	if( compareCard(card[0],card[1]) )
		return stResult(TYPE_3DAI2,(int)CardVector[0]/4,CardVector.size());

	return stResult(TYPE_NULL,-1,0);

}
//�ж��Ƿ�3����
stResult logic::judgeThreeBandZero(vector<enCard>& CardVector)
{
	if( CardVector.size() != 3 )
		return stResult(TYPE_NULL,-1,0);

	enCard temp = CardVector[0];

	for( int i = 1; i < (int)CardVector.size(); ++i )
	{
		if( !compareCard(temp,CardVector[i]) )
			return stResult(TYPE_NULL,-1,0);
	}

	return stResult(TYPE_3BUDAI,temp/4,CardVector.size());
}
//�ж��Ƿ�����
stResult logic::judgeContinuousTwain(vector<enCard>& CardVector)
{
	if( CardVector.size() < 6 || CardVector.size() % 2 != 0 )
		return stResult(TYPE_NULL,-1,0);

	for( int i = 0; i < (int)CardVector.size(); ++i )
	{
		//�Ƿ���2
		if( CardVector[i] == Spandes_2 || CardVector[i] ==  RedStar_2 )
			return stResult(TYPE_NULL,-1,0);
		if( CardVector[i] == Club_2    || CardVector[i] ==  Hearts_2 )
			return stResult(TYPE_NULL,-1,0);

		//�Ƿ�����
		if( CardVector[i] == Little_King || CardVector[i] == Big_King)
			return stResult(TYPE_NULL,-1,0);
	}

	//����CardVector����һ���enCard����
	//������¼�Ѿ�ȷ���Ƕ��ӵ���
	int size = CardVector.size() / 2;
	vector<enCard> vCard ;
	vector<int> vectorContinue;//�������Ѿ��жϹ��˵��±�

	for( int i = 0; i < (int)CardVector.size(); ++i )
	{
		//�Ƿ��Ѿ��жϹ���
		if( IsIndexInVector(i,vectorContinue) )
			continue;

		enCard tempCard = CardVector[i];
		int equalityCount = 1;//��ȵĴ���

		for( int j = 0; j < (int)CardVector.size();++j )
		{
			//�����ǰ�±����Լ����߱ȽϹ���
			if( i == j || IsIndexInVector(j,vectorContinue))
				continue;

			//�Ƿ���ͬ������
			if( compareCard(tempCard,CardVector[j]) )
			{
				vCard.push_back(tempCard);
				equalityCount += 1;
				vectorContinue.push_back(i);
				vectorContinue.push_back(j);
			}
		}

		//����2����ȣ��ʹ���������
		if( equalityCount != 2 )
		{
			return stResult(TYPE_NULL,-1,0);
		}
	}
	//����
	sort_quick(vCard,0,vCard.size() - 1);

	//ֵ
	int value = 0;
	//�ж��Ƿ��������Ķ���
	for( int i = 0; i < vCard.size()-1; ++i )
	{
		if( (int)vCard[i+1] / 4 - (int)vCard[i] / 4 != 1 )
		{
			return stResult(TYPE_NULL,-1,0);
		}
	}

	value = vCard[0];
	return stResult(TYPE_LIANDUI,value/4,CardVector.size());
}
//�ж��ǲ�����ը
stResult logic::judgeWangBomb(vector<enCard>& CardVector)
{
	if( CardVector.size() != 2 )
		return stResult(TYPE_NULL,-1,0);

	for( int i = 0; i < (int)CardVector.size(); ++i )
	{
		//����ȣ�������4��һ����
		if( !compareCard(Little_King,CardVector[i]) )
			return stResult(TYPE_NULL,-1,0);
	}

	return stResult(TYPE_WANGZHA,VALUE_MAX,0);

}
//�ж��Ƿ���ը��
stResult logic::judgeBomb(vector<enCard>& CardVector)
{
	//��������ը��������
	if( CardVector.size() != 4 && CardVector.size() != 2 )
		return stResult(TYPE_NULL,-1,0);

	//��ը��
	if( CardVector.size() == 2 )
	{
		//������ը
		if( CardVector[0] != Little_King && CardVector[0] != Big_King )
			return stResult(TYPE_NULL,-1,0);

		//������ը
		if( CardVector[1] != Little_King && CardVector[1] != Big_King )
			return stResult(TYPE_NULL,-1,0);
	}

	enCard temp = CardVector[0];
	for( int i = 0; i < (int)CardVector.size(); ++i )
	{
		//����ȣ�������4��һ����
		if( !compareCard(temp,CardVector[i]) )
			return stResult(TYPE_NULL,-1,0);
	}

	return stResult(TYPE_ZHADAN,temp/4,CardVector.size());
}
//�ж��Ƿ�˳��
stResult logic::judgeStraight(vector<enCard>& CardVector)
{
	if( CardVector.size() < 5 )
		return stResult(TYPE_NULL,-1,0);

	for(int i = 0; i < CardVector.size(); ++i )
	{
		if( compareCard(CardVector[i],Little_King) || compareCard(CardVector[i],Spandes_2) )
			return stResult(TYPE_NULL,-1,0);
	}

	//��������
	sort_quick(CardVector,0,CardVector.size() - 1);

	for( int i = 0; i < CardVector.size() - 1; ++i )
	{
		if( CardVector[i+1]/4 - CardVector[i]/4 != 1 )
			return stResult(TYPE_NULL,-1,0);
	}

	return stResult(TYPE_SHUNZI,CardVector[0]/4,CardVector.size());
}
//�ж��Ƿ��Ƿɻ�
stResult logic::judgeAircraft(vector<enCard>& CardVector)
{
	if( CardVector.size() < 8 || CardVector.size() % 2 != 0 )
		return stResult(TYPE_NULL,-1,0);

	//��¼�жϹ����±�
	vector<int> ContinueVect;
	//������¼�Ѿ�����3�ε�enCard
	vector<enCard> tempVect;

	for( int i = 0; i < (int)CardVector.size(); ++i )
	{
		//�Ƿ��Ѿ��жϹ���
		if( IsIndexInVector(i,ContinueVect) )
			continue;

		enCard temp = CardVector[i];//��ʱ�ı���
		int equalityCount = 1;//���ֵĴ���

		for( int j = 0; j < (int)CardVector.size(); ++j )
		{
			//�Ƿ��Ѿ��жϹ���
			if( i == j || IsIndexInVector(j,ContinueVect) )
				continue;

			//���
			if( compareCard(temp,CardVector[j]) )
			{
				equalityCount += 1;
				ContinueVect.push_back(i);
				ContinueVect.push_back(j);
			}
		}

		//���ִ�������3��
		if( equalityCount > 3 )
			return stResult(TYPE_NULL,-1,0);
		else if( equalityCount == 3 )//�պ�3��
			tempVect.push_back(temp);
	}

	//û��2������
	if( tempVect.size() < 2 )
		return stResult(TYPE_NULL,-1,0);

	//�����Ƕ�����
	for( int i = 0; i < (int)tempVect.size(); ++i )
	{
		if( compareCard(tempVect[i],Spandes_2) || compareCard(tempVect[i],Little_King) )
			return stResult(TYPE_NULL,-1,0);
	}

	//����
	sort_quick(tempVect,0,tempVect.size() - 1);

	//�ж��Ƿ�����
	for( int i = 0; i < (int)tempVect.size() - 1 ; ++i )
	{
		if( (int)tempVect[i+1] / 4 - (int)tempVect[i] / 4 != 1 )
			return stResult(TYPE_NULL,-1,0);
	}

	int remain =  CardVector.size() - tempVect.size() * 3;//ʣ�µ�����
	if( remain != tempVect.size() )
		return stResult(TYPE_NULL,-1,0);

	//ֵ
	int value = tempVect[0] / 4;
	return stResult(TYPE_FEIJI,value,tempVect.size() * 3 );
}
//�ж���4��2
stResult logic::judgeFourBandTwo(vector<enCard>& CardVector)
{
	if( CardVector.size() != 6 )
		return stResult(TYPE_NULL,-1,0);

	//��¼�жϹ����±�
	vector<int> ContinueVect;
	int value = 0;//ֵ
	for( int i = 0; i < (int)CardVector.size(); ++i )
	{
		//�Ƿ��Ѿ��жϹ���
		if( IsIndexInVector(i,ContinueVect) )
			continue;

		enCard temp = CardVector[i];//��ʱ�ı���
		int equalityCount = 1;//���ֵĴ���

		for( int j = 0; j < (int)CardVector.size(); ++j )
		{
			//�Ƿ��Ѿ��жϹ���
			if( i == j || IsIndexInVector(j,ContinueVect) )
				continue;

			//���
			if( compareCard(temp,CardVector[j]) )
			{
				equalityCount += 1;
				ContinueVect.push_back(i);
				ContinueVect.push_back(j);
			}
		}

		//���ִ�������3��
		if( equalityCount > 4 )
			return stResult(TYPE_NULL,-1,0);
		else if( equalityCount == 4 )//�պ�3��
			return stResult(TYPE_SIDAI2,temp/4,CardVector.size());
	}

	return stResult(TYPE_NULL,-1,0);
}
//��������
void logic::sort_quick(vector<enCard>& vect,int b,int e)
{
	if( b < e )
	{
		int pos = b;
		for( int i = b + 1; i <= e; ++i )
		{
			if( vect[i] < vect[b] )
			{
				pos += 1;
				if( pos != i )
				{
					enCard t = vect[i];
					vect[i] = vect[pos];
					vect[pos] = t;
				}
			}
		}

		if( pos != b )
		{
			enCard t = vect[b];
			vect[b] = vect[pos];
			vect[pos] = t;
		}

		sort_quick(vect,b,pos - 1);
		sort_quick(vect,pos + 1,e);
	}
}