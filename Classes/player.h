#pragma once 
#include "logic.h"
#include <string>
#include "cocos2d.h"
#include "public.h"
#include "timer.h"


USING_NS_CC;

#define DOWN_Y     40	//���·���ҵ��Ƶ�Y
#define SHOWCARD_Y 240  //���ȥ���Ƶ�Y


//���
class CPlayer : public logic
{
public:

	//���еķ���
	enum enDir
	{
		DOWN,
		RIGTH,
		LEFT,
		_NULL
	};

	//�Ƶ���Ϣ
	struct stCardSpr
	{
		bool opt;			   //�Ƿ�ѡ��
		bool popup;			   //�Ƿ񵯳�
		enCard card;		   //��
		Sprite* sprite;		   //ͼƬ
		LayerColor* layerColor;//��ѡ��ʱ��ӵİ�͸����
	};

	Node* m_Praent;			//���ڵ�
	enDir m_Dir;			//�Լ������ķ���
	string m_sName;			//����
	string m_sUserId;		//�˺�
	string m_sPassWord;		//����
	Sprite* m_SpriteFigure;	//��������
	Sprite* m_SpriteJoyBean;//���ֶ�ͼƬ
	Label* m_Label;			//���� ��ʾ �е��� ���� ������ ���� �ӱ� ���ӱ�
	Label* m_LabelName;		//����
	Label* m_LabelJoyBean;	//���ֶ�
	Label* m_LabelCardCount;//��ʣ������
	Label* m_LabelUpString; //ͷ�ϵ����壬 ������ʾ�ǵ�������ũ��

	stResult m_Result;//���ƵĽ��
	CTimer*  m_Timer;//��ʱ��

	float  m_fTime;		  //��ʱ
	int m_iJoyBean;		  //���ֶ�
	bool m_bShowCard;     //�Ƿ�����
	bool m_bLandlord;	  //�Ƿ��ǵ���
	bool m_bSex;		  //�Ա� trueΪ�� falseΪŮ
	bool m_bCardUpdate;   //�Ʒ����˸���
	bool m_bMoveCard;	  //�ƶ���
	bool m_bNoCryLoadlord;//�Ƿ񲻽е���

	static Sprite* m_SpriteMultiple;//����ͼƬ
	static Label* m_LabelMultiple;  //����
	static int m_iMultiple;		    //����
	static int m_iRobLoadlordLevel; //�������������ȼ�
	static int m_iFollowCardLevel;  //���Ƶ������ȼ�
	static int m_iNoAskLevel;       //Ҫ���������ȼ�
	static int m_iCardGreaterLevel; //���������ȼ�
	static enType m_LandlordNoCryCard; //������Ҫ����
	

	vector<stCardSpr*> m_vCardVect;		  //��¼�ܹ��е�ֽ��
	vector<stCardSpr*> m_vDeleteCardVect; //��¼Ҫdelete����ֽ��
	vector<stCardSpr*> m_vMoveVect;		  //��¼Ҫ�ƶ�����
	vector<stCardSpr*> m_vSendVect;		  //��¼Ҫ�������
	vector<stCardSpr*> m_vShowSendVect;   //��ʾҪ���ȥ����

	CPlayer(string userId,string passWord,string name,enDir dir,enState state,int integral,int multiple,bool sex,Node* parent);
	~CPlayer();

	//����ȫ��������ʾ���
	void setAllLabelHide(bool show);

	//����
	enAiResult Run();
	
	//����
	void showCard(bool sound);

	//��ʱ������֮��Ļص�����
	void TimerCallBackFun();

	//��ʼ��ʱ
	void startTimer(int timeMax);

	//ֹͣ��ʱ
	void stopTimer();

	//�����ȼ���ʼ��
	static void SoundLevelInit();  

	//��ȡ���ƵĽ��
	stResult getResult();

	//Ai
	enAiResult Ai(stPlayerData lastPlayer,stPlayerData nowPlayer,stPlayerData nextPlayer,
					int dir,//������ҵķ���
					stResult result = stResult(TYPE_NULL,-1,0));//��һ����ҳ����ƣ�Ĭ��û��			

	//�����Լ�ͷ�ϵ��֣��ǵ�������ũ��
	void setUpString(string s);

	//���
	void clear();			

	//��Ҫ
	void noDenmand();		

	//�ӱ�
	void Double(bool dou);		

	//������ȫ�����Գ�ʼ��
	void setCardInit();		

	//�����Ƶİ�͸�����ʼ��
	void setCardLayerInit();	

	//�ƶ���ѡ�е���
	void moveOptCard();			

	//������
	void RobLoadlord(bool rob);		

	//�е���
	void CryLoadlord(bool cry);	

	//�Ƶ�λ�ø���
	void cardPosUpdate(enDir dir);	

	//����һ�������Ƶ����X��X+W
	bool getCardPoint(Point p,Point& left,Point& rigth);	

	//����
	bool appearCard(stResult result);	

	//����
	void chupai(string sound,int SoundIndex = 0);	

	//��������λ��
	void setTextPos(Label* label,enDir dir);	

	//����
	void my_Sort(vector<stCardSpr*>& vect);		

	//�е�����תŮ������
	void manTurnWomanSound(string& sound);		

	//����Ϊ����
	void setLandlord(const vector<stCardSpr>& vect);

	//�Ű�
	void arrange(vector<stCardSpr*>& vect,enDir dir);

	//���Ҫ������Ƶ�M_SendVect
	void addSendCardVect(vector<stCardSpr*>& vect );	

	//��ʾ
	bool hint(stResult result,stPlayerData lastPlayer,stPlayerData nowPlayer,stPlayerData nextPlayer,int dir,bool flag);

	//�ж�vector�������ʲô�Ʒ���type_null�������ʹ���
	stResult ifVectCardType(vector<stCardSpr*>& vect );	

	//Ϊ2��֮��������͸����
	void addInTowPointCardLayer(Point begin,Point end);	

	//�����Ʋ���ʾ
	void setVectorCardHide(vector<stCardSpr*>& vect,bool hide);	

	//���ô��ȥ���Ƶ�λ��
	void setShowCardPos(vector<stCardSpr*>& vect,enDir dir);	

};