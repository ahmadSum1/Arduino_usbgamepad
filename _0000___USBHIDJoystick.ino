#include <hid.h>
#include <hiduniversal.h>
#include <usbhub.h>

// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

#include "hidjoystickrptparser.h"
////////// for  manual bot

#include <AFMotor.h>

//  #include <Servo.h>
//  Servo grabber1;
//  Servo grabber2;


AF_DCMotor motorLF(1);
AF_DCMotor motorLB(2);
AF_DCMotor motorRF(3);
AF_DCMotor motorRB(4);


static int speed1=170;
static int speed2=120;

///hand


const int Motor1Pin1 = 24;
const int Motor1Pin2 = 22;

const int Motor2Pin1 = 26;
const int Motor2Pin2 = 28;

////////////////////

//////try


//buttons
static int totalBtns=10;

static bool btns [] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /// trur/false conditions of the buttons

static bool joystickAvailable=false;


 
// movements
static uint8_t xCor;
static uint8_t yCor;
static uint8_t ac;
static uint8_t ad;
static uint8_t ae;
static uint8_t af;

//comands

bool hold1=btns[2];
bool let1=btns[4];


bool up1=btns[1];
bool down1=btns[3];

////try

USB Usb;
USBHub Hub(&Usb);
HIDUniversal Hid(&Usb);
JoystickEvents JoyEvents;
JoystickReportParser Joy(&JoyEvents);

void setup() {
        Serial.begin(115200);
#if !defined(__MIPSEL__)
        while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
        Serial.println("Start");

        if (Usb.Init() == -1)
                Serial.println("OSC did not start.");

        delay(200);

        if (!Hid.SetReportParser(0, &Joy))
                ErrorMessage<uint8_t > (PSTR("SetReportParser"), 1);


         ////manual bot


         //////////////



                
}

void loop() {

        Usb.Task();
//speed controll
        if(joystickAvailable && btns[5]){
          if(speed1<255  && speed1>1) {

                  if ( yCor==0 ) { 
                    speed1++;
                    
                  }
                
                  if ( yCor > 200 ) { 
                      speed1=0;
                     
                    }
                    
                  delay(2);
            }

         
        
        }

        
        
        
        
        if(joystickAvailable && !btns[5]){
          
          if ( yCor==127 || yCor==128 ){ 
                      
                      
                      if ( xCor==0 ) moveLeft();
                      
          
                      else if ( xCor > 200 ) moveRight();            

                      else moveStop();
            }
          
          if ( yCor==0 ) moveForward();
          
          if ( yCor > 200 ) moveBackward();

          if( hold1 ) hold();

          if( let1 ) let();
          
          if( up1 ) up();
            
          
          if( down1 ) down();


          
          }
   else stopper();


        Serial.print("xCor :");
        Serial.print(xCor);
        Serial.print("   yCor:");
        Serial.print(yCor);
        Serial.print("   z1:");
        Serial.print(ac);
        Serial.print("   z2:");
        Serial.print(ad);
        Serial.print("      ??:");
        Serial.println(ae);


        

        //delay(1000);
        Serial.println("");
}




///////////////////////////joystick class//////////////////


#include "hidjoystickrptparser.h"

JoystickReportParser::JoystickReportParser(JoystickEvents *evt) :
joyEvents(evt),
oldHat(0xDE),
oldButtons(0) {
        for (uint8_t i = 0; i < RPT_GEMEPAD_LEN; i++)
                oldPad[i] = 0xD;
}

void JoystickReportParser::Parse(HID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) {
        bool match = true;

        // Checking if there are changes in report since the method was last called
        for (uint8_t i = 0; i < RPT_GEMEPAD_LEN; i++)
                if (buf[i] != oldPad[i]) {
                        match = false;
                        break;
                }

        // Calling Game Pad event handler
        if (!match && joyEvents) {
                joyEvents->OnGamePadChanged((const GamePadEventData*)buf);

                for (uint8_t i = 0; i < RPT_GEMEPAD_LEN; i++) oldPad[i] = buf[i];
        }

        uint8_t hat = (buf[5] & 0xF);

        // Calling Hat Switch event handler
        if (hat != oldHat && joyEvents) {
                joyEvents->OnHatSwitch(hat);
                oldHat = hat;
        }

        uint16_t buttons = (0x0000 | buf[6]);
        buttons <<= 4;
        buttons |= (buf[5] >> 4);
        uint16_t changes = (buttons ^ oldButtons);

        // Calling Button Event Handler for every button changed
        if (changes) {
                for (uint8_t i = 0; i < 0x0C; i++) {
                        uint16_t mask = (0x0001 << i);

                        if (((mask & changes) > 0) && joyEvents)
                                if ((buttons & mask) > 0)
                                        joyEvents->OnButtonDn(i + 1);
                                else
                                        joyEvents->OnButtonUp(i + 1);
                }
                oldButtons = buttons;
        }
}

void JoystickEvents::OnGamePadChanged(const GamePadEventData *evt) {


       xCor = (evt->X);
       yCor = (evt->Y);
       ac = (evt->Z1);
       ad = (evt->Z2);
       ae = (evt->Rz);
       



//        Serial.print("X1: ");
//        Serial.print(evt->X);
        
//        Serial.print(" X1:h ");
//        PrintHex<uint8_t > (evt->X, 0x80);
//        Serial.print("......\tY1: ");
//        Serial.print(evt->Y);
//        Serial.print(" \tY1:h ");
//        PrintHex<uint8_t > (evt->Y, 0x80);
//        Serial.print("......\tX2: ");
//        Serial.print(evt->Z1);
//        Serial.print(" \tX2:h ");
//        PrintHex<uint8_t > (evt->Z1, 0x80);
//        Serial.print(".......\tY2: ");
//        Serial.print(evt->Z2);
//        Serial.print(" \tY2:h ");
//        PrintHex<uint8_t > (evt->Z2, 0x80);
//        Serial.print(".....\tRz: ");
//        Serial.print(evt->Rz);
//        Serial.print(" \tRz:h ");
//        PrintHex<uint8_t > (evt->Rz, 0x80);
//        Serial.println("");
}

void JoystickEvents::OnHatSwitch(uint8_t hat) {
        Serial.print("................Hat Switch:............    ");
        PrintHex<uint8_t > (hat, 0x80);
        Serial.println("joystickAvailable = true");

        joystickAvailable = true;

}

void JoystickEvents::OnButtonUp(uint8_t but_id) {
        Serial.print("Up: ");
        Serial.println(but_id, DEC);
        
        btns[(but_id)]=0;

               //delay(2);
        
}

void JoystickEvents::OnButtonDn(uint8_t but_id) {
        Serial.print("Dn: ");
        Serial.println(but_id, DEC);

       btns[(but_id)]=1;

        //delay(2);
}














/////////////////////////////
/// methods////////////////


void moveForward()
{
    motorLF.setSpeed(speed1);
    motorLB.setSpeed(speed1);
    motorRF.setSpeed(speed1);
    motorRB.setSpeed(speed1);
    motorLF.run(FORWARD);
    motorRF.run(FORWARD);
    motorLB.run(FORWARD);
    motorRB.run(FORWARD);
}

void moveBackward()
{ 
    motorLF.setSpeed(speed1);
    motorLB.setSpeed(speed1);
    motorRF.setSpeed(speed1);
    motorRB.setSpeed(speed1);
    motorLF.run(BACKWARD);
    motorRF.run(BACKWARD);
    motorLB.run(BACKWARD);
    motorRB.run(BACKWARD);
}

void moveStop()
{ 
    motorLF.setSpeed(0);
    motorLB.setSpeed(0);
    motorRF.setSpeed(0);
    motorRB.setSpeed(0);
    motorLF.run(BRAKE);
    motorRF.run(BRAKE);
    motorLB.run(BRAKE);
    motorRB.run(BRAKE);
}


void moveLeft()
{
    motorLF.setSpeed(speed2);
    motorLB.setSpeed(speed2);
    motorRF.setSpeed(speed2);
    motorRB.setSpeed(speed2);
    motorLF.run(BACKWARD);
    motorRF.run(FORWARD);
    motorLB.run(BACKWARD);
    motorRB.run(FORWARD);
}

void moveRight()
{
    motorLF.setSpeed(speed2);
    motorLB.setSpeed(speed2);
    motorRF.setSpeed(speed2);
    motorRB.setSpeed(speed2);
    motorRF.run(BACKWARD);
    motorLF.run(FORWARD);
    motorRB.run(BACKWARD);
    motorLB.run(FORWARD);
}

void moveSlowLeft()
{
    motorLF.setSpeed(speed2);
    motorLB.setSpeed(speed2);
    motorRF.setSpeed(speed1);
    motorRB.setSpeed(speed1);
    motorLF.run(BACKWARD);
    motorRF.run(BACKWARD);
    motorLB.run(BACKWARD);
    motorRB.run(BACKWARD);
}

void moveSlowRight()
{
    motorLF.setSpeed(speed1);
    motorLB.setSpeed(speed1);
    motorRF.setSpeed(speed2);
    motorRB.setSpeed(speed2);
    motorRF.run(BACKWARD);
    motorLF.run(BACKWARD);
    motorRB.run(BACKWARD);
    motorLB.run(BACKWARD);
}

void moveLeftBackward()
{
    motorLF.setSpeed(80);
    motorLB.setSpeed(80);
    motorRF.setSpeed(200);
    motorRB.setSpeed(200);
    motorLF.run(BACKWARD);
    motorRF.run(BACKWARD);
    motorLB.run(BACKWARD);
    motorRB.run(BACKWARD);
}

void moveRightBackward()
{
    motorLF.setSpeed(200);
    motorLB.setSpeed(200);
    motorRF.setSpeed(80);
    motorRB.setSpeed(80);
    motorRF.run(BACKWARD);
    motorLF.run(BACKWARD);
    motorRB.run(BACKWARD);
    motorLB.run(BACKWARD);
}

//void prepareObject()
//{
//   grabber1.write(50);
//   delay(400);
//   grabber2.write(0);
//   delay(400);
//   grabber1.write(10);
//   delay(400);
//}
//
//void grabObject()
//{
//    grabber1.write(100);
//    delay(500);
//    grabber2.write(100);
//    delay(2000);
//    grabber1.write(20);
//    delay(400);
//}

/////////////////////////


void up()
{
  digitalWrite(Motor1Pin1, LOW);
  digitalWrite(Motor1Pin2, HIGH);
  delay(2);
 
}

void down()
{
  digitalWrite(Motor1Pin1, HIGH);
  digitalWrite(Motor1Pin2, LOW);
  delay(2);
}


void hold()
{
  digitalWrite(Motor2Pin1, LOW);
  digitalWrite(Motor2Pin2, HIGH);
  delay(2);
}

void let()
{
  digitalWrite(Motor2Pin1, HIGH);
  digitalWrite(Motor2Pin2, LOW);
  delay(2);
  
}

void stopper1(){
  
  digitalWrite(Motor1Pin1, LOW);
  digitalWrite(Motor1Pin2, LOW);
}


void stopper2(){
  
  digitalWrite(Motor2Pin1, LOW);
  digitalWrite(Motor2Pin2, LOW);
}







void stopper(){

  moveStop();
  stopper1();
  stopper2();
  delay(5);
  }
