#include "arrayUtilities.h"
#include <cstdlib>
#include <string>
#include <cstring>
#include <memory>
#include <iostream>
#include <fstream>
#include <cstddef>
#include <ctype.h>

using namespace std;


/**
* @brief	Calculate the number of days in the given month and year.
*
* A leap year can be calculated with the following set of rules
* if( (year mod 4 == 0) AND (year mod 100 != 0) OR (year mod 400 == 0))
*		set true
*	else set false
*
* @param year represents the year an integer value.
* @param month represents the month as an integer value, 
*     where a value of zero indicates January, 
*     a value of one February, and so forth.
*
* @return the calculated number of days
*/
int numDaysInMonth(int year, int month)
{

	bool isLeap = false;

	if( (year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0))
		isLeap = true;
	else 
	{
		isLeap = false;
	}
	
	int leap[12];
	leap[0] = 31; 
	leap[1] = 28;
	leap[2] = 31;
	leap[3] = 30;
	leap[4] = 31;
	leap[5] = 30;
	leap[6] = 31;
	leap[7] = 31;
	leap[8] = 30;
	leap[9] = 31;
	leap[10] = 30;
	leap[11] = 31;

	int days;
	if(isLeap == true && month == 1)
	{
		days = 29;
		return days;
	}

	else
	{
		days = leap[month];		
		return days;
	}
	
}


/**
* @brief	Calculate all the month's total number of days for the year.
*
* int numDaysInMonth(int year, int month) 
* may be used to calculate this information.
*
* @return an one dimensional array with 12 cells - one for each month. 
* Each cell should contain the amount of days of a specific month. 
* The first cell represents January's number of days, 
* the second cell repersents Februry'snumber of dats, etc.
*/
size_t* yearsMonthSize(int year)
{
	size_t *numOfDays = new size_t[12];

	for(int k =0; k<12; k++)
	{
			size_t temp = 0;
			
			numOfDays[k] = static_cast<size_t>(numDaysInMonth(year,k));
	}

	
	return numOfDays;


}


/**
* @brief	convert the string in the form YYYY/MM/DD, to integer values.
*
* @param	stampString is the string to be converted.
* @param	stampArray contains the converted information, 
* 			index 0 represents the year, 1 the month, and 2 the day.
*
* NOTE: stampArray must store -1 at the indexes where the conversion failed.
*/
void parseDateStamp(std::string stampString, int *stampArray[3])
{
	int static nyear, nmonth, nday;
	
	string year(stampString, 0, 4);
	string month(stampString, 5, 2);
	string day(stampString, 8, 2);

	int static invalid = -1;
	
	if (nyear = atoi(year.c_str()))
	{
		stampArray[0] = &nyear;
	}

	else
	{
		stampArray[0] = &invalid;
	}
	
	if (nmonth = atoi(month.c_str()))
	{
		stampArray[1] = &nmonth;
	}

	else
	{
		stampArray[1] = &invalid;
	}
	
	if (nday = atoi(day.c_str()))
	{
		stampArray[2] = &nday;

	}
	else
	{

		stampArray[2] = &invalid;
	}
}


/**
* @brief	Parse a file containing the rainfall data into a 3D array
*
* @details This function must parse the opened file 
* and read the values to dataPtr.
*
*	The data in the file is represented by a timestamp paired with a value. 
*	The value indicates the amount of rain for that day in millimetres.
*	The value is separated with a tab. 
*	Each timestamp value pair is on a new line. 
*
*	It is assumed that all timestamps will follow consecutively,
*	however, the apparatus taking these values may sometimes fail. 
*	The absence of an entry for a day indicates that the apparatus 
*	has failed on that specific day.  The value of -1 must be stored 
*	in dataPtr to indicate the absence of that data.
*	
*	dataPtr must be able to grow dynamically, with the help of the relevant
*	array functions. dataPtr may only grow with one extra year element 
*	at a time. This growth must be triggered if the function detects a new 
*	year in a timestamp.
*
*	The month-dimension's size will always be 12. The day-dimension's 
*	size must be adapted according to the number of days in the month.
*		
*	The use of the following functions are recommended:
*		void reallocShort3d(short ***&ptr, size_t oldSize, size_t newSize) 
*		short ** newShort2d(size_t numRow, size_t *colSize) 
*		size_t* yearsMonthSize(int year)
*
* @param filename is the file to be opened.
*
* @param dataPtr is the data-structure that will contain all read in values. 
* Note that dataPtr is a referance to a 3D array.
* The dimensions can be described as [year][month][day].
*
* @param firstYear will store the value of the first year read from the file.
*
* @return the number of years read in from the file. -1 on file open error.
*/
size_t parseDataFile(std::string const & filename, short ***&dataPtr, int &firstYear)
{
	fstream Inputfile;
	Inputfile.open(filename, ios::in);

	int month, year, day;

	size_t numOfYears = 0;
	size_t PrevYear = 0;
	bool firstRun = true;
	size_t months = 12;

	if(Inputfile.fail())
	{
		return -1;
	}

	else
	{
		while(!Inputfile.eof())
		{
			
			string stampString = " ";

			//for millimeter read
			short rain = -1;

			//read timestamp
			getline(Inputfile,stampString,'-');

			string buffer = " ";

			//Get rid of time
			getline(Inputfile,buffer,'\t');


			//read millimeter
			Inputfile >> rain;

			//Reading is over, now time for processing:

				

			if(isdigit(stampString[0]))
			{	

				int **stampArray = new int *[3];

				for(int x = 0; x<3; x++)
				{
					stampArray[x] = 0;				
				}

				parseDateStamp(stampString,stampArray);

				 year = *(stampArray[0]);
				 month = *(stampArray[1]);
				 day = *(stampArray[2]);

				if (firstRun)
				{
					firstYear = year;
					
					PrevYear = firstYear -1;

					reallocShort3d(dataPtr,(numOfYears),numOfYears+1);
					numOfYears++;

					size_t *numOfDays = yearsMonthSize(year);

					dataPtr[numOfYears-1] = newShort2d(12,numOfDays);


					//Set all to -1 in case a failiure occurs

					for(int row = 0; row<12; row++)
					{
						for(int col = 0; col < numOfDays[row];col++)
						{
							dataPtr[numOfYears-1][row][col] = -1;
						}
					}

					delete [] numOfDays;
					
					firstRun = false;
					PrevYear++;
				}

				else if (year == PrevYear+1)
				{
					PrevYear++;
					numOfYears++;

					reallocShort3d(dataPtr,(numOfYears-1),numOfYears);

					size_t *numOfDays = yearsMonthSize(year);

					dataPtr[numOfYears-1] = newShort2d(months,numOfDays);

					//set to -1 incase failiure occurs
					

					for(int row = 0; row<months; row++)
					{
						for(int col = 0; col < numOfDays[row];col++)
						{
							dataPtr[numOfYears-1][row][col] = -1;
						}
					}

					delete [] numOfDays;
					
				}

				size_t *numOfDays = yearsMonthSize(year);

				if((month<= months) && (month > 0) && (day>0) && (day <= numOfDays[month -1]))
				{
					short rrain = static_cast<short>(rain);
					dataPtr[numOfYears - 1][month - 1][day - 1] = rrain;
				}

				else
				{
					cout << "Error!! Incorrect information(date) submitted. The program will now exit!.\n"
					<< month << " " << day << " " << numOfDays[month-1] <<endl;
					return -1;
				}				
				
				delete [] numOfDays;
				numOfDays = 0;

				delete [] stampArray;
				stampArray = 0;
			}	
		
		}//eow		
		
	}//eoe

	Inputfile.close();
	return numOfYears;
}//end of function


/**
* @brief reallocates/resizes a 3D array of shorts to fit into a new size. 
* Size here refers to the first dimension. i.e. ptr[size][row][col].
*
* @note after the function executed, the original data in the array must 
* not be altered in any way. That is to say, only the first dimension of 
* ptr may be altered.
*
* @NOTE! this function must work in general and not specifically 
*	in the way it is used in parseDataFile
*
* @param ptr is the pointer that refers to the 3D array to be resized.
* @param oldSize is the current total number of elements in the first 
*		dimension of ptr. e.g.   ptr[this amount here][row][col]
* @param newSize is the new size that ptr will have after the function 
* has been executed.   ptr[the new amount here][row][col]
*/
void reallocShort3d(short ***&ptr, size_t oldSize, size_t newSize)
{
	short ***newArray = new short**[newSize];

	for(size_t q =0; q<oldSize; q++)
	{
		newArray[q] = ptr[q];
	}

	
 	ptr = newArray;
}


/**
* @brief creates a new varied length 2D array of shorts. 
* To clarify, the amount of columns in each row may vary
*		e.g. row one might look like this {1, 3, 5, 6, END}, with a total 
*		of four columns, where row two might look like this {1, 5, END} 
*		with a total of 2 columns, and row three, {4, 5, 7, END}
*				|__COL__
*				R[1][3][5][6]
*				O[1][5]
*				W[4][5][7]
*|
* @NOTE! this function must work in general and not specifically 
*	in the way it is used in parseDataFile

* @param numRow is the number of rows that the created array will have. 
*	In the example above, it is 3
* @param colSize is a one dimensional array that indicates the number 
*	of columns for each of the rows. In the example above, 
*	the array would look like this: {4, 2, 3}
*
* @return will return the 2D pointer that was created.
*/
short ** newShort2d(size_t numRow, size_t const *colSize)
{
	short **array = new short* [numRow];

	for(size_t  x = 0; x < numRow; x++)
	{
		array[x] = new short [colSize[x]];
	}

	return array;
}


/*
* @brief This function will delete all the elements af a given 2D array.
*
* @param ptr is the pointer, pointing to the data that is to be deleted. 
*       this pointer must be set to zero after the deletion process
* @param rowSize is the total rows in the array, i.g. ptr[row][col]. 
*       The function must loop through every row to delete its columns.
*/
void deleteShort2d(short **&ptr, size_t rowSize)
{
	for(size_t y= 0; y < rowSize; y++)
	{
		delete [] ptr[y];		
	}

	delete [] ptr;

	ptr = 0;
}