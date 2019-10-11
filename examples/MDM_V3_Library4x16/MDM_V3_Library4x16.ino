/********************************************************************
 *                       MiDispositivoMIDI V3                       *
 ********************************************************************
 * 2018 BanaNut Electronics                                         *
 * bananutelectronics@gmail.com                                     *
 *                                                                  *
 * See license and documentation in the repository.                 *
 * https://github.com/bananut-electronics                           *
 *                                                                  *
 * Code:     Expansion_4x8.ino                                      *
 * Hardware: Four 4x4 boards connected in 4x16 configuration        *
 * Brief:    This code handles a MiDispositivoMIDI V3 with three    *
 *           expansions, which is, four devices in total. They have *
 *           to be connected in a 4x16, creating an array of 64     *
 *           buttons. With this code you can set a default color    *
 *           when the button is an idle state, and another color    *
 *           when the button is pushed. There is also a subroutine  *
 *           that does some animation with the leds.                *
 *                                                                  *
 ********************************************************************/

// Include the library
#include <MiDispositivoMIDI_V3.h>

BoardRoles bRoles[4] = {TOUCHPAD, TOUCHPAD, TOUCHPAD, TOUCHPAD};
MiDispositivoMIDI_V3 mdm = MiDispositivoMIDI_V3(FOUR_DEVICES_4X16, bRoles);

// Couple of variables for the led animation
int buttonCounter    = 0;
int expansionCounter = 0;
bool myflag          = 0;

void setup() {

  // Important note: Since this version contains a total of 64
  // buttons and 64 RGB leds, it is strongly recommended to not
  // use values greater than 150 for each color. In a 4x4 version
  // they can be up to 255, but in this case, the USB does not have
  // enough power to light up all leds in more than 150.
  // If you are not going to have all leds on, you can use 255, but
  // keep in mind that when the power is not enough, the behaviour
  // is unexpected.

  // Define your colors when the button is off
  uint8_t offColors[3] = {40, 40, 0};
  mdm.setOffColors( offColors );

  // Define your colors when the button is pressed
  uint8_t onColors[3] = {0, 50, 0};
  mdm.setOnColors( onColors );

  // Set a call to an animation each .025 sec
  mdm.setInterrupt(doFunnyAnimation, 0.025);
}

void loop() {
  // Run the loop
  mdm.loop();
}

// Function that is called periodically
void doFunnyAnimation(){
  
  // This animation wipes between two colors
  uint8_t colores1[3] = {0, 50, 0};
  uint8_t colores2[3] = {0, 50, 50};

  mdm.setPixelColor(expansionCounter, buttonCounter, myflag ? colores1 : colores2);

  // Generates the following sequence.
  //      for (int i = 0; i < 4; i++)
  //        for (int j = 0; j < 16; j++)
  
  if (buttonCounter == 15)
  {
    buttonCounter = 0;
    if (expansionCounter == 3)
    {
      expansionCounter = 0;
      myflag = !myflag;
    }
    else{
      expansionCounter += 1;
    }
  }
  else
  {
    buttonCounter += 1;
  }
}
