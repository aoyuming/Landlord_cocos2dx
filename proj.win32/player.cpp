#include "player.h"

CPlayer::CPlayer(string userId,string passWord,string name,enDir dir,int integral,int multiple,bool sex)
	:
	m_sUserId(userId),
	m_sPassWord(passWord),
	m_sName(name),
	m_Dir(dir),
	m_iIntegral(integral),
	m_iMultiple(multiple),
	m_bSex(sex),
	m_bCardUpdate(false)
{

}

CPlayer::~CPlayer()
{

}


//排序
void CPlayer::my_Sort(vector<stCardSpr>& vect)
{
	for( int i = 0; i < vect.size(); ++i )
	{
		int j = i;
		for( int k = j + 1; k < vect.size(); ++k )
		{
			if( vect[j].card > vect[k].card )
				j = k;
		}
		if( j != i )
		{
			stCardSpr t = vect[j];
			vect[j] = vect[i];
			vect[i] = t;
		}
	}
}
//排版
void CPlayer::arrange(vector<stCardSpr>& vect,enDir dir)
{
	switch( dir )
	{
	case DOWN:
		{
			//总共牌的长度
			int length = vect.size() * 20;
			Size size = Director::getInstance()->getVisibleSize();//屏幕大小
			Vec2 p = Vec2(size.width / 2 - length / 2,size.height + 100);
			int count = 0;
			//排序
			my_Sort(vect);
			
			for( vector<stCardSpr>::iterator it = vect.begin(); it != vect.end(); ++it )
			{
				(*it).sprite->setPosition(20 * count + p.x,p.y);
			}

		}break;
	case RIGTH:
		{
			int length = vect.size() * 10;
			Size size = Director::getInstance()->getVisibleSize();
			Vec2 p = Vec2(size.width - 100,size.height / 2 - length / 2);
			int count = 0;

			//排序
			my_Sort(vect);
			
			for( vector<stCardSpr>::iterator it = vect.begin(); it != vect.end(); ++it )
			{
				(*it).sprite->setPosition(p.x,p.y + count * 10);
			}
		}break;
	case LEFT:
		{
			int length = vect.size() * 10;
			Size size = Director::getInstance()->getVisibleSize();
			Vec2 p = Vec2(100,size.height / 2 - length / 2);
			int count = 0;

			//排序
			my_Sort(vect);
			
			for( vector<stCardSpr>::iterator it = vect.begin(); it != vect.end(); ++it )
			{
				(*it).sprite->setPosition(p.x,p.y + count * 10);
			}
		}break;
	}

}

//牌的位置更新
void CPlayer::CardPosUpdate(enDir dir)
{
	arrange(m_vCardVect,dir);
}

//运行
void CPlayer::Run()
{
	//重新排版
	if( m_bCardUpdate )
	{
		CardPosUpdate(m_Dir);
	}

}