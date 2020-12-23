#include "stdafx.h"
#include "In.h"
#include "Error.h"
//в ин передается инфа для лексич анализатора(лексемы)
namespace In
{
	IN GetIn(wchar_t infile[])
	{
		ifstream source_file(infile);         //открывает потом для счтывания из файла
		if (source_file.fail())
		{
			throw ERROR_THROW(110);
		}
		IN in;                                // возвращаемая структура
		Error::ERROR err;                     // ошибка для работы фунции записи ошибки в массив ошибок
		Error::ARRAY_OF_ERRORS errors;		  // массив для возможных ошибок
		in.size = in.ignor = 0;	  // инициализация структуры структуры In::IN
		in.lines = 1;
		unsigned char symb;		              //текущий символ входного файла(буферный)
		int size = 0,			              // текущий размер динамического массива
			col = 0;			              // номер текущей позиции в строке	
		char temp[IN_WORD_MAX_LEN];        	  // буферная строка для формирования слов-лексем
		short wordLen = 0;			          // текущая длина temp

		while ((!source_file.eof()) && (symb = source_file.get())) //... и есть символ кот счит-ся
		{
			if (symb == '\"')
			{
				temp[wordLen++] = symb;//записываем и переходим на след позицию
				do                     //пока не встретим вторую ковычку
				{
					symb = source_file.get();
					temp[wordLen++] = symb;
					in.size++; if (wordLen == IN_WORD_MAX_LEN)
					{
						throw ERROR_THROW_IN(101, in.lines, col);
					}
				} while (symb != '\"' && wordLen < IN_WORD_MAX_LEN);

				temp[wordLen] = '\0';               //зкрываем строку
				in.Lexema[size++] = { temp, in.lines }; //записыается  динам масс лексема в виде структуры типа sep
				wordLen = 0;
				in.size += 1;
				continue;
			}

			if (symb == IN_CODE_SPACE || in.code[(int)symb] == IN::S || symb == IN_CODE_ENDL)
			{
				if (temp && wordLen)//если строка не пустая и текуща длина не 0
				{
					temp[wordLen] = '\0';
					in.Lexema[size++] = { temp, in.lines }; //записыается  динам масс лексема в виде структуры типа sep
					wordLen = 0;
				}
				if (symb == IN_CODE_SPACE)				// проверка на пробелы
				{
					in.ignor += 1;
				}
				if (in.code[(int)symb] == IN::S)		// проверка на символ-сепаратор
				{                                       //дделаем из одиночного сепаратора строку
					char buf[2];
					buf[0] = symb;
					buf[1] = '\0';
					in.Lexema[size++] = { buf, in.lines }; //записыается  динам масс лексема в виде структуры типа sep
					wordLen = 0;
				}
				if (symb == IN_CODE_ENDL)				// проверка на символ перехода на новую строку
				{
					in.lines++;
					col = 0;
				}

			}
			else if (in.code[(int)symb] == IN::T)         //если не запрещён
			{
				col++;
				temp[wordLen++] = symb;
				in.size++;
			}
			else
			{
				err = ERROR_THROW_IN(111, in.lines, col);
				Error::AddError(&errors, err);        //добавляет ошибку в массив ошибок
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
			throw errors;     //выводим массив errors
		}
		source_file.close();
		in.array_size = size;
		return in;
	}
}