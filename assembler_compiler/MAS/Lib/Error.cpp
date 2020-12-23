#include "stdafx.h"
#include "Error.h"
namespace Error
{
	//	0   -  99  -  системные
	//	100 - 109  -  параметров
	//	110 - 119  -  открытия и чтения файлов
	//	120 - 200  -  ошибки лексического анализа
	//  600 - 699  -  ошибки синтаксического анализа
	//  700 - 799  -  ошибки семантического анализа
	ERROR errors[ERROR_MAX_ENTRY] = //таблица ошибок
	{
		ERROR_ENTRY(0, "Недопустимый код ошибки"), // код ошибки вне диапазона 0 - ERROR_MAX_ENTRY
		ERROR_ENTRY(1, "Системный сбой"),
		ERROR_ENTRY_NODEF(2), ERROR_ENTRY_NODEF(3), ERROR_ENTRY_NODEF(4), ERROR_ENTRY_NODEF(5),
		ERROR_ENTRY_NODEF(6), ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50),
		ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),

		ERROR_ENTRY(100, "Ошибка запуска: Параметр -in должен быть задан"),
		ERROR_ENTRY(101, "Ошибка запуска: Превышена длина входного параметра"),
		ERROR_ENTRY_NODEF(102), ERROR_ENTRY_NODEF(103), ERROR_ENTRY_NODEF(104), ERROR_ENTRY_NODEF(105),
		ERROR_ENTRY_NODEF(106), ERROR_ENTRY_NODEF(107), ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),

		ERROR_ENTRY(110, "Ошибка проверки входного файла: Ошибка при открытии файла с исходным кодом (-in)"),
		ERROR_ENTRY(111, "Ошибка проверки входного файла: Недопустимый символ в исходном файле (-in)"),
		ERROR_ENTRY(112, "Ошибка проверки входного файла: Ошибка при создании файла протокола(-log)"),
		ERROR_ENTRY(113, "Ошибка проверки входного файла: Превышена длина идентификатора"), ERROR_ENTRY_NODEF(114), ERROR_ENTRY_NODEF(115), ERROR_ENTRY_NODEF(116), ERROR_ENTRY_NODEF(117), ERROR_ENTRY_NODEF(118), ERROR_ENTRY_NODEF(119),

		ERROR_ENTRY(120, "Ошибка лексики: Цепочка символов не разобрана"),
		ERROR_ENTRY(121, "Ошибка лексики: Таблица лексем переполнена"),
		ERROR_ENTRY(122, "Ошибка лексики: Таблица идентификаторов переполнена"),
		ERROR_ENTRY(123, "Ошибка лексики: Дублирование идентификатора"),
		ERROR_ENTRY(124, "Ошибка лексики: Два арифметических знака подряд"),	ERROR_ENTRY_NODEF(125), ERROR_ENTRY_NODEF(126),ERROR_ENTRY_NODEF(127),ERROR_ENTRY_NODEF(128), ERROR_ENTRY_NODEF(129),
		ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140), ERROR_ENTRY_NODEF10(150),
		ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),

		ERROR_ENTRY_NODEF100(200), ERROR_ENTRY_NODEF100(300), ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),

		ERROR_ENTRY(600, "Ошибка синтаксиса: Неверная структура программы"),
		ERROR_ENTRY(601, "Ошибка синтаксиса: Ошибочный оператор"),
		ERROR_ENTRY(602, "Ошибка синтаксиса: Неверное выражение"),
		ERROR_ENTRY(603, "Ошибка синтаксиса: Ошибка в параметрах функции или операторе объявления"),
		ERROR_ENTRY(604, "Ошибка синтаксиса: Ошибка в параметре вызываемой функции out стандартной библиотеки"),
		ERROR_ENTRY(605, "Ошибка синтаксиса: Ошибка в параметрах вызываемой функции squar стандартной библиотеки"),
		ERROR_ENTRY(606, "Ошибка синтаксиса: Ошибка в параметре вызываемой функции strl стандартной библиотеки"),
		ERROR_ENTRY(607, "Ошибка синтаксиса: Ошибка в параметрах вызываемой функции"),
		ERROR_ENTRY(608, "Ошибка синтаксиса: Ошибка арифметического оператора"),
		ERROR_ENTRY(609, "Ошибка синтаксиса: Ошибка в операторе ветвления"),
		ERROR_ENTRY(610, "Ошибка синтаксиса: Ошибка в возвращаемом выражении"),
		ERROR_ENTRY(611, "Ошибка синтаксиса: Не найден конец правила"),
		ERROR_ENTRY(612, "Ошибка синтаксиса: Цепочка разобрана не полностью (стек не пустой)"),
		ERROR_ENTRY_NODEF(613), ERROR_ENTRY_NODEF(614),ERROR_ENTRY_NODEF(615),
		ERROR_ENTRY_NODEF(616), ERROR_ENTRY_NODEF(617),ERROR_ENTRY_NODEF(618),ERROR_ENTRY_NODEF(619),
		ERROR_ENTRY_NODEF10(620), ERROR_ENTRY_NODEF10(630), ERROR_ENTRY_NODEF10(640),
		ERROR_ENTRY_NODEF10(650), ERROR_ENTRY_NODEF10(660), ERROR_ENTRY_NODEF10(670), ERROR_ENTRY_NODEF10(680),
		ERROR_ENTRY_NODEF10(690),

		ERROR_ENTRY(700, "Ошибка семантики: Повторное объявление идентификатора"),
		ERROR_ENTRY(701, "Ошибка семантики: Ошибка в типе идентификатора"),
		ERROR_ENTRY(702, "Ошибка семантики: Ошибка в передаваемых значениях в функцию"),
		ERROR_ENTRY(703, "Ошибка семантики: В функцию не переданы параметры"),
		ERROR_ENTRY(704, "Ошибка семантики: Тип данных результата выражения не соответствует присваиваемому идентификатору"),
		ERROR_ENTRY(705, "Ошибка семантики: Ошибка в параметре вызываемой функции strlen стандартной библиотеки"),
		ERROR_ENTRY(706, "Ошибка семантики: Необъявленный идентификатор"),
		ERROR_ENTRY(707, "Ошибка семантики: Несоответствие типов в операторе присваивания"),

		ERROR_ENTRY_NODEF(708),	ERROR_ENTRY_NODEF(709),
		ERROR_ENTRY_NODEF10(710),ERROR_ENTRY_NODEF10(720),ERROR_ENTRY_NODEF10(730),ERROR_ENTRY_NODEF10(740),
		ERROR_ENTRY_NODEF10(750),ERROR_ENTRY_NODEF10(760),ERROR_ENTRY_NODEF10(770),ERROR_ENTRY_NODEF10(780),
		ERROR_ENTRY_NODEF10(790),
		ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900)
	};
	ERROR geterror(int id)//там где не над указ стр в исх файле,ф-ия для вызова макроса Error throw
	{
		if (id > ERROR_MAX_ENTRY || id < 0)
		{
			return errors[0];
		}
		return errors[id];
	}
	ERROR geterrorin(int id, int line = -1, int col = -1)//если нужна стр и позиция в исх коде error throw in
	{
		if (id > ERROR_MAX_ENTRY || id < 0)
		{
			return errors[0];
		}
		errors[id].intext.line = line;
		errors[id].intext.col = col;
		return errors[id];
	}
	void AddError(ARRAY_OF_ERRORS* arr, ERROR err)//добавляет ошбику в масив ошибок или генерирует искл если идёт переполнение массива ошибок (передаём через указатель* массив ошибок)чтобы можн было изменять имненно его
	{
		if (arr->size < ARRAY_OF_ERRORS_MAX_SIZE)
		{
			arr->arr_of_err[arr->size++] = err;//записыв в массив ошибок ошибку кот передаём в кач 2го параметра
		}
		else
		{
			throw *arr; //если размер бобльше 5 выбр в общ массив ошибок
		}
	}
}