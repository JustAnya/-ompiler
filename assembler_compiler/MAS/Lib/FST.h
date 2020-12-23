#pragma once
#include "stdafx.h"
//�������� ��������
#define ARR_OF_FST_WORD_SIZE 12 // ������ �� ����
#define ARR_OF_FST_OP_SIZE 10 // ������ ��������� �������� ��( ������� + - ...)
namespace FST
{
	enum LEX_TYPE { SYMB_NOT_FOUND = -1, SYMB_TABLE_ID = 0, SYMB_NEED_PRIORITY, SYMB_LEXEMS, SYMB_ARIPH, SYMB_STATIC_LIB, SYMB_LITERAL };//��� ������� //SYMB_NOT_FOUND = -1 �� ����� ������,SYMB_NEED_PRIORITY �������� � ���������� ��� ������� : -1 - ������, 0 - ��� ������� ���������������, 1 - ��� ���������� ����������, 2 - "������� �������"


	struct RELATION		 // �����: ������ -> ������� ����� �������� ��
	{
		char symbol;			// ������ ��������
		short nnode;			// ����� ������� �������
		RELATION(
			char c = 0x00,			// ������ ��������
			short ns = NULL			// ����� ���������
		);
	};

	struct NODE			// ������� ����� ���������
	{
		short n_relation;		// ���������� ����������� ����
		RELATION *relations;	// ����������� ����
		NODE();
		NODE(
			short n,				// ���������� ����������� ����
			RELATION rel, ...		// ������ ����
		);
	};

	struct KA        //����� ���������� ����� ������ �� �������� �� 
	{
		LEX_TYPE lex_type;
		char lexema;
	};

	struct FST			// ������������������� �������� �������
	{
		char lexema;			// �������
		LEX_TYPE lex_type;		// ��� �������
		string str;				// ������� (������, ����������� 0�00)
		short position;			// ������� ������� � �������
		short nstates;			// ���������� ��������� ��������
		NODE* nodes;			// ���� ���������: [0] - ��������� ���������, [nstates - 1] - ��������
		short* rstates;			// ��������� ��������� �������� �� ������ �������
		FST();
		FST(
			char c,					// �������
			LEX_TYPE l_t,			// ��� �������
			string s,				// �������(������, ����������� 0�00)
			short ns,				// ���������� ��������� ��������
			NODE n, ...				// ������ ��������� (���� ���������)
		);
	};

	bool execute(		// ��������� ������������� �������
		FST& fst			// ������������������� �������� �������
	);
	FST* arr_of_fst_op();		//�������� ������ ��������� ������-��������� ��������
	FST* arr_of_fst_word();		// �������� ������ ��������� ������-����
}