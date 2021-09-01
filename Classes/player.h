#pragma once 
#include "logic.h"
#include <string>
#include "cocos2d.h"
#include "public.h"
#include "timer.h"


USING_NS_CC;

#define DOWN_Y     40	//正下方玩家的牌的Y
#define SHOWCARD_Y 240  //打出去的牌的Y


//玩家
class CPlayer : public logic
{
public:

	//排列的方向
	enum enDir
	{
		DOWN,
		RIGTH,
		LEFT,
		_NULL
	};

	//牌的信息
	struct stCardSpr
	{
		bool opt;			   //是否选中
		bool popup;			   //是否弹出
		enCard card;		   //牌
		Sprite* sprite;		   //图片
		LayerColor* layerColor;//被选中时添加的半透明层
	};

	Node* m_Praent;			//父节点
	enDir m_Dir;			//自己所处的方向
	string m_sName;			//名字
	string m_sUserId;		//账号
	string m_sPassWord;		//密码
	Sprite* m_SpriteFigure;	//人物形象
	Sprite* m_SpriteJoyBean;//欢乐豆图片
	Label* m_Label;			//用来 显示 叫地主 不叫 抢地主 不抢 加倍 不加倍
	Label* m_LabelName;		//名字
	Label* m_LabelJoyBean;	//欢乐豆
	Label* m_LabelCardCount;//还剩多少牌
	Label* m_LabelUpString; //头上的字体， 用来显示是地主还是农民

	stResult m_Result;//出牌的结果
	CTimer*  m_Timer;//计时器

	float  m_fTime;		  //计时
	int m_iJoyBean;		  //欢乐豆
	bool m_bShowCard;     //是否明牌
	bool m_bLandlord;	  //是否是地主
	bool m_bSex;		  //性别 true为男 false为女
	bool m_bCardUpdate;   //牌发生了更新
	bool m_bMoveCard;	  //移动牌
	bool m_bNoCryLoadlord;//是否不叫地主

	static Sprite* m_SpriteMultiple;//倍数图片
	static Label* m_LabelMultiple;  //倍数
	static int m_iMultiple;		    //倍数
	static int m_iRobLoadlordLevel; //抢地主的声音等级
	static int m_iFollowCardLevel;  //跟牌的声音等级
	static int m_iNoAskLevel;       //要不起声音等级
	static int m_iCardGreaterLevel; //出牌声音等级
	static enType m_LandlordNoCryCard; //地主不要的牌
	

	vector<stCardSpr*> m_vCardVect;		  //记录总共有的纸牌
	vector<stCardSpr*> m_vDeleteCardVect; //记录要delete掉的纸牌
	vector<stCardSpr*> m_vMoveVect;		  //记录要移动的牌
	vector<stCardSpr*> m_vSendVect;		  //记录要打出的牌
	vector<stCardSpr*> m_vShowSendVect;   //显示要打出去的牌

	CPlayer(string userId,string passWord,string name,enDir dir,enState state,int integral,int multiple,bool sex,Node* parent);
	~CPlayer();

	//设置全部字体显示与否
	void setAllLabelHide(bool show);

	//运行
	enAiResult Run();
	
	//明牌
	void showCard(bool sound);

	//计时器完了之后的回调函数
	void TimerCallBackFun();

	//开始计时
	void startTimer(int timeMax);

	//停止计时
	void stopTimer();

	//声音等级初始化
	static void SoundLevelInit();  

	//获取出牌的结果
	stResult getResult();

	//Ai
	enAiResult Ai(stPlayerData lastPlayer,stPlayerData nowPlayer,stPlayerData nextPlayer,
					int dir,//出牌玩家的方向
					stResult result = stResult(TYPE_NULL,-1,0));//上一个玩家出的牌，默认没出			

	//设置自己头上的字，是地主还是农民
	void setUpString(string s);

	//清空
	void clear();			

	//不要
	void noDenmand();		

	//加倍
	void Double(bool dou);		

	//设置牌全部属性初始化
	void setCardInit();		

	//设置牌的半透明层初始化
	void setCardLayerInit();	

	//移动被选中的牌
	void moveOptCard();			

	//抢地主
	void RobLoadlord(bool rob);		

	//叫地主
	void CryLoadlord(bool cry);	

	//牌的位置更新
	void cardPosUpdate(enDir dir);	

	//返回一个点上牌的起点X与X+W
	bool getCardPoint(Point p,Point& left,Point& rigth);	

	//出牌
	bool appearCard(stResult result);	

	//出牌
	void chupai(string sound,int SoundIndex = 0);	

	//设置字体位置
	void setTextPos(Label* label,enDir dir);	

	//排序
	void my_Sort(vector<stCardSpr*>& vect);		

	//男的声音转女的声音
	void manTurnWomanSound(string& sound);		

	//设置为地主
	void setLandlord(const vector<stCardSpr>& vect);

	//排版
	void arrange(vector<stCardSpr*>& vect,enDir dir);

	//添加要打出的牌到M_SendVect
	void addSendCardVect(vector<stCardSpr*>& vect );	

	//提示
	bool hint(stResult result,stPlayerData lastPlayer,stPlayerData nowPlayer,stPlayerData nextPlayer,int dir,bool flag);

	//判断vector里的牌是什么牌返回type_null代表类型错误
	stResult ifVectCardType(vector<stCardSpr*>& vect );	

	//为2点之间的牌添加透明层
	void addInTowPointCardLayer(Point begin,Point end);	

	//设置牌不显示
	void setVectorCardHide(vector<stCardSpr*>& vect,bool hide);	

	//设置打出去的牌的位置
	void setShowCardPos(vector<stCardSpr*>& vect,enDir dir);	

};