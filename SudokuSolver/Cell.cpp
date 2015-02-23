#include "Cell.h"
#include <stdexcept>
using namespace std;

Cell::Cell()
{
	_value = -1;
}

Cell::Cell(int value)
{
	if (value < 0 || value > 9)
	{
		_isEmpty = true;
		throw invalid_argument("incorrect input value for Cell");
	}
	else
	{
		_value = value;
		_isEmpty = false;
	}
	
}

bool Cell::IsEmpty() const
{
	return _isEmpty;
}

//Bad practiiiiiiiiiice
Cell& Cell::operator=(unsigned char iValue)
{
	_value = iValue;
	return *this;
}

Cell::operator unsigned char() const
{
	return _value;
}

bool Cell::operator==(unsigned char iValue) const
{
	return (_value == iValue);
}

Cell::~Cell()
{
}
