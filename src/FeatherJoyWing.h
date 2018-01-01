#ifndef LIB_FEATHERJOYWING_H
#define LIB_FEATHERJOYWING_H

#include "Adafruit_seesaw.h"

#ifndef JOYWING_ADDRESS
#define JOYWING_ADDRESS SEESAW_ADDRESS
#endif

#define FJ_NUM_OF_BUTTONS   5

#define BUTTON_RIGHT        6
#define BUTTON_DOWN         7
#define BUTTON_LEFT         9
#define BUTTON_UP           10
#define BUTTON_SEL          14

#define JOYSTICK_H          3
#define JOYSTICK_V          2

#ifndef FEATHERJOYWING_IRQ_PIN
#define FEATHERJOYWING_IRQ_PIN         6
#endif

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
        bool begin();
        bool begin(uint32_t irq_pin, uint8_t joywing_address);
        bool update();
        void registerJoystickCallback(featherjoywing_joystick_callback callback) { this->joystickCallback = callback; };
        void registerButtonCallback(featherjoywing_button_callback callback) { this->buttonCallback = callback; };
    private:
        Adafruit_seesaw &ss;
        featherjoywing_joystick_callback joystickCallback;
        featherjoywing_button_callback buttonCallback;
        uint8_t joystick_zero_threshold;
        uint8_t joystick_react_threshold;
        FJBUTTON buttons[FJ_NUM_OF_BUTTONS];
};

#endif