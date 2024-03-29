#include "Region.h"
#include <vector>
using namespace std;

Region::Region()
{

}

Region::Region(string initialValue)
{
	if (initialValue.length() != 9) {
		throw invalid_argument("Length of the string should be 9.\n");
	}

	string allowed = "123456789-";
	int value = initialValue[0];
	_cNW = Cell(value, allowed);
	value = initialValue[1];
	_cN = Cell(value, allowed);
	value = initialValue[2];
	_cNE = Cell(value, allowed);
	value = initialValue[3];
	_cW = Cell(value, allowed);
	value = initialValue[4];
	_cC = Cell(value, allowed);
	value = initialValue[5];
	_cE = Cell(value, allowed);
	value = initialValue[6];
	_cSW = Cell(value, allowed);
	value = initialValue[7];
	_cS = Cell(value, allowed);
	value = initialValue[8];
	_cSE = Cell(value, allowed);
}


Region::~Region()
{
}


Cell& Region::getCell(unsigned char i, unsigned char j)
{
	if (i == 0 && j == 0) return _cNW;
	if (i == 0 && j == 1) return _cN;
	if (i == 0 && j == 2) return _cNE;
	if (i == 1 && j == 0) return _cW;
	if (i == 1 && j == 1) return _cC;
	if (i == 1 && j == 2) return _cE;
	if (i == 2 && j == 0) return _cSW;
	if (i == 2 && j == 1) return _cS;
	if (i == 2 && j == 2) return _cSE;
	else throw invalid_argument("i and j must be between 0 and 2.\n");
}

bool Region::isFull()
{
	bool gridIsEmpty = _cNW.IsEmpty() || _cN.IsEmpty() || _cNE.IsEmpty() || _cW.IsEmpty() || _cC.IsEmpty() || _cE.IsEmpty() || _cSW.IsEmpty() || _cS.IsEmpty() || _cSE.IsEmpty();

	return !gridIsEmpty;
}

Region& Region::operator=(const Region& r)
{
	_cNW = r._cNW;
	_cN = r._cN;
	_cNE = r._cNE;
	_cW = r._cW;
	_cC = r._cC;
	_cE = r._cE;
	_cSW = r._cSW;
	_cS = r._cS;
	_cSE = r._cSE;
	return *this;
}
