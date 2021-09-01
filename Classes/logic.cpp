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
//初始化
void logic::Init()
{
	m_vSingle.clear();		    //存放单牌
	m_vTwain.clear();		    //存放对子
	m_vThreeBandOne.clear();	//存放三带一
	m_vThreeBandTwo.clear();	//存放三带二
	m_vThreeBandZero.clear();	//存放三带零
	m_vContinuousTwain.clear(); //存放连对
	m_vBomb.clear();			//存放炸弹
	m_vStraight.clear();		//存放顺子
	m_vAircraft.clear();		//存放飞机
	m_vFourBandTwo.clear();		//存放四带二
	m_vWangBomb.clear();		//存放王炸
}
//计算需要几手牌才能出完
int logic::countHandCard()
{
	int count = 0;
	count += m_vStraight.size();//顺子
	count += m_vContinuousTwain.size();//连对
	count += m_vAircraft.size();//飞机
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
//拆牌
void logic::unseamCard(vector<enCard>& CardVector)
{
	Init();//清空
	FindWangBomb(CardVector,m_vWangBomb);//查找王炸
	FindBomb(CardVector,m_vBomb);//查找炸弹
	FindThreeBandZero(CardVector,m_vThreeBandZero);//查找3不带
	FindAircraft(CardVector,m_vAircraft);//查找飞机
	FindStraight(CardVector,m_vStraight);//查找顺子
	FindContinuousTwain(CardVector,m_vContinuousTwain);//查找连对
	FindTwain(CardVector,m_vTwain);//查找对子
	FindSingle(CardVector,m_vSingle);//查找单牌
}
//拆牌
//当xx牌无法出的时候，可以选择拆vectSaveCard里的牌来出
vector<enCard> logic::unseamCard(stResult result,//牌型
								 int addNum,//需要的添加到Vect里牌的张数
								 int cardCount,//vectSaveCard里的牌必须大于cardCount
								 bool compare,//必须是相同数量
								 bool end,//反向查找
								 int jumpIndex,//下标跳跃
								 int addJumpIndex,//添加的时候跳跃下标
								 vector<stSaveCardData>& vectSaveCard)//牌
{
	vector<enCard> vect;

	for( int i = 0; i < (int)vectSaveCard.size(); ++i )
	{
		if( compare )//必须数量相同
		{
			if( vectSaveCard[i].result.conut != cardCount )
				continue;
		}
		else//必须大于指定的数量
		{
			if( vectSaveCard[i].result.conut < cardCount )
				continue;
		}

		if( end )//反向查找
		{
			int beginIndex = 0;
			int endIndex   = vectSaveCard[i].vect.size() - 1;
			if( vectSaveCard[i].vect[beginIndex]/4 > result.value )
			{
				for( int j = 0; j < addNum; ++j )//要添加的牌张数
					vect.push_back(vectSaveCard[i].vect[beginIndex + j]);
				return vect;
			}

			if( vectSaveCard[i].vect[endIndex]/4 > result.value )
			{
				for( int j = 0; j < addNum; ++j )//要添加的牌张数
					vect.push_back(vectSaveCard[i].vect[endIndex - j]);
				return vect;
			}
		}
		else
		{
			//必须大过对方的牌
			for( int k = 0; k < vectSaveCard[i].vect.size() ; k += jumpIndex )
			{
				bool Flags = true;
				if( this->compareCard(vectSaveCard[i].vect[k],Little_King) )
					Flags = false;

				if( Flags ? vectSaveCard[i].vect[k]/4 > result.value : vectSaveCard[i].vect[k] > result.value )
				{
					for( int j = 0; j < addNum / addJumpIndex ; ++j )//要添加的张数
					{
						for( int q = 0; q < addJumpIndex; ++q )
						{
							//越界判断
							if( vect.size() != addNum && k + (j * addJumpIndex) + q >= vectSaveCard[i].vect.size() )
							{
								vect.clear();
								return vect;
							}

							//是否足够
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
//出单牌
vector<enCard> logic::appearSingle(stResult result,bool team,bool genpai)
{
	vector<enCard> Vect;
	if( genpai)
	{
		//当一个偏家打出的是单牌时，一般情况下能跟就跟。如果手中必须跟2或以上的牌时，选择PASS。 
		if( result.value >= Spandes_A / 4 && team  && m_iCountHandCard > 2 )
			return Vect;
		else if( team  && m_iCountHandCard > 2 )//队友
		{
			//单牌
			if( (Vect = unseamCard(result,1,1,false,false,1,1,m_vSingle)).size() <= 0 )
				return Vect;
			//不能大于K
			if( Vect[0] / 4 >= Spandes_A / 4 )
				Vect.clear();

			return Vect;
		}

		/*
		单牌的跟牌原则：如果手中有单牌，则跟之,否则拆对牌跟之，否则拆6连以上的单顺顶张跟之，
		否则拆三条跟之，否则拆三顺跟之，否则拆5连单顺跟之，否则拆双顺跟之，否则炸之，否则PASS。 
		*/

		//单牌
		if( (Vect = unseamCard(result,1,1,false,false,1,1,m_vSingle)).size() > 0 )
			return Vect;
		//拆对子
		if( (Vect = unseamCard(result,1,1,false,false,1,1,m_vTwain)).size() > 0 )
			return Vect;
		//拆6连以上的顺子
		if( (Vect = unseamCard(result,1,6,false,true,1,1,m_vStraight)).size() > 0 )
			return Vect;
		//拆3条
		if( (Vect = unseamCard(result,1,1,false,false,1,1,m_vThreeBandZero)).size() > 0 )
			return Vect;
		//拆飞机
		if( (Vect = unseamCard(result,1,1,false,false,3,1,m_vAircraft)).size() > 0 )
			return Vect;
		//拆5连以上的顺子
		if( (Vect = unseamCard(result,1,5,false,false,1,1,m_vStraight)).size() > 0 )
			return Vect;
		//拆连对
		if( (Vect = unseamCard(result,1,1,false,true,2,1,m_vContinuousTwain)).size() > 0 )
			return Vect;
	}
	else
	{
		/*
		单牌的出牌原则：因为单牌可以用三条、三顺等带出，所以在出单牌时，
		应该先检测一下三条 + 三顺中三条的数量，如果所有三条数量 <= 对子 + 单牌数量总和 - 2时
		出单牌，否则出三带1等等。
		*/
		int _3TiaoConut = m_vThreeBandZero.size();
		for( int i = 0; i < m_vAircraft.size(); ++i )
			_3TiaoConut += m_vAircraft[i].result.conut / 3;

		//出单牌
		if( _3TiaoConut < m_vTwain.size() + m_vSingle.size() )
		{
			//单牌
			if( (Vect = unseamCard(result,1,1,false,false,1,1,m_vSingle)).size() > 0 )
				return Vect;
		}
		else
		{
			//出3带1
			if(( Vect = appearThreeBandOne(result,false,false)).size() > 0)
				return Vect;
			//出3带2
			if(( Vect = appearThreeBandTwo(result,false,false)).size() > 0)
				return Vect;
		}
	}
	return Vect;
}
//出对子
vector<enCard> logic::appearTwain(stResult result,bool team,bool genpai)		
{
	vector<enCard> Vect;
	if( genpai )
	{
		//当一个偏家打出的是对子时，一般情况下能跟就跟。如果手中必须跟AA或以上的牌时，选择PASS。 
		if( result.value >= Spandes_K / 4 && team  && m_iCountHandCard > 2 )
			return Vect;
		else if( team  && m_iCountHandCard > 2 )//队友
		{
			//拆对子
			if( (Vect = unseamCard(result,2,2,false,false,1,1,m_vTwain)).size() <= 0 )
				return Vect;
			//不能大于K
			if( Vect[0] / 4 >= Spandes_A / 4 )
				Vect.clear();

			return Vect;
		}

		/*
		对牌的根牌原则：如果手中有对子，则跟之，否则拆4连以上的双顺顶张跟之，否则拆三条跟之，
		否则拆双顺跟之，否则拆三顺跟之，否则炸之，否则PASS。 
		*/

		//拆对子
		if( (Vect = unseamCard(result,2,2,false,false,1,1,m_vTwain)).size() > 0 )
			return Vect;
		//拆4连以上的的双顺
		if( (Vect = unseamCard(result,2,8,false,true,2,1,m_vContinuousTwain)).size() > 0 )
			return Vect;
		//拆3条
		if( (Vect = unseamCard(result,2,2,false,false,1,1,m_vThreeBandZero)).size() > 0 )
			return Vect;
		//拆连队
		if( (Vect = unseamCard(result,2,2,false,false,2,1,m_vContinuousTwain)).size() > 0 )
			return Vect;
		//拆飞机
		if( (Vect = unseamCard(result,2,2,false,false,3,1,m_vAircraft)).size() > 0 )
			return Vect;
	}
	else
	{
		/*
		对子的出牌原则：因为对子可以用三条、三顺等带出，所以在出对子时，应该先检测一下三条＋三顺（中三条）的数量，
		如果所有三条数量 <= 对子＋单牌数量总和－2时，出对子，否则出三带2等等。 
		*/
		int _3TiaoConut = m_vThreeBandZero.size();
		for( int i = 0; i < m_vAircraft.size(); ++i )
			_3TiaoConut += m_vAircraft[i].result.conut / 3;

		if( _3TiaoConut <= m_vSingle.size() + m_vTwain.size() )//出对子
		{
			if( m_vTwain.size() > 0 )
				Vect = m_vTwain[0].vect;
		}
		else//出三带2
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
//出3带1
vector<enCard> logic::appearThreeBandOne(stResult result,bool team,bool genpai)		
{
	vector<enCard> Vect;
	vector<enCard> temp;
	if( genpai )
	{
		//如果一个偏家打出的牌是除了单牌及对子以外的牌型，则选择PASS。 
		if( team && m_iCountHandCard > 2 )
			return Vect;

		/*
		三条、三带1、三带2等牌的跟牌原则：如果手中有相同牌型的牌则跟之，否则拆三顺跟之，否则炸之，否则PASS。
		注意,只有在手中牌在出了以后还剩一手牌时，或直接出完的情况下，才允许带王或2。 
		*/

		//添加3条
		Vect = unseamCard(result,3,3,false,false,1,1,m_vThreeBandZero);

		//没有三条
		if( Vect.size() == 0 )
		{
			//拆飞机
			if( (Vect = unseamCard(result,3,3,false,false,3,1,m_vAircraft)).size() == 0 )
				return Vect;
		}

		//不能是2并且还有3张牌以上 不然就出2
		if( this->compareCard(Vect[0],Spandes_2) && m_AppCardPlayer.cardCount > 3 )
		{
			Vect.clear();
			return Vect;
		}

		//添加最小的单牌
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
		//拆6连以上的顺子
		if( (temp = unseamCard(stResult(TYPE_NULL,-1,0),1,6,false,true,1,1,m_vStraight)).size() > 0 )
		{
			Vect.push_back(temp[0]);
			return Vect;
		}
		//拆对子
		if( (temp = unseamCard(stResult(TYPE_NULL,-1,0),1,1,false,false,1,1,m_vTwain)).size() > 0 )
		{
			Vect.push_back(temp[0]);
			return Vect;
		}

		//没有带的 不要
		Vect.clear();
		return Vect;
	}
	else
	{
		/*
		三条的出牌原则：因为三条出牌可以带一张单牌或一个对子，所以在出三条时需要检测是否有单牌，
		如果有，则带一张最小的单牌，如果没有，则再检测是否存在对子，如果有，则跟一个最小的对子，
		如果单牌和对子都没有，则出三条。 
		*/

		//出3条
		if((Vect = unseamCard(stResult(TYPE_NULL,-1,0),3,3,false,false,1,1,m_vThreeBandZero)).size() == 0)
			return Vect;

		//添加最小的单牌
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
		//拆对子
		if( (temp = this->unseamCard(stResult(TYPE_NULL,-1,0),2,2,false,false,1,1,m_vTwain)).size() > 0 )
		{
			Vect.push_back(temp[0]);
			return Vect;
		}

		//出三不带
		return Vect;
	}
	return Vect;
}
//出3带2
vector<enCard> logic::appearThreeBandTwo(stResult result,bool team,bool genpai)	
{
	vector<enCard> Vect;
	vector<enCard> temp;
	if( genpai )
	{
		//如果一个偏家打出的牌是除了单牌及对子以外的牌型，则选择PASS。 
		if( team && m_iCountHandCard > 2 )
			return Vect;

		/*
		三条、三带1、三带2等牌的跟牌原则：如果手中有相同牌型的牌则跟之，否则拆三顺跟之，否则炸之，否则PASS。
		注意,只有在手中牌在出了以后还剩一手牌时，或直接出完的情况下，才允许带王或2。 
		*/

		//添加3条
		Vect = unseamCard(result,3,3,false,false,1,1,m_vThreeBandZero);

		//没有三条
		if( Vect.size() == 0 )
		{
			//拆飞机
			if( (Vect = unseamCard(result,3,3,false,false,3,1,m_vAircraft)).size() == 0 )
				return Vect;
		}
		//不能是2并且还有3张牌以上 不然就出2
		if( this->compareCard(Vect[0],Spandes_2) && m_AppCardPlayer.cardCount > 3 )
		{
			Vect.clear();
			return Vect;
		}
		//添加最小的对子
		if( (temp = this->unseamCard(stResult(TYPE_NULL,-1,0),2,2,false,false,1,1,m_vTwain)).size() > 0 )
		{
			Vect.push_back(temp[0]);
			Vect.push_back(temp[1]);
			return Vect;
		}
		//拆4连以上的连对
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
		三条的出牌原则：因为三条出牌可以带一张单牌或一个对子，所以在出三条时需要检测是否有单牌，
		如果有，则带一张最小的单牌，如果没有，则再检测是否存在对子，如果有，则跟一个最小的对子，
		如果单牌和对子都没有，则出三条。 
		*/

		//出3条
		if((Vect = unseamCard(stResult(TYPE_NULL,-1,0),3,3,false,false,1,1,m_vThreeBandZero)).size() == 0)
			return Vect;

		//添加最小的单牌
		if( (temp = this->unseamCard(stResult(TYPE_NULL,-1,0),1,1,false,false,1,1,m_vSingle)).size() > 0 )
		{
			Vect.push_back(temp[0]);
			return Vect;
		}
		//拆对子
		if( (temp = this->unseamCard(stResult(TYPE_NULL,-1,0),2,2,false,false,1,1,m_vTwain)).size() > 0 )
		{
			Vect.push_back(temp[0]);
			Vect.push_back(temp[1]);
			return Vect;
		}

		//出三不带
		return Vect;
	}

	return Vect;
}
//出3不带
vector<enCard> logic::appearThreeBandZero(stResult result,bool team,bool genpai)	
{
	vector<enCard> Vect;
	vector<enCard> temp;
	if( genpai )
	{
		//如果一个偏家打出的牌是除了单牌及对子以外的牌型，则选择PASS。 
		if( team  && m_iCountHandCard > 2 )
			return Vect;

		/*
		三条、三带1、三带2等牌的跟牌原则：如果手中有相同牌型的牌则跟之，否则拆三顺跟之，否则炸之，否则PASS。
		注意,只有在手中牌在出了以后还剩一手牌时，或直接出完的情况下，才允许带王或2。 
		*/

		//添加3条
		Vect = unseamCard(result,3,3,false,false,1,1,m_vThreeBandZero);

		//没有三条
		if( Vect.size() == 0 )
		{
			//拆飞机
			if( (Vect = unseamCard(result,3,3,false,true,3,1,m_vAircraft)).size() == 0 )
				return Vect;
		}

		//不能是2并且还有3张牌以上 不然就出2
		if( this->compareCard(Vect[0],Spandes_2) && m_AppCardPlayer.cardCount > 3 )
		{
			Vect.clear();
			return Vect;
		}

		//出3不带
		return Vect;
	}
	else
	{
		/*
		三条的出牌原则：因为三条出牌可以带一张单牌或一个对子，所以在出三条时需要检测是否有单牌，
		如果有，则带一张最小的单牌，如果没有，则再检测是否存在对子，如果有，则跟一个最小的对子，
		如果单牌和对子都没有，则出三条。 
		*/

		//出3条
		if((Vect = unseamCard(stResult(TYPE_NULL,-1,0),3,3,false,false,1,1,m_vThreeBandZero)).size() == 0)
			return Vect;

		//添加最小的单牌
		if( (temp = this->unseamCard(stResult(TYPE_NULL,-1,0),1,1,false,false,1,1,m_vSingle)).size() > 0 )
		{
			Vect.push_back(temp[0]);
			return Vect;
		}
		//拆对子
		if( (temp = this->unseamCard(stResult(TYPE_NULL,-1,0),2,2,false,false,1,1,m_vTwain)).size() > 0 )
		{
			Vect.push_back(temp[0]);
			Vect.push_back(temp[1]);
			return Vect;
		}

		//出三不带
		return Vect;
	}

	return Vect;
}
//出连对
vector<enCard> logic::appearContinuousTwain(stResult result,bool team,bool genpai)
{
	vector<enCard> vect;
	if( genpai )
	{
		//如果一个偏家打出的牌是除了单牌及对子以外的牌型，则选择PASS。 
		if( team  && m_iCountHandCard > 2 )
			return vect;

		/*
		双顺的跟牌原则：有相同牌型的牌，则跟之，否则拆不同张数的双顺，否则拆相同张数的三顺，
		否则拆不同张数的三顺,否则炸之，否则PASS。 
		*/

		//找相同数量的双顺
		if((vect = this->unseamCard(result,result.conut,result.conut,true,false,2,2,m_vContinuousTwain)).size() > 0 )
			return vect;
		//拆不同数量的双顺
		if((vect = this->unseamCard(result,result.conut,result.conut,false,false,2,2,m_vContinuousTwain)).size() > 0 )
			return vect;
		//找相同数量的三顺
		if((vect = this->unseamCard(result,result.conut,result.conut,true,false,3,2,m_vAircraft)).size() > 0 )
			return vect;
		//拆不同数量的三顺
		if((vect = this->unseamCard(result,result.conut,result.conut,false,false,3,2,m_vAircraft)).size() > 0 )
			return vect;
	}
	else
	{
		/*
		双顺的出牌原则：直接出
		*/
		if( m_vContinuousTwain.size() > 0)
			vect = m_vContinuousTwain[0].vect;

	}
	return vect;
}
//出炸弹
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
//出王炸
vector<enCard> logic::appearWangBomb(stResult result,bool team,bool genpai)		
{
	vector<enCard> Vect;
	if( m_vBomb.size() > 0 )
		return m_vBomb[0].vect;

	return Vect;
}
//出顺子
vector<enCard> logic::appearStraight(stResult result,bool team,bool genpai)			
{
	vector<enCard> vect;
	if( genpai )
	{
		//如果一个偏家打出的牌是除了单牌及对子以外的牌型，则选择PASS。 
		if( team  && m_iCountHandCard > 2 )
			return vect;

		/*
		连牌的跟牌原则：如果有相应的牌型，则跟之，否则拆相同张数的双顺，否则拆相同张数的三顺，
		否则拆不同张数的连牌，否则拆不同张数的双顺，否则拆不同张数的三顺，否则炸之，否则PASS。 
		*/

		//出相同张数的顺子
		if( (vect = unseamCard(result,result.conut,result.conut,true,false,1,1,m_vStraight)).size() > 0 )
			return vect;
		//出相同张数的双顺
		if( (vect = unseamCard(result,result.conut,result.conut * 2,true,false,2,2,m_vContinuousTwain)).size() > 0 )
			return vect;
		//出相同张数的三顺
		if( (vect = unseamCard(result,result.conut,result.conut * 3,true,false,3,3,m_vAircraft)).size() > 0 )
			return vect;
		//出不同张数的顺子
		if( (vect = unseamCard(result,result.conut,result.conut,false,false,1,1,m_vStraight)).size() > 0 )
			return vect;
		//出不同张数的双顺
		if( (vect = unseamCard(result,result.conut,result.conut * 2,false,false,2,2,m_vContinuousTwain)).size() > 0 )
			return vect;
		//出不同张数的三顺
		if( (vect = unseamCard(result,result.conut,result.conut * 3,false,false,3,3,m_vAircraft)).size() > 0 )
			return vect;
	}
	else
	{
		//连牌的出牌原则：直接出。
		if( m_vStraight.size() > 0 )
			vect = m_vStraight[0].vect;
		return vect;
	}

	return vect;
}
//出飞机
vector<enCard> logic::appearAircraft(stResult result,bool team,bool genpai)			
{
	vector<enCard> vect;
	if( genpai )
	{
		//如果一个偏家打出的牌是除了单牌及对子以外的牌型，则选择PASS。 
		if( team  && m_iCountHandCard > 2 )
			return vect;

		/*
		三顺及三顺带牌的根牌原则：如果有相应的牌型，则跟之，否则可以将大的三顺拆成小的三顺跟之，
		否则炸之，否则PASS。注意，只有在手中牌在出了以后还剩一手牌时，或直接出完的情况下，才允许带王或2。 
		*/
		//出相同张数的三顺
		if( (vect = unseamCard(result,result.conut,result.conut,true,false,3,3,m_vAircraft)).size() <= 0 )
		{
			//出不同张数的三顺
			if( (vect = unseamCard(result,result.conut,result.conut,false,false,3,3,m_vAircraft)).size() <= 0 )
				return vect;
		}

		//翅膀
		vector<enCard> chibangVect;
		for( int i = 0; i < m_vSingle.size(); ++i )
		{
			vector<enCard> temp;
			temp = unseamCard(stResult(TYPE_NULL,-1,0),1,1,false,false,1,1,m_vSingle);
			chibangVect.push_back(temp[0]);

			//够了
			if( chibangVect.size() >= result.conut/3 )
			{
				for( int j = 0; j < chibangVect.size(); ++j )
					vect.push_back(chibangVect[j]);
				return vect;
			}
		}

		//拆6连以上的顺子
		for( int i = 0; i < m_vStraight.size(); ++i )
		{
			if( m_vStraight[i].vect.size() >= 6 )
			{
				for( int j = 0; j < m_vStraight[i].vect.size() - 5; ++j )
				{
					chibangVect.push_back(m_vStraight[i].vect[j]);

					//够了
					if( chibangVect.size() >= result.conut/3 )
					{
						for( int j = 0; j < chibangVect.size(); ++j )
							vect.push_back(chibangVect[j]);
						return vect;
					}
				}
			}
		}

		//拆对子
		for( int i = 0; i < m_vTwain.size(); ++i )
		{
			chibangVect.push_back(m_vStraight[i].vect[0]);

			//够了
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
		三顺的出牌原则：因为三顺出牌可以带两张（或更多）单牌或两个（或更多）对子，所以与出三条一样，
		需要检测是否有单牌或对子。如果有足够多的单牌或对子， 则将其带出。如果有单牌，但没有足够多的单牌，
		则检查是否有6连以上的连牌，如果有将连牌的最小张数当作单牌带出。如果有对子，但没有足够多的对子，
		则检 查是否有4连以上的双顺，如果有将双顺的最小对子当作对子带出
		*/

		if( m_vAircraft.size() > 0)
			vect = m_vAircraft[0].vect;
		else
			return vect;

		int count = m_vAircraft[0].result.conut;

		//带翅膀
		vector<enCard> chibangVect;
		stResult rs = stResult(TYPE_NULL,-1,0);
		for( int i = 0; i < m_vSingle.size(); ++i )
		{
			vector<enCard> temp;
			temp = unseamCard(rs,1,1,false,false,1,1,m_vSingle);
			chibangVect.push_back(temp[0]);
			rs.value = temp[0] / 4; 

			//够了
			if( chibangVect.size() >= count/3 )
			{
				for( int j = 0; j < chibangVect.size(); ++j )
					vect.push_back(chibangVect[j]);
				return vect;
			}
		}

		//拆6连以上的顺子
		for( int i = 0; i < m_vStraight.size(); ++i )
		{
			if( m_vStraight[i].vect.size() >= 6 )
			{
				for( int j = 0; j < m_vStraight[i].vect.size() - 5; ++j )
				{
					chibangVect.push_back(m_vStraight[i].vect[j]);

					//够了
					if( chibangVect.size() >= count/3 )
					{
						for( int j = 0; j < chibangVect.size(); ++j )
							vect.push_back(chibangVect[j]);
						return vect;
					}
				}
			}
		}

		//拆对子
		for( int i = 0; i < m_vTwain.size(); ++i )
		{
			chibangVect.push_back(m_vTwain[i].vect[0]);

			//够了
			if( chibangVect.size() >= count/3 )
			{
				for( int j = 0; j < chibangVect.size(); ++j )
					vect.push_back(chibangVect[j]);
				return vect;
			}
		}

		//出3不带
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
//出4带2
vector<enCard> logic::appearFourBandTwo(stResult result,bool team,bool genpai)
{
	vector<enCard> Vect;
	return Vect;
}
//找出相等的牌 
bool logic::FindCompareCard(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect,enType type)
{
	//快速排序
	sort_quick(CardVector,0,CardVector.size() - 1);

	int count = (int)type;
	if( type == TYPE_WANGZHA )
		count = 2;

	for( int i = 0; i < (int)CardVector.size(); )
	{
		vector<enCard> temp;//size 大于等于4 就是炸弹 等于3 就是 3不带 等于2 就是连对 或者王炸
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
		//相同的牌
		if( temp.size() == count )
		{
			//设置牌型的信息
			stSaveCardData tempCardData;
			tempCardData.result.conut = count;
			tempCardData.result.type = type;
			tempCardData.result.value = temp[0] / 4;

			//添加到tempCardData
			for(int k = 0; k < (int)temp.size(); ++k )
				tempCardData.vect.push_back(temp[k]);

			//添加到saveCardVect
			saveCardVect.push_back(tempCardData);

			//从CardVector里删除
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
//查找单牌
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
//查找连对
bool logic::FindContinuousTwain(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect)
{
	//查找对子
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
			if( this->compareCard(m_vTwain[j].vect[0],Spandes_2) ) //不能是2
				Flags = false;
			if( this->compareCard(m_vTwain[j].vect[0],Little_King)) //不能是王
				Flags = false;

			int NowEndIndex = temp.vect.size() - 1;
			int NextEndIndex = m_vTwain[j].vect.size() - 1;

			if( m_vTwain[j].vect[NextEndIndex]/4 - temp.vect[NowEndIndex]/4 == 1 && Flags )//连续的
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

		//是连对
		//从m_vTwain里删除
		if( temp.vect.size() >= 6 )
		{
			m_vTwain.erase(m_vTwain.begin() + i);
			saveCardVect.push_back(temp);
		}
		else//如果不是 把删除的还回去
		{
			for( vector<stSaveCardData>::iterator it = tempVect.begin(); it != tempVect.end(); ++it )
				m_vTwain.push_back(*it);

			i += 1;
		}
	}

	return saveCardVect.size() > 0;
}
//查找顺子
bool logic::FindStraight(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect)
{
	//快速排序
	sort_quick(CardVector,0,CardVector.size() - 1);

	//找出5连顺子
	for( int i = 0; i < (int)CardVector.size(); )
	{
		vector<enCard> tempCardVect;//size等于5代表是顺子
		tempCardVect.push_back(CardVector[i]);//临时存储牌
		bool spill = false;
		for( int j = 0; tempCardVect.size() != 5 ; ++j )
		{
			int NowIndex = 0;
			for( int k = 0; k < (int)CardVector.size(); ++k )
			{
				//找出牌在CardVector里的下标
				int index = tempCardVect.size() - 1;
				if( CardVector[k] == tempCardVect[index] )
				{
					NowIndex = k;
					break;
				}
			}

			//下一个判断的下标
			int NextIndex = i + j + 1;

			//是否越界
			if( NextIndex >= (int)CardVector.size() )
			{
				spill = true;
				break;
			}

			//不能是王和2
			if( compareCard(CardVector[NextIndex],Little_King) || compareCard(CardVector[NextIndex],Spandes_2))
				break;

			//重复的
			if( CardVector[NextIndex]/4 - CardVector[NowIndex]/4 == 0 )
				continue;

			//是连续的
			if( CardVector[NextIndex]/4 - CardVector[NowIndex]/4 == 1 )
				tempCardVect.push_back(CardVector[NextIndex]);
		}

		//越界
		if( spill )
			break;

		if( tempCardVect.size() == 5 )//代表是顺子 
		{
			//把后面5个牌加入到temp里
			stSaveCardData temp;
			for(int k = 0; k < tempCardVect.size(); ++k )
				temp.vect.push_back(tempCardVect[k]);

			//从CardVector里删除找出来的牌
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

			//设置顺子的信息
			temp.result.value = temp.vect[0] / 4;
			temp.result.type = TYPE_SHUNZI;
			temp.result.conut = 5;
			saveCardVect.push_back(temp);
		}
		else
			i += 1;
	}

	//没找到
	if( saveCardVect.size() == 0 )
		return false;

	//扩展连顺，在剩余里的牌找，如果某张牌能
	//与已知的顺子能组成更大的连牌，就合并在一起
	for( int i = 0; i < (int)CardVector.size(); )
	{
		bool Flags = false;
		for( int j = 0; j < saveCardVect.size(); ++j )
		{
			//末尾下标
			int endIndex = saveCardVect[j].vect.size() - 1;

			//不能组合
			if( CardVector[i]/4 - saveCardVect[j].vect[endIndex]/4 != 1 )
				continue;

			//不能跟王和2组合
			if( compareCard(CardVector[i],Little_King) || compareCard(CardVector[i],Spandes_2) )
				continue;

			//能组合
			Flags = true;
			saveCardVect[j].vect.push_back(CardVector[i]);
			saveCardVect[j].result.conut += 1;
			CardVector.erase(CardVector.begin() + i);
			break;
		}

		i = ( Flags ) ? i : i + 1;
	}

	//合并连顺
	//如果某两组牌能无缝连接成更大的连牌，则将其合并在一组
	for( int i = 0; i < saveCardVect.size(); i++ )
	{
		//末尾下标
		int endIndex = saveCardVect[i].vect.size() - 1;

		for( int k = 0; k < saveCardVect.size(); ++k )
		{
			if( i == k )
				continue;

			//不是连续的，不能组合
			if( saveCardVect[k].vect[0]/4 - saveCardVect[i].vect[endIndex]/4 != 1 )
				continue;

			//合并
			for( int j = 0; j < saveCardVect[k].vect.size(); ++j )
				saveCardVect[i].vect.push_back(saveCardVect[k].vect[j]);

			//删除
			saveCardVect.erase(saveCardVect.begin() + k);
			break;
		}
	}

	return true;
}
//查找飞机
bool logic::FindAircraft(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect)
{
	if( m_vThreeBandZero.size() == 0 )
		FindThreeBandZero(CardVector,m_vThreeBandZero);

	for( int i = 0; i < (int)m_vThreeBandZero.size(); )
	{
		//默认当前的3条能组成飞机
		//添加到Temp里
		stSaveCardData temp;
		temp.result = m_vThreeBandZero[i].result;
		temp.vect   = m_vThreeBandZero[i].vect;
		for( int j = 0; j < (int)m_vThreeBandZero.size(); )
		{
			bool Flags = true;
			if( this->compareCard(m_vThreeBandZero[j].vect[0],Spandes_2) )//不能是2
				Flags = false;
			if( this->compareCard(m_vThreeBandZero[j].vect[0],Little_King))//不能是王
				Flags = false;

			int NowEndIndex = temp.vect.size() - 1;//最后下标
			int NextEndIndex = m_vThreeBandZero[j].vect.size() - 1;//最后下标
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

		//找到了飞机 
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
//查找4带2
bool logic::FindFourBandTwo(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect)
{
	return false;
}
//查找对子
bool logic::FindTwain(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect)
{
	return FindCompareCard(CardVector,saveCardVect,TYPE_DUIZI);
}
//查找3带1
bool logic::FindThreeBandOne(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect)
{
	return false;
}
//查找3带2
bool logic::FindThreeBandTwo(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect)
{
	return false;
}
//查找3不带
bool logic::FindThreeBandZero(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect)
{
	return FindCompareCard(CardVector,saveCardVect,TYPE_3BUDAI);
}
//查找炸弹
bool logic::FindBomb(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect)
{ 
	return FindCompareCard(CardVector,saveCardVect,TYPE_ZHADAN);
}
//查找王炸
bool logic::FindWangBomb(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect)
{
	return FindCompareCard(CardVector,saveCardVect,TYPE_WANGZHA);
}
//出任意牌
vector<enCard> logic::ChuRenYiPai(bool myLandlord,enType type,stPlayerData last,stPlayerData next,stResult result)
{
	vector<enCard> vect;

	int _3TiaoConut = m_vThreeBandZero.size();
	for( int i = 0; i < m_vAircraft.size(); ++i )
		_3TiaoConut += m_vAircraft[i].result.conut / 3;

	//如果自己是地主
	if( myLandlord )
	{
		//在出牌时，如果偏家有一个人只剩一张牌时，尽量不出单牌，否则单牌由大到小出。 
		if( last.cardCount == 1 || next.cardCount == 1 )
		{
			//出对子
			if(( vect = appearTwain(result,false,false)).size() > 0 && _3TiaoConut < m_vTwain.size() ) 
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//出飞机
			if(( vect = appearAircraft(result,false,false)).size() > 0)
				return vect;
			//出3带2
			if(( vect = appearThreeBandTwo(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//出3带1
			if(( vect = appearThreeBandOne(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//出3不带
			if(( vect = appearThreeBandZero(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//出顺子
			if(( vect = appearStraight(result,false,false)).size() > 0)
				return vect;
			//出连对
			if(( vect = appearContinuousTwain(result,false,false)).size() > 0)
				return vect;
			//出单牌 由大到小出
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
			//出炸弹
			if(( vect = appearBomb(result,false,false)).size() > 0)
				return vect;
			//出王炸
			if(( vect = appearWangBomb(result,false,false)).size() > 0)
				return vect;
		}
		else if( last.cardCount == 2 || next.cardCount == 2 )//敌人剩下2张牌
		{
			//出飞机
			if(( vect = appearAircraft(result,false,false)).size() > 0)
				return vect;
			//出3带2
			if(( vect = appearThreeBandTwo(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//出3带1
			if(( vect = appearThreeBandOne(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//出3不带
			if(( vect = appearThreeBandZero(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//出顺子
			if(( vect = appearStraight(result,false,false)).size() > 0)
				return vect;
			//出连对
			if(( vect = appearContinuousTwain(result,false,false)).size() > 0)
				return vect;
			//出单牌
			if(( vect = appearSingle(result,false,false)).size() > 0  )
				return vect;
			//出对子
			//从大到小出
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
			//出炸弹
			if(( vect = appearBomb(result,false,false)).size() > 0)
				return vect;
			//出王炸
			if(( vect = appearWangBomb(result,false,false)).size() > 0)
				return vect;

		}
		else//正常出牌
		{
			//算单牌的数量，2和王不算
			int singleSize = m_vSingle.size();
			for( int i = 0; i < m_vSingle.size(); ++i )
			{
				if( this->compareCard(m_vSingle[i].vect[0],Little_King) || this->compareCard(m_vSingle[i].vect[0],Spandes_2) ) 
					singleSize -= 1;
			}

			singleSize -= _3TiaoConut;

			if( singleSize > 2 )
			{
				//出单牌
				if(( vect = appearSingle(result,false,false)).size() > 0 && !this->compareCard(vect[0],Spandes_2 ) 
					&& !this->compareCard(vect[0],Little_King)  )
					return vect;
			}
			//出对子
			if(( vect = appearTwain(result,false,false)).size() > 0 && _3TiaoConut < m_vTwain.size() ) 
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//出飞机
			if(( vect = appearAircraft(result,false,false)).size() > 0)
				return vect;
			//出3带2
			if(( vect = appearThreeBandTwo(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//出3带1
			if(( vect = appearThreeBandOne(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//出3不带
			if(( vect = appearThreeBandZero(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//出顺子
			if(( vect = appearStraight(result,false,false)).size() > 0)
				return vect;
			//出连对
			if(( vect = appearContinuousTwain(result,false,false)).size() > 0)
				return vect;
			if( singleSize <= 2 )
			{
				//出单牌
				if(( vect = appearSingle(result,false,false)).size() > 0 )
					return vect;
			}
			//出炸弹
			if(( vect = appearBomb(result,false,false)).size() > 0)
				return vect;
			//出王炸
			if(( vect = appearWangBomb(result,false,false)).size() > 0)
				return vect;
		}
	}
	else // 不是地主
	{

		// 1) 在出牌时，如果下家是庄家，且庄家只剩一张牌时，尽量不出单牌，否则单牌由大到小出。
		if( next.landlord && next.cardCount == 1 || last.landlord && last.cardCount == 1 )
		{
			//出对子
			if(( vect = appearTwain(result,false,false)).size() > 0 && _3TiaoConut < m_vTwain.size() ) 
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//出飞机
			if(( vect = appearAircraft(result,false,false)).size() > 0)
				return vect;
			//出3带2
			if(( vect = appearThreeBandTwo(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//出3带1
			if(( vect = appearThreeBandOne(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//出3不带
			if(( vect = appearThreeBandZero(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//出顺子
			if(( vect = appearStraight(result,false,false)).size() > 0)
				return vect;
			//出连对
			if(( vect = appearContinuousTwain(result,false,false)).size() > 0)
				return vect;
			//出单牌 由大到小出
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
			//出炸弹
			if(( vect = appearBomb(result,false,false)).size() > 0)
				return vect;
			//出王炸
			if(( vect = appearWangBomb(result,false,false)).size() > 0)
				return vect;
		}

		if( next.landlord && next.cardCount == 2 || last.landlord && last.cardCount == 2 )
		{
			//出飞机
			if(( vect = appearAircraft(result,false,false)).size() > 0)
				return vect;
			//出3带2
			if(( vect = appearThreeBandTwo(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//出3带1
			if(( vect = appearThreeBandOne(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//出3不带
			if(( vect = appearThreeBandZero(result,false,false)).size() > 0 )
			{
				if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
					vect.clear();
				else
					return vect;
			}
			//出顺子
			if(( vect = appearStraight(result,false,false)).size() > 0)
				return vect;
			//出连对
			if(( vect = appearContinuousTwain(result,false,false)).size() > 0)
				return vect;
			//出单牌
			if(( vect = appearSingle(result,false,false)).size() > 0  )
				return vect;
			//出对子
			//从大到小出
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
			//出炸弹
			if(( vect = appearBomb(result,false,false)).size() > 0)
				return vect;
			//出王炸
			if(( vect = appearWangBomb(result,false,false)).size() > 0)
				return vect;
		}


		// 2) 如果处在下家的偏家只剩一张牌时，在出牌时出手中最小的牌。跟牌还按照一般的原则。 
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

		//算单牌的数量，2和王不算
		int singleSize = m_vSingle.size();
		for( int i = 0; i < m_vSingle.size(); ++i )
		{
			if( this->compareCard(m_vSingle[i].vect[0],Little_King) || this->compareCard(m_vSingle[i].vect[0],Spandes_2) ) 
				singleSize -= 1;
		}

		singleSize -= _3TiaoConut;

		if( singleSize > 2 )
		{
			//出单牌
			if(( vect = appearSingle(result,false,false)).size() > 0 && !this->compareCard(vect[0],Spandes_2) 
				&& !this->compareCard(vect[0],Little_King)  )
				return vect;
		}
		//出对子
		if(( vect = appearTwain(result,false,false)).size() > 0 && _3TiaoConut < m_vTwain.size() ) 
		{
			if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
				vect.clear();
			else
				return vect;
		}
		//出飞机
		if(( vect = appearAircraft(result,false,false)).size() > 0)
			return vect;
		//出3带2
		if(( vect = appearThreeBandTwo(result,false,false)).size() > 0 )
		{
			if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
				vect.clear();
			else
				return vect;
		}
		//出3带1
		if(( vect = appearThreeBandOne(result,false,false)).size() > 0 )
		{
			if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
				vect.clear();
			else
				return vect;
		}
		//出3不带
		if(( vect = appearThreeBandZero(result,false,false)).size() > 0 )
		{
			if( this->compareCard(vect[0],Spandes_2) && m_iCountHandCard > 2 )
				vect.clear();
			else
				return vect;
		}
		//出顺子
		if(( vect = appearStraight(result,false,false)).size() > 0)
			return vect;
		//出连对
		if(( vect = appearContinuousTwain(result,false,false)).size() > 0)
			return vect;
		if( singleSize <= 2 )
		{
			//出单牌
			if(( vect = appearSingle(result,false,false)).size() > 0)
				return vect;
		}
		//出炸弹
		if(( vect = appearBomb(result,false,false)).size() > 0)
			return vect;
		//出王炸
		if(( vect = appearWangBomb(result,false,false)).size() > 0)
			return vect;
	}

	return vect;
}
//Ai
logic::enAiResult logic::Ai(enState state,//现在什么状态
							vector<enCard> cardVect,//手里有的牌
							vector<enCard>& ReturnVect,//要返回出去的牌
							enType LandlordNoCard,//地主不要的牌型
							stPlayerData lastPlayer,//上一个玩家的信息
							stPlayerData nowPlayer,//当前玩家的信息
							stPlayerData nextPlayer,//下一个玩家的信息
							int dir,//出牌玩家的方向
							stResult result)//上一个玩家出的什么牌，默认没出牌
{

	bool myLandlord			= (!lastPlayer.landlord && !nextPlayer.landlord);//自己是不是地主
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
	case en_Wait://等待
		{
			return en_AiWait;
		}break;
	case en_JiaoDiZhu://叫地主
		{
			bool flags = ifCallCard(cardVect);
			return flags ? en_CryLoadlord : en_NoCryLoadlord;
		}break;
	case en_QiangDiZhu://抢地主
		{
			bool flags = ifCallCard(cardVect);
			return flags ? en_RobLaodlord : en_NoRobLaodlord;
		}break;
	case en_JiaBei://加倍
		{
			bool flags = ifCallCard(cardVect);
			return flags ? en_Double : en_NoDouble;
		}break;
	case en_ChuPai://出任意牌
		{
			//拆牌
			unseamCard(cardVect);

			//计算需要多少手出完牌
			m_iCountHandCard = this->countHandCard();

			//出牌
			if((ReturnVect = ChuRenYiPai(myLandlord,LandlordNoCard,lastPlayer,nextPlayer,result)).size() == 0 )
				ReturnVect.push_back(tempVect[0]); 

			return en_AppearCard;

		}break;
	case en_GenPai://必须出指定的类型牌
		{
			ReturnVect = hint(cardVect,result,LandlordNoCard,lastPlayer,nowPlayer,nextPlayer,dir,false);//提示
			if( ReturnVect.size() != 0 )
				return en_AppearCard;
			else
				return en_NoDenmand;
		}break;
	}

	return en_AiWait;
}
//提示 返回的vect的size如果为0 就表示要不起
vector<enCard> logic::hint(vector<enCard> CardVector,stResult result,enType type,
						   stPlayerData lastPlayer,stPlayerData nowPlayer,stPlayerData nextPlayer,int dir,bool flag)//是否是玩家提示)
{
	vector<enCard> hintVect;			//返回出去牌的表
	vector<enCard> tempVect = CardVector;//找出3带一

	//拆牌
	unseamCard(CardVector);

	//计算需要多少手出完牌
	m_iCountHandCard = this->countHandCard();

	bool myLandlord,team;
	if( dir == -1 )//还没出牌
	{
		myLandlord = true;
		team = false;
	}
	else
	{
		myLandlord = nowPlayer.landlord;//自己是不是地主
		team = ( !m_AppCardPlayer.landlord && !myLandlord );//是不是队友
	}

	switch( result.type )
	{
	case TYPE_NULL://出任意牌型
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
	case TYPE_3DAI1://3带1
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
	case TYPE_3BUDAI://3不带
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
	case TYPE_3DAI2://3带2
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
	case TYPE_DUIZI://对子
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
	case TYPE_DANPAI://单牌
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
	case TYPE_LIANDUI://连对
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
	case TYPE_SHUNZI://顺子
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
	case TYPE_FEIJI://飞机
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

//是否叫地主
//如果手上有大王 和两个2以上 其余的牌不是单牌就叫地主
bool logic::ifCallCard(vector<enCard>& CardVector)
{
	bool big_king = false;//大王
	bool lit_king = false;//小王
	int _2 = 0;//2
	int _A = 0;//A
	int _K = 0;//K
	int bomb = 0;//炸弹
	int aircraft = 0;//飞机
	int Straight = 0;//顺子
	int sandaiyi = 0;//三带一
	int liandui = 0;//连对

	vector<int> continueVect;

	for( int i = 0; i < (int)CardVector.size(); ++i  )
	{
		if( IsIndexInVector(i,continueVect) )
			continue;

		if( CardVector[i] == Big_King )//大王
		{
			big_king = true;
			continueVect.push_back(i);
		}
		if( CardVector[i] == Little_King )//小王
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

	//判断有几个炸弹和3带1
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

		if( compareCount == 3 )//3带1
			sandaiyi += 1;
		else if( compareCount == 4 )//炸弹
			bomb += 1;
	}

	if( big_king || lit_king && /*bomb >= 1 &&*/ _2 > 1 && _A >= 1 )
		return true;

	return false;
}
//是否抢地主
bool logic::ifRobLandlord(vector<enCard>& CardVector)
{
	return true;
}
//是否加倍
int logic::ifDouble(vector<enCard>& CardVector)
{
	return true;
}
//牌转换为int
int logic::cardTransformInt(enCard card)
{
	return (int)(card) / 4;
}
//判断牌是否相等
bool logic::compareCard(enCard card1,enCard card2)
{
	if( card1 == card2 )
		return true;

	int index1 = cardTransformInt(card1);
	int index2 = cardTransformInt(card2);

	return ( index1 == index2 );
}
//判断一个int是否在一个容器中
bool logic::IsIndexInVector(int index,vector<int>& vect)
{
	//判断是否重复
	for( vector<int>::iterator it = vect.begin(); it != vect.end(); ++it)
	{
		if( index == *it )
		{
			return true;
		}
	}

	return false;
}
//判断是否是单牌
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
//判断是否是对子
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
//判断是否3带1
stResult logic::judgeThreeBandOne(vector<enCard>& CardVector)
{
	if( CardVector.size() != 4 )
		return stResult(TYPE_NULL,-1,0);

	//判断一个数是否出现了3次
	for( int i = 0; i < (int)CardVector.size(); ++i)
	{
		enCard tempCard = CardVector[i];
		int equalityCount = 1;//相等的次数

		for( int j = 0; j < (int)CardVector.size(); ++j )
		{
			if( i == j )
				continue;

			if( compareCard(tempCard,CardVector[j]) )
				equalityCount++;
		}

		if( equalityCount == 3 )//3带1
		{
			int value = tempCard/4;
			return stResult(TYPE_3DAI1,value,CardVector.size());
		}
	}

	return stResult(TYPE_NULL,-1,0);
}
//判断是否3带2
stResult logic::judgeThreeBandTwo(vector<enCard>& CardVector)
{
	if( CardVector.size() != 5 )
		return stResult(TYPE_NULL,-1,0);

	enCard tempCard;
	//判断一个数是否出现了3次
	for( int i = 0; i < (int)CardVector.size(); ++i)
	{
		tempCard = CardVector[i];
		int equalityCount = 1;//相等的次数

		for( int j = 0; j < (int)CardVector.size(); ++j )
		{
			if( i == j )
				continue;

			if( compareCard(tempCard,CardVector[j]) )
				equalityCount++;
		}

		if( equalityCount == 3 )//3带1
			break;
	}

	//用来记录剩下的数
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
//判断是否3不带
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
//判断是否连对
stResult logic::judgeContinuousTwain(vector<enCard>& CardVector)
{
	if( CardVector.size() < 6 || CardVector.size() % 2 != 0 )
		return stResult(TYPE_NULL,-1,0);

	for( int i = 0; i < (int)CardVector.size(); ++i )
	{
		//是否有2
		if( CardVector[i] == Spandes_2 || CardVector[i] ==  RedStar_2 )
			return stResult(TYPE_NULL,-1,0);
		if( CardVector[i] == Club_2    || CardVector[i] ==  Hearts_2 )
			return stResult(TYPE_NULL,-1,0);

		//是否有王
		if( CardVector[i] == Little_King || CardVector[i] == Big_King)
			return stResult(TYPE_NULL,-1,0);
	}

	//创建CardVector总数一半的enCard变量
	//用来记录已经确认是对子的牌
	int size = CardVector.size() / 2;
	vector<enCard> vCard ;
	vector<int> vectorContinue;//用来存已经判断过了的下标

	for( int i = 0; i < (int)CardVector.size(); ++i )
	{
		//是否已经判断过了
		if( IsIndexInVector(i,vectorContinue) )
			continue;

		enCard tempCard = CardVector[i];
		int equalityCount = 1;//相等的次数

		for( int j = 0; j < (int)CardVector.size();++j )
		{
			//如果当前下标是自己或者比较过了
			if( i == j || IsIndexInVector(j,vectorContinue))
				continue;

			//是否是同样的牌
			if( compareCard(tempCard,CardVector[j]) )
			{
				vCard.push_back(tempCard);
				equalityCount += 1;
				vectorContinue.push_back(i);
				vectorContinue.push_back(j);
			}
		}

		//超出2个相等，就代表不是连对
		if( equalityCount != 2 )
		{
			return stResult(TYPE_NULL,-1,0);
		}
	}
	//排序
	sort_quick(vCard,0,vCard.size() - 1);

	//值
	int value = 0;
	//判断是否是连续的对子
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
//判断是不是王炸
stResult logic::judgeWangBomb(vector<enCard>& CardVector)
{
	if( CardVector.size() != 2 )
		return stResult(TYPE_NULL,-1,0);

	for( int i = 0; i < (int)CardVector.size(); ++i )
	{
		//不相等，代表不是4个一样的
		if( !compareCard(Little_King,CardVector[i]) )
			return stResult(TYPE_NULL,-1,0);
	}

	return stResult(TYPE_WANGZHA,VALUE_MAX,0);

}
//判断是否是炸弹
stResult logic::judgeBomb(vector<enCard>& CardVector)
{
	//数量不是炸弹的数量
	if( CardVector.size() != 4 && CardVector.size() != 2 )
		return stResult(TYPE_NULL,-1,0);

	//王炸？
	if( CardVector.size() == 2 )
	{
		//不是王炸
		if( CardVector[0] != Little_King && CardVector[0] != Big_King )
			return stResult(TYPE_NULL,-1,0);

		//不是王炸
		if( CardVector[1] != Little_King && CardVector[1] != Big_King )
			return stResult(TYPE_NULL,-1,0);
	}

	enCard temp = CardVector[0];
	for( int i = 0; i < (int)CardVector.size(); ++i )
	{
		//不相等，代表不是4个一样的
		if( !compareCard(temp,CardVector[i]) )
			return stResult(TYPE_NULL,-1,0);
	}

	return stResult(TYPE_ZHADAN,temp/4,CardVector.size());
}
//判断是否顺子
stResult logic::judgeStraight(vector<enCard>& CardVector)
{
	if( CardVector.size() < 5 )
		return stResult(TYPE_NULL,-1,0);

	for(int i = 0; i < CardVector.size(); ++i )
	{
		if( compareCard(CardVector[i],Little_King) || compareCard(CardVector[i],Spandes_2) )
			return stResult(TYPE_NULL,-1,0);
	}

	//快速排序
	sort_quick(CardVector,0,CardVector.size() - 1);

	for( int i = 0; i < CardVector.size() - 1; ++i )
	{
		if( CardVector[i+1]/4 - CardVector[i]/4 != 1 )
			return stResult(TYPE_NULL,-1,0);
	}

	return stResult(TYPE_SHUNZI,CardVector[0]/4,CardVector.size());
}
//判断是否是飞机
stResult logic::judgeAircraft(vector<enCard>& CardVector)
{
	if( CardVector.size() < 8 || CardVector.size() % 2 != 0 )
		return stResult(TYPE_NULL,-1,0);

	//记录判断过的下标
	vector<int> ContinueVect;
	//用来记录已经出现3次的enCard
	vector<enCard> tempVect;

	for( int i = 0; i < (int)CardVector.size(); ++i )
	{
		//是否已经判断过了
		if( IsIndexInVector(i,ContinueVect) )
			continue;

		enCard temp = CardVector[i];//临时的变量
		int equalityCount = 1;//出现的次数

		for( int j = 0; j < (int)CardVector.size(); ++j )
		{
			//是否已经判断过了
			if( i == j || IsIndexInVector(j,ContinueVect) )
				continue;

			//相等
			if( compareCard(temp,CardVector[j]) )
			{
				equalityCount += 1;
				ContinueVect.push_back(i);
				ContinueVect.push_back(j);
			}
		}

		//出现次数超过3次
		if( equalityCount > 3 )
			return stResult(TYPE_NULL,-1,0);
		else if( equalityCount == 3 )//刚好3次
			tempVect.push_back(temp);
	}

	//没有2个以上
	if( tempVect.size() < 2 )
		return stResult(TYPE_NULL,-1,0);

	//不能是二和王
	for( int i = 0; i < (int)tempVect.size(); ++i )
	{
		if( compareCard(tempVect[i],Spandes_2) || compareCard(tempVect[i],Little_King) )
			return stResult(TYPE_NULL,-1,0);
	}

	//排序
	sort_quick(tempVect,0,tempVect.size() - 1);

	//判断是否连续
	for( int i = 0; i < (int)tempVect.size() - 1 ; ++i )
	{
		if( (int)tempVect[i+1] / 4 - (int)tempVect[i] / 4 != 1 )
			return stResult(TYPE_NULL,-1,0);
	}

	int remain =  CardVector.size() - tempVect.size() * 3;//剩下的牌数
	if( remain != tempVect.size() )
		return stResult(TYPE_NULL,-1,0);

	//值
	int value = tempVect[0] / 4;
	return stResult(TYPE_FEIJI,value,tempVect.size() * 3 );
}
//判断是4带2
stResult logic::judgeFourBandTwo(vector<enCard>& CardVector)
{
	if( CardVector.size() != 6 )
		return stResult(TYPE_NULL,-1,0);

	//记录判断过的下标
	vector<int> ContinueVect;
	int value = 0;//值
	for( int i = 0; i < (int)CardVector.size(); ++i )
	{
		//是否已经判断过了
		if( IsIndexInVector(i,ContinueVect) )
			continue;

		enCard temp = CardVector[i];//临时的变量
		int equalityCount = 1;//出现的次数

		for( int j = 0; j < (int)CardVector.size(); ++j )
		{
			//是否已经判断过了
			if( i == j || IsIndexInVector(j,ContinueVect) )
				continue;

			//相等
			if( compareCard(temp,CardVector[j]) )
			{
				equalityCount += 1;
				ContinueVect.push_back(i);
				ContinueVect.push_back(j);
			}
		}

		//出现次数超过3次
		if( equalityCount > 4 )
			return stResult(TYPE_NULL,-1,0);
		else if( equalityCount == 4 )//刚好3次
			return stResult(TYPE_SIDAI2,temp/4,CardVector.size());
	}

	return stResult(TYPE_NULL,-1,0);
}
//快速排序
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