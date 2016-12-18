#pragma once

#include <windows.h>

#include "Parameters.h"
#include "Node.h"

using namespace std;

// 乱数生成
double myrand();

// ニューラルネットワークの各層に関するクラス
class Layer {

private:

	vector<Node> nodes;
	int number_of_nodes;

public:

	void Setting(int _number_of_nodes);

	// この層が持つノードの数を返す
	int Get_Number_Of_Nodes();

	// ひとつ後ろの層に対する結合荷重の生成
	void Generate_Weights_Of_Nodes_Next(int number_of_nodes_of_next_layer);

	// ひとつ前の層に対する結合荷重の生成
	// 現状だと、入力層と最初の中間層の結合荷重の生成にのみに使用しています.
	void Generate_Weights_Of_Nodes_Previous(int number_of_nodes_of_previous_layer);

	// ひとつ後ろの層への結合荷重を乱数で初期化する
	void Initizlize_Weights_Of_Nodes();

	// 特定の値で結合荷重を初期化したい場合、こちらを使うこと
	void Initizlize_Weights_Of_Nodes(double weights_data[]);

	// ひとつ前の層への結合荷重を乱数で初期化する
	void Initialize_Weights_For_Previous_Layer();

	double* Get_Specified_Cell_p(int which_cell);

	double Get_Specified_Weight(int which_cell, int which_weight);

	// 自分と、一個前のノード
	double Get_Specified_Weight_Pre(int which_cell, int which_weight);

	double* Get_Specified_Bias_p(int which_cell);

	void Update_Specific_Weight(int which_cell, int which_weight, double data);

	void Update_Specific_Weight_Pre(int which_cell, int which_weight, double data);

	void Initialize_Bias_Of_Nodes();
};

void Layer::Setting(int _number_of_nodes) {
	nodes.resize(_number_of_nodes);
	number_of_nodes = _number_of_nodes;
}

// この層が持つノードの数を返す
int Layer::Get_Number_Of_Nodes() {
	return number_of_nodes;
}

// ひとつ後ろの層に対する結合荷重の生成
void Layer::Generate_Weights_Of_Nodes_Next(int number_of_nodes_of_next_layer) {
	for(int i = 0; i < number_of_nodes; i++) {
		nodes[i].Generate_Weights_Next(number_of_nodes_of_next_layer);
	}
}

// ひとつ前の層に対する結合荷重の生成
// 現状だと、入力層と最初の中間層の結合荷重の生成にのみに使用しています.
void Layer::Generate_Weights_Of_Nodes_Previous(int number_of_nodes_of_previous_layer) {
	for(int i = 0; i < number_of_nodes; i++) {
		nodes[i].Generate_Weights_Previous(number_of_nodes_of_previous_layer);
	}
}

// ひとつ後ろの層への結合荷重を乱数で初期化する
void Layer::Initizlize_Weights_Of_Nodes() {
	for(int i = 0; i < number_of_nodes; i++) { 
		nodes[i].Initialize_Weights_Connect_To_Next();
	}
}

// 特定の値で結合荷重を初期化したい場合、こちらを使うこと
void Layer::Initizlize_Weights_Of_Nodes(double weights_data[]) {
	for(int i = 0; i < number_of_nodes; i++) { 
		nodes[i].Initialize_Weights_Connect_To_Next(weights_data);
	}
}

// ひとつ前の層への結合荷重を乱数で初期化する
void Layer::Initialize_Weights_For_Previous_Layer() {
	for(int i = 0; i < number_of_nodes; i++) {
		nodes[i].Initialize_Weights_Connect_To_Previous();
	}
}

double* Layer::Get_Specified_Cell_p(int which_cell) {
	return nodes[which_cell].Get_X_p();
}

double Layer::Get_Specified_Weight(int which_cell, int which_weight) {
	return nodes[which_cell].Get_Specific_Weight(which_weight);
}

// 自分と、一個前のノード
double Layer::Get_Specified_Weight_Pre(int which_cell, int which_weight) {
	return nodes[which_cell].Get_Specific_Weight_Pre(which_weight);
}

double* Layer::Get_Specified_Bias_p(int which_cell) {
	return nodes[which_cell].Get_Bias_p();
}

void Layer::Update_Specific_Weight(int which_cell, int which_weight, double data) {
	return nodes[which_cell].Update_Weight(which_weight, data);
}

void Layer::Update_Specific_Weight_Pre(int which_cell, int which_weight, double data) {
	return nodes[which_cell].Update_Weight_Pre(which_weight, data);
}

void Layer::Initialize_Bias_Of_Nodes() {
	for(int i = 0; i < number_of_nodes; i++) { 
		nodes[i].Initialize_Bias();
	}
}

double myrand() {

	double rndno;

	while( (rndno = (double)rand() / RAND_MAX) == 1.0f);

	rndno = rndno * 2.0 - 1.0f;
	return rndno;
}
