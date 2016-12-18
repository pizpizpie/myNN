#pragma once

#include "Parameters.h"

// 乱数生成
double myrand();

// 各層が持つノードに関するクラス
class Node {

private:
	// ノードの持つ値
	double x;

	// １つ前の層のとノードとの結合荷重
	vector<double> w_connect_to_pre;

	// １つ後ろのレイヤーとのノードとの結合荷重
	vector<double> w_connect_to_next;

	// バイアス
	double bias;

public:

	Node();

	// 次の層への結合荷重を生成する
	void Generate_Weights_Next(int number_of_nodes_of_next_layer);

	// 前の層への結合荷重を生成する
	// 今のところ最初の中間層から入力層へしか使っていない
	void Generate_Weights_Previous(int number_of_previous_layer);

	// 結合荷重の更新
	void Update_Weight(int which_weight, double data);

	void Update_Weight_Pre(int which_weight, double data);

	// バイアス値の初期化
	void Initialize_Bias();

	// 次の層への結合荷重を乱数で初期化
	void Initialize_Weights_Connect_To_Next();

	// 次の層への結合荷重を特定の値で初期化したい場合、こちらを使うこと
	void Initialize_Weights_Connect_To_Next(double weights_data[]);

	// 前の層への結合荷重を乱数で初期化
	// 今のところ最初の中間層から入力層へしか使っていない
	void Initialize_Weights_Connect_To_Previous();

	// 前の層への結合荷重を特定の値で初期化したい場合、こちらを使うこと
	void Initialize_Weights_Connect_To_Previous(double weights_data[]);

	double* Get_X_p();

	double Get_Specific_Weight(int which_weight);

	double Get_Specific_Weight_Pre(int which_weight);

	double* Get_Bias_p();

	vector<double> Get_W_Connect_To_Next();
};

Node::Node() {
	x = 0.0f;
	bias = 0.0f;
}

// 次の層への結合荷重を生成する
void Node::Generate_Weights_Next(int number_of_nodes_of_next_layer) {
	if(number_of_nodes_of_next_layer) {
		w_connect_to_next.resize(number_of_nodes_of_next_layer);
	};
}

// 前の層への結合荷重を生成する
// 今のところ最初の中間層から入力層へしか使っていない
void Node::Generate_Weights_Previous(int number_of_previous_layer) {
	w_connect_to_pre.resize(number_of_previous_layer);
}

// 結合荷重の更新
void Node::Update_Weight(int which_weight, double data) {
	w_connect_to_next[which_weight] += data;
}

void Node::Update_Weight_Pre(int which_weight, double data) {
	w_connect_to_pre[which_weight] += data;
}

// バイアス値の初期化
void Node::Initialize_Bias() {
	bias = INITIAL_VALUE_OF_BIAS;
}

// 次の層への結合荷重を乱数で初期化
void Node::Initialize_Weights_Connect_To_Next() {
	for(int i = 0; i < w_connect_to_next.size(); i++) {
		w_connect_to_next[i] =  myrand();
	}
}

// 次の層への結合荷重を特定の値で初期化したい場合、こちらを使うこと
void Node::Initialize_Weights_Connect_To_Next(double weights_data[]) {
	for(int i = 0; i < w_connect_to_next.size(); i++) {
		w_connect_to_next[i] = weights_data[i];
	}
}

// 前の層への結合荷重を乱数で初期化
// 今のところ最初の中間層から入力層へしか使っていない
void Node::Initialize_Weights_Connect_To_Previous() {
	for(int i = 0; i < w_connect_to_pre.size(); i++) {
		w_connect_to_pre[i] =  myrand();
	}
}

// 前の層への結合荷重を特定の値で初期化したい場合、こちらを使うこと
void Node::Initialize_Weights_Connect_To_Previous(double weights_data[]) {
	for(int i = 0; i < w_connect_to_pre.size(); i++) {
		w_connect_to_pre[i] = weights_data[i];
	}
}

double* Node::Get_X_p() {
	return &x;
}

double Node::Get_Specific_Weight(int which_weight) {
	return w_connect_to_next[which_weight];
}

double Node::Get_Specific_Weight_Pre(int which_weight) {
	return w_connect_to_pre[which_weight];
}

double* Node::Get_Bias_p() {
	return &bias;
}

vector<double> Node::Get_W_Connect_To_Next() {
	return w_connect_to_next;
}
