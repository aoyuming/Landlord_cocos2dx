#pragma once
#include <hash_map>
#include <string>
#include <io.h>

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "iconv.h"

#define LIAOTIAN_MUSIC		"sound//Man_Chat_0.mp3"		//���� ��10��
#define JIAODIZHU_MUSIC		"sound//Man_Order.mp3"		//�е���
#define BUJIAO_MUSIC		"sound//Man_NoOrder.mp3"	//���е���
#define QIANGDIZHU_MUSIC	"sound//Man_Rob1.mp3"		//������ ��3��
#define BUQIANG_MUSIC		"sound//Man_NoRob.mp3"		//����
#define BUYAO_MUSIC			"sound//Man_buyao1.mp3"		//��Ҫ ��4��
#define JIABEI_MUSIC		"sound//Man_jiabei.mp3"		//�ӱ�
#define BUJIABEI_MUSIC		"sound//Man_bujiabei.mp3"	//���ӱ�
#define SANDAIYI_MUSIC		"sound//Man_sandaiyi.mp3"   //����һ					
#define SANDAIYIDUI_MUSIC	"sound//Man_sandaiyidui.mp3"//����һ��
#define SANBUDAI_MUSIC		"sound//Man_tuple1.mp3"     //������ �� A��ʼ 	��K	
#define SIDAIER_MUSIC		"sound//Man_sidaier.mp3"     //�Ĵ�2
#define SIDAILIANGDUI_MUSIC	"sound//Man_sidailiangdui.mp3"//�Ĵ�2��
#define DANPAI_MUSIC		"sound//Man_1.mp3"          //����   �� A��ʼ 	������     14С�� 15����	
#define YIDUI_MUSIC			"sound//Man_dui1.mp3"       //һ��   �� A��ʼ 	��K	
#define FEIJI_MUSIC         "sound//Man_feiji.mp3"		//�ɻ�
#define SHUNZI_MUSIC        "sound//Man_shunzi.mp3"		//˳��
#define LIANDUI_MUSIC		"sound//Man_liandui.mp3"	//����
#define ZHADAN_MUSIC		"sound//Man_zhadan.mp3"		//ը��
#define WANGZHA_MUSIC		"sound//Man_wangzha.mp3"	//��ը
#define MINGPAI_MUSIC		"sound//Man_Share.mp3"		//����
#define FAPAI_MUSIC			"sound//SpecSelectCard.mp3" //���Ƶ�����
#define BAOJING1_MUSICE     "sound//Man_baojing1.mp3"   //1���Ʊ���
#define BAOJING2_MUSICE     "sound//Man_baojing2.mp3"   //2���Ʊ���
#define DANI1_MUSICE		"sound//Man_dani1.mp3"		//������3��

#define BGM_WELCOME_MUSICE	  "sound//MusicEx_Welcome.mp3"	//�ս���Ϸ��BGM
#define BGM_ZHENGCHANG_MUSICE "sound//MusicEx_Normal.mp3"	//������BGM
#define BGM_ZHADAN_MUSICE	  "sound//MusicEx_Exciting.mp3"	//ը�����BGM
#define BGM_WIN_MUSICE		  "sound//MusicEx_Win.mp3"	    //Ӯ���BGM
#define BGM_LOSE_MUSICE       "sound//MusicEx_Lose.mp3"	    //����BGM



 
USING_NS_CC;
using namespace std;
using namespace CocosDenshion;

//���η�Χ
struct ME_Rect
{
	int left;
	int right;
	int top;
	int bottom;
};

//����һ���ļ������Ե������ļ�
int LoadDirectory(const char* fn);
//������ײ
bool MyIntersectRect(ME_Rect* rect1,ME_Rect* rect2);
//�жϵ��Ƿ��ھ���
bool MyPtInRect(ME_Rect* rect,int x,int y);
//����ת��
int GBKToUTF8(std::string &gbkStr,const char* toCode,const char* formCode);

