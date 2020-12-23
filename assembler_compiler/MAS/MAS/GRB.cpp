#include "stdafx.h"
#include "GRB.h"

#define GRB_ERROR_SERIES 600
//из лабы(синтаксич)
namespace GRB
{
#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)
	Greibach greibach(
		NS('S'), TS('$'),                     // стартовый символ, дно стека NS-нетерминал(большие буквы),TS-терминал
		6, 									  // количество правил
		Rule(
			NS('S'), GRB_ERROR_SERIES + 0,    // неверная структура программы  
			2,                                // S->tfi(F){N};S | m{N};
			Rule::Chain(11, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('}'), TS(';'), NS('S')),
			Rule::Chain(5, TS('m'), TS('{'), NS('N'), TS('}'), TS(';'))
		),
		Rule(
			NS('N'), GRB_ERROR_SERIES + 1,    // ошибочный оператор
			10,                               // N->dti;N | rE; | i=E;N | o(i);N | o(c);N | on;N
			Rule::Chain(5, TS('v'), TS('t'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(3, TS('r'), NS('E'), TS(';')),
			Rule::Chain(4, TS('r'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(6, TS('o'), TS('('), TS('i'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(6, TS('o'), TS('('), TS('c'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(6, TS('o'), TS('('), TS('n'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(5, TS('o'), TS('('), TS('c'), TS(')'), TS(';')),
			Rule::Chain(5, TS('o'), TS('('), TS('i'), TS(')'), TS(';')),
			Rule::Chain(5, TS('o'), TS('('), TS('n'), TS(')'), TS(';'))
		),
		Rule(
			NS('E'), GRB_ERROR_SERIES + 2,    // ошибка в выражении
			15,                                // E->i | c | n | (E) | nM | i(W) | iM | dM | (E)M | i(W)M | q(i) | s(i)
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('c')),
			Rule::Chain(1, TS('n')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(2, TS('n'), NS('M')),
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(2, TS('d'), NS('M')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M')),
			Rule::Chain(6, TS('p'), TS('('), TS('i'), TS(','), TS('n'), TS(')')),
			Rule::Chain(6, TS('p'), TS('('), TS('i'), TS(','), TS('i'), TS(')')),
			Rule::Chain(6, TS('p'), TS('('), TS('n'), TS(','), TS('n'), TS(')')),
			Rule::Chain(4, TS('s'), TS('('), TS('i'), TS(')')),
			Rule::Chain(4, TS('s'), TS('('), TS('c'), TS(')'))
		),
		Rule(
			NS('F'), GRB_ERROR_SERIES + 3,    // ошибка в параметрах функции
			2,                                // F -> ti | ti,F
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F'))
		),
		Rule(
			NS('W'), GRB_ERROR_SERIES + 4,    // ошибка в параметрах вызываемой функции 
			6,                                // W -> i | c | n | i,W | c,W | n,W 
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('c')),
			Rule::Chain(1, TS('n')),
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('c'), TS(','), NS('W')),
			Rule::Chain(3, TS('n'), TS(','), NS('W'))
		),
		Rule(
			NS('M'), GRB_ERROR_SERIES + 5,    // оператор
			3,								  // M -> a | aE | aEM 
			Rule::Chain(1, TS('a')),
			Rule::Chain(2, TS('a'), NS('E')),
			Rule::Chain(3, TS('a'), NS('E'), NS('M'))
		)
	);

	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)  //конструктор цепочки - праыой части правила(кол-во символов в цепочке, терминал или нетерминал...)
	{
		nt = new GRBALPHABET[size = psize];    //цепочка терминалов
		int*o = (int*)&s;                      //присваиваем символ указателю п
		for (short i = 0; i < psize; ++i)
			nt[i] = (GRBALPHABET)o[i]; //заполняем цепочку терминалов
	};
	Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...) //конструктор правила
	{															//(нетерминал, идентификатор диагностического сообщения, количество цепочек(правых частей правила), множество цепочек (правых частей правила)
		nn = pnn;    //нетерминал
		iderror = piderror; //идентификатор
		chains = new Chain[size = psize]; //место для цепочки
		Chain*o = &c;
		for (int i = 0; i < size; i++)
			chains[i] = o[i]; //заполняем множество цепочек
	};
	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottom, short psize, Rule r, ...)//конструктор гграматики Грейбаха(стартовый символ, дно стека, количество правил, правила...)
	{
		startN = pstartN; //стартовый символ
		stbottomT = pstbottom;//дно стека
		rules = new Rule[size = psize];//выделяем память
		Rule*o = &r;
		for (int i = 0; i < size; i++) rules[i] = o[i];//заполняем правила
	};
	Greibach getGreibach() { return greibach; }; //получить грамматику
	short Greibach::getRule(GRBALPHABET pnn, Rule& prule) //получить правило (левый символ правила, возвращаемое правило грамматики)
	{
		short rc = -1;
		short k = 0;
		while (k < size && rules[k].nn != pnn)
			k++;   //пока К меньше количества правил и пока левый символ правила не равен парметру ф-ции
		if (k < size)
			prule = rules[rc = k];    //возвращаемое правило граматики равно правилу с индексом К
		return rc; //возвращается номер правила или -1
	};
	Rule Greibach::getRule(short n) //получить правило по номеру
	{
		Rule rc;      //создаём правило рц
		if (n < size)rc = rules[n];  //присваиваем правилу рц правило н
		return rc; //возвращаем созданное правило
	};
	char* Rule::getCRule(char* b, short nchain) //получить правило в виде N->цепочка (буфер, номер цепочки(правой части) в правиле)
	{
		char bchain[200]; //строка
		b[0] = Chain::alphabet_to_char(nn); b[1] = '-'; b[2] = '>'; b[3] = 0x00; //терминал -> 
		chains[nchain].getCChain(bchain); //получает правую сторонц правила
		strcat_s(b, sizeof(bchain) + 5, bchain);//добавляем строку (куда, с какого элемента, строку)
		return b;
	};
	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j) //получить следующую за j подходящую цепочку, вернуть её номер или -1 
	{                                                                //(первый символ цепочки, возвращаемая цепочка, номер цепочки)
		short rc = -1;
		while (j < size && chains[j].nt[0] != t)++j;
		rc = (j < size ? j : -1);
		if (rc >= 0)pchain = chains[rc];
		return rc;
	};
	char*Rule::Chain::getCChain(char*b) //получить правую сторону правила
	{
		for (int i = 0; i < size; i++)b[i] = Chain::alphabet_to_char(nt[i]);
		b[size] = 0x00;
		return b;
	};
};