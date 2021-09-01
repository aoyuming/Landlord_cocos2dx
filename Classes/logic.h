#pragma once
/*
	本头文件是斗地主的逻辑类，里面的内容不包含平台相关的
	东西，跨平台开发的时候可以直接继承下来使用
											2016.11.7
									

										Ai算法


一、牌型 
        1 火箭：大小王在一起的牌型，即双王牌，此牌型最大，什么牌型都可以打。 
        2 炸弹：相同点数的四张牌在一起的牌型，比如四条A。除火箭外，它可以打任何牌型，炸弹对炸弹时，要比大小。 
        3 单支（一手牌）：单张牌，如一支3。 
        4 对子（一手牌）：相同点数的两张牌在一起的牌型，比如55。 
        5 三条：相同点数的三张牌在一起的牌型，比如三条4。 
        6 三带一手：三条 ＋ 一手牌的牌型，比如AAA+9或AAA+77。 
        7 单顺：五张或更多的连续单支牌组成的牌型，比如45678或345678910JQKA。2和大小王不可以连。 
        8 双顺：三对或更多的连续对子组成的牌型，比如334455或445566778899。2和大小王不可以连。 
        9 三顺：二个或更多的连续三条组成的牌型，比如777888或444555666777。2和大小王不可以连。 
        10 飞机带翅膀：三顺 ＋ 同数量的一手牌，比如777888+3+6或444555666+33+77+88。 
		11 四带二：四条+两手牌。比如AAAA+7+9或9999+33+55。 
二、牌型分析 
		1 单顺的确定 
        a) 选取五连，先取出最小的一个五连，再在剩余的牌中取出最小的一个五连，依此类推，直到没有五连为止。 
        b) 扩展五连，将剩余的牌与已经取出的牌进行比对，如果某张剩余的牌与已知的连牌能组成更大的连牌，则将其合并。一直到无法合并为止。 
        c) 合并连牌，如果某两组连牌能无缝连接成更大的连牌，则将其合并成一组。 
        经过上述选取、扩展和合并，则将一手牌中的所有连牌提取出来了，举例如下： 
        假定一手牌是：2AKQJ1099877766543 
        第一步，选取出34567，678910两个连牌组。剩余的牌还有79JQKA2 
        第二步，剩余的JQKA能和678910组成新的连牌678910JQKA。 
        第三步，已知的两个连牌组不能合并成新的、更大的连牌组，则这手牌就被分成了34567、678910JQKA两个连牌组和7、9、2三张单牌。 
 
2 双顺的确定 
        将一副牌中所有的对子检测出来，然后将对子排序，按照检测三连的方式可以将所有的双顺都提取出来。 
 
3 三条的确定 
        一副牌中的所有三条都能比较方便地提取出来。 
 
4 三顺的确定 
        在三条的基础上进行比较，如果有相连的三条，则将其合并成（尽可能大的）三顺。 
 
5 炸弹的确定 
        所有的四头都能很方便地确定。 
 
6 火箭的确定 
        只要牌张包含大小王，就将其组成火箭。 
 
7 牌张的分类方法 
        a) 先确定火箭：判断是否有大小王。 
        b) 再确定炸弹：判明是否有四头。 
        c) 再确定三条：在除了炸弹以外的牌中判断是否包含三条。 
        d) 再确定三顺：在已经确定的三条中判断是否包含相邻的三条，如果有，则将其组成三顺。注意，应该使三顺的数量尽可能大。即如果有444555666，则将其合成一个三顺，而不是分成444555一个三顺和666一个三条。 
        e) 再确定单顺：判断单顺时必须去除四个2以外的所有炸弹。首先判断是否存在除了三条牌（这里的三条是指所有的三条）以外的连牌，如果有，则将其提取出来。其 次，将剩余的牌与每一个三条（不包含三顺）进行试组合，如果能够重新组成单顺和对子，则将原有的三条取消，重新组合成连牌和单顺（例子4566678重新 组成45678和66）。最后，将已知的连牌、三条（不包含三顺）和剩下的牌张再试组合，将所有如45678（已知的连牌）999（三条）10J（单 牌），重新组合成45678910J和99。通过以上的方法，就能将连牌和三条很好地重新组合。 
        f) 再确定双顺：首先，如果两单顺牌完全重合，则将其重新组合成双顺。其次，在除炸弹、三顺、三条、单顺以外的牌中检测是否包含双顺。如果有，将其提取出来。 
        g) 再确定对子：在炸弹、三顺、三条、连牌、双顺以外的牌中检测是否存在对子，如果存在将其提取出来。 
        h) 再确定单牌：除了炸弹、三顺、三条、连牌、双顺、对子以外的所有牌张都是单牌。 
 
8 对子的确定 
        参见上一节的“再确定对子”。 
 
9 单牌的确定 
        参见上一节的“再确定单牌”。 
 
三、一手牌的手数分析 
        为 了使斗地主机器人有一定的智能，必须对一手牌的手数进行分析。所谓一手牌的手数，指的是在没有别人压牌的情况下需要出几把能将牌出完。这个数字在经过上面 的分类后是可以确定的。还以上面的一手牌22AAKQJ9987776654为例，这手牌按照前面的分析方法可以分析出以下的牌型： 
        22、AA、K、Q、J、987654、9、77、6的牌型 
        通过对上面牌型的计算，可以容易得出这手牌需要9手才能出完，因此这手牌的手数就是9。 
 
四、绝对手数和相对手数的概念 
        因为在斗地主游戏中存在火箭或炸弹的可能，所以理论上只有火箭才不可能被别人压住，所以，可以不考虑相对手数的概念。以后都简称手数。 
 
五、叫牌原则分析 
        因为在斗地主中，火箭、炸弹、王和2可以认为是大牌，所以叫牌需要按照这些牌的多少来判断。下面是一个简单的原则： 
        假定火箭为8分，炸弹为6分，大王4分，小王3分，一个2为2分，则当分数 
        大于等于7分时叫三倍； 
        大于等于5分时叫二倍； 
        大于等于3分时叫一倍； 
        小于三分不叫。 
 
六、出牌的一般原则 
        1 出牌的原则一般按照从小到大的原则，即首先出包含最小牌的组合（单牌、对子、双顺、连牌、三顺、三条等，炸弹、火箭不包括在内）。 
        2 三条的出牌原则：因为三条出牌可以带一张单牌或一个对子，所以在出三条时需要检测是否有单牌，如果有，则带一张最小的单牌，如果没有，则再检测是否存在对子，如果有，则跟一个最小的对子，如果单牌和对子都没有，则出三条。 
        在带牌时，除非是只剩两手牌，否则不能带王或2。 
        3 三顺的出牌原则：因为三顺出牌可以带两张（或更多）单牌或两个（或更多）对子，所以与出三条一样，需要检测是否有单牌或对子。如果有足够多的单牌或对子， 则将其带出。如果有单牌，但没有足够多的单牌，则检查是否有6连以上的连牌，如果有将连牌的最小张数当作单牌带出。如果有对子，但没有足够多的对子，则检 查是否有4连以上的双顺，如果有将双顺的最小对子当作对子带出。 
        在带牌时，除非是只剩两手牌，否则不能带王或2。 
        4 连牌的出牌原则：直接出。 
        5 双顺的出牌原则：直接出。 
        6 对子的出牌原则：因为对子可以用三条、三顺等带出，所以在出对子时，应该先检测一下三条＋三顺（中三条）的数量，如果所有三条数量 <= 对子＋单牌数量总和－2时，出对子，否则出三带2等等。 
        7 单牌的出牌原则：因为单牌可以用三条、三顺等带出，所以在出单牌时，应该先检测一下三条＋三顺（中三条）的数量，如果所有三条数量 <= 对子＋单牌数量总和－2时，出单牌，否则出三带1等等。 
 
七、跟牌的一般原则 
        1 如果手中有独立的，与所出的牌一样牌型的牌时，先跟之。 
        2 2可以作为单牌、对子、三条等形式跟出。 
        3 当手中没有相应牌跟时，如果是本方人员出的牌，可以不跟，如果是对方出的牌，则必须拆牌跟，如果再没有，出炸弹或火箭，否则PASS。 
        4 如果手中的牌除了炸弹。火箭外还剩一手牌，则如果牌型相符，则先跟之，否则炸之。 
        5 单牌的跟牌原则：如果手中有单牌，则跟之，否则拆2跟之，否则拆对牌跟之，否则拆6连以上的单顺顶张跟之，否则拆三条跟之，否则拆三顺跟之，否则拆5连单顺跟之，否则拆双顺跟之，否则炸之，否则PASS。 
        6 对牌的根牌原则：如果手中有对子，则跟之，否则拆4连以上的双顺顶张跟之，否则拆三条跟之，否则拆双顺跟之，否则拆三顺跟之，否则炸之，否则PASS。 
        7 三条、三带1、三带2等牌的根牌原则：如果手中有相同牌型的牌则跟之，否则拆三顺跟之，否则炸之，否则PASS。注意，只有在手中牌在出了以后还剩一手牌时，或直接出完的情况下，才允许带王或2。 
        在没有足够牌带的情况下，参照单牌。对子的拆牌原则进行拆牌处理。 
        8 三顺及三顺带牌的根牌原则：如果有相应的牌型，则跟之，否则可以将大的三顺拆成小的三顺跟之，否则炸之，否则PASS。注意，只有在手中牌在出了以后还剩一手牌时，或直接出完的情况下，才允许带王或2。 
        在没有足够牌带的情况下，参照单牌。对子的拆牌原则进行拆牌处理。 
        9 连牌的跟牌原则：如果有相应的牌型，则跟之，否则拆相同张数的双顺，否则拆相同张数的三顺，否则拆不同张数的连牌，否则拆不同张数的双顺，否则拆不同张数的三顺，否则炸之，否则PASS。 
        10 双顺的跟牌原则：有相同牌型的牌，则跟之，否则拆不同张数的双顺，否则拆不同张数的三顺，否则拆相同张数的三顺，否则炸之，否则PASS。 
        11 炸弹的跟牌原则：有超过所出炸弹的炸弹，或有火箭，则炸之，否则PASS。 
        12 炸弹带两手牌的跟牌原则：如果有炸弹，则炸之，否则PASS。 
 
八、打牌原则解析 
        1 坐庄打法：因为坐庄的只是自己一个人，不存在配合问题，所以一般按照前面的原则出牌即可。 
            a) 在出牌时，如果偏家有一个人只剩一张牌时，尽量不出单牌，否则单牌由大到小出。 
            b) 在跟牌时，如果偏家有一个人只剩一张牌时，跟手中最大的牌。 
        2 偏家打法：偏家因为牵涉到配合问题，所以打法有一些不同。 
            a) 在出牌时，如果是庄家的上家，且庄家只剩一张牌时，尽量不出单牌，否则单牌由大到小出。 
            b) 在跟牌时，如果是庄家的上家，且庄家只剩一张牌时，跟手中最大的牌。 
            c) 当一个偏家打出的是单牌时，一般情况下能跟就跟。如果手中必须跟2或以上的牌时，选择PASS。 
            d) 当一个偏家打出的是对子时，一般情况下能跟就跟。如果手中必须跟AA或以上的牌时，选择PASS。 
            e) 如果一个偏家打出的牌是除了单牌及对子以外的牌型，则选择PASS。 
            f) 如果处在下家的偏家只剩一张牌时，在出牌时出手中最小的牌。跟牌还按照一般的原则。 
*/


#include <vector>
#include <list>
using namespace std;

//纸牌		强制转换为int * 图片宽就是牌在图片里的x
enum enCard
{
	//3
	Spandes_3,
	RedStar_3,
	Club_3,
	Hearts_3,
	//4
	Spandes_4,
	RedStar_4,
	Club_4,
	Hearts_4,
	//5
	Spandes_5,
	RedStar_5,
	Club_5,
	Hearts_5,
	//6
	Spandes_6,
	RedStar_6,
	Club_6,
	Hearts_6,
	//7
	Spandes_7,
	RedStar_7,
	Club_7,
	Hearts_7,
	//8
	Spandes_8,
	RedStar_8,
	Club_8,
	Hearts_8,
	//9
	Spandes_9,
	RedStar_9,
	Club_9,
	Hearts_9,
	//10
	Spandes_10,
	RedStar_10,
	Club_10,
	Hearts_10,
	//J
	Spandes_J,
	RedStar_J,
	Club_J,
	Hearts_J,
	//Q
	Spandes_Q,
	RedStar_Q,
	Club_Q,
	Hearts_Q,
	//K
	Spandes_K,
	RedStar_K,
	Club_K,
	Hearts_K,
	//A
	Spandes_A,
	RedStar_A,
	Club_A,
	Hearts_A,
	//2
	Spandes_2,
	RedStar_2,
	Club_2,
	Hearts_2,
	//大王，小王
	Little_King,
	Big_King
};

//出牌的类型
enum enType 
{
	TYPE_NULL,//空
	TYPE_DANPAI,//单牌
	TYPE_DUIZI,//对子
	TYPE_3BUDAI,//3不带
	TYPE_ZHADAN,//炸弹
	TYPE_3DAI1,//3带1
	TYPE_3DAI2,//3带一对
	TYPE_LIANDUI,//连对
	TYPE_WANGZHA,//王炸
	TYPE_SHUNZI,//顺子
	TYPE_FEIJI,//飞机
	TYPE_SIDAI2//4带2
};

#define VALUE_MAX 10000 //最大牌型的值

//结果
struct stResult
{
	stResult(){}
	stResult(enType ty,int va,int ct)
	{
		type = ty;
		value = va;
		conut = ct;
	}
	//判断是否相等
	bool operator == (const stResult& that)
	{
		if( type != that.type )
			return false;
		if( value != that.value )
			return false;
		if( conut != that.conut )
			return false;
		return true;
	}

	enType type;//出牌的类型
	int value;//出牌的值
	int conut;//出牌的数量

	//值的计算
	//单牌：返回本身牌的值 / 4  大王再加2 小王再加1
	//对子：返回本身牌的值 / 4
	//连对：返回最小的牌的值 / 4
	//顺子：返回最小的牌的值 / 4
	//炸弹：返回第一个牌的值 / 4
	//飞机：返回最小的牌的值 / 4
	//王炸：返回 VALUE_MAX
	//三带一： 返回第一个牌的值 / 4
	//三带二： 返回第一个牌的值 / 4
	//三不带： 返回第一个牌的值 / 4
};

//玩家基本信息
struct stPlayerData
{
	int dir;//方向 0下 1右 2左
	bool landlord;//是否是地主
	int cardCount;//剩下的牌的数量
};

//牌的类型与信息
struct stSaveCardData
{
	stResult result;//牌的信息
	vector<enCard> vect;//牌
};

//逻辑
class logic
{
public:

	//状态
	enum enState
	{
		en_Wait,	  //等待
		en_JiaoDiZhu, //叫地主
		en_QiangDiZhu,//抢地主
		en_JiaBei,	  //加倍
		en_ChuPai,	  //出任意牌
		en_GenPai	  //必须出指定的类型牌
	};

	//Ai返回结果
	enum enAiResult
	{
		en_AiWait,		 //等待
		en_CryLoadlord,	 //叫地主
		en_NoCryLoadlord,//不叫地主
		en_RobLaodlord,	 //抢地主
		en_NoRobLaodlord,//不抢地主
		en_Double,		 //加倍
		en_NoDouble,	 //不加倍
		en_NoDenmand,    //要不起
		en_AppearCard	 //出牌
	};

	stPlayerData m_AppCardPlayer;//上一个玩家出牌的信息
	int			 m_iCountHandCard;//最少几手牌出完

	enState m_enState;//状态
	vector<enCard>   m_TempCard;
	vector<stSaveCardData> m_vSingle;		    //存放单牌
	vector<stSaveCardData> m_vTwain;		    //存放对子
	vector<stSaveCardData> m_vThreeBandOne;	    //存放三带一
	vector<stSaveCardData> m_vThreeBandTwo;		//存放三带二
	vector<stSaveCardData> m_vThreeBandZero;	//存放三带零
	vector<stSaveCardData> m_vContinuousTwain;  //存放连对
	vector<stSaveCardData> m_vBomb;				//存放炸弹
	vector<stSaveCardData> m_vWangBomb;			//存放王炸
	vector<stSaveCardData> m_vStraight;			//存放顺子
	vector<stSaveCardData> m_vAircraft;			//存放飞机
	vector<stSaveCardData> m_vFourBandTwo;		//存放四带二

	logic(enState state);
	~logic(void);

	//初始化
	void Init();

	//Ai
	virtual enAiResult Ai(enState state,//现在什么状态
						 vector<enCard> cardVect,//手里有的牌
					     vector<enCard>& ReturnVect,//要打出去的牌
						 enType LandlordNoCard,//地主不要的牌型
						 stPlayerData lastPlayer,//上一个玩家的信息
						 stPlayerData nowPlayer,//当前玩家的信息
						 stPlayerData nextPlayer,//下一个玩家的信息
						 int dir,//出牌玩家的方向
						 stResult result = stResult(TYPE_NULL,-1,0));//上一个玩家出的牌，默认没出牌

	//提示
	virtual vector<enCard> hint(vector<enCard> CardVector,//手里的牌
								stResult result,//上一手出的什么牌
								enType type,//地主 不要的牌
								stPlayerData lastPlayer,//上一个玩家的信息
								stPlayerData nowPlayer,//当前玩家的信息
								stPlayerData nextPlayer,//下一个玩家的信息
								int dir,//出牌玩家的方向
								bool flag);//是否是玩家提示
	//计算需要几手牌才能出完
	int countHandCard();

	//出任意牌
	vector<enCard> ChuRenYiPai(bool myLandlord,enType type,stPlayerData last,stPlayerData next,stResult result);

	//快速排序
	void sort_quick(vector<enCard>& vect,int b,int e);      

	//是否叫地主
	bool ifCallCard(vector<enCard>& CardVector);		

	//是否抢地主
	bool ifRobLandlord(vector<enCard>& CardVector);		

	//是否加倍
	int ifDouble(vector<enCard>& CardVector);		

	//判断一个下标是否在VECTOR中
	bool IsIndexInVector(int index,vector<int>& vect);	

	//判断牌是不是相等
	bool compareCard(enCard card1,enCard card2);	

	//牌转换为int
	int cardTransformInt(enCard card);			

	//判断是否是单牌
	stResult judgeSingle(vector<enCard>& CardVector);	

	//判断是否是对子
	stResult judgeTwain(vector<enCard>& CardVector);			

	//判断是否3带1
	stResult judgeThreeBandOne(vector<enCard>& CardVector);		

	//判断是否3带2
	stResult judgeThreeBandTwo(vector<enCard>& CardVector);		

	//判断是否3不带
	stResult judgeThreeBandZero(vector<enCard>& CardVector);	

	//判断是否连对
	stResult judgeContinuousTwain(vector<enCard>& CardVector);	

	//判断是否是炸弹
	stResult judgeBomb(vector<enCard>& CardVector);			

	//判断是不是王炸
	stResult judgeWangBomb(vector<enCard>& CardVector);		

	//判断是否顺子
	stResult judgeStraight(vector<enCard>& CardVector);			

	//判断是否是飞机
	stResult judgeAircraft(vector<enCard>& CardVector);			

	//判断是4带2
	stResult judgeFourBandTwo(vector<enCard>& CardVector);		

	//拆牌
	void unseamCard(vector<enCard>& CardVector);

	//拆牌
	//当xx牌无法出的时候，可以选择拆xxx牌来出
	vector<enCard> unseamCard(stResult result,//牌型
								int addNum,//需要的添加到Vect里牌的张数
								int cardCount,//vectSaveCard里的牌必须大于cardCount
								bool compare,//必须是相同数量
								bool end,//反向查找
								int jumpIndex,//下标跳跃
								int addJumpIndex,//添加的时候跳跃下标
								vector<stSaveCardData>& vectSaveCard);//牌

	//找出相等的牌
	bool FindCompareCard(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect,enType type);

	//查找单牌
	bool FindSingle(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect);	

	//查找对子
	bool FindTwain(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect);			

	//查找3带1
	bool FindThreeBandOne(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect);		

	//查找3带2
	bool FindThreeBandTwo(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect);		

	//查找3不带
	bool FindThreeBandZero(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect);	

	//查找连对
	bool FindContinuousTwain(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect);	

	//查找炸弹
	bool FindBomb(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect);			

	//查找王炸
	bool FindWangBomb(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect);		

	//查找顺子
	bool FindStraight(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect);			

	//查找飞机
	bool FindAircraft(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect);			

	//查找4带2
	bool FindFourBandTwo(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect);

	//出单牌
	vector<enCard> appearSingle(stResult result,bool team,bool genpai);	

	//出对子
	vector<enCard> appearTwain(stResult result,bool team,bool genpai);			

	//出3带1
	vector<enCard> appearThreeBandOne(stResult result,bool team,bool genpai);		

	//出3带2
	vector<enCard> appearThreeBandTwo(stResult result,bool team,bool genpai);		

	//出3不带
	vector<enCard> appearThreeBandZero(stResult result,bool team,bool genpai);	

	//出连对
	vector<enCard> appearContinuousTwain(stResult result,bool team,bool genpai);	

	//出炸弹
	vector<enCard> appearBomb(stResult result,bool team,bool genpai);			

	//出王炸
	vector<enCard> appearWangBomb(stResult result,bool team,bool genpai);		

	//出顺子
	vector<enCard> appearStraight(stResult result,bool team,bool genpai);			

	//出飞机
	vector<enCard> appearAircraft(stResult result,bool team,bool genpai);			

	//出4带2
	vector<enCard> appearFourBandTwo(stResult result,bool team,bool genpai);	
};

