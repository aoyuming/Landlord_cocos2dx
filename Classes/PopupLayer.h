//
//#ifndef __PopupLayer__
//#define __PopupLayer__
//
//#include "cocos2d.h"
//#include "ui\CocosGUI.h"
//#include <string>
//
//using namespace std;
//using namespace cocos2d;
//using namespace cocos2d::ui;
//
//
////ģ��Ի���
//class PopupLayer : public Layer
//{
//
//public:
//
//
//
//	virtual bool init();
//	virtual void onEnter();
//	virtual void onExit();
//	virtual void update(float fd);
//
//    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
//    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
//    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
//
//	// ���ܣ������öԻ��򱳾�ͼƬ
//	static PopupLayer* create(const char* backgroundImage);     
//
//	//��Ӱ�ť
//	bool addButton(const char* pressImage,const char* normalImage,
//				  string text,Size contSize,int textSize,Color3B textColor,Point pos,Node* node,void (*callBack)());
//
//	//����ı�
//	bool addMenu(Node* node);
//	CREATE_FUNC(PopupLayer);
//
//private:
//
//	Sprite* m_BackSprite;
//	Button* m_ColseButton;
//	
//};
//
//
//
//#endif /* defined(__PopupDemo__PopupLayer__) */