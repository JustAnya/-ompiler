#include "stdafx.h"
#include "GRB.h"
#include "MFST.h"
#include "LexAnalize.h"

int FST_TRACE_n = -1;
Error::ARRAY_OF_ERRORS errors;
char rbuf[205], sbuf[205], lbuf[1024];  //---------------------------------
#define NS(n)	GRB::Rule::Chain::N(n)
#define TS(n)	GRB::Rule::Chain::T(n)
#define ISNS(n)	GRB::Rule::Chain::isN(n)
#define MFST_TRACE1(log)	*(log.stream)<<setw(4)<<left<<++FST_TRACE_n << ": "\
						<<setw(20)<<left<<rule.getCRule(rbuf, nrulechain)\
						<<setw(30)<<left<<getCLenta(lbuf, lenta_position)\
						<<setw(20)<<left<<getCSt(sbuf)<<endl;
#define MFST_TRACE2(log)	*(log.stream)<<setw(4)<<left<< FST_TRACE_n << ": "\
						<<setw(20)<<left<<" "\
						<<setw(30)<<left<<getCLenta(lbuf, lenta_position)\
						<<setw(20)<<left<<getCSt(sbuf)<<endl;
#define MFST_TRACE3(log)	*(log.stream)<<setw(4)<<left<< ++FST_TRACE_n << ": "\
						<<setw(20)<<left<<" "\
						<<setw(30)<<left<<getCLenta(lbuf, lenta_position)\
						<<setw(20)<<left<<getCSt(sbuf)<<endl;
#define MFST_TRACE4(c, log)  *(log.stream)<<setw(4)<<left<< ++FST_TRACE_n<<": "<<setw(20)<<left<<c<<endl;
#define MFST_TRACE5(c, log)  *(log.stream)<<setw(4)<<left<< FST_TRACE_n<<": "<<setw(20)<<left<<c<<endl;
#define MFST_TRACE6(c,k, log) *(log.stream)<<setw(4)<<left<< FST_TRACE_n<<": "<<setw(20)<<left<<c<<k<<endl;
#define MFST_TRACE7(log)	  *(log.stream)<<setw(4)<<left<< state.lenta_position<<": " \
								<<setw(20)<<left<<rule.getCRule(rbuf,state.nrulechain)<<endl;
										//-------------------------------------------------------------------------
namespace MFST
{
	MfstState::MfstState() // конструктор по умолчанию
	{
		lenta_position = 0; //позиция на ленте
		nrulechain = -1; //номер текущего правила
		nrule = -1; //номер текущей цепочки
	}
	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain) //конструктор (позиция на ленте, стек автомата, номер текущей цепочки и текущего правила)
	{
		lenta_position = pposition; //позиция на ленте
		st = pst; //стек автомата
		nrulechain = pnrulechain; //номер текущего правила и цепочки
	}
	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain) //конструктор (позиция на ленте, стек автомата,номер текущего правила, номер текущей цепочки)
	{
		lenta_position = pposition; //позиция на ленте
		st = pst;  //стек автомата
		nrule = pnrule;
		nrulechain = pnrulechain; //номер текущего правила и цепочки
	}
	Mfst::MfstDiagnosis::MfstDiagnosis()
	{
		lenta_position = -1;
		rc_step = SURPRISE;
		nrule = -1;
		nrule_chain = -1;
	}
	Mfst::MfstDiagnosis::MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain)//диагностика
	{                                              //(позиция на ленте, код завершения шага, номер правила, номер цепочки правила)
		lenta_position = plenta_position; //позиция на ленте
		rc_step = prc_step; // код завершения шага
		nrule = pnrule; //номер правила
		nrule_chain = pnrule_chain; //номер цепочки правила
	}
	Mfst::Mfst()
	{
		lenta = 0;
		lenta_position = 0;
		lenta_size = 0;
	}
	Mfst::Mfst(Lex::LEX plex, GRB::Greibach pgreibach) //(результат работы лексического анализатора, грамматика Грейбах)
	{
		greibach = pgreibach; //Грейбах 
		lex = plex; //результат работы лекс анализатора
		lenta = new short[lenta_size = lex.lextable.size]; //размер ленты = текущий размер таблицы лексем
		for (int k = 0; k < lenta_size; k++)					//перекодирование ленты(заносит в ленту терминалы)
		{
			lenta[k] = TS(lex.lextable.table[k].lexema);
		}
		lenta_position = 0;
		st.push(greibach.stbottomT); //добавляет дно стека
		st.push(greibach.startN); //добавляет стартовый символ
		nrulechain = -1; //изначально правило равно -1
	};
	Mfst::RC_STEP Mfst::step(Log::LOG log)//( bool need_trace) //выполнить шаг автомата----------------------------------------------------?
	{
		RC_STEP rc = SURPRISE; //код возврата = ошибка возврата
		if (lenta_position < lenta_size)
		{
			if (ISNS(st.top())) //извлекаем последний элемент стека и проверяем нетерминал ли он
			{
				GRB::Rule rule;
				if ((nrule = greibach.getRule(st.top(), rule)) >= 0) //смотрим, если такое правило есть идём дальше, если нет то елсе
				{
					GRB::Rule::Chain chain;//используем это правило
					if ((nrulechain = rule.getNextChain(lenta[lenta_position], chain, nrulechain + 1)) >= 0) //получаем следующую цепочку и выводим её номер, илбо возвращаем -1
					{
						MFST_TRACE1(log) //вывод
							savestate(log); //сохранить состояние автомата
						st.pop(); //извлекаем верхушку стека.
						push_chain(chain); //поместить цепочку правила в стек
						rc = NS_OK; //найдено правило и цепочка... цепочка записана в стек
						MFST_TRACE2(log) //вывод
					}
					else
					{
						MFST_TRACE4("TNS_NORULECHAIN/NS_NORULE", log)//вывод
							savediagnosis(NS_NORULECHAIN);//код завершения
						rc = reststate(log) ? NS_NORULECHAIN : NS_NORULE;//восстановить состояние автомата
					};
				}
				else
				{
					rc = NS_ERROR;//неизвестный нетерминальный символ грамматики
				};
			}
			else if ((st.top() == lenta[lenta_position]))//если текущий символ ленты равен вершине стека
			{
				lenta_position++;//продвигаем ленту
				st.pop();//вершина стека
				nrulechain = -1;//номер текущего правила равен -1
				rc = TS_OK;
				MFST_TRACE3(log)
			}
			else
			{
				MFST_TRACE4("TS_NOK/NS_NORULECHAIN", log)
					rc = reststate(log) ? TS_NOK : NS_NORULECHAIN;
			};
		}
		else
		{
			if (st.top() == greibach.stbottomT)//если символ в стеке и символ дна стека их грамматики грейбах совпадают
			{
				rc = LENTA_END;
				MFST_TRACE4("--------------LENTA_END", log)
			}
			else
			{
				throw ERROR_THROW(611);
			}
		};
		return rc;
	};
	bool Mfst::push_chain(GRB::Rule::Chain chain)//поместить цепочку правила в стек (цепочка правила)
	{
		for (int k = chain.size - 1; k >= 0; k--)//к = длинне цепочке-1. заносим цепочку в стек
		{
			st.push(chain.nt[k]);
		}
		return true;
	}
	bool Mfst::savestate(Log::LOG log) //сохранить состояние автомата
	{
		storestate.push(MfstState(lenta_position, st, nrule, nrulechain));//стек для сохранения состояния. заносим сохраняемое состояние
		MFST_TRACE6("SAVESTATE: ", storestate.size(), log)
			return true;
	}
	bool Mfst::reststate(Log::LOG log)//восстановить состояние автомата
	{
		bool rc = false;
		MfstState state;
		if (rc = (storestate.size() > 0))
		{
			state = storestate.top();
			lenta_position = state.lenta_position;
			st = state.st;
			nrule = state.nrule;
			nrulechain = state.nrulechain;
			storestate.pop();
			MFST_TRACE5("RESTATE", log)
				MFST_TRACE2(log)
		}
		return rc;
	}
	bool Mfst::savediagnosis(RC_STEP pprc_step)
	{
		bool rc = false;
		short k = 0;
		while (k < MFST_DIAGN_NUMBER && lenta_position <= diagnosis[k].lenta_position)
		{
			k++;
		}
		if (rc = (k < MFST_DIAGN_NUMBER)) {
			diagnosis[k] = MfstDiagnosis(lenta_position, pprc_step, nrule, nrulechain);
			for (short j = k + 1; j < MFST_DIAGN_NUMBER; j++)
			{
				diagnosis[j].lenta_position = -1;
			}
		}
		return rc;
	}
	bool Mfst::start(Log::LOG log)
	{
		bool rc = false;
		RC_STEP rc_step = SURPRISE;
		char buf[MFST_DIAGN_MAXSIZE];
		rc_step = step(log);
		while (rc_step == NS_OK || rc_step == NS_NORULECHAIN || rc_step == TS_OK || rc_step == TS_NOK)
		{
			rc_step = step(log);
		}
		switch (rc_step)
		{
		case LENTA_END:
		{
			MFST_TRACE4("----------------------------->LENTA_END", log)
				*(log.stream) << "---------------------------------------------------------------------------------------\n";
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: всего строк %d, синтаксический анализ выполнен без ошибок", 0, lenta_size);
			*(log.stream) << setw(4) << left << 0 << ": всего строк " << lenta_size << ", синтаксический анализ выполнен без ошибок\n";
			rc = true;
			break;
		}
		case NS_NORULE:
		{	*(log.stream) << "NS_NORULE";
		*(log.stream) << getDiagnosis(0, buf) << endl;
		*(log.stream) << getDiagnosis(1, buf) << endl;
		*(log.stream) << getDiagnosis(2, buf) << endl;
		break;
		}
		case NS_NORULECHAIN:	MFST_TRACE4("---------->NS_NORULECHAIN", log) break;
		case NS_ERROR:			MFST_TRACE4("---------->NS_ERROR", log) break;
		case SURPRISE:			MFST_TRACE4("---------->SURPRISE", log) break;
		}
		if (errors.size)
		{
			throw errors;
		}
		return rc;
	}
	char* Mfst::getCSt(char* buf)
	{
		for (int k = (signed)st.size() - 1; k >= 0; k--)
		{
			short p = st._Get_container()[k];
			buf[st.size() - 1 - k] = GRB::Rule::Chain::alphabet_to_char(p);
		};
		buf[st.size()] = 0x00;
		return buf;
	}
	char* Mfst::getCLenta(char* buf, short pos, short n)
	{
		short i, k = (pos + n < lenta_size) ? pos + n : lenta_size;
		for (i = pos; i < k; i++)
		{
			buf[i - pos] = GRB::Rule::Chain::alphabet_to_char(lenta[i]);
		}
		buf[i - pos] = 0x00;
		return buf;
	}
	char* Mfst::getDiagnosis(short n, char* buf)
	{
		char *rc = "";
		int errid = 0;
		int lpos = -1;
		if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].lenta_position) >= 0)
		{
			errid = greibach.getRule(diagnosis[n].nrule).iderror;
			Error::ERROR err = ERROR_THROW(errid);
			Error::AddError(&errors, err);
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: строка %d, %s", err.id, lex.lextable.table[lpos].sn, err.message);
			rc = buf;
		}
		return rc;
	}
	void Mfst::printrules(Log::LOG &log)
	{
		MfstState state;
		GRB::Rule rule;
		for (unsigned short k = 0; k < storestate.size(); k++)
		{
			state = storestate._Get_container()[k];
			rule = greibach.getRule(state.nrule);
			MFST_TRACE7(log)
		};
	}
	bool Mfst::savededucation()
	{
		MfstState state;
		GRB::Rule rule;
		deducation.nrules = new short[deducation.size = storestate.size()];
		deducation.nrulechains = new short[deducation.size];
		for (unsigned short k = 0; k < storestate.size(); k++)
		{
			state = storestate._Get_container()[k];
			deducation.nrules[k] = state.nrule;
			deducation.nrulechains[k] = state.nrulechain;
		}
		return true;
	}
}