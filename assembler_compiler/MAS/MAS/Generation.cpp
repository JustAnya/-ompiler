#include "stdafx.h"
#include "Generation.h"
#include <stack>
//���������� �����, ������ �������� ��� ���� ��������, � ������� ���������� � ��� ����������� ����������, � ������ ��� �������������� ��� ���������
//� ������� �� ������� ��� ��� ����� ������ �� ����������
namespace Generation
{
	void Generation(Lex::LEX lex, Log::LOG log)//������� ��������� ���� � ������� ���� ������ � ������ � ���-�� ��� � ��� ������ ������� ����� ��� ������ � ���� 
	{
		*(log.stream) << ".586\n\t.model flat, stdcall\n\tincludelib libucrt.lib\n\tincludelib kernel32.lib\n\t	includelib ../Debug/MASAsmLibrary.lib\n\tExitProcess PROTO :DWORD\n\n";
		*(log.stream) << "\toutc PROTO: DWORD\n\toutd PROTO: DWORD\n\tstrln PROTO: DWORD\n\tpowr PROTO: DWORD, :DWORD\n";
		*(log.stream) << "\n.stack 4096\n";

		*(log.stream) << ".const\n";
		for (int i = 0; i < lex.idtable.size; i++)//�������� �� ���� ���� ����� � ���� ��������
		{
			if (lex.idtable.table[i].id_type == IT::L)
			{
				*(log.stream) << "\t" << lex.idtable.table[i].id;
				if (lex.idtable.table[i].id_data_type == IT::CHAR)//������� �� ���� ������
				{
					*(log.stream) << " BYTE " << lex.idtable.table[i].value.vstr.str << ", 0\n";//���� ����������(����� ��������)
				}
				else
				{
					*(log.stream) << " DWORD " << lex.idtable.table[i].value.vint << endl;//���� �������������
				}
			}
		}

		*(log.stream) << ".data\n";
		for (int i = 0; i < lex.lextable.size; i++)//�� ������� ������
		{
			if (lex.lextable.table[i].lexema == LEX_VAR)
			{
				*(log.stream) << "\t" << lex.idtable.table[lex.lextable.table[i + 2].idxTI].id;//������� �� �������-��,���������� ��� � ���� ��� 4���� �������
				*(log.stream) << " DWORD ?\n";
				i += 3;
			}
		}

		stack<string> stk;                      // ���� ��� ���������� ������������������ �������� ���������� � ������� ����������
		string func_name = "";					// ��� ��������� �������
		bool flag_ret = false,
			flag_funcion = false,				// ������ ��������� �������
			flag_main = false;					// ������ ������� �������
		int num_of_ret = 0;

		*(log.stream) << "\n.code\n\n";
		for (int i = 0; i < lex.lextable.size; i++)
		{
			switch (lex.lextable.table[i].lexema)//������� ����� ������ ����
			{
			case LEX_FUNCTION://���� �-��
			{
				*(log.stream) << (func_name = lex.idtable.table[lex.lextable.table[++i].idxTI].id) << " PROC ";//������� ��� �-�� �  ����� � func_name
				while (lex.lextable.table[i].lexema != LEX_RIGHTHESIS)
				{
					if (lex.idtable.table[lex.lextable.table[i].idxTI].id_type == IT::P)//���� ��� ��������
					{
						*(log.stream) << lex.idtable.table[lex.lextable.table[i].idxTI].id << " : ";//������� ��� �� � ����
						if (lex.idtable.table[lex.lextable.table[i].idxTI].id_data_type == IT::INT)//������� �� ��� ���� ������
						{
							*(log.stream) << "SDWORD";//���������� ���� ��� �� �������� 4��������
						}
						else
						{
							*(log.stream) << "DWORD";//��� ������ �����. ����� ���������, ����� -������ ��� ������ �� ���� ����������
						}
					}
					if (lex.lextable.table[i].lexema == LEX_COMMA)
					{
						*(log.stream) << ", ";
					}
					i++;
				}
				flag_funcion = true;
				*(log.stream) << endl;
				break;
			}
			case LEX_MAIN:
			{
				flag_main = true;
				*(log.stream) << "main PROC\n";
				break;
			}
			case LEX_EQUAL:
			{
				int result_position = i - 1;//������� �������. ����������
				while (lex.lextable.table[i].lexema != LEX_SEMICOLON)
				{
					switch (lex.lextable.table[i].lexema)
					{
					case LEX_ID:
					case LEX_INT:
					{
						*(log.stream) << "\tpush " << lex.idtable.table[lex.lextable.table[i].idxTI].id << endl;//��������� � ���� � �������
						stk.push(lex.idtable.table[lex.lextable.table[i].idxTI].id);//� ������������� ��� ����
						break;
					}
					case LEX_CHAR:
					{
						*(log.stream) << "\tpush offset " << lex.idtable.table[lex.lextable.table[i].idxTI].id << endl;//��������� ���������(offset)
						stk.push("offset " + lex.idtable.table[lex.lextable.table[i].idxTI].id);
						break;
					}
					case LEX_SUBST://@-������������� �-�� ����� �������� ������
					{
						for (int j = 1; j <= lex.lextable.table[i].priority; j++)//� �������� ���������� �������� ���-�� ����������
						{
							*(log.stream) << "\tpop edx\n";//"�����������" ���� � �������
						}
						for (int j = 1; j <= lex.lextable.table[i].priority; j++)//stdcall
						{
							*(log.stream) << "\tpush " << stk.top() << endl;//���������� � �������� �������
							stk.pop();//�������� ����,�� ��������� ������������ ����������������� ����������
						}
						*(log.stream) << "\t\tcall " << lex.idtable.table[lex.lextable.table[i].idxTI].id << "\n\tpush eax\n";//��� ��� call �������� �-�� � �������� � ��������� �� eax � ����������� ���� ����������
						break;
					}
					case LEX_ARIPH:
					{
						switch (lex.idtable.table[lex.lextable.table[i].idxTI].id[0])
						{
						case LEX_STAR:
						{
							*(log.stream) << "\tpop eax\n\tpop ebx\n";
							*(log.stream) << "\tmul ebx\n\tpush eax\n";
							break;
						}
						case LEX_PLUS:
						{
							*(log.stream) << "\tpop eax\n\tpop ebx\n";
							*(log.stream) << "\tadd eax, ebx\n\tpush eax\n";
							break;
						}
						case LEX_MINUS:
						{
							*(log.stream) << "\tpop ebx\n\tpop eax\n";
							*(log.stream) << "\tsub eax, ebx\n\tpush eax\n";
							break;
						}
						}
						break;
					}
					}
					i++;
				}
				*(log.stream) << "\tpop " << lex.idtable.table[lex.lextable.table[result_position].idxTI].id << "\n";//����� ��������� ; �� ����� ���������� ������ �������� ����� ��������� � ����������� ��� ���������� � ���. ������
				break;
			}
			case LEX_RET:
			{
				*(log.stream) << "\tpush ";
				i++;
				if (lex.idtable.table[lex.lextable.table[i].idxTI].id_type == IT::L)//���� ������� 
				{
					*(log.stream) << lex.idtable.table[lex.lextable.table[i++].idxTI].value.vint;//���������� ��� ���� � ����������� ����
				}
				else
				{
					*(log.stream) << lex.idtable.table[lex.lextable.table[i++].idxTI].id;//���-� ���� �� � ������. ����
				}
				if (flag_funcion)
				{
					*(log.stream) << "\n\t\tjmp local" << num_of_ret << endl;
					flag_ret = true;
				}
				if (flag_main)
				{
					*(log.stream) << "\n\t\tjmp theend\n";
					flag_ret = true;
				}
				break;
			}
			case LEX_RIGHTBRACE:
			{
				if (flag_main && !flag_funcion)
				{
					if (flag_ret)
					{
						*(log.stream) << "theend:\n";
						flag_ret = false;
					}
					*(log.stream) << "\tcall ExitProcess\nmain ENDP\nend main";
				}
				if (flag_funcion)
				{
					if (flag_ret)
					{
						*(log.stream) << "local" << num_of_ret++ << ":\n";
						*(log.stream) << "\tpop eax\n\tret\n";
						flag_ret = false;
					}
					*(log.stream) << func_name << " ENDP\n\n";
					flag_funcion = false;
				}

				break;
			}
			case LEX_OUT:
			{
				if (lex.idtable.table[lex.lextable.table[i + 2].idxTI].id_data_type == IT::INT)
				{
					*(log.stream) << "\tpush " << lex.idtable.table[lex.lextable.table[i + 2].idxTI].id << "\n\t\tcall outd\n";
				}
				else if (lex.idtable.table[lex.lextable.table[i + 2].idxTI].id_type != IT::V)//���� �� ��� �������� �-�� ��� ����� � ��������� ��������� � ����
				{
					*(log.stream) << "\tpush offset " << lex.idtable.table[lex.lextable.table[i + 2].idxTI].id << "\n\t\tcall outc\n";
				}
				else
				{
					*(log.stream) << "\tpush " << lex.idtable.table[lex.lextable.table[i + 2].idxTI].id << "\n\t\tcall outc\n";
				}
				break;
			}
			}
		}
	}
}