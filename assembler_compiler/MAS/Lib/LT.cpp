#include "stdafx.h"
#include "LT.h"
namespace LT
{
	LexTable Create(int size)                               //конструктор таблицы лексем
	{
		LexTable *newTabl = new LexTable;					//создание экзмепляра таблицы лексем
		newTabl->maxize = size;								//задание максимальнго размера
		newTabl->size = 0;									//начальный размер равен нулю
		newTabl->table = new Entry[size];					//создание массива строк таблицы лексем
		memset(newTabl->table, 255, 255);					//заполнение экземпляров таблицы строк на 255
		return *newTabl;
	}

	Entry Add(LexTable& lextable, Entry entry)
	{
		return(lextable.table[lextable.size++] = entry);	//добавление на позицию текущего размера таблицы лексем элемента таблицы строк лексем
	}

	Entry GetEntry(LexTable& lextable, int n)             //возврат n-го элемента таблицы лексем
	{
		return (lextable.table[n]);
	}

	void Delete(LexTable& lextable)
	{
		delete[] lextable.table;
	}
	void ShowTable(Log::LOG log, LexTable& lexTable)  //вывод в лог таблицы лексем
	{
		int k = 1;
		*(log.stream) << "\n\n1: ";
		for (int i = 0; i < lexTable.size; i++)
		{
			if (lexTable.table[i].sn != k) //проверка на совпадение номера строки из исходного кода с номером строки в лексеме
			{
				*(log.stream) << "\n" << lexTable.table[i].sn << ": ";
				k = lexTable.table[i].sn;
				if (lexTable.table[i].idxTI != (int)TL_TI_NULLIDX)
				{
					*(log.stream) << lexTable.table[i].lexema /*<< "[" << lexTable.table[i].idxTI << "]"*/; //если есть ссылка на табл идентификаторов
				}
				else
				{
					*(log.stream) << lexTable.table[i].lexema;
				}
			}
			else
			{
				if (lexTable.table[i].idxTI != (int)TL_TI_NULLIDX)
				{
					*(log.stream) << lexTable.table[i].lexema /*<< "[" << lexTable.table[i].idxTI << "]"*/;//---------------------?
				}
				else
				{
					*(log.stream) << lexTable.table[i].lexema;
				}
			}
		}
		*(log.stream) << "\nКоличество лексем - " << lexTable.size << endl;
	}
}