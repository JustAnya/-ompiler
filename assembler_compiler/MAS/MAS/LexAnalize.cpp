#include "stdafx.h"
#include "LexAnalize.h"

namespace Lex
{
	LT::LexTable lexTable = LT::Create(LT_MAXSIZE);//��������� ��� �������� ������� ����������
	IT::IdTable idTable = IT::Create(TI_MAXSIZE);//
												 ///////////////////////////////���������///////////////////////
	struct Funcion
	{
		int indexTI;
		int size = 0;//���-�� ����������
		string name;
		IT::IDDATATYPE array_of_data_type[FUNC_MAX_SIZE];//������ ����� ����������
	};
	struct Function_name //����������� ��������� ��� Funcion
	{
		Funcion arr_of_func[FUNC_MAX_SIZE];//������ �������� Funcion
		int size = -1;//������� ������
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
		result.value.vint = vint;//��� ���������
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
		bool flag_in_func = false,			// ������ ��� ���������� �������
			flag_hesis_func = false,		// ��� ����������� ���������� �������
			flag_id_func = false,			// ��� ����� �������
			flag_into_func = false,			// ������ �������
			flag_main = false,				// � main
			func_var = true,				// ����� �� ��������� ����������
			func_type = false,				// ���� �� ��� ������������� ��������

			flag_return = false,			// �������� return
			flag_squar = false,				// ����� ����������� ������� squar
			flag_strlen = false,			// ����� ����������� ������� strlen
			flag_func = false,              // ������������� �������
			flag_ariph = false,             //���������� ������ �������������� ����
			flag_equal = false;			    // �������� '='

		string func_name = "",				// ��� ������� ������� (��� ������� ���������)
			func_hesis_name;				// ��� ������� (��� �������������� ������� � ����������)
		Error::ERROR err;
		Error::ERROR err_to_arr;			// ������ � ������	
		IT::IDDATATYPE arr_of_it_entry[FUNC_MAX_SIZE];//������ ����� ���������� ���������� ���.�-��( � ����)
		Error::ARRAY_OF_ERRORS errors;		// ������ ������ �� ������ �����
		In::Sep lex_from_queue;				// �����-�������, ���������� ������� �� ������ ��������
		LT::Entry lex_constr;				// �������, ������� ����������� � ���������� ������ ��������
		FST::KA fst_st_mach;		        // �������� ������� ��� ����������� �����-�������
		IT::IDDATATYPE id_data_type;		// ������� ��� ��������������
		int size = in.array_size,			// ���������� ������ 
			size_arr_it_entry = 0,			    // ������� ������� � ������� ����� ������ ���������� ��������� �������
			num_of_lit = 0;					    // ���������� ���������
		IT::IDDATATYPE data_type;

		for (int i = 0; i < size; i++)
		{
			lex_from_queue = in.Lexema[i];
			fst_st_mach = SearchFST(lex_from_queue.word);//���� �� ��� �����
			lex_constr = CreateLex(fst_st_mach.lexema, lex_from_queue.line, TL_TI_NULLIDX, LT_NOPRIORITY);
			switch (fst_st_mach.lex_type)
			{
			case FST::SYMB_NOT_FOUND:
			{
				err_to_arr = ERROR_THROW_IN(120, lex_constr.sn, -1);//������ �������: ������� �������� �� ���������
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
					if (IT::IsDublId(idTable, lex_from_queue.word) != TI_NULLIDX)		// ��������� ��������� ���� �� � ��� �-�� � ����� ������
					{
						err_to_arr = ERROR_THROW_IN(700, lex_constr.sn, -1);//������ ���������: ��������� ���������� ��������������
						Error::AddError(&errors, err_to_arr);
					}
					else
					{
						idTable.table[idTable.size] = CreateId(lexTable.size, lex_from_queue.word, data_type, IT::F);//���������� � ���� ������� ������� ������,�����,��� �������� � ���������� ��� ��� �������. �-��
						lex_constr.idxTI = idTable.size++;
						Func_Han.size++;//����������,��� ���������� ��������� �-��
					}
					break;
				}
				if (flag_hesis_func)//���� ��������� ������ () �-��
				{
					if ((index = IT::IsId(idTable, func_name + lex_from_queue.word)) == TI_NULLIDX && lexTable.table[lexTable.size - 1].lexema == LEX_T)//���� ������ ��� ��� � ���� ���������������
					{
						idTable.table[idTable.size] = CreateId(lexTable.size, func_name + lex_from_queue.word, data_type, IT::P);
						lex_constr.idxTI = idTable.size++;
					}
					else
					{
						if (flag_main)
						{
							arr_of_it_entry[size_arr_it_entry++] = idTable.table[index].id_data_type;//���������� � ������ ����� ���������� �-�� ��� ������� �������
						}
						lex_constr.idxTI = index;
					}
					if (lex_constr.idxTI == TL_TI_NULLIDX)
					{
						err_to_arr = ERROR_THROW_IN(706, lex_from_queue.line, -1); //������������� �������������
						Error::AddError(&errors, err_to_arr);
					}
				}
				else if (flag_into_func || flag_main) //���� �������� ������ {}
				{
					if ((index = IT::IsId(idTable, func_name + lex_from_queue.word)) == TI_NULLIDX)
					{
						if ((index = IT::IsId(idTable, lex_from_queue.word)) != TI_NULLIDX) // ���� ��������� �������
						{
							lex_constr.idxTI = index;//�������� ��� 
							if (idTable.table[index].id_type == IT::F)//���� ��� ������������� �-��
							{
								func_hesis_name = idTable.table[index].id;//���������� � ��� � �����
								flag_hesis_func = true; //������ ��� ���������� �-��
							}
						}
						if (func_var)
						{
							if (data_type == IT::CHAR)
							{
								idTable.table[idTable.size] = CreateId(lexTable.size, func_name + lex_from_queue.word, data_type, IT::V, "");//������������� ""
							}
							else
							{
								idTable.table[idTable.size] = CreateId(lexTable.size, func_name + lex_from_queue.word, data_type, IT::V, 0);//������������� 0
							}
							lex_constr.idxTI = idTable.size++;
						}
						/////////////////////////////////////////���������////////////////////////////////////
						else if (index == TI_NULLIDX) //���� �� ����� �� ��� ���.�-�� ��� ��������-�� �������������
						{
							err_to_arr = ERROR_THROW_IN(706, lex_from_queue.line, -1); //������������� �������������
							Error::AddError(&errors, err_to_arr);
						}
					}
					else //���� ���� ������ ��������� � ���� ���������������
					{
						lex_constr.idxTI = index;
						if (flag_equal)
						{
							if (idTable.table[lex_constr.idxTI].id_data_type != id_data_type && idTable.table[lex_constr.idxTI].id_type != IT::P && !flag_strlen)
							{
								err_to_arr = ERROR_THROW_IN(707, lex_from_queue.line, -1); //�������������� ����� � ��������� ������������
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
						err_to_arr = ERROR_THROW_IN(701, lex_constr.sn, -1); //������ � ���� ��������������
						Error::AddError(&errors, err_to_arr);
					}

					if (flag_squar)
					{
						if (idTable.table[lex_constr.idxTI].value.vint < 0) //��������� �� ��������� ����
						{
							err_to_arr = ERROR_THROW_IN(701, lex_constr.sn, -1); //������ � ���� ��������������
							Error::AddError(&errors, err_to_arr);
						}
					}
					flag_return = flag_squar = false;
				}
				if (flag_strlen)
				{
					if (idTable.table[lex_constr.idxTI].id_data_type != IT::CHAR)
					{
						err_to_arr = ERROR_THROW_IN(705, lex_constr.sn, -1); //������ � ��������� ���������� ������� strln ����������� ����������
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
					int index = IT::CheckId(idTable, atoi(lex_from_queue.word.c_str()));//���� �� ������ ����� ����� � ���� ���������
					if (index == TI_NULLIDX) //���� ��� �� ����������
					{
						idTable.table[position = idTable.size] = CreateId(lexTable.size, MakeString(LEX_L, num_of_lit++), IT::INT, IT::L, atoi(lex_from_queue.word.c_str()));
						lex_constr.idxTI = idTable.size++;
					}
					else
					{
						position = lex_constr.idxTI = index;//���������� � ����� position ������ ���� ����������
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
				if (flag_equal && !flag_hesis_func)			///////////////////���������
				{
					if (idTable.table[position].id_data_type != id_data_type)
					{
						err_to_arr = ERROR_THROW_IN(707, lex_from_queue.line, -1); //�������������� ����� � ��������� ������������
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
						lex_constr.priority = 4; ////////////////////////////�������� ����������
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
							if (Check(arr_of_it_entry, func_hesis_name))//�������� �� ���������,������������ � ���.�-��
							{
								err_to_arr = ERROR_THROW_IN(702, lex_constr.sn, -1);//������ � ������������ ��������� � �������
								Error::AddError(&errors, err_to_arr);
							}
							func_hesis_name = "";
							size_arr_it_entry = 0;//�������� ������ � ������  arr_of_it_entry
						}
						lex_constr.priority = 4;
						flag_in_func = flag_hesis_func = flag_squar = false;//��������� �����,�����������,��� ��� ���� ������ �� �-��
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
					func_var = true;//���������� ��� ��� ���������� ���������
					lex_constr.lexema = LEX_T;//���������� ��� ��� ������� ���� ������
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
					id_data_type = idTable.table[lexTable.table[lexTable.size - 1].idxTI].id_data_type;//���������� ������� ��� ������ � ���������
					break;
				}
				case LEX_SEMICOLON:
				{
					if (flag_hesis_func)
					{
						err_to_arr = ERROR_THROW_IN(602, lex_constr.sn, -1); //������ ����������: �������� ���������
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
				}///////////////////////���������
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
				if (lex_constr.lexema == LEX_POWR)		// ��� ��������� ����� ����� ���������� ����� ���� ������ ���������� � �-��
				{
					flag_squar = true;
				}
				if (lex_constr.lexema == LEX_STRLEN)		// ��� ���������
				{
					flag_strlen = true;
				}
				flag_hesis_func = true;
				break;
			}
			}
			lexTable.table[lexTable.size++] = lex_constr;//��������� ��� �������������� ������� � ����. ������
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
		return { lexTable, idTable };//������. ���� ������ � ���������. ��� ��������� �� ����. ������
	}
}