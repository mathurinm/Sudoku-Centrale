#include "stdafx.h"
#include "CppUnitTest.h"
#include "Cell.h"
#include "Region.h"
#include "Grid.h"
#include "RegionHolder.h"
#include "LastCellFinder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SudokuUnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		//TODO rename tests explicitly
		TEST_METHOD(CellValue1)
		{
			Cell cell = Cell(2);
			Assert::AreEqual(cell.GetValue(), 2);
		}
		TEST_METHOD(EmptyCell)
		{
			Cell cell = Cell();
			Assert::AreEqual(cell.GetValue(), -1);
		}

		TEST_METHOD(RegionStringConstructor)
		{
			Region myRegion1 = Region("1-3456789");

			Assert::AreEqual(1, myRegion1.Get_cNW().GetValue());
			Assert::AreEqual(-1, myRegion1.Get_cN().GetValue());
			Assert::AreEqual(3, myRegion1.Get_cNE().GetValue());
			Assert::AreEqual(4, myRegion1.Get_cW().GetValue());
			Assert::AreEqual(5, myRegion1.Get_cC().GetValue());
			Assert::AreEqual(6, myRegion1.Get_cE().GetValue());
			Assert::AreEqual(7, myRegion1.Get_cSW().GetValue());
			Assert::AreEqual(8, myRegion1.Get_cS().GetValue());
			Assert::AreEqual(9, myRegion1.Get_cSE().GetValue());
		}

		TEST_METHOD(LastCellFinderWithHolderAccessors)
		{
			vector<string> stringInput = vector<string>(9);
			stringInput[0] = "1234-6789";
			for (int i = 1; i < 9; i++)
			{
				stringInput[i] = "123456789";
			}

			Grid grid1 = Grid(stringInput);

			RegionHolder regHold = RegionHolder(grid1.Get_rNW());

			RowHolder topRow1 = regHold.TopRow();
			RowHolder middleRow1 = regHold.MiddleRow();
			RowHolder bottomRow1 = regHold.BottomRow();

			LastCellFinder last = LastCellFinder(topRow1, middleRow1, bottomRow1);
			Assert::AreEqual(-1 , grid1.Get_rNW().Get_cC().GetValue());
			last.fill();
			Assert::AreEqual(5, grid1.Get_rNW().Get_cC().GetValue());
		}

		TEST_METHOD(LastCellFinderWithHolderCells)
		{


			vector<string> stringInput = vector<string>(9);
			stringInput[0] = "1234-6789";
			for (int i = 1; i < 9; i++)
			{
				stringInput[i] = "123456789";
			}

			Grid grid1 = Grid(stringInput);
			//Here's the different part
			TripleHolder topRow1 = TripleHolder(grid1.Get_rNW().Get_cNW(), grid1.Get_rNW().Get_cN(), grid1.Get_rNW().Get_cNE());
			TripleHolder middleRow1 = TripleHolder(grid1.Get_rNW().Get_cW(), grid1.Get_rNW().Get_cC(), grid1.Get_rNW().Get_cE());
			TripleHolder bottomRow1 = TripleHolder(grid1.Get_rNW().Get_cSW(), grid1.Get_rNW().Get_cS(), grid1.Get_rNW().Get_cSE());

			LastCellFinder lastCF = LastCellFinder(topRow1, middleRow1, bottomRow1);
			Assert::AreEqual(-1, grid1.Get_rNW().Get_cC().GetValue());
			lastCF.fill();
			Assert::AreEqual(5, grid1.Get_rNW().Get_cC().GetValue());
		}

		TEST_METHOD(OnlyOneChoiceRow)
		{
			vector<string> stringInput = vector<string>(9);
		    /*grid:
			-23456789
			123456789
			etc
			First row should be filled
			*/

			stringInput[0] = "-23123123";
			stringInput[1] = "456456456";
			stringInput[2] = "789789789";
			for (int i = 3; i < 9; i++)
			{
				stringInput[i] = "123456789";
			}

			Grid grid1 = Grid(stringInput);
		}

	};
}