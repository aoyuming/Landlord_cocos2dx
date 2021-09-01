#pragma once
#include <hash_map>
#include <string>
#include <io.h>

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "iconv.h"

#define LIAOTIAN_MUSIC		"sound//Man_Chat_0.mp3"		//聊天 有10个
#define JIAODIZHU_MUSIC		"sound//Man_Order.mp3"		//叫地主
#define BUJIAO_MUSIC		"sound//Man_NoOrder.mp3"	//不叫地主
#define QIANGDIZHU_MUSIC	"sound//Man_Rob1.mp3"		//抢地主 有3个
#define BUQIANG_MUSIC		"sound//Man_NoRob.mp3"		//不抢
#define BUYAO_MUSIC			"sound//Man_buyao1.mp3"		//不要 有4个
#define JIABEI_MUSIC		"sound//Man_jiabei.mp3"		//加倍
#define BUJIABEI_MUSIC		"sound//Man_bujiabei.mp3"	//不加倍
#define SANDAIYI_MUSIC		"sound//Man_sandaiyi.mp3"   //三带一					
#define SANDAIYIDUI_MUSIC	"sound//Man_sandaiyidui.mp3"//三带一对
#define SANBUDAI_MUSIC		"sound//Man_tuple1.mp3"     //三不带 从 A开始 	到K	
#define SIDAIER_MUSIC		"sound//Man_sidaier.mp3"     //四带2
#define SIDAILIANGDUI_MUSIC	"sound//Man_sidailiangdui.mp3"//四带2对
#define DANPAI_MUSIC		"sound//Man_1.mp3"          //单牌   从 A开始 	到大王     14小王 15大王	
#define YIDUI_MUSIC			"sound//Man_dui1.mp3"       //一对   从 A开始 	到K	
#define FEIJI_MUSIC         "sound//Man_feiji.mp3"		//飞机
#define SHUNZI_MUSIC        "sound//Man_shunzi.mp3"		//顺子
#define LIANDUI_MUSIC		"sound//Man_liandui.mp3"	//连对
#define ZHADAN_MUSIC		"sound//Man_zhadan.mp3"		//炸弹
#define WANGZHA_MUSIC		"sound//Man_wangzha.mp3"	//王炸
#define MINGPAI_MUSIC		"sound//Man_Share.mp3"		//明牌
#define FAPAI_MUSIC			"sound//SpecSelectCard.mp3" //发牌的声音
#define BAOJING1_MUSICE     "sound//Man_baojing1.mp3"   //1张牌报警
#define BAOJING2_MUSICE     "sound//Man_baojing2.mp3"   //2张牌报警
#define DANI1_MUSICE		"sound//Man_dani1.mp3"		//大你有3个

#define BGM_WELCOME_MUSICE	  "sound//MusicEx_Welcome.mp3"	//刚进游戏的BGM
#define BGM_ZHENGCHANG_MUSICE "sound//MusicEx_Normal.mp3"	//正常的BGM
#define BGM_ZHADAN_MUSICE	  "sound//MusicEx_Exciting.mp3"	//炸弹后的BGM
#define BGM_WIN_MUSICE		  "sound//MusicEx_Win.mp3"	    //赢后的BGM
#define BGM_LOSE_MUSICE       "sound//MusicEx_Lose.mp3"	    //输后的BGM



 
USING_NS_CC;
using namespace std;
using namespace CocosDenshion;

//矩形范围
struct ME_Rect
{
	int left;
	int right;
	int top;
	int bottom;
};

//加载一个文件下所以的音乐文件
int LoadDirectory(const char* fn);
//矩形碰撞
bool MyIntersectRect(ME_Rect* rect1,ME_Rect* rect2);
//判断点是否在矩形
bool MyPtInRect(ME_Rect* rect,int x,int y);
//中文转换
int GBKToUTF8(std::string &gbkStr,const char* toCode,const char* formCode);

