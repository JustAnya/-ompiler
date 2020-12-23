#include "stdafx.h"
#include "In.h"
#include "Error.h"
//� �� ���������� ���� ��� ������ �����������(�������)
namespace In
{
	IN GetIn(wchar_t infile[])
	{
		ifstream source_file(infile);         //��������� ����� ��� ��������� �� �����
		if (source_file.fail())
		{
			throw ERROR_THROW(110);
		}
		IN in;                                // ������������ ���������
		Error::ERROR err;                     // ������ ��� ������ ������ ������ ������ � ������ ������
		Error::ARRAY_OF_ERRORS errors;		  // ������ ��� ��������� ������
		in.size = in.ignor = 0;	  // ������������� ��������� ��������� In::IN
		in.lines = 1;
		unsigned char symb;		              //������� ������ �������� �����(��������)
		int size = 0,			              // ������� ������ ������������� �������
			col = 0;			              // ����� ������� ������� � ������	
		char temp[IN_WORD_MAX_LEN];        	  // �������� ������ ��� ������������ ����-������
		short wordLen = 0;			          // ������� ����� temp

		while ((!source_file.eof()) && (symb = source_file.get())) //... � ���� ������ ��� ����-��
		{
			if (symb == '\"')
			{
				temp[wordLen++] = symb;//���������� � ��������� �� ���� �������
				do                     //���� �� �������� ������ �������
				{
					symb = source_file.get();
					temp[wordLen++] = symb;
					in.size++; if (wordLen == IN_WORD_MAX_LEN)
					{
						throw ERROR_THROW_IN(101, in.lines, col);
					}
				} while (symb != '\"' && wordLen < IN_WORD_MAX_LEN);

				temp[wordLen] = '\0';               //�������� ������
				in.Lexema[size++] = { temp, in.lines }; //�����������  ����� ���� ������� � ���� ��������� ���� sep
				wordLen = 0;
				in.size += 1;
				continue;
			}

			if (symb == IN_CODE_SPACE || in.code[(int)symb] == IN::S || symb == IN_CODE_ENDL)
			{
				if (temp && wordLen)//���� ������ �� ������ � ������ ����� �� 0
				{
					temp[wordLen] = '\0';
					in.Lexema[size++] = { temp, in.lines }; //�����������  ����� ���� ������� � ���� ��������� ���� sep
					wordLen = 0;
				}
				if (symb == IN_CODE_SPACE)				// �������� �� �������
				{
					in.ignor += 1;
				}
				if (in.code[(int)symb] == IN::S)		// �������� �� ������-���������
				{                                       //������� �� ���������� ���������� ������
					char buf[2];
					buf[0] = symb;
					buf[1] = '\0';
					in.Lexema[size++] = { buf, in.lines }; //�����������  ����� ���� ������� � ���� ��������� ���� sep
					wordLen = 0;
				}
				if (symb == IN_CODE_ENDL)				// �������� �� ������ �������� �� ����� ������
				{
					in.lines++;
					col = 0;
				}

			}
			else if (in.code[(int)symb] == IN::T)         //���� �� ��������
			{
				col++;
				temp[wordLen++] = symb;
				in.size++;
			}
			else
			{
				err = ERROR_THROW_IN(111, in.lines, col);
				Error::AddError(&errors, err);        //��������� ������ � ������ ������
			}
			if (wordLen == IN_MAX_ID)
			{
				err = ERROR_THROW_IN(113, in.lines, col);
				Error::AddError(&errors, err);
			}
			if (in.size == IN_MAX_LEN_TEXT)
			{
				throw ERROR_THROW(104);
			}
		}
		if (errors.size)
		{
			throw errors;     //������� ������ errors
		}
		source_file.close();
		in.array_size = size;
		return in;
	}
}