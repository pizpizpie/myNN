#pragma once

#define _CRT_SECURE_NO_WAITINGS

#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <tchar.h>
#include <vector>
#include <time.h>

#include "Parameters.h"
#include "FeatureVector.h"
#include "File_Name_List.h"
#include "Layer.h"

using namespace std;

double Activation_Function(double u);

// ���̃v���O�����̖{��
// �w�K����уe�X�g���s������
class NeuralNetwork {

private:

	vector<Layer> input;			// ���͑w�Avector�^�������ۂ͈�w�̂�
	vector<Layer> output;			// �o�͑w�Avector�^�������ۂ͈�w�̂�
	vector<Layer> hidden;			// ���ԑw
	int number_of_hidden_layers;	// ���ԑw�̑w��

	vector<Layer> test_input;		// �e�X�g�f�[�^

public:

	NeuralNetwork ();

	void Initialize();

	// �w�K��
	void Training(char training_data_path[]);

	// �e�X�g��
	void Test(char test_data_path[]);

	// �w�K�ƃe�X�g�̎��s
	void Do();

	// ���ԑw�̑w����ݒ�
	void Create_Hidden_Layers(int _number_of_hidden_layers);

	// �B��w�Ƀm�[�h����ݒ肷��
	// (�ǂ̉B��w��, �����̃m�[�h��ݒ肷�邩���w��)
	void Setting_Hidden(int which, int number_of_nodes);

	// ���ԑw���m�̌����׏d�̐ݒ�
	// (�Ώۂ̑w��, ���̎��̑w�̃m�[�h��)
	void Generate_Weights_For_Hidden_Layer_Next(int which_layer,  int number_of_nodes_of_next_layer);

	// �S�ẲB��w�̑S�m�[�h�̃o�C�A�X��������
	void Initialize_Biases_of_Hidden_Layers();

	// �S�ẲB��w�̑S�Ă̌����׏d��������
	void Initialize_Weights_For_Hidden_Layer();

	// ���t�f�[�^�ɂ�������͑w����ŏ��̒��ԑw�̊w�K
	void Learn_Input_To_Hidden(vector<Layer> nnetinput);

	// �B��w���m�̊w�K, �B��w��2�w�ȏ�̂Ƃ��̂�
	double Learn_Hidden_To_Hidden();

	// 3�wNN��p
	double Learn_Hidden();

	// �o�͑w����Ō�̉B��w�ւ̃o�b�N�v���p�Q�[�V����
	void BackPropagation_Output_To_Hidden(double o);

	// �B��w���m�̃o�b�N�v���p�Q�[�V����
	// �Ō�̉B��w��1��O����A�ŏ��̉B��w�܂�
	void BackPropagation_Hidden_To_Hidden(double o);

	// �ŏ��̉B��w�Ɠ��͑w�ւ̃o�b�N�v���p�Q�[�V����
	void BackPropagation_Hidden_To_Input_For_Multiple_Layer(double o);

	void BackPropagation_Hidden_To_Input_For_3_Layer_NN(double o);

	// ���t�f�[�^�̓��͑w�̃m�[�h����ݒ肷��
	void Setting_Input(int number_of_nodes);

	// �e�X�g�f�[�^�̓��͑w�̃m�[�h����ݒ肷��
	void Setting_Test_Input(int number_of_nodes);

	// �o�͑w�̃m�[�h����ݒ肷��
	void Setting_Output(int number_of_nodes);

	// ���t�f�[�^�̓��͑w�̃m�[�h����Ԃ�
	int Get_Number_Of_Nodes_In_Input_Layer();

	// �o�͑w�̃m�[�h����Ԃ�
	int Get_Number_Of_Nodes_In_Output_Layer();

	double* Get_Cell_From_Input_p(int which_cell);

	double* Get_Cell_From_Test_Input_p(int which_cell);

	double* Get_Cell_From_Output_p(int which_cell);
};

NeuralNetwork::NeuralNetwork () {

#if	(USE_RANDOM_SEED == TRUE)
	// �����̃V�[�h�̐ݒ�
	srand((unsigned)time(NULL));
#endif

	// ���t�f�[�^�̓��͑w�Əo�͑w�̑w�������ꂼ��1�w��
	input.resize(1);
	output.resize(1);

	// �e�X�g�f�[�^�̓��͑w�����l
	test_input.resize(1);

	// �w�K����уe�X�g�̎��s
	Do();
}

void NeuralNetwork::Initialize() {

	// �o�͑w�̃m�[�h����ݒ�
	Setting_Output(NUMBER_OF_OUT_NODES);

	// ���ԑw�̐���ݒ�
	Create_Hidden_Layers(NUMBER_OF_HIDDEN_LAYERS);

	vector<int> number_of_nodes_in_hidden_layers;
	number_of_nodes_in_hidden_layers.resize(number_of_hidden_layers);

	for(int i = 0; i < number_of_hidden_layers; i++) {

		number_of_nodes_in_hidden_layers[i] = NUMBER_OF_HIDDEN_NODES;
	}

	// ���ԑw�̃m�[�h���̐ݒ�
	for(int i = 0; i < number_of_hidden_layers; i++) {

		Setting_Hidden(i, number_of_nodes_in_hidden_layers[i]);
	}

	// ���ԑw�̍ŏ��̑w�ƁA���͑w�̊Ԃ̌����׏d�����
	hidden[0].Generate_Weights_Of_Nodes_Previous(Get_Number_Of_Nodes_In_Input_Layer());

	// ���ԑw�̌����׏d
	for(int i = 0; i < number_of_hidden_layers; i++) {

		// �Ō�̒��ԑw�́A�o�͑w�Ƃ̌����׏d�����
		if(i == number_of_hidden_layers - 1) {
			Generate_Weights_For_Hidden_Layer_Next(i, Get_Number_Of_Nodes_In_Output_Layer());
		}

		else {
			Generate_Weights_For_Hidden_Layer_Next(i, hidden[i+1].Get_Number_Of_Nodes());
		}
	}

	// ���ԑw�̃o�C�A�X��������
	Initialize_Biases_of_Hidden_Layers();

	// ���͑w�̌����׏d�𗐐��ŏ���������
	hidden[0].Initialize_Weights_For_Previous_Layer();

	// ���ԑw�̌����׏d�𗐐��ŏ���������
	Initialize_Weights_For_Hidden_Layer();
}

// �w�K��
void NeuralNetwork::Training(char training_data_path[]) {

	// ���t�f�[�^�̃��X�g���擾����
	File_Name_List training_file_list = File_Name_List(training_data_path);

	FeatureVector training_fvec = FeatureVector(NUMBER_OF_TRAINING_DATA);

	if(training_fvec.Load(training_data_path, training_file_list) == E_FAIL) {
		getch();
		exit(1);
	}

#if	(FSHOW_LOG == TRUE)
	training_file_list.Show();
	training_fvec.Show_Dimensionality();
#endif

	// ���͑w�̃m�[�h���̐ݒ�(�N���X�͏���)
	Setting_Input(training_fvec.Get_Specified_Dimention(DIM_COL) - 1);
	Initialize();

	double err = INITIAL_ERR;

	double o = 0.0;

	unsigned long cnt = 0;

	while ((err > LIMIT) && (cnt < EPOCH)) {
		err = 0.0f;

		for(int i = 0; i < NUMBER_OF_TRAINING_DATA; i++) {

			// ���͑w�ɓ����x�N�g���̒l������
			for(int j = 0; j < input[0].Get_Number_Of_Nodes(); j++) {
				double *p = Get_Cell_From_Input_p(j);
				*p = training_fvec.Get_Specified_Feature(0, i, j);
			}

			// �o�͑w�ɃN���X�̒l������
			for(int j = 0; j < output[0].Get_Number_Of_Nodes(); j++) {
				double *p = Get_Cell_From_Output_p(j);
				*p = training_fvec.Get_Specified_Feature(0, i, training_fvec.Get_Specified_Dimention(DIM_COL) - 1);
			}

			//////////////////////////
			// �t�H���[�h�w�K
			//////////////////////////

			Learn_Input_To_Hidden(this->input);

			// ���wNN�̏ꍇ
			if(number_of_hidden_layers > 1) {
				o = Learn_Hidden_To_Hidden();
			}

			// 3�wNN�̏ꍇ
			else {
				o = Learn_Hidden();
			}

			//////////////////////////
			// �o�b�N�v���p�Q�[�V����
			//////////////////////////

			BackPropagation_Output_To_Hidden(o);

			// ���wNN�̏ꍇ
			if(number_of_hidden_layers > 1) {
				BackPropagation_Hidden_To_Hidden(o);
				BackPropagation_Hidden_To_Input_For_Multiple_Layer(o);
			}

			// 3�wNN�̏ꍇ
			else {
				BackPropagation_Hidden_To_Input_For_3_Layer_NN(o);
			}

			// �G���[���̌v�Z
			err += pow((o - (*output[0].Get_Specified_Cell_p(0))), 2.0);
		}

		cnt++;
		std::cout << cnt << "\t" << err << std::endl;
	}
}

// �e�X�g��
void NeuralNetwork::Test(char test_data_path[]) {

	// �e�X�g�f�[�^�̃��X�g���擾����
	File_Name_List test_file_list = File_Name_List(test_data_path);

	FeatureVector test_fvec = FeatureVector(NUMBER_OF_TEST_DATA);

	if(test_fvec.Load(test_data_path, test_file_list) == E_FAIL) {

		getch();
		exit(1);
	}

#ifdef	FSHOW_LOG
	test_file_list.Show();
	test_fvec.Show_Dimensionality();
#endif

	// ���͑w�̃m�[�h���̐ݒ�
	Setting_Test_Input(test_fvec.Get_Specified_Dimention(DIM_COL));

	// ����
	unsigned int class1_correct = 0, class2_correct = 0;

	for(int i = 0; i < NUMBER_OF_TEST_DATA; i++) {

		// ���͑w�ɓ����x�N�g���̒l������
		for(int j = 0; j < test_input[0].Get_Number_Of_Nodes(); j++) {

			double *p = Get_Cell_From_Test_Input_p(j);
			*p = test_fvec.Get_Specified_Feature(0, i, j);
		}

		Learn_Input_To_Hidden(this->test_input);
		double result = 0.0f;

		// �l�b�g���[�N�����w�̏ꍇ
		if(number_of_hidden_layers > 1) {

			result = Learn_Hidden_To_Hidden();
		}

		// �l�b�g���[�N��3�w�̏ꍇ
		else {

			result = Learn_Hidden();
		}

		std::cout << result << " :\t" << ((result >= THRESHOLD) ? "1" : "0") << std::endl;

		if(i < LEN_CLASS1) {
			if(result >= THRESHOLD) {
				class1_correct++;
			}
		}

		else {
			if(result < THRESHOLD) {
				class2_correct++;
			}
		}
	}

	double class1_accuracy = ((double)class1_correct / LEN_CLASS1);
	double class2_accuracy = ((double)class2_correct / LEN_CLASS2);

	std::cout << std::endl;
	std::cout << "Class1 Accuracy = " << class1_accuracy << std::endl; 
	std::cout << "Class2 Accuracy = " << class2_accuracy << std::endl;
	std::cout << std::endl;
}

// �w�K����уe�X�g�̎��s
void NeuralNetwork::Do() {

	Training(PATH_TRAINING_DATA);
	Test(PATH_TEST_DATA);
	getch();
}

// ���ԑw�̑w����ݒ�
void NeuralNetwork::Create_Hidden_Layers(int _number_of_hidden_layers) {

	hidden.resize(_number_of_hidden_layers);
	number_of_hidden_layers = _number_of_hidden_layers;
}

// �B��w�Ƀm�[�h����ݒ肷��
// (�ǂ̉B��w��, �����̃m�[�h��ݒ肷�邩���w��)
void NeuralNetwork::Setting_Hidden(int which, int number_of_nodes) {

	hidden[which].Setting(number_of_nodes);
}

// ���ԑw���m�̌����׏d�̐ݒ�
// (�Ώۂ̑w��, ���̎��̑w�̃m�[�h��)
void NeuralNetwork::Generate_Weights_For_Hidden_Layer_Next(
	int which_layer,  
	int number_of_nodes_of_next_layer) {

		hidden[which_layer].Generate_Weights_Of_Nodes_Next(number_of_nodes_of_next_layer);
}

// �S�ẲB��w�̑S�m�[�h�̃o�C�A�X��������
void NeuralNetwork::Initialize_Biases_of_Hidden_Layers() {

	for(int i = 0; i < number_of_hidden_layers; i++) {

		hidden[i].Initialize_Bias_Of_Nodes();
	}
}

// �S�ẲB��w�̑S�Ă̌����׏d��������
void NeuralNetwork::Initialize_Weights_For_Hidden_Layer() {

	for(int i = 0; i < number_of_hidden_layers; i++) {

		hidden[i].Initizlize_Weights_Of_Nodes();
	}
}

// ���t�f�[�^�ɂ�������͑w����ŏ��̒��ԑw�̊w�K
void NeuralNetwork::Learn_Input_To_Hidden(vector<Layer> nnetinput) {

	// �ŏ��̉B��w�̃m�[�h�����擾����
	int number_of_hidden_nodes = hidden[0].Get_Number_Of_Nodes();

	for(int i = 0; i < number_of_hidden_nodes; i++) {

		double value = 0.0f;

		int number_of_input_nodes = nnetinput[0].Get_Number_Of_Nodes();

		// ���͑w�̃m�[�h(j)�̎��l * �����׏d(�B��w�̃m�[�hi�Ɠ��͑w�̃m�[�hj) �̑��a�����
		for(int j = 0; j < number_of_input_nodes; j++) {

			value += (*nnetinput[0].Get_Specified_Cell_p(j)) 
				* hidden[0].Get_Specified_Weight_Pre(i, j);
		}

		// �B��w�̃m�[�h(i)�̃o�C�A�X���������l���������֐��ɓ���A�B��w�̃m�[�h(i)�̎��l�Ƃ���
		value -= (*hidden[0].Get_Specified_Bias_p(i));
		double *tmp = hidden[0].Get_Specified_Cell_p(i);
		*tmp = Activation_Function(value);
	}
}

// �B��w���m�̊w�K, �B��w��2�w�ȏ�̂Ƃ��̂�
double NeuralNetwork::Learn_Hidden_To_Hidden() {

	if(number_of_hidden_layers < 1) {
		std::cout << "This network doesn't have more than 2 hidden layers." << std::endl;
		getch();
		return 0.0f;
	}

	// �B��w2�w�ڂ���A�Ō�̉B��w�܂�
	for(int i = 1; i < number_of_hidden_layers; i++) {

		// ����������B��w�̃m�[�h��
		for(int j = 0; j < hidden[i].Get_Number_Of_Nodes(); j++) {

			double value = 0.0f;

			// ����������B��w�̈�w�O�̉B��w�̃m�[�h��
			for(int k = 0; k < hidden[i-1].Get_Number_Of_Nodes(); k++) {

				// �B��w i-1 �w�ڂ̃m�[�h(k) * �����׏d(�B��w i-1 �̃m�[�h k �� �B��w i �̃m�[�h j) �̑��a�����
				value += (*hidden[i-1].Get_Specified_Cell_p(k)) 
					* hidden[i-1].Get_Specified_Weight(k, j);
			}

			// �B��w i �̃m�[�h j �̃o�C�A�X���������l���������֐��ɓ���A�B��w i �̃m�[�h j �̎��l�Ƃ���
			value -= *(hidden[i].Get_Specified_Bias_p(j));
			double *tmp = hidden[i].Get_Specified_Cell_p(j);
			*tmp = Activation_Function(value);
		}
	}

	double output_from_last_hidden_layer = 0.0f;

	// �Ō�̉B��w�̏o�͒l
	for(int i = 0; i < hidden[number_of_hidden_layers - 1].Get_Number_Of_Nodes(); i++) {

		// �Ō�̉B��w�̃m�[�h i * �����׏d(�Ō�̉B��w�̃m�[�h i �� �o�͑w�m�[�h (0)) �̑��a�����
		// �o�͑w�̃m�[�h�������̏ꍇ���l�����Ȃ��Ă͂����Ȃ����߁A����C�����邱��
		double *tmp = hidden[number_of_hidden_layers - 1].Get_Specified_Cell_p(i);
		output_from_last_hidden_layer += ((*tmp) * hidden[number_of_hidden_layers - 1].Get_Specified_Weight(i, 0));
	}

	// �o�͑w�̃m�[�h�������̏ꍇ���l�����Ȃ��Ă͂����Ȃ����߁A����C�����邱��
	output_from_last_hidden_layer -= *(output[0].Get_Specified_Bias_p(0));

	return Activation_Function(output_from_last_hidden_layer);
}

// 3�wNN��p
double NeuralNetwork::Learn_Hidden() {

	if(number_of_hidden_layers != 1)
		return 0.0f;

	double value = 0.0f;

	// �o�͑w�̃m�[�h�������̏ꍇ���l�����Ȃ��Ă͂����Ȃ����߁A����C�����邱��
	// �o�͑w�̃m�[�h��
	for(int i = 0; i < output[0].Get_Number_Of_Nodes(); i++) {

		// �o�͑w�̒��O�̉B��w�̃m�[�h��
		for(int j = 0; j < hidden[0].Get_Number_Of_Nodes(); j++) {
			value += (*hidden[0].Get_Specified_Cell_p(j)) 
				* hidden[0].Get_Specified_Weight(j, i);
		}

		value -= *(output[0].Get_Specified_Bias_p(i));
	}

	return Activation_Function(value);
}

// �o�͑w����Ō�̉B��w�ւ̃o�b�N�v���p�Q�[�V����
void NeuralNetwork::BackPropagation_Output_To_Hidden(double o) {

	double delta = *(output[0].Get_Specified_Cell_p(0)) - o;
	double d = delta * o * (1.0f - o);

	for(int i = 0; i < hidden[number_of_hidden_layers - 1].Get_Number_Of_Nodes(); i++) {
		double tmp = (*(hidden[number_of_hidden_layers - 1].Get_Specified_Cell_p(i)));

		double dif_weight = ALPHA * d * tmp;

		hidden[number_of_hidden_layers - 1].Update_Specific_Weight(i, 0, dif_weight);
	}

	// �o�͑w�̃o�C�A�X�̏C��
	*(output[0].Get_Specified_Bias_p(0)) += ALPHA * (-1.0) * d;
}

// �B��w���m�̃o�b�N�v���p�Q�[�V����
// �Ō�̉B��w��1��O����A�ŏ��̉B��w�܂�
void NeuralNetwork::BackPropagation_Hidden_To_Hidden(double o) {

	double delta = *(output[0].Get_Specified_Cell_p(0)) - o;
	double d = delta * o * (1.0f - o);

	for(int i = (number_of_hidden_layers - 2); i > -1; i--) {
		for(int j = 0; j < hidden[i].Get_Number_Of_Nodes(); j++) {

			double dif_weight = 1e+0;

			double tmp = (*(hidden[i].Get_Specified_Cell_p(j)));

			for(int k = i; k <= (number_of_hidden_layers - 2); k++) {
				for(int l = 0; l < hidden[k].Get_Number_Of_Nodes(); l++) {
					for(int m = 0; m < hidden[k + 1].Get_Number_Of_Nodes(); m++) {
						// (1) ������ dif_weight *= (d * ...) ���� "d * "�𔲂���
						dif_weight *= (d * (*hidden[k].Get_Specified_Cell_p(l)) 
							* (1.0 - (*hidden[k].Get_Specified_Cell_p(l))) 
							* hidden[k].Get_Specified_Weight(l, m));
					}
				}
			}

			// �����׏d�ƃo�C�A�X���X�V����
			// �ŏ��̉B��w�ł͎��s���Ȃ�
			// ���͑w�ƍŏ��̑w�̌����׏d��BackPropagation_Hidden_To_Input()�Ŏ�舵��
			if(i > 0) {
				double dif_weight_tmp = ALPHA * dif_weight * tmp;

				for(int k = 0; k < hidden[i + 1].Get_Number_Of_Nodes(); k++) {
					hidden[i].Update_Specific_Weight(j, k, dif_weight_tmp);
				}

				*(hidden[i + 1].Get_Specified_Bias_p(j)) += ALPHA * (-1.0) * dif_weight_tmp;
			}
		}
	}
}

// �ŏ��̉B��w�Ɠ��͑w�ւ̃o�b�N�v���p�Q�[�V����
void NeuralNetwork::BackPropagation_Hidden_To_Input_For_Multiple_Layer(double o) {

	double delta = *(output[0].Get_Specified_Cell_p(0)) - o;
	double d = delta * o * (1.0f - o);
	double tmp = 0.0;

	double dif_weight;

	for(int i = (number_of_hidden_layers - 2); i > -1; i--) {
		for(int j = 0; j < hidden[i].Get_Number_Of_Nodes(); j++) {

			dif_weight = 1e+0;

			tmp = (*(hidden[i].Get_Specified_Cell_p(j)));

			for(int k = i; k <= (number_of_hidden_layers - 2); k++) {
				for(int l = 0; l < hidden[k].Get_Number_Of_Nodes(); l++) {
					for(int m = 0; m < hidden[k + 1].Get_Number_Of_Nodes(); m++) {
						// (1) ������ dif_weight *= (d * ...) ���� "d * "�𔲂���
						dif_weight *= (d * (*hidden[k].Get_Specified_Cell_p(l)) 
							* (1.0 - (*hidden[k].Get_Specified_Cell_p(l))) 
							* hidden[k].Get_Specified_Weight(l, m));
					}
				}
			}
		}

		if(!i) {
			for(int k = 0; k < hidden[0].Get_Number_Of_Nodes(); k++) {
				tmp = (*(hidden[0].Get_Specified_Cell_p(k)));
				double dif_weight_tmp = ALPHA * dif_weight * tmp;

				for(int l = 0; l < input[0].Get_Number_Of_Nodes(); l++) {
					hidden[0].Update_Specific_Weight_Pre(k, l, dif_weight_tmp);
				}

				*(hidden[0].Get_Specified_Bias_p(k)) += ALPHA * (-1.0) * dif_weight_tmp;
			}
		}
	}
}

void NeuralNetwork::BackPropagation_Hidden_To_Input_For_3_Layer_NN(double o) {

	double delta = *(output[0].Get_Specified_Cell_p(0)) - o;
	double d = delta * o * (1.0f - o);

	double dif_weight = 1e+0;

	for(int i = 0; i < hidden[0].Get_Number_Of_Nodes(); i++) {

		dif_weight *= (d * (*hidden[0].Get_Specified_Cell_p(i)) 
			* (1.0 - (*hidden[0].Get_Specified_Cell_p(i))) 
			* hidden[0].Get_Specified_Weight(i, 0));

		for(int j = 0; j < input[0].Get_Number_Of_Nodes(); j++) {
			hidden[0].Update_Specific_Weight_Pre(i, j, dif_weight);
		}

		*(hidden[0].Get_Specified_Bias_p(i)) += ALPHA * (-1.0) * dif_weight;
	}
}

// ���t�f�[�^�̓��͑w�̃m�[�h����ݒ肷��
void NeuralNetwork::Setting_Input(int number_of_nodes) {

	input[0].Setting(number_of_nodes);
}

// �e�X�g�f�[�^�̓��͑w�̃m�[�h����ݒ肷��
void NeuralNetwork::Setting_Test_Input(int number_of_nodes) {

	test_input[0].Setting(number_of_nodes);
}

// �o�͑w�̃m�[�h����ݒ肷��
void NeuralNetwork::Setting_Output(int number_of_nodes) {

	output[0].Setting(number_of_nodes);
}

// ���t�f�[�^�̓��͑w�̃m�[�h����Ԃ�
int NeuralNetwork::Get_Number_Of_Nodes_In_Input_Layer() {

	return input[0].Get_Number_Of_Nodes();
}

// �o�͑w�̃m�[�h����Ԃ�
int NeuralNetwork::Get_Number_Of_Nodes_In_Output_Layer() {

	return output[0].Get_Number_Of_Nodes();
}

double* NeuralNetwork::Get_Cell_From_Input_p(int which_cell) {

	return input[0].Get_Specified_Cell_p(which_cell);
}

double* NeuralNetwork::Get_Cell_From_Test_Input_p(int which_cell) {

	return test_input[0].Get_Specified_Cell_p(which_cell);
}

double* NeuralNetwork::Get_Cell_From_Output_p(int which_cell) {

	return output[0].Get_Specified_Cell_p(which_cell);
}

double Activation_Function(double u) {
	return 1.0 / (1.0 + exp(-u));
}
