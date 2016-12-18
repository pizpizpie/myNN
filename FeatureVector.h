#pragma once

#include <windows.h>

#include "File_Name_List.h"
#include "Parameters.h"

using namespace std;

#define MAX_LINE							(2000)
#define BUF_SIZE							(15)

#define NUM_DIMENSIONS_OF_VECTOR			(3)
#define DIM_FILES							(0)
#define DIM_ROW								(1)
#define DIM_COL								(2)

// バッファを空にする
void Clear_Buf(char buf[], int *pbuf_pos);

class FVector {

private:

	vector<vector<vector<float>>> data;		// 特徴
	int num_row;							// データのパターン数(ファイルの行数に等しい)

public:
	explicit FVector(int num_row_) : num_row(num_row_) {

		data.resize(MAX_FILES);

		for(int i = 0; i < MAX_FILES; i++) {

			data[i].resize(num_row_);

			for(int j = 0; j < num_row_; j++) {
				data[i][j].resize(MAX_COLUMN);
			}
		}
	}

	void Assign(int num_files, int row, int col, double value) {

		data[num_files][row][col] = value;
	}

	double Get_Data(int num_files, int row, int col) {

		return data[num_files][row][col];
	}
};

// 特徴ベクトルに関するクラス
// データファイルの読み取りを行う
class FeatureVector {

private:

	FVector vector;											// 特徴ベクトル
	unsigned long dimensions[NUM_DIMENSIONS_OF_VECTOR];		// 特徴ベクトルの各軸の次元数を記録する
	int num_row;											// パターン数

public:

	FeatureVector(int num_row_);
	HRESULT Load(char path[], File_Name_List file_list);
	void Register_col(int num_files, int row, int col, char buf[], int buf_pos);
	void Add(int num_files, int row, int col, double value);
	void Clear();
	void Show_Data();
	void Show_Dimensionality();
	int Get_Specified_Dimention(int d);
	double Get_Specified_Feature(int num_files, int row, int col);
};

FeatureVector::FeatureVector(int num_row_) : vector(num_row_) {
		num_row = num_row_;
		Clear();
}

// データの読み取り
HRESULT FeatureVector::Load(char path[], File_Name_List file_list) {

	FILE *fp = NULL;
	errno_t error_no;

	std::cout << "______________________ "  << std::endl;
	std::cout << "Reading Files from " << path << std::endl;
	std::cout << "______________________ "  << std::endl;

	dimensions[DIM_FILES] = file_list.Get_Length();

	for(int i = 0; i < dimensions[DIM_FILES]; i++) {

		// path + ファイル名を入れる文字列
		char reading_file[MAX_DIR_NAME + MAX_PATH] = {};

		strcpy(reading_file, path);
		strcat(reading_file, file_list.Get_Files(i)->Get_Name());
		strcat(reading_file, "\0");

		if((error_no = fopen_s(&fp, reading_file, "r")) != 0) {

			fclose(fp);
			std::cout << reading_file << ": This network doesn't have more than 2 hidden layers." << std::endl;
			return E_FAIL;
		}

		// ファイルを正常に読み込めた
		else {

			std::cout << "Reading:" << reading_file << std::endl;

			// 行数と列数
			int row = 0, col = 0;

			// ファイルから一行ずつ読み込み
			char line[MAX_LINE];

			for(; fgets(line, sizeof(line), fp) != NULL;) {

				// バッファ、列の内容を一時記録する配列
				char buf[BUF_SIZE];

				// バッファの配列内の位置を保存する
				int buf_pos = 0;

				Clear_Buf(buf, &buf_pos);

				// 読み込んだ行を調べる
				for(int j=0; line[j] != '\0'; j++) {

					if(line[j] == '\n') {

						Register_col(i, row, col, buf, buf_pos);

						// 次の列の移行する(次元数の記録に必要)
						col++;

						// 次の行に移行する
						row++;

						// 行数と列数を特徴ベクトルの次元数として記録
						dimensions[DIM_ROW] = row;
						dimensions[DIM_COL] = col;

						col = 0;

						Clear_Buf(buf, &buf_pos);
					}

					// ひとつのセル（列）の終わりに来た場合、一時記録配列の中身をノードに与える
					// line[j] == 13はCRの意
					if(line[j] == ' ' || line[j] == 13) {

						Register_col(i, row, col, buf, buf_pos);

						// 次の列に移行する
						col++;

						Clear_Buf(buf, &buf_pos);
					}

					// 列の終わりまで来ていない(空白でも'\n'でもない)場合、bufに内容を保存し続ける
					else {
						// 文字列の大きさがバッファの容量をオーバーする場合、文字列の後ろは無視する
						// (BUF_SIZE-2)は、バッファの最後に改行コードとNULL文字が入るため、容量を空けておく必要がある
						if(buf_pos < (BUF_SIZE-2)) {
							buf[buf_pos] = line[j];
							buf_pos++;
						}
					}
				}
			}

			fclose(fp);
		}
	}

	// 正常に全てのファイルをディレクトリから読み込めた
	return S_OK;
}

// ファイルから読み込んだ情報を特徴ベクトルに入れる
void FeatureVector::Register_col(int num_files, int row, int col, char buf[], int buf_pos) {

	// 引数にbufを取っているが、bufに処理を加えると呼び出し元の関数の方でも値が変わってしまうためtmpを置く
	char tmp[BUF_SIZE+1];

	strcpy(tmp, buf);

	// バッファには単語のみでNULL文字が入っていない状態で送られてくるため、これを添付する
	tmp[buf_pos] = '\0';

	// 単語リストに登録する
	Add(num_files, row, col, atof(tmp));
}

// 特徴ベクトルに特徴を追加する
void FeatureVector::Add(int num_files, int row, int col, double value) {

	vector.Assign(num_files, row, col, value);
}

// 特徴ベクトルを空にする
void FeatureVector::Clear() {

	for(int i = 0; i < NUM_DIMENSIONS_OF_VECTOR; i++) {
		dimensions[i] = 0.0f;
	}

	for(int i = 0; i < MAX_FILES; i++) {
		for(int j = 0; j < num_row; j++) {
			for(int k = 0; k < MAX_COLUMN; k++) {
				vector.Assign(i, j, k, 0.0f);
			}
		}
	}
}

// 特徴ベクトルの中身を表示
void FeatureVector::Show_Data() {

	for(int i = 0; i < dimensions[DIM_FILES]; i++) {

		std::cout << "_____________________" << std::endl;
		std::cout << "File Number: \t" << i << std::endl;

		for(int j = 0; j < dimensions[DIM_ROW]; j++) { 
			for(int k = 0; k < dimensions[DIM_COL]; k++) {

				std::cout << vector.Get_Data(i, j, k) << std::endl;
			}
			std::cout << std::endl;
		}
		std::cout << "_____________________" << std::endl;
	}
}

// 特徴ベクトルの次元数を表示
// 読み込んだファイル数、ベクトルの行数、ベクトルの列数
void FeatureVector::Show_Dimensionality() {

	std::cout << "_____________________" << std::endl;

	std::cout << "This Feature Vector has <file = "<< dimensions[DIM_FILES] << ", " 
	<< "row = " << dimensions[DIM_ROW] << ", " 
	<< "col = " << dimensions[DIM_COL] << "> dimensions"
	<< std::endl;
		
	std::cout << "_____________________" << std::endl;

	getch();
}

// 特徴ベクトルの特定の次元数を取得
int FeatureVector::Get_Specified_Dimention(int d) {

	if(d > NUM_DIMENSIONS_OF_VECTOR) {
		return -1;
	}

	return dimensions[d];
}

// 特徴ベクトルから特徴を取ってくる
double FeatureVector::Get_Specified_Feature(int num_files, int row, int col) {

	return vector.Get_Data(num_files, row, col);
}

void Clear_Buf(char buf[], int *pbuf_pos) {

	// バッファの位置を最初に戻す
	*pbuf_pos = 0;

	// バッファを空にする。これを行わないと前の列の情報が次のデータに影響してしまうことがある
	for(int i=0; i<BUF_SIZE; i++)
		buf[i] = 0;
}
