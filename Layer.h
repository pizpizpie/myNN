#pragma once

#include <windows.h>

#include "Parameters.h"
#include "Node.h"

using namespace std;

// ��������
double myrand();

// �j���[�����l�b�g���[�N�̊e�w�Ɋւ���N���X
class Layer {

private:

	vector<Node> nodes;
	int number_of_nodes;

public:

	void Setting(int _number_of_nodes);

	// ���̑w�����m�[�h�̐���Ԃ�
	int Get_Number_Of_Nodes();

	// �ЂƂ��̑w�ɑ΂��錋���׏d�̐���
	void Generate_Weights_Of_Nodes_Next(int number_of_nodes_of_next_layer);

	// �ЂƂO�̑w�ɑ΂��錋���׏d�̐���
	// ���󂾂ƁA���͑w�ƍŏ��̒��ԑw�̌����׏d�̐����ɂ݂̂Ɏg�p���Ă��܂�.
	void Generate_Weights_Of_Nodes_Previous(int number_of_nodes_of_previous_layer);

	// �ЂƂ��̑w�ւ̌����׏d�𗐐��ŏ���������
	void Initizlize_Weights_Of_Nodes();

	// ����̒l�Ō����׏d���������������ꍇ�A��������g������
	void Initizlize_Weights_Of_Nodes(double weights_data[]);

	// �ЂƂO�̑w�ւ̌����׏d�𗐐��ŏ���������
	void Initialize_Weights_For_Previous_Layer();

	double* Get_Specified_Cell_p(int which_cell);

	double Get_Specified_Weight(int which_cell, int which_weight);

	// �����ƁA��O�̃m�[�h
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

// ���̑w�����m�[�h�̐���Ԃ�
int Layer::Get_Number_Of_Nodes() {
	return number_of_nodes;
}

// �ЂƂ��̑w�ɑ΂��錋���׏d�̐���
void Layer::Generate_Weights_Of_Nodes_Next(int number_of_nodes_of_next_layer) {
	for(int i = 0; i < number_of_nodes; i++) {
		nodes[i].Generate_Weights_Next(number_of_nodes_of_next_layer);
	}
}

// �ЂƂO�̑w�ɑ΂��錋���׏d�̐���
// ���󂾂ƁA���͑w�ƍŏ��̒��ԑw�̌����׏d�̐����ɂ݂̂Ɏg�p���Ă��܂�.
void Layer::Generate_Weights_Of_Nodes_Previous(int number_of_nodes_of_previous_layer) {
	for(int i = 0; i < number_of_nodes; i++) {
		nodes[i].Generate_Weights_Previous(number_of_nodes_of_previous_layer);
	}
}

// �ЂƂ��̑w�ւ̌����׏d�𗐐��ŏ���������
void Layer::Initizlize_Weights_Of_Nodes() {
	for(int i = 0; i < number_of_nodes; i++) { 
		nodes[i].Initialize_Weights_Connect_To_Next();
	}
}

// ����̒l�Ō����׏d���������������ꍇ�A��������g������
void Layer::Initizlize_Weights_Of_Nodes(double weights_data[]) {
	for(int i = 0; i < number_of_nodes; i++) { 
		nodes[i].Initialize_Weights_Connect_To_Next(weights_data);
	}
}

// �ЂƂO�̑w�ւ̌����׏d�𗐐��ŏ���������
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

// �����ƁA��O�̃m�[�h
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
