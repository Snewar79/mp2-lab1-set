// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле
// Далее накостылено Волковым П.С. 381606-2 (03.10.17)

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len <= 0) throw len;
	BitLen = len;
	MemLen = (BitLen + 1) / (sizeof(TELEM) * 8) + 1;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++)
			pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete pMem;
	pMem = NULL;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << n % (sizeof(TELEM) * 8);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0) throw n;
	if (n >= BitLen) throw n;
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0) throw n;
	if (n >= BitLen) throw n;
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0) throw n;
	if (n >= BitLen) throw n;
	if ((pMem[GetMemIndex(n)] & GetMemMask(n)) != 0) return 1;
	else return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	delete pMem;
	MemLen = bf.MemLen;
	BitLen = bf.BitLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
	return *this;

}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen)
		return false;
	for (int i = 0; i < MemLen; i++)	
		if (pMem[i] != bf.pMem[i])
			return false;

	return true;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (operator==(bf) == 1) return 0;
	else return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int max = BitLen;
	if (BitLen < bf.BitLen) max = bf.BitLen;
	TBitField tmp(max);
	for (int i = 0; i < tmp.MemLen; i++)
		tmp.pMem[i] = pMem[i] | bf.pMem[i];
	return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int max = BitLen;
	int min_ml = MemLen;
	if (BitLen < bf.BitLen) max = bf.BitLen;
	if (MemLen > bf.MemLen) min_ml = bf.MemLen;
	TBitField tmp(max);
	for (int i = 0; i < min_ml; i++)
		tmp.pMem[i] = pMem[i] & bf.pMem[i];
	return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(*this);
	for (int i = 0; i < MemLen; i++)
		tmp.pMem[i] = ~pMem[i];
	TELEM mask = 0;
	for (unsigned int i = 0; i < BitLen % (sizeof(TELEM) * 8); i++)
	{
		mask = mask << 1;
		mask += 1;
	}
	tmp.pMem[MemLen - 1] &= mask;
	return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	string str;
	char c;
	int counter = 0;
	istr.get(c);
	while (c != '\n')
	{
		if (c == '1') {
			bf.SetBit(counter); 
			counter++;
		}
		if (c == '0') {
			bf.ClrBit(counter);
			counter++;
		}
		istr.get(c);
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
		if (bf.GetBit(i) != 0)
			ostr << "1 ";
		else ostr << "0 ";

	//	if (bf.GetBit(bf.BitLen - 1) != '0') ostr << "1";
	//	else ostr << "0";
	return ostr;
}
