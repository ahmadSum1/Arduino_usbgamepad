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

AF_DCMotor motorLF(4);
AF_DCMotor motorLB(1);
AF_DCMotor motorRF(3);
AF_DCMotor motorRB(2);


unsigned int speed1=50;
unsigned int speed2=120;
         double angle=3;


//////////////////// servooooooooooooooooooo
#include <Servo.h>
Servo servo1;
Servo servo2;

unsigned int pos1 = 20;
unsigned int pos2 = 100;

int led = A5;






//////try


//buttons
static int totalBtns=10;

static bool btns [] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  /// trur/false conditions of the buttons

static bool joystickAvailable=false;


 
// movements
static uint8_t xCor;
static uint8_t yCor;
static uint8_t ac;
static uint8_t ad;
static uint8_t ae;
static uint8_t af;

//comands

bool *hold1= &btns[2];
bool *let1= &btns[4];


bool *up1= &btns[3];
bool *down1= &btns[1];

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

    servo1.attach(9);
    servo1.write(pos1);
    
    servo2.attach(10);
    servo2.write(pos2);


     pinMode(led, OUTPUT);


         //////////////

         



                
}

void loop() {


  
        Serial.print("servo1 pos1 :     ");
        
        Serial.print(pos1);
        
        Serial.print("     servo2 pos2 :     ");
        
        Serial.println(pos2);

         if(pos1>30){
          digitalWrite(led, HIGH);
          delay(10);
          }
         else {               
          digitalWrite(led, LOW); 
          delay(10);
          } 




        
                
       // delay(100);

        Usb.Task();
    
    
    
 /////////    //speed controll     angle controll
    
        if(joystickAvailable && btns[5] && !btns[7] ){
          
      
                if (  btns[8] ) { 
                  speed1+=2;
                  
                }
              
                if (  btns[6] ) { 
                    speed1-=2;
                   
                  }
                  
                delay(50);
                
                if (speed1>255) speed1 =255 ;
                
                if (speed1<25) speed1 =25 ;
          
      
       
        
        }

        if(joystickAvailable && btns[7] && ! btns[5]){
          
      
                if ( btns[8] ) { 
                  speed2+=2;
                  
                }
              
                if ( btns[6] ) { 
                    speed2-=2;
                   
                  }
                  
                delay(100);
                
                if (speed2>255) speed2 =255 ;
                
                if (speed2<25) speed2 =25 ;
          
      
       
        
        }



        
        
      //movements      
        
        if(joystickAvailable){     ////// && !btns[5]){
                                                                   ////CHQQQQQ  angular movement

          
          
          if ( yCor==127 || yCor==128 ){ 
                      
                      
                      if ( xCor==0 ) moveLeft();
                      
          
                      else if ( xCor > 200 ) moveRight();            

                      else moveStop();
            }
          
          if ( yCor==0 ){

            if ( xCor==0 ) moveLeftForward();
                      
            else if ( xCor > 200 ) moveRightForward(); 

            
            else moveForward();
          }
          
          if ( yCor > 200 ) {

            if ( xCor==0 ) moveLeftBackward();
                      
            else if ( xCor > 200 ) moveRightBackward(); 
            
            
            else moveBackward();
            
            }

          if( *hold1 ){
            Serial.print("hold status  ");
            Serial.println(*hold1);
           // delay(1000);
            
            hold();
            delay(50);
          }

          if( *let1 ){ 
            Serial.print("let status  ");
            Serial.println(*let1);
           // delay(1000);
            
            let();
            delay(50);
          }
          
          if( *up1 ) {
            Serial.print("up status  ");
            Serial.println(*up1);
           // delay(30);
            up();
            
            delay(15);
          }
            
          
          if( *down1 ) {
            Serial.print("down status  ");
            Serial.println(*down1);
            //delay(30);

            down();
            delay(15);
          }


          
          }
   else stopper();

//  debugg//////  print

/*
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

        Serial.println("");

        */
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
    
        motorRF.run(FORWARD);
        motorRB.run(FORWARD);
        motorLF.run(BACKWARD);
        motorLB.run(BACKWARD);
}

void moveRight()
{
    motorLF.setSpeed(speed2);
    motorLB.setSpeed(speed2);
    motorRF.setSpeed(speed2);
    motorRB.setSpeed(speed2);
    
        motorLF.run(FORWARD);
        motorLB.run(FORWARD);
        motorRB.run(BACKWARD);
        motorRF.run(BACKWARD);
}

// angular movesssssssssss

void moveLeftForward()
{
    motorLF.setSpeed(speed1/angle);
    motorLB.setSpeed(speed1/angle);
    motorRF.setSpeed(speed1);
    motorRB.setSpeed(speed1);
    motorLF.run(FORWARD);
    motorRF.run(FORWARD);
    motorLB.run(FORWARD);
    motorRB.run(FORWARD);
}

void moveRightForward()
{
    motorLF.setSpeed(speed1);
    motorLB.setSpeed(speed1);
    motorRF.setSpeed(speed1/angle);
    motorRB.setSpeed(speed1/angle);
    motorRF.run(FORWARD);
    motorLF.run(FORWARD);
    motorRB.run(FORWARD);
    motorLB.run(FORWARD);
}






void moveLeftBackward()
{
    motorLF.setSpeed(speed1/angle);
    motorLB.setSpeed(speed1/angle);
    motorRF.setSpeed(speed1);
    motorRB.setSpeed(speed1);
    motorLF.run(BACKWARD);
    motorRF.run(BACKWARD);
    motorLB.run(BACKWARD);
    motorRB.run(BACKWARD);
}

void moveRightBackward()
{
    motorLF.setSpeed(speed1);
    motorLB.setSpeed(speed1);
    motorRF.setSpeed(speed1/angle);
    motorRB.setSpeed(speed1/angle);
    motorRF.run(BACKWARD);
    motorLF.run(BACKWARD);
    motorRB.run(BACKWARD);
    motorLB.run(BACKWARD);
}



/////////////////




// extra moves


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



/////////////////////////


void up()
{
  pos2+=1;
  
  if(pos2>165) pos2=165;
  servo2.write(pos2);
  delay(15);



//  Serial.print("up  pos2: ");
//  Serial.println(pos2);
//
//  delay(50);
 
}

void down()
{
  pos2-=1;
  
  if(pos2<60) pos2=60;
  servo2.write(pos2);
  delay(15);



//  Serial.print("down  pos2: ");
//  Serial.println(pos2);
// delay(40);
 
  
}


void hold()
{

  pos1+=1;
  
  if(pos1>60) pos1=60;
  servo1.write(pos1);
  delay(15);




//  Serial.print("hold  pos: ");
//  Serial.println(pos1);
//
//  delay(50);  

  
}

void let()
{
 
  pos1-=1;
  
  if(pos1<10) pos1=10;
  servo1.write(pos1);
  delay(15);



//  Serial.print("release  pos1: ");
//  Serial.println(pos1);
//
//  delay(50);
// 
 
  
}






void stopper(){

  moveStop();
  delay(5);
  }
