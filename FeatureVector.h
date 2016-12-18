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

// �o�b�t�@����ɂ���
void Clear_Buf(char buf[], int *pbuf_pos);

class FVector {

private:

	vector<vector<vector<float>>> data;		// ����
	int num_row;							// �f�[�^�̃p�^�[����(�t�@�C���̍s���ɓ�����)

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

// �����x�N�g���Ɋւ���N���X
// �f�[�^�t�@�C���̓ǂݎ����s��
class FeatureVector {

private:

	FVector vector;											// �����x�N�g��
	unsigned long dimensions[NUM_DIMENSIONS_OF_VECTOR];		// �����x�N�g���̊e���̎��������L�^����
	int num_row;											// �p�^�[����

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

// �f�[�^�̓ǂݎ��
HRESULT FeatureVector::Load(char path[], File_Name_List file_list) {

	FILE *fp = NULL;
	errno_t error_no;

	std::cout << "______________________ "  << std::endl;
	std::cout << "Reading Files from " << path << std::endl;
	std::cout << "______________________ "  << std::endl;

	dimensions[DIM_FILES] = file_list.Get_Length();

	for(int i = 0; i < dimensions[DIM_FILES]; i++) {

		// path + �t�@�C���������镶����
		char reading_file[MAX_DIR_NAME + MAX_PATH] = {};

		strcpy(reading_file, path);
		strcat(reading_file, file_list.Get_Files(i)->Get_Name());
		strcat(reading_file, "\0");

		if((error_no = fopen_s(&fp, reading_file, "r")) != 0) {

			fclose(fp);
			std::cout << reading_file << ": This network doesn't have more than 2 hidden layers." << std::endl;
			return E_FAIL;
		}

		// �t�@�C���𐳏�ɓǂݍ��߂�
		else {

			std::cout << "Reading:" << reading_file << std::endl;

			// �s���Ɨ�
			int row = 0, col = 0;

			// �t�@�C�������s���ǂݍ���
			char line[MAX_LINE];

			for(; fgets(line, sizeof(line), fp) != NULL;) {

				// �o�b�t�@�A��̓��e���ꎞ�L�^����z��
				char buf[BUF_SIZE];

				// �o�b�t�@�̔z����̈ʒu��ۑ�����
				int buf_pos = 0;

				Clear_Buf(buf, &buf_pos);

				// �ǂݍ��񂾍s�𒲂ׂ�
				for(int j=0; line[j] != '\0'; j++) {

					if(line[j] == '\n') {

						Register_col(i, row, col, buf, buf_pos);

						// ���̗�̈ڍs����(�������̋L�^�ɕK�v)
						col++;

						// ���̍s�Ɉڍs����
						row++;

						// �s���Ɨ񐔂�����x�N�g���̎������Ƃ��ċL�^
						dimensions[DIM_ROW] = row;
						dimensions[DIM_COL] = col;

						col = 0;

						Clear_Buf(buf, &buf_pos);
					}

					// �ЂƂ̃Z���i��j�̏I���ɗ����ꍇ�A�ꎞ�L�^�z��̒��g���m�[�h�ɗ^����
					// line[j] == 13��CR�̈�
					if(line[j] == ' ' || line[j] == 13) {

						Register_col(i, row, col, buf, buf_pos);

						// ���̗�Ɉڍs����
						col++;

						Clear_Buf(buf, &buf_pos);
					}

					// ��̏I���܂ŗ��Ă��Ȃ�(�󔒂ł�'\n'�ł��Ȃ�)�ꍇ�Abuf�ɓ��e��ۑ���������
					else {
						// ������̑傫�����o�b�t�@�̗e�ʂ��I�[�o�[����ꍇ�A������̌��͖�������
						// (BUF_SIZE-2)�́A�o�b�t�@�̍Ō�ɉ��s�R�[�h��NULL���������邽�߁A�e�ʂ��󂯂Ă����K�v������
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

	// ����ɑS�Ẵt�@�C�����f�B���N�g������ǂݍ��߂�
	return S_OK;
}

// �t�@�C������ǂݍ��񂾏�������x�N�g���ɓ����
void FeatureVector::Register_col(int num_files, int row, int col, char buf[], int buf_pos) {

	// ������buf������Ă��邪�Abuf�ɏ�����������ƌĂяo�����̊֐��̕��ł��l���ς���Ă��܂�����tmp��u��
	char tmp[BUF_SIZE+1];

	strcpy(tmp, buf);

	// �o�b�t�@�ɂ͒P��݂̂�NULL�����������Ă��Ȃ���Ԃő����Ă��邽�߁A�����Y�t����
	tmp[buf_pos] = '\0';

	// �P�ꃊ�X�g�ɓo�^����
	Add(num_files, row, col, atof(tmp));
}

// �����x�N�g���ɓ�����ǉ�����
void FeatureVector::Add(int num_files, int row, int col, double value) {

	vector.Assign(num_files, row, col, value);
}

// �����x�N�g������ɂ���
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

// �����x�N�g���̒��g��\��
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

// �����x�N�g���̎�������\��
// �ǂݍ��񂾃t�@�C�����A�x�N�g���̍s���A�x�N�g���̗�
void FeatureVector::Show_Dimensionality() {

	std::cout << "_____________________" << std::endl;

	std::cout << "This Feature Vector has <file = "<< dimensions[DIM_FILES] << ", " 
	<< "row = " << dimensions[DIM_ROW] << ", " 
	<< "col = " << dimensions[DIM_COL] << "> dimensions"
	<< std::endl;
		
	std::cout << "_____________________" << std::endl;

	getch();
}

// �����x�N�g���̓���̎��������擾
int FeatureVector::Get_Specified_Dimention(int d) {

	if(d > NUM_DIMENSIONS_OF_VECTOR) {
		return -1;
	}

	return dimensions[d];
}

// �����x�N�g���������������Ă���
double FeatureVector::Get_Specified_Feature(int num_files, int row, int col) {

	return vector.Get_Data(num_files, row, col);
}

void Clear_Buf(char buf[], int *pbuf_pos) {

	// �o�b�t�@�̈ʒu���ŏ��ɖ߂�
	*pbuf_pos = 0;

	// �o�b�t�@����ɂ���B������s��Ȃ��ƑO�̗�̏�񂪎��̃f�[�^�ɉe�����Ă��܂����Ƃ�����
	for(int i=0; i<BUF_SIZE; i++)
		buf[i] = 0;
}
