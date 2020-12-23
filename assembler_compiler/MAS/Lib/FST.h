#pragma once
#include "stdafx.h"
//конечные автоматы
#define ARR_OF_FST_WORD_SIZE 12 // массив  ј слов
#define ARR_OF_FST_OP_SIZE 10 // массив одиночных символов  ј( символы + - ...)
namespace FST
{
	enum LEX_TYPE { SYMB_NOT_FOUND = -1, SYMB_TABLE_ID = 0, SYMB_NEED_PRIORITY, SYMB_LEXEMS, SYMB_ARIPH, SYMB_STATIC_LIB, SYMB_LITERAL };//тип лексемы //SYMB_NOT_FOUND = -1 не нашли ошибку,SYMB_NEED_PRIORITY нужаетс€ в приоритете тип лексемы : -1 - ошибка, 0 - дл€ таблицы идентификаторов, 1 - дл€ присвоени€ приоритета, 2 - "обычные лексемы"


	struct RELATION		 // ребро: символ -> вершина графа перехода  ј
	{
		char symbol;			// символ перехода
		short nnode;			// номер смежной вершины
		RELATION(
			char c = 0x00,			// символ перехода
			short ns = NULL			// новое состо€ние
		);
	};

	struct NODE			// вершина графа переходов
	{
		short n_relation;		// количество инцидентных рЄбер
		RELATION *relations;	// инцидентные рЄбра
		NODE();
		NODE(
			short n,				// количество инцидентных рЄбер
			RELATION rel, ...		// список рЄбер
		);
	};

	struct KA        //будет возвращена после поиска по массивам  ј 
	{
		LEX_TYPE lex_type;
		char lexema;
	};

	struct FST			// недетерминированный конечный автомат
	{
		char lexema;			// лексема
		LEX_TYPE lex_type;		// тип лексемы
		string str;				// цепочка (строка, завершаетс€ 0х00)
		short position;			// текуща€ позици€ в цепочке
		short nstates;			// количество состо€ний автомата
		NODE* nodes;			// граф переходов: [0] - начальное состо€ние, [nstates - 1] - конечное
		short* rstates;			// возможные состо€ни€ автомата на данной позиции
		FST();
		FST(
			char c,					// лексема
			LEX_TYPE l_t,			// тип лексемы
			string s,				// цепочка(строка, завершаетс€ 0х00)
			short ns,				// количество состо€ний автомата
			NODE n, ...				// список состо€ний (граф переходов)
		);
	};

	bool execute(		// выполнить распознавание цепочки
		FST& fst			// недетерминированный конечный автомат
	);
	FST* arr_of_fst_op();		//прототип массив автоматов лексем-одиночных символов
	FST* arr_of_fst_word();		// прототип массив автоматов лексем-слов
}