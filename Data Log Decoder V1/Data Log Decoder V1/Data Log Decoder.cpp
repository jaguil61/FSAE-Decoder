/***************************
* SAE UIC MOTORSPORTS TEAM *
*              *************
*ALPHA VERSION *
***************/

/*This Program decodes CAN codes that are passed into it.
1.) First we will pass the data into a two dimensional vector and then "if"/"if else" statements will decode the data.
		(Before moving onto next step make sure everything can decoded)
			(In this version I used arrays to test the functions that will decode the data)
2.) After the program is able to decode the CAN codes then a GUI will be devolped with graphs.
		Check out: https://www.qcustomplot.com/ for graphing libraries.
3.) Try and keep the main as simple as possible. This means you'll need to call functions (Maybe make other headerfiles?)
4.) Once everything is coded create a .exe file so that the program can be used without the need of Visual Studio

THINGS TO IMPROVE/CHANGE
1.) Change the output to print out in tables (Potential multiple for loops for each column
2.) The variable names such as "engSpeed, throttlePerc, intAirTemp, etc..." could be changed to something like "engSpeedBit" to signify that they
	are a "byte" of data from the "CAN EL219.xlsx" file and not the actual value that we want.
3.) Change function names so they almost don't match the variable names. Change to to something like "engSpeedCalc"?
4.) Implement a "while" loop so that the user can choose another .csv file to decode after they're done with one. If they don't want to choose
	another file then quit the program.
5.) Remove functions that have scaling value of 1.0 (Not needed)
6.) Move the functions into another cpp file
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
using namespace std;


// FUNCTION PROTOTYPES
double timeCalc(double initTime, double tStamp); //Will calculate seconds 
double engineSpeedCalc(double columnC, double columnD); //Will calculate the engine's speed (RPM)
double throttleCalc(double columnE); //Will calculate the throttle percentage (%)
double vehicleSpeedCalc(double columnH); //Will calculate the vehicles speed (KPH)
double lambdaTwoCalc(double columnI); //Will calculate Lambda #2 (Lambda)
double batteryVoltCalc(double columnJ); //Will calculate the battery's voltage (V)
double manifoldPressCalc(double columnC, double columnD); //Will calculate the manifold absolute pressure
double oilPressureCalc(double columnE); //Will calculate the oil pressure
double brakeTempFLCalc(double columnG, double columnH); //Will calculate the brake temo (FL)
double accelXCalc(double columnC, double columnD); //Will calculate the acceleration (X axis)
double accelYCalc(double columnE, double columnF); //Will calculate the acceleration (Y axis)
double accelZCalc(double columnG, double columnH); //Will calculate the acceleration (Z axis)

//ACTUAL FUNCTIONS
double timeCalc(double initTime, double timeStamp) //Will calculate seconds 
{
	double actualTime; //Variable for the actual time that will get returned
	actualTime = (timeStamp - initTime)/10000; //Time intervals are 1/100th of a second

	return actualTime;
}

double engineSpeedCalc(double columnC, double columnD)  //Will determine the engine's speed (RPM)
{
	double RPM; //The engine speed
	double scaling = 0.3906241; //0.3906241 RPM/bit
	RPM = ((columnC * 256) + columnD) * scaling; //Calculates the RPM based on the bit in column C and D

	return RPM;
}//END OF engineSpeedCalc

double throttleCalc(double columnE) //Will determine the throttle percentage
{
	double perc; //The percentage that the throttle is open
	double scaling = 0.39062; //0.39062 %/bit
	perc = columnE * scaling; //Calculates the throttle percentage based on the bit in column E

	return perc;
}//END OF throttleCalc

double vehicleSpeedCalc(double columnH) //Will Calculate the vehicles speed
{
	double speed; //The vehicles speed
	double scaling = 1.6093203; //1.6093203 kph/bit
	speed = columnH * scaling; //Calculates the vehicles speed based on the bit in column H

	return speed;
}//END OF vehicleSpeedCalc

double lambdaTwoCalc(double columnI) //Will calculate Lambda #2 (Lambda)
{
	double lamb; //Lambda #2
	double scaling = 0.00390625; //0.00390625 Lambda/bit
	lamb = columnI * scaling; //Calculates Lambda #2 based on what's in column I

	return lamb;
}//END OF lambdaTwoCalc

double batteryVoltCalc(double columnJ) //Will calculate the battery's voltage (V)
{
	double volt; //Battery Voltage
	double scaling = 0.0628477; //0.0628477 V/bit
	volt = columnJ * scaling; //Calculates the battery's voltage based on what's in column J

	return volt;
}//END OF batteryVoltCalc

double manifoldPressCalc(double columnC, double columnD) //Will calculate the manifolds absolute pressure
{
	double manAbsoPres; //Manifold absolute pressure (MAP)
	double scaling = 0.1; //0.1 kPa/bit
	manAbsoPres = ((columnC * 256) + columnD) * scaling; //Calculates the Manifold absolute pressure based on what's in columns C and D

	return manAbsoPres;
}// END OF manifoldPressCalc

double oilPressureCalc(double columnE) //Will calculate the oil pressure
{
	double pressure;
	double scaling = 0.580151; //0.580151 PSIg/bit
	pressure = columnE * scaling;

	return pressure;
}//END OF oilPressureCalc

double brakeTempFLCalc(double columnG, double columnH) //Will calculate the brake temo (FL)
{
	double brakeTempFL;
	double scaling = 0.01220703; // 0.01220703 C/bit
	brakeTempFL = ((columnG * 256) + columnH) * scaling;

	return brakeTempFL;
}

double accelXCalc(double columnC, double columnD) //Will calculate the acceleration (X axis)
{
	double accelX;
	double accelXRaw;
	double scaling = 0.001; //0.001 (G)/bit
	
	accelXRaw = (columnC * 256) + columnD;

	if (accelXRaw > 32768)
	{
		accelXRaw = accelXRaw - 65537;
	}

	accelX = (accelXRaw) * scaling;

	return accelX;
}

double accelYCalc(double columnE, double columnF) //Will calculate the acceleration (Y axis)
{
	double accelY;
	double accelYRaw;
	double scaling = 0.001; //0.001 (G)/bit
	
	accelYRaw = (columnE * 256) + columnF;

	//Converts a unsigned 16 bit to signed 16 bit
	if (accelYRaw > 32768)
	{
		accelYRaw = accelYRaw - 65537;
	}

	accelY = (accelYRaw)*scaling;

	return accelY;
}

double accelZCalc(double columnG, double columnH) //Will calculate the acceleration (Z axis)
{
	double accelZ;
	double accelZRaw;
	double scaling = 0.001; //0.001 (G)/bit

	accelZRaw = (columnG * 256) + columnH;

	if (accelZRaw > 32768)
	{
		accelZRaw = accelZRaw - 65537;
	}

	accelZ = (accelZRaw)*scaling;

	return accelZ;
}

//Struct that will be used to hold the data from the csv files columns
typedef struct _logData
{
	double timeStamp; //The time stamp in column A
	double table; //The table we'll shoose from

	//The columns in the CSV file
	double colC; 
	double colD;
	double colE;
	double colF;
	double colG;
	double colH;
	double colI;
	double colJ;

}logData;

int main()
{
	char choice = 'A'; //Character that will store the users choice to end or restart the "do while" loop

	do
	{
		double initTime = 0.0, tStamp = 0.0; //The time timestamp in the data will get subtracted by the initial timestamp (first time stamp in the csv file) and then divided by 100000
		
		double tableOne = 372225; //Table 0x0005AE01 from "CAN EL129 UIC.xlsx" file 
		//Table 0x0005AE01 includes engine speed, throttle, intake air temp, coolant temp, vehicle speed, lambda #2, and battery voltage
		double engSpeed = 0.0, throttlePerc = 0.0, intAirTemp = 0.0, coolTemp = 0.0, vehSpeed = 0.0, lambTwo = 0.0, batVolts = 0.0;

		double tableTwo = 372226; //Table 0x0005AE02 from "CAN EL129 UIC.xlsx" file
		//Table 0x0005AE02 inludes MAP, VE, oil pressure, and the nuetral switch
		double MAP = 0.0, VE = 0.0, oilPres = 0.0, NueSwitch = 0.0;

		double tableFive = 372229; // Table 0x0005AE05 from "CAN EL129 UIC.xlsx" file
		//Table 0x0005AE05 inludes TireTemp_RM, TireTemp_RR, BrakeTemp_FL, and BrakeTemp_FR
		double tireTempRM = 0, tireTempRR = 0, brakeTempFL = 0, brakeTempFR = 0;

		double tableSeven = 372231;// Table 0x0005AE07 from "CAN EL129 UIC.xlsx" file
		//Table 0x0005AE07 inludes BNO055_Accel_X, BNO055_Accel_Y, BNO055_Accel_Z, and BNO055_Magne_X
		double accelX = 0, accelY = 0, accelZ = 0, magneX = 0;

		double tableThirteen = 372243; //Table 0x0005AE13 from "CAN EL129 UIC.xlsx" file
		//Table 0x0005AE13 includes logging status, config file read, and SD card file write error
		double logStat = 0.0, confFileRead = 0.0, SDError = 0.0;

		vector<logData> logDataStore; //Vector of the log data struct

		string line;

		string fileName; //Used to store the name of the file the user wants to decode
		std::cout << "Enter the file name of the file you want to decode (exampleName.csv): ";
		std::cin >> fileName;
		std::cout << endl;

		ifstream csvFile(fileName);

		if (!csvFile) //Will give an error if that file doesn't exist
		{
			std::cout << "ERROR: FILE NOT AVAILABLE\n\n";
			continue;
		}

		//Loops through each row and parses each number after a comma and saves that value into the struct
		while (getline(csvFile, line))
		{
			stringstream lineStream(line);
			logData columns;
			string token;

			getline(lineStream, token, ',');
			columns.timeStamp = stod(token); // stod() turns a string into a double

			getline(lineStream, token, ',');
			columns.table = stod(token);

			getline(lineStream, token, ',');
			columns.colC = stod(token);

			getline(lineStream, token, ',');
			columns.colD = stod(token);

			getline(lineStream, token, ',');
			columns.colE = stod(token);

			getline(lineStream, token, ',');
			columns.colF = stod(token);

			getline(lineStream, token, ',');
			columns.colG = stod(token);

			getline(lineStream, token, ',');
			columns.colH = stod(token);

			getline(lineStream, token, ',');
			columns.colI = stod(token);

			getline(lineStream, token, ',');
			columns.colJ = stod(token);

			logDataStore.push_back(columns);
		}

		/*for (unsigned i = 0; i < logDataStore.size(); i++)
		{
			std::cout << logDataStore[i].timeStamp << "\t" << logDataStore[i].table << "\t" << logDataStore[i].colC << "\t"
				<< logDataStore[i].colD << "\t" << logDataStore[i].colE << "\t" << logDataStore[i].colF << "\t"
				<< logDataStore[i].colG << "\t" << logDataStore[i].colH << "\t" << logDataStore[i].colI << "\t"
				<< logDataStore[i].colJ;
			std::cout << "\n";
		}*/

		initTime = logDataStore[0].timeStamp;

		string userFileName;

		string tableOneCsv; //The name of the file the user is going to output all the data to (Table one from "CAN EL129 UIC")
		ofstream writeFileOne; //File is created

		string tableTwoCsv; //The name of the second file the data is going to get written to  (Table two from the "CAN EL129 UIC")
		ofstream writeFileTwo; //File is created

		string tableFiveCsv; //The name of the fifth file the data is going to get written to  (Table five from the "CAN EL129 UIC")
		ofstream writeFileFive; //File is created

		string tableSevenCsv; //The name of the seventh file the data is going to get written to  (Table five from the "CAN EL129 UIC")
		ofstream writeFileSeven; //File is created

		std::cout << "Name your csv files: ";
		std::cin >> userFileName;
		std::cout << endl;

		//Properly names the files for the user
		tableOneCsv = userFileName + "TableOne.csv"; 
		tableTwoCsv = userFileName + "TableTwo.csv";
		tableFiveCsv = userFileName + "TableFive.csv";
		tableSevenCsv = userFileName + "TableSeven.csv";

		writeFileOne.open(tableOneCsv, ios::out); //Opens the first file
		writeFileTwo.open(tableTwoCsv, ios::out); //Opens the second file 
		writeFileFive.open(tableFiveCsv, ios::out); //Opens the fifth file 
		writeFileSeven.open(tableSevenCsv, ios::out); //Opens the seventh file 
		
		//Headers for each file
		writeFileOne << "---SAE UIC MOTORSPORTS---\n" << "---Version #: 1.7---\n" << "Message ID: 0x0005AE01\n\n";
		writeFileTwo << "---SAE UIC MOTORSPORTS---\n" << "---Version #: 1.7---\n" << "Message ID: 0x0005AE02\n\n";
		writeFileFive << "---SAE UIC MOTORSPORTS---\n" << "---Version #: 1.7---\n" << "Message ID: 0x0005AE05\n\n";
		writeFileSeven << "---SAE UIC MOTORSPORTS---\n" << "---Version #: 1.7---\n" << "Message ID: 0x0005AE07\n\n";

		/*//Loops through each row and does the calculations and prints the data into a textfile
		for (unsigned i = 0; i < logDataStore.size(); i++)
		{
			tStamp = logDataStore[i].timeStamp;
			tStamp = timeCalc(initTime, tStamp);
			writeFile << "Time Stamp: " << tStamp << " Second(s)";

			if (logDataStore[i].table == tableOne)
			{
				engSpeed = (logDataStore[i].colC * 255) + logDataStore[i].colD;
				engSpeed = engineSpeedCalc(engSpeed);
				writeFile << "\nEngine Speed: " << engSpeed << " RPM";

				throttlePerc = logDataStore[i].colE;
				throttlePerc = throttleCalc(throttlePerc);
				writeFile << "\nThrottle Percentage: " << throttlePerc << " %";

				intAirTemp = logDataStore[i].colF;
				intAirTemp = intakeAirTempCalc(intAirTemp);
				writeFile << "\nIntake Air Temperature: " << intAirTemp << " C";

				coolTemp = logDataStore[i].colG;
				coolTemp = coolantTempCalc(coolTemp);
				writeFile << "\nCoolant Temperature: " << coolTemp << " C";

				vehSpeed = logDataStore[i].colH;
				vehSpeed = vehicleSpeedCalc(vehSpeed);
				writeFile << "\nVehicle Speed: " << vehSpeed << " KPH (NO DATA)" ;

				lambTwo = logDataStore[i].colI;
				lambTwo = lambdaTwoCalc(lambTwo);
				writeFile << "\nLambda #2: " << lambTwo << " Lambda";

				batVolts = logDataStore[i].colJ;
				batVolts = batteryVoltCalc(batVolts);
				writeFile << "\nBattery Voltage: " << batVolts << " V\n\n";
			}

			else if (logDataStore[i].table == tableTwo)
			{
				MAP = (logDataStore[i].colC * 255) + logDataStore[i].colD;
				MAP = manifoldPressCalc(MAP);
				writeFile << "\nMAP: "<< MAP << " kPa";

				VE = logDataStore[i].colE;
				writeFile << "\nVE: " << VE << "%";

				oilPres = logDataStore[i].colF;
				oilPres = oilPressureCalc(oilPres);
				writeFile << "\nOil Pressure: " << oilPres << " PSIg";

				NueSwitch = logDataStore[i].colG;
				if (NueSwitch == 0)
					writeFile << "\nNuetral Switch: FALSE\n\n";
				else
					writeFile << "\nNuetral Switch: TRUE\n\n";
			}

			else if (logDataStore[i].table == tableThirteen)
			{
				logStat = logDataStore[i].colC;
				if (logStat == 0)
					writeFile << "\nLogging Status: FALSE";
				else
					writeFile << "\nLogging Status: TRUE";

				confFileRead = logDataStore[i].colD;
				if (confFileRead == 0)
					writeFile << "\nConfig File Read: FALSE";
				else
					writeFile << "\nConfig File Read: TRUE";

				SDError = logDataStore[i].colE;
				if (SDError == 0)
					writeFile << "\nSD Card File Write Error: FALSE\n\n";
				else
					writeFile << "\nSD Card File Write Error: TRUE\n\n";
			}

			else
				writeFile << "\n---Unkown Table---\n\n";
		}*/
		writeFileOne << "Time Stamp (Second(s)), Engine Speed (RPM), Throttle (%), Intake Temp (C), Coolant Temp (C), Vehicle Speed (KPH), Lambda #2, Battery Voltage (V)\n";

		for (unsigned i = 0; i < logDataStore.size(); i++)
		{
			if (logDataStore[i].table == tableOne)
			{
				tStamp = timeCalc(initTime, logDataStore[i].timeStamp);

				engSpeed = engineSpeedCalc(logDataStore[i].colC, logDataStore[i].colD);

				throttlePerc = throttleCalc(logDataStore[i].colE);

				intAirTemp = logDataStore[i].colF;

				coolTemp = logDataStore[i].colG;

				vehSpeed = vehicleSpeedCalc(logDataStore[i].colH);

				lambTwo = lambdaTwoCalc(logDataStore[i].colI);

				batVolts = batteryVoltCalc(logDataStore[i].colJ);

				writeFileOne<< tStamp << "," << engSpeed << "," << throttlePerc << "," << intAirTemp << "," << coolTemp << "," << vehSpeed << "," << lambTwo << "," << batVolts << "\n";
			}
		}

		writeFileTwo << "Time Stamp (Second(s)), MAP (kPA), VE (%), Oil Pressure (PSIg)\n";

		for (unsigned i = 0; i < logDataStore.size(); i++)
		{
			if (logDataStore[i].table == tableTwo)
			{
				tStamp = timeCalc(initTime, logDataStore[i].timeStamp);

				MAP = manifoldPressCalc(logDataStore[i].colC, logDataStore[i].colD);

				VE = logDataStore[i].colE;

				oilPres = oilPressureCalc(logDataStore[i].colF);

				writeFileTwo << tStamp << "," << MAP << "," << VE << "," << oilPres << "\n";
			}
		}

		writeFileFive << "Time Stamp (Second(s)), Brake Temp FL (C)\n";
		
		for (unsigned i = 0; i < logDataStore.size(); i++)
		{
			if (logDataStore[i].table == tableFive)
			{
				tStamp = timeCalc(initTime, logDataStore[i].timeStamp);

				brakeTempFL = brakeTempFLCalc(logDataStore[i].colG, logDataStore[i].colH);

				writeFileFive << tStamp << "," << brakeTempFL << "\n";
			}
		}

		writeFileSeven << "Time Stamp (Second(s)), Accel X (G), Accel Y (G), Accel Z (G)\n";

		for (unsigned i = 0; i < logDataStore.size(); i++)
		{
			if (logDataStore[i].table == tableSeven)
			{
				tStamp = timeCalc(initTime, logDataStore[i].timeStamp);

				accelX = accelXCalc(logDataStore[i].colC, logDataStore[i].colD);

				accelY = accelYCalc(logDataStore[i].colE, logDataStore[i].colF);

				accelZ = accelZCalc(logDataStore[i].colG, logDataStore[i].colH);

				writeFileSeven << tStamp << "," << accelX << "," << accelY << "," << accelZ << "\n";
			}
		}

		logDataStore.clear();
		csvFile.close();
		writeFileOne.close();
		writeFileTwo.close();
		writeFileFive.close();
		writeFileSeven.close();

		std::cout << "Would you like to decode another file? (y|n): ";
		std::cin >> choice;
		std::cout << endl;

	} while (choice != 'n');

	std::system("pause");
	return 0;
}