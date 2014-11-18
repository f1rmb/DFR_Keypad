/*
  DFR_Keypad class library for Arduino (tm), version 2.3

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
        DFR_Keypad(uint8_t cols, uint8_t rows, uint8_t keyPin, int8_t bcl = -1, uint8_t rs = 8, uint8_t enable = 9, uint8_t d0 = 4, uint8_t d1 = 5, uint8_t d2 = 6, uint8_t d3 = 7);

        DFR_Key_t       getKey();
        uint16_t        getAnalogValue();

        void            setRefreshRate(uint16_t rate);
        uint16_t        getRefreshRate();

        void            setAnalogThreshold(uint16_t threshold);
        uint16_t        getAnalogThreshold();

        void            setLongPressThreshold(uint16_t threshold);
        uint16_t        getLongPressThreshold();
        bool            isLongPressed();

        void            setRepeatMode(bool sets = true);
        bool            getRepeatMode();

        void            initLCD(uint8_t rs, uint8_t enable, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3);

        uint8_t         getCols();
        uint8_t         getRows();

        void            setCursor(uint8_t col, uint8_t row);
        void            getCursor(uint8_t &col, uint8_t &row);

        void            printCenter(const char *str);
        void            printCenter(const __FlashStringHelper *ifsh);

        void            setBacklightTimeout(unsigned long ms);
        unsigned long   getBacklightTimeout();
        void            timeoutEvent();

    private:
        bool            _wakeupBacklight();
        DFR_Key_t       _getKeyFromAnalogValue(int curInput);

        static const unsigned long SCROLL_DELAY = 300;

    private:
        uint8_t         m_cols, m_rows;
        unsigned int    m_refreshRate;
        uint8_t         m_keyPin;
        int16_t         m_threshold;
        int             m_curInput;
        DFR_Key_t       m_curKey;
        DFR_Key_t       m_oldKey;
        unsigned long   m_oldTime;

        // LCD
        uint8_t         m_curCol, m_curRow;

        // backlight dimming
        int8_t          m_bclPin;
        unsigned long   m_lastKeyTime;
        unsigned long   m_bclTimeout;
        bool            m_dimmed;

        // Support repeat
        bool            m_repeat;

        // Long press feature
        unsigned long   m_pressTime;
        bool            m_longPress;
        uint16_t        m_longPressThreshold;
};

#endif
