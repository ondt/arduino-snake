# Video: https://youtu.be/k6CHJ9VKTnY


# Tutorial
1. Get the parts, install [Arduino IDE](https://www.arduino.cc/en/Main/Software), install drivers for Arduino (if you have Arduino clone AND you are using Windows)
2. Copy-paste [the code](https://github.com/ondt/arduino-snake/blob/master/Snake.ino) into Arduino IDE
3. Install the `LedControl` library using Arduino IDE [Library Manager](https://www.arduino.cc/en/Guide/Libraries#toc2)
4. Wire everything up _(see the Wiring Diagram)_
5. Connect your Arduino and select it in `Tools > Board` and `Tools > Port`
6. Upload
7. _(optional)_ Tweak the variables, explore the code :wink:


# Parts
Part Name            |      Amazon link       | Note
:------------------- | ---------------------- | :------------------------------------------------
Arduino NANO (clone) | https://www.amazon.com/s?k=arduino+nano | UNO can be used as well
LED Matrix           | https://www.amazon.com/s?k=arduino+matrix+MAX7219 | MAX7219 controlled 8x8 LED Matrix
Joystick             | https://www.amazon.com/s?k=arduino+joystick+breakout | 
Potentiometer        | https://www.amazon.com/s?k=10k+potentiometer | any 1k ohm to 100k ohm should be fine
Some wires           | https://www.amazon.com/s?k=arduino+wires | 12 wires needed
Breadboard           | https://www.amazon.com/s?k=arduino+breadboard | 



# Wiring diagram
Pin           | Arduino NANO or UNO
:------------ | :------------------
Matrix CLK    | 10
Matrix CS     | 11
Matrix DIN    | 12
Joystick X    | A2
Joystick Y    | A3
Potentiometer | A7

![wiring diagram](https://raw.githubusercontent.com/ondt/arduino-snake/master/images/snake_joystick.png "wiring diagram")
The exact pin order for the matrix or joystick may be different from the one shown on the image, so please be intelligent and use the table above.



# Forks
- Arduino snake with sound: https://github.com/der-pw/arduino-snake




# Other videos
- Mine, old version: https://youtu.be/k6CHJ9VKTnY
- Build tutorial: https://youtu.be/u9vSvXD0lks
- Build tutorial, english: https://youtu.be/XmKCHVKZTkE
- Very nice LEGO case: https://youtu.be/4oAePbd0kgw
- French video: https://youtu.be/tpbvVYAhIFE

