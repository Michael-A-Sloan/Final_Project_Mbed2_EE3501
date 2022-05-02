#include "mbed.h"
#include "TextLCD.h"
#include <cstdint>
#include <string>
#include <iostream>
#include <sstream>


using namespace std;

TextLCD lcd(PA_0, PA_1, PA_4, PB_0, PC_1, PC_0); // rs, e, d4-d7

//InterruptIn Pass();
//InterruptIn Button(USER_BUTTON);


DigitalOut ROW1 (PA_8); 
DigitalOut ROW2 (PB_10);
DigitalOut ROW3 (PB_4); 
DigitalOut ROW4 (PB_5); 

DigitalIn COL1 (PA_7,PullUp);
DigitalIn COL2 (PB_6,PullUp);
DigitalIn COL3 (PC_7,PullUp);
DigitalIn COL4 (PA_9,PullUp);

AnalogIn LM35(PC_3); // for my temperature sensor

int Sec = 00; // time in seconds
int Min = 59; // time in minutes
int Hr = 12; // time in hours
int Hr1 = 0;
int Hr2 = 0;
int AMPM = 2; // This is for my switch code for AM / Pm operation
string Time = "AM"; //For switch AM PM 

int Volt; // voltage that controls temperature sensor
char Unit = 'C'; // C stands for celcius
float Value, TempF, TempC; // temperature values

char Key; //keypad key
string Star; //keypad star press
stringstream ss;

bool Norm = true; // controls while loop on normal mode
bool Set = false; // controls while loop on set mode



//Begining  of Keypad CODE

char key_map [4][4] = {
    {'1', '2', '3', 'A'}, //1st row
    {'4', '5', '6', 'B'}, //2nd row
    {'7', '8', '9', 'C'}, //3rd row
    {'*', '0', '#', 'D'}, //4th row
};



int col_scan(void) //start of col_scan(void)
{


    if (COL1 == 0) {
        return 0;
    }

    else if (COL2 == 0) {
        return 1;
    }

    else if (COL3 == 0) {
        return 2;
    }

    else if (COL4 == 0) {
        return 3;
    }

    else {
        return 0xFF;
    }

} //end of int col_scan(void)



char keypad_scan(void)  //start of keypad_scan(void)
{

    int row=0;
    int col=0;



    for (row = 0; row <=4; row++) {
        ROW1 = 1;
        ROW2 = 1;
        ROW3 = 1;
        ROW4 = 1;

        if(row == 0) {
            ROW1 = 0;
            col =col_scan();
            if(col <4) {
                return key_map[row][col];
            }
        } else if(row == 1) {
            ROW2 = 0;
            col = col_scan();
            if(col < 4) {
                return key_map[row][col];
            }
        } else if(row == 2) {
            ROW3 = 0;
            col = col_scan();
            if(col < 4) {
                return key_map[row][col];
            }
        } else if(row == 3) {
            ROW4 = 0;
            col = col_scan();
            if(col < 4) {
                return key_map[row][col];
            }
        }
    }

    return 0xFF;


} //end of int keypad_scan(void)


void Temperature() //Begining of temp code
{

Value = (LM35.read()*5000); // need to figure out this crap here *(5.0/4092.0)*100

 
TempC=((Value)/10);
TempF=(9.0*TempC)/5.0 + 32.0;


} //End Of temp code


void normal() //Start of Normal Clock Mode
{
    wait(1);
    Temperature();

   Sec = Sec + 1;

    if (Sec == 60)   /* seconds is equal to 60 then again start from zero and add an increment of one in the minute value */
    {
        Sec = 00;
        Min = Min + 1;
    }

    if (Min == 60) /* if minute is equal to 60 then again start from zero and add an increment of one in the hour value */
    {
        Min = 00;
        Hr = Hr + 1;
    }

    if (Hr == 13) /* if hour value is 13 then replace its value from 13 to 1 to change it to 12 hour format*/
    {
        Hr = 01;

        switch (AMPM)
        {
            case 1:
                Time = "AM";
                AMPM ++;
                break;
            case 2:
                Time = "PM";
                AMPM --;
                break;
        }
    
    }

    cout << "Normal Mode" << endl;
    cout << "Hr " << Hr << " Min " << Min << " Sec " << Sec << " " << Time << " "  << AMPM << endl;
    cout << "Keypad Press: " << keypad_scan() << "  Temp: " << Value << endl;
    cout << "Normal " << Norm << " Ser " << Set << endl;
    
    lcd.cls (); // lcd cls clears the display*/

    lcd.printf ("Time ");
    lcd.printf ("%02i",Hr); // print hours used %02i for an integer with 2 digits and Preceeding Zeros.
    lcd.printf ( ":" );
    lcd.printf ("%02i",Min); // print minutes used %02i for an integer with 2 digits and Preceeding Zeros.
    lcd.printf ( ":" );
    lcd.printf ("%02i",Sec); // print seconds used %02i for an integer with 2 digits and Preceeding Zeros.
    lcd.printf (" ");
    lcd.printf ("%s",Time); //AM and PM output onto lcd

    lcd.locate (0,2); //Second Row on LCD
    lcd.printf ("Temp ");
    lcd.printf ("%02.0f",TempC); // print temperature value used %02.0f for a float with 2 digits rounding up and Preceeding Zeros.
    lcd.printf (" ");
    lcd.printf ("%c",Unit);
    

} //End of Normal Clock Mode


int setMode() // Begining Of Setmode code. slow code but workse
{
    string shour;
    shour.erase();

    string smin;
    smin.erase();

    string sAMPM;
    sAMPM.erase();

    //normal();

    for (int i=1; i < 5;) //trying a for loop to do the set clock...
    {
        switch (i)
        {
            case 1:

            lcd.cls ();
            lcd.printf ("HOUR");

             //   if(Hr < 13 && Hr > 0) 
            //    {
                    wait(3);
                    lcd.cls();
                    lcd.printf("Set Hr: ");
                    wait(2);
                
                    lcd.cls();
                    lcd.printf("First Digit");
                    wait(3);

                    if (Set==true)
                    {
                        Key=keypad_scan();
                        shour += Key;
                    }
                    

                    lcd.cls();
                    lcd.printf("Second Digit");
                    wait(3);

                    if (Set==true)
                    {
                    
                        Key=keypad_scan();
                        shour += Key;

                    }
                    
                    ss << shour;
                    ss >> Hr;
                    cout << "shour " << shour << " Hr " << Hr << endl;

                   switch (Hr)
                   {
                	case 01:
		                Hr = 1;
		            break;

	                case 02:
		                Hr = 2;
		            break;

	                case 03:
		                Hr = 3;
		            break;

	                case 04:
		                Hr = 4;
		            break;

	                case 05:
		                Hr = 5;
		            break;

	                case 06:
		                Hr = 6;
		            break;

	                case 07:
		                Hr = 7;
		            break;

	                case '08':
		                Hr = 8;
		            break;

	                case '09':
		                Hr = 9;
		            break;

	                case 10:
		                Hr = 10;
		            break;

	                case 11:
		                Hr = 11;
		            break;

	                case 12:
		                Hr = 12;
		            break;
                    }
                   
                   
                    if(Hr < 13 && Hr > 0)
                    {

                        wait(1);
                        lcd.locate(0,2);
                        lcd.printf("HOUR: ");
                        lcd.printf("%02i", Hr);
                        wait(2);
                        shour.erase(); 
                    i++;

                    }
                    
                    else 
                    {
                        wait(2);
                        lcd.cls();
                        lcd.printf("ERROR");
                        shour.erase();
                    } 
            break;

            case 2:
                
                lcd.cls ();
                lcd.printf ("Set MIN");
               wait(2);
               
                    lcd.cls();
                    lcd.printf("First Digit");
                    wait(3);

                    if (Set==true)
                    {
                        Key=keypad_scan();
                        smin += Key;
                    }
                    

                    lcd.cls();
                    lcd.printf("Second Digit");
                    wait(3);

                    if (Set==true)
                    {
                    
                        Key=keypad_scan();
                        smin += Key;

                    }
                    
                    ss << smin;
                    ss >> Min;
                    cout << "smin " << smin << " Min " << Min << endl;

                    switch(Min)
                    {
	                case 01:
		                Min = 1;
		            break;

	                case 02:
		                Min = 2;
		            break;

	                case 03:
		                Min = 3;
		            break;

	                case 04:
		                Min = 4;
		            break;

	                case 05:
		                Min = 5;
		            break;

	                case 06:
		                Min = 6;
		            break;

	                case 07:
		                Min = 7;
		            break;

	                case '08':
		                Min = 8;
		            break;

	                case '09':
		                Min = 9;
		            break;

	                case 10:
		                Min = 10;
		            break;

	                case 11:
		                Min = 11;
		            break;

	                case 12:
		                Min = 12;
		            break;

	                case 13:
		                Min = 13;
		            break;

	                case 14:
		                Min = 14;
		            break;

	                case 15:
		                Min = 15;
		            break;

	                case 16:
		                Min = 16;
		            break;

	                case 17:
		                Min = 17;
		            break;

	                case 18:
		                Min = 18;
		            break;

	                case 19:
		                Min = 19;
		            break;

	                case 20:
		                Min = 20;
		            break;

	                case 21:
		                Min = 21;
		            break;

	                case 22:
		                Min = 22;
		            break;

	                case 23:
		                Min = 23;
		            break;

	                case 24:
		                Min = 24;
		            break;

	                case 25:
		                Min = 25;
		            break;

	                case 26:
		                Min = 26;
	            	break;

	                case 27:
	                	Min = 27;
	            	break;

	                case 28:
	                	Min = 28;
	            	break;

	                case 29:
	                	Min = 29;
	            	break;

                	case 30:
                		Min = 30;
            		break;

                	case 31:
                		Min = 31;
            		break;

	                case 32:
	                	Min = 32;
            		break;

                	case 33:
                		Min = 33;
            		break;

                	case 34:
                		Min = 34;
            		break;

                	case 35:
                		Min = 35;
            		break;

                	case 36:
                		Min = 36;
            		break;

                	case 37:
                		Min = 37;
            		break;

                	case 38:
                		Min = 38;
            		break;

                	case 39:
                		Min = 39;
            		break;

                	case 40:
                		Min = 40;
            		break;

                	case 41:
	                	Min = 41;
	            	break;

                	case 42:
                		Min = 42;
            		break;

                	case 43:
                		Min = 43;
            		break;

                	case 44:
                		Min = 44;
            		break;

                	case 45:
                		Min = 45;
            		break;

                	case 46:
                		Min = 46;
            		break;

                	case 47:
                		Min = 47;
            		break;

                	case 48:
                		Min = 48;
            		break;

                	case 49:
                		Min = 49;
            		break;

                	case 50:
                		Min = 50;
            		break;

                	case 51:
                		Min = 51;
            		break;

                	case 52:
                		Min = 52;
            		break;

	                case 53:
                		Min = 53;
            		break;

                	case 54:
                   		Min = 54;
            		break;

                	case 55:
                		Min = 55;
            		break;

                	case 56:
                		Min = 56;
            		break;

                	case 57:
                		Min = 57;
	            	break;

                	case 58:
                		Min = 58;
            		break;

                	case 59:
	                	Min = 59;
            		break;
                    }


                    if(Min < 60 && Min >= 0)
                    {

                        wait(1);
                        lcd.locate(0,2);
                        lcd.printf("MIN: ");
                        lcd.printf("%02i", Min);
                        wait(2);
                        smin.erase();

                    i++;

                    }
                    
                    else 
                    {
                        wait(2);
                        lcd.cls();
                        lcd.printf("ERROR");
                        smin.erase();
                    } 

            break;

            case 3:

            lcd.cls ();
            lcd.printf ("AM / PM");
            wait(3);

                lcd.cls();
                lcd.printf("Set AM / PM: (1,2) ");
                wait(3);

                    if (Set==true)
                    {
                    
                        Key=keypad_scan();
                        sAMPM += Key;

                    }
                    
                    ss << sAMPM;
                    ss >> AMPM;
                    cout << "sAMPM " << sAMPM << " AMPM " << AMPM << endl;

                    switch (AMPM)
                    {
                        case 1:
                        Time = "AM";
                        AMPM ++;
                        break;

                        case 2:
                        Time = "PM";
                        AMPM --;
                        break;
                    }
    

                    if(AMPM < 3 && AMPM > 0)
                    {

                        wait(1);
                        lcd.locate(0,2);
                        lcd.printf("AM /PM: ");
                        lcd.printf("%s", Time);
                        wait(2);

                    i++;

                    }

                    else 
                    {
                        wait(2);
                        lcd.cls();
                        lcd.printf("ERROR");
                    }

            break;

            case 4:

            Sec = 00;
            Set = false;
            Norm = true;
            //normal();
            return(Sec);

            break;


            

        }
    } // end of for loop for set clock didnt work.

    /*lcd.cls (); //this is to prove my switch of modes work
    lcd.printf ("HOUR");
    wait(1);

                if (Key != 0xFF) // Determin Setmode or Normal Mode
                {
                    Star += Key;
                    if (Star == "*")
                    {
                        Set = false;
                        Norm = true;
                    }
                }

    lcd.cls ();
    lcd.printf ("MIN");
    wait(1);

                if (Key != 0xFF) // Determin Setmode or Normal Mode
                {
                    Star += Key;
                    if (Star == "*")
                    {
                        Set = false;
                        Norm = true;
                    }
                }


    lcd.cls ();
    lcd.printf ("AM/PM");
    //wait(1);*/


    cout << "Set Mode" << endl;
    cout << "Hr " << Hr << " Min " << Min << " Sec " << Sec << " " << Time << " "  << AMPM << endl;
    cout << "Keypad Press: " << keypad_scan() << "  Temp: " << Value << endl;
    cout << "Normal " << Norm << " Ser " << Set << endl;


} // mode switch works here



int main()
{


    while (true) //this is to be able to switch back and fourth from normal to setmode. and also to run the code for each.
    {

        if (Norm == true)
        {

            while (Norm)
            {

                normal();
                Star.clear();
                Key = keypad_scan();

                if (Key != 0xFF ) // Determin Setmode or Normal Mode
                {
                    Star += Key;
                    if (Star == "*")
                    {
                        Set = true;
                        Norm = false;
                    }
                }
            }
        }

        else if (Set == true)
        {

            while (Set)
            {
                wait(1);
                setMode();  
                Star.clear();
                Key = keypad_scan();   
                if (Key != 0xFF) // Determin Setmode or Normal Mode
                {
                    Star += Key;
                    if (Star == "*")
                    {
                        Set = false;
                        Norm = true;
                    }
                }
            }
        }


    }

}