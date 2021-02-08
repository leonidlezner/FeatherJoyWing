#ifndef LIB_FEATHERJOYWING_H
#define LIB_FEATHERJOYWING_H

#include "Adafruit_seesaw.h"

#ifndef JOYWING_ADDRESS
#define JOYWING_ADDRESS SEESAW_ADDRESS
#endif

#ifndef FEATHERJOYWING_IRQ_PIN
#define FEATHERJOYWING_IRQ_PIN         6
#endif

const uint8_t FJ_NUM_OF_BUTTONS = 5;

const uint32_t BUTTON_RIGHT = 6;
const uint32_t BUTTON_DOWN  = 7;
const uint32_t BUTTON_LEFT  = 9;
const uint32_t BUTTON_UP    = 10;
const uint32_t BUTTON_SEL   = 14;

const uint8_t JOYSTICK_H = 3;
const uint8_t JOYSTICK_V = 2;

typedef struct __fj_button__
{
    uint8_t pinId;
    bool pressed;
    bool hasChanged;
} FJBUTTON;

typedef void (*featherjoywing_joystick_callback) (int8_t, int8_t);
typedef void (*featherjoywing_button_callback) (FJBUTTON*, uint8_t);

class FeatherJoyWing
{
    public:
        FeatherJoyWing(Adafruit_seesaw &ss);
        ~FeatherJoyWing() {};
        bool begin(uint32_t irq_pin = FEATHERJOYWING_IRQ_PIN, uint8_t joywing_address = JOYWING_ADDRESS);
        bool update();
        void registerJoystickCallback(featherjoywing_joystick_callback callback) { this->joystickCallback = callback; };
        void registerButtonCallback(featherjoywing_button_callback callback) { this->buttonCallback = callback; };
        uint8_t joystick_zero_threshold;
        uint8_t joystick_react_threshold;
        int8_t joystick_x_correction;
        int8_t joystick_y_correction;
    private:
        Adafruit_seesaw &ss;
        featherjoywing_joystick_callback joystickCallback;
        featherjoywing_button_callback buttonCallback;
        FJBUTTON buttons[FJ_NUM_OF_BUTTONS];

        int8_t last_x;
        int8_t last_y;
        uint32_t irq_pin;
};

#endif