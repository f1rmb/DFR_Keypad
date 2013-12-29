/*
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
*/
#include <Arduino.h>
#include "DFR_Keypad.h"

static const int DEFAULT_THRESHOLD  = 5;

// Analog readed value for each buttons
static const int UPKEY_ARV          = 98;
static const int DOWNKEY_ARV        = 251;
static const int LEFTKEY_ARV        = 402;
static const int RIGHTKEY_ARV       = 0;
static const int SELKEY_ARV         = 636;

static struct
{
    DFR_Key_t   key;
    int         value;
} keys[] =
{
    { KEY_UP,       UPKEY_ARV       },
    { KEY_DOWN,     DOWNKEY_ARV     },
    { KEY_LEFT,     LEFTKEY_ARV     },
    { KEY_RIGHT,    RIGHTKEY_ARV    },
    { KEY_SELECT,   SELKEY_ARV      },
    { KEY_NO,       0               }
};


DFR_Keypad::DFR_Keypad(uint8_t keyPin, uint8_t rs, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3) : LiquidCrystal(rs, enable, d0, d1, d2, d3),
    m_refreshRate(10), m_keyPin(keyPin), m_threshold(DEFAULT_THRESHOLD), m_keyIn(KEY_NO),
    m_curInput(0), m_curKey(KEY_NO), m_prevInput(0), m_prevKey(KEY_NO), m_changed(false), m_oldTime(0)
{
    initLCD(rs, enable, d0, d1, d2, d3);
}

DFR_Key_t DFR_Keypad::getKey()
{

    if (millis() > (m_oldTime + m_refreshRate))
    {
        m_prevInput = m_curInput;
        m_curInput  = analogRead(m_keyPin);

        if (m_curInput == m_prevInput)
        {
            m_changed = false;
            m_curKey  = m_prevKey;
        }
        else
        {
            m_changed = true;
            m_prevKey = m_curKey;
            m_curKey  = _getKeyFromAnalogValue();
        }

        m_oldTime = millis();

        if (m_changed)
            return m_curKey;
    }

    return KEY_WAIT;
}

DFR_Key_t DFR_Keypad::_getKeyFromAnalogValue()
{
    for (int i = 0; keys[i].key != KEY_NO; i++)
    {
        if ((m_curInput > (keys[i].value - m_threshold)) && (m_curInput < (keys[i].value + m_threshold)))
            return keys[i].key;
    }

    return KEY_NO;
}

int DFR_Keypad::getAnalogValue()
{
    return m_curInput;
}

void DFR_Keypad::setRefreshRate(unsigned int rate)
{
    m_refreshRate = rate;
}

unsigned int DFR_Keypad::getRefreshRate()
{
    return m_refreshRate;
}

void DFR_Keypad::setKeyPin(uint8_t keyPin)
{
    m_keyPin = keyPin;
}

uint8_t DFR_Keypad::getKeyPin()
{
    return m_keyPin;
}

void DFR_Keypad::setThreshold(int threshold)
{
    m_threshold = threshold;
}

int DFR_Keypad::getThreshold()
{
    return m_threshold;
}

void DFR_Keypad::initLCD(uint8_t rs, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3)
{
    LiquidCrystal::init(1, rs, 255, enable, d0, d1, d2, d3, 0, 0, 0, 0);
    LiquidCrystal::begin(16, 2);
    LiquidCrystal::clear();
    LiquidCrystal::setCursor(0, 0);
}
