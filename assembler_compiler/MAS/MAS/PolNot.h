#pragma once
#include "stdafx.h"
#define LEX_SUB '@'

void CallPolishNot(LT::LexTable* lextable, IT::IdTable* idtable);

bool PolishNot( // ���������� �������� ������ � ������� ������
	int				lextable_pos,	// ������� ��������� � lextable
	LT::LexTable*	lextable,		// ������� ������
	IT::IdTable*	idtable			// ������� ���������������
);
