/*
  DFR_Keypad class library for Arduino (tm), version 2.2

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


DFR_Keypad::DFR_Keypad(uint8_t cols, uint8_t rows, uint8_t keyPin, int8_t bcl, uint8_t rs, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3) : LiquidCrystal(rs, enable, d0, d1, d2, d3),
    m_cols(cols), m_rows(rows),
    m_refreshRate(10), m_keyPin(keyPin), m_threshold(DEFAULT_THRESHOLD), m_keyIn(KEY_NO),
    m_curInput(0), m_curKey(KEY_NO), m_prevInput(0), m_prevKey(KEY_NO), m_changed(false), m_oldTime(0),
    m_curCol(0), m_curRow(0), m_bclPin(bcl), m_lastKeyTime(0), m_bclTimeout(0), m_dimmed(false)
{
    initLCD(rs, enable, d0, d1, d2, d3);

    if(m_bclPin != -1)
    {
        pinMode(m_bclPin, OUTPUT);
        digitalWrite(m_bclPin, HIGH);
        m_bclTimeout = 10000; // 10s timeout;
    }
}

DFR_Key_t DFR_Keypad::getKey()
{
    if (m_lastKeyTime == 0)
        m_lastKeyTime = millis();

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
        {
            bool returnWait = false;

            if (m_bclPin != -1)
            {
                if (m_dimmed)
                {
                    uint8_t n = 0;

                    while (n < 254)
                    {
                        analogWrite(m_bclPin, n);
                        delay(2);
                        n += 2;
                    }

                    digitalWrite(m_bclPin, HIGH);
                    m_dimmed   = false;
                    returnWait = true;
                }

                m_lastKeyTime = millis();
            }

            return (returnWait ? KEY_WAIT : m_curKey);
        }
    }

    if ((m_bclPin != -1) && (!m_dimmed) && (m_bclTimeout > 0) && ((millis() - m_lastKeyTime) >= m_bclTimeout))
    {
        uint8_t n = 254;

        while (n != 0)
        {
            analogWrite(m_bclPin, n);
            delay(2);
            n -= 2;
        }

        digitalWrite(m_bclPin, LOW);
        m_lastKeyTime = millis();
        m_dimmed = true;
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
    m_curCol = 0;
    m_curRow = 0;
    LiquidCrystal::init(1, rs, 255, enable, d0, d1, d2, d3, 0, 0, 0, 0);
    LiquidCrystal::begin(m_cols, m_rows);
    LiquidCrystal::clear();
    LiquidCrystal::setCursor(m_curCol, m_curRow);
}

void DFR_Keypad::getCursor(uint8_t &col, uint8_t &row)
{
    col = m_curCol;
    row = m_curRow;
}

void  DFR_Keypad::setCursor(uint8_t col, uint8_t row)
{
    m_curCol = col;
    m_curRow = row;

    LiquidCrystal::setCursor(col, row);
}

uint8_t DFR_Keypad::getCols()
{
    return m_cols;
}

uint8_t DFR_Keypad::getRows()
{
    return m_rows;
}

void DFR_Keypad::printCenter(const char *str)
{
    if (str)
    {
        uint8_t len = strlen(str);

        if (len <= m_cols)
        {
            uint8_t x = (m_cols - len) >> 1;

            setCursor(x, m_curRow);
            LiquidCrystal::print(str);
        }
        else
        {
            char buf[m_cols + 1];
            char *p = (char *)str + m_cols;
            char *pp = p;

            snprintf(buf, sizeof(buf), "%s", str);

            setCursor(0, m_curRow);
            LiquidCrystal::print(buf);

            delay(SCROLL_DELAY);

            setCursor(m_cols, m_curRow);
            LiquidCrystal::autoscroll();

            while (*p != '\0')
            {
                LiquidCrystal::print(*p);
                delay(SCROLL_DELAY);
                p++;
            }

            LiquidCrystal::rightToLeft();

            do
            {
                LiquidCrystal::print(*p);
                delay(SCROLL_DELAY);
                p--;
            } while (p != pp);

            LiquidCrystal::leftToRight();
            LiquidCrystal::noAutoscroll();
        }
    }
}

void DFR_Keypad::setBacklightTimeout(unsigned long ms)
{
    if (m_bclPin != -1)
        m_bclTimeout = ms;
}

unsigned long DFR_Keypad::getBacklightTimeout()
{
    return m_bclTimeout;
}
