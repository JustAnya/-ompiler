#pragma once
//табл лексем
#include "Log.h"
#define LT_NOPRIORITY 0xffffffff		// нет приоритета
#define LEXEMA_FIXZISE 1				// фиксированный размер лексемы
#define LT_MAXSIZE 4096					// максимальное кол-во строк в таблице лексем
#define TL_TI_NULLIDX 0xffffffff		// нет элемента таблицы идентификаторов
#define LEX_T			't'				//	тип
#define LEX_INT			'n'				//	int
#define LEX_CHAR	    'c'				//	char(c)
#define LEX_VAR		    'v'				//	var(объ€вление)тайп
#define LEX_FUNCTION	'f'				//	function
#define LEX_MAIN		'm'				//	main
#define LEX_RET  	    'r'				//	return
#define LEX_LITERAL		'l'				//	литерал
#define LEX_OUT			'o'				//	out
#define LEX_POWR		'p'				//	power,степень
#define LEX_STRLEN		's'				//	strlen
#define LEX_ID			'i'				//	идентификатор
#define LEX_ARIPH		'a'
#define LEX_SEMICOLON	';'				//	;
#define LEX_COLON		':'				//	:
#define LEX_COMMA		','				//	,
#define LEX_LEFTHESIS	'('				//	(
#define LEX_RIGHTHESIS	')'				//	)
#define LEX_EQUAL		'='				//  =
#define LEX_LEFTBRACE	'{'				//	{
#define LEX_RIGHTBRACE	'}'				//	}
#define LEX_PLUS		'+'				//	+
#define LEX_MINUS		'-'				//	-
#define LEX_STAR		'*'				//	*
#define LEX_STRLIT		'"'				//  "
#define LEX_SUBST		'@'
#define LEX_L			'L'				//  L
#define LEX_SEP			'|'				// символ конца строки
namespace LT
{										// таблица лексем
	struct Entry
	{
		char lexema;						// лексема
		int sn;								// номер строки в исходном коде
		int idxTI;							// индекс в таблице идентификаторов 
		short priority;						// приоритет дл€ операций
	};

	struct LexTable
	{
		int maxize;							// емкость таблицы лексем 
		int size;							// текущий размер таблицы лексем
		Entry* table;						// массив строк табилцы лексем	
	};

	LexTable Create(						// таблица лексем
		int size							// емкость
	);

	Entry GetEntry(LexTable& lextable, int n);

	Entry Add(								// получить строку таблицы лексем
		LexTable& lextable,					// экземпл€р таблицы
		Entry entry							// строка таблцы лексем
	);

	void Delete(LexTable& lextable);		// удалить таблицу лексем
	void ShowTable(Log::LOG log, LexTable& lexTable);//вывод таблицы в лог файл
}
