#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include "PopupLayer.h"
#include "cocos2d.h"
#include "player.h"
#include "public.h"
#include <time.h>
#include "ui\CocosGUI.h"
#include "GameLogic.h"

#define PLAYER_DOWN  0 //�·������������±�
#define PLAYER_RIGHT 1 //�ҷ������������±�
#define PLAYER_LEFT  2 //�������������±�

//using namespace CocosDenshion;
using namespace std;
using namespace cocos2d::ui;




USING_NS_CC;
class HelloWorld : public cocos2d::Layer
{

public:

	vector<CPlayer*> m_vPlayer;
	vector<Point> m_PressVector;

	//�ܹ�����
	vector<enCard> m_AllCardVect;
	//����
	vector<CPlayer::stCardSpr> m_vBackCard;

	CGameLogic* m_pLogic;//�߼�
	stResult m_TempResult;//���

	CPlayer::enDir m_CryDir;//��¼��ǰ�е��������ĸ������
	CPlayer::enDir m_RobDir;//��¼��ǰ�����������ĸ������

	//�Ƿ���
	bool m_bDeal;
	int m_iQiangDiZhu; //������������

	//���µĵ� ���ƶ��ĵ�
	Point m_PressLeftPoint;
	Point m_PressRightPoint;
	Point m_PressPoint;
	Point m_MovePoint;
	Point m_EndPoint;
	bool  m_bDownAi;
	bool  m_bTouchPress;
	bool  m_bTouchMove;
	bool  m_bTouchEnd;
	bool  m_bFlags;


	Button* m_ButtonChuPai;//����
	Button* m_ButtonBuYao;//��Ҫ
	Button* m_ButtonTiShi;//��ʾ
	Button* m_ButtonOneScore;//1��
	Button* m_ButtonTwoScore;//2��
	Button* m_ButtonThreeScore;//3��
	Button* m_ButtonCryLandlord;//�е���
	Button* m_ButtonNoCryLandlord;//����
	Button* m_ButtonCobLandlord;//������
	Button* m_ButtonNoCobLandlord;//����
	Button* m_ButtonDouble;//�ӱ�
	Button* m_ButtonNoDouble;//���ӱ�
	Button* m_ButtonAnew;//����


	
	//����滻��
	void tihuanCard(CPlayer* player);

	//��ʾ����
	void showBackCard(vector<CPlayer::stCardSpr>& vect,bool show);

	//һ�����·���
	void anewSendCard();

	//���õ���
	void setBaseCard(vector<enCard>& vect);

	//����
	void deal(float dt);

	//���ر���ͼƬ
	void loadBack(const char* fn);

	//��ť ���� ��Ҫ ��ʾ �Ļص�����
	void ButtonClick_ChuPai(Ref* node);
	void ButtonClick_BuYao(Ref* node);
	void ButtonClick_TiShi(Ref* node);

	//��ť �е��� ���� ������ �Ļص�����
	void ButtonClick_CryLandlord(Ref* node);
	void ButtonClick_NoCryLandlord(Ref* node);
	void ButtonClick_CobLandlord(Ref* node);

	//��ť ���� �ӱ� ���ӱ�  �Ļص�����
	void ButtonClick_NoCobLandlord(Ref* node);
	void ButtonClick_Double(Ref* node);
	void ButtonClick_NoDouble(Ref* node);

	void ButtonClick_Anew(Ref* node);

	//���ð�ťλ��
	void setButtonPos(Button* button,int i,int j);

	//���ð�ť�Ļص�����
	void setButtonCallBcak(Button* button,int i,int j);

	//���ð�ťVisible
	void setButtonVisible(CPlayer::enState state);

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	virtual void onEnter();
	virtual void onExit();
	virtual void update(float fd);

    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
