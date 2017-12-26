
/**
   Mouse library: https://github.com/arduino-libraries/Mouse
*/

#include "Mouse.h"
#include "Adafruit_seesaw.h"
#include "FeatherJoyWing.h"

Adafruit_seesaw ss;
FeatherJoyWing joy(ss);

int8_t mx;
int8_t my;

void joystickCallback(int8_t x, int8_t y)
{
  Serial.print(x); Serial.print(" : "); Serial.println(y);

  mx = x / 10;
  my = -y / 10;
}

void buttonCallback(FJBUTTON* buttons, uint8_t count)
{
  for (int i = 0; i < count; i++)
  {
    if (buttons[i].hasChanged)
    {
      switch (buttons[i].pinId)
      {
        case BUTTON_LEFT:
          if (buttons[i].pressed)
          {
            Mouse.press(MOUSE_LEFT);
          }
          else
          {
            Mouse.release(MOUSE_LEFT);
          }
          break;

        case BUTTON_RIGHT:
          if (buttons[i].pressed)
          {
            Mouse.press(MOUSE_RIGHT);
          }
          else
          {
            Mouse.release(MOUSE_RIGHT);
          }
          break;

        default:
          break;
      }
    }


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

  Serial.println("JoyWing Demo started");

  joy.begin();

  joy.registerJoystickCallback(joystickCallback);
  joy.registerButtonCallback(buttonCallback);

  Mouse.begin();
}

void loop()
{
  joy.update();
  delay(10);

  Mouse.move(mx, my, 0);
}
