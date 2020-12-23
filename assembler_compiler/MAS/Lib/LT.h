#pragma once
//���� ������
#include "Log.h"
#define LT_NOPRIORITY 0xffffffff		// ��� ����������
#define LEXEMA_FIXZISE 1				// ������������� ������ �������
#define LT_MAXSIZE 4096					// ������������ ���-�� ����� � ������� ������
#define TL_TI_NULLIDX 0xffffffff		// ��� �������� ������� ���������������
#define LEX_T			't'				//	���
#define LEX_INT			'n'				//	int
#define LEX_CHAR	    'c'				//	char(c)
#define LEX_VAR		    'v'				//	var(����������)����
#define LEX_FUNCTION	'f'				//	function
#define LEX_MAIN		'm'				//	main
#define LEX_RET  	    'r'				//	return
#define LEX_LITERAL		'l'				//	�������
#define LEX_OUT			'o'				//	out
#define LEX_POWR		'p'				//	power,�������
#define LEX_STRLEN		's'				//	strlen
#define LEX_ID			'i'				//	�������������
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
#define LEX_SEP			'|'				// ������ ����� ������
namespace LT
{										// ������� ������
	struct Entry
	{
		char lexema;						// �������
		int sn;								// ����� ������ � �������� ����
		int idxTI;							// ������ � ������� ��������������� 
		short priority;						// ��������� ��� ��������
	};

	struct LexTable
	{
		int maxize;							// ������� ������� ������ 
		int size;							// ������� ������ ������� ������
		Entry* table;						// ������ ����� ������� ������	
	};

	LexTable Create(						// ������� ������
		int size							// �������
	);

	Entry GetEntry(LexTable& lextable, int n);

	Entry Add(								// �������� ������ ������� ������
		LexTable& lextable,					// ��������� �������
		Entry entry							// ������ ������ ������
	);

	void Delete(LexTable& lextable);		// ������� ������� ������
	void ShowTable(Log::LOG log, LexTable& lexTable);//����� ������� � ��� ����
}
