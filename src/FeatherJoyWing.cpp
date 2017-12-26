#include "FeatherJoyWing.h"

static uint32_t FeatherJoyWing_Button_Mask = (1 << BUTTON_RIGHT) | (1 << BUTTON_DOWN) |
                       (1 << BUTTON_LEFT) | (1 << BUTTON_UP) | (1 << BUTTON_SEL);


FeatherJoyWing::FeatherJoyWing(Adafruit_seesaw &seasaw):
    ss(seasaw), joystickCallback(NULL), buttonCallback(NULL),
    joystick_zero_threshold(5), joystick_react_threshold(2)
{
    for(int i = 0; i < FJ_NUM_OF_BUTTONS; i++)
    {
        buttons[i].pressed = false;
        buttons[i].hasChanged = false;
    }

    buttons[0].pinId = BUTTON_RIGHT;
    buttons[1].pinId = BUTTON_DOWN;
    buttons[2].pinId = BUTTON_LEFT;
    buttons[3].pinId = BUTTON_UP;
    buttons[4].pinId = BUTTON_SEL;
}

bool FeatherJoyWing::begin()
{
    if (!this->ss.begin(JOYWING_ADDRESS))
    {
        Serial.println("JOYWING ERROR!");
        return false;
    }

    this->ss.pinModeBulk(FeatherJoyWing_Button_Mask, INPUT_PULLUP);

    this->ss.setGPIOInterrupts(FeatherJoyWing_Button_Mask, true);
    
    pinMode(FEATHERJOYWING_IRQ_PIN, INPUT);

    return true;
}

bool FeatherJoyWing::update()
{
    static int8_t last_x = 0, last_y = 0;

    if(NULL != this->joystickCallback)
    {
        // x: -128...0...127. y: 127...0...-128
        int8_t x = ss.analogRead(JOYSTICK_H) / 4 - 128;
        int8_t y = 127 - ss.analogRead(JOYSTICK_V) / 4;

        if(abs(x) < this->joystick_zero_threshold)
        {
            x = 0;
        }

        if(abs(y) < this->joystick_zero_threshold)
        {
            y = 0;
        }

        if ((abs(x - last_x) > this->joystick_react_threshold)  
            ||  (abs(y - last_y) > this->joystick_react_threshold))
        {
            this->joystickCallback(x, y);

            last_x = x;
            last_y = y;
        }
    }

    if(NULL != this->buttonCallback)
    {

        if(!digitalRead(FEATHERJOYWING_IRQ_PIN))
        {
            uint32_t buttons_state = this->ss.digitalReadBulk(FeatherJoyWing_Button_Mask);
            
            for(int i = 0; i < FJ_NUM_OF_BUTTONS; i++)
            {
                bool new_state = false;

                if(!(buttons_state & (1 << this->buttons[i].pinId)))
                {
                    new_state = true;
                }
                else
                {
                    new_state = false;
                }

                if(new_state != this->buttons[i].pressed)
                {
                    this->buttons[i].hasChanged = true;
                }
                else
                {
                    this->buttons[i].hasChanged = false;
                }

                this->buttons[i].pressed = new_state;
            }
            
            this->buttonCallback(this->buttons, FJ_NUM_OF_BUTTONS);
        }
    }

    return true;
}