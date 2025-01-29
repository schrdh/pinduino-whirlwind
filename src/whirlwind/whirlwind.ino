// schrdh 2025
// Interfaced for sys11 pinduino shield
// Uses pinduino library
// v.0.0.1
// (2)

#include <pinduino.h>
int N_LEDS = 80;
int mid = N_LEDS / 2;
int aLEDNum1 = N_LEDS;
int aLEDNum2 = 0;

pinduino pd(aLEDNum1, aLEDNum2, "Nano");

int attract_on = 1;
unsigned long timeLastEvent = 0;      // time last event was last triggered
unsigned long attractWaitTime = 2000; // Amount of time to wait before chase lights start up again 1000 == 1 second
unsigned long timeFirstEvent = 0;     // The time the very first event was triggered
String attractColor = "yellow";       // attract color

void setup()
{
  Serial.begin(115200);
  pd.adrLED1()->clear();
  pd.adrLED2()->clear();
  pd.pinState()->reset();
}

void loop()
{
  if (attract_on)
  {
    backgroundChase();
  }
  pd.pinState()->update();
  //  Print the pin states out to serial
  //  pd.pinState()->print();
  checkPinStates();
  if (millis() - timeLastEvent > attractWaitTime)
  {
    attract_on = 1;
  }
}

void checkPinStates()
{
  int trigger = 0;

  // Whirlwind
  // 11 - P12(4) - GI Upper Playfield Relay
  // 12 - P12(5) - AC Select Relay
  // 16 - P12(9) - GI Lower Playfield & B'box Relay

  // 01C Bottom Right Flasher (1, 12, !11, !16)
  if (pd.pinState()->P11(1) && pd.pinState()->P12(5) && !pd.pinState()->P12(4) && !pd.pinState()->P12(9))
  {
    Serial.println("01C Bottom Right Flasher");
    pd.adrLED1()->chase("blue", 10, 5, 1);
    trigger = 1;
  }

  // 02C Spinner Flasher (2, 12, !11, !16)
  if (pd.pinState()->P11(2) && pd.pinState()->P12(5) && !pd.pinState()->P12(4) && !pd.pinState()->P12(9))
  {
    Serial.println("02C Spinner Flasher");
    pd.adrLED1()->spreadInFromPoint2Color(mid, "orange", "yellow", 150);
    trigger = 1;
  }

  // 03C R Ramp Top/Upr Jets Flashers (3, 12, !11, !16)
  if (pd.pinState()->P11(3) && pd.pinState()->P12(5) && !pd.pinState()->P12(4) && !pd.pinState()->P12(9))
  {
    Serial.println("03C R Ramp Top/Upr Jets Flashers");
    pd.adrLED1()->chase("red", 10, 5, 1);
    trigger = 1;
  }

  // 06C R Ramp Upr Mdl/Million Flashers (6, 12, !11, !16)
  if (pd.pinState()->P11(6) && pd.pinState()->P12(5) && !pd.pinState()->P12(4) && !pd.pinState()->P12(9))
  {
    Serial.println("06C R Ramp Upr Mdl/Million Flashers");
    for (int i = 0; i < 8; i++)
    {
      pd.adrLED1()->color("red");
      delay(250);
      pd.adrLED1()->clear();
      delay(250);
    }
    trigger = 1;
  }

  // clear out if there is noise due to coils (11, 12, 13, 14)
  if (pd.pinState()->J7(4) && pd.pinState()->J7(6) && pd.pinState()->J7(7) && pd.pinState()->J7(8))
  {
    trigger = 1;
  }

  // trigger is to take care of any cleanup after a sequence has been triggered.
  if (trigger)
  {
    timeLastEvent = millis();
    pd.adrLED1()->clear();
    pd.adrLED2()->clear();
    pd.pinState()->reset();
    trigger = 0;
    attract_on = 0;

    if (timeFirstEvent == 0)
    {
      timeFirstEvent = millis();
      randomSeed(timeFirstEvent);
    }
  }
}

void backgroundChase()
{
  pd.adrLED1()->sparkle(attractColor, 20, 30);
  pd.adrLED2()->sparkle(attractColor, 20, 30);
  if (random(250) == 0)
  {
    if (attractColor == "yellow")
      attractColor = "orange";
    else if (attractColor == "orange")
      attractColor = "blue";
    else if (attractColor == "blue")
      attractColor = "red";
    else
      attractColor = "yellow";
  }
}
