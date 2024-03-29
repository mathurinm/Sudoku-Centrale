#pragma once

#ifdef SUDOKUSOLVER_EXPORTS
#define ExportedByDll __declspec(dllexport)
#else
#define ExportedByDll __declspec(dllimport)
#endif

//forward declarations
class ValueEliminator;
#include "Cell.h"

class ExportedByDll TripleHolder
{
public:
	TripleHolder(Cell& cell1, Cell& cell2, Cell& cell3 );
	~TripleHolder();
	Cell& Cell1();
	Cell& Cell2();
	Cell& Cell3();
	void flagValues(ValueEliminator& v);
	bool isValuePresent(unsigned char);

protected:
	Cell& _cell1;
	Cell& _cell2;
	Cell& _cell3;
};

