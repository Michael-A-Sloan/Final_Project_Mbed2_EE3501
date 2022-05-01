#include "mbed.h"
#include "TextLCD.h"
#include <cstdint>
#include <string>
#include <iostream>


using namespace std;

TextLCD lcd(PA_0, PA_1, PA_4, PB_0, PC_1, PC_0); // rs, e, d4-d7

InterruptIn star(PA_8);
InterruptIn Button(USER_BUTTON);

DigitalOut ROW1 (PA_8); 
DigitalOut ROW2 (PB_10);
DigitalOut ROW3 (PB_4); 
DigitalOut ROW4 (PB_5); 

DigitalIn COL1 (PA_7,PullUp);
DigitalIn COL2 (PB_6,PullUp);
DigitalIn COL3 (PC_7,PullUp);
DigitalIn COL4 (PA_9,PullUp);

AnalogIn LM35(PC_3); // board pin

int Sec = 50; // time in seconds
int Min = 59; // time in minutes
int Hr = 12;// time in hours
//float Temp = TempIn; // temperature C or F
int Volt; // voltage that controls temperature sensor
char Unit = 'C'; // C stands for celcius
int AMPM = 2; // time in 12 hour format
string Time = "AM"; //For switch AM PM 
float tempC,tempF,a[10],avg;


  ///////dont modify beforre this

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


void normal() //Start of Normal Clock Mode
{
   
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

 /*   else if (Hr ==13 && AMPM == "PM")
    {
        AMPM = "AM";
    }*/

} //End of Normal Clock Mode


/*void Temperature()
{
    int i;
 
while(1)
{
 
    avg=0;
    for(i=0;i<10;i++)
    {
        a[i]=LM35.read();
        wait(.02);
    }
    for(i=0;i<10;i++)
    {
        avg=avg+(a[i]/10);
    }
 
 
tempC=(avg*3.685503686*100);
tempF=(9.0*tempC)/5.0 + 32.0;


}*/


void setMode()
{

    lcd.cls ();
    lcd.printf ("Enter Hr 12:");
    lcd.printf ("Enter Min 59:");
    lcd.printf ("Enter Sec 00:");
    lcd.printf ("Enter current temperature: ");
    lcd.printf ("Is temperature C or F: ");

}// print out statements to print the time in hours, minutes, seconds, temperature, and finally display C and F


/*void flip()
{
    if (unit== 'F') {

    }
    if (unit== 'C') {
    }
    // if unit is farenheight then f, unit c for celcius
}*/

int main()
{
  //  star.fall(&setMode);
 //   Button.fall(&flip);
    // when you press button, sets the mode to either farenheight or celcius
    // button fall meaning flip the button when it falls / flips from num mode to set mode

    while (1) 
    {
        cout << "Hr " << Hr << " Min " << Min << " Sec " << Sec << " " << Time << " "  << AMPM << endl;
        cout << "Keypad Press: " << keypad_scan() << endl;

        wait(1); // wait 1 miliseconds
        normal(); // normal displays the input of enter minute, hour, second
   //     Temperature(); // temperature method to print farenheight or celcius
        lcd.cls (); // lcd cls clears the display

        lcd.printf ("Time ");
        lcd.printf ("%02i",Hr); // print hours used %02i for an integer with 2 digits and Preceeding Zeros.
        lcd.printf ( ":" );
        lcd.printf ("%02i",Min); // print minutes used %02i for an integer with 2 digits and Preceeding Zeros.
        lcd.printf ( ":" );
        lcd.printf ("%02i",Sec); // print seconds used %02i for an integer with 2 digits and Preceeding Zeros.
        lcd.printf (" ");
        lcd.printf ("%s", Time);
        lcd.locate (0,2);
        lcd.printf ("Temp ");
        lcd.printf ("%i",LM35); // print temperature in F or C

//test

    }

}
