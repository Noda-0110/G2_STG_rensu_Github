//#CRT_SECURE_NO_WARNINGS
//�w�b�_�t�@�C���ǂݍ���
#include "game.h"		//�Q�[�����Ǘ�����ׂ̃w�b�_�t�@�C���@�����܂ł��Q�[���̊Ǘ�
#include "keyboard.h"	//�L�[�{�[�h�̏���
#include "FPS.h"		//FPS�̏���

//�}�N����`
#define TAMA_DIV_MAX 4	//���̉摜�̍ő吔
#define TAMA_MAX 10	//���̉摜�̍ő吔

//�\���̂̒�`

//�摜�̍\����
struct IMAGE
{
	int handle = -1;	//�摜�̃n���h���i�Ǘ��ԍ��j
	char path[255];		//�摜�̏ꏊ�i�p�X�j

	int x;				//X�ʒu
	int y;				//Y�ʒu
	int width;			//��
	int height;			//����

	BOOL IsDraw = FALSE;//�摜���`��ł��邩
};

//�L�����N�^�̍\����
struct CHARACTOR
{
	IMAGE img;			//�摜�\����
	int speed = 1;

	RECT coll;			//�㉺���E�����蔻��̗̈� RECT�͎l�p�`�̈ʒu��������
};

//����̍\����
struct MOVIE
{
	int handle = -1;	//����̃n���h��
	char path[255];		//����̃p�X

	int x;				//���ʒu
	int y;				//���ʒu
	int width;			//��
	int height;			//����
	int Volume = 255;	//�{�����[���i�ŏ��j0�`255�i�ő�j
};

//���y�̍\����
struct AUDIO
{
	int handle = -1;	//���y�̃n���h��
	char path[255];		//���y�̃p�X

	int volume = -1;	//�{�����[���iMIN 0�@�`�@�Q�T�T�@MAX�j
	int playType = -1;
};

//���̍\����
struct TAMA
{
	int handle[TAMA_DIV_MAX];	//�摜�̃n���h��
	char path[255];	//�摜�̃p�X

	int DivTate;	//�������i�c�j
	int DivYoko;	//�������i���j
	int DivMax;		//��������

	int AnimeCnt = 0;		//�A�j���[�V�����J�E���^
	int AnimeCntMAX = 0;	//�A�j���[�V�����J�E���^�}�b�N�X

	int NonIndex = 0;		//���݂̉摜�̗v�f��

	int x;			//x�̈ʒu
	int y;			//y�̈ʒu
	int width;		//��
	int height;		//����
	int spped;		//���̑���

	RECT coll;				//�����蔻��i��`�j
	BOOL IsDraw = FALSE;	//�`��ł��邩
};

//�O���[�o���ϐ�
//�V�[�����Ǘ�����ϐ�
GAME_SCENE GameScene;		//���݂̃Q�[���̃V�[��
GAME_SCENE OldGameScene;	//�O����Q�[���̃V�[��
GAME_SCENE NextGameScene;	//���̃Q�[���̃V�[��


//��ʂ̐؂�ւ�
BOOL IsFadeOut = FALSE;		//�t�F�[�h�A�E�g
BOOL IsFadeIn = FALSE;		//�t�F�[�h�C��

int FadeTimeMill = 2000;					//�؂�ւ��~���b
int FadeTimeMax = FadeTimeMill / 1000 * 60;	//�~���b���t���[���a�ɕϊ�

//�t�F�[�h�A�E�g
int FadeOutCntInit = 0;				//�����l
int FadeOutCnt = FadeOutCntInit;	//�t�F�[�h�A�E�g�̃J�E���^
int FadeOutCntMax = FadeTimeMax;	//�t�F�[�h�A�E�g�̃J�E���^MAX

//�t�F�[�h�C��
int FadeInCntInit = FadeTimeMax;	//�����l
int FadeInCnt = FadeInCntInit;		//�t�F�[�h�C���̃J�E���^
int FadeInCntMax = FadeTimeMax;		//�t�F�[�h�C���̃J�E���^MAX

//���̍\���̕ϐ�
struct TAMA tama_moto;				//��
struct TAMA tama[TAMA_MAX];				//���ۂɎg��

//�v���g�^�C�v�錾
VOID Title(VOID);		//�^�C�g�����
VOID TitleProc(VOID);	//�^�C�g����ʁi�����j
VOID TitleDraw(VOID);	//�^�C�g����ʁi�`��j

VOID Play(VOID);		//�v���C���
VOID PlayProc(VOID);	//�v���C��ʁi�����j
VOID PlayDraw(VOID);	//�v���C��ʁi�`��j

VOID End(VOID);			//�G���h���
VOID EndProc(VOID);		//�G���h��ʁi�����j
VOID EndDraw(VOID);		//�G���h��ʁi�`��j

VOID Change(VOID);		//�؂�ւ����
VOID ChangeProc(VOID);	//�؂�ւ���ʁi�����j
VOID ChangeDraw(VOID);	//�؂�ւ���ʁi�`��j


VOID ChangeScene(GAME_SCENE scene);						//�V�[���؂�ւ�

VOID collUpdateplayer(CHARACTOR* chara);				//�����蔻��̗̈���X�V

VOID collUpdateTama(TAMA* tama);						//�����蔻��̗̈���X�V

VOID collUpdateenemy(CHARACTOR* chara);					//�����蔻��̗̈���X�V


BOOL colltouch(RECT player, RECT goal);					//�����蔻��̐G��Ă��邩�G��Ă��Ȃ����̔���
BOOL GameLoad(VOID);									//�Q�[���f�[�^�̓ǂݍ���
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType);	
BOOL LoadImageMem(IMAGE* image, const char* path);					//���y�̓ǂݍ���
VOID GameInit(VOID);												//�Q�[���̏�����

VOID DrawTama(TAMA* tama);											//���̕`��

BOOL LoadImageDivMem(int* handle, const char* path, int bunkatuYoko, int bunkatuTate);	//�Q�[���̉摜�̕����ǂݍ���

// �v���O������ WinMain ����n�܂�܂�
//Windows�̃v���O���~���O�@�œ����Ă���
//DxLib�́ADirectX�Ƃ����A�Q�[���v���O���~���O���ȒP�Ɉ�����d�g��
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	//�ǉ���
	SetOutApplicationLogValidFlag(FALSE);	//Log.txt���o�͂��Ȃ�

	ChangeWindowMode(TRUE);					//�E�B���h�E���[�h�ɐݒ�
	SetMainWindowText(GAME_TITLE);			//�E�B���h�E�̃^�C�g������
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//�E�B���h�E�̉𑜓x��ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//�E�B���h�E�̑傫����ݒ�
	SetBackgroundColor(255, 255, 255);					//�f�t�H���g�̔w�i�F
	SetWindowIconID(GAME_ICON_ID);						//�A�C�R���t�@�C���̓ǂݍ���

	//�ǉ���
	SetWindowStyleMode(GAME_WINDOW_BAR);	//�E�B���h�E�o�[�̏��
	SetWaitVSyncFlag(TRUE);					//�f�B�X�v���C�̐���������L���ɂ���
											//���x�ȏ�����`����s�����ɂ�FALSE

	SetAlwaysRunFlag(TRUE);					//�E�B���h�E�������ƃA�N�e�B�u�ɂ���


	if (DxLib_Init() == -1)		//�c�w���C�u���������������@-1���A���Ă����狭���I��
	{
		// �G���[���N�����璼���ɏI��
		return -1;
	}

	//�_�u���o�b�t�@�����O��L��
	SetDrawScreen(DX_SCREEN_BACK);


	//�ŏ��̃V�[���̓^�C�g�����
	GameScene == GAME_SCENE_TITLE;

	//�Q�[���S�̂̏�����

	//�Q�[���̓ǂݍ���
	if (!GameLoad())
	{
		//�f�[�^�̓ǂݍ���
		DxLib_End();	//DXLib�I��
		return -1;		//�ُ�I��
	}

	//�Q�[���̏�����
	GameInit();

	//�������[�v�@�󂯎�葱����
	while (1)
	{

		if (ProcessMessage() != 0) { break; }	//���b�Z�[�W���󂯎�葱����
		if (ClearDrawScreen() != 0) { break; }	//��ʂ���������

		//�L�[�{�[�h�̓��͍X�V
		AllKeyUpdate();


		//FPS�l�̍X�V
		FPSUpdate();

		//ESC�L�[�ŋ����I��
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }


		//DrawCircle(X, Y, radius, GetColor(255, 255, 0), TRUE); �~����ԉ�


		//�ȑO�̃V�[��������
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//�V�[�����Ƃɏ������s��
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();	//�^�C�g�����
			break;
		case GAME_SCENE_PLAY:
			Play();		//�v���C���
			break;
		case GAME_SCENE_END:
			End();		//�G���h���
			break;
		case GAME_SCENE_CHANGE:
			Change();	//�؂�ւ����
			break;
		default:
			break;
		}

		//�V�[����؂�ւ���
		if (OldGameScene != GameScene)
		{
			//���݃V�[�����؂�ւ���ʁi�^�C�g���v���C�G���h�̉�ʁj�łȂ��Ƃ�
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		//���̃V�[����ۑ�
				GameScene = GAME_SCENE_CHANGE;	//��ʐ؂�ւ��V�[���ɕς���
			}
		}

		//FPS�l��`��
		FPSDraw();
		//FPS������҂�
		FPSWait();

		ScreenFlip();	//�_�u���o�b�t�@�����O������ʂ�`��
	}
	//�ǂݍ��񂾉摜���J��
	for (int i = 0; i < TAMA_DIV_MAX; i++) { DeleteGraph(tama_moto.handle[i]); }

	// �c�w���C�u�����g�p�̏I�������i�����j
	DxLib_End();

	return 0;				// �\�t�g�̏I���i���s�j 
}




// - - - - �f�[�^���[�h - - - - //

/// <summary>
/// �Q�[���̃f�[�^��ǂݍ���
/// </summary>
/// <returns>�ǂݍ��߂���TRUE / �ǂݍ��߂Ȃ�������FALSE</returns>
BOOL GameLoad()
{
	//���̕�������ݒ�
	tama_moto.x = 4;
	tama_moto.y = 1;

	//���̃p�X
	strcpyDx(tama_moto.path,".\\image\\dia_green.png");

	//�摜�𕪊����ēǂݍ���
	if (LoadImageDivMem(&tama_moto.handle[0], ".\\image\\dia_green.png", 4, 1) == FALSE) { return FALSE; }

	//�ʒu��ݒ�
	tama_moto.x = GAME_WIDTH / 2 - tama_moto.width / 2;		//�������낦
	tama_moto.y = GAME_HEIGHT - tama_moto.height;			//��ʁ�

	tama_moto.spped = 10;		//���x
	//�����蔻��̍X�V
	collUpdateTama(&tama_moto);

	//�S�Ă̋��̏����R�s�[
	for (int i = 0; i < TAMA_MAX; i++)
	{
		tama[i] = tama_moto;
	}

	//�摜�̕\��
	tama_moto.IsDraw = FALSE;

	return TRUE;	//�S�Ă�ǂݍ��߂�
}
// - - - - - �f�[�^���[�h - - - - - //

/// <summary>
/// �摜�𕪊����ă������ɓǂ݂���
/// </summary>
/// <param name="handle">�n���h���z��̐擪�A�h���X</param>
/// <param name="path">�摜�̃p�X</param>
/// <param name="bunkatuYoko">��������Ƃ��̉��̐�</param>
/// <param name="bunkatuTate">��������Ƃ��̏c�̐�</param>
/// <returns></returns>
BOOL LoadImageDivMem(int* handle, const char* path, int bunkatuYoko, int bunkatuTate)
{
	//���̓ǂݍ���
	int IsTamaLoad = -1;

	//�ꎞ�I�Ɏg���n���h����p�ӂ���i�摜�T�C�Y�̓ǂݍ��݂̂��߁j
	int Tamahandle = LoadGraph(path);

	if (Tamahandle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//�E�C���h�E�n���h��
			path,					//�{��
			"�摜�ǂݍ��݃G���[",	//�^�C�g��
			MB_OK					//�{�^��
		);
		return FALSE;
	}

	//�摜�̕��ƍ������擾
	int TamaWidth = -1;
	int TamaHeight = -1;
	GetGraphSize(Tamahandle, &TamaWidth, &TamaHeight);

	//�������ēǂݍ���
	IsTamaLoad = LoadDivGraph(
		path,						//�摜�̃p�X
		TAMA_DIV_MAX,					//��������
		bunkatuYoko, bunkatuTate,							//���̕����A�c�̕���
		TamaWidth / bunkatuYoko, TamaHeight / bunkatuTate,	//�摜����̕��A����
		handle						//�A������擪�A�h���X
	);

	//�����G���[
	if (Tamahandle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//�E�C���h�E�n���h��
			path,				//�{��
			"�摜�����G���[",	//�^�C�g��
			MB_OK
		);
		return FALSE;
	}

	//�ꎞ�I�ɓǂݍ��񂾃n���h�����J������
	DeleteGraph(Tamahandle);

	return TRUE;
}

VOID GameInit(VOID)
{
}

/// <summary>
/// ���y���������ւƓǂݍ���
/// </summary>
/// <param name="audio">Audio�\���̂̃A�h���X &�`�`�`</param>
/// <param name="path">Audio�̃p�X�@.\\Audio\\�`�`�`</param>
/// <param name="volume">�{�����[��</param>
/// <param name="playType">DX_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK</param>
/// <returns></returns>
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType)
{
	strcpyDx(audio->path,path);
	audio->handle = LoadSoundMem(audio->path);

	//���y��ǂݍ��݂��ł��Ȃ�����
	if (audio->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			audio->path,				//���b�Z�[�W�{��
			"���y�ǂݍ��݃G���[�I",	//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);
		return FALSE;				//�G���[�I��
	}

	audio->volume = volume;
	audio->playType = playType;

	return TRUE;
}

// ------------  �V�[���֐��@�@------------ //
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;	//�V�[����؂�ւ�
	IsFadeIn = FALSE;	//�t�F�[�h�C�����Ȃ�
	IsFadeOut = TRUE;	//�t�F�[�h�A�E�g����

	return;
}

/// <summary>
/// �^�C�g�����
/// </summary>
VOID Title(VOID)
{
	TitleProc();	//����
	TitleDraw();	//�`��

	return;
}
/// <summary>
/// �^�C�g����ʁ@����
/// </summary>
VOID TitleProc(VOID)

{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		
		ChangeScene(GAME_SCENE_PLAY);

		return;
	}

	return;
}
/// <summary>
/// �^�C�g����ʁ@���� 
/// </summary>
VOID TitleDraw(VOID)
{
	DrawTama(&tama[0]);
	DrawString(0, 0, "�^�C�g�����", GetColor(255, 0, 0));
	return;
}

/// <summary>
/// ���̕`��
/// </summary>
/// <param name="tama">���̍\����</param>
VOID DrawTama(TAMA* tama)
{
	//���̕`��
	DrawGraph(0, 0, tama->handle[tama->NonIndex], TRUE);

	//�摜��ς���^�C�~���O
	if (tama->AnimeCnt < tama->AnimeCntMAX)
	{
		tama->AnimeCnt++;
	}
	else
	{
		//���̓Y���������̕������̍ő�������Ȃ�
		if (tama->NonIndex < TAMA_DIV_MAX - 1)
		{
			tama->NonIndex++;		//���̉摜��
		}
		else
		{
			tama->NonIndex = 0;		//�ŏ��ɖ߂�
		}
		tama->AnimeCnt = 0;
	}
}

/// <summary>
/// �v���C���
/// </summary>
VOID Play(VOID)
{
	PlayProc();
	PlayDraw();
	return;
}
/// <summary>
/// �v���C��ʁ@����
/// </summary>
VOID PlayProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{

		ChangeScene(GAME_SCENE_END);

		return;
	}

	return;
}
/// <summary>
/// �v���C��ʁ@�`��
/// </summary>
VOID PlayDraw(VOID)

{
	DrawString(0, 0, "�v���C���", GetColor(0, 0, 0));
	return;
}


/// <summary>
/// �G���h���
/// </summary>
VOID End(VOID)
{
	EndProc();
	EndDraw();
	return;
}
/// <summary>
/// �G���h��ʁ@����
/// </summary>
VOID EndProc(VOID)
{

	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{

		ChangeScene(GAME_SCENE_TITLE);

		return;
	}

	return;
}
/// <summary>
/// �G���h��� �`��
/// </summary>
VOID EndDraw(VOID)
{
	DrawString(0, 0, "�G���h���", GetColor(0, 0, 0));
	return;
}


/// <summary>
/// �؂�ւ����
/// </summary>
VOID Change(VOID)
{
	ChangeProc();
	ChangeDraw();
	return;
}
/// <summary>
/// �؂�ւ���� ����
/// </summary>
VOID ChangeProc(VOID)
{
	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		if (FadeInCnt > FadeInCntMax)
		{
			FadeInCnt--;	//�J�E���^�����炷�A�����ɂ��Ă���
		}
		else
		{
			//�t�F�[�h�C�������̏I���� �����J�E���^��MAX�𒴂����Ƃ�
			FadeInCnt = FadeInCntInit;	//�J�E���^������
			IsFadeIn = FALSE;			//�t�F�[�h�C�������̏I��
		}

	}
	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		if (FadeOutCnt < FadeOutCntMax)
		{
			FadeOutCnt++;	//�J�E���^�𑝂₷�A�F��Z���ɂ��Ă���
		}
		else
		{
			//�t�F�[�h�C�������̏I���� �����J�E���^��MAX�𒴂����Ƃ�
			FadeOutCnt = FadeOutCntInit;	//�J�E���^������
			IsFadeOut = FALSE;			//�t�F�[�h�C�������̏I��
		}

	}

	//�؂�ւ������I���H
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//�t�F�[�h�A�E�g���t�F�[�h�C�������Ă��Ȃ�
		GameScene = NextGameScene;	//���̃V�[���ɐ؂�ւ�
		OldGameScene = GameScene;	//�ȑO�̃V�[���ɍX�V
	}
	return;
}

/// <summary>
/// �؂�ւ���ʁ@�`��
/// </summary>
VOID ChangeDraw(VOID)
{
	//�ȑO�̃V�[����`��
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//�^�C�g����ʂ̕`��
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		//�v���C��ʂ̕`��
		break;
	case GAME_SCENE_END:
		EndDraw();		//�G���h��ʂ̕`��
		break;
	default:
		break;
	}

	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)FadeInCnt / FadeInCntMax) * 255);	//�p�[�Z���e�[�W

	}
	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)FadeOutCnt / FadeOutCntMax) * 255);	//�p�[�Z���e�[�W

	}

	//�l�p��`��
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//�������I��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "�؂�ւ����", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �����蔻��̍X�V(�v���C���[��p)
/// </summary>
/// <param name="coll">�����蔻��̗̈�</param>
VOID collUpdateplayer(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x + 30;		//�����蔻��������
	chara->coll.top = chara->img.y + 25;		//�����蔻��������
	chara->coll.right = chara->img.x + chara->img.width - 15;	//�����蔻��������
	chara->coll.bottom = chara->img.y + chara->img.height - 20;	//�����蔻��������

	return;
}

/// <summary>
/// �����蔻��̍X�V(�S�[���p)
/// </summary>
/// <param name="coll">�����蔻��̗̈�</param>
VOID collUpdateTama(TAMA* tama)
{
	tama->coll.left = tama->x;
	tama->coll.top = tama->y;
	tama->coll.right = tama->x + tama->width;
	tama->coll.bottom = tama->y + tama->height;

	return;
}

/// <summary>
/// �����蔻��̍X�V(�G�p)
/// </summary>
/// <param name="coll">�����蔻��̗̈�</param>
VOID collUpdateenemy(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}


BOOL colltouch(RECT p,RECT g)
{
	if (
		p.left < g.right &&	//p�̍���x < g�̉E��x���W
		p.right > g.left &&	//p�̉E��x < g�̍���x���W
		p.bottom > g.top &&	//p�̏��y < g�̉���y���W
		p.top < g.bottom	//p�̉���y < g�̏��y���W
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}