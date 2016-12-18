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

// このプログラムの本体
// 学習およびテストを行う部分
class NeuralNetwork {

private:

	vector<Layer> input;			// 入力層、vector型だが実際は一層のみ
	vector<Layer> output;			// 出力層、vector型だが実際は一層のみ
	vector<Layer> hidden;			// 中間層
	int number_of_hidden_layers;	// 中間層の層数

	vector<Layer> test_input;		// テストデータ

public:

	NeuralNetwork ();

	void Initialize();

	// 学習部
	void Training(char training_data_path[]);

	// テスト部
	void Test(char test_data_path[]);

	// 学習とテストの実行
	void Do();

	// 中間層の層数を設定
	void Create_Hidden_Layers(int _number_of_hidden_layers);

	// 隠れ層にノード数を設定する
	// (どの隠れ層に, いくつのノードを設定するかを指定)
	void Setting_Hidden(int which, int number_of_nodes);

	// 中間層同士の結合荷重の設定
	// (対象の層と, その次の層のノード数)
	void Generate_Weights_For_Hidden_Layer_Next(int which_layer,  int number_of_nodes_of_next_layer);

	// 全ての隠れ層の全ノードのバイアスを初期化
	void Initialize_Biases_of_Hidden_Layers();

	// 全ての隠れ層の全ての結合荷重を初期化
	void Initialize_Weights_For_Hidden_Layer();

	// 教師データにおける入力層から最初の中間層の学習
	void Learn_Input_To_Hidden(vector<Layer> nnetinput);

	// 隠れ層同士の学習, 隠れ層が2層以上のときのみ
	double Learn_Hidden_To_Hidden();

	// 3層NN専用
	double Learn_Hidden();

	// 出力層から最後の隠れ層へのバックプロパゲーション
	void BackPropagation_Output_To_Hidden(double o);

	// 隠れ層同士のバックプロパゲーション
	// 最後の隠れ層の1つ手前から、最初の隠れ層まで
	void BackPropagation_Hidden_To_Hidden(double o);

	// 最初の隠れ層と入力層へのバックプロパゲーション
	void BackPropagation_Hidden_To_Input_For_Multiple_Layer(double o);

	void BackPropagation_Hidden_To_Input_For_3_Layer_NN(double o);

	// 教師データの入力層のノード数を設定する
	void Setting_Input(int number_of_nodes);

	// テストデータの入力層のノード数を設定する
	void Setting_Test_Input(int number_of_nodes);

	// 出力層のノード数を設定する
	void Setting_Output(int number_of_nodes);

	// 教師データの入力層のノード数を返す
	int Get_Number_Of_Nodes_In_Input_Layer();

	// 出力層のノード数を返す
	int Get_Number_Of_Nodes_In_Output_Layer();

	double* Get_Cell_From_Input_p(int which_cell);

	double* Get_Cell_From_Test_Input_p(int which_cell);

	double* Get_Cell_From_Output_p(int which_cell);
};

NeuralNetwork::NeuralNetwork () {

#if	(USE_RANDOM_SEED == TRUE)
	// 乱数のシードの設定
	srand((unsigned)time(NULL));
#endif

	// 教師データの入力層と出力層の層数をそれぞれ1層に
	input.resize(1);
	output.resize(1);

	// テストデータの入力層も同様
	test_input.resize(1);

	// 学習およびテストの実行
	Do();
}

void NeuralNetwork::Initialize() {

	// 出力層のノード数を設定
	Setting_Output(NUMBER_OF_OUT_NODES);

	// 中間層の数を設定
	Create_Hidden_Layers(NUMBER_OF_HIDDEN_LAYERS);

	vector<int> number_of_nodes_in_hidden_layers;
	number_of_nodes_in_hidden_layers.resize(number_of_hidden_layers);

	for(int i = 0; i < number_of_hidden_layers; i++) {

		number_of_nodes_in_hidden_layers[i] = NUMBER_OF_HIDDEN_NODES;
	}

	// 中間層のノード数の設定
	for(int i = 0; i < number_of_hidden_layers; i++) {

		Setting_Hidden(i, number_of_nodes_in_hidden_layers[i]);
	}

	// 中間層の最初の層と、入力層の間の結合荷重を作る
	hidden[0].Generate_Weights_Of_Nodes_Previous(Get_Number_Of_Nodes_In_Input_Layer());

	// 中間層の結合荷重
	for(int i = 0; i < number_of_hidden_layers; i++) {

		// 最後の中間層の、出力層との結合荷重を作る
		if(i == number_of_hidden_layers - 1) {
			Generate_Weights_For_Hidden_Layer_Next(i, Get_Number_Of_Nodes_In_Output_Layer());
		}

		else {
			Generate_Weights_For_Hidden_Layer_Next(i, hidden[i+1].Get_Number_Of_Nodes());
		}
	}

	// 中間層のバイアスを初期化
	Initialize_Biases_of_Hidden_Layers();

	// 入力層の結合荷重を乱数で初期化する
	hidden[0].Initialize_Weights_For_Previous_Layer();

	// 中間層の結合荷重を乱数で初期化する
	Initialize_Weights_For_Hidden_Layer();
}

// 学習部
void NeuralNetwork::Training(char training_data_path[]) {

	// 教師データのリストを取得する
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

	// 入力層のノード数の設定(クラスは除く)
	Setting_Input(training_fvec.Get_Specified_Dimention(DIM_COL) - 1);
	Initialize();

	double err = INITIAL_ERR;

	double o = 0.0;

	unsigned long cnt = 0;

	while ((err > LIMIT) && (cnt < EPOCH)) {
		err = 0.0f;

		for(int i = 0; i < NUMBER_OF_TRAINING_DATA; i++) {

			// 入力層に特徴ベクトルの値を入れる
			for(int j = 0; j < input[0].Get_Number_Of_Nodes(); j++) {
				double *p = Get_Cell_From_Input_p(j);
				*p = training_fvec.Get_Specified_Feature(0, i, j);
			}

			// 出力層にクラスの値を入れる
			for(int j = 0; j < output[0].Get_Number_Of_Nodes(); j++) {
				double *p = Get_Cell_From_Output_p(j);
				*p = training_fvec.Get_Specified_Feature(0, i, training_fvec.Get_Specified_Dimention(DIM_COL) - 1);
			}

			//////////////////////////
			// フォワード学習
			//////////////////////////

			Learn_Input_To_Hidden(this->input);

			// 多層NNの場合
			if(number_of_hidden_layers > 1) {
				o = Learn_Hidden_To_Hidden();
			}

			// 3層NNの場合
			else {
				o = Learn_Hidden();
			}

			//////////////////////////
			// バックプロパゲーション
			//////////////////////////

			BackPropagation_Output_To_Hidden(o);

			// 多層NNの場合
			if(number_of_hidden_layers > 1) {
				BackPropagation_Hidden_To_Hidden(o);
				BackPropagation_Hidden_To_Input_For_Multiple_Layer(o);
			}

			// 3層NNの場合
			else {
				BackPropagation_Hidden_To_Input_For_3_Layer_NN(o);
			}

			// エラー率の計算
			err += pow((o - (*output[0].Get_Specified_Cell_p(0))), 2.0);
		}

		cnt++;
		std::cout << cnt << "\t" << err << std::endl;
	}
}

// テスト部
void NeuralNetwork::Test(char test_data_path[]) {

	// テストデータのリストを取得する
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

	// 入力層のノード数の設定
	Setting_Test_Input(test_fvec.Get_Specified_Dimention(DIM_COL));

	// 正解数
	unsigned int class1_correct = 0, class2_correct = 0;

	for(int i = 0; i < NUMBER_OF_TEST_DATA; i++) {

		// 入力層に特徴ベクトルの値を入れる
		for(int j = 0; j < test_input[0].Get_Number_Of_Nodes(); j++) {

			double *p = Get_Cell_From_Test_Input_p(j);
			*p = test_fvec.Get_Specified_Feature(0, i, j);
		}

		Learn_Input_To_Hidden(this->test_input);
		double result = 0.0f;

		// ネットワークが多層の場合
		if(number_of_hidden_layers > 1) {

			result = Learn_Hidden_To_Hidden();
		}

		// ネットワークが3層の場合
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

// 学習およびテストの実行
void NeuralNetwork::Do() {

	Training(PATH_TRAINING_DATA);
	Test(PATH_TEST_DATA);
	getch();
}

// 中間層の層数を設定
void NeuralNetwork::Create_Hidden_Layers(int _number_of_hidden_layers) {

	hidden.resize(_number_of_hidden_layers);
	number_of_hidden_layers = _number_of_hidden_layers;
}

// 隠れ層にノード数を設定する
// (どの隠れ層に, いくつのノードを設定するかを指定)
void NeuralNetwork::Setting_Hidden(int which, int number_of_nodes) {

	hidden[which].Setting(number_of_nodes);
}

// 中間層同士の結合荷重の設定
// (対象の層と, その次の層のノード数)
void NeuralNetwork::Generate_Weights_For_Hidden_Layer_Next(
	int which_layer,  
	int number_of_nodes_of_next_layer) {

		hidden[which_layer].Generate_Weights_Of_Nodes_Next(number_of_nodes_of_next_layer);
}

// 全ての隠れ層の全ノードのバイアスを初期化
void NeuralNetwork::Initialize_Biases_of_Hidden_Layers() {

	for(int i = 0; i < number_of_hidden_layers; i++) {

		hidden[i].Initialize_Bias_Of_Nodes();
	}
}

// 全ての隠れ層の全ての結合荷重を初期化
void NeuralNetwork::Initialize_Weights_For_Hidden_Layer() {

	for(int i = 0; i < number_of_hidden_layers; i++) {

		hidden[i].Initizlize_Weights_Of_Nodes();
	}
}

// 教師データにおける入力層から最初の中間層の学習
void NeuralNetwork::Learn_Input_To_Hidden(vector<Layer> nnetinput) {

	// 最初の隠れ層のノード数を取得する
	int number_of_hidden_nodes = hidden[0].Get_Number_Of_Nodes();

	for(int i = 0; i < number_of_hidden_nodes; i++) {

		double value = 0.0f;

		int number_of_input_nodes = nnetinput[0].Get_Number_Of_Nodes();

		// 入力層のノード(j)の持つ値 * 結合荷重(隠れ層のノードiと入力層のノードj) の総和を取る
		for(int j = 0; j < number_of_input_nodes; j++) {

			value += (*nnetinput[0].Get_Specified_Cell_p(j)) 
				* hidden[0].Get_Specified_Weight_Pre(i, j);
		}

		// 隠れ層のノード(i)のバイアスを引いた値を活性化関数に入れ、隠れ層のノード(i)の持つ値とする
		value -= (*hidden[0].Get_Specified_Bias_p(i));
		double *tmp = hidden[0].Get_Specified_Cell_p(i);
		*tmp = Activation_Function(value);
	}
}

// 隠れ層同士の学習, 隠れ層が2層以上のときのみ
double NeuralNetwork::Learn_Hidden_To_Hidden() {

	if(number_of_hidden_layers < 1) {
		std::cout << "This network doesn't have more than 2 hidden layers." << std::endl;
		getch();
		return 0.0f;
	}

	// 隠れ層2層目から、最後の隠れ層まで
	for(int i = 1; i < number_of_hidden_layers; i++) {

		// 処理をする隠れ層のノード数
		for(int j = 0; j < hidden[i].Get_Number_Of_Nodes(); j++) {

			double value = 0.0f;

			// 処理をする隠れ層の一層前の隠れ層のノード数
			for(int k = 0; k < hidden[i-1].Get_Number_Of_Nodes(); k++) {

				// 隠れ層 i-1 層目のノード(k) * 結合荷重(隠れ層 i-1 のノード k と 隠れ層 i のノード j) の総和を取る
				value += (*hidden[i-1].Get_Specified_Cell_p(k)) 
					* hidden[i-1].Get_Specified_Weight(k, j);
			}

			// 隠れ層 i のノード j のバイアスを引いた値を活性化関数に入れ、隠れ層 i のノード j の持つ値とする
			value -= *(hidden[i].Get_Specified_Bias_p(j));
			double *tmp = hidden[i].Get_Specified_Cell_p(j);
			*tmp = Activation_Function(value);
		}
	}

	double output_from_last_hidden_layer = 0.0f;

	// 最後の隠れ層の出力値
	for(int i = 0; i < hidden[number_of_hidden_layers - 1].Get_Number_Of_Nodes(); i++) {

		// 最後の隠れ層のノード i * 結合荷重(最後の隠れ層のノード i と 出力層ノード (0)) の総和を取る
		// 出力層のノードが複数の場合も考慮しなくてはいけないため、後程修正すること
		double *tmp = hidden[number_of_hidden_layers - 1].Get_Specified_Cell_p(i);
		output_from_last_hidden_layer += ((*tmp) * hidden[number_of_hidden_layers - 1].Get_Specified_Weight(i, 0));
	}

	// 出力層のノードが複数の場合も考慮しなくてはいけないため、後程修正すること
	output_from_last_hidden_layer -= *(output[0].Get_Specified_Bias_p(0));

	return Activation_Function(output_from_last_hidden_layer);
}

// 3層NN専用
double NeuralNetwork::Learn_Hidden() {

	if(number_of_hidden_layers != 1)
		return 0.0f;

	double value = 0.0f;

	// 出力層のノードが複数の場合も考慮しなくてはいけないため、後程修正すること
	// 出力層のノード数
	for(int i = 0; i < output[0].Get_Number_Of_Nodes(); i++) {

		// 出力層の直前の隠れ層のノード数
		for(int j = 0; j < hidden[0].Get_Number_Of_Nodes(); j++) {
			value += (*hidden[0].Get_Specified_Cell_p(j)) 
				* hidden[0].Get_Specified_Weight(j, i);
		}

		value -= *(output[0].Get_Specified_Bias_p(i));
	}

	return Activation_Function(value);
}

// 出力層から最後の隠れ層へのバックプロパゲーション
void NeuralNetwork::BackPropagation_Output_To_Hidden(double o) {

	double delta = *(output[0].Get_Specified_Cell_p(0)) - o;
	double d = delta * o * (1.0f - o);

	for(int i = 0; i < hidden[number_of_hidden_layers - 1].Get_Number_Of_Nodes(); i++) {
		double tmp = (*(hidden[number_of_hidden_layers - 1].Get_Specified_Cell_p(i)));

		double dif_weight = ALPHA * d * tmp;

		hidden[number_of_hidden_layers - 1].Update_Specific_Weight(i, 0, dif_weight);
	}

	// 出力層のバイアスの修正
	*(output[0].Get_Specified_Bias_p(0)) += ALPHA * (-1.0) * d;
}

// 隠れ層同士のバックプロパゲーション
// 最後の隠れ層の1つ手前から、最初の隠れ層まで
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
						// (1) ここの dif_weight *= (d * ...) から "d * "を抜いた
						dif_weight *= (d * (*hidden[k].Get_Specified_Cell_p(l)) 
							* (1.0 - (*hidden[k].Get_Specified_Cell_p(l))) 
							* hidden[k].Get_Specified_Weight(l, m));
					}
				}
			}

			// 結合荷重とバイアスを更新する
			// 最初の隠れ層では実行しない
			// 入力層と最初の層の結合荷重はBackPropagation_Hidden_To_Input()で取り扱う
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

// 最初の隠れ層と入力層へのバックプロパゲーション
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
						// (1) ここの dif_weight *= (d * ...) から "d * "を抜いた
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

// 教師データの入力層のノード数を設定する
void NeuralNetwork::Setting_Input(int number_of_nodes) {

	input[0].Setting(number_of_nodes);
}

// テストデータの入力層のノード数を設定する
void NeuralNetwork::Setting_Test_Input(int number_of_nodes) {

	test_input[0].Setting(number_of_nodes);
}

// 出力層のノード数を設定する
void NeuralNetwork::Setting_Output(int number_of_nodes) {

	output[0].Setting(number_of_nodes);
}

// 教師データの入力層のノード数を返す
int NeuralNetwork::Get_Number_Of_Nodes_In_Input_Layer() {

	return input[0].Get_Number_Of_Nodes();
}

// 出力層のノード数を返す
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
