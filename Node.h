#pragma once

#include "Parameters.h"

// ��������
double myrand();

// �e�w�����m�[�h�Ɋւ���N���X
class Node {

private:
	// �m�[�h�̎��l
	double x;

	// �P�O�̑w�̂ƃm�[�h�Ƃ̌����׏d
	vector<double> w_connect_to_pre;

	// �P���̃��C���[�Ƃ̃m�[�h�Ƃ̌����׏d
	vector<double> w_connect_to_next;

	// �o�C�A�X
	double bias;

public:

	Node();

	// ���̑w�ւ̌����׏d�𐶐�����
	void Generate_Weights_Next(int number_of_nodes_of_next_layer);

	// �O�̑w�ւ̌����׏d�𐶐�����
	// ���̂Ƃ���ŏ��̒��ԑw������͑w�ւ����g���Ă��Ȃ�
	void Generate_Weights_Previous(int number_of_previous_layer);

	// �����׏d�̍X�V
	void Update_Weight(int which_weight, double data);

	void Update_Weight_Pre(int which_weight, double data);

	// �o�C�A�X�l�̏�����
	void Initialize_Bias();

	// ���̑w�ւ̌����׏d�𗐐��ŏ�����
	void Initialize_Weights_Connect_To_Next();

	// ���̑w�ւ̌����׏d�����̒l�ŏ������������ꍇ�A��������g������
	void Initialize_Weights_Connect_To_Next(double weights_data[]);

	// �O�̑w�ւ̌����׏d�𗐐��ŏ�����
	// ���̂Ƃ���ŏ��̒��ԑw������͑w�ւ����g���Ă��Ȃ�
	void Initialize_Weights_Connect_To_Previous();

	// �O�̑w�ւ̌����׏d�����̒l�ŏ������������ꍇ�A��������g������
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

// ���̑w�ւ̌����׏d�𐶐�����
void Node::Generate_Weights_Next(int number_of_nodes_of_next_layer) {
	if(number_of_nodes_of_next_layer) {
		w_connect_to_next.resize(number_of_nodes_of_next_layer);
	};
}

// �O�̑w�ւ̌����׏d�𐶐�����
// ���̂Ƃ���ŏ��̒��ԑw������͑w�ւ����g���Ă��Ȃ�
void Node::Generate_Weights_Previous(int number_of_previous_layer) {
	w_connect_to_pre.resize(number_of_previous_layer);
}

// �����׏d�̍X�V
void Node::Update_Weight(int which_weight, double data) {
	w_connect_to_next[which_weight] += data;
}

void Node::Update_Weight_Pre(int which_weight, double data) {
	w_connect_to_pre[which_weight] += data;
}

// �o�C�A�X�l�̏�����
void Node::Initialize_Bias() {
	bias = INITIAL_VALUE_OF_BIAS;
}

// ���̑w�ւ̌����׏d�𗐐��ŏ�����
void Node::Initialize_Weights_Connect_To_Next() {
	for(int i = 0; i < w_connect_to_next.size(); i++) {
		w_connect_to_next[i] =  myrand();
	}
}

// ���̑w�ւ̌����׏d�����̒l�ŏ������������ꍇ�A��������g������
void Node::Initialize_Weights_Connect_To_Next(double weights_data[]) {
	for(int i = 0; i < w_connect_to_next.size(); i++) {
		w_connect_to_next[i] = weights_data[i];
	}
}

// �O�̑w�ւ̌����׏d�𗐐��ŏ�����
// ���̂Ƃ���ŏ��̒��ԑw������͑w�ւ����g���Ă��Ȃ�
void Node::Initialize_Weights_Connect_To_Previous() {
	for(int i = 0; i < w_connect_to_pre.size(); i++) {
		w_connect_to_pre[i] =  myrand();
	}
}

// �O�̑w�ւ̌����׏d�����̒l�ŏ������������ꍇ�A��������g������
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
