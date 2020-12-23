#pragma once
#include "stdafx.h"
#define LEX_SUB '@'

void CallPolishNot(LT::LexTable* lextable, IT::IdTable* idtable);

bool PolishNot( // построение польской записи в таблице лексем
	int				lextable_pos,	// позиция выражения в lextable
	LT::LexTable*	lextable,		// таблица лексем
	IT::IdTable*	idtable			// таблица идентификаторов
);
