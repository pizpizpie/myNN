#pragma once

// �����V�[�h�����������邩�̃t���O
#define USE_RANDOM_SEED				(TRUE)

// ���ԃf�[�^��\�����邩�̃t���O
#define FSHOW_LOG					(TRUE)

// ���t�f�[�^�̑��΃p�X
#define PATH_TRAINING_DATA			(_T("data/Training/\0"))

// �e�X�g�f�[�^�̑��΃p�X
#define PATH_TEST_DATA				(_T("data/Test/\0"))

// ���t�f�[�^�A�e�X�g�f�[�^���܂ރt�@�C�����̍ő�l
#define MAX_FILES					(1)

// ���t�f�[�^�̍s��
#define NUMBER_OF_TRAINING_DATA		(140)

// ���t�f�[�^�̗�
#define MAX_COLUMN					(409)

// �e�X�g�f�[�^�̍s��
#define NUMBER_OF_TEST_DATA			(260)

// �e�X�g�f�[�^�Ɋ܂܂��N���X1�̐�
#define LEN_CLASS1					(60)

// �e�X�g�f�[�^�Ɋ܂܂��N���X2�̐�
#define LEN_CLASS2					(200)

// ���͑w�̃m�[�h��
// ���t�f�[�^�̗�(�������̂݁A�N���X�̓J�E���g���Ȃ�)
#define INPUTNO						(MAX_COLUMN - 1)

// ���ԑw�̑���
// 3�wNN�Ȃ�1�ɐݒ�A���w�Ȃ炻��ȏ��
#define NUMBER_OF_HIDDEN_LAYERS		(1)

// ���ԑw�e�w�̃m�[�h��
#define NUMBER_OF_HIDDEN_NODES		(50)

// �o�͑w�̃m�[�h��
#define NUMBER_OF_OUT_NODES			(1)

// �o�C�A�X�̏����l
#define INITIAL_VALUE_OF_BIAS		(1.0f)

// �w�K�W��
#define ALPHA						(0.5)

// �����̃V�[�h
#define SEED						(65535)

// �덷�̏����l
#define INITIAL_ERR					(10)

// �G���[���̉����A�l�b�g���[�N�̌덷�����������ꂪ�w�K�𒆒f
// (�G�|�b�N���Ƃ̌��ˍ����ɒ���)
#define LIMIT						(0.001)

// �G�|�b�N��
#define EPOCH						(100)

// 臒l, �e�X�g�f�[�^�̌��ʂ̔��ʂɗ��p 
// (���ʂ�����𒴂�����N���X1, �����Ȃ�������N���X2, �Ȃ�)
#define THRESHOLD					(0.5f)
