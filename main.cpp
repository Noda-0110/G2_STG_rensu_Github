//#CRT_SECURE_NO_WARNINGS
//ヘッダファイル読み込み
#include "game.h"		//ゲームを管理する為のヘッダファイル　あくまでもゲームの管理
#include "keyboard.h"	//キーボードの処理
#include "FPS.h"		//FPSの処理

#include <math.h>

//マクロ定義
#define TAMA_DIV_MAX 4	//球の画像の最大数
#define TAMA_MAX 100	//球の画像の最大数
#define TEKI_KIND 8		//敵の画像の種類
#define TEKI_MAX 10		//敵の画像の総数

//構造体の定義

//画像の構造体
struct IMAGE
{
	int handle = -1;	//画像のハンドル（管理番号）
	char path[255];		//画像の場所（パス）

	int x;				//X位置
	int y;				//Y位置
	int width;			//幅
	int height;			//高さ

	BOOL IsDraw = FALSE;//画像が描画できるか
};

//キャラクタの構造体
struct CHARACTOR
{
	IMAGE img;			//画像構造体
	int speed = 1;		//移動速度
	RECT coll;			//上下左右当たり判定の領域 RECTは四角形の位置を扱える
};

//動画の構造体
struct MOVIE
{
	int handle = -1;	//動画のハンドル
	char path[255];		//動画のパス

	int x;				//ｘ位置
	int y;				//ｙ位置
	int width;			//幅
	int height;			//高さ
	int Volume = 255;	//ボリューム（最小）0～255（最大）
};

//音楽の構造体
struct AUDIO
{
	int handle = -1;	//音楽のハンドル
	char path[255];		//音楽のパス

	int volume = -1;	//ボリューム（MIN 0　～　２５５　MAX）
	int playType = -1;
};

//球の構造体
struct TAMA
{
	int handle[TAMA_DIV_MAX];	//画像のハンドル
	char path[255];	//画像のパス

	int DivTate;	//分割数（縦）
	int DivYoko;	//分割数（横）
	int DivMax;		//分割総数

	int AnimeCnt = 0;		//アニメーションカウンタ
	int AnimeCntMAX = 0;	//アニメーションカウンタマックス

	int NowIndex = 0;		//現在の画像の要素数

	int Startx;		//X最初位置
	int Starty;		//Y最初位置

	float radius;	//半径
	float degree;	//角度

	int x;			//xの位置
	int y;			//yの位置
	int width;		//幅
	int height;		//高さ
	int spped;		//球の速さ

	RECT coll;				//当たり判定（矩形）
	BOOL IsDraw = FALSE;	//描画できるか
};

//グローバル変数
//シーンを管理する変数
GAME_SCENE GameScene;		//現在のゲームのシーン
GAME_SCENE OldGameScene;	//前回もゲームのシーン
GAME_SCENE NextGameScene;	//次のゲームのシーン


//画面の切り替え
BOOL IsFadeOut = FALSE;		//フェードアウト
BOOL IsFadeIn = FALSE;		//フェードイン

int FadeTimeMill = 2000;					//切り替えミリ秒
int FadeTimeMax = FadeTimeMill / 1000 * 60;	//ミリ秒をフレーム病に変換

//フェードアウト
int FadeOutCntInit = 0;				//初期値
int FadeOutCnt = FadeOutCntInit;	//フェードアウトのカウンタ
int FadeOutCntMax = FadeTimeMax;	//フェードアウトのカウンタMAX

//フェードイン
int FadeInCntInit = FadeTimeMax;	//初期値
int FadeInCnt = FadeInCntInit;		//フェードインのカウンタ
int FadeInCntMax = FadeTimeMax;		//フェードインのカウンタMAX

//球の構造体変数
struct TAMA tama_moto;				//元
struct TAMA tama[TAMA_MAX];			//実際に使う

//球の発射カウント
int tamashotcnt = 0;
int tamashotcntMAX = 5;

//プレイヤー構造体宣言
CHARACTOR player;

//背景画像(シームレスだから２つ)
IMAGE back[2];

//敵データ(弾と同じく元となるもの)
CHARACTOR teki_moto[TEKI_KIND];
//実敵データ
CHARACTOR teki[TEKI_MAX];

char tekiPath[TEKI_KIND][255] =
{
	{".\\\\"},
	{".\\\\"},
	{".\\\\"},
	{".\\\\"},
	{".\\\\"},
	{".\\\\"},
	{".\\\\"},
	{".\\\\"},
};

//プロトタイプ宣言
VOID Title(VOID);		//タイトル画面
VOID TitleProc(VOID);	//タイトル画面（処理）
VOID TitleDraw(VOID);	//タイトル画面（描画）

VOID Play(VOID);		//プレイ画面
VOID PlayProc(VOID);	//プレイ画面（処理）
VOID PlayDraw(VOID);	//プレイ画面（描画）

VOID End(VOID);			//エンド画面
VOID EndProc(VOID);		//エンド画面（処理）
VOID EndDraw(VOID);		//エンド画面（描画）

VOID Change(VOID);		//切り替え画面
VOID ChangeProc(VOID);	//切り替え画面（処理）
VOID ChangeDraw(VOID);	//切り替え画面（描画）


VOID ChangeScene(GAME_SCENE scene);						//シーン切り替え

VOID collUpdateplayer(CHARACTOR* chara);				//当たり判定の領域を更新
VOID collUpdateTama(TAMA* tama);						//当たり判定の領域を更新
VOID collUpdateenemy(CHARACTOR* chara);					//当たり判定の領域を更新


BOOL colltouch(RECT a, RECT b);					//当たり判定の触れているか触れていないかの判定
BOOL GameLoad(VOID);									//ゲームデータの読み込み
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType);	//音楽の読み込み
BOOL LoadImageMem(IMAGE* image, const char* path);

VOID GameInit(VOID);									//ゲームの初期化

VOID DrawTama(TAMA* tama);								//球の描画
VOID shottama(TAMA* tama, float deg);					//球の発射

BOOL LoadImageDivMem(int* handle, const char* path, int bunkatuYoko, int bunkatuTate);	//ゲームの画像の分割読み込み


// プログラムは WinMain から始まります
//Windowsのプログラミング法で動いている
//DxLibは、DirectXという、ゲームプログラミングを簡単に扱える仕組み
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	//追加分
	SetOutApplicationLogValidFlag(FALSE);	//Log.txtを出力しない

	ChangeWindowMode(TRUE);					//ウィンドウモードに設定
	SetMainWindowText(GAME_TITLE);			//ウィンドウのタイトル文字
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//ウィンドウの解像度を設定
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//ウィンドウの大きさを設定
	SetBackgroundColor(255, 255, 255);					//デフォルトの背景色
	SetWindowIconID(GAME_ICON_ID);						//アイコンファイルの読み込み

	//追加分
	SetWindowStyleMode(GAME_WINDOW_BAR);	//ウィンドウバーの状態
	SetWaitVSyncFlag(TRUE);					//ディスプレイの垂直同期を有効にする
											//高度な処理や描画を行う時にはFALSE

	SetAlwaysRunFlag(TRUE);					//ウィンドウをずっとアクティブにする


	if (DxLib_Init() == -1)		//ＤＸライブラリ初期化処理　-1が帰ってきたら強制終了
	{
		// エラーが起きたら直ちに終了
		return -1;
	}

	//ダブルバッファリングを有効
	SetDrawScreen(DX_SCREEN_BACK);


	//最初のシーンはタイトル画面
	GameScene == GAME_SCENE_TITLE;

	//ゲーム全体の初期化

	//ゲームの読み込み
	if (!GameLoad())
	{
		//データの読み込み
		DxLib_End();	//DXLib終了
		return -1;		//異常終了
	}

	//ゲームの初期化
	GameInit();

	//無限ループ　受け取り続ける
	while (1)
	{

		if (ProcessMessage() != 0) { break; }	//メッセージを受け取り続ける
		if (ClearDrawScreen() != 0) { break; }	//画面を消去する

		//キーボードの入力更新
		AllKeyUpdate();

		//FPS値の更新
		FPSUpdate();

		//ESCキーで強制終了
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }


		//DrawCircle(X, Y, radius, GetColor(255, 255, 0), TRUE); 円が一番下


		//以前のシーンを所得
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//シーンごとに処理を行う
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();	//タイトル画面
			break;
		case GAME_SCENE_PLAY:
			Play();		//プレイ画面
			break;
		case GAME_SCENE_END:
			End();		//エンド画面
			break;
		case GAME_SCENE_CHANGE:
			Change();	//切り替え画面
			break;
		default:
			break;
		}

		//シーンを切り替える
		if (OldGameScene != GameScene)
		{
			//現在シーンが切り替え画面（タイトルプレイエンドの画面）でないとき
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		//次のシーンを保存
				GameScene = GAME_SCENE_CHANGE;	//画面切り替えシーンに変える
			}
		}

		//FPS値を描画
		FPSDraw();
		//FPS処理を待つ
		FPSWait();

		ScreenFlip();	//ダブルバッファリングした画面を描画
	}
	//読み込んだ画像を開放
	for (int i = 0; i < TAMA_DIV_MAX; i++) { DeleteGraph(tama_moto.handle[i]); }

	//プレイヤーを開放
	DeleteGraph(player.img.handle);

	//背景画像を開放
	DeleteGraph(back[0].handle);
	DeleteGraph(back[1].handle);
	// ＤＸライブラリ使用の終了処理（準備）
	DxLib_End();

	return 0;				// ソフトの終了（実行） 
}




// - - - -  - - - - データロード - - - -  - - - - //

/// <summary>
/// ゲームのデータを読み込む
/// </summary>
/// <returns>読み込めたらTRUE / 読み込めなかったらFALSE</returns>
BOOL GameLoad()
{
	//球の分割数を設定
	tama_moto.DivYoko = 4;
	tama_moto.DivTate = 1;

	//球のパスをコピー
	strcpyDx(tama_moto.path,".\\image\\dia_green.png");

	//画像を分割して読み込み
	if (LoadImageDivMem(&tama_moto.handle[0], ".\\image\\dia_green.png",tama_moto.DivYoko,tama_moto.DivTate) == FALSE) { return FALSE; }

	//幅と高さを所得
	GetGraphSize(tama_moto.handle[0], &tama_moto.width, &tama_moto.height);

	//位置を設定
	tama_moto.x = GAME_WIDTH / 2 - tama_moto.width / 2;		//中央ぞろえ
	tama_moto.y = GAME_HEIGHT - tama_moto.height;			//画面↓

	tama_moto.spped = 1;		//速度

	//アニメを変える速度
	tama_moto.AnimeCntMAX = 10;

	//当たり判定の更新
	collUpdateTama(&tama_moto);


	//画像の表示
	tama_moto.IsDraw = FALSE;

	//全ての球の情報をコピー
	for (int i = 0; i < TAMA_MAX; i++)
	{
		tama[i] = tama_moto;
	}

	//プレイヤーの読み込み
	if (LoadImageMem(&player.img, ".\\image\\player.png") == FALSE) { return FALSE; }
	player.img.x = GAME_WIDTH / 2 - player.img.width;
	player.img.y = GAME_HEIGHT / 2 - player.img.height;
	collUpdateplayer(&player);
	player.img.IsDraw == TRUE;

	//背景の読み込み
	if (LoadImageMem(&back[0], ".\\image\\hosi.png") == FALSE) { return FALSE; }
	player.img.x = 0;				//画像の左端
	player.img.y = -back[0].height;	//画像の高さ分、位置を上に（ー方向）挙げる
	player.img.IsDraw == TRUE;

	if (LoadImageMem(&back[1], ".\\image\\hosi_rev.png") == FALSE) { return FALSE; }
	player.img.x = 0;				//画像の左端
	player.img.y = 0;	//画像の高さ分、位置を上に（ー方向）挙げる
	player.img.IsDraw == TRUE;

	//敵の読み込み
	for (int i = 0; i < TEKI_KIND; i++)
	{
		if (LoadImageMem(&teki_moto[0].img, tekiPath[i]) == FALSE) { return FALSE; }
		teki_moto[i].img.x = GAME_WIDTH / 2 - teki_moto[i].img.width;
		teki_moto[i].img.y = -teki_moto[i].img.height;
		collUpdateplayer(&teki_moto[i]);
		teki_moto[i].img.IsDraw == TRUE;
	}
	return TRUE;	//全てを読み込めた
}
// - - - - - データロード - - - - - //

/// <summary>
/// 画像を分割してメモリに読みこみ
/// </summary>
/// <param name="handle">ハンドル配列の先頭アドレス</param>
/// <param name="path">画像のパス</param>
/// <param name="bunkatuYoko">分割するときの横の数</param>
/// <param name="bunkatuTate">分割するときの縦の数</param>
/// <returns></returns>
BOOL LoadImageDivMem(int* handle, const char* path, int bunkatuYoko, int bunkatuTate)
{
	//球の読み込み
	int IsTamaLoad = -1;

	//一時的に使うハンドルを用意する（画像サイズの読み込みのため）
	int Tamahandle = LoadGraph(path);

	if (Tamahandle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//ウインドウハンドル
			path,					//本文
			"画像読み込みエラー",	//タイトル
			MB_OK					//ボタン
		);
		return FALSE;
	}

	//画像の幅と高さを取得
	int TamaWidth = -1;
	int TamaHeight = -1;
	GetGraphSize(Tamahandle, &TamaWidth, &TamaHeight);

	//分割して読み込み
	IsTamaLoad = LoadDivGraph(
		path,						//画像のパス
		TAMA_DIV_MAX,					//分割総数
		bunkatuYoko, bunkatuTate,							//横の分割、縦の分割
		TamaWidth / bunkatuYoko, TamaHeight / bunkatuTate,	//画像一つ分の幅、高さ
		handle						//連続する先頭アドレス
	);



	//分割エラー
	if (Tamahandle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//ウインドウハンドル
			path,				//本文
			"画像分割エラー",	//タイトル
			MB_OK
		);
		return FALSE;
	}

	//一時的に読み込んだハンドルを開放する
	DeleteGraph(Tamahandle);

	return TRUE;
}

VOID GameInit(VOID)
{
	player.img.x = GAME_WIDTH / 2 - player.img.width;
	player.img.y = GAME_HEIGHT / 2 - player.img.height;
	collUpdateplayer(&player);
	player.img.IsDraw = TRUE;

	//背景画像の設定1
	back[0].x = 0;
	back[0].y = -back[0].height;
	back[0].IsDraw = TRUE;

	//背景画像の設定1
	back[1].x = 0;
	back[1].y = 0;
	back[1].IsDraw = TRUE;

}

/// <summary>
/// 音楽をメモリへと読み込む
/// </summary>
/// <param name="audio">Audio構造体のアドレス &～～～</param>
/// <param name="path">Audioのパス　.\\Audio\\～～～</param>
/// <param name="volume">ボリューム</param>
/// <param name="playType">DX_PLAYTYPE_LOOP or DX_PLAYTYPE_BACK</param>
/// <returns></returns>
BOOL LoadAudio(AUDIO* audio, const char* path, int volume, int playType)
{
	strcpyDx(audio->path,path);
	audio->handle = LoadSoundMem(audio->path);

	//音楽を読み込みができなかった
	if (audio->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			audio->path,				//メッセージ本文
			"音楽読み込みエラー！",	//メッセージタイトル
			MB_OK					//ボタン
		);
		return FALSE;				//エラー終了
	}

	audio->volume = volume;
	audio->playType = playType;

	return TRUE;
}

// ------------  シーン関数　　------------ //
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;	//シーンを切り替え
	IsFadeIn = FALSE;	//フェードインしない
	IsFadeOut = TRUE;	//フェードアウトする

	return;
}

/// <summary>
/// タイトル画面
/// </summary>
VOID Title(VOID)
{
	TitleProc();	//処理
	TitleDraw();	//描画

	return;
}
/// <summary>
/// タイトル画面　処理
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
/// タイトル画面　処理 
/// </summary>
VOID TitleDraw(VOID)
{
	DrawString(0, 0, "タイトル画面", GetColor(255, 0, 0));
	return;
}

/// <summary>
/// 球の描画
/// </summary>
/// <param name="tama">球の構造体</param>
VOID DrawTama(TAMA* tama)
{
	//球が描画できるとき
	if (tama->IsDraw == TRUE)
	{
		//球の描画
		DrawGraph(tama->x, tama->y, tama->handle[tama->NowIndex], TRUE);

		//画像を変えるタイミング
		if (tama->AnimeCnt < tama->AnimeCntMAX)
		{
			tama->AnimeCnt++;
		}
		else
		{
			//球の添え字が球の分割数の最大よりも少ない
			if (tama->NowIndex < TAMA_DIV_MAX - 1)
			{
				tama->NowIndex++;		//次の画像へ
			}
			else
			{
				tama->NowIndex = 0;		//最初に戻す
			}
			tama->AnimeCnt = 0;
		}
	}
}

/// <summary>
/// プレイ画面
/// </summary>
VOID Play(VOID)
{
	PlayProc();
	PlayDraw();
	return;
}
/// <summary>
/// プレイ画面　処理
/// </summary>
VOID PlayProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{

		ChangeScene(GAME_SCENE_END);

		return;
	}

	//プレイヤーを操作
	if (KeyDown(KEY_INPUT_LEFT) == TRUE)
	{
		if (player.img.x - player.speed >= 0)
		{
			player.img.x -= player.speed;
		}
	}
	//プレイヤーを操作
		if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
	{
		if (player.img.x + player.speed <= GAME_WIDTH)
		{
			player.img.x += player.speed;
		}

	}
	//プレイヤーを操作
		if (KeyDown(KEY_INPUT_UP) == TRUE)
	{
		if (player.img.y - player.speed >= 0)
		{
			player.img.y -= player.speed;
		}
	}
		//プレイヤーを操作
	if (KeyDown(KEY_INPUT_DOWN) == TRUE)
	{
		if (player.img.y + player.speed <= GAME_HEIGHT)
		{
			player.img.y += player.speed;
		}
	}

	collUpdateplayer(&player);

	if (KeyDown(KEY_INPUT_SPACE) == TRUE)
	{
		if (tamashotcnt == 0)
		{
			//弾を発射する(弾を描画する) 
			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (tama[i].IsDraw == FALSE)
				{
					shottama(&tama[i], 240);

					//弾を出したら、ループを抜ける
					break;
				}
			}

			//弾を発射(弾の描画) 
			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (tama[i].IsDraw == FALSE)
				{
					shottama(&tama[i], 270);

					//弾を１発出したら、ループを抜ける
					break;
				}
			}

			//弾を発射(弾の描画) 
			for (int i = 0; i < TAMA_MAX; i++)
			{
				if (tama[i].IsDraw == FALSE)
				{
					shottama(&tama[i], 300);

					//弾を出したら、ループを抜ける
					break;
				}
			}

		}

		//弾の発射待ち
		if (tamashotcnt < tamashotcntMAX)
		{
			tamashotcnt++;
		}
		else
		{
			tamashotcnt = 0;
		}
	}


	for (int i = 0; i < TAMA_MAX; i++)
	{
		//描画されているとき
		if (tama[i].IsDraw == TRUE)
		{
			//弾の位置
			//　中心位置　＋　飛ばす角度→飛ばす距離を計算　＊　距離
			tama[i].x = tama[i].Startx + cos(tama[i].degree * DX_PI / 180.0f) * tama[i].radius;
			tama[i].y = tama[i].Starty + sin(tama[i].degree * DX_PI / 180.0f) * tama[i].radius;

			//半径を足す
			tama[i].radius += tama[i].spped;


			//球の当たり判定を更新
			collUpdateTama(&tama[i]);

			//画面外に出たら、描画しない
			if (tama[i].y + tama[i].height < 0 ||	//画面外（上）
				tama[i].y > GAME_HEIGHT ||			//画面外（下）
				tama[i].x + tama[i].width < 0 ||	//画面外（左）
				tama[i].x > GAME_WIDTH)				//画面外（右）
			{
				tama[i].IsDraw = FALSE;
			}
		}
	}

	return;
}

/// <summary>
/// 球を飛ばす
/// </summary>
/// <param name="tama"></param>
VOID shottama(TAMA* tama,float deg)
{
		//球を発射(描画)
		tama->IsDraw = TRUE;

		//球の発射（描画)位置
		tama->Startx = player.img.x + player.img.width / 2 - tama->width / 2;
		tama->Starty = player.img.y;


		//球の速度を変える
		tama->spped = 6;

		//球の角度
		tama->degree = deg;

		//球の半径
		tama->radius = 0.0f;

		//当たり判定を更新
		collUpdateTama(tama);
}


/// <summary>
/// プレイ画面　描画
/// </summary>
VOID PlayDraw(VOID)
{
	//背景の描画
	for (int i = 0; i < 2; i++)
	{
		//背景の描画
		DrawGraph(back[i].x, back[i].y, back[i].handle, TRUE);

		//画像が下まで進んだ時
		if (back[i].y > GAME_HEIGHT)
		{
			back[i].y = -back[i].height + 1;
		}
		back[i].y += 5 ;
	}
	//プレイヤーの描画
	if (player.img.IsDraw == TRUE)
	{
		DrawGraph(player.img.x, player.img.y, player.img.handle, TRUE);

		if (GAME_DEBUG == TRUE)
		{
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}

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
	DrawString(0, 0, "プレイ画面", GetColor(0, 0, 0));
	return;
}


/// <summary>
/// エンド画面
/// </summary>
VOID End(VOID)
{
	EndProc();
	EndDraw();
	return;
}
/// <summary>
/// エンド画面　処理
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
/// エンド画面 描画
/// </summary>
VOID EndDraw(VOID)
{
	DrawString(0, 0, "エンド画面", GetColor(0, 0, 0));
	return;
}


/// <summary>
/// 切り替え画面
/// </summary>
VOID Change(VOID)
{
	ChangeProc();
	ChangeDraw();
	return;
}
/// <summary>
/// 切り替え画面 処理
/// </summary>
VOID ChangeProc(VOID)
{
	//フェードイン
	if (IsFadeIn == TRUE)
	{
		if (FadeInCnt > FadeInCntMax)
		{
			FadeInCnt--;	//カウンタを減らす、透明にしていく
		}
		else
		{
			//フェードイン処理の終了時 透明カウンタのMAXを超えたとき
			FadeInCnt = FadeInCntInit;	//カウンタ初期化
			IsFadeIn = FALSE;			//フェードイン処理の終了
		}

	}
	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		if (FadeOutCnt < FadeOutCntMax)
		{
			FadeOutCnt++;	//カウンタを増やす、色を濃くにしていく
		}
		else
		{
			//フェードイン処理の終了時 透明カウンタのMAXを超えたとき
			FadeOutCnt = FadeOutCntInit;	//カウンタ初期化
			IsFadeOut = FALSE;			//フェードイン処理の終了
		}

	}

	//切り替え処理終了？
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//フェードアウトもフェードインもしていない
		GameScene = NextGameScene;	//次のシーンに切り替え
		OldGameScene = GameScene;	//以前のシーンに更新
	}
	return;
}

/// <summary>
/// 切り替え画面　描画
/// </summary>
VOID ChangeDraw(VOID)
{
	//以前のシーンを描画
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();	//タイトル画面の描画
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();		//プレイ画面の描画
		break;
	case GAME_SCENE_END:
		EndDraw();		//エンド画面の描画
		break;
	default:
		break;
	}

	//フェードイン
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)FadeInCnt / FadeInCntMax) * 255);	//パーセンテージ

	}
	//フェードアウト
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)FadeOutCnt / FadeOutCntMax) * 255);	//パーセンテージ

	}

	//四角を描画
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//半透明終了
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "切り替え画面", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// 当たり判定の更新(プレイヤー専用)
/// </summary>
/// <param name="coll">当たり判定の領域</param>
VOID collUpdateplayer(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;		//当たり判定を微調整
	chara->coll.top = chara->img.y;		//当たり判定を微調整
	chara->coll.right = chara->img.x + chara->img.width;	//当たり判定を微調整
	chara->coll.bottom = chara->img.y + chara->img.height;	//当たり判定を微調整

	return;
}

/// <summary>
/// 当たり判定の更新(ゴール用)
/// </summary>
/// <param name="coll">当たり判定の領域</param>
VOID collUpdateTama(TAMA* tama)
{
	tama->coll.left = tama->x;
	tama->coll.top = tama->y;

	tama->coll.right = tama->x + tama->width;
	tama->coll.bottom = tama->y + tama->height;

	return;
}

/// <summary>
/// 当たり判定の更新(敵用)
/// </summary>
/// <param name="coll">当たり判定の領域</param>
VOID collUpdateenemy(CHARACTOR* chara)
{
	chara->coll.left = chara->img.x;
	chara->coll.top = chara->img.y;
	chara->coll.right = chara->img.x + chara->img.width;
	chara->coll.bottom = chara->img.y + chara->img.height;

	return;
}

/// <summary>
/// 矩形同士の当たり判定
/// </summary>
/// <param name="a">何らかの矩形"a"</param>
/// <param name="b">何らかの矩形"b"</param>
/// <returns>当たったらTRUE / 当たらなければFALSE</returns>
BOOL colltouch(RECT a,RECT b)
{
	if (
		a.left < b.right &&	//pの左辺x < gの右辺x座標
		a.right > b.left &&	//pの右辺x < gの左辺x座標
		a.bottom > b.top &&	//pの上辺y < gの下辺y座標
		a.top < b.bottom	//pの下辺y < gの上辺y座標
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
/// タイトルロゴの読み込み
/// </summary>
/// <param name="image"></param>
/// <param name="path"></param>
/// <returns></returns>
BOOL LoadImageMem(IMAGE* image, const char* path)
{
	//画像を読み込み
	strcpyDx(image->path, path);
	image->handle = LoadGraph(image->path);	//画像を読み込み

	//画像が読み込めなかったとき、エラー（ー１）が入る
	if (image->handle == -1)
	{
		MessageBox(
			GetMainWindowHandle(),	//メインのウィンドウハンドル
			image->path,				//メッセージ本文
			"画像読み込みエラー！",	//メッセージタイトル
			MB_OK					//ボタン
		);
		return FALSE;				//エラー終了
	}

	//画像の幅と高さを所得
	GetGraphSize(image->handle, &image->width, &image->height);
	return TRUE;

}