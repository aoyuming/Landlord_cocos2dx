#pragma once
/*
	��ͷ�ļ��Ƕ��������߼��࣬��������ݲ�����ƽ̨��ص�
	��������ƽ̨������ʱ�����ֱ�Ӽ̳�����ʹ��
											2016.11.7
									

										Ai�㷨


һ������ 
        1 �������С����һ������ͣ���˫���ƣ����������ʲô���Ͷ����Դ� 
        2 ը������ͬ��������������һ������ͣ���������A��������⣬�����Դ��κ����ͣ�ը����ը��ʱ��Ҫ�ȴ�С�� 
        3 ��֧��һ���ƣ��������ƣ���һ֧3�� 
        4 ���ӣ�һ���ƣ�����ͬ��������������һ������ͣ�����55�� 
        5 ��������ͬ��������������һ������ͣ���������4�� 
        6 ����һ�֣����� �� һ���Ƶ����ͣ�����AAA+9��AAA+77�� 
        7 ��˳�����Ż�����������֧����ɵ����ͣ�����45678��345678910JQKA��2�ʹ�С������������ 
        8 ˫˳�����Ի���������������ɵ����ͣ�����334455��445566778899��2�ʹ�С������������ 
        9 ��˳����������������������ɵ����ͣ�����777888��444555666777��2�ʹ�С������������ 
        10 �ɻ��������˳ �� ͬ������һ���ƣ�����777888+3+6��444555666+33+77+88�� 
		11 �Ĵ���������+�����ơ�����AAAA+7+9��9999+33+55�� 
�������ͷ��� 
		1 ��˳��ȷ�� 
        a) ѡȡ��������ȡ����С��һ������������ʣ�������ȡ����С��һ���������������ƣ�ֱ��û������Ϊֹ�� 
        b) ��չ��������ʣ��������Ѿ�ȡ�����ƽ��бȶԣ����ĳ��ʣ���������֪����������ɸ�������ƣ�����ϲ���һֱ���޷��ϲ�Ϊֹ�� 
        c) �ϲ����ƣ����ĳ�����������޷����ӳɸ�������ƣ�����ϲ���һ�顣 
        ��������ѡȡ����չ�ͺϲ�����һ�����е�����������ȡ�����ˣ��������£� 
        �ٶ�һ�����ǣ�2AKQJ1099877766543 
        ��һ����ѡȡ��34567��678910���������顣ʣ����ƻ���79JQKA2 
        �ڶ�����ʣ���JQKA�ܺ�678910����µ�����678910JQKA�� 
        ����������֪�����������鲻�ܺϲ����µġ�����������飬�������ƾͱ��ֳ���34567��678910JQKA�����������7��9��2���ŵ��ơ� 
 
2 ˫˳��ȷ�� 
        ��һ���������еĶ��Ӽ�������Ȼ�󽫶������򣬰��ռ�������ķ�ʽ���Խ����е�˫˳����ȡ������ 
 
3 ������ȷ�� 
        һ�����е������������ܱȽϷ������ȡ������ 
 
4 ��˳��ȷ�� 
        �������Ļ����Ͻ��бȽϣ����������������������ϲ��ɣ������ܴ�ģ���˳�� 
 
5 ը����ȷ�� 
        ���е���ͷ���ܷܺ����ȷ���� 
 
6 �����ȷ�� 
        ֻҪ���Ű�����С�����ͽ�����ɻ���� 
 
7 ���ŵķ��෽�� 
        a) ��ȷ��������ж��Ƿ��д�С���� 
        b) ��ȷ��ը���������Ƿ�����ͷ�� 
        c) ��ȷ���������ڳ���ը������������ж��Ƿ���������� 
        d) ��ȷ����˳�����Ѿ�ȷ�����������ж��Ƿ�������ڵ�����������У����������˳��ע�⣬Ӧ��ʹ��˳�����������ܴ󡣼������444555666������ϳ�һ����˳�������Ƿֳ�444555һ����˳��666һ�������� 
        e) ��ȷ����˳���жϵ�˳ʱ����ȥ���ĸ�2���������ը���������ж��Ƿ���ڳ��������ƣ������������ָ���е���������������ƣ�����У�������ȡ�������� �Σ���ʣ�������ÿһ����������������˳����������ϣ�����ܹ�������ɵ�˳�Ͷ��ӣ���ԭ�е�����ȡ����������ϳ����ƺ͵�˳������4566678���� ���45678��66������󣬽���֪�����ơ���������������˳����ʣ�µ�����������ϣ���������45678����֪�����ƣ�999��������10J���� �ƣ���������ϳ�45678910J��99��ͨ�����ϵķ��������ܽ����ƺ������ܺõ�������ϡ� 
        f) ��ȷ��˫˳�����ȣ��������˳����ȫ�غϣ�����������ϳ�˫˳����Σ��ڳ�ը������˳����������˳��������м���Ƿ����˫˳������У�������ȡ������ 
        g) ��ȷ�����ӣ���ը������˳�����������ơ�˫˳��������м���Ƿ���ڶ��ӣ�������ڽ�����ȡ������ 
        h) ��ȷ�����ƣ�����ը������˳�����������ơ�˫˳������������������Ŷ��ǵ��ơ� 
 
8 ���ӵ�ȷ�� 
        �μ���һ�ڵġ���ȷ�����ӡ��� 
 
9 ���Ƶ�ȷ�� 
        �μ���һ�ڵġ���ȷ�����ơ��� 
 
����һ���Ƶ��������� 
        Ϊ ��ʹ��������������һ�������ܣ������һ���Ƶ��������з�������νһ���Ƶ�������ָ������û�б���ѹ�Ƶ��������Ҫ�������ܽ��Ƴ��ꡣ��������ھ������� �ķ�����ǿ���ȷ���ġ����������һ����22AAKQJ9987776654Ϊ���������ư���ǰ��ķ����������Է��������µ����ͣ� 
        22��AA��K��Q��J��987654��9��77��6������ 
        ͨ�����������͵ļ��㣬�������׵ó���������Ҫ9�ֲ��ܳ��꣬��������Ƶ���������9�� 
 
�ġ�������������������ĸ��� 
        ��Ϊ�ڶ�������Ϸ�д��ڻ����ը���Ŀ��ܣ�����������ֻ�л���Ų����ܱ�����ѹס�����ԣ����Բ�������������ĸ���Ժ󶼼�������� 
 
�塢����ԭ����� 
        ��Ϊ�ڶ������У������ը��������2������Ϊ�Ǵ��ƣ����Խ�����Ҫ������Щ�ƵĶ������жϡ�������һ���򵥵�ԭ�� 
        �ٶ����Ϊ8�֣�ը��Ϊ6�֣�����4�֣�С��3�֣�һ��2Ϊ2�֣��򵱷��� 
        ���ڵ���7��ʱ�������� 
        ���ڵ���5��ʱ�ж����� 
        ���ڵ���3��ʱ��һ���� 
        С�����ֲ��С� 
 
�������Ƶ�һ��ԭ�� 
        1 ���Ƶ�ԭ��һ�㰴�մ�С�����ԭ�򣬼����ȳ�������С�Ƶ���ϣ����ơ����ӡ�˫˳�����ơ���˳�������ȣ�ը����������������ڣ��� 
        2 �����ĳ���ԭ����Ϊ�������ƿ��Դ�һ�ŵ��ƻ�һ�����ӣ������ڳ�����ʱ��Ҫ����Ƿ��е��ƣ�����У����һ����С�ĵ��ƣ����û�У����ټ���Ƿ���ڶ��ӣ�����У����һ����С�Ķ��ӣ�������ƺͶ��Ӷ�û�У���������� 
        �ڴ���ʱ��������ֻʣ�����ƣ������ܴ�����2�� 
        3 ��˳�ĳ���ԭ����Ϊ��˳���ƿ��Դ����ţ�����ࣩ���ƻ�����������ࣩ���ӣ������������һ������Ҫ����Ƿ��е��ƻ���ӡ�������㹻��ĵ��ƻ���ӣ� �������������е��ƣ���û���㹻��ĵ��ƣ������Ƿ���6�����ϵ����ƣ�����н����Ƶ���С�����������ƴ���������ж��ӣ���û���㹻��Ķ��ӣ���� ���Ƿ���4�����ϵ�˫˳������н�˫˳����С���ӵ������Ӵ����� 
        �ڴ���ʱ��������ֻʣ�����ƣ������ܴ�����2�� 
        4 ���Ƶĳ���ԭ��ֱ�ӳ��� 
        5 ˫˳�ĳ���ԭ��ֱ�ӳ��� 
        6 ���ӵĳ���ԭ����Ϊ���ӿ�������������˳�ȴ����������ڳ�����ʱ��Ӧ���ȼ��һ����������˳��������������������������������� <= ���ӣ����������ܺͣ�2ʱ�������ӣ����������2�ȵȡ� 
        7 ���Ƶĳ���ԭ����Ϊ���ƿ�������������˳�ȴ����������ڳ�����ʱ��Ӧ���ȼ��һ����������˳��������������������������������� <= ���ӣ����������ܺͣ�2ʱ�������ƣ����������1�ȵȡ� 
 
�ߡ����Ƶ�һ��ԭ�� 
        1 ��������ж����ģ�����������һ�����͵���ʱ���ȸ�֮�� 
        2 2������Ϊ���ơ����ӡ���������ʽ������ 
        3 ������û����Ӧ�Ƹ�ʱ������Ǳ�����Ա�����ƣ����Բ���������ǶԷ������ƣ��������Ƹ��������û�У���ը������������PASS�� 
        4 ������е��Ƴ���ը��������⻹ʣһ���ƣ������������������ȸ�֮������ը֮�� 
        5 ���Ƶĸ���ԭ����������е��ƣ����֮�������2��֮���������Ƹ�֮�������6�����ϵĵ�˳���Ÿ�֮�������������֮���������˳��֮�������5����˳��֮�������˫˳��֮������ը֮������PASS�� 
        6 ���Ƶĸ���ԭ����������ж��ӣ����֮�������4�����ϵ�˫˳���Ÿ�֮�������������֮�������˫˳��֮���������˳��֮������ը֮������PASS�� 
        7 ����������1������2���Ƶĸ���ԭ�������������ͬ���͵������֮���������˳��֮������ը֮������PASS��ע�⣬ֻ�����������ڳ����Ժ�ʣһ����ʱ����ֱ�ӳ��������£������������2�� 
        ��û���㹻�ƴ�������£����յ��ơ����ӵĲ���ԭ����в��ƴ��� 
        8 ��˳����˳���Ƶĸ���ԭ���������Ӧ�����ͣ����֮��������Խ������˳���С����˳��֮������ը֮������PASS��ע�⣬ֻ�����������ڳ����Ժ�ʣһ����ʱ����ֱ�ӳ��������£������������2�� 
        ��û���㹻�ƴ�������£����յ��ơ����ӵĲ���ԭ����в��ƴ��� 
        9 ���Ƶĸ���ԭ���������Ӧ�����ͣ����֮���������ͬ������˫˳���������ͬ��������˳�������ͬ���������ƣ������ͬ������˫˳�������ͬ��������˳������ը֮������PASS�� 
        10 ˫˳�ĸ���ԭ������ͬ���͵��ƣ����֮�������ͬ������˫˳�������ͬ��������˳���������ͬ��������˳������ը֮������PASS�� 
        11 ը���ĸ���ԭ���г�������ը����ը�������л������ը֮������PASS�� 
        12 ը���������Ƶĸ���ԭ�������ը������ը֮������PASS�� 
 
�ˡ�����ԭ����� 
        1 ��ׯ�򷨣���Ϊ��ׯ��ֻ���Լ�һ���ˣ�������������⣬����һ�㰴��ǰ���ԭ����Ƽ��ɡ� 
            a) �ڳ���ʱ�����ƫ����һ����ֻʣһ����ʱ�������������ƣ��������ɴ�С���� 
            b) �ڸ���ʱ�����ƫ����һ����ֻʣһ����ʱ�������������ơ� 
        2 ƫ�Ҵ򷨣�ƫ����Ϊǣ�浽������⣬���Դ���һЩ��ͬ�� 
            a) �ڳ���ʱ�������ׯ�ҵ��ϼң���ׯ��ֻʣһ����ʱ�������������ƣ��������ɴ�С���� 
            b) �ڸ���ʱ�������ׯ�ҵ��ϼң���ׯ��ֻʣһ����ʱ�������������ơ� 
            c) ��һ��ƫ�Ҵ�����ǵ���ʱ��һ��������ܸ��͸���������б����2�����ϵ���ʱ��ѡ��PASS�� 
            d) ��һ��ƫ�Ҵ�����Ƕ���ʱ��һ��������ܸ��͸���������б����AA�����ϵ���ʱ��ѡ��PASS�� 
            e) ���һ��ƫ�Ҵ�������ǳ��˵��Ƽ�������������ͣ���ѡ��PASS�� 
            f) ��������¼ҵ�ƫ��ֻʣһ����ʱ���ڳ���ʱ��������С���ơ����ƻ�����һ���ԭ�� 
*/


#include <vector>
#include <list>
using namespace std;

//ֽ��		ǿ��ת��Ϊint * ͼƬ���������ͼƬ���x
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
	//������С��
	Little_King,
	Big_King
};

//���Ƶ�����
enum enType 
{
	TYPE_NULL,//��
	TYPE_DANPAI,//����
	TYPE_DUIZI,//����
	TYPE_3BUDAI,//3����
	TYPE_ZHADAN,//ը��
	TYPE_3DAI1,//3��1
	TYPE_3DAI2,//3��һ��
	TYPE_LIANDUI,//����
	TYPE_WANGZHA,//��ը
	TYPE_SHUNZI,//˳��
	TYPE_FEIJI,//�ɻ�
	TYPE_SIDAI2//4��2
};

#define VALUE_MAX 10000 //������͵�ֵ

//���
struct stResult
{
	stResult(){}
	stResult(enType ty,int va,int ct)
	{
		type = ty;
		value = va;
		conut = ct;
	}
	//�ж��Ƿ����
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

	enType type;//���Ƶ�����
	int value;//���Ƶ�ֵ
	int conut;//���Ƶ�����

	//ֵ�ļ���
	//���ƣ����ر����Ƶ�ֵ / 4  �����ټ�2 С���ټ�1
	//���ӣ����ر����Ƶ�ֵ / 4
	//���ԣ�������С���Ƶ�ֵ / 4
	//˳�ӣ�������С���Ƶ�ֵ / 4
	//ը�������ص�һ���Ƶ�ֵ / 4
	//�ɻ���������С���Ƶ�ֵ / 4
	//��ը������ VALUE_MAX
	//����һ�� ���ص�һ���Ƶ�ֵ / 4
	//�������� ���ص�һ���Ƶ�ֵ / 4
	//�������� ���ص�һ���Ƶ�ֵ / 4
};

//��һ�����Ϣ
struct stPlayerData
{
	int dir;//���� 0�� 1�� 2��
	bool landlord;//�Ƿ��ǵ���
	int cardCount;//ʣ�µ��Ƶ�����
};

//�Ƶ���������Ϣ
struct stSaveCardData
{
	stResult result;//�Ƶ���Ϣ
	vector<enCard> vect;//��
};

//�߼�
class logic
{
public:

	//״̬
	enum enState
	{
		en_Wait,	  //�ȴ�
		en_JiaoDiZhu, //�е���
		en_QiangDiZhu,//������
		en_JiaBei,	  //�ӱ�
		en_ChuPai,	  //��������
		en_GenPai	  //�����ָ����������
	};

	//Ai���ؽ��
	enum enAiResult
	{
		en_AiWait,		 //�ȴ�
		en_CryLoadlord,	 //�е���
		en_NoCryLoadlord,//���е���
		en_RobLaodlord,	 //������
		en_NoRobLaodlord,//��������
		en_Double,		 //�ӱ�
		en_NoDouble,	 //���ӱ�
		en_NoDenmand,    //Ҫ����
		en_AppearCard	 //����
	};

	stPlayerData m_AppCardPlayer;//��һ����ҳ��Ƶ���Ϣ
	int			 m_iCountHandCard;//���ټ����Ƴ���

	enState m_enState;//״̬
	vector<enCard>   m_TempCard;
	vector<stSaveCardData> m_vSingle;		    //��ŵ���
	vector<stSaveCardData> m_vTwain;		    //��Ŷ���
	vector<stSaveCardData> m_vThreeBandOne;	    //�������һ
	vector<stSaveCardData> m_vThreeBandTwo;		//���������
	vector<stSaveCardData> m_vThreeBandZero;	//���������
	vector<stSaveCardData> m_vContinuousTwain;  //�������
	vector<stSaveCardData> m_vBomb;				//���ը��
	vector<stSaveCardData> m_vWangBomb;			//�����ը
	vector<stSaveCardData> m_vStraight;			//���˳��
	vector<stSaveCardData> m_vAircraft;			//��ŷɻ�
	vector<stSaveCardData> m_vFourBandTwo;		//����Ĵ���

	logic(enState state);
	~logic(void);

	//��ʼ��
	void Init();

	//Ai
	virtual enAiResult Ai(enState state,//����ʲô״̬
						 vector<enCard> cardVect,//�����е���
					     vector<enCard>& ReturnVect,//Ҫ���ȥ����
						 enType LandlordNoCard,//������Ҫ������
						 stPlayerData lastPlayer,//��һ����ҵ���Ϣ
						 stPlayerData nowPlayer,//��ǰ��ҵ���Ϣ
						 stPlayerData nextPlayer,//��һ����ҵ���Ϣ
						 int dir,//������ҵķ���
						 stResult result = stResult(TYPE_NULL,-1,0));//��һ����ҳ����ƣ�Ĭ��û����

	//��ʾ
	virtual vector<enCard> hint(vector<enCard> CardVector,//�������
								stResult result,//��һ�ֳ���ʲô��
								enType type,//���� ��Ҫ����
								stPlayerData lastPlayer,//��һ����ҵ���Ϣ
								stPlayerData nowPlayer,//��ǰ��ҵ���Ϣ
								stPlayerData nextPlayer,//��һ����ҵ���Ϣ
								int dir,//������ҵķ���
								bool flag);//�Ƿ��������ʾ
	//������Ҫ�����Ʋ��ܳ���
	int countHandCard();

	//��������
	vector<enCard> ChuRenYiPai(bool myLandlord,enType type,stPlayerData last,stPlayerData next,stResult result);

	//��������
	void sort_quick(vector<enCard>& vect,int b,int e);      

	//�Ƿ�е���
	bool ifCallCard(vector<enCard>& CardVector);		

	//�Ƿ�������
	bool ifRobLandlord(vector<enCard>& CardVector);		

	//�Ƿ�ӱ�
	int ifDouble(vector<enCard>& CardVector);		

	//�ж�һ���±��Ƿ���VECTOR��
	bool IsIndexInVector(int index,vector<int>& vect);	

	//�ж����ǲ������
	bool compareCard(enCard card1,enCard card2);	

	//��ת��Ϊint
	int cardTransformInt(enCard card);			

	//�ж��Ƿ��ǵ���
	stResult judgeSingle(vector<enCard>& CardVector);	

	//�ж��Ƿ��Ƕ���
	stResult judgeTwain(vector<enCard>& CardVector);			

	//�ж��Ƿ�3��1
	stResult judgeThreeBandOne(vector<enCard>& CardVector);		

	//�ж��Ƿ�3��2
	stResult judgeThreeBandTwo(vector<enCard>& CardVector);		

	//�ж��Ƿ�3����
	stResult judgeThreeBandZero(vector<enCard>& CardVector);	

	//�ж��Ƿ�����
	stResult judgeContinuousTwain(vector<enCard>& CardVector);	

	//�ж��Ƿ���ը��
	stResult judgeBomb(vector<enCard>& CardVector);			

	//�ж��ǲ�����ը
	stResult judgeWangBomb(vector<enCard>& CardVector);		

	//�ж��Ƿ�˳��
	stResult judgeStraight(vector<enCard>& CardVector);			

	//�ж��Ƿ��Ƿɻ�
	stResult judgeAircraft(vector<enCard>& CardVector);			

	//�ж���4��2
	stResult judgeFourBandTwo(vector<enCard>& CardVector);		

	//����
	void unseamCard(vector<enCard>& CardVector);

	//����
	//��xx���޷�����ʱ�򣬿���ѡ���xxx������
	vector<enCard> unseamCard(stResult result,//����
								int addNum,//��Ҫ����ӵ�Vect���Ƶ�����
								int cardCount,//vectSaveCard����Ʊ������cardCount
								bool compare,//��������ͬ����
								bool end,//�������
								int jumpIndex,//�±���Ծ
								int addJumpIndex,//��ӵ�ʱ����Ծ�±�
								vector<stSaveCardData>& vectSaveCard);//��

	//�ҳ���ȵ���
	bool FindCompareCard(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect,enType type);

	//���ҵ���
	bool FindSingle(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect);	

	//���Ҷ���
	bool FindTwain(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect);			

	//����3��1
	bool FindThreeBandOne(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect);		

	//����3��2
	bool FindThreeBandTwo(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect);		

	//����3����
	bool FindThreeBandZero(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect);	

	//��������
	bool FindContinuousTwain(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect);	

	//����ը��
	bool FindBomb(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect);			

	//������ը
	bool FindWangBomb(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect);		

	//����˳��
	bool FindStraight(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect);			

	//���ҷɻ�
	bool FindAircraft(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect);			

	//����4��2
	bool FindFourBandTwo(vector<enCard>& CardVector,vector<stSaveCardData>& saveCardVect);

	//������
	vector<enCard> appearSingle(stResult result,bool team,bool genpai);	

	//������
	vector<enCard> appearTwain(stResult result,bool team,bool genpai);			

	//��3��1
	vector<enCard> appearThreeBandOne(stResult result,bool team,bool genpai);		

	//��3��2
	vector<enCard> appearThreeBandTwo(stResult result,bool team,bool genpai);		

	//��3����
	vector<enCard> appearThreeBandZero(stResult result,bool team,bool genpai);	

	//������
	vector<enCard> appearContinuousTwain(stResult result,bool team,bool genpai);	

	//��ը��
	vector<enCard> appearBomb(stResult result,bool team,bool genpai);			

	//����ը
	vector<enCard> appearWangBomb(stResult result,bool team,bool genpai);		

	//��˳��
	vector<enCard> appearStraight(stResult result,bool team,bool genpai);			

	//���ɻ�
	vector<enCard> appearAircraft(stResult result,bool team,bool genpai);			

	//��4��2
	vector<enCard> appearFourBandTwo(stResult result,bool team,bool genpai);	
};

