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

    string smin;

    string sAMPM;

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

                    if(Hr < 13 && Hr > 0)
                    {

                        wait(1);
                        lcd.locate(0,2);
                        lcd.printf("HOUR: ");
                        lcd.printf("%02i", Hr);
                        wait(2);
                        shour.clear(); 
                    i++;

                    }
                    
                    else 
                    {
                        wait(2);
                        lcd.cls();
                        lcd.printf("ERROR");
                        shour.clear();
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

                    if(Min < 60 && Min >= 0)
                    {

                        wait(1);
                        lcd.locate(0,2);
                        lcd.printf("MIN: ");
                        lcd.printf("%02i", Min);
                        wait(2);
                        smin.clear();

                    i++;

                    }
                    
                    else 
                    {
                        wait(2);
                        lcd.cls();
                        lcd.printf("ERROR");
                        smin.clear();
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