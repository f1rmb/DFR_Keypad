/*
  DFR_Keypad class library example for Arduino (tm), version 0.1

  Copyright (C) 2013 F1RMB, Daniel Caujolle-Bert <f1rmb.daniel@gmail.com>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

  Based on DFR_Key code Glendon Klassen <gjklassen@gmail.com>
    - http://www.sourceforge.net/users/ecefixer
    - http://ecefixer.tumblr.com
*/
#include <Arduino.h>
#include <DFR_Keypad.h>

/*
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
DFR_Keypad  keypad(16, 2, keyPin, 10);

void setup()
{
    keypad.setRepeatMode(false);

    Serial.begin(9600);
    keypad.print(F("Key Grab v1.0"));
    keypad.setCursor(0, 1);
    keypad.print(F("(c) 2013 - f1rmb"));
    delay(1000);

    keypad.clear();
    keypad.setCursor(0, 0);
    keypad.print(F("Press a Key:"));
    keypad.setCursor(0, 1);
    /*
    ** OPTIONAL
    ** keypad.setRate(x);
    ** Sets the sample rate at once every x milliseconds.
    **
    ** Default: 100ms
    */
    //keypad.setRefreshRate(200);

    /*
    ** OPTIONAL
    ** keypad.setLongPressThreshold(x);
    ** Sets the threshold value for long press key state
    **
    ** Default: 800ms
    */
    keypad.setLongPressThreshold(1000);
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
        keypad.print(F("Press a Key:"));
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
                //keypad.display();
                break;
            case KEY_DOWN:
                keyString = "DOWN";
                //keypad.noDisplay();
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

        keyString = keyString + "(" + keypad.getAnalogValue() + ")" + "L: " + (keypad.isLongPressed() ? "1" : "0");
        keypad.print(keyString);
    }
}
