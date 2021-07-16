#pragma once

//ヘッダファイルの読み込み
#include"game.h"
#include"shape.h"

//マウスのボタン
#define MOUSE_BUTTON_CODE	129	//0x0000～0x0080まで


//マウス構造体
struct  MOUSE
{
	int InputValue = 0;		//GetMouseInputの値が入る
	int WheelValue = 0;		//マウスホイールの回転量（奥はプラス値　手前はマイナス値）
	iPOINT Point;			//マウスの座標が入る
	iPOINT OldPoint;		//マウスの座標（直前）が入る
	int OldButton[MOUSE_BUTTON_CODE];	//マウスのボタン入力（直前）が入る
	int Button[MOUSE_BUTTON_CODE];		//マウスのボタン入力
};

//マウスの入力を取得
extern MOUSE mouse;


//プロトタイプ宣言
//externで外部に関数がある、と目印

extern VOID MouseUpdate(VOID);
extern BOOL MouseDown(int MOUSE_INPUT_);
extern BOOL MouseUp(int MOUSE_INPUT_);
extern VOID MouseDraw(VOID);
extern BOOL MouseDownKeep(int MOUSE_INPUT_, int milliTime);
extern BOOL MouseClick(int MOUSE_INPUT_);

extern BOOL MouseRectClick(RECT rect, int MOUSE_INPUT_);
extern BOOL MouseMaruClick(MARU circle, int MOUSE_INPUT_);
