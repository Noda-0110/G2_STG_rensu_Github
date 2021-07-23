//#CRT_SECURE_NO_WARNINGS
//�w�b�_�t�@�C���ǂݍ���
#include "game.h"		//�Q�[�����Ǘ�����ׂ̃w�b�_�t�@�C���@�����܂ł��Q�[���̊Ǘ�
#include "keyboard.h"	//�L�[�{�[�h�̏���
#include "FPS.h"		//FPS�̏���

#include "mouse.h"		//FPS�̏���
#include "shape.h"		//FPS�̏���

#include <math.h>

//�}�N����`
#define TAMA_DIV_MAX 4	//���̉摜�̍ő吔
#define TAMA_MAX 100	//���̉摜�̍ő吔
#define TEKI_KIND 8		//�G�̉摜�̎��
#define TEKI_MAX 10		//�G�̉摜�̑���

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
	int speed = 5;		//�ړ����x
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

	int NowIndex = 0;		//���݂̉摜�̗v�f��

	int Startx;		//X�ŏ��ʒu
	int Starty;		//Y�ŏ��ʒu

	float radius;	//���a
	float degree;	//�p�x

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

//PushEnter
int PushEnterCnt = 0;
const int PushEnterCntMAX = 60;
BOOL PushEnterBrink = FALSE;

//���̍\���̕ϐ�
struct TAMA tama_moto;				//��
struct TAMA tama[TAMA_MAX];			//���ۂɎg��

//���̔��˃J�E���g
int tamashotcnt = 0;
int tamashotcntMAX = 5;

int subtamashotcnt = 0;
int subtamashotcntMAX = 60;

//�v���C���[�\���̐錾
CHARACTOR player;

//�w�i�摜(�V�[�����X������Q��)
IMAGE back[2];

//�w�i�摜�i�^�C�g���ƃG���h�j
MOVIE titleMovie;
IMAGE finish[1];

//�G�f�[�^(�e�Ɠ��������ƂȂ����)
CHARACTOR teki_moto[TEKI_KIND];

//���G�f�[�^
CHARACTOR teki[TEKI_MAX];

//���S�̓ǂݍ���
IMAGE TitleLogo;
IMAGE EndLogo;
IMAGE PlayEnter;

//BGM�̓ǂݍ���
AUDIO TitleBGM;
AUDIO PlayBGM;
AUDIO EndBGM;

//�G�̃p�X�����Ă���
char tekiPath[TEKI_KIND][255] =
{
	{".\\image\\enemy_blue.png"},
	{".\\image\\enemy_siber.png"},
	{".\\image\\enemy_green.png"},
	{".\\image\\enemy_bag.png"},
	{".\\image\\enemy_purple.png"},
	{".\\image\\enemy_red_mini.png"},
	{".\\image\\enemy_red.png"},
	{".\\image\\enemy_yelow.png"}
};

//�G���o�Ă���J�E���g�i�Ԋu�j
int tekiAddcnt = 0;
int tekiAddcntMax = 60;	//60fps�ň�b�E�E�E��b�Ɉ��o����


//�Q�[���̃X�R�A
int score = 0;


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
VOID collUpdate(CHARACTOR* chara);						//�����蔻��
VOID collUpdateTama(TAMA* tama);						//�����蔻��̗̈���X�V
VOID collUpdateenemy(CHARACTOR* chara);					//�����蔻��̗̈���X�V


BOOL colltouch(RECT a, RECT b);					//�����蔻��̐G��Ă��邩�G��Ă��Ȃ����̔���
BOOL GameLoad(VOID);									//�Q�[���f�[�^�̓ǂݍ���
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType);	//���y�̓ǂݍ���
BOOL LoadImageMem(IMAGE* image, const char* path);

VOID GameInit(VOID);									//�Q�[���̏�����

VOID DrawTama(TAMA* tama);								//���̕`��
VOID shottama(TAMA* tama, float deg);					//���̔���

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

		//�}�E�X���͂̍X�V
		MouseUpdate();

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

	//�v���C���[���J��
	DeleteGraph(player.img.handle);
	DeleteGraph(titleMovie.handle);

	//�w�i�摜���J��
	DeleteGraph(back[0].handle);
	DeleteGraph(back[1].handle);
	DeleteGraph(finish[0].handle);

	//���y���폜
	DeleteSoundMem(TitleBGM.handle);
	DeleteSoundMem(PlayBGM.handle);
	DeleteSoundMem(EndBGM.handle);

	//�G�̉摜���J��
	for (int i = 0; i < TEKI_KIND; i++)
	{
		DeleteGraph(teki_moto[i].img.handle);
	}

	// �c�w���C�u�����g�p�̏I�������i�����j
	DxLib_End();

	return 0;				// �\�t�g�̏I���i���s�j 
}




// - - - -  - - - - �f�[�^���[�h - - - -  - - - - //

/// <summary>
/// �Q�[���̃f�[�^��ǂݍ���
/// </summary>
/// <returns>�ǂݍ��߂���TRUE / �ǂݍ��߂Ȃ�������FALSE</returns>
BOOL GameLoad()
{
	//�^�C�g����ʂ̔w�i��ǂݍ���
	strcpyDx(titleMovie.path, ".\\Movie\\Diamonds.mp4");
	titleMovie.handle = LoadGraph(titleMovie.path);
	if (titleMovie.handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			titleMovie.path,		//���b�Z�[�W�{��
			"����ǂݍ��݃G���[",	//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);
	}
	//����̃n���h���A���ƍ���
	GetGraphSize(titleMovie.handle, &titleMovie.width, &titleMovie.height);
	//����̃{�����[��
	titleMovie.Volume = 255;
	
	//���̕�������ݒ�
	tama_moto.DivYoko = 4;
	tama_moto.DivTate = 1;

	//���̃p�X���R�s�[
	strcpyDx(tama_moto.path, ".\\image\\dia_green.png");

	//�摜�𕪊����ēǂݍ���
	if (LoadImageDivMem(&tama_moto.handle[0], ".\\image\\dia_green.png", tama_moto.DivYoko, tama_moto.DivTate) == FALSE) { return FALSE; }

	//���ƍ���������
	GetGraphSize(tama_moto.handle[0], &tama_moto.width, &tama_moto.height);

	//�ʒu��ݒ�
	tama_moto.x = GAME_WIDTH / 2 - tama_moto.width / 2;		//�������낦
	tama_moto.y = GAME_HEIGHT - tama_moto.height;			//��ʁ�

	tama_moto.spped = 1;		//���x

	//�A�j����ς��鑬�x
	tama_moto.AnimeCntMAX = 10;

	//�����蔻��̍X�V
	collUpdateTama(&tama_moto);

	//�摜�̕\��
	tama_moto.IsDraw = FALSE;

	//�S�Ă̋��̏����R�s�[
	for (int i = 0; i < TAMA_MAX; i++)
	{
		tama[i] = tama_moto;
	}

	//�v���C���[�̓ǂݍ���
	if (LoadImageMem(&player.img, ".\\image\\player.png") == FALSE) { return FALSE; }
	player.img.x = GAME_WIDTH / 2 - player.img.width;
	player.img.y = GAME_HEIGHT / 2 - player.img.height;
	collUpdateplayer(&player);
	player.img.IsDraw == TRUE;

	//�w�i�̓ǂݍ���
	if (LoadImageMem(&back[0], ".\\image\\hosi.png") == FALSE) { return FALSE; }
	back[0].x = 0;				//�摜�̍��[
	back[0].y = -back[0].height;	//�摜�̍������A�ʒu����Ɂi�[�����j������
	back[0].IsDraw == TRUE;

	if (LoadImageMem(&back[1], ".\\image\\hosi_rev.png") == FALSE) { return FALSE; }
	back[1].x = 0;				//�摜�̍��[
	back[1].y = 0;	
	back[1].IsDraw == TRUE;

	//�G���h��ʂ̔w�i
	if (LoadImageMem(&finish[0], ".\\image\\db.jpg") == FALSE) { return FALSE; }
	finish[0].x = 0;				//�摜�̍��[
	finish[0].y = 0;	
	finish[0].IsDraw == TRUE;


	//���S�̓ǂݍ���
	if (!LoadImageMem(&TitleLogo, ".\\image\\�^�C�g�����S.png")) { return FALSE; }
	if (!LoadImageMem(&EndLogo, ".\\image\\�N���A���S.png")) { return FALSE; }
	if (!LoadImageMem(&PlayEnter, ".\\image\\PushEnter.png")) { return FALSE; }

	//���y�̓ǂݍ���
	if (!LoadAudio(&TitleBGM, ".\\Audio\\�^�C�g��BGM.mp3",255,DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&PlayBGM, ".\\Audio\\�v���CBGM.mp3",255,DX_PLAYTYPE_LOOP)) { return FALSE; }
	if (!LoadAudio(&EndBGM, ".\\Audio\\�G���h.mp3",255,DX_PLAYTYPE_LOOP)) { return FALSE; }

	//�G�̓ǂݍ���
	for (int i = 0; i < TEKI_KIND; i++)
	{
		if (LoadImageMem(&teki_moto[i].img, tekiPath[i]) == FALSE) { return FALSE; }
		teki_moto[i].img.x = GAME_WIDTH / 2 - teki_moto[i].img.width;
		teki_moto[i].img.y = -teki_moto[i].img.height;
		collUpdateplayer(&teki_moto[i]);	//�����蔻��̍X�V
		teki_moto[i].img.IsDraw == FALSE;	//�`�悵�܂���
	}
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

/// <summary>
/// �Q�[���f�[�^�̏�����
/// </summary>
/// <param name=""></param>
VOID GameInit(VOID)
{
	//�X�R�A�̏�����
	score = 0;

	//�v���C���[�̏�����
	player.img.x = GAME_WIDTH / 2 - player.img.width;
	player.img.y = GAME_HEIGHT / 2 - player.img.height;
	collUpdateplayer(&player);
	player.img.IsDraw = TRUE;

	//�w�i�摜�̐ݒ�1
	back[0].x = 0;
	back[0].y = -back[0].height;
	back[0].IsDraw = TRUE;

	//�w�i�摜�̐ݒ�1
	back[1].x = 0;
	back[1].y = 0;
	back[1].IsDraw = TRUE;

	//�^�C�g�����S�̈ʒu�����߂�
	TitleLogo.x = GAME_WIDTH / 2 - TitleLogo.width / 2;		//�������낦
	TitleLogo.y = 100;
	//�N���A���S�̈ʒu�����߂�
	EndLogo.x = GAME_WIDTH / 2 - EndLogo.width / 2;		//�������낦
	EndLogo.y = GAME_HEIGHT / 2 - EndLogo.height / 2;

	//PushEnter�̈ʒu�����߂�
	PlayEnter.x = GAME_WIDTH / 2 - PlayEnter.width / 2;	//�������낦	
	PlayEnter.y = 600;

	//PushEnter�̓_��
	PushEnterCnt = 0;
	PushEnterBrink = FALSE;
	
	//�G�̏�����
	for (int i = 0; i < TEKI_KIND; i++)
	{
		teki_moto[i].img.x = GAME_WIDTH / 2 - teki_moto[i].img.width;
		teki_moto[i].img.y = teki_moto[i].img.height;
		collUpdateplayer(&teki_moto[i]);	//�����蔻��̍X�V
		teki_moto[i].img.IsDraw = FALSE;	//�`��͂��Ȃ�
	}

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
	strcpyDx(audio->path, path);
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
		//BGM���~�߂�
		StopSoundMem(TitleBGM.handle);
		
		GameInit();

		//�v���C��ʂ�
		ChangeScene(GAME_SCENE_PLAY);

		//�}�E�X��`�悵�Ȃ�
		SetMouseDispFlag(FALSE);

		return;
	}
	if (CheckSoundMem(TitleBGM.handle) == 0)
	{
		PlaySoundMem(TitleBGM.handle, TitleBGM.playType);
	}

	return;
}
/// <summary>
/// �^�C�g����ʁ@���� 
/// </summary>
VOID TitleDraw(VOID)
{
	//�^�C�g���œ����w�i�ɕ`�悷��
	//�������悪�Đ�����Ă��Ȃ����
	if (GetMovieStateToGraph(titleMovie.handle) == 0)
	{
		//�Đ�����
		SeekMovieToGraph(titleMovie.handle, 0);	//�V�[�N�o�[��߂�
		PlayMovieToGraph(titleMovie.handle);	//������Đ��i�`��͂��Ă��Ȃ��j
	}
	//�����`��
	DrawExtendGraph(0, 0, GAME_WIDTH, GAME_HEIGHT,titleMovie.handle, TRUE);

	//�^�C�g�����S
	DrawGraph(TitleLogo.x, TitleLogo.y, TitleLogo.handle, TRUE);

	if (PushEnterCnt < PushEnterCntMAX) { PushEnterCnt++; }
	else
	{
		if (PushEnterBrink == TRUE)PushEnterBrink = FALSE;
		else if (PushEnterBrink == FALSE)PushEnterBrink = TRUE;

		PushEnterCnt = 0;	//�J�E���^��������
	}

	if (PushEnterBrink == TRUE)
	{
		//�������ɂ���
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)PushEnterCnt / PushEnterCntMAX) * 255);

		//PushEnter�̕`��
		DrawGraph(PlayEnter.x, PlayEnter.y, PlayEnter.handle, TRUE);

		//�������I��
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (PushEnterBrink == FALSE)
	{
		//�������ɂ���
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)(PushEnterCntMAX - PushEnterCnt) / PushEnterCntMAX) * 255);

		//PushEnter�̕`��
		DrawGraph(PlayEnter.x, PlayEnter.y, PlayEnter.handle, TRUE);

		//�������I��
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	DrawString(0, 0, "�^�C�g�����", GetColor(255, 0, 0));
	return;
}

/// <summary>
/// ���̕`��
/// </summary>
/// <param name="tama">���̍\����</param>
VOID DrawTama(TAMA* tama)
{
	//�����`��ł���Ƃ�
	if (tama->IsDraw == TRUE)
	{
		//���̕`��
		DrawGraph(tama->x, tama->y, tama->handle[tama->NowIndex], TRUE);

		//�摜��ς���^�C�~���O
		if (tama->AnimeCnt < tama->AnimeCntMAX)
		{
			tama->AnimeCnt++;
		}
		else
		{
			//���̓Y���������̕������̍ő�������Ȃ�
			if (tama->NowIndex < TAMA_DIV_MAX - 1)
			{
				tama->NowIndex++;		//���̉摜��
			}
			else
			{
				tama->NowIndex = 0;		//�ŏ��ɖ߂�
			}
			tama->AnimeCnt = 0;
		}
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
	if (CheckSoundMem(PlayBGM.handle) == 0)
	{
		PlaySoundMem(PlayBGM.handle, PlayBGM.playType);
	}

	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		StopSoundMem(PlayBGM.handle);

		ChangeScene(GAME_SCENE_END);

		//�}�E�X��`�悷��
		SetMouseDispFlag(TRUE);

		return;
	}
	/*
	//�v���C���[�𑀍�
	if (KeyDown(KEY_INPUT_LEFT) == TRUE)
	{
		if (player.img.x - player.speed >= 0)
		{
			player.img.x -= player.speed;
		}
	}
	//�v���C���[�𑀍�
		if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
	{
		if (player.img.x + player.img.width + player.speed <= GAME_WIDTH)
		{
			player.img.x += player.speed;
		}
	}
	//�v���C���[�𑀍�
		if (KeyDown(KEY_INPUT_UP) == TRUE)
	{
		if (player.img.y - player.speed >= 0)
		{
			player.img.y -= player.speed;
		}
	}
		//�v���C���[�𑀍�
	if (KeyDown(KEY_INPUT_DOWN) == TRUE)
	{
		if (player.img.y + player.speed <= GAME_HEIGHT)
		{
			player.img.y += player.speed;
		}
	}
	*/

	//�}�E�X�̈ʒu�Ƀv���C���[��u��
	player.img.x = mouse.Point.x - player.img.width / 2;	//�}�E�X�̈ʒu���摜�̒��S�ɂ���
	player.img.y = mouse.Point.y - player.img.height / 2;	//�}�E�X�̈ʒu���摜�̒��S�ɂ���

	collUpdateplayer(&player);
	/*
	//�X�y�[�X�L�[�������Ă���Ƃ�
	if (KeyDown(KEY_INPUT_SPACE) == TRUE)
	*/
	if (MouseDown(MOUSE_INPUT_LEFT) == TRUE)
	{
		if (tamashotcnt == 0)
		{
			//�e�𔭎˂���(�e��`�悷��) 
			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (tama[i].IsDraw == FALSE)
				{
					shottama(&tama[i], 240);

					//�e���o������A���[�v�𔲂���
					break;
				}
			}

			//�e�𔭎�(�e�̕`��) 
			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (tama[i].IsDraw == FALSE)
				{
					shottama(&tama[i], 270);

					//�e���P���o������A���[�v�𔲂���
					break;
				}
			}


			//�e�𔭎�(�e�̕`��) 
			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (tama[i].IsDraw == FALSE)
				{
					shottama(&tama[i], 300);

					//�e���o������A���[�v�𔲂���
					break;
				}
			}
		}
		//�e�̔��ˑ҂�
		if (tamashotcnt < tamashotcntMAX)
		{
			tamashotcnt++;
		}
		else
		{
			tamashotcnt = 0;
		}
	}

	/* - - - - - - - - -  ����e - - - - - - - - - - - - - - */
	if (MouseDown(MOUSE_INPUT_RIGHT) == TRUE)
	{
		if (subtamashotcnt == 0)
		{
			for (int deg = 0; deg < 360; deg += 30)
			{
				//�e�𔭎�(�e�̕`��) 
				for (int i = 0; i < TAMA_MAX; i++)
				{
					if (tama[i].IsDraw == FALSE)
					{
						shottama(&tama[i], deg);

						//�e���o������A���[�v�𔲂���
						break;
					}
				}
			}
		}
		//�e�̔��ˑ҂�
		if (subtamashotcnt < subtamashotcntMAX)
		{
			subtamashotcnt++;
		}
		else
		{
			subtamashotcnt = 0;
		}
	}
	/* - - - - - - - - -  ����e - - - - - - - - - - - - - - */

	for (int i = 0; i < TAMA_MAX; i++)
	{
		//�`�悳��Ă���Ƃ�
		if (tama[i].IsDraw == TRUE)
		{
			//�e�̈ʒu
			//�@���S�ʒu�@�{�@��΂��p�x����΂��������v�Z�@���@����
			tama[i].x = tama[i].Startx + cos(tama[i].degree * DX_PI / 180.0f) * tama[i].radius;
			tama[i].y = tama[i].Starty + sin(tama[i].degree * DX_PI / 180.0f) * tama[i].radius;

			//���a�𑫂�
			tama[i].radius += tama[i].spped;


			//���̓����蔻����X�V
			collUpdateTama(&tama[i]);

			//��ʊO�ɏo����A�`�悵�Ȃ�
			if (tama[i].y + tama[i].height < 0 ||	//��ʊO�i��j
				tama[i].y > GAME_HEIGHT ||			//��ʊO�i���j
				tama[i].x + tama[i].width < 0 ||	//��ʊO�i���j
				tama[i].x > GAME_WIDTH)				//��ʊO�i�E�j
			{
				tama[i].IsDraw = FALSE;
			}
		}
	}

	if (tekiAddcnt < tekiAddcntMax)
	{
		tekiAddcnt++;
	}

	else {
		tekiAddcnt = 0;

		//�G�̕`��i�����j
		for (int i = 0; i < TEKI_MAX; i++)
		{
			//�`�悳��Ă��Ȃ��G��T��
			if (teki[i].img.IsDraw == FALSE)
			{
				int Bunkatu = 10;

				if (score < 1000)
				{
					teki[i] = teki_moto[3];
				}
				else if (score < 2000)
				{
					teki[i] = teki_moto[1];
				}
				else if (score < 5000)
				{
					teki[i] = teki_moto[GetRand(TEKI_KIND - 1)];
				}
				//GetRand()��()���̐�������O�܂Ń����_���Ŏ擾
				teki[i].img.x = GetRand(Bunkatu - 1) * GAME_WIDTH / Bunkatu;
				teki[i].img.y = -teki[i].img.height;

				teki[i].img.IsDraw = TRUE;
				break;
			}
		}
	}

	//�G�̏���
	for (int i = 0; i < TEKI_MAX; i++)
	{
		if (teki[i].img.IsDraw == TRUE)
		{
			teki[i].img.y += 1;	//���ֈړ�

			collUpdateenemy(&teki[i]);

			//�G�����܂Ō������Ƃ��\�����~�߂�
			if (teki[i].img.y > GAME_HEIGHT)
			{
				teki[i].img.IsDraw = FALSE;
			}

			//�G�ɒe�������������A�G�͕\�����~�߂�
			for (int cnt = 0; cnt < TAMA_MAX; cnt++)
			{
				//�`�悳��Ă���Ƃ�
				if (tama[cnt].IsDraw == TRUE)
				{
					if (colltouch(teki[i].coll, tama[cnt].coll) == TRUE)
					{
						tama[cnt].IsDraw = FALSE;		//�e�̕`����~�߂�
						teki[i].img.IsDraw = FALSE;	//�G�̕`����~�߂�

						score += 100;				//�X�R�A�����Z����
					}
				}
			}
		}
	}

	return;
}

/// <summary>
/// �����΂�
/// </summary>
/// <param name="tama"></param>
VOID shottama(TAMA* tama, float deg)
{
	//���𔭎�(�`��)
	tama->IsDraw = TRUE;

	//���̔��ˁi�`��)�ʒu
	tama->Startx = player.img.x + player.img.width / 2 - tama->width / 2;
	tama->Starty = player.img.y;


	//���̑��x��ς���
	tama->spped = 6;

	//���̊p�x
	tama->degree = deg;

	//���̔��a
	tama->radius = 0.0f;

	//�����蔻����X�V
	collUpdateTama(tama);
}


/// <summary>
/// �v���C��ʁ@�`��
/// </summary>
VOID PlayDraw(VOID)
{
	//�w�i�̕`��
	for (int i = 0; i < 2; i++)
	{
		//�w�i�̕`��
		DrawGraph(back[i].x, back[i].y, back[i].handle, TRUE);

		//�摜�����܂Ői�񂾎�
		if (back[i].y > GAME_HEIGHT)
		{
			back[i].y = -back[i].height + 1;

		}
		back[i].y++;
	}

	//�G�̕`��
	for (int i = 0; i < TEKI_MAX; i++)
	{
		/*
		if (teki[i].img.IsDraw == TRUE)
		{
			DrawGraph(teki[i].img.x, teki[i].img.y, teki[i].img.handle, TRUE);
		}
		if (GAME_DEBUG == TRUE)
		{
			DrawBox(
				teki[i].coll.left, teki[i].coll.top, teki[i].coll.right, teki[i].coll.bottom,
				GetColor(0, 0, 255), FALSE);
		}
		*/

		//�G���`��ł���Ƃ���
		if (teki[i].img.IsDraw == TRUE)
		{
			DrawGraph(teki[i].img.x, teki[i].img.y, teki[i].img.handle, TRUE);


			if (GAME_DEBUG == TRUE)
			{
				DrawBox(
					teki[i].coll.left, teki[i].coll.top, teki[i].coll.right, teki[i].coll.bottom,
					GetColor(0, 0, 255), FALSE);
			}
		}
	}

	//�v���C���[�̕`��
	if (player.img.IsDraw == TRUE)
	{
		DrawGraph(player.img.x, player.img.y, player.img.handle, TRUE);

		if (GAME_DEBUG == TRUE)
		{
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}

	//�e�̕`��
	for (int i = 0; i < TAMA_MAX; i++)
	{
		if (tama[i].IsDraw == TRUE)
		{
			DrawTama(&tama[i]);

			if (GAME_DEBUG == TRUE)
			{
				DrawBox(tama[i].coll.left, tama[i].coll.top, tama[i].coll.right, tama[i].coll.bottom,
					GetColor(255, 0, 0), FALSE);
			}
		}
	}

	//�X�R�A�̕`��
	int old = GetFontSize();//�ȑO�̃t�H���g�̃T�C�Y���擾���Ă���
	SetFontSize(40);		//�t�H���g��傫������

	DrawFormatString(0, 50, GetColor(0, 255, 0), "SCORE:%05d", score);

	SetFontSize(old);		//�t�H���g�����ɖ߂�

	//�}�E�X�̈ʒu��`��
	MouseDraw();

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
		StopSoundMem(EndBGM.handle);


		ChangeScene(GAME_SCENE_TITLE);

		return;
	}
	if (CheckSoundMem(EndBGM.handle) == 0)
	{
		PlaySoundMem(EndBGM.handle, EndBGM.playType);
	}

	return;
}
/// <summary>
/// �G���h��� �`��
/// </summary>
VOID EndDraw(VOID)
{
	DrawGraph(finish[0].x, finish[0].y, finish[0].handle, TRUE);

	DrawGraph(EndLogo.x, EndLogo.y, EndLogo.handle, TRUE);
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
	chara->coll.left = chara->img.x + 10;		//�����蔻��������
	chara->coll.top = chara->img.y + 10;		//�����蔻��������
	chara->coll.right = chara->img.x + chara->img.width - 10;	//�����蔻��������
	chara->coll.bottom = chara->img.y + chara->img.height - 10;	//�����蔻��������

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

VOID collUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

/// <summary>
/// ��`���m�̓����蔻��
/// </summary>
/// <param name="a">���炩�̋�`"a"</param>
/// <param name="b">���炩�̋�`"b"</param>
/// <returns>����������TRUE / ������Ȃ����FALSE</returns>
BOOL colltouch(RECT a, RECT b)
{
	if (
		a.left < b.right &&	//p�̍���x < g�̉E��x���W
		a.right > b.left &&	//p�̉E��x < g�̍���x���W
		a.bottom > b.top &&	//p�̏��y < g�̉���y���W
		a.top < b.bottom	//p�̉���y < g�̏��y���W
		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/// <summary>
/// �^�C�g�����S�̓ǂݍ���
/// </summary>
/// <param name="image"></param>
/// <param name="path"></param>
/// <returns></returns>
BOOL LoadImageMem(IMAGE* image, const char* path)
{
	//�摜��ǂݍ���
	strcpyDx(image->path, path);
	image->handle = LoadGraph(image->path);	//�摜��ǂݍ���

	//�摜���ǂݍ��߂Ȃ������Ƃ��A�G���[�i�[�P�j������
	if (image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//���C���̃E�B���h�E�n���h��
			image->path,				//���b�Z�[�W�{��
			"�摜�ǂݍ��݃G���[�I",	//���b�Z�[�W�^�C�g��
			MB_OK					//�{�^��
		);
		return FALSE;				//�G���[�I��
	}

	//�摜�̕��ƍ���������
	GetGraphSize(image->handle, &image->width, &image->height);
	return TRUE;

}