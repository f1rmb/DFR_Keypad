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
#ifndef DFR_Keypad_h
#define DFR_Keypad_h

#include <Arduino.h>
#include <LiquidCrystal.h>

typedef enum
{
    KEY_WAIT = -1,
    KEY_NO = 0,
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_SELECT
} DFR_Key_t;

class DFR_Keypad : public LiquidCrystal
{
    public:
        DFR_Keypad(uint8_t keyPin = 0, uint8_t rs = 8, uint8_t enable = 9, uint8_t d0 = 4, uint8_t d1 = 5, uint8_t d2 = 6, uint8_t d3 = 7);

        DFR_Key_t       getKey();
        int             getAnalogValue();

        void            setRefreshRate(unsigned int rate);
        unsigned int    getRefreshRate();

        void            setKeyPin(uint8_t keyPin);
        uint8_t         getKeyPin();

        void            setThreshold(int threshold);
        int             getThreshold();

        void            initLCD(uint8_t rs, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);

    private:
        DFR_Key_t       _getKeyFromAnalogValue();

    private:
        unsigned int    m_refreshRate;
        uint8_t         m_keyPin;
        int             m_threshold;
        DFR_Key_t       m_keyIn;
        int             m_curInput;
        DFR_Key_t       m_curKey;
        int             m_prevInput;
        DFR_Key_t       m_prevKey;
        boolean         m_changed;
        unsigned long   m_oldTime;
};

#endif
