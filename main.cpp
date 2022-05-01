#include "mbed.h"
#include "TextLCD.h"
#include <cstdint>
#include <string>
#include <iostream>


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

int Sec = 50; // time in seconds
int Min = 59; // time in minutes
int Hr = 12; // time in hours
int AMPM = 2; // This is for my switch code for AM / Pm operation
string Time = "AM"; //For switch AM PM 

int Volt; // voltage that controls temperature sensor
char Unit = 'C'; // C stands for celcius
float Value, TempF, TempC; // temperature values

char Key; //keypad key
string Star; //keypad star press

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
   Temperature();
   Key = keypad_scan();

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


void setMode() // Begining Of Setmode code.
{
    //while(Set)
    //{

       /* wait(1); // wait 1 miliseconds
        normal(); // normal displays the input of enter minute, hour, second*/

        //Key = keypad_scan();

        /*if (Key != 0xFF) // Determin Setmode or Normal Mode
        {

        Star += Key;

        if (Star == "*")
        {

            Set = false;
            Norm = true;
            normal();

        }

        }*/

        //normal(); // normal displays the input of enter minute, hour, second
        //setMode();
        //Temperature(); // temperature method to print Temp in C at the moment

    lcd.cls ();
    lcd.printf ("HOUR");
    wait(1); // wait 1 miliseconds

    lcd.cls ();
    lcd.printf ("MIN");
    wait(1); // wait 1 miliseconds


    lcd.cls ();
    lcd.printf ("AM/PM");
    wait(1);

    //normal();


   // }

} // print out statements to print the time in hours, minutes, seconds, temperature, and finally display C and F



int main()
{


    while (true) 
    {
        /*cout << "Hr " << Hr << " Min " << Min << " Sec " << Sec << " " << Time << " "  << AMPM << endl;
        cout << "Keypad Press: " << keypad_scan() << "  Temp: " << Value << endl;
        cout << "Normal " << Norm << " Ser " << Set << endl;

        //wait(1); // wait 1 miliseconds
        //normal(); // normal displays the input of enter minute, hour, second
        //setMode(); // for setting time
        Temperature(); // temperature method to print Temp in C at the moment

        Key = keypad_scan();*/

        if (Norm == true)
        {

            while (Norm)
            {
                wait(1);
                normal();


                /*if (Key != 0xFF) // Determin Setmode or Normal Mode
                {
                    Star += Key;
                    if (Star == "*")
                    {

                        Set = true;
                        Norm = false;
                        while (Set)
                        {
                            wait (1);
                            setMode();
                        }

                    }
                }*/ 
            }
        }

        else if (Set == true)
        {

            while (Set)
            {
                wait(1);
                setMode();

                /*if (Key != 0xFF) // Determin Setmode or Normal Mode
                {
                    Star += Key;
                    if (Star == "*")
                    {

                        Set = false;
                        Norm = true;
                        while (Norm)
                        {
                            wait(1);
                            normal();
                        }

                    }
                }*/        
            }
        }


    }

}