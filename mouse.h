#pragma once

//�w�b�_�t�@�C���̓ǂݍ���
#include"game.h"
#include"shape.h"

//�}�E�X�̃{�^��
#define MOUSE_BUTTON_CODE	129	//0x0000�`0x0080�܂�


//�}�E�X�\����
struct  MOUSE
{
	int InputValue = 0;		//GetMouseInput�̒l������
	int WheelValue = 0;		//�}�E�X�z�C�[���̉�]�ʁi���̓v���X�l�@��O�̓}�C�i�X�l�j
	iPOINT Point;			//�}�E�X�̍��W������
	iPOINT OldPoint;		//�}�E�X�̍��W�i���O�j������
	int OldButton[MOUSE_BUTTON_CODE];	//�}�E�X�̃{�^�����́i���O�j������
	int Button[MOUSE_BUTTON_CODE];		//�}�E�X�̃{�^������
};

//�}�E�X�̓��͂��擾
extern MOUSE mouse;


//�v���g�^�C�v�錾
//extern�ŊO���Ɋ֐�������A�Ɩڈ�

extern VOID MouseUpdate(VOID);
extern BOOL MouseDown(int MOUSE_INPUT_);
extern BOOL MouseUp(int MOUSE_INPUT_);
extern VOID MouseDraw(VOID);
extern BOOL MouseDownKeep(int MOUSE_INPUT_, int milliTime);
extern BOOL MouseClick(int MOUSE_INPUT_);

extern BOOL MouseRectClick(RECT rect, int MOUSE_INPUT_);
extern BOOL MouseMaruClick(MARU circle, int MOUSE_INPUT_);
