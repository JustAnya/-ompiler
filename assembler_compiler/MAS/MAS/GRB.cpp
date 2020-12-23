#include "stdafx.h"
#include "GRB.h"

#define GRB_ERROR_SERIES 600
//�� ����(���������)
namespace GRB
{
#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)
	Greibach greibach(
		NS('S'), TS('$'),                     // ��������� ������, ��� ����� NS-����������(������� �����),TS-��������
		6, 									  // ���������� ������
		Rule(
			NS('S'), GRB_ERROR_SERIES + 0,    // �������� ��������� ���������  
			2,                                // S->tfi(F){N};S | m{N};
			Rule::Chain(11, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('}'), TS(';'), NS('S')),
			Rule::Chain(5, TS('m'), TS('{'), NS('N'), TS('}'), TS(';'))
		),
		Rule(
			NS('N'), GRB_ERROR_SERIES + 1,    // ��������� ��������
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
			NS('E'), GRB_ERROR_SERIES + 2,    // ������ � ���������
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
			NS('F'), GRB_ERROR_SERIES + 3,    // ������ � ���������� �������
			2,                                // F -> ti | ti,F
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F'))
		),
		Rule(
			NS('W'), GRB_ERROR_SERIES + 4,    // ������ � ���������� ���������� ������� 
			6,                                // W -> i | c | n | i,W | c,W | n,W 
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('c')),
			Rule::Chain(1, TS('n')),
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('c'), TS(','), NS('W')),
			Rule::Chain(3, TS('n'), TS(','), NS('W'))
		),
		Rule(
			NS('M'), GRB_ERROR_SERIES + 5,    // ��������
			3,								  // M -> a | aE | aEM 
			Rule::Chain(1, TS('a')),
			Rule::Chain(2, TS('a'), NS('E')),
			Rule::Chain(3, TS('a'), NS('E'), NS('M'))
		)
	);

	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)  //����������� ������� - ������ ����� �������(���-�� �������� � �������, �������� ��� ����������...)
	{
		nt = new GRBALPHABET[size = psize];    //������� ����������
		int*o = (int*)&s;                      //����������� ������ ��������� �
		for (short i = 0; i < psize; ++i)
			nt[i] = (GRBALPHABET)o[i]; //��������� ������� ����������
	};
	Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...) //����������� �������
	{															//(����������, ������������� ���������������� ���������, ���������� �������(������ ������ �������), ��������� ������� (������ ������ �������)
		nn = pnn;    //����������
		iderror = piderror; //�������������
		chains = new Chain[size = psize]; //����� ��� �������
		Chain*o = &c;
		for (int i = 0; i < size; i++)
			chains[i] = o[i]; //��������� ��������� �������
	};
	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottom, short psize, Rule r, ...)//����������� ���������� ��������(��������� ������, ��� �����, ���������� ������, �������...)
	{
		startN = pstartN; //��������� ������
		stbottomT = pstbottom;//��� �����
		rules = new Rule[size = psize];//�������� ������
		Rule*o = &r;
		for (int i = 0; i < size; i++) rules[i] = o[i];//��������� �������
	};
	Greibach getGreibach() { return greibach; }; //�������� ����������
	short Greibach::getRule(GRBALPHABET pnn, Rule& prule) //�������� ������� (����� ������ �������, ������������ ������� ����������)
	{
		short rc = -1;
		short k = 0;
		while (k < size && rules[k].nn != pnn)
			k++;   //���� � ������ ���������� ������ � ���� ����� ������ ������� �� ����� �������� �-���
		if (k < size)
			prule = rules[rc = k];    //������������ ������� ��������� ����� ������� � �������� �
		return rc; //������������ ����� ������� ��� -1
	};
	Rule Greibach::getRule(short n) //�������� ������� �� ������
	{
		Rule rc;      //������ ������� ��
		if (n < size)rc = rules[n];  //����������� ������� �� ������� �
		return rc; //���������� ��������� �������
	};
	char* Rule::getCRule(char* b, short nchain) //�������� ������� � ���� N->������� (�����, ����� �������(������ �����) � �������)
	{
		char bchain[200]; //������
		b[0] = Chain::alphabet_to_char(nn); b[1] = '-'; b[2] = '>'; b[3] = 0x00; //�������� -> 
		chains[nchain].getCChain(bchain); //�������� ������ ������� �������
		strcat_s(b, sizeof(bchain) + 5, bchain);//��������� ������ (����, � ������ ��������, ������)
		return b;
	};
	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j) //�������� ��������� �� j ���������� �������, ������� � ����� ��� -1 
	{                                                                //(������ ������ �������, ������������ �������, ����� �������)
		short rc = -1;
		while (j < size && chains[j].nt[0] != t)++j;
		rc = (j < size ? j : -1);
		if (rc >= 0)pchain = chains[rc];
		return rc;
	};
	char*Rule::Chain::getCChain(char*b) //�������� ������ ������� �������
	{
		for (int i = 0; i < size; i++)b[i] = Chain::alphabet_to_char(nt[i]);
		b[size] = 0x00;
		return b;
	};
};