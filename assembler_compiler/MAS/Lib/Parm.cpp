#include "stdafx.h"
#include "Error.h"
#include "Parm.h"
//анализ параметров которые мы передаём черз консоль при вызове приложуньки
namespace Parm
{
	//argv[0]-название самого приложение(непосрественно то что мы передаём)
	//argc-кол-во параметров которое передаём в вызов приложения через консоль
	PARM getparm(int argc, wchar_t* argv[])
	{
		bool fl = false, fl_out = false, fl_log = false, fl_lex = false, fl_sin = false; //флаги на наличие этих параметров
		PARM p;              //структура с возвр значением
		wchar_t *buf;          //переменная для записи какихто параметров
		for (int i = 1; i < argc; i++)
		{
			if (wcslen((wchar_t*)argv[i]) >= PARM_MAX_SIZE)
			{
				throw ERROR_THROW(104);
			}
			if (buf = wcsstr((wchar_t*)argv[i], PARM_IN))//возвр строку после вхождения этой подстроки в строку
			{
				wcscpy_s(p.in, buf + wcslen(PARM_IN));
				fl = true;
			}
			if (buf = wcsstr((wchar_t*)argv[i], PARM_OUT))
			{
				wcscpy_s(p.out, buf + wcslen(PARM_OUT));
				fl_out = true;
			}
			if (buf = wcsstr((wchar_t*)argv[i], PARM_LOG))
			{
				wcscpy_s(p.log, buf + wcslen(PARM_LOG));
				fl_log = true;
			}
			if (buf = wcsstr((wchar_t*)argv[i], PARM_LEX))
			{
				wcscpy_s(p.log, buf + wcslen(PARM_LEX));
				fl_lex = true;
			}
			if (buf = wcsstr((wchar_t*)argv[i], PARM_SIN))
			{
				wcscpy_s(p.in, buf + wcslen(PARM_SIN));
				fl_sin = true;
			}
		}
		if (!fl)
		{
			throw ERROR_THROW(100);
		}
		if (!fl_out)
		{
			wcscpy_s(p.out, p.in);
			wcsncat_s(p.out, PARM_OUT_DEFAULT_EXT, wcslen(PARM_OUT_DEFAULT_EXT));
		}
		if (!fl_log)
		{
			wcscpy_s(p.log, p.in);
			wcsncat_s(p.log, PARM_LOG_DEFAULT_EXT, wcslen(PARM_LOG_DEFAULT_EXT));
		}
		if (!fl_lex)
		{
			wcscpy_s(p.lex, p.in);
			wcsncat_s(p.lex, PARM_LEX_DEFAULT_EXT, wcslen(PARM_LEX_DEFAULT_EXT));
		}
		if (!fl_sin)
		{
			wcscpy_s(p.sin, p.in);
			wcsncat_s(p.sin, PARM_SIN_DEFAULT_EXT, wcslen(PARM_SIN_DEFAULT_EXT));
		}
		return p;
	}
};