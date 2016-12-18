#pragma once

// 乱数シードを初期化するかのフラグ
#define USE_RANDOM_SEED				(TRUE)

// 中間データを表示するかのフラグ
#define FSHOW_LOG					(TRUE)

// 教師データの相対パス
#define PATH_TRAINING_DATA			(_T("data/Training/\0"))

// テストデータの相対パス
#define PATH_TEST_DATA				(_T("data/Test/\0"))

// 教師データ、テストデータを含むファイル数の最大値
#define MAX_FILES					(1)

// 教師データの行数
#define NUMBER_OF_TRAINING_DATA		(140)

// 教師データの列数
#define MAX_COLUMN					(409)

// テストデータの行数
#define NUMBER_OF_TEST_DATA			(260)

// テストデータに含まれるクラス1の数
#define LEN_CLASS1					(60)

// テストデータに含まれるクラス2の数
#define LEN_CLASS2					(200)

// 入力層のノード数
// 教師データの列数(特徴数のみ、クラスはカウントしない)
#define INPUTNO						(MAX_COLUMN - 1)

// 中間層の総数
// 3層NNなら1に設定、多層ならそれ以上に
#define NUMBER_OF_HIDDEN_LAYERS		(1)

// 中間層各層のノード数
#define NUMBER_OF_HIDDEN_NODES		(50)

// 出力層のノード数
#define NUMBER_OF_OUT_NODES			(1)

// バイアスの初期値
#define INITIAL_VALUE_OF_BIAS		(1.0f)

// 学習係数
#define ALPHA						(0.5)

// 乱数のシード
#define SEED						(65535)

// 誤差の初期値
#define INITIAL_ERR					(10)

// エラー率の下限、ネットワークの誤差がこれを下回れが学習を中断
// (エポック数との兼ね合いに注意)
#define LIMIT						(0.001)

// エポック数
#define EPOCH						(100)

// 閾値, テストデータの結果の判別に利用 
// (結果がこれを超えたらクラス1, 超えなかったらクラス2, など)
#define THRESHOLD					(0.5f)
