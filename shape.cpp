//�w�b�_�t�@�C���̓ǂݍ���
#include "game.h"
#include "shape.h"
#include "math.h"

/// <summary>
/// ��`�̈擯�m�̓����蔻����s��
/// </summary>
/// <param name="a">�̈�A</param>
/// <param name="b">�̈�B</param>
/// <returns></returns>
BOOL CheckCollRectToRect(RECT a, RECT b)
{
	if (a.left < b.right &&
		a.top < b.bottom &&
		a.right > b.left &&
		a.bottom > b.top)
	{
		return TRUE;	//�������Ă���
	}
	return FALSE;		//�������Ă��Ȃ�
}


/// <summary>
/// �l�p�Ɠ_�̓����蔻��
/// </summary>
/// <param name="pt">iPOINT�^�̓_</param>
/// <param name="r">��`�̈�</param>
/// <returns></returns>
BOOL CheckColliPointToRect(iPOINT pt, RECT r)
{
	if (pt.x > r.left && pt.x < r.right &&	//�_��X�ʒu���l�p�̍��E�̊Ԃɂ���
		pt.y > r.top && pt.y > r.bottom)	//�_��Y�ʒu���l�p�̏㉺�ɂ���
	{
		return TRUE;
	}
	return FALSE;
}


/// <summary>
/// �~�Ɠ_�̓����蔻��
/// </summary>
/// <param name="pt">iPOINT�^�̓_</param>
/// <param name="m">�~�̈�</param>
/// <returns></returns>
BOOL CheckColliPointToMaru(iPOINT pt, MARU m)
{
	//�O�p�֐����g�� c^2 = a^2 + b^2 ������ ��c = ��a + ��b

	//�O�p�֐������߂�
	float a = pt.x - m.center.x;		//�O�p�`�̒�ӂ̒��������߂�
	float b = pt.y - m.center.y;		//�O�p�`�̍����̒��������߂�
	float c = sqrtf(a * a + b * b);		//�O�p�`�̎Εӂ̒��������߂�(sq rt = square root = ������)

	//�Εӂ̒������A�~�̔��a���Z���ꍇ�A�~�̒��ɂ���
	if (c <= m.radius)
	{
		return TRUE;
	}
	return FALSE;
}

/// <summary>
/// �~�Ɖ~�̓����蔻��
/// </summary>
/// <param name="maru1">�~�̈�P</param>
/// <param name="maru2">�~�̈�Q</param>
/// <returns></returns>
BOOL CheckCollMaruToMaru(MARU maru1, MARU maru2)
{
		//�O�p�֐����g�� c^2 = a^2 + b^2 ������ ��c = ��a + ��b

		//�O�p�֐������߂�
	float a = maru1.center.x - maru2.center.x;	//�O�p�`�̒�ӂ̒��������߂�
	float b = maru1.center.y - maru2.center.y;	//�O�p�`�̍����̒��������߂�
	float c = sqrtf(a * a + b * b);				//�O�p�`�̎Εӂ̒��������߂�(sq rt = square root = ������)

	//�Εӂ̒������A�~�̔��a1+�~�̔��a2���Z���ꍇ�A�~�̒��ɂ���
	if (c <= maru1.radius + maru2.radius)
	{
		return TRUE;
	}
	return FALSE;

}


/// <summary>
/// RECT�^���ꎞ�I�ɓn��
/// </summary>
/// <param name="left">��</param>
/// <param name="top">��</param>
/// <param name="right">�E</param>
/// <param name="bottom">��</param>
/// <returns></returns>
RECT GetRect(int left, int top, int right, int bottom)
{
	//�ꎞ�I��RECT�^�̕ϐ����g��
	RECT rect = { left,top,right,bottom };

	//RECT�^��Ԃ�
	return rect;
}


/// <summary>
/// RECT�𗘗p���Ďl�p��`��
/// </summary>
/// <param name="r">RECT�\����</param>
/// <param name="color">�`�悷��F</param>
/// <param name="b">�h��Ԃ��Ȃ��Ȃ�FALSE�@�h��Ԃ���TRUE</param>
VOID DrawRect(RECT r, unsigned int color, bool b)
{
	//��������ɕ`��
	DrawBox(r.left, r.top, r.right, r.bottom, color, b);
	return;
}


/// <summary>
/// MARU�𗘗p���Ďl�p��`��
/// </summary>
/// <param name="c">MARU�\����</param>
/// <param name="color">�`�悷��F</param>
/// <param name="b">�h��Ԃ��Ȃ��Ȃ�FALSE�@�h��Ԃ���TRUE</param>
/// <param name="thick">��̑���</param>
VOID DrawMaru(MARU c, unsigned int color, bool b,int thick)
{
	//��������ɕ`��
	DrawCircle(c.center.x, c.center.y, c.radius, color, b, thick);
	return;
}