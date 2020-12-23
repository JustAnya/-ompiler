#include "stdafx.h"
#include "FST.h"
#include "LT.h"

namespace FST
{
	bool step(FST& fst, short* &rstates)	//один шаг автомата
	{
		bool rc = false;
		swap(rstates, fst.rstates);	//смена массивов
		for (short i = 0; i < fst.nstates; i++) // проходимся по всем состояниям
		{
			if (rstates[i] == fst.position) // если позиция найдена, то..
				for (short j = 0; j < fst.nodes[i].n_relation; j++) // двигаемся по всем ремрам этой вершины
				{
					if (fst.nodes[i].relations[j].symbol == fst.str[fst.position])
					{
						fst.rstates[fst.nodes[i].relations[j].nnode] = fst.position + 1;
						rc = true;
					};
				};
		};
		return rc;
	};

	bool execute(FST& fst)             //разбор КА
	{
		short* rstates = new short[fst.nstates];
		memset(rstates, 0xff, sizeof(short)*fst.nstates);
		short lstring = fst.str.size();
		bool rc = true;
		for (short i = 0; i < lstring && rc; i++)
		{
			fst.position++;				//продвинули позицию
			rc = step(fst, rstates);	//один шаг автомата
		};
		delete[] rstates;
		return (rc ? (fst.rstates[fst.nstates - 1] == lstring) : rc);
	}

	RELATION::RELATION(char c, short nn)
	{
		symbol = c;
		nnode = nn;
	};

	NODE::NODE()	//по умолчанию
	{
		n_relation = 0;
		RELATION *relations = NULL;
	};

	NODE::NODE(short n, RELATION rel, ...)	//с параметрами
	{
		n_relation = n;
		RELATION *p = &rel;
		relations = new RELATION[n];
		for (short i = 0; i < n; i++)
		{
			relations[i] = p[i];
		}
	};

	FST::FST(char c, LEX_TYPE l_t, string s, short ns, NODE n, ...) //--------------------------------------------------?
	{
		lexema = c;
		str = s;
		nstates = ns;
		lex_type = l_t;
		nodes = new NODE[ns];
		NODE *p = &n;
		for (int k = 0; k < ns; k++) nodes[k] = p[k];
		rstates = new short[nstates];
		memset(rstates, 0xff, sizeof(short)*nstates);
		rstates[0] = 0;
		position = -1;
	};
	FST::FST()
	{}

	FST* arr_of_fst_op()//массив КА символов    
	{
		FST Arifm(LEX_ARIPH, SYMB_ARIPH, "", 2, NODE(3, RELATION('+', 1), RELATION('-', 1), RELATION('*', 1)), \
			NODE()
		);
		FST LeftHes(LEX_LEFTHESIS, SYMB_NEED_PRIORITY, "", 2, NODE(1, RELATION('(', 1)), \
			NODE()
		);
		FST RightHes(LEX_RIGHTHESIS, SYMB_NEED_PRIORITY, "", 2, NODE(1, RELATION(')', 1)), \
			NODE()
		);
		FST Equally(LEX_EQUAL, SYMB_LEXEMS, "", 2, NODE(1, RELATION('=', 1)), \
			NODE()
		);
		FST LeftBrace(LEX_LEFTBRACE, SYMB_LEXEMS, "", 2, NODE(1, RELATION('{', 1)), \
			NODE()
		);
		FST RightBrace(LEX_RIGHTBRACE, SYMB_LEXEMS, "", 2, NODE(1, RELATION('}', 1)), \
			NODE()
		);
		FST Semicolon(LEX_SEMICOLON, SYMB_LEXEMS, "", 2, NODE(1, RELATION(';', 1)), \
			NODE()
		);

		FST Comma(LEX_COMMA, SYMB_NEED_PRIORITY, "", 2, NODE(1, RELATION(',', 1)), \
			NODE()
		);

		FST IntLiteral(LEX_INT, SYMB_LITERAL, "", 2, NODE(10,
			RELATION('0', 1), RELATION('1', 1), RELATION('2', 1),
			RELATION('3', 1), RELATION('4', 1), RELATION('5', 1),
			RELATION('6', 1), RELATION('7', 1), RELATION('8', 1),
			RELATION('9', 1)),
			NODE()
		);
		FST Id(LEX_ID, SYMB_TABLE_ID, "", 2, NODE(26, RELATION('a', 1), RELATION('b', 1), RELATION('c', 1), RELATION('d', 1),
			RELATION('e', 1), RELATION('f', 1), RELATION('g', 1), RELATION('h', 1),
			RELATION('i', 1), RELATION('j', 1), RELATION('k', 1), RELATION('l', 1),
			RELATION('m', 1), RELATION('n', 1), RELATION('o', 1), RELATION('p', 1),
			RELATION('q', 1), RELATION('r', 1), RELATION('s', 1), RELATION('t', 1),
			RELATION('u', 1), RELATION('v', 1), RELATION('w', 1), RELATION('x', 1),
			RELATION('y', 1), RELATION('z', 1)),
			NODE()
		);
		FST* result = new FST[ARR_OF_FST_OP_SIZE]; //массив для поиска лексемы
		result[0] = Arifm;				//
		result[1] = Semicolon;			//
		result[2] = Comma;				//
		result[3] = LeftHes;			//
		result[4] = RightHes;			//
		result[5] = Equally;			//
		result[6] = LeftBrace;			//	
		result[7] = RightBrace;			//
		result[8] = IntLiteral;
		result[9] = Id;
		return result;
	}
	FST* arr_of_fst_word()//массив КА слов
	{
		FST Int(LEX_INT, SYMB_LEXEMS, "", 4, NODE(1, RELATION('i', 1)),
			NODE(1, RELATION('n', 2)),
			NODE(1, RELATION('t', 3)),
			NODE()
		);
		FST Char(LEX_CHAR, SYMB_LEXEMS, "", 5, NODE(1, RELATION('c', 1)),
			NODE(1, RELATION('h', 2)),
			NODE(1, RELATION('a', 3)),
			NODE(1, RELATION('r', 4)),
			NODE()
		);
		FST Function(LEX_FUNCTION, SYMB_LEXEMS, "", 9, NODE(1, RELATION('f', 1)),
			NODE(1, RELATION('u', 2)),
			NODE(1, RELATION('n', 3)),
			NODE(1, RELATION('c', 4)),
			NODE(1, RELATION('t', 5)),
			NODE(1, RELATION('i', 6)),
			NODE(1, RELATION('o', 7)),
			NODE(1, RELATION('n', 8)),
			NODE()
		);
		FST Var(LEX_VAR, SYMB_LEXEMS, "", 4, NODE(1, RELATION('v', 1)),
			NODE(1, RELATION('a', 2)),
			NODE(1, RELATION('r', 3)),
			NODE()
		);
		FST Main(LEX_MAIN, SYMB_LEXEMS, "", 5, NODE(1, RELATION('m', 1)),
			NODE(1, RELATION('a', 2)),
			NODE(1, RELATION('i', 3)),
			NODE(1, RELATION('n', 4)),
			NODE()
		);
		FST Ret(LEX_RET, SYMB_LEXEMS, "", 4, NODE(1, RELATION('r', 1)),
			NODE(1, RELATION('e', 2)),
			NODE(1, RELATION('t', 3)),
			NODE()
		);
		FST IntLiteral(LEX_INT, SYMB_LITERAL, "", 2, NODE(20, RELATION('0', 0), RELATION('1', 0), RELATION('2', 0),
			RELATION('3', 0), RELATION('4', 0), RELATION('5', 0),
			RELATION('6', 0), RELATION('7', 0), RELATION('8', 0),
			RELATION('9', 0),
			RELATION('0', 1), RELATION('1', 1), RELATION('2', 1),
			RELATION('3', 1), RELATION('4', 1), RELATION('5', 1),
			RELATION('6', 1), RELATION('7', 1), RELATION('8', 1),
			RELATION('9', 1)),
			NODE()
		);
		FST Id(LEX_ID, SYMB_TABLE_ID, "", 2, NODE(52, RELATION('a', 0), RELATION('b', 0), RELATION('c', 0), RELATION('d', 0),
			RELATION('e', 0), RELATION('f', 0), RELATION('g', 0), RELATION('h', 0),
			RELATION('i', 0), RELATION('j', 0), RELATION('k', 0), RELATION('l', 0),
			RELATION('m', 0), RELATION('n', 0), RELATION('o', 0), RELATION('p', 0),
			RELATION('q', 0), RELATION('r', 0), RELATION('s', 0), RELATION('t', 0),
			RELATION('u', 0), RELATION('v', 0), RELATION('w', 0), RELATION('x', 0),
			RELATION('y', 0), RELATION('z', 0),
			RELATION('a', 1), RELATION('b', 1), RELATION('c', 1), RELATION('d', 1),
			RELATION('e', 1), RELATION('f', 1), RELATION('g', 1), RELATION('h', 1),
			RELATION('i', 1), RELATION('j', 1), RELATION('k', 1), RELATION('l', 1),
			RELATION('m', 1), RELATION('n', 1), RELATION('o', 1), RELATION('p', 1),
			RELATION('q', 1), RELATION('r', 1), RELATION('s', 1), RELATION('t', 1),
			RELATION('u', 1), RELATION('v', 1), RELATION('w', 1), RELATION('x', 1),
			RELATION('y', 1), RELATION('z', 1)),
			NODE()
		);//символьный литерал
		FST StringLit(LEX_CHAR, SYMB_LITERAL, "", 4, NODE(2, RELATION('"', 1), RELATION('"', 2)), \
			NODE(158, \
				RELATION('a', 1), RELATION('b', 1), RELATION('c', 1), RELATION('d', 1), RELATION('e', 1), RELATION('f', 1), \
				RELATION('g', 1), RELATION('h', 1), RELATION('i', 1), RELATION('j', 1), RELATION('k', 1), RELATION('l', 1), \
				RELATION('m', 1), RELATION('n', 1), RELATION('o', 1), RELATION('p', 1), RELATION('q', 1), RELATION('r', 1), \
				RELATION('s', 1), RELATION('t', 1), RELATION('u', 1), RELATION('v', 1), RELATION('w', 1), RELATION('x', 1), \
				RELATION('y', 1), RELATION('z', 1), RELATION('1', 1), RELATION('2', 1), RELATION('3', 1), RELATION('4', 1), \
				RELATION('5', 1), RELATION('6', 1), RELATION('7', 1), RELATION('8', 1), RELATION('9', 1), RELATION('0', 1), \
				\
				RELATION('а', 1), RELATION('б', 1), RELATION('в', 1), RELATION('г', 1), RELATION('д', 1), RELATION('е', 1), \
				RELATION('ё', 1), RELATION('ж', 1), RELATION('з', 1), RELATION('и', 1), RELATION('й', 1), RELATION('к', 1), \
				RELATION('л', 1), RELATION('м', 1), RELATION('н', 1), RELATION('о', 1), RELATION('п', 1), RELATION('р', 1), \
				RELATION('с', 1), RELATION('т', 1), RELATION('у', 1), RELATION('ф', 1), RELATION('х', 1), RELATION('ц', 1), \
				RELATION('ч', 1), RELATION('ш', 1), RELATION('щ', 1), RELATION('ъ', 1), RELATION('ы', 1), RELATION('ь', 1), \
				RELATION('э', 1), RELATION('ю', 1), RELATION('я', 1), RELATION(' ', 1), RELATION('.', 1), RELATION(',', 1), \
				RELATION('?', 1), RELATION('!', 1), RELATION(';', 1), RELATION(':', 1), RELATION('-', 1), RELATION(')', 1), \
				RELATION('(', 1), \
				\
				RELATION('a', 2), RELATION('b', 2), RELATION('c', 2), RELATION('d', 2), RELATION('e', 2), RELATION('f', 2), \
				RELATION('g', 2), RELATION('h', 2), RELATION('i', 2), RELATION('j', 2), RELATION('k', 2), RELATION('l', 2), \
				RELATION('m', 2), RELATION('n', 2), RELATION('o', 2), RELATION('p', 2), RELATION('q', 2), RELATION('r', 2), \
				RELATION('s', 2), RELATION('t', 2), RELATION('u', 2), RELATION('v', 2), RELATION('w', 2), RELATION('x', 2), \
				RELATION('y', 2), RELATION('z', 2), RELATION('1', 2), RELATION('2', 2), RELATION('3', 2), RELATION('4', 2), \
				RELATION('5', 2), RELATION('6', 2), RELATION('7', 2), RELATION('8', 2), RELATION('9', 2), RELATION('0', 2), \
				\
				RELATION('а', 2), RELATION('б', 2), RELATION('в', 2), RELATION('г', 2), RELATION('д', 2), RELATION('е', 2), \
				RELATION('ё', 2), RELATION('ж', 2), RELATION('з', 2), RELATION('и', 2), RELATION('й', 2), RELATION('к', 2), \
				RELATION('л', 2), RELATION('м', 2), RELATION('н', 2), RELATION('о', 2), RELATION('п', 2), RELATION('р', 2), \
				RELATION('с', 2), RELATION('т', 2), RELATION('у', 2), RELATION('ф', 2), RELATION('х', 2), RELATION('ц', 2), \
				RELATION('ч', 2), RELATION('ш', 2), RELATION('щ', 2), RELATION('ъ', 2), RELATION('ы', 2), RELATION('ь', 2), \
				RELATION('э', 2), RELATION('ю', 2), RELATION('я', 2), RELATION(' ', 2), RELATION('.', 2), RELATION(',', 2), \
				RELATION('?', 2), RELATION('!', 2), RELATION(';', 2), RELATION(':', 2), RELATION('-', 2), RELATION(')', 2), \
				RELATION('(', 2)), \
			\
			NODE(1, RELATION('"', 3)), \
			NODE()
		);

		FST Out(LEX_OUT, SYMB_STATIC_LIB, "", 4, NODE(1, RELATION('o', 1)), \
			NODE(1, RELATION('u', 2)), \
			NODE(1, RELATION('t', 3)), \
			NODE()
		);
		FST Powr(LEX_POWR, SYMB_STATIC_LIB, "", 5, NODE(1, RELATION('p', 1)), \
			NODE(1, RELATION('o', 2)), \
			NODE(1, RELATION('w', 3)), \
			NODE(1, RELATION('r', 4)), \
			NODE()
		);
		FST Strlen(LEX_STRLEN, SYMB_STATIC_LIB, "", 6, NODE(1, RELATION('s', 1)), \
			NODE(1, RELATION('t', 2)), \
			NODE(1, RELATION('r', 3)), \
			NODE(1, RELATION('l', 4)), \
			NODE(1, RELATION('n', 5)), \
			NODE()
		);
		FST* result = new FST[ARR_OF_FST_WORD_SIZE];
		result[0] = Int;
		result[1] = Char;
		result[2] = Var;
		result[3] = Function;
		result[4] = Main;
		result[5] = Ret;
		result[6] = IntLiteral;
		result[7] = StringLit;
		result[8] = Out;
		result[9] = Powr;
		result[10] = Strlen;
		result[11] = Id;
		return result;
	}
}