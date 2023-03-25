#include "cmsis_os.h"
#include "mbed.h"
#include "PwmOut.h"
#include "uLCD_4DGL.h"
#include "rtos.h"
#include "Motor.h"
#include "PinDetect.h"
#include "responses.h"

//Define necessary components for the robot
uLCD_4DGL uLCD(p9,p10,p11);
Serial device(p13, p14);  // tx, rx
PwmOut bLED(p21);
PwmOut yLED(p22);
PwmOut gLED(p23);
PwmOut rLED(p24);
PinDetect stopMotorButton(p20);
Motor motor(p25, p16, p15);
AnalogOut DACout(p18);
AnalogIn temp_sensor(p17);
Timer t;
Ticker sampletick;
Ticker led_ticker;
bool stop_firework = true;
int motorMode = 0;
int syllable_counter=0;
bool speaking = false;  //only one response at a time
float brightness = 1;
volatile int color = WHITE; //default color

/*
 * Function to perform special lighting mode for the robot when the user says "Show me firework".
 */
void firework_perform() {
    if (!stop_firework) {
        if (brightness < 0) {
            bLED = 0;
            yLED = 0;
            Thread::wait(50);
            brightness = 1;
        }
        bLED = brightness;
        Thread::wait(50);
        brightness -= 0.1;
        yLED = brightness;
        Thread::wait(50);
        brightness -= 0.1;
        rLED = brightness;
        Thread::wait(50);
        brightness -= 0.1;
        gLED = brightness;
        Thread::wait(50);
        brightness -= 0.1;
    }
}

/*
 * Function to express the robot's facial expressions after the user says "Show me firework".
 * The robot displays expressions first before turn on the firework. 
 */
void led_indicator_animation(float waitTime, int color)
{
    uLCD.circle(85,40,15,color);
    uLCD.circle(85,85,15,color);
    Thread::wait(waitTime);
    uLCD.circle(85,40,11,color);
    uLCD.circle(85,85,11,color);
    Thread::wait(waitTime);
    uLCD.circle(85,40,6,color);  
    uLCD.circle(85,85,6,color);
    Thread::wait(waitTime);
    uLCD.circle(85,40,3,color);  
    uLCD.circle(85,85,3,color);  
    Thread::wait(waitTime);
    uLCD.circle(85,40,1,color);  
    uLCD.circle(85,85,1,color);
    Thread::wait(waitTime);
}

/*
 * Function to perform normal face expression for the robot. 
 */
void face(int x1, int y1, int x2, int y2, int s1, int s2, int color) { 
    //relax face: 85,40,15,G and 85, 85, 15, G
    uLCD.filled_circle(x1, y1, s1, color);
    uLCD.filled_circle(x1, y2, s2, color);
}

/*
 * Function to perform different types of expressions for the robot.
 * Expressions include: smile, angry, and troll face. 
 */
void facial_animation(int type, int eyeColor) {
    if (type == 1) { //smile-relax face
        uLCD.filled_circle(85, 40, 15, eyeColor);
        uLCD.filled_circle(85, 85, 15, eyeColor);
        uLCD.filled_circle(80, 40, 15, BLACK); //cover left eye
        uLCD.filled_circle(80, 85, 15, BLACK); //cover right eye
        uLCD.filled_circle(50, 64, 20, eyeColor); //cover mouth
        uLCD.filled_circle(55, 64, 20, BLACK);
    } else if (type == 2) { //angry face
        uLCD.filled_circle(85, 40, 15, eyeColor);
        uLCD.filled_circle(85, 85, 15, eyeColor);
        uLCD.filled_circle(100, 50, 15, BLACK);
        uLCD.filled_circle(100, 75, 15, BLACK);
        uLCD.filled_circle(20, 64, 25, eyeColor);
        uLCD.filled_circle(15, 64, 25, BLACK);
    } else if (type == 3) { //troll face
        uLCD.filled_circle(85, 40, 15, eyeColor);
        uLCD.filled_circle(85, 85, 15, eyeColor);
        uLCD.filled_circle(70, 40, 15, BLACK); //cover left eye
        uLCD.filled_circle(70, 85, 15, BLACK); //cover right eye
        uLCD.filled_rectangle(40,40,45,85,eyeColor);
        uLCD.filled_rectangle(25,55,45,70,eyeColor); //tongue
    }
}

/*
 * Function to control the gearmotor of the robot. 
 * There are 2 modes: stop and start.
 */
void motorControl(int type, int eyeColor) {
    if (type == 0) {
        motor.speed(0);
    } else {
        face(85, 25, 85, 65, 18, 15, eyeColor);
        Thread::wait(1000);
        for (float i = 0; i <= 1; i+=0.1) {
            motor.speed(i);
            Thread::wait(1000);;
        }
        face(85, 25, 85, 65, 18, 15, BLACK);
    }
}

/*
 * An interrupt that regularly check for the status of a pushbutton. 
 * This pushbutton allows the user to stop the motor manually. 
 */
void stopMotor_hit_callback(void) {
    motor.speed(0.0); 
}

////////////////////////////////////All response functions///////////////////////////////
/*
 * Function to help robot express its answer for not being able to do something. 
 */
void cannot() {
    speaking = true;
    DACout.write_u16(cannot_data[syllable_counter]);
    syllable_counter++;
    if (syllable_counter >= NUM_ELEMENTS_CANNOT) {
        syllable_counter = 0;
        sampletick.detach();
        DACout = 0.0;
        speaking = false;
    }
}

/*
 * Function to help robot answer the questions "How are you doing?"
 */
void howareyou() {
    speaking = true;
    DACout.write_u16(howareyou_data[syllable_counter]);
    syllable_counter++;
    if (syllable_counter >= NUM_ELEMENTS_HOWAREYOU) {
        syllable_counter = 0;
        sampletick.detach();
        DACout = 0.0;
        speaking = false;
    }
}

/*
 * Function to help robot respond to any insults it receives from the user.
 */
void insult() {
    speaking = true;
    DACout.write_u16(insult_data[syllable_counter]);
    syllable_counter++;
    if (syllable_counter >= NUM_ELEMENTS_INSULT) {
        syllable_counter = 0;
        sampletick.detach();
        DACout = 0.0;
        speaking = false;
    }
}

/*
 * Function to help robot respond to any kind words it gets from the user
 */
void love() {
    speaking = true;
    DACout.write_u16(love_data[syllable_counter]);
    syllable_counter++;
    if (syllable_counter >= NUM_ELEMENTS_LOVE) {
        syllable_counter = 0;
        sampletick.detach();
        DACout = 0.0;
        speaking = false;
    }
}

/*
 * Function to help robot answer when the user is in a bad/sad mood
 */
void sad() {
    speaking = true;
    DACout.write_u16(sad_data[syllable_counter]);
    syllable_counter++;
    if (syllable_counter >= NUM_ELEMENTS_SAD) {
        syllable_counter = 0;
        sampletick.detach();
        DACout = 0.0;
        speaking = false;
    }
}

/*
 * Function to help robot tell the users a joke. 
 */
void joke() {
    speaking = true;
    DACout.write_u16(joke_data[syllable_counter]);
    syllable_counter++;
    if (syllable_counter >= NUM_ELEMENTS_JOKE) {
        syllable_counter = 0;
        sampletick.detach();
        DACout = 0.0;
        speaking = false;
    }
}
//////////////////////////////////////////////////////////////////////////////////////////
/*
 * Main function for the robot. 
 * This function helps the robot listen to any user's commands and perform suitable actions. 
 * Say "Patrick" to get the robot ready to listen to commands.
 * After hearing the commands, the robot shows some facial expressions first before doing something.
 * 
 * Robot can perform many actions including: listen to commands, turn on/off LEDs, turn on/off fan, measure and show temperature, 
 * answer simple questions, and display its facial expressions.   
 *
 * To power up the robot, the user can use four AA batteries or use 5V wall adapter. 
 */
int main() {
    char rchar=0;
    bool trigger = false;
    bool firework_on = false;
    DACout = 0;
    bLED = 0;
    yLED = 0;
    gLED = 0;
    rLED = 0;
    float tempC = 0;
    float tempF = 0;

    //Interupts
    led_ticker.attach(firework_perform, 0.3);
    stopMotorButton.mode(PullUp);
    stopMotorButton.attach_deasserted(&stopMotor_hit_callback);
    stopMotorButton.setSampleFrequency();
    
    //set up uLCD for display
    uLCD.cls();
    uLCD.background_color(BLACK);
    uLCD.baudrate(3000000); //jack up baud rate to max for fast display

    //waking up the device and calibrate it
    device.putc('b');
    while (device.getc()!='o') {
        device.putc('b');
        Thread::wait(200);
    }
    while (1) {
        face(85,40,85,85,15,15,color); 
        Thread::wait(200);

        device.putc('d'); //Start Recognition
        device.putc('A'); //check for trigger word 'Patrick'
        if (device.getc() == 'r') {
            device.putc(' ');
            if (device.getc() == 'A') {
                face(85,40,85,85,15,15,BLACK); 
                face(85,40,90,85,13,17,color);
                trigger = true;
                t.start();
            }
        }

        while (trigger) {           //loop to wait for the action word
            device.putc('d');       //Start checking for the action word
            device.putc('B');       //group command 1
            if (device.getc()=='r') {   //the action word is spelled out
                device.putc(' ');
                rchar=device.getc();
                trigger = false;
                face(85,40,90,85,13,17,BLACK);
            } else {
                t.stop();
                if (t.read() > 8) {
                    t.reset();
                    trigger = false;
                    face(85,40,90,85,13,17,BLACK);
                }
                t.start();
            }
        }

        //after the action word is detected, perform appropriate action in word set 1 for now
        if (rchar == 'A') {           //blue light on
            bLED = 1;
            color = BLUE;
            led_indicator_animation(200,color);
            rchar = 0;
        } else if (rchar == 'B') {    //blue off
            bLED = 0;
            color = WHITE;
            rchar = 0;
        } else if (rchar == 'C') {  //yellow light on
            yLED = 1;
            color = YELLOW;
            led_indicator_animation(200,color);
            rchar = 0;
        } else if (rchar == 'D') {  //yellow off
            yLED = 0; 
            color = WHITE;
            rchar = 0;
        } else if (rchar == 'E') {  //show me firework
            if (!firework_on) {
                led_indicator_animation(100, BLUE);
                led_indicator_animation(100, YELLOW);
                led_indicator_animation(100, RED);
                led_indicator_animation(100, GREEN);
                brightness = 1;
                stop_firework = false;
                firework_on = true;
            }
            rchar = 0;
        } else if (rchar == 'F') { //stop firework
            stop_firework = true;
            firework_on = false;
            color = WHITE;
            bLED = 0;
            yLED = 0;
            rLED = 0;
            gLED = 0;
            rchar = 0;
            brightness = 0;
        } else if (rchar == 'G') {  //turn on fan
            motorMode = 1;
            motorControl(motorMode, color);
            rchar = 0;
        } else if (rchar == 'H') {  //stop fan
            motorMode = 0;
            motorControl(motorMode, color);
            rchar = 0;
        } else if (rchar == 'I' || rchar == 'J') {  //show the temperature, what's the temperature
            tempC = temp_sensor * 33;
            tempF = (9.0 * tempC) / 5.0 + 32.0;
            uLCD.text_height(2);
            uLCD.text_width(2);
            uLCD.printf("\n  TEMP\n");
            uLCD.printf("\n %.2f C\n", tempC);
            uLCD.printf("\n %.2f F\n", tempF);
            Thread::wait(5000);
            uLCD.cls();
            rchar = 0;
        } else if (rchar == 'K' || rchar == 'L' || rchar == 'M') { //how are you doing, how is everything, how is it going?
            if (!speaking) {
                sampletick.attach(&howareyou, 1.0 / 11025.0);
                facial_animation(1, color);
                Thread::wait(3000);
                uLCD.cls();
            }
            rchar = 0;
        } else if (rchar == 'N' || rchar == 'O' || rchar == 'P') {   //can you dance, can you move around, can you sing a song?
            if (!speaking) {
                sampletick.attach(&cannot, 1.0 / 11025.0);
                facial_animation(3, color);
                Thread::wait(3000);
                uLCD.cls();
            }
            rchar = 0;
        } else if (rchar == 'Q' || rchar == 'R' || rchar == 'S') {   //i love you, I adore you, I love you so much
            if (!speaking) {
                sampletick.attach(&love, 1.0 / 11025.0);
                facial_animation(1, color);
                Thread::wait(3000);
                uLCD.cls();
            }
            rchar = 0;
        } else if (rchar == 'T' || rchar == 'U' || rchar == 'V') {   //I am sad, I am feeling blue, I feel down right now
            if (!speaking) {
                sampletick.attach(&sad, 1.0 / 11025.0);
                facial_animation(1, color);
                Thread::wait(3000);
                uLCD.cls();
            }
            rchar = 0;
        } else if (rchar == 'W' || rchar == 'X' || rchar == 'Y') {   //Insult
            if (!speaking) {
                sampletick.attach(&insult, 1.0 / 11025.0);
                facial_animation(2, color);
                Thread::wait(3000);
                uLCD.cls();
            }
            rchar = 0;
        } else if (rchar == 'Z'){   //tell me a joke
            if (!speaking) {
                sampletick.attach(&joke, 1.0 / 11025.0);   
                facial_animation(3, color);
                Thread::wait(3000);
                uLCD.cls();
            }
            rchar = 0;
        }
    }
}