#include "FeatherJoyWing.h"

static uint32_t FeatherJoyWing_Button_Mask = (1 << BUTTON_RIGHT) | (1 << BUTTON_DOWN) |
                       (1 << BUTTON_LEFT) | (1 << BUTTON_UP) | (1 << BUTTON_SEL);


FeatherJoyWing::FeatherJoyWing(Adafruit_seesaw &seasaw):
    joystick_zero_threshold(5), joystick_react_threshold(2),
    joystick_x_correction(0), joystick_y_correction(0),
    ss(seasaw), joystickCallback(NULL), buttonCallback(NULL),
    last_x(0), last_y(0), irq_pin(0)
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

bool FeatherJoyWing::begin(uint32_t irq_pin, uint8_t joywing_address)
{
    this->irq_pin = irq_pin;

    if (!this->ss.begin(joywing_address))
    {
        return false;
    }

    this->ss.pinModeBulk(FeatherJoyWing_Button_Mask, INPUT_PULLUP);

    this->ss.setGPIOInterrupts(FeatherJoyWing_Button_Mask, true);
    
    pinMode(irq_pin, INPUT);

    return true;
}

bool FeatherJoyWing::update()
{
    if(NULL != this->joystickCallback)
    {
        // x: -128...0...127. y: 127...0...-128
        int8_t x = (ss.analogRead(JOYSTICK_H) / 4 - 128) + joystick_x_correction;
        int8_t y = (127 - ss.analogRead(JOYSTICK_V) / 4) + joystick_y_correction;

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
        if(!digitalRead(irq_pin))
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