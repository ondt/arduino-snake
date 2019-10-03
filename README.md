# Video: https://youtu.be/k6CHJ9VKTnY


# Tutorial :loudspeaker:
1. Get the parts, install [Arduino IDE](https://www.arduino.cc/en/Main/Software), install drivers for Arduino (if you have Arduino clone AND you are using Windows for some strange reason)
2. Copy-paste [the code](https://github.com/ondt/arduino-snake/blob/master/Snake.ino) into Arduino IDE
3. Install the `LedControl` library using Arduino IDE [Library Manager](https://www.arduino.cc/en/Guide/Libraries#toc2)
4. Wire everything up _(see the Wiring Diagram)_
5. Connect your Arduino and select it in `Tools > Board` and `Tools > Port`
6. Upload
7. _(optional)_ Tweak the variables, explore the code :wink:


# Parts :balloon:
Part Name            |       Ebay Link        |         Price | Note
:------------------- | ---------------------- | ------------: | :------------------------------------------------
Arduino NANO (clone) | https://ebay.us/ggAEYn |  **3.42 USD** | [UNO](https://ebay.us/Y1kYb3) can be used as well
LED Matrix           | https://ebay.us/fXkeKo |  **1.99 USD** | MAX7219 controlled 8x8 LED Matrix
Joystick             | https://ebay.us/zobEEV |  **0.99 USD** | 
Potentiometer        | https://ebay.us/CGqV6q |  **1.37 USD** | any 1k ohm to 100k ohm should be fine
Some wires           | https://ebay.us/ktnbye |  **1.20 USD** | 12 wires needed
Breadboard           | https://ebay.us/e6J8uB |  **1.59 USD** | 
**Total**            |                        | **10.56 USD** | 



# Wiring diagram :tada:
Pin           | Arduino NANO or UNO
:------------ | :------------------
Matrix CLK    | 10
Matrix CS     | 11
Matrix DIN    | 12
Joystick X    | A2
Joystick Y    | A3
Potentiometer | A7

![wiring diagram](https://raw.githubusercontent.com/ondt/arduino-snake/master/images/snake_joystick.png "wiring diagram")
The exact pin order for the matrix or joystick may be different from the one shown on the image, so please be intelligent and use the table above

# Other variants :eyes:
- Arduino snake with sound: https://github.com/der-pw/arduino-snake

