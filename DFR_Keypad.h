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
