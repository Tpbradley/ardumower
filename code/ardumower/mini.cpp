/*
  Ardumower (www.ardumower.de)
  Copyright (c) 2013-2014 by Alexander Grau
  Copyright (c) 2013-2014 by Sven Gennat
  Copyright (c) 2014 by Maxime Carpentieri
  
  Private-use only! (you need to ask for a commercial-use)
 
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  
  Private-use only! (you need to ask for a commercial-use)

*/

/* Ardumower Mini - robot configuration (Ardumower electronics, Arduino Mega) 
   http://wiki.ardumower.de/index.php?title=Ardumower_Mini
*/

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// NOTE: Verify in config.h that you have enabled 'USE_MINI' ! 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *   


#include "config.h"
#ifdef USE_MINI

#ifdef __AVR__
  #include "NewPing.h"
#endif

#include <Arduino.h>
#include "mini.h"
#include "due.h"
#include "drivers.h"
#include "pinman.h"

// ------ pins---------------------------------------
#define pinMotorEnable  37         // EN motors enable
#define pinMotorLeftPWM 5          // M1_IN1 left motor PWM pin
#define pinMotorLeftDir 31         // M1_IN2 left motor Dir pin
#define pinMotorLeftSense A1       // M1_FB  left motor current sense
#define pinMotorLeftFault 25       // M1_SF  left motor fault
                                                             
#define pinMotorRightPWM  3        // M2_IN1 right motor PWM pin
#define pinMotorRightDir 33        // M2_IN2 right motor Dir pin
#define pinMotorRightSense A0      // M2_FB  right motor current sense
#define pinMotorRightFault 27      // M2_SF  right motor fault
                                    
#define pinMotorMowPWM 2           // M1_IN1 mower motor PWM pin
#define pinMotorMowDir 29          // M1_IN2 mower motor Dir pin
#define pinMotorMowSense A3        // M1_FB  mower motor current sense
#define pinMotorMowFault 26        // M1_SF  mower motor fault
#define pinMotorMowEnable 28       // EN mower motor enable
#define pinMotorMowRpm 18
    
#define pinBumperLeft 39           // bumper pins
#define pinBumperRight 38

#define pinDropLeft 45           // drop pins                                                                                          Dropsensor - Absturzsensor
#define pinDropRight 23          // drop pins                                                                                          Dropsensor - Absturzsensor

#define pinVoltageMeasurement A7   // test pin for your own voltage measurements

#define pinSonarCenterTrigger 24   // ultrasonic sensor pins
#define pinSonarCenterEcho 22
#define pinSonarRightTrigger 30    
#define pinSonarRightEcho 32
#define pinSonarLeftTrigger 34         
#define pinSonarLeftEcho 36
#define pinPerimeterRight A4       // perimeter
#define pinPerimeterLeft A5
//#define pinPerimeterMuxZ A6      // perimeter mux Z (only TC-G158 board)
#define pinLED 13                  // LED
#define pinBuzzer 53               // Buzzer
#define pinTilt 35                 // Tilt sensor (required for TC-G158 board)
#define pinButton 51               // digital ON/OFF button
#define pinBatteryVoltage A2       // battery voltage sensor
#define pinBatterySwitch 4         // battery-OFF switch   
#define pinChargeVoltage A9        // charging voltage sensor
#define pinChargeCurrent A8        // charge current sensor
#define pinChargeRelay 49        // charge current sensor  
#define pinRemoteMow 12            // remote control mower motor
#define pinRemoteSteer 11          // remote control steering 
#define pinRemoteSpeed 10          // remote control speed
#define pinRemoteSwitch 52         // remote control switch
//#define pinMuxS0 28              // mux S0 (only TC-G158 board)
//#define pinMuxS1 26              // mux S1 (only TC-G158 board)
//#define pinMuxZ A7                 // mux Z (only TC-G158 board)
#ifdef __AVR__
  #define pinOdometryLeft A12      // left odometry sensor
  #define pinOdometryLeft2 A13     // left odometry sensor (optional two-wire)
  #define pinOdometryRight A14     // right odometry sensor
  #define pinOdometryRight2 A15    // right odometry sensor (optional two-wire)
#else
  #define pinOdometryLeft DAC0     // left odometry sensor
  #define pinOdometryLeft2 DAC1    // left odometry sensor (optional two-wire)
  #define pinOdometryRight CANTX   // right odometry sensor  
  #define pinOdometryRight2 CANRX  // right odometry sensor (optional two-wire)  
#endif
#define pinLawnFrontRecv 40        // lawn sensor front receive
#define pinLawnFrontSend 41        // lawn sensor front sender 
#define pinLawnBackRecv 42         // lawn sensor back receive
#define pinLawnBackSend 43         // lawn sensor back sender 
#define pinUserSwitch1 46          // user-defined switch 1
#define pinUserSwitch2 47          // user-defined switch 2
#define pinUserSwitch3 48          // user-defined switch 3
#define pinRain 44                 // rain sensor
// IMU (compass/gyro/accel): I2C  (SCL, SDA) 
// Bluetooth: Serial2 (TX2, RX2)
// GPS: Serial3 (TX3, RX3) 

// ------- baudrates---------------------------------
#define CONSOLE_BAUDRATE    19200       // baudrate used for console
#define BLUETOOTH_BAUDRATE  19200       // baudrate used for communication with Bluetooth module
#define ESP8266_BAUDRATE    115200  // baudrate used for communication with esp8266 Wifi module
#define BLUETOOTH_PIN 1234          // Bluetooth pin

//#define USE_DEVELOPER_TEST     1      // uncomment for new perimeter signal test (developers)

Mini robot;


Mini::Mini()
{
  name = "Mini";
  // ------- wheel motors -----------------------------
  motorAccel       = 0.03;  // motor wheel acceleration (warning: do not set too high)
  motorSpeedMaxRpm    = 120;   // motor wheel max RPM
  motorSpeedMaxPwm    = 127;  // motor wheel max Pwm  (8-bit PWM=255, 10-bit PWM=1023)
  //  motorPowerMax     = 30;    // motor wheel max power (Watt)
  motorPowerMax     = 30000;    // motor wheel max power (Watt)
  motorSenseRightScale = 15.3; // motor right sense scale (mA=(ADC-zero)/scale)
  motorSenseLeftScale = 15.3; // motor left sense scale  (mA=(ADC-zero)/scale)
  motorRollTimeMax    = 2000;  // max. roll time (ms)
  motorReverseTime    = 2000;  // max. reverse time (ms)
  motorForwTimeMax   = 60000; // max. forward time (ms) / timeout
  motorBiDirSpeedRatio1 = 0.3;   // bidir mow pattern speed ratio 1
  motorBiDirSpeedRatio2 = 0.92;   // bidir mow pattern speed ratio 2
  motorLeftPID.Kp       = 0.2;    // motor wheel PID controller
  motorLeftPID.Ki       = 0.0;
  motorLeftPID.Kd       = 0.0;  
  motorRightSwapDir     = 0;    // inverse right motor direction? 
  motorLeftSwapDir      = 0;    // inverse left motor direction?  
  motorErrorSense       = 0;    // motor error sense on/off
  // ------ mower motor -------------------------------
  motorMowAccel       = 0.1;  // motor mower acceleration (warning: do not set too high)
  motorMowSpeedMaxPwm = 255;    // motor mower max PWM
  //  motorMowPowerMax = 50.0;     // motor mower max power (Watt)
  motorMowPowerMax = 50000.0;     // motor mower max power (Watt)
  motorMowModulate  = 0;      // motor mower cutter modulation?
  motorMowRPMSet     = 3300;   // motor mower RPM (only for cutter modulation)
  motorMowSenseScale = 15.3; // motor mower sense scale (mA=(ADC-zero)/scale)
  motorMowPID.Kp = 0.005;    // motor mower RPM PID controller
  motorMowPID.Ki = 0.01;
  motorMowPID.Kd = 0.01;
  //  ------ bumper -----------------------------------
  bumperUse         = 0;      // has bumpers?
  //  ------ drop -----------------------------------
  dropUse          = 0;     // has drops?                                                                                              Dropsensor - Absturzsensor vorhanden ?
  dropcontact      = 1;     //contact 0-openers 1-closers                                                                              Dropsensor - Kontakt 0-Öffner - 1-Schließer betätigt gegen GND
  // ------ rain ------------------------------------
  rainUse          = 0;      // use rain sensor?
  // ------ sonar ------------------------------------
  sonarUse          = 0;      // use ultra sonic sensor? (WARNING: robot will slow down, if enabled but not connected!)
  sonarTriggerBelow = 300;     // ultrasonic sensor trigger distance
  // ------ perimeter ---------------------------------
  perimeterUse       = 0;      // use perimeter?    
  perimeterTriggerTimeout = 0; // perimeter trigger timeout (ms)  
  perimeterOutRollTimeMax  = 2000;   // roll time max after perimeter out (ms)
  perimeterOutRollTimeMin = 750;    // roll time min after perimeter out (ms)
  perimeterOutRevTime   = 2200;   // reverse time after perimeter out (ms)
  perimeterTrackRollTime  = 2000;   // perimter tracking roll time (ms)
  perimeterTrackRevTime = 2200;  // reverse time during perimeter tracking
  perimeterPID.Kp    = 60.0;  // perimeter PID controller
  perimeterPID.Ki    = 6.0;
  perimeterPID.Kd    = 5.0;
  trackingPerimeterTransitionTimeOut = 2000;
  trackingErrorTimeOut = 10000;
  trackingBlockInnerWheelWhilePerimeterStruggling = 1;
  // ------ lawn sensor --------------------------------
  lawnSensorUse     = 0;       // use capacitive Sensor
  // ------  IMU (compass/accel/gyro) ----------------------
  imuUse            = 0;       // use IMU?
  imuCorrectDir     = 0;       // correct direction by compass?
  imuDirPID.Kp      = 0.6;     // direction PID controller
  imuDirPID.Ki      = 1.2;
  imuDirPID.Kd      = 0.3;    
  imuRollPID.Kp     = 0.6;   // roll PID controller
  imuRollPID.Ki     = 1.2;
  imuRollPID.Kd     = 0.3;  
  // ------ model R/C ------------------------------------
  remoteUse         = 1;       // use model remote control (R/C)?
  // ------ battery -------------------------------------
  batMonitor = 0;              // monitor battery and charge voltage?
  batGoHomeIfBelow = 23.7;     // drive home voltage (Volt)
  //batSwitchOffIfBelow = 21.7;  // switch off if below voltage (Volt)
  batSwitchOffIfBelow = 0.0;  // switch off if below voltage (Volt)
  batSwitchOffIfIdle = 1;      // switch off battery if idle (minutes)
  batFactor       = 0.0658;     // battery conversion factor
  batChgFactor    = 0.0658;     // battery conversion factor
  //batSenseZero       =77;        // battery volt sense zero point                                                                              
  batFull          =29.4;      // battery reference Voltage (fully charged)
  batChargingCurrentMax =1.6;  // maximum current your charger can devliver
  batFullCurrent  = 0.3;      // current flowing when battery is fully charged
  startChargingIfBelow = 27.0; // start charging if battery Voltage is below
  chargingTimeout = 12600000; // safety timer for charging (ms) 12600000 = 3.5hrs
  chgSelection    = 2;
  chgSenseZero      = 0;       // charge current sense zero point
  chgFactor       = 2.7;     // charge current conversion factor
  chgSense        = 185.0;      // mV/A empfindlichkeit des Ladestromsensors in mV/A (Für ACS712 5A = 185)
  chgChange       = 0;          // Messwertumkehr von - nach +         1 oder 0
  chgNull         = 2;          // Nullduchgang abziehen (1 oder 2)
  // ------  charging station ---------------------------
  stationRevTime     = 4000;    // charge station reverse time (ms)
  stationRollTime    = 2000;    // charge station roll time (ms)
  stationForwTime    = 2000;    // charge station forward time (ms)
  stationCheckTime   = 1700;    // charge station reverse check time (ms)
  // ------ odometry ------------------------------------
  odometryUse       = 1;       // use odometry?
  twoWayOdometrySensorUse = 0; // use optional two-wire odometry sensor?
  odometryTicksPerRevolution = 20;   // encoder ticks per one full resolution
  odometryTicksPerCm = 0.5;    // encoder ticks per cm
  odometryWheelBaseCm = 14;    // wheel-to-wheel distance (cm)
  odometryRightSwapDir = 0;       // inverse right encoder direction?
  odometryLeftSwapDir  = 0;       // inverse left encoder direction?  
  // ----- GPS -------------------------------------------
  gpsUse            = 0;       // use GPS?
  stuckIfGpsSpeedBelow = 0.2; // if Gps speed is below given value the mower is stuck
  gpsSpeedIgnoreTime = 5000; // how long gpsSpeed is ignored when robot switches into a new STATE (in ms)
  // ----- other -----------------------------------------
  buttonUse         = 1;       // has digital ON/OFF button?
  // ----- user-defined switch ---------------------------
  userSwitch1       = 0;       // user-defined switch 1 (default value)
  userSwitch2       = 0;       // user-defined switch 2 (default value)
  userSwitch3       = 0;       // user-defined switch 3 (default value)
  // ----- timer -----------------------------------------
  timerUse          = 0;       // use timer?
  // ----- bluetooth -------------------------------------
  bluetoothUse      = 1;       // use Bluetooth module?
  // ----- esp8266 ---------------------------------------
  esp8266Use        = 0;       // use ESP8266 Wifi module?
  esp8266ConfigString = "123test321";
  // ------ mower stats-------------------------------------------  
  statsOverride = false; // if set to true mower stats are overwritten - be careful
  statsMowTimeMinutesTotal = 300;
  statsBatteryChargingCounterTotal = 11;
  statsBatteryChargingCapacityTotal = 30000;
  // ------ configuration end -------------------------------------------   
}


// remote control (RC) ppm signal change interrupt
ISR(PCINT0_vect){   
  unsigned long timeMicros = micros();
  boolean remoteSpeedState = digitalRead(pinRemoteSpeed);
  boolean remoteSteerState = digitalRead(pinRemoteSteer);
  boolean remoteMowState = digitalRead(pinRemoteMow);    
  boolean remoteSwitchState = digitalRead(pinRemoteSwitch);    
  robot.setRemotePPMState(timeMicros, remoteSpeedState, remoteSteerState, remoteMowState, remoteSwitchState);    
}

// odometry signal change interrupt
// mower motor speed sensor interrupt
// NOTE: when choosing a higher perimeter sample rate (38 kHz) and using odometry interrupts, 
// the Arduino Mega cannot handle all ADC interrupts anymore - the result will be a 'noisy'
// perimeter filter output (mag value) which disappears when disabling odometry interrupts.
// SOLUTION: allow odometry interrupt handler nesting (see odometry interrupt function)
// http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
#ifdef __AVR__

  volatile byte oldOdoPins = 0;
  ISR(PCINT2_vect, ISR_NOBLOCK)
  {
    const byte actPins = PINK;                				// read register PINK
    const byte setPins = (oldOdoPins ^ actPins);
    if (setPins & 0b00010000)                 				// pin left has changed 
    {
      if (robot.motorLeftPWMCurr >= 0)						// forward
        robot.odometryLeft++;
      else
        robot.odometryLeft--;									// backward
    }
    if (setPins & 0b01000000)                  				// pin right has changed
    {
      if (robot.motorRightPWMCurr >= 0)
        robot.odometryRight++;								// forward
      else
        robot.odometryRight--;								// backward
    }  
    oldOdoPins = actPins;
  }

#else

  volatile long oldOdoPins_A = 0;
  volatile long oldOdoPins_B = 0;
  ISR(PCINT2_vect)
  {
    const long actPins_A = REG_PIOA_PDSR;       			// read PIO A
    const long actPins_B = REG_PIOB_PDSR;                               // read PIO B
    const long setPins_A = (oldOdoPins_A ^ actPins_A);
    const long setPins_B = (oldOdoPins_B ^ actPins_B);
    
    //Right
    if (setPins_A & 0b00000000000000000000000000000010)			// pin right has changed 
    {
      if (robot.motorRightPWMCurr >= 0)					// forward
        robot.odometryRight++;
      else
        robot.odometryRight--;	
        								// backward
      oldOdoPins_A = actPins_A;
    }
    
    //Left
    if (setPins_B & 0b00000000000000001000000000000000)         	// pin left has changed
    {
      if (robot.motorLeftPWMCurr >= 0)
        robot.odometryLeft++;						// forward
      else
        robot.odometryLeft--;						// backward

      oldOdoPins_B = actPins_B;
    }  
  }

#endif

// mower motor speed sensor interrupt
//void rpm_interrupt(){
//}
#ifdef __AVR__
   NewPing NewSonarLeft(pinSonarLeftTrigger, pinSonarLeftEcho, 500);
   NewPing NewSonarRight(pinSonarRightTrigger, pinSonarRightEcho, 500);
   NewPing NewSonarCenter(pinSonarCenterTrigger, pinSonarCenterEcho, 500);
#endif

// WARNING: never use 'Serial' in the Ardumower code - use 'Console' instead
// (required so we can use Arduino Due native port)

void Mini::setup()
{
  Wire.begin();            
  Console.begin(CONSOLE_BAUDRATE);   
  //while (!Console) ; // required if using Due native port
  Console.println("SETUP");
  
  // keep battery switched ON
  pinMode(pinBatterySwitch, OUTPUT);
  digitalWrite(pinBatterySwitch, HIGH);
    
    
  
  // LED, buzzer, battery
  pinMode(pinLED, OUTPUT);    
  pinMode(pinBuzzer, OUTPUT);    
  digitalWrite(pinBuzzer,0);    
  pinMode(pinBatteryVoltage, INPUT);        
  pinMode(pinChargeCurrent, INPUT);          
  pinMode(pinChargeVoltage, INPUT);            
  pinMode(pinChargeRelay, OUTPUT);
  setActuator(ACT_CHGRELAY, 0);
  
  // left wheel motor
  pinMode(pinMotorEnable, OUTPUT);  
  digitalWrite(pinMotorEnable, HIGH);
  pinMode(pinMotorLeftPWM, OUTPUT);
  pinMode(pinMotorLeftDir, OUTPUT);   
  pinMode(pinMotorLeftSense, INPUT);     
  pinMode(pinMotorLeftFault, INPUT);    
  
  // right wheel motor
  pinMode(pinMotorRightPWM, OUTPUT);
  pinMode(pinMotorRightDir, OUTPUT); 
  pinMode(pinMotorRightSense, INPUT);       
  pinMode(pinMotorRightFault, INPUT);  
  
  // mower motor
  pinMode(pinMotorMowDir, OUTPUT); 
  pinMode(pinMotorMowPWM, OUTPUT);     
  pinMode(pinMotorMowSense, INPUT);     
  pinMode(pinMotorMowRpm, INPUT);    
  pinMode(pinMotorMowEnable, OUTPUT);
  digitalWrite(pinMotorMowEnable, HIGH);  
  pinMode(pinMotorMowFault, INPUT);      
    
  // lawn sensor
  pinMode(pinLawnBackRecv, INPUT);
  pinMode(pinLawnBackSend, OUTPUT);
  pinMode(pinLawnFrontRecv, INPUT);
  pinMode(pinLawnFrontSend, OUTPUT);  
  
  // perimeter
  pinMode(pinPerimeterRight, INPUT);    
  pinMode(pinPerimeterLeft, INPUT);        
  
  // button
  pinMode(pinButton, INPUT);
  pinMode(pinButton, INPUT_PULLUP);

  // bumpers
  pinMode(pinBumperLeft, INPUT);
  pinMode(pinBumperLeft, INPUT_PULLUP);
  pinMode(pinBumperRight, INPUT);
  pinMode(pinBumperRight, INPUT_PULLUP);
 
 // drops
  pinMode(pinDropLeft, INPUT);                                                                                                         // Dropsensor - Absturzsensor - Deklariert als Eingang
  pinMode(pinDropLeft, INPUT_PULLUP);                                                                                                  // Dropsensor - Absturzsensor - Intern Pullab Widerstand aktiviert (Auslösung erfolgt gegen GND)
  pinMode(pinDropRight, INPUT);                                                                                                        // Dropsensor - Absturzsensor - Deklariert als Eingang 
  pinMode(pinDropRight, INPUT_PULLUP);                                                                                                 // Dropsensor - Absturzsensor - Intern Pullab Widerstand aktiviert (Auslösung erfolgt gegen GND)
  
  // sonar
  pinMode(pinSonarCenterTrigger, OUTPUT); 
  pinMode(pinSonarCenterEcho, INPUT); 
  pinMode(pinSonarLeftTrigger, OUTPUT); 
  pinMode(pinSonarLeftEcho, INPUT); 
  pinMode(pinSonarRightTrigger, OUTPUT); 
  pinMode(pinSonarRightEcho, INPUT); 
  
  // rain
  pinMode(pinRain, INPUT);
        
  // R/C
  pinMode(pinRemoteMow, INPUT);
  pinMode(pinRemoteSteer, INPUT);
  pinMode(pinRemoteSpeed, INPUT); 
  pinMode(pinRemoteSwitch, INPUT);       

  // odometry
  pinMode(pinOdometryLeft, INPUT_PULLUP);  
  pinMode(pinOdometryLeft2, INPUT_PULLUP);    
  pinMode(pinOdometryRight, INPUT_PULLUP);
  pinMode(pinOdometryRight2, INPUT_PULLUP);  
  
  // user switches
  pinMode(pinUserSwitch1, OUTPUT);
  pinMode(pinUserSwitch2, OUTPUT);
  pinMode(pinUserSwitch3, OUTPUT);   
  // other
  pinMode(pinVoltageMeasurement, INPUT);

  // PWM frequency setup  
  // For obstacle detection, motor torque should be detectable - torque can be computed by motor current.
  // To get consistent current values, PWM frequency should be 3.9 Khz
  // http://wiki.ardumower.de/index.php?title=Motor_driver  
  // http://sobisource.com/arduino-mega-pwm-pin-and-frequency-timer-control/
  // http://www.atmel.com/images/doc2549.pdf
  #ifdef __AVR__  
    TCCR3B = (TCCR3B & 0xF8) | 0x02;    // set PWM frequency 3.9 Khz (pin2,3,5)     
  #else
    PinMan.analogWrite(pinMotorMowPWM, 0); // sets PWMEnabled=true in Arduino library
    pmc_enable_periph_clk(PWM_INTERFACE_ID);
    PWMC_ConfigureClocks(3900 * PWM_MAX_DUTY_CYCLE, 0, VARIANT_MCK);   // 3.9 Khz  
  #endif  

  // ADC
  ADCMan.init();
  ADCMan.setCapture(pinChargeCurrent, 1, true);//Aktivierung des LaddeStrom Pins beim ADC-Managers      
  ADCMan.setCapture(pinMotorMowSense, 1, true);
  ADCMan.setCapture(pinMotorLeftSense, 1, true);
  ADCMan.setCapture(pinMotorRightSense, 1, true);
  ADCMan.setCapture(pinBatteryVoltage, 1, false);
  ADCMan.setCapture(pinChargeVoltage, 1, false);  
  ADCMan.setCapture(pinVoltageMeasurement, 1, false);    
  perimeter.setPins(pinPerimeterLeft, pinPerimeterRight);      
    
  imu.init(pinBuzzer);
	  
  gps.init();

  Robot::setup();  

  if (esp8266Use)
  {
    Console.println(F("Sending ESP8266 Config"));
    ESP8266port.begin(ESP8266_BAUDRATE);
    ESP8266port.println(esp8266ConfigString);
    ESP8266port.flush();
    ESP8266port.end();
    rc.initSerial(&ESP8266port, ESP8266_BAUDRATE);
  }
  else if (bluetoothUse)
  {
    rc.initSerial(&Bluetooth, BLUETOOTH_BAUDRATE);
  }
 
  //-------------------------------------------------------------------------
  // enable interrupts
  //-------------------------------------------------------------------------
  #ifdef __AVR__
 
    //-------------------------------------------------------------------------
    // Switch
    //-------------------------------------------------------------------------
    PCICR |= (1<<PCIE0);
    PCMSK0 |= (1<<PCINT1);
    //-------------------------------------------------------------------------
    // R/C
    //-------------------------------------------------------------------------
    PCICR |= (1<<PCIE0);
    if (remoteUse)
    {
      PCMSK0 |= (1<<PCINT4);
      PCMSK0 |= (1<<PCINT5);
      PCMSK0 |= (1<<PCINT6);
    }
	 
    //-------------------------------------------------------------------------    
    // odometry
    //-------------------------------------------------------------------------
    // Wenn odometryUse == 1 dann:
    // PCMSK2, PCINT20, HIGH			-> für links
    // PCMSK2, PCINT22, HIGH			-> für rechts
    //
    // Wenn twoWayOdo == 1 dann:
    // PCMSK2, PCINT21, HIGH
    // PCMSK2, PCINT23, HIGH
    if (odometryUse)
    { 
      PCICR |= (1<<PCIE2);
      PCMSK2 |= (1<<PCINT20);
      PCMSK2 |= (1<<PCINT22);
      if (twoWayOdometrySensorUse)
      {  
        PCMSK2 |= (1<<PCINT21);  
	PCMSK2 |= (1<<PCINT23);   
      }
    }
		
    //-------------------------------------------------------------------------	
    // mower motor speed sensor interrupt
    //-------------------------------------------------------------------------	
    
    //attachInterrupt(5, rpm_interrupt, CHANGE);  
    
    
  #else
    
    // Due interrupts
    // ODO
    attachInterrupt(pinOdometryLeft, PCINT2_vect, CHANGE);
    attachInterrupt(pinOdometryLeft2, PCINT2_vect, CHANGE);
    attachInterrupt(pinOdometryRight, PCINT2_vect, CHANGE);    
    attachInterrupt(pinOdometryRight2, PCINT2_vect, CHANGE);            
    
    // RC
    attachInterrupt(pinRemoteSpeed, PCINT0_vect, CHANGE);            
    attachInterrupt(pinRemoteSteer, PCINT0_vect, CHANGE);            
    attachInterrupt(pinRemoteMow, PCINT0_vect, CHANGE);   
    
    //Switch
    attachInterrupt(pinRemoteSwitch, PCINT0_vect, CHANGE);       
    
    //Motor Mow RPM	
    attachInterrupt(pinMotorMowRpm, PCINT2_vect, CHANGE);    

  #endif   

}
    
void checkMotorFault()
{
  if (robot.motorErrorSense == 1) 
  {
    if (digitalRead(pinMotorLeftFault)==LOW)
    {
      robot.addErrorCounter(ERR_MOTOR_LEFT);
      //Console.println(F("Error: motor left fault"));
      robot.setNextState(STATE_ERROR, 0);
      //digitalWrite(pinMotorEnable, LOW);
      //digitalWrite(pinMotorEnable, HIGH);
    }
    if  (digitalRead(pinMotorRightFault)==LOW)
    {
      robot.addErrorCounter(ERR_MOTOR_RIGHT);
      //Console.println(F("Error: motor right fault"));
      robot.setNextState(STATE_ERROR, 0);
      //digitalWrite(pinMotorEnable, LOW);
      //digitalWrite(pinMotorEnable, HIGH);
    }
    if (digitalRead(pinMotorMowFault)==LOW)
    {  
      robot.addErrorCounter(ERR_MOTOR_MOW);
      //Console.println(F("Error: motor mow fault"));
      robot.setNextState(STATE_ERROR, 0);
      //digitalWrite(pinMotorMowEnable, LOW);
      //digitalWrite(pinMotorMowEnable, HIGH);
    }
  }
}


void Mini::resetMotorFault()
{
  if (digitalRead(pinMotorLeftFault)==LOW)
  {
    digitalWrite(pinMotorEnable, LOW);
    digitalWrite(pinMotorEnable, HIGH);
    Console.println(F("Reset motor left fault"));
  }
  if  (digitalRead(pinMotorRightFault)==LOW)
  {
    digitalWrite(pinMotorEnable, LOW);
    digitalWrite(pinMotorEnable, HIGH);
    Console.println(F("Reset motor right fault"));
  }
  if (digitalRead(pinMotorMowFault)==LOW)
  {  
    digitalWrite(pinMotorMowEnable, LOW);
    digitalWrite(pinMotorMowEnable, HIGH);
    Console.println(F("Reset motor mow fault"));
  }
}

 
int Mini::readSensor(char type){
  switch (type) {
// motors------------------------------------------------------------------------------------------------
    case SEN_MOTOR_MOW: return ADCMan.read(pinMotorMowSense); break;
    case SEN_MOTOR_RIGHT: checkMotorFault(); return ADCMan.read(pinMotorRightSense); break;
    case SEN_MOTOR_LEFT:  checkMotorFault(); return ADCMan.read(pinMotorLeftSense); break;
    //case SEN_MOTOR_MOW_RPM: break; // not used - rpm is upated via interrupt

// perimeter----------------------------------------------------------------------------------------------
    case SEN_PERIM_LEFT: return perimeter.getMagnitude(0); break;
    //case SEN_PERIM_RIGHT: return Perimeter.getMagnitude(1); break;
    
// battery------------------------------------------------------------------------------------------------
    case SEN_BAT_VOLTAGE: ADCMan.read(pinVoltageMeasurement);  return ADCMan.read(pinBatteryVoltage); break;
    case SEN_CHG_VOLTAGE: return ADCMan.read(pinChargeVoltage); break;
    //case SEN_CHG_VOLTAGE: return((int)(((double)analogRead(pinChargeVoltage)) * batFactor)); break;
    case SEN_CHG_CURRENT: return ADCMan.read(pinChargeCurrent); break;
    
// buttons------------------------------------------------------------------------------------------------
    case SEN_BUTTON: return(digitalRead(pinButton)); break; 
    
//bumper----------------------------------------------------------------------------------------------------
    case SEN_BUMPER_RIGHT: return(digitalRead(pinBumperRight)); break;
    case SEN_BUMPER_LEFT: return(digitalRead(pinBumperLeft)); break;      
    
//drop----------------------------------------------------------------------------------------------------
    case SEN_DROP_RIGHT: return(digitalRead(pinDropRight)); break;                                                                                      // Dropsensor - Absturzsensor
    case SEN_DROP_LEFT: return(digitalRead(pinDropLeft)); break;                                                                                        // Dropsensor - Absturzsensor

// sonar---------------------------------------------------------------------------------------------------
    //case SEN_SONAR_CENTER: return(readURM37(pinSonarCenterTrigger, pinSonarCenterEcho)); break;  
    //case SEN_SONAR_CENTER: return(readHCSR04(pinSonarCenterTrigger, pinSonarCenterEcho)); break;
    //case SEN_SONAR_LEFT: return(readHCSR04(pinSonarLeftTrigger, pinSonarLeftEcho)); break;
    //case SEN_SONAR_RIGHT: return(readHCSR04(pinSonarRightTrigger, pinSonarRightEcho)); break;
    #ifdef __AVR__
      case SEN_SONAR_CENTER: return(NewSonarCenter.ping_cm()); break;
      case SEN_SONAR_LEFT: return(NewSonarLeft.ping_cm()); break;
      case SEN_SONAR_RIGHT: return(NewSonarRight.ping_cm()); break;
    #endif
   // case SEN_LAWN_FRONT: return(measureLawnCapacity(pinLawnFrontSend, pinLawnFrontRecv)); break;    
    //case SEN_LAWN_BACK: return(measureLawnCapacity(pinLawnBackSend, pinLawnBackRecv)); break;    
    
// imu-------------------------------------------------------------------------------------------------------
    //case SEN_IMU: imuYaw=imu.ypr.yaw; imuPitch=imu.ypr.pitch; imuRoll=imu.ypr.roll; break;    
// rtc--------------------------------------------------------------------------------------------------------
    case SEN_RTC: 
      if (!readDS1307(datetime)) {
        Console.println("RTC data error!");        
        addErrorCounter(ERR_RTC_DATA);         
        setNextState(STATE_ERROR, 0);       
      }
      break;
// rain--------------------------------------------------------------------------------------------------------
    case SEN_RAIN: if (digitalRead(pinRain)==LOW) return 1; break;
 
  }
  return 0;   
}

void Mini::setActuator(char type, int value){
  switch (type){
    case ACT_MOTOR_MOW: setMC33926(pinMotorMowDir, pinMotorMowPWM, value); break;//                                                                     Motortreiber einstellung - bei Bedarf ändern z.B setL298N auf setMC33926
    case ACT_MOTOR_LEFT: setL298N(pinMotorLeftDir, pinMotorLeftPWM, value); break;//                                                                  Motortreiber einstellung - bei Bedarf ändern z.B setL298N auf setMC33926
    case ACT_MOTOR_RIGHT: setL298N(pinMotorRightDir, pinMotorRightPWM, value); break; //                                                              Motortreiber einstellung - bei Bedarf ändern z.B setL298N auf setMC33926
    case ACT_BUZZER: if (value == 0) noTone(pinBuzzer); else tone(pinBuzzer, value); break;
    case ACT_LED: digitalWrite(pinLED, value); break;    
    case ACT_USER_SW1: digitalWrite(pinUserSwitch1, value); break;     
    case ACT_USER_SW2: digitalWrite(pinUserSwitch2, value); break;     
    case ACT_USER_SW3: digitalWrite(pinUserSwitch3, value); break;         
    case ACT_RTC:  
      if (!setDS1307(datetime)) {
        Console.println("RTC comm error!");
        addErrorCounter(ERR_RTC_COMM); 
        setNextState(STATE_ERROR, 0);       
      }
      break;
    case ACT_CHGRELAY: digitalWrite(pinChargeRelay, value); break;
    //case ACT_CHGRELAY: digitalWrite(pinChargeRelay, !value); break;
    case ACT_BATTERY_SW: digitalWrite(pinBatterySwitch, value); break;
  }
}

void Mini::configureBluetooth(boolean quick){
  BluetoothConfig bt;
  bt.setParams(name, BLUETOOTH_PIN, BLUETOOTH_BAUDRATE, quick);  
}

#endif

