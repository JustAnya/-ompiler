#include "stdafx.h"
#include "Generation.h"
#include <stack>
//генерируем шапку, егмент констант все наши литераоы, в сегмент переменных и все объявленные переменные, а дальше идёт неполедственно код программы
//и смотрим по лексеме что нам нужно делать на ассемблере
namespace Generation
{
	void Generation(Lex::LEX lex, Log::LOG log)//передаём структуру лекс в которой табл лексем и иентиф и стр-ру лог в кот открыт входной поток для записи в фойл 
	{
		*(log.stream) << ".586\n\t.model flat, stdcall\n\tincludelib libucrt.lib\n\tincludelib kernel32.lib\n\t	includelib ../Debug/MASAsmLibrary.lib\n\tExitProcess PROTO :DWORD\n\n";
		*(log.stream) << "\toutc PROTO: DWORD\n\toutd PROTO: DWORD\n\tstrln PROTO: DWORD\n\tpowr PROTO: DWORD, :DWORD\n";
		*(log.stream) << "\n.stack 4096\n";

		*(log.stream) << ".const\n";
		for (int i = 0; i < lex.idtable.size; i++)//проходим по всей табл идент и ищем литералы
		{
			if (lex.idtable.table[i].id_type == IT::L)
			{
				*(log.stream) << "\t" << lex.idtable.table[i].id;
				if (lex.idtable.table[i].id_data_type == IT::CHAR)//смотрим по типу данных
				{
					*(log.stream) << " BYTE " << lex.idtable.table[i].value.vstr.str << ", 0\n";//если символьный(саамо значение)
				}
				else
				{
					*(log.stream) << " DWORD " << lex.idtable.table[i].value.vint << endl;//если целочесленный
				}
			}
		}

		*(log.stream) << ".data\n";
		for (int i = 0; i < lex.lextable.size; i++)//по таблице лексем
		{
			if (lex.lextable.table[i].lexema == LEX_VAR)
			{
				*(log.stream) << "\t" << lex.idtable.table[lex.lextable.table[i + 2].idxTI].id;//выводим ид идентиф-ра,показываем что у него тип 4байт беззнак
				*(log.stream) << " DWORD ?\n";
				i += 3;
			}
		}

		stack<string> stk;                      // стек для правильной последовательности передачи параметров в функцию ассемблера
		string func_name = "";					// имя локальной функции
		bool flag_ret = false,
			flag_funcion = false,				// внутри локальной функции
			flag_main = false;					// внутри главной функции
		int num_of_ret = 0;

		*(log.stream) << "\n.code\n\n";
		for (int i = 0; i < lex.lextable.size; i++)
		{
			switch (lex.lextable.table[i].lexema)//смотрим какую лесему встр
			{
			case LEX_FUNCTION://если ф-ия
			{
				*(log.stream) << (func_name = lex.idtable.table[lex.lextable.table[++i].idxTI].id) << " PROC ";//выводим имя ф-ии и  запис в func_name
				while (lex.lextable.table[i].lexema != LEX_RIGHTHESIS)
				{
					if (lex.idtable.table[lex.lextable.table[i].idxTI].id_type == IT::P)//если тип параметр
					{
						*(log.stream) << lex.idtable.table[lex.lextable.table[i].idxTI].id << " : ";//выводим его ид в файл
						if (lex.idtable.table[lex.lextable.table[i].idxTI].id_data_type == IT::INT)//смотрим по его типу данных
						{
							*(log.stream) << "SDWORD";//показываем если инт мб знаковый 4байтовый
						}
						else
						{
							*(log.stream) << "DWORD";//или строка перед. через указатель, дворд -стандр тип данных ля хран указателей
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
				int result_position = i - 1;//позиция идентиф. результата
				while (lex.lextable.table[i].lexema != LEX_SEMICOLON)
				{
					switch (lex.lextable.table[i].lexema)
					{
					case LEX_ID:
					case LEX_INT:
					{
						*(log.stream) << "\tpush " << lex.idtable.table[lex.lextable.table[i].idxTI].id << endl;//добавляем в стек в ассембл
						stk.push(lex.idtable.table[lex.lextable.table[i].idxTI].id);//в промежуточный наш стек
						break;
					}
					case LEX_CHAR:
					{
						*(log.stream) << "\tpush offset " << lex.idtable.table[lex.lextable.table[i].idxTI].id << endl;//записывам указатель(offset)
						stk.push("offset " + lex.idtable.table[lex.lextable.table[i].idxTI].id);
						break;
					}
					case LEX_SUBST://@-идентификатор ф-ии после польской записи
					{
						for (int j = 1; j <= lex.lextable.table[i].priority; j++)//в значении приоритета записано кол-во параметров
						{
							*(log.stream) << "\tpop edx\n";//"освобождаем" стек в ассембл
						}
						for (int j = 1; j <= lex.lextable.table[i].priority; j++)//stdcall
						{
							*(log.stream) << "\tpush " << stk.top() << endl;//записываем в обратном порядке
							stk.pop();//локальнй стек,от имитирует правильность последоательности параметров
						}
						*(log.stream) << "\t\tcall " << lex.idtable.table[lex.lextable.table[i].idxTI].id << "\n\tpush eax\n";//при пом call вызываем ф-ию и помещаем её результат из eax в стандартный стек ассемблера
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
				*(log.stream) << "\tpop " << lex.idtable.table[lex.lextable.table[result_position].idxTI].id << "\n";//когда встречаем ; из стека ассемблера достаём значение этого выражения и присваиваем его переменной в нач. строки
				break;
			}
			case LEX_RET:
			{
				*(log.stream) << "\tpush ";
				i++;
				if (lex.idtable.table[lex.lextable.table[i].idxTI].id_type == IT::L)//если литерал 
				{
					*(log.stream) << lex.idtable.table[lex.lextable.table[i++].idxTI].value.vint;//записываем его знач в стандратный стек
				}
				else
				{
					*(log.stream) << lex.idtable.table[lex.lextable.table[i++].idxTI].id;//зап-м знач ид в стандр. стек
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
				else if (lex.idtable.table[lex.lextable.table[i + 2].idxTI].id_type != IT::V)//если не инт вызываем ф-ию для строк и добавляем указатель в стек
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