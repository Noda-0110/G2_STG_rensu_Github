#pragma once

//�w�b�_�t�@�C���̓ǂݍ���
#include "game.h"

//int�^��iPOINT�\����
struct iPOINT
{
	int x = -1;	//���W��������
	int y = -1;	//���W��������
};

//�~�̍\����
struct MARU
{
	iPOINT center;	//���S�_
	int radius;		//���a
};

//�v���g�^�C�v�錾
//extern�ŊO���Ɋ֐�������A�Ɩڈ�

extern BOOL CheckCollRectToRect(RECT a, RECT b);
extern BOOL CheckColliPointToRect(iPOINT pt, RECT r);
extern BOOL CheckColliPointToMaru(iPOINT pt, MARU m);
extern BOOL CheckCollMaruToMaru(MARU maru1, MARU maru2);

extern RECT GetRect(int left, int top, int right, int bottom);
extern void DrawRect(RECT r, unsigned int color, bool b);
extern VOID DrawMaru(MARU c, unsigned int color, bool b, int thick);


