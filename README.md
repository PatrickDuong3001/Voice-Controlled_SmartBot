# Voice-Controlled SmartBot (PatriceBot)
## 1. General Details
### A. Project Goals
* A robot that can provide reliable lightings and mini-fan in case of emergencies or for decorations.
* A robot that can measure the temperature if the users don't have a temperature meter or a reliable WIFI.
* A robotic friend who listens to and chats with us to lighten up our days or alleviate our boredom.  
* A deeper unstanding of how different electronic components work and how to program then using C and C++.
* Better electronic soldering and 3D design skills. 
### B. Technology
* C and C++ programming languages
* GitHub
* AutoCAD
* ARM Mbed LPC1768 Microcontroller 
* Keil Studio Cloud Compiler
* CLion IDE
* Fritzing
* Electronic Soldering
* Ultimaker Cura
* Ultimaker 3D Printer
## 2. Technical Details
### A. Complete Circuit
![Alt text](https://github.com/PatrickDuong3001/Voice-Controlled_SmartBot/blob/master/SmartBot_Circuit.png)
### B. Electronic Components
1. ARM Mbed LPC1768: https://os.mbed.com/platforms/mbed-LPC1768/
2. uLCD-144-g2 (128x128): https://www.sparkfun.com/products/11377
3. EasyVR 3 Plus: https://www.sparkfun.com/products/15453
4. PCB-Mount Speaker: https://www.sparkfun.com/products/11089
5. LEDs: https://www.sparkfun.com/products/12062
6. Dual H-Bridge Motor Driver: https://www.sparkfun.com/products/14451
7. Temperature Sensor TMP36: https://learn.adafruit.com/tmp36-temperature-sensor/overview
8. Barrel Jack: https://www.adafruit.com/product/373
9. Battery Holder with Switch: https://www.adafruit.com/product/3859
10. Driver Transistor: https://www.sparkfun.com/products/521
11. DC Motor: https://www.sparkfun.com/products/11696
12. Resistors (1kΩ and 330Ω)
### C. Exterior Design (dimensions in mm)
#### Complete House
* The house has the dimensions 261x151x120 to easily contain the complete circuit and shock-absorption sponges.
![Alt text](https://github.com/PatrickDuong3001/Voice-Controlled_SmartBot/blob/master/complete%20house.png)
* The house is splitted into two halves to fit the 3D printer. Each half has puzzle-style joints so that the house can be assembled. 
![Alt text](https://github.com/PatrickDuong3001/Voice-Controlled_SmartBot/blob/master/two%20halves.png)
#### Front Half
![Alt text](https://github.com/PatrickDuong3001/Voice-Controlled_SmartBot/blob/master/front%20half.png)
* An 21x7 ellipse hole on the left for the motor's head to go through. 
* A 38x30 rectangular hole on the center for the robot's microLCD.
* A 5m-radius circular hole on the right for the robot's microphone to listen to commands.
* 4 positive puzzle-style joints for assembling purpose. 
#### Back Half
![Alt text](https://github.com/PatrickDuong3001/Voice-Controlled_SmartBot/blob/master/back%20half.png)
* Left Side: 
1. A 13x10 rectangular hole for a USB cable to go through in case the mbed inside needs the cable to update the software. 
2. A 8x3 rectangular hole for the pushbutton wires to go through. 
3. 2 puzzle-style holes to fit the positive joints of the front half for assembling purpose. 
* Right side:
1. A 19x8 cylinder hole for the power wall adpater or the battery case's leads to go through.
2. 2 puzzle-style holes to fit the positive joints of the front half for assembling purpose. 
#### Cap
![Alt text](https://github.com/PatrickDuong3001/Voice-Controlled_SmartBot/blob/master/Robot's%20Cap.png)
* A 265x154x120 cap to cover the internal electronic components of the robot. 
* A 70x134 hole on the left so that the users can see the robot's lights.
### D. Real-Life Image
