#include "stdafx.h"
#include "IT.h"

namespace IT
{
	IdTable Create(int size)   //конструктор для табли идентифик
	{
		IdTable* p = new IdTable;
		p->maxsize = size;
		p->size = 0;
		p->table = new Entry[size];
		return (*p);
	}

	void Add(IdTable& idtable, Entry entry) //добавление в таблицу структуры Entry
	{
		idtable.table[idtable.size++] = entry;
	}

	Entry GetEntry(IdTable& idtable, int n)
	{
		return (idtable.table[n]);
	}

	int IsId(IdTable& idtable, string id)        //возвращает есть или нет этот элемент в табл идентифик по названию
	{
		for (int i = 0; i < idtable.size; i++)
		{
			if (idtable.table[i].id == id)
			{
				return i;
			}
		}
		return TI_NULLIDX;
	}

	int IsDublId(IdTable& idtable, string id)    //можно ливнуть наверное-------------------------------?
	{
		for (int i = 0; i < idtable.size - 1; i++)
		{
			if (idtable.table[i].id == id)
			{
				return i;
			}
		}
		return TI_NULLIDX;
	}

	int CheckId(IdTable& idtable, string value)    //проверяем по идентификатору
	{
		bool rc = false;
		int i;
		for (i = 0; i < idtable.size; i++)
		{
			if (idtable.table[i].value.vstr.str == value)
			{
				rc = true;
				break;
			}
		}
		return rc ? i : TI_NULLIDX;
	}

	int CheckId(IdTable& idtable, int value)
	{
		bool rc = false;
		int i;
		for (i = 0; i < idtable.size; i++)
		{
			if (idtable.table[i].value.vint == value)
			{
				if (idtable.table[i].id_type == L && idtable.table[i].id_data_type == INT)
				{
					rc = true;
					break;
				}
			}
		}
		return rc ? i : TI_NULLIDX;
	}

	void Delete(IdTable& idtable)
	{
		delete[] idtable.table;
	}
	void ShowTable(Log::LOG log, IdTable& idtable)
	{
		int i, numberOP = 0;
		*(log.stream) << setfill('-') << setw(87) << '-' << endl;
		*(log.stream) << "   №" << " | " << "Идентификатор" << " | " << "Тип данных" << " | " << "Тип идентификатора" << " | " << "Индекс в Тл" << " | " << "Значение   " << endl;
		*(log.stream) << setw(87) << '-' << endl;
		for (i = 0; i < idtable.size; i++)
		{
			*(log.stream) << setfill(' ') << setw(4) << std::right << i << " | ";
			*(log.stream) << setw(13) << left << idtable.table[i].id << " | ";
			switch (idtable.table[i].id_data_type)
			{
			case INT:
			{
				*(log.stream) << setw(10) << left;
				if (idtable.table[i].id_type == O)
				{
					*(log.stream) << "-" << " | ";
				}
				else
				{
					*(log.stream) << "int" << " | ";
				}
				break;
			}
			case CHAR:
			{
				*(log.stream) << setw(10) << left;
				if (idtable.table[i].id_type == O)
				{
					*(log.stream) << "-" << " | ";
				}
				else
				{
					*(log.stream) << "char" << " | ";
				}
				break;
			}
			default: *(log.stream) << setw(10) << left << "неизвестно" << " | "; break;
			}
			switch (idtable.table[i].id_type)
			{
			case V: *(log.stream) << setw(18) << left << "переменная" << " | "; break;
			case F: *(log.stream) << setw(18) << left << "функция" << " | "; break;
			case P: *(log.stream) << setw(18) << left << "параметр" << " | "; break;
			case L: *(log.stream) << setw(18) << left << "литерал" << " | "; break;
			case O: *(log.stream) << setw(18) << left << "оператор" << " | ";
				numberOP++;
				break;
			default: *(log.stream) << setw(18) << left << "неизвестно" << " | "; break;
			}
			*(log.stream) << setw(11) << left << idtable.table[i].id_first_LE << " | ";
			if (idtable.table[i].id_data_type == INT && (idtable.table[i].id_type == V || idtable.table[i].id_type == L))
			{
				*(log.stream) << setw(18) << left << idtable.table[i].value.vint;
			}
			else if (idtable.table[i].id_data_type == CHAR && (idtable.table[i].id_type == V || idtable.table[i].id_type == L))
			{
				*(log.stream) << "[" << idtable.table[i].value.vstr.len << "]\"" << idtable.table[i].value.vstr.str << "\"";
			}
			else
				*(log.stream) << "-";
			*(log.stream) << endl;
		}
		*(log.stream) << setfill('-') << setw(87) << '-' << endl;
		*(log.stream) << "******************Количество идентификаторов:" << i - numberOP << "******************" << endl;
		*(log.stream) << setw(87) << '-' << endl;
	}
}