#include "stdafx.h"
#include "LexAnalize.h"

namespace Lex
{
	LT::LexTable lexTable = LT::Create(LT_MAXSIZE);//структуры для проверки входных параметров
	IT::IdTable idTable = IT::Create(TI_MAXSIZE);//
												 ///////////////////////////////СЕМАНТИКА///////////////////////
	struct Funcion
	{
		int indexTI;
		int size = 0;//кол-во параметров
		string name;
		IT::IDDATATYPE array_of_data_type[FUNC_MAX_SIZE];//массив типов параметров
	};
	struct Function_name //управляющая структура для Funcion
	{
		Funcion arr_of_func[FUNC_MAX_SIZE];//массив структур Funcion
		int size = -1;//текущий размер
	}Func_Han;
	/////////////////////////////////////////////////////
	FST::KA SearchFST(string src)
	{
		int size;
		FST::KA result;
		FST::FST* arr_of_fst = new FST::FST;
		if (src.size() == 1)
		{
			arr_of_fst = FST::arr_of_fst_op();
			size = ARR_OF_FST_OP_SIZE;
		}
		else
		{
			arr_of_fst = FST::arr_of_fst_word();
			size = ARR_OF_FST_WORD_SIZE;
		}
		for (int i = 0; i < size; i++)
		{
			arr_of_fst[i].str = src;
			if (execute(arr_of_fst[i]))
			{
				result.lexema = arr_of_fst[i].lexema;
				result.lex_type = arr_of_fst[i].lex_type;
				return result;
			}
		}
		return { FST::SYMB_NOT_FOUND, 0 };
	}

	LT::Entry CreateLex(char lex, int line, int id_in_TI, int prior)
	{
		LT::Entry result;
		result.priority = prior;
		result.lexema = lex;
		result.sn = line;
		result.idxTI = id_in_TI;
		return result;
	}

	IT::Entry CreateId(int line, string id, IT::IDDATATYPE id_data_type, IT::IDTYPE id_type, int vint)
	{
		IT::Entry result;
		result.id = id;
		result.id_first_LE = line;
		result.id_data_type = id_data_type;
		result.id_type = id_type;
		result.value.vint = vint;//для литералов
		return result;
	}
	IT::Entry CreateId(int line, string id, IT::IDDATATYPE id_data_type, IT::IDTYPE id_type)
	{
		IT::Entry result;
		result.id = id;
		result.id_first_LE = line;
		result.id_data_type = id_data_type;
		result.id_type = id_type;
		return result;
	}

	IT::Entry CreateId(int line, string id, IT::IDDATATYPE id_data_type, IT::IDTYPE id_type, string vstr)
	{
		IT::Entry result;
		result.id = id;
		result.id_first_LE = line;
		result.id_data_type = id_data_type;
		result.id_type = id_type;
		result.value.vstr.str = vstr;
		result.value.vstr.len = vstr.size();
		return result;
	}

	string MakeString(char symb, int num_of_lit)
	{
		string result = "1";
		result[0] = symb;
		return result + to_string(num_of_lit);
	}

	bool Check(IT::IDDATATYPE *arr_of_it_entry, string name)
	{
		for (int i = 0; i < Func_Han.size + 1; i++)
		{
			if (Func_Han.arr_of_func[i].name == name)
			{
				for (int j = 0; j < Func_Han.arr_of_func[i].size; j++)
				{
					if (Func_Han.arr_of_func[i].array_of_data_type[j] != arr_of_it_entry[j])
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	LEX Lexer(In::IN in, Log::LOG log)
	{
		bool flag_in_func = false,			// скобки для параметров функции
			flag_hesis_func = false,		// для определения параметров функции
			flag_id_func = false,			// для имени функции
			flag_into_func = false,			// внутри функции
			flag_main = false,				// в main
			func_var = true,				// можно ли объявлять переменные
			func_type = false,				// есть ли тип возвращаемого значения

			flag_return = false,			// оператор return
			flag_squar = false,				// вызов стандартной функции squar
			flag_strlen = false,			// вызов стандартной функции strlen
			flag_func = false,              // идентификатор функции
			flag_ariph = false,             //предыдущий символ арифметический знак
			flag_equal = false;			    // оператор '='

		string func_name = "",				// имя текущей функции (для области видимости)
			func_hesis_name;				// имя функции (для семантического анализа её параметров)
		Error::ERROR err;
		Error::ERROR err_to_arr;			// ошибка в массив	
		IT::IDDATATYPE arr_of_it_entry[FUNC_MAX_SIZE];//массив типов параметров вызываемой лок.ф-ии( в мейн)
		Error::ARRAY_OF_ERRORS errors;		// массив ошибок на данном этапе
		In::Sep lex_from_queue;				// слово-лексема, подлежащая разбору на данной итерации
		LT::Entry lex_constr;				// лексема, которая формируется в результате данной итерации
		FST::KA fst_st_mach;		        // конечный автомат для конкретного слова-лексемы
		IT::IDDATATYPE id_data_type;		// текущий тип идентификатора
		int size = in.array_size,			// количество лексем 
			size_arr_it_entry = 0,			    // текущая позиция в массиве типов данных параметров локальной функции
			num_of_lit = 0;					    // количество литералов
		IT::IDDATATYPE data_type;

		for (int i = 0; i < size; i++)
		{
			lex_from_queue = in.Lexema[i];
			fst_st_mach = SearchFST(lex_from_queue.word);//ищем КА для слова
			lex_constr = CreateLex(fst_st_mach.lexema, lex_from_queue.line, TL_TI_NULLIDX, LT_NOPRIORITY);
			switch (fst_st_mach.lex_type)
			{
			case FST::SYMB_NOT_FOUND:
			{
				err_to_arr = ERROR_THROW_IN(120, lex_constr.sn, -1);//Ошибка лексики: Цепочка символов не разобрана
				Error::AddError(&errors, err_to_arr);
				break;
			}
			case FST::SYMB_TABLE_ID:
			{
				int index = 0;
				if (flag_id_func)
				{
					flag_hesis_func = true;
					func_name = lex_from_queue.word;
					flag_id_func = false;
					if (IT::IsDublId(idTable, lex_from_queue.word) != TI_NULLIDX)		// семантика проверяем есть ли у нас ф-ия с таким именем
					{
						err_to_arr = ERROR_THROW_IN(700, lex_constr.sn, -1);//Ошибка семантики: Повторное объявление идентификатора
						Error::AddError(&errors, err_to_arr);
					}
					else
					{
						idTable.table[idTable.size] = CreateId(lexTable.size, lex_from_queue.word, data_type, IT::F);//записываем в табл идентиф текущий размер,слово,тип значения и показываем что это идентиф. ф-ии
						lex_constr.idxTI = idTable.size++;
						Func_Han.size++;//показываем,что добавилась локальная ф-ия
					}
					break;
				}
				if (flag_hesis_func)//если находимся внутри () ф-ии
				{
					if ((index = IT::IsId(idTable, func_name + lex_from_queue.word)) == TI_NULLIDX && lexTable.table[lexTable.size - 1].lexema == LEX_T)//если такого ещё нет в табл идентификаторов
					{
						idTable.table[idTable.size] = CreateId(lexTable.size, func_name + lex_from_queue.word, data_type, IT::P);
						lex_constr.idxTI = idTable.size++;
					}
					else
					{
						if (flag_main)
						{
							arr_of_it_entry[size_arr_it_entry++] = idTable.table[index].id_data_type;//записываем в массив типов параметров ф-ии тип текущей лексемы
						}
						lex_constr.idxTI = index;
					}
					if (lex_constr.idxTI == TL_TI_NULLIDX)
					{
						err_to_arr = ERROR_THROW_IN(706, lex_from_queue.line, -1); //Необъявленный идентификатор
						Error::AddError(&errors, err_to_arr);
					}
				}
				else if (flag_into_func || flag_main) //если находмся внутри {}
				{
					if ((index = IT::IsId(idTable, func_name + lex_from_queue.word)) == TI_NULLIDX)
					{
						if ((index = IT::IsId(idTable, lex_from_queue.word)) != TI_NULLIDX) // если локальная функция
						{
							lex_constr.idxTI = index;//присваем имя 
							if (idTable.table[index].id_type == IT::F)//если это действительно ф-ия
							{
								func_hesis_name = idTable.table[index].id;//записываем её имя в буфер
								flag_hesis_func = true; //скобки для параметров ф-ии
							}
						}
						if (func_var)
						{
							if (data_type == IT::CHAR)
							{
								idTable.table[idTable.size] = CreateId(lexTable.size, func_name + lex_from_queue.word, data_type, IT::V, "");//инициализация ""
							}
							else
							{
								idTable.table[idTable.size] = CreateId(lexTable.size, func_name + lex_from_queue.word, data_type, IT::V, 0);//инициализация 0
							}
							lex_constr.idxTI = idTable.size++;
						}
						/////////////////////////////////////////СЕМАНТИКА////////////////////////////////////
						else if (index == TI_NULLIDX) //если мы задаём не имя лок.ф-ии или необъявл-ый идентификатор
						{
							err_to_arr = ERROR_THROW_IN(706, lex_from_queue.line, -1); //Необъявленный идентификатор
							Error::AddError(&errors, err_to_arr);
						}
					}
					else //если есть индекс вхождения в табл идентификаторов
					{
						lex_constr.idxTI = index;
						if (flag_equal)
						{
							if (idTable.table[lex_constr.idxTI].id_data_type != id_data_type && idTable.table[lex_constr.idxTI].id_type != IT::P && !flag_strlen)
							{
								err_to_arr = ERROR_THROW_IN(707, lex_from_queue.line, -1); //Несоответствие типов в операторе присваивания
								Error::AddError(&errors, err_to_arr);
							}
						}
					}
				}
				flag_ariph = false;

				if (flag_return || flag_squar)
				{
					if (idTable.table[lex_constr.idxTI].id_data_type != IT::INT)
					{
						err_to_arr = ERROR_THROW_IN(701, lex_constr.sn, -1); //Ошибка в типе идентификатора
						Error::AddError(&errors, err_to_arr);
					}

					if (flag_squar)
					{
						if (idTable.table[lex_constr.idxTI].value.vint < 0) //проверяем на неотрицат знач
						{
							err_to_arr = ERROR_THROW_IN(701, lex_constr.sn, -1); //Ошибка в типе идентификатора
							Error::AddError(&errors, err_to_arr);
						}
					}
					flag_return = flag_squar = false;
				}
				if (flag_strlen)
				{
					if (idTable.table[lex_constr.idxTI].id_data_type != IT::CHAR)
					{
						err_to_arr = ERROR_THROW_IN(705, lex_constr.sn, -1); //Ошибка в параметре вызываемой функции strln стандартной библиотеки
						Error::AddError(&errors, err_to_arr);
					}
					flag_strlen = false;
				}
				break;
			}
			///////////////////////////////////////
			case FST::SYMB_LITERAL:
			{
				int position;
				switch (fst_st_mach.lexema)
				{
				case LEX_INT:
				{
					int index = IT::CheckId(idTable, atoi(lex_from_queue.word.c_str()));//есть ли именно такое число в табл идентифик
					if (index == TI_NULLIDX) //если нет то заиссываем
					{
						idTable.table[position = idTable.size] = CreateId(lexTable.size, MakeString(LEX_L, num_of_lit++), IT::INT, IT::L, atoi(lex_from_queue.word.c_str()));
						lex_constr.idxTI = idTable.size++;
					}
					else
					{
						position = lex_constr.idxTI = index;//записываем в перем position индекс табл идеднтифик
					}
					break;
				}
				case LEX_CHAR:
				{
					int index = IT::CheckId(idTable, lex_from_queue.word);
					if (index == TI_NULLIDX)
					{
						idTable.table[position = idTable.size] = CreateId(lexTable.size, MakeString(LEX_L, num_of_lit++), IT::CHAR, IT::L, lex_from_queue.word);
						lex_constr.idxTI = idTable.size++;
					}
					else
					{
						position = lex_constr.idxTI = index;
					}
					break;
				}
				}
				if (flag_equal && !flag_hesis_func)			///////////////////СЕМАНТИКА
				{
					if (idTable.table[position].id_data_type != id_data_type)
					{
						err_to_arr = ERROR_THROW_IN(707, lex_from_queue.line, -1); //Несоответствие типов в операторе присваивания
						Error::AddError(&errors, err_to_arr);
					}
				}
				break;
			}
			case FST::SYMB_NEED_PRIORITY:
			{
				switch (lex_from_queue.word[0])
				{
				case LEX_COMMA:
				{
					lex_constr.priority = 1;
					break;
				}
				case LEX_LEFTHESIS:
				{
					if (flag_hesis_func)
					{
						lex_constr.priority = 4; ////////////////////////////СРАВНИТЬ ПРИОРИТЕТЫ
						flag_in_func = true;
					}
					else
					{
						lex_constr.priority = 0;
					}
					break;
				}
				case LEX_RIGHTHESIS:
				{
					if (flag_hesis_func && flag_in_func)
					{
						if (flag_main)
						{
							if (Check(arr_of_it_entry, func_hesis_name))//проверка на параметры,передаваемые в лок.ф-ию
							{
								err_to_arr = ERROR_THROW_IN(702, lex_constr.sn, -1);//Ошибка в передаваемых значениях в функцию
								Error::AddError(&errors, err_to_arr);
							}
							func_hesis_name = "";
							size_arr_it_entry = 0;//обнуляем запись в массив  arr_of_it_entry
						}
						lex_constr.priority = 4;
						flag_in_func = flag_hesis_func = flag_squar = false;//закрываем флаги,указывающие,что это были скобки на ф-ии
					}
					else
					{
						lex_constr.priority = 0;
					}
					break;
				}
				}
				break;
			}
			case FST::SYMB_LEXEMS:
			{
				switch (fst_st_mach.lexema)
				{
				case LEX_MAIN:
				{
					flag_main = true;
					func_name = lex_from_queue.word;
					break;
				}
				case LEX_FUNCTION:
				{
					flag_id_func = true;
					break;
				}
				case LEX_CHAR:
				{
					data_type = IT::CHAR;
					func_var = true;//показываем что это объъявлеие идентифик
					lex_constr.lexema = LEX_T;//показываем что идёт лексема типа данных
					break;
				}
				case LEX_INT:
				{
					data_type = IT::INT;
					flag_ariph = false;
					func_var = true;
					lex_constr.lexema = LEX_T;
					break;
				}
				case LEX_RET:
				{
					flag_return = true;
					break;
				}
				case LEX_EQUAL:
				{
					flag_equal = true;
					id_data_type = idTable.table[lexTable.table[lexTable.size - 1].idxTI].id_data_type;//записываем текущий тип данных в выражении
					break;
				}
				case LEX_SEMICOLON:
				{
					if (flag_hesis_func)
					{
						err_to_arr = ERROR_THROW_IN(602, lex_constr.sn, -1); //Ошибка синтаксиса: Неверное выражение
						Error::AddError(&errors, err_to_arr);
					}
					func_var = flag_equal = flag_return = flag_strlen = flag_squar = false;
					break;
				}
				case LEX_LEFTBRACE:
				{
					flag_into_func = true;
					break;
				}
				case LEX_RIGHTBRACE:
				{
					flag_into_func = false;
					break;
				}
				}
				break;
			}
			case FST::SYMB_ARIPH:
			{
				switch (lex_from_queue.word[0])
				{
				case LEX_PLUS:
				{
					lex_constr.priority = 2;
					break;
				}
				case LEX_MINUS:
				{
					lex_constr.priority = 2;
					break;
				}
				case LEX_STAR:
				{
					lex_constr.priority = 3;
					break;
				}
				}///////////////////////СЕМАНТИКА
				if (flag_ariph)
				{
					err_to_arr = ERROR_THROW_IN(124, lex_constr.sn, -1);
					Error::AddError(&errors, err_to_arr);
				}
				flag_ariph = true;

				lex_constr.idxTI = idTable.size;
				idTable.table[idTable.size++] = CreateId(lexTable.size, lex_from_queue.word, IT::OPERATOR, IT::O);
				break;
			}
			case FST::SYMB_STATIC_LIB:
			{
				int index;
				if ((index = IT::IsId(idTable, lex_from_queue.word)) == TI_NULLIDX)///////////////////
				{
					idTable.table[idTable.size] = CreateId(lexTable.size, lex_from_queue.word, IT::INT, IT::F);
					lex_constr.idxTI = idTable.size++;
				}
				else
				{
					lex_constr.idxTI = index;
				}
				if (lex_constr.lexema == LEX_POWR)		// для семантики чтобы потом показывать какие типы данных передаются в ф-ию
				{
					flag_squar = true;
				}
				if (lex_constr.lexema == LEX_STRLEN)		// для семантики
				{
					flag_strlen = true;
				}
				flag_hesis_func = true;
				break;
			}
			}
			lexTable.table[lexTable.size++] = lex_constr;//добавляем уже сформированную лексему в табл. лексем
			if (lexTable.size == LT_MAXSIZE)
			{
				throw ERROR_THROW(121);
			}
			if (idTable.size == TI_MAXSIZE)
			{
				throw ERROR_THROW(122);
			}
		}
		if (flag_main == false)
		{
			throw ERROR_THROW(124);
		}
		if (errors.size)
		{
			throw errors;
		}
		return { lexTable, idTable };//возвра. табл лексем и идентифик. для обработки на след. этапах
	}
}