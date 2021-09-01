//#include "PopupLayer.h"
//
//using namespace cocos2d;
//
//// 构架，并设置对话框背景图片
//PopupLayer* PopupLayer::create(const char* backgroundImage)
//{
//	PopupLayer* p	= PopupLayer::create();
//	p->m_BackSprite->setTexture(backgroundImage);
//	return p;
//}
////添加按钮
//bool PopupLayer::addButton(const char* pressImage,const char* normalImage,
//				  string text,Size contSize,int textSize,Color3B textColor,Point pos,Node* node,void (*callBack)())
//
//{
//	Button* button = Button::create();
//	button->loadTextureNormal(normalImage);
//	button->loadTexturePressed(pressImage);
//	button->setTitleText(text);
//	button->setTitleColor(textColor);
//	button->setTitleFontSize(textSize);
//	button->setContentSize(contSize);
//	button->setScale9Enabled(true);
//	button->setPressedActionEnabled(true);
//	button->setZoomScale(0.1);
//	button->setPosition(pos);
//	button->addClickEventListener(CC_CALLBACK_1(callBack,node));
//	addChild(button);
//
//	return true;
//}
//
////添加文本
//bool PopupLayer::addMenu(Node* node)
//{
//	return false;
//}
//
//bool PopupLayer::init()
//{
//	m_BackSprite = Sprite::create();
//	addChild(m_BackSprite);
//}
//void PopupLayer::onEnter()
//{
//
//}
//void PopupLayer::onExit()
//{
//
//}
//void PopupLayer::update(float fd)
//{
//
//}
//void PopupLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
//{
//
//}
//void PopupLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
//{
//
//}
//void PopupLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
//{
//
//}