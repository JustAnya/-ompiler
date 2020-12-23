#pragma once
#include "stdafx.h"
#define IN_MAX_LEN_TEXT 1024*1024			// ������������ ������ ��������� ���� 1��
#define IN_MAX_ID 10                        //���� ����� ��
#define IN_CODE_ENDL	'\n'				// ������ ����� ������
#define STR_NUMB_SEP	'|'					// ��������� ��� ����� ������
#define IN_CODE_SPACE	' '					// ������
#define IN_WORD_MAX_LEN 255					// ������������ ������ �����
#define LEXEMA_MAX_SIZE 300
// �������� IN::F -- ����������� ������, IN::T  --  ����������� ������,
// IN::S -- ����������	(	)	*	+	-	:	;	{	} =	
#define IN_CODE_TABLE {\
	   	 /*0*/	/*1*/  /*2*/  /*3*/  /*4*/  /*5*/  /*6*/  /*7*/  /*8*/  /*9*/  /*A*/  /*B*/  /*C*/  /*D*/  /*E*/  /*F*/  \
/*0*/    IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::T, IN::F, IN::F, IN::F, IN::F, IN::F, \
/*1*/    IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
/*2*/    IN::T, IN::F, IN::T, IN::F, IN::F, IN::F, IN::F, IN::F, IN::S, IN::S, IN::S, IN::S, IN::S, IN::S, IN::T, IN::F, \
/*3*/    IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::S, IN::S, IN::F, IN::S, IN::F, IN::F, \
/*4*/	 IN::F, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
/*5*/    IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::F, IN::F, IN::F, IN::F, IN::F, \
/*6*/    IN::F, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
/*7*/    IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::S, IN::F, IN::S, IN::F, IN::F, \
     \
/*8*/    IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
/*9*/    IN::F, IN::F, IN::F, IN::T, IN::T, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
/*A*/    IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
/*B*/    IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, IN::F, \
/*C*/    IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
/*D*/    IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
/*E*/    IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, \
/*F*/    IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T, IN::T \
    }//��������� ������ ���,������� ; : - * +,() {}

namespace In
{
	struct Sep                              //��� �� �� ������ ������ ��� ������ ����� ���� �����������
	{
		string word;                        //����� �������� �� ������
		int line;                           //������ � �������� ����� ����� �����
	};
	struct IN
	{
		enum { T = 1, F = 2, S = 3 };		// � - ���������� ������; F - ������������ ������; S - ������-���������
		int size;							// ������� ������ ��������� ����
		int lines;							// ���������� �����
		int ignor;							// ���������� ����������������� ��������
		Sep Lexema[LEXEMA_MAX_SIZE];	    // ������ ���� (��� �������� � ��������)
		int array_size;						// ���������� ���� � ������� Lexema
		int code[256] = IN_CODE_TABLE;		// ������� ��������
	};
	IN GetIn(wchar_t infile[]);             // �������� �������� �������� ����� �� ������������(������ � �������� ������� �����)
};
