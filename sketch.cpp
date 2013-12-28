#include <Arduino.h>
#include <DFR_Keypad.h>


/*
** Based on DFRobot LCD Shield for Arduino
** from Glendon Klassen <gjklassen@gmail.com>
** http://www.sourceforge.net/users/ecefixer
** http://ecefixer.tumblr.com
**
** Heavily modified by
** Daniel Caujolle-Bert <f1rmb.daniel@gmail.com>
** http://f1rmb.blog.free.fr
**
**

    Displays the currently pressed key on the LCD screen.

        Key Codes (in left-to-right order):

            None   - KEY_NO
            Select - KEY_SELECT
            Left   - KEY_LEFT
            Up     - KEY_UP
            Down   - KEY_DOWN
            Right  - KEY_RIGHT
*/

//---------------------------------------------
int         keyPin = 0;
int         localKey = 0;
String      keyString;
/*
** Pin assignments for DFRobot LCD Keypad Shield
**   LiquidCrystal(8, 9, 4, 5, 6, 7);
** DFR_Keypad class use these as default pins
*/
DFR_Keypad  keypad(keyPin);

void setup()
{
    keypad.print("Key Grab v1.0");
    keypad.setCursor(0, 1);
    keypad.print("(c) 2013 - f1rmb");
    delay(2500);

    /*
    ** OPTIONAL
    ** keypad.setRate(x);
    ** Sets the sample rate at once every x milliseconds.
    **
    ** Default: 10ms
    */
    //keypad.setRefreshRate(10);
}

void loop()
{
    /*
    ** keypad.getKey();
    ** Grabs the current key.
    ** Returns a non-zero integer corresponding to the pressed key,
    **   OR
    ** Returns KEY_NO for no keys pressed,
    **   OR
    ** Returns KEY_WAIT (sample wait) when no key is available to be sampled.
    */

    localKey = keypad.getKey();

    if (localKey != KEY_WAIT)
    {
        keypad.clear();
        keypad.setCursor(0, 0);
        keypad.print("Press a Key:");
        keypad.setCursor(0, 1);

        switch (localKey)
        {
            case KEY_WAIT:
                keyString = "WAIT";
                break;
            case KEY_NO:
                keyString = "NO-KEY";
                break;
            case KEY_UP:
                keyString = "UP";
                break;
            case KEY_DOWN:
                keyString = "DOWN";
                break;
            case KEY_LEFT:
                keyString = "LEFT";
                break;
            case KEY_RIGHT:
                keyString = "RIGHT";
                break;
            case KEY_SELECT:
                keyString = "SELECT";
                break;
            default:
                keyString = "UNK";
        }

        keyString = keyString + " (" + keypad.getAnalogValue() + ")";
        keypad.print(keyString);
    }
}
