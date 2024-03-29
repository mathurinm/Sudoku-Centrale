#include "stdafx.h"
#include "CppUnitTest.h"
#include "Cell.h"
#include "Region.h"
#include "Grid.h"
#include "RegionHolder.h"
#include "LastCellFinder.h"
#include "OnlyOneChoiceInRowVisitor.h"
#include "OnlyOneChoiceInColumnVisitor.h"
#include "OnlyOneChoiceInRegionVisitor.h"
#include "OnlySquareVisitor.h"
#include "TwoOutOfThreeRowVisitor.h"
#include "TwoOutOfThreeColumnVisitor.h"
#include "OnlyOneChoiceGlobalVisitor.h"
#include "RowColumnRegionVisitor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SudokuUnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_METHOD(CellValue1)
		{
			Cell cell(2);
			Assert::AreEqual(cell.getValue(), 2);
		}

		TEST_METHOD(EmptyCell)
		{
			Cell cell;
			Assert::AreEqual(cell.getValue(), -1);
		}

		TEST_METHOD(WrongInputCell1)
		{
			bool thrown = false;
			try{ Cell cell(10); }
			catch (invalid_argument& e){
				thrown = true;
			}
			Assert::IsTrue(thrown);
		}

		TEST_METHOD(WrongInputCell2)
		{
			bool thrown = false;
			try{ Cell cell(0); }
			catch (invalid_argument& e){
				thrown = true;
			}
			Assert::IsTrue(thrown);
		}

		TEST_METHOD(CellToCharCast)
		{
			Cell cell(5);
			Assert::AreEqual((unsigned char)cell, (unsigned char)5);
		}

		TEST_METHOD(CellEqualCharOperator)
		{
			Cell cell(2);
			Assert::AreEqual(cell.getValue(), 2);
			cell = 5;
			Assert::AreEqual(cell.getValue(), 5);
		}

		TEST_METHOD(RegionStringConstructor1)
		{
			Region myRegion1("1-3456789");

			Assert::AreEqual(1, myRegion1.getCell(0, 0).getValue());
			Assert::AreEqual(-1, myRegion1.getCell(0, 1).getValue());
			Assert::AreEqual(3, myRegion1.getCell(0, 2).getValue());
			Assert::AreEqual(4, myRegion1.getCell(1, 0).getValue());
			Assert::AreEqual(5, myRegion1.getCell(1, 1).getValue());
			Assert::AreEqual(6, myRegion1.getCell(1, 2).getValue());
			Assert::AreEqual(7, myRegion1.getCell(2, 0).getValue());
			Assert::AreEqual(8, myRegion1.getCell(2, 1).getValue());
			Assert::AreEqual(9, myRegion1.getCell(2, 2).getValue());
		}

		TEST_METHOD(RegionStringConstructor2)
		{
			bool thrown = false;
			try{
				Region myRegion1("1-345678a"); // a is not a valid character
			}
			catch (invalid_argument& e){
				thrown = true;
			}
			Assert::IsTrue(thrown);
		}

		TEST_METHOD(RegionStringConstructor3)
		{
			bool thrown = false;
			try{
				Region myRegion1("1-34567890"); //input string is too long
			}
			catch (invalid_argument& e){
				thrown = true;
			}
			Assert::IsTrue(thrown);
		}

		TEST_METHOD(RegionStringConstructor4)
		{
			bool thrown = false;
			try{
				Region myRegion1("12345678"); //input string is too short
			}
			catch (invalid_argument& e){
				thrown = true;
			}
			Assert::IsTrue(thrown);
		}

		TEST_METHOD(LastCellFinderWithHolderAccessors)
		{
			vector<string> stringInput = vector<string>(9);
			stringInput[0] = "1234-6789";
			for (unsigned char i = 1; i < 9; i++)
			{
				stringInput[i] = "123456789";
			}
			Grid grid1(stringInput);
			RegionHolder regHold(grid1.getRegion(0, 0));
			RowHolder topRow1 = regHold.TopRow();
			RowHolder middleRow1 = regHold.MiddleRow();
			RowHolder bottomRow1 = regHold.BottomRow();

			LastCellFinder last(topRow1, middleRow1, bottomRow1);
			Assert::AreEqual(-1 , grid1.getRegion(0, 0).getCell(1, 1).getValue());
			bool visited = last.fill();
			Assert::IsTrue(visited);
			Assert::AreEqual(5, grid1.getRegion(0, 0).getCell(1, 1).getValue());
		}

		TEST_METHOD(LastCellFinderWithHolderCells)
		{
			vector<string> stringInput = vector<string>(9);
			stringInput[0] = "1234-6789";
			for (unsigned char i = 1; i < 9; i++)
			{
				stringInput[i] = "123456789";
			}
			Grid grid1(stringInput);
			//Here's the different part compared to LastCellFinderWithHolderAccessors
			TripleHolder topRow1(grid1.getRegion(0, 0).getCell(0, 0), grid1.getRegion(0, 0).getCell(0, 1), grid1.getRegion(0, 0).getCell(0, 2));
			TripleHolder middleRow1(grid1.getRegion(0, 0).getCell(1, 0), grid1.getRegion(0, 0).getCell(1, 1), grid1.getRegion(0, 0).getCell(1, 2));
			TripleHolder bottomRow1(grid1.getRegion(0, 0).getCell(2, 0), grid1.getRegion(0, 0).getCell(2, 1), grid1.getRegion(0, 0).getCell(2, 2));

			LastCellFinder lastCF(topRow1, middleRow1, bottomRow1);
			Assert::AreEqual(-1, grid1.getRegion(0, 0).getCell(1, 1).getValue());
			bool visited = lastCF.fill();
			Assert::IsTrue(visited);
			Assert::AreEqual(5, grid1.getRegion(0, 0).getCell(1, 1).getValue());
		}

		TEST_METHOD(OnlyOneChoiceRow1)
		{
			vector<string> stringInput = vector<string>(9);
		    /*grid:
			-23456789
			---------
			etc
			First cell should be filled with a 1 by OnlyOneChoiceInRowVisitor
			*/
			stringInput[0] = "-23------";
			stringInput[1] = "456------";
			stringInput[2] = "789------";
			for (unsigned char i = 3; i < 9; i++)
			{
				stringInput[i] = "---------"; 			//we don't care about the rest

			}
			Grid grid(stringInput);
			Assert::AreEqual(-1, grid.getRegion(0, 0).getCell(0, 0).getValue());

			OnlyOneChoiceInRowVisitor visitor;
			bool visited = grid.Accept(visitor);
			Assert::IsTrue(visited);
			Assert::AreEqual(1, grid.getRegion(0, 0).getCell(0, 0).getValue());
		}

		TEST_METHOD(OnlyOneChoiceRow2)
		{
			vector<string> stringInput = vector<string>(9);
			/*grid:
			Rows : -23456789
			First column should be filled with ones
			*/

			for (unsigned char i = 0; i < 3; i++)
			{
				stringInput[0+3*i] = "-23-23-23";
				stringInput[1+3*i] = "456456456";
				stringInput[2+3*i] = "789789789";
			}

			OnlyOneChoiceInRowVisitor visitor;
			Grid grid(stringInput);
			Assert::AreEqual(-1, grid.getRegion(0, 0).getCell(0, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(0, 0).getCell(1, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(0, 0).getCell(2, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(1, 0).getCell(0, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(1, 0).getCell(1, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(1, 0).getCell(2, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(2, 0).getCell(0, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(2, 0).getCell(1, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(2, 0).getCell(2, 0).getValue());
			bool visited = grid.Accept(visitor);
			Assert::IsTrue(visited);

			//First column of grid should have been filled with 1s
			Assert::AreEqual(1, grid.getRegion(0, 0).getCell(0, 0).getValue());
			Assert::AreEqual(1, grid.getRegion(0, 0).getCell(1, 0).getValue());
			Assert::AreEqual(1, grid.getRegion(0, 0).getCell(2, 0).getValue());
			Assert::AreEqual(1, grid.getRegion(1, 0).getCell(0, 0).getValue());
			Assert::AreEqual(1, grid.getRegion(1, 0).getCell(1, 0).getValue());
			Assert::AreEqual(1, grid.getRegion(1, 0).getCell(2, 0).getValue());
			Assert::AreEqual(1, grid.getRegion(2, 0).getCell(0, 0).getValue());
			Assert::AreEqual(1, grid.getRegion(2, 0).getCell(1, 0).getValue());
			Assert::AreEqual(1, grid.getRegion(2, 0).getCell(2, 0).getValue());
		}

		TEST_METHOD(OnlyOneChoiceColumn)
		{
			vector<string> stringInput = vector<string>(9);
			/*grid:
			Columns : -23456789
			First row should be filled with ones
			*/
			stringInput[0] = "---222333";
			stringInput[1] = "---222333";
			stringInput[2] = "---222333";
			stringInput[3] = "444555666";
			stringInput[4] = "444555666";
			stringInput[5] = "444555666";
			stringInput[6] = "777888999";
			stringInput[7] = "777888999";
			stringInput[8] = "777888999";

			OnlyOneChoiceInColumnVisitor visitor;
			Grid grid(stringInput);
			Assert::AreEqual(-1, grid.getRegion(0, 0).getCell(0, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(0, 0).getCell(0, 1).getValue());
			Assert::AreEqual(-1, grid.getRegion(0, 0).getCell(0, 2).getValue());
			Assert::AreEqual(-1, grid.getRegion(0, 1).getCell(0, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(0, 1).getCell(0, 1).getValue());
			Assert::AreEqual(-1, grid.getRegion(0, 1).getCell(0, 2).getValue());
			Assert::AreEqual(-1, grid.getRegion(0, 2).getCell(0, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(0, 2).getCell(0, 1).getValue());
			Assert::AreEqual(-1, grid.getRegion(0, 2).getCell(0, 2).getValue());
			bool visited = grid.Accept(visitor);
			Assert::IsTrue(visited);

			//First column of grid should have been filled with 1s
			Assert::AreEqual(1, grid.getRegion(0, 0).getCell(0, 0).getValue());
			Assert::AreEqual(1, grid.getRegion(0, 0).getCell(0, 1).getValue());
			Assert::AreEqual(1, grid.getRegion(0, 0).getCell(0, 2).getValue());
			Assert::AreEqual(1, grid.getRegion(0, 1).getCell(0, 0).getValue());
			Assert::AreEqual(1, grid.getRegion(0, 1).getCell(0, 1).getValue());
			Assert::AreEqual(1, grid.getRegion(0, 1).getCell(0, 2).getValue());
			Assert::AreEqual(1, grid.getRegion(0, 2).getCell(0, 0).getValue());
			Assert::AreEqual(1, grid.getRegion(0, 2).getCell(0, 1).getValue());
			Assert::AreEqual(1, grid.getRegion(0, 2).getCell(0, 2).getValue());
		}

		TEST_METHOD(OnlyOneChoiceRegion)
		{
			vector<string> stringInput = vector<string>(9);
			/*grid:
			Regions : 123456-89
			7th cells should be filled with 7s
			*/
			for (unsigned char i = 0; i < 9; i++)
			{
				stringInput[i] = "123456-89";
			}

			OnlyOneChoiceInRegionVisitor visitor;
			Grid grid(stringInput);
			Assert::AreEqual(-1, grid.getRegion(0, 0).getCell(2, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(0, 1).getCell(2, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(0, 2).getCell(2, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(1, 0).getCell(2, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(1, 1).getCell(2, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(1, 2).getCell(2, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(2, 0).getCell(2, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(2, 1).getCell(2, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(2, 2).getCell(2, 0).getValue());
			bool visited = grid.Accept(visitor);
			Assert::IsTrue(visited);

			//Cells should have been filled with 7s
			Assert::AreEqual(7, grid.getRegion(0, 0).getCell(2, 0).getValue());
			Assert::AreEqual(7, grid.getRegion(0, 1).getCell(2, 0).getValue());
			Assert::AreEqual(7, grid.getRegion(0, 2).getCell(2, 0).getValue());
			Assert::AreEqual(7, grid.getRegion(1, 0).getCell(2, 0).getValue());
			Assert::AreEqual(7, grid.getRegion(1, 1).getCell(2, 0).getValue());
			Assert::AreEqual(7, grid.getRegion(1, 2).getCell(2, 0).getValue());
			Assert::AreEqual(7, grid.getRegion(2, 0).getCell(2, 0).getValue());
			Assert::AreEqual(7, grid.getRegion(2, 1).getCell(2, 0).getValue());
			Assert::AreEqual(7, grid.getRegion(2, 2).getCell(2, 0).getValue());
		}

		TEST_METHOD(NineHolderForGrid)
		{
			vector<string> stringInput = vector<string>(9);
			/*grid:
			3 first rows: 123456789
			*/

			stringInput[0] = "123123123";
			stringInput[1] = "456456456";
			stringInput[2] = "789789789";
			for (unsigned char i = 3; i < 9; i++)
			{
				stringInput[i] = "123456789";
			}
			Grid grid(stringInput);
			NineHolder nh = grid.getRow(0);
			for (unsigned char i = 0; i < 9; i++){
				Assert::AreEqual(i + 1, nh.getCell(i).getValue());
			}
		}

		TEST_METHOD(NineHolderIsValuePresent)
		{
			vector<string> stringInput = vector<string>(9);
			/*grid:
			First row: 157------
			*/

			stringInput[0] = "157123123";
			stringInput[1] = "---456456";
			stringInput[2] = "---789789";
			for (unsigned char i = 3; i < 9; i++)
			{
				stringInput[i] = "123456789";
			}
			Grid grid(stringInput);
			NineHolder nh = grid.getRow(0);
			Assert::IsTrue(nh.isValuePresent(1));
			Assert::IsTrue(nh.isValuePresent(5));
			Assert::IsTrue(nh.isValuePresent(7));
			Assert::IsFalse(nh.isValuePresent(2));
			Assert::IsFalse(nh.isValuePresent(3));
			Assert::IsFalse(nh.isValuePresent(4));
			Assert::IsFalse(nh.isValuePresent(6));
			Assert::IsFalse(nh.isValuePresent(8));
			Assert::IsFalse(nh.isValuePresent(9));
		}

		TEST_METHOD(OnlySquareOnRowNoRegion)
		{
			vector<string> stringInput = vector<string>(9);
			// grid taken from SudokuDragon tutorial for OnlySquare
			stringInput[0] = "2619-87--";
			stringInput[1] = "495176832";
			stringInput[2] = "873-45--6";
			stringInput[3] = "12638-4--";
			stringInput[4] = "987654321";
			stringInput[5] = "--4-27689";
			stringInput[6] = "5--69-873";
			stringInput[7] = "763518249";
			stringInput[8] = "--87-2561";

			Grid grid(stringInput);
			OnlySquareVisitor visitor;
			Assert::AreEqual(-1, grid.getRegion(0, 0).getCell(1, 1).getValue());
			Assert::AreEqual(-1, grid.getRegion(0, 2).getCell(1, 0).getValue());
			bool visited = grid.Accept(visitor);
			Assert::IsTrue(visited);
			Assert::AreEqual(3, grid.getRegion(0, 0).getCell(1, 1).getValue());
			Assert::AreEqual(2, grid.getRegion(0, 2).getCell(1, 0).getValue());
		}

		TEST_METHOD(OnlySquareOnColumnNoRegion)
		{
			vector<string> stringInput = vector<string>(9);
			// grid is the previous one transposed
			stringInput[0] = "2976--18-";
			stringInput[1] = "13428-6--";
			stringInput[2] = "568-97--3";
			stringInput[3] = "418973562";
			stringInput[4] = "963852741";
			stringInput[5] = "752614389";
			stringInput[6] = "8--74-356";
			stringInput[7] = "--6-28479";
			stringInput[8] = "-75--6821";

			Grid grid(stringInput);
			OnlySquareVisitor visitor;
			Assert::AreEqual(-1, grid.getRegion(0, 0).getCell(1, 1).getValue());
			Assert::AreEqual(-1, grid.getRegion(2, 0).getCell(0, 1).getValue());
			bool visited = grid.Accept(visitor);
			Assert::IsTrue(visited);
			Assert::AreEqual(3, grid.getRegion(0, 0).getCell(1, 1).getValue());
			Assert::AreEqual(2, grid.getRegion(2, 0).getCell(0, 1).getValue());
		}


		TEST_METHOD(OnlySquareOnRow2)
		{
			vector<string> stringInput = vector<string>(9);
			// grid taken from SudokuDragon: http://www.sudokudragon.com/tutorialgentle2.htm
			stringInput[0] = "38-6-5-1-";
			stringInput[1] = "-96---345";
			stringInput[2] = "---1--6--";
			stringInput[3] = "5-8-26-9-";
			stringInput[4] = "---587---";
			stringInput[5] = "-1-93-8-5";
			stringInput[6] = "--1--4---";
			stringInput[7] = "754---96-";
			stringInput[8] = "-8-7-2-51";

			Grid grid(stringInput);
			OnlySquareVisitor visitor;
			Assert::AreEqual(-1, grid.getRegion(1, 0).getCell(1, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(1, 2).getCell(1, 2).getValue());
			bool visited = grid.Accept(visitor);
			Assert::IsTrue(visited);
			Assert::AreEqual(1, grid.getRegion(1, 0).getCell(1, 0).getValue());
			Assert::AreEqual(4, grid.getRegion(1, 2).getCell(1, 2).getValue());
		}


		TEST_METHOD(OnlySquareRegion)
		{
			vector<string> stringInput = vector<string>(9);
			// grid taken from SudokuDragon tutorial for OnlySquare
			stringInput[0] = "2619-87--";
			stringInput[1] = "495176832";
			stringInput[2] = "873-45--6";
			stringInput[3] = "1-638-4-7";
			stringInput[4] = "987654321";
			stringInput[5] = "--4-27689";
			stringInput[6] = "5--69-873";
			stringInput[7] = "763518249";
			stringInput[8] = "--87-2561";

			Grid grid(stringInput);
			OnlySquareVisitor visitor;
			Assert::AreEqual(-1, grid.getRegion(0, 0).getCell(1, 1).getValue());
			Assert::AreEqual(-1, grid.getRegion(0, 2).getCell(1, 0).getValue());
			bool visited = grid.Accept(visitor);
			Assert::IsTrue(visited);
			Assert::AreEqual(3, grid.getRegion(0, 0).getCell(1, 1).getValue());
			Assert::AreEqual(2, grid.getRegion(0, 2).getCell(1, 0).getValue());
		}

		TEST_METHOD(TwoOutOfThreeRows)
		{
			vector<string> stringInput = vector<string>(9);
			
			stringInput[0] = "----6-9--";
			stringInput[1] = "17----4-8";
			stringInput[2] = "---81---2";
			stringInput[3] = "--1------";
			stringInput[4] = "---------";
			stringInput[5] = "---------";
			stringInput[6] = "---------";
			stringInput[7] = "---------";
			stringInput[8] = "---------";

			Grid grid(stringInput);
			TwoOutOfThreeRowVisitor visitor;
			Assert::AreEqual(-1, grid.getRegion(0, 0).getCell(2, 1).getValue());
			bool visited = grid.Accept(visitor);
			Assert::IsTrue(visited);
			Assert::AreEqual(1, grid.getRegion(0, 0).getCell(2, 1).getValue());
		}

		TEST_METHOD(TwoOutOfThreeColumns)
		{
			vector<string> stringInput = vector<string>(9);
			/*Grid:
			4--------
			---5-----
			---Y-----    Y must be filled with a 4
			----4----
			---------
			---------
			---------
			-----4---
			---------
			*/
			stringInput[0] = "4--------";
			stringInput[1] = "---5-----";
			stringInput[2] = "---------";
			stringInput[3] = "---------";
			stringInput[4] = "-4-------";
			stringInput[5] = "---------";
			stringInput[6] = "---------";
			stringInput[7] = "-----4---";
			stringInput[8] = "---------";

			Grid grid(stringInput);
			TwoOutOfThreeColumnVisitor visitor;
			Assert::AreEqual(-1, grid.getRegion(0, 1).getCell(2, 0).getValue());
			bool visited = grid.Accept(visitor);
			Assert::IsTrue(visited);
			Assert::AreEqual(4, grid.getRegion(0, 1).getCell(2, 0).getValue());
		}


		TEST_METHOD(RowColumnRegionVis)
		{
			vector<string> stringInput = vector<string>(9);
			//grid taken from http://www.sudokudragon.com/tutorialgentle2.htm
			stringInput[0] = "38-6-5-1-";
			stringInput[1] = "-96-7-345"; //I added the 7 here
			stringInput[2] = "---1--6--";
			stringInput[3] = "5-8-26-9-";
			stringInput[4] = "---587---";
			stringInput[5] = "-1-93-8-5";
			stringInput[6] = "--1--4---";
			stringInput[7] = "754---96-";
			stringInput[8] = "-8-7-2-51";

			Grid grid(stringInput);
			RowColumnRegionVisitor visitor;
			Assert::AreEqual(-1, grid.getRegion(0, 0).getCell(1, 1).getValue());
			bool visited = grid.Accept(visitor);
			Assert::IsTrue(visited);
			Assert::AreEqual(4, grid.getRegion(0, 0).getCell(1, 1).getValue());
		}

		TEST_METHOD(NineHolderRowConsistent)
		{
			vector<string> stringInput = vector<string>(9);
			/*grid:
			3 first rows: 123456789
			Then, not consistent
			*/

			stringInput[0] = "123123123";
			stringInput[1] = "456456456";
			stringInput[2] = "789789789";
			for (unsigned char i = 3; i < 9; i++)
			{
				stringInput[i] = "123456789";
			}
			Grid grid(stringInput);
			NineHolder firstRow = grid.getRow(0);
			NineHolder fourthRow = grid.getRow(3);
			Assert::IsTrue(firstRow.isConsistent());
			Assert::IsFalse(fourthRow.isConsistent());
		}

		TEST_METHOD(RegionHolderConsistent)
		{
			vector<string> stringInput = vector<string>(9);
			/*grid:
			3 first regions not consistent
			Then, 123456789
			*/

			stringInput[0] = "123123123";
			stringInput[1] = "456456456";
			stringInput[2] = "789789789";
			for (unsigned char i = 3; i < 9; i++)
			{
				stringInput[i] = "123456789";
			}
			Grid grid(stringInput);
			RegionHolder firstReg = grid.getRegion(0, 0);
			RegionHolder secondReg = grid.getRegion(1, 2);
			Assert::IsFalse(firstReg.isConsistent());
			Assert::IsTrue(secondReg.isConsistent());
		}

		TEST_METHOD(GridNotConsistent)
		{
			vector<string> stringInput = vector<string>(9);
			/*grid:
			3 first regions not consistent
			Then, 123456789
			*/

			stringInput[0] = "123123123";
			stringInput[1] = "456456456";
			stringInput[2] = "789789789";
			for (unsigned char i = 3; i < 9; i++)
			{
				stringInput[i] = "123456789";
			}
			Grid grid(stringInput);
			Assert::IsFalse(grid.isConsistent());
		}

		TEST_METHOD(FullGridConsistent)
		{
			vector<string> stringInput = vector<string>(9);
			// grid correctly filled
			stringInput[0] = "418526379";
			stringInput[1] = "592347681";
			stringInput[2] = "673981452";
			stringInput[3] = "782963154";
			stringInput[4] = "416725938";
			stringInput[5] = "395148726";
			stringInput[6] = "247891635";
			stringInput[7] = "153264879";
			stringInput[8] = "869537214";

			Grid grid(stringInput);
			Assert::IsTrue(grid.isConsistent());
		}

		TEST_METHOD(FullGridNotConsistent)
		{
			vector<string> stringInput = vector<string>(9);
			// grid not correctly filled (two 4s in region 6)
			stringInput[0] = "418526379";
			stringInput[1] = "592347681";
			stringInput[2] = "673981452";
			stringInput[3] = "782963154";
			stringInput[4] = "416725938";
			stringInput[5] = "395148726";
			stringInput[6] = "247894635";
			stringInput[7] = "153264879";
			stringInput[8] = "869537214";

			Grid grid(stringInput);
			Assert::IsFalse(grid.isConsistent());
		}

		TEST_METHOD(HolderConsistentWithEmptyCells)
		{
			vector<string> stringInput = vector<string>(9);
			stringInput[0] = "41-5263-9";
			stringInput[1] = "592347681";
			stringInput[2] = "673981452";
			stringInput[3] = "78296-154";
			stringInput[4] = "416725938";
			stringInput[5] = "39-148726";
			stringInput[6] = "247891635";
			stringInput[7] = "15--64879";
			stringInput[8] = "-6953-14-";

			Grid grid(stringInput);
			NineHolder fullRow = grid.getRow(0);
			RegionHolder fullReg = grid.getRegion(2, 1);
			Assert::IsTrue(fullRow.isConsistent());
			Assert::IsTrue(fullReg.isConsistent());
		}

		TEST_METHOD(GridConsistentWithEmptyCells)
		{
			vector<string> stringInput = vector<string>(9);
			stringInput[0] = "41-526379";
			stringInput[1] = "59-347681";
			stringInput[2] = "67-981452";
			stringInput[3] = "7829--154";
			stringInput[4] = "416-25938";
			stringInput[5] = "395148726";
			stringInput[6] = "247-91635";
			stringInput[7] = "15326--79";
			stringInput[8] = "86953721-";

			Grid grid(stringInput);
			Assert::IsTrue(grid.isConsistent());
		}

		TEST_METHOD(GridIsFull1)
		{
			vector<string> stringInput = vector<string>(9);
			// grid fully filled

			stringInput[0] = "418526379";
			stringInput[1] = "592347681";
			stringInput[2] = "673981452";
			stringInput[3] = "782963154";
			stringInput[4] = "416725938";
			stringInput[5] = "395148726";
			stringInput[6] = "247891635";
			stringInput[7] = "153264879";
			stringInput[8] = "869537214";

			Grid grid(stringInput);
			Assert::IsTrue(grid.isFull());
		}

		TEST_METHOD(GridIsFull2)
		{
			vector<string> stringInput = vector<string>(9);
			// grid not fully filled
			stringInput[0] = "418526379";
			stringInput[1] = "592347681";
			stringInput[2] = "673981452";
			stringInput[3] = "782963154";
			stringInput[4] = "416725938";
			stringInput[5] = "395148726";
			stringInput[6] = "247891635";
			stringInput[7] = "153264879";
			stringInput[8] = "86953721-";

			Grid grid(stringInput);
			Assert::IsFalse(grid.isFull());
		}

		TEST_METHOD(OnlyOneChoiceGlobal)
		{
			vector<string> stringInput = vector<string>(9);
			/*grid:
			Regions : 123456-89
			7th cell should be filled with 7s
			*/
			for (unsigned char i = 0; i < 9; i++)
			{
				stringInput[i] = "123456-89";
			}

			OnlyOneChoiceGlobalVisitor visitor;
			Grid grid(stringInput);
			Assert::AreEqual(-1, grid.getRegion(0, 0).getCell(2, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(0, 1).getCell(2, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(0, 2).getCell(2, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(1, 0).getCell(2, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(1, 1).getCell(2, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(1, 2).getCell(2, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(2, 0).getCell(2, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(2, 1).getCell(2, 0).getValue());
			Assert::AreEqual(-1, grid.getRegion(2, 2).getCell(2, 0).getValue());
			bool visited = grid.Accept(visitor);
			Assert::IsTrue(visited);

			//Cells should have been filled with 7s
			Assert::AreEqual(7, grid.getRegion(0, 0).getCell(2, 0).getValue());
			Assert::AreEqual(7, grid.getRegion(0, 1).getCell(2, 0).getValue());
			Assert::AreEqual(7, grid.getRegion(0, 2).getCell(2, 0).getValue());
			Assert::AreEqual(7, grid.getRegion(1, 0).getCell(2, 0).getValue());
			Assert::AreEqual(7, grid.getRegion(1, 1).getCell(2, 0).getValue());
			Assert::AreEqual(7, grid.getRegion(1, 2).getCell(2, 0).getValue());
			Assert::AreEqual(7, grid.getRegion(2, 0).getCell(2, 0).getValue());
			Assert::AreEqual(7, grid.getRegion(2, 1).getCell(2, 0).getValue());
			Assert::AreEqual(7, grid.getRegion(2, 2).getCell(2, 0).getValue());
		}

		TEST_METHOD(SolveEasyGrid1)
		{
			vector<string> stringInput = vector<string>(9);
			// one empty cell per region
			stringInput[0] = "418-26379";
			stringInput[1] = "59234-681";
			stringInput[2] = "-73981452";
			stringInput[3] = "-82963154";
			stringInput[4] = "41672593-";
			stringInput[5] = "39514-726";
			stringInput[6] = "24-891635";
			stringInput[7] = "1532-4879";
			stringInput[8] = "86953721-";

			Grid grid(stringInput);
			grid.SolveWithEasyStrategies();
			Assert::AreEqual(5, grid.getRegion(0, 0).getCell(1, 0).getValue());
			Assert::AreEqual(7, grid.getRegion(0, 1).getCell(1, 2).getValue());
			Assert::AreEqual(6, grid.getRegion(0, 2).getCell(0, 0).getValue());
			Assert::AreEqual(7, grid.getRegion(1, 0).getCell(0, 0).getValue());
			Assert::AreEqual(8, grid.getRegion(1, 1).getCell(2, 2).getValue());
			Assert::AreEqual(8, grid.getRegion(1, 2).getCell(1, 2).getValue());
			Assert::AreEqual(7, grid.getRegion(2, 0).getCell(0, 2).getValue());
			Assert::AreEqual(6, grid.getRegion(2, 1).getCell(1, 1).getValue());
			Assert::AreEqual(4, grid.getRegion(2, 2).getCell(2, 2).getValue());
		}

		TEST_METHOD(SolveEasyGrid2)
		{
			vector<string> stringInput = vector<string>(9);

			stringInput[0] = "418-26379";
			stringInput[1] = "59-34-681";
			stringInput[2] = "-73981452";
			stringInput[3] = "-82963154";
			stringInput[4] = "4167259--";
			stringInput[5] = "39514-726";
			stringInput[6] = "24-891635";
			stringInput[7] = "1532-4879";
			stringInput[8] = "-6953721-";

			Grid grid(stringInput);
			grid.SolveWithEasyStrategies();
			Assert::AreEqual(5, grid.getRegion(0, 0).getCell(1, 0).getValue());
			Assert::AreEqual(2, grid.getRegion(0, 1).getCell(0, 2).getValue());
			Assert::AreEqual(7, grid.getRegion(0, 1).getCell(1, 2).getValue());
			Assert::AreEqual(6, grid.getRegion(0, 2).getCell(0, 0).getValue());
			Assert::AreEqual(7, grid.getRegion(1, 0).getCell(0, 0).getValue());
			Assert::AreEqual(8, grid.getRegion(1, 1).getCell(2, 2).getValue());
			Assert::AreEqual(3, grid.getRegion(1, 1).getCell(2, 1).getValue());
			Assert::AreEqual(8, grid.getRegion(1, 2).getCell(1, 2).getValue());
			Assert::AreEqual(7, grid.getRegion(2, 0).getCell(0, 2).getValue());
			Assert::AreEqual(6, grid.getRegion(2, 1).getCell(1, 1).getValue());
			Assert::AreEqual(4, grid.getRegion(2, 2).getCell(2, 2).getValue());
			Assert::AreEqual(8, grid.getRegion(2, 2).getCell(0, 0).getValue());
		}


		TEST_METHOD(SolveMediumGridPart3)
		{
			vector<string> stringInput = vector<string>(9);
			// grid taken from assignment
			stringInput[0] = "-----6--9";
			stringInput[1] = "--23----1";
			stringInput[2] = "6-3--1-52";
			stringInput[3] = "782----5-";
			stringInput[4] = "---------";
			stringInput[5] = "-9----726";
			stringInput[6] = "24-8--6-5";
			stringInput[7] = "1----48--";
			stringInput[8] = "8--5-----";

			Grid grid(stringInput);
			grid.SolveWithEasyStrategies();
			Assert::IsTrue(grid.isFull());
			Assert::IsTrue(grid.isConsistent());
		}


		TEST_METHOD(SolveMediumGridPart3_corrected)
		{
			vector<string> stringInput = vector<string>(9);
			//grid taken from http://www.sudokudragon.com/tutorialgentle2.htm
			stringInput[0] = "38-6-5-1-";
			stringInput[1] = "-96---345";
			stringInput[2] = "---1--6--";
			stringInput[3] = "5-8-26-9-";
			stringInput[4] = "---587---";
			stringInput[5] = "-1-93-8-5";
			stringInput[6] = "--1--4---";
			stringInput[7] = "754---96-";
			stringInput[8] = "-8-7-2-51";

			Grid grid(stringInput);
			grid.SolveWithEasyStrategies();
			Assert::IsTrue(grid.isFull());
			Assert::IsTrue(grid.isConsistent());
		}

		TEST_METHOD(GetPossibleValuesForACell)
		{
			vector<string> stringInput = vector<string>(9);
			stringInput[0] = "8-5-3-4-6";
			stringInput[1] = "---9---3-";
			stringInput[2] = "-3-------";
			stringInput[3] = "6---5---9";
			stringInput[4] = "-1-3-8-4-";
			stringInput[5] = "9---7---1";
			stringInput[6] = "-------7-";
			stringInput[7] = "-2---9---";
			stringInput[8] = "3-8-2-5-4";

			Grid grid(stringInput);
			set<unsigned char> possibleVal = grid.getPossibleValues(0, 4);
			int nbPossVal = possibleVal.size();
			set<unsigned char>::iterator iter = possibleVal.begin();
			int firstValue = *iter;
			iter++;
			int secondValue = *iter;
			Assert::AreEqual(2, nbPossVal);
			Assert::AreEqual(6, firstValue);
			Assert::AreEqual(7,secondValue);
		}

		TEST_METHOD(GetIndicesCellWithLessChoices)
		{
			vector<string> stringInput = vector<string>(9);
			stringInput[0] = "8-5-3-4-6";
			stringInput[1] = "---9---3-";
			stringInput[2] = "-3-------";
			stringInput[3] = "6---5---9";
			stringInput[4] = "-1-3-8-4-";
			stringInput[5] = "9---7---1";
			stringInput[6] = "-------7-";
			stringInput[7] = "-2---9---";
			stringInput[8] = "3-8-2-5-4";

			Grid grid(stringInput);
			vector<unsigned char> bestIndices = grid.getIndicesCellWithLessChoices();
			int rowIdx = bestIndices[0];
			int colIdx = bestIndices[1];
			Assert::AreEqual(0, rowIdx);
			Assert::AreEqual(4, colIdx);
		}

		TEST_METHOD(SolveDiabolicalGridPart4_1)
		{
			vector<string> stringInput = vector<string>(9);
			//own grid such that easy strategies do not work, but ok with hypothesis
			stringInput[0] = "8-5-3-4-6";
			stringInput[1] = "---9---3-";
			stringInput[2] = "-3-------";
			stringInput[3] = "6---5---9";
			stringInput[4] = "-1-3-8-4-";
			stringInput[5] = "9---7---1";
			stringInput[6] = "-------7-";
			stringInput[7] = "-2---9---";
			stringInput[8] = "3-8-2-5-4";

			Grid grid(stringInput);
			grid.SolveWithEasyStrategies();
			Assert::IsFalse(grid.isFull()); //grid too complicated without hypothesis
			Assert::IsTrue(grid.isConsistent());
			grid.Solve();
			Assert::IsTrue(grid.isFull());
			Assert::IsTrue(grid.isConsistent());
		}


		TEST_METHOD(SolveDiabolicalGridPart4)
		{
			vector<string> stringInput = vector<string>(9);
			//Grid taken from subject
			stringInput[0] = "-54--16--";
			stringInput[1] = "-7--9-5--";
			stringInput[2] = "---3--2--";
			stringInput[3] = "---------";
			stringInput[4] = "36---84--";
			stringInput[5] = "1-46---3-";
			stringInput[6] = "-8--36--5";
			stringInput[7] = "9-----2--";
			stringInput[8] = "--6--14--";

			Grid grid(stringInput);
			grid.SolveWithEasyStrategies();
			Assert::IsFalse(grid.isFull()); //grid too complicated without hypothesis
			Assert::IsTrue(grid.isConsistent());
			//grid.Solve();
			//This grid is in fact NOT solvable, even with assumptions.
			//Assert::IsTrue(grid.isConsistent());
			//Assert::IsTrue(grid.isFull());
		}
	};
}