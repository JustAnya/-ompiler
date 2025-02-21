#include "stdafx.h"
#include "LexAnalize.h"
#include "MFST.h"
#include "GRB.h"
#include "PolNot.h"
#include "Generation.h"

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(0, "");
	Log::LOG log = Log::INITLOG;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::GetLog(parm.log);
		Log::WriteLog(log);
		Log::WriteParm(log, parm);

		//проверка файла с исходным кодом на допустимость символов
		In::IN in = In::GetIn(parm.in);
		Log::WriteIn(log, in);

		log = Log::GetLog(parm.lex);
		Log::WriteLine(log, "Лексический и семантический анализ ", "");
		Lex::LEX lex = Lex::Lexer(in, log);
		Log::WriteLine(log, "выполнены без ошибок", "");
		LT::ShowTable(log, lex.lextable);
		IT::ShowTable(log, lex.idtable);

		log = Log::GetLog(parm.sin);
		Log::WriteLine(log, "Синтаксический анализ\n", "");

		MFST_TRACE_START(log)
			MFST::Mfst mfst(lex, GRB::getGreibach());
		mfst.start(log);
		mfst.savededucation();
		mfst.printrules(log);
		Log::WriteLine(log, "\nПольская запись выполнена без ошибок", "");
		CallPolishNot(&lex.lextable, &lex.idtable);
		LT::ShowTable(log, lex.lextable);
		IT::ShowTable(log, lex.idtable);
		log = Log::GetLog(parm.out);
		Generation::Generation(lex, log);
		cout << "Выполнено без ошибок.\n";
		Log::Close(log);
	}
	catch (Error::ERROR e)
	{
		wcout << L"Ошибка! Файл \n" << log.logfile << endl;
		Log::WriteError(log, e);
	}
	catch (Error::ARRAY_OF_ERRORS e)
	{
		wcout << L"Ошибка! Файл \n" << log.logfile << endl;
		for (int i = 0; i < e.size; i++)
		{
			Log::WriteError(log, e.arr_of_err[i]);
		}
	}

	catch (...)
	{
		cout << "Ошибка: сбой системы" << endl;
	}
	system("notepad D:\\Курсовой мой\\MAS\\MAS\\in.txt.log");
	system("notepad D:\\Курсовой мой\\MAS\\MAS\\in.txt.lx");
	system("notepad D:\\Курсовой мой\\MAS\\MAS\\in.txt.sin");
	return 0;
};