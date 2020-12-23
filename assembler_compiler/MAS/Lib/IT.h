#pragma once
#include "Log.h"
#define ID_MAXSIZE		10			//������������ ���-�� �������� � ��������������
#define TI_MAXSIZE		4096		//������������ ���-�� ����� � ��������������
#define TI_INT_DEFAULT	0x00000000	//�������� �� ��������� ��� ���� integer
#define TI_STRDEFAULT	0x00		//�������� �� ��������� ��� ���� string
#define TI_NULLIDX		0xffffffff	//��� �������� ������� ���������������
#define TI_STR_MAXSIZE	255	
//���� ���������������
namespace IT
{
	enum IDDATATYPE { OFF = 0, INT = 1, CHAR = 2, OPERATOR = 3 };
	enum IDTYPE { N = 0, F = 1, V = 2, P = 3, L = 4, O = 5 };   //��� ����, �������, ����������, �������� �������, �������, �������� 

	struct Entry
	{
		int			id_first_LE;		//������ ������� ��������� � ������� ������
		string		id;					//�������������	(�������������� ��������� �� ID_MAXSIZE)
		IDDATATYPE	id_data_type;		//��� ������
		IDTYPE		id_type;			//��� ��������������
		struct
		{
			int vint;				//�������� int
			struct {
				int len;						//����� string
				string str;						//������� string
			} vstr;								//�������� string
		} value;				//�������� ��������������
	};

	struct IdTable				//��������� ������� ���������������
	{
		int maxsize;						//������� ������� ��������������� < TI_MAXSIZE
		int size;								//������� ������ ������� ��������������� < maxsize
		Entry* table;							//������ ����� ������� ���������������
	};

	IdTable Create(							//������� ������� ���������������
		int size								//������� ������� ���������������
	);

	void Add(								//�������� ������ � ������� ���������������
		IdTable& idtable,						//��������� ������� ���������������
		Entry entry								//������ ������� ���������������
	);

	Entry GetEntry(							//�������� ������ ������� ���������������
		IdTable& idtable,						//��������� ������� ���������������
		int n									//����� ���������� ������
	);

	int IsId(								//�������: ����� ������(���� ����), TI_NULLIDX(���� ���)
		IdTable& idtable,						//��������� ������� ���������������
		string id								//�������������
	);

	void Delete(IdTable& idtable);					//������� ������� id
	int CheckId(IdTable& idtable, string value);
	int IsDublId(IdTable& idtable, string id);		//�������� �� ������������ ��
	int CheckId(IdTable& idtable, int value);       //������� ������� ������
	void ShowTable(Log::LOG log, IdTable& idtable);

};