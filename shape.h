#pragma once

//ヘッダファイルの読み込み
#include "game.h"

//int型のiPOINT構造体
struct iPOINT
{
	int x = -1;	//座標を初期化
	int y = -1;	//座標を初期化
};

//円の構造体
struct MARU
{
	iPOINT center;	//中心点
	int radius;		//半径
};

//プロトタイプ宣言
//externで外部に関数がある、と目印

extern BOOL CheckCollRectToRect(RECT a, RECT b);
extern BOOL CheckColliPointToRect(iPOINT pt, RECT r);
extern BOOL CheckColliPointToMaru(iPOINT pt, MARU m);
extern BOOL CheckCollMaruToMaru(MARU maru1, MARU maru2);

extern RECT GetRect(int left, int top, int right, int bottom);
extern void DrawRect(RECT r, unsigned int color, bool b);
extern VOID DrawMaru(MARU c, unsigned int color, bool b, int thick);


