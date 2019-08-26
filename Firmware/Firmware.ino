
// Include Libraries
#include "Arduino.h"
#include "DCMDriverL298.h"
#include "NewPing.h"
#include "RFID.h"
#include "SevSeg.h"


// Pin Definitions
#define DCMOTORDRIVERL298_PIN_INT1	4
#define DCMOTORDRIVERL298_PIN_ENB	3
#define DCMOTORDRIVERL298_PIN_INT2	5
#define DCMOTORDRIVERL298_PIN_ENA	2
#define DCMOTORDRIVERL298_PIN_INT3	6
#define DCMOTORDRIVERL298_PIN_INT4	7
#define HCSR04_1_PIN_TRIG	9
#define HCSR04_1_PIN_ECHO	8
#define HCSR04_2_PIN_TRIG	11
#define HCSR04_2_PIN_ECHO	10
#define RFID_PIN_RST	12
#define RFID_PIN_SDA	53
#define S7SEG_PIN_DIG1	20
#define S7SEG_PIN_DIG2	21
#define S7SEG_PIN_DIG3	22
#define S7SEG_PIN_DIG4	23
#define S7SEG_PIN_A	13
#define S7SEG_PIN_B	14
#define S7SEG_PIN_C	15
#define S7SEG_PIN_D	16
#define S7SEG_PIN_E	24
#define S7SEG_PIN_F	25
#define S7SEG_PIN_G	27
#define S7SEG_PIN_DECIMAL	17



// Global variables and defines
int s7segCounter = 0; 
unsigned long s7segTimer = millis();
byte s7segDigitPins[] = { S7SEG_PIN_DIG1, S7SEG_PIN_DIG2, S7SEG_PIN_DIG3, S7SEG_PIN_DIG4 };
byte s7segSegmentPins[] = { S7SEG_PIN_A, S7SEG_PIN_B, S7SEG_PIN_C, S7SEG_PIN_D, S7SEG_PIN_E, S7SEG_PIN_F ,S7SEG_PIN_G, S7SEG_PIN_DECIMAL };
// object initialization
HardwareSerial& bthc05(Serial1);
DCMDriverL298 dcMotorDriverL298(DCMOTORDRIVERL298_PIN_ENA,DCMOTORDRIVERL298_PIN_INT1,DCMOTORDRIVERL298_PIN_INT2,DCMOTORDRIVERL298_PIN_ENB,DCMOTORDRIVERL298_PIN_INT3,DCMOTORDRIVERL298_PIN_INT4);
NewPing hcsr04_1(HCSR04_1_PIN_TRIG,HCSR04_1_PIN_ECHO);
NewPing hcsr04_2(HCSR04_2_PIN_TRIG,HCSR04_2_PIN_ECHO);
RFID rfid(RFID_PIN_SDA,RFID_PIN_RST);
SevSeg s7seg(s7segDigitPins, s7segSegmentPins);


// define vars for testing menu
const int timeout = 10000;       //define timeout of 10 sec
char menuOption = 0;
long time0;

// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
    // Setup Serial which is useful for debugging
    // Use the Serial Monitor to view printed messages
    Serial.begin(9600);
    while (!Serial) ; // wait for serial port to connect. Needed for native USB
    Serial.println("start");
    
    bthc05.begin(9600);
    //This example uses HC-05 Bluetooth to communicate with an Android device.
    //Download bluetooth terminal from google play store, https://play.google.com/store/apps/details?id=Qwerty.BluetoothTerminal&hl=en
    //Pair and connect to 'HC-05', the default password for connection is '1234'.
    //You should see this message from your arduino on your android device
    bthc05.println("Bluetooth On....");
    //initialize RFID module
    rfid.init();
    s7seg.setBrightness(90);      //Seven-Segment LED brightness 0 - 100
    menuOption = menu();
    
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() 
{
    
    
    if(menuOption == '1') {
    // HC - 05 Bluetooth Serial Module - Test Code
    String bthc05Str = "";
    //Receive String from bluetooth device
    if (bthc05.available())
    {
    //Read a complete line from bluetooth terminal
    bthc05Str = bthc05.readStringUntil('\n');
    // Print raw data to serial monitor
    Serial.print("BT Raw Data: ");
    Serial.println(bthc05Str);
    }
    //Send sensor data to Bluetooth device  
    bthc05.println("PUT YOUR SENSOR DATA HERE");
    }
    else if(menuOption == '2') {
    // L298N Motor Driver with Dual Hobby DC motors - Test Code
    //Start both motors. note that rotation direction is determined by the motors connection to the driver.
    //You can change the speed by setting a value between 0-255, and set the direction by changing between 1 and 0.
    dcMotorDriverL298.setMotorA(200,1);
    dcMotorDriverL298.setMotorB(200,0);
    delay(2000);
    //Stop both motors
    dcMotorDriverL298.stopMotors();
    delay(2000);

    }
    else if(menuOption == '3') {
    // Ultrasonic Sensor - HC-SR04 #1 - Test Code
    // Read distance measurment from UltraSonic sensor           
    int hcsr04_1Dist = hcsr04_1.ping_cm();
    delay(10);
    Serial.print(F("Distance: ")); Serial.print(hcsr04_1Dist); Serial.println(F("[cm]"));

    }
    else if(menuOption == '4') {
    // Ultrasonic Sensor - HC-SR04 #2 - Test Code
    // Read distance measurment from UltraSonic sensor           
    int hcsr04_2Dist = hcsr04_2.ping_cm();
    delay(10);
    Serial.print(F("Distance: ")); Serial.print(hcsr04_2Dist); Serial.println(F("[cm]"));

    }
    else if(menuOption == '5') {
    // RFID Card Reader - RC522 - Test Code
    //Read RFID tag if present
    String rfidtag = rfid.readTag();
    //print the tag to serial monitor if one was discovered
    rfid.printTag(rfidtag);

    }
    else if(menuOption == '6') {
    // 7 - Segment Display - 20mm - Test Code
    if (millis() - s7segTimer >= 50) 
    {
    s7segTimer = millis();
    s7segCounter++;                           //increment counter
    s7seg.setNumber(s7segCounter, 1);   //set display value
    }
    s7seg.refreshDisplay(); // Must run repeatedly
    }
    
    if (millis() - time0 > timeout)
    {
        menuOption = menu();
    }
    
}



// Menu function for selecting the components to be tested
// Follow serial monitor for instrcutions
char menu()
{

    Serial.println(F("\nWhich component would you like to test?"));
    Serial.println(F("(1) HC - 05 Bluetooth Serial Module"));
    Serial.println(F("(2) L298N Motor Driver with Dual Hobby DC motors"));
    Serial.println(F("(3) Ultrasonic Sensor - HC-SR04 #1"));
    Serial.println(F("(4) Ultrasonic Sensor - HC-SR04 #2"));
    Serial.println(F("(5) RFID Card Reader - RC522"));
    Serial.println(F("(6) 7 - Segment Display - 20mm"));
    Serial.println(F("(menu) send anything else or press on board reset button\n"));
    while (!Serial.available());

    // Read data from serial monitor if received
    while (Serial.available()) 
    {
        char c = Serial.read();
        if (isAlphaNumeric(c)) 
        {   
            
            if(c == '1') 
    			Serial.println(F("Now Testing HC - 05 Bluetooth Serial Module"));
    		else if(c == '2') 
    			Serial.println(F("Now Testing L298N Motor Driver with Dual Hobby DC motors"));
    		else if(c == '3') 
    			Serial.println(F("Now Testing Ultrasonic Sensor - HC-SR04 #1"));
    		else if(c == '4') 
    			Serial.println(F("Now Testing Ultrasonic Sensor - HC-SR04 #2"));
    		else if(c == '5') 
    			Serial.println(F("Now Testing RFID Card Reader - RC522"));
    		else if(c == '6') 
    			Serial.println(F("Now Testing 7 - Segment Display - 20mm"));
            else
            {
                Serial.println(F("illegal input!"));
                return 0;
            }
            time0 = millis();
            return c;
        }
    }
}


