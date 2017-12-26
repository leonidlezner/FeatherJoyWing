# FeatherJoyWing library

Simple wrapper for the Adafruit Seesaw library for easy access to the JoyWing Feather.

## Dependencies

The library relies on the Seesaw Library by Adafruit: https://github.com/adafruit/Adafruit_Seesaw

## Usage

### Setup

You can set the a callback for joystick and buttons independently.

```
joy.begin();
joy.registerJoystickCallback(joystickCallback);
joy.registerButtonCallback(buttonCallback);
```

Update is performed in the loop() function by calling

```
joy.update();
```


### Callback

Joystick callback gets called with parameters x and y representing the coordinates (-128...128).


```
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
```
