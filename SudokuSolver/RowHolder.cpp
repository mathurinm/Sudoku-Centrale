#include "RowHolder.h"


RowHolder::RowHolder(Cell& cell1, Cell& cell2, Cell& cell3) :TripleHolder(cell1, cell2, cell3)
{
}


RowHolder::~RowHolder()
{
}

Cell& RowHolder::L()
{
	return Cell1();
}

Cell& RowHolder::C()
{
	return Cell2();
}

Cell& RowHolder::R()
{
	return Cell3();
}