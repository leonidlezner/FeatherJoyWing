#include "Adafruit_seesaw.h"
#include "FeatherJoyWing.h"

Adafruit_seesaw ss;
FeatherJoyWing joy(ss);

void joystickCallback(int8_t x, int8_t y)
{
    Serial.print(x); Serial.print(" : "); Serial.println(y);
}

void buttonCallback(FJBUTTON* buttons, uint8_t count)
{
    for(int i = 0; i < count; i++)
    {
        Serial.print(buttons[i].pinId);
        Serial.print(": "); 
        Serial.print(buttons[i].pressed); 
        Serial.print(" - ");
        Serial.print(buttons[i].hasChanged); 
        Serial.print(" | ");
    }

    Serial.println("");
}

void setup()
{
    Serial.begin(115200);
    while (!Serial);

    Serial.println("JoyWing Demo started");

    joy.begin();

    joy.registerJoystickCallback(joystickCallback);
    joy.registerButtonCallback(buttonCallback);
}

void loop()
{
    joy.update();
    delay(10);
}