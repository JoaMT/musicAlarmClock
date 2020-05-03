#include <Arduino.h>
#include "pitches.h"
#include "TM1637Display.h"
#include "DS3231.h"
#include <EEPROM.h>

#define CLK 2 // display clock
#define DIO 3 // display data in
#define BUZZ 9 // buzzer pin
#define LIGHT_APIN 0 // light sensor analog pin 
#define TEMP_APIN 2 // temperature sensor analog pin 

#define CHANGE_TEMP_INTERVAL   16 // count interval to show temperature 
#define CHANGE_TIME_INTERVAL   10 // count interval to show time 
#define TIME_DELAY   500 // half second delay
#define TIME_DELAY_2   1000 // second delay

DS3231 clock2; // define RT clock
RTCDateTime dt; // define date time
TM1637Display display(CLK, DIO); // define display

// 7 segment display numbers and letters manual definition
const uint8_t table7segments[]=
{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c
,0x39,0x5e,0x79,0x71,0x00,B01100011,B01000000};

const int BUTTON_TIME = 5; //change time button pin
const int BUTTON_ALARM = 6; //change alarm button pin
const int BUTTON_SNOOZE = 7; //snooze button pin

int hourAlarm; // alarm hours var
int minuteAlarm; // alarm minutes var
boolean snooze=false; // check if snooze is activated when alarm starting to sound
int tiempoDisplay=CHANGE_TEMP_INTERVAL; // counter to check if we are in show time interval or show temp interval
boolean alarm=true;

// init
void setup(){

  // buttons init
  pinMode(BUTTON_TIME, INPUT_PULLUP);
  pinMode(BUTTON_ALARM, INPUT_PULLUP);
  pinMode(BUTTON_SNOOZE, INPUT_PULLUP);

  // load saved alarm from eeprom
  hourAlarm=EEPROM.read(0);
  minuteAlarm=EEPROM.read(2);

  //clock begin
  clock2.begin();

  // date when this program is finished
  //clock2.setDateTime("May 03 2020", "14:44:00");
  
}

// main loop
void loop(){

  // check environment light to change display brightness
  if (analogRead(LIGHT_APIN)>100) {
    display.setBrightness(0x0f);
  } else if (analogRead(0)<101 && analogRead(0)>10) {
    display.setBrightness(0x02);
  } else if (analogRead(0)<11) {
    display.setBrightness(0x00);
  }  

  dt = clock2.getDateTime(); // get time

  // check if is time to play alarm music
  if (alarm && dt.hour==hourAlarm && dt.minute==minuteAlarm) {
    if (snooze==false){ musicaFeliz(); } // but only if snooze not yet pressed
  } else {
    snooze=false;
  }

  if (--tiempoDisplay>CHANGE_TIME_INTERVAL) { // when change time is in show temperature range
    displayTemp(); delayWithButtonCheck(TIME_DELAY_2);

  } else if (tiempoDisplay<=CHANGE_TIME_INTERVAL && tiempoDisplay>0) { // when change time is in show time range
    display.showNumberDec((dt.hour*100)+dt.minute, true, 4,0);
    delayWithButtonCheck(TIME_DELAY);
    display.showNumberDecEx((dt.hour*100)+dt.minute, 0b11100000, true, 4, 0);
    delayWithButtonCheck(TIME_DELAY);

  } else { // when change time ends cycle, restart again
    tiempoDisplay=CHANGE_TEMP_INTERVAL;
  }
  
}

// replaces normal delay to check if main buttons are pressed while the delay is running
void delayWithButtonCheck(int tiempoDelay) {
  for (int i=0;i<tiempoDelay;i++){
    checkIfSetTime();
    checkIfSetAlarm();
    checkButtonAlarm();
    delay(1);
  }
}

// awesome monkey island 2 music for alarm
void musicaFeliz() {

  tone(BUZZ, NOTE_C5, 1400); if (checkSnooze(1400)) { return; }
  tone(BUZZ, NOTE_E5, 200); if (checkSnooze(200)) { return; }
  tone(BUZZ, NOTE_G5, 400); if (checkSnooze(400)) { return; }
  tone(BUZZ, NOTE_G5, 800); if (checkSnooze(800)) { return; }
  tone(BUZZ, NOTE_G5, 400); if (checkSnooze(400)) { return; }
  tone(BUZZ, NOTE_A5, 800); if (checkSnooze(800)) { return; }
  tone(BUZZ, NOTE_F5, 600); if (checkSnooze(600)) { return; }
  tone(BUZZ, NOTE_A5, 200); if (checkSnooze(200)) { return; }
  tone(BUZZ, NOTE_G5, 400); if (checkSnooze(400)) { return; }
  tone(BUZZ, NOTE_E5, 800); if (checkSnooze(800)) { return; }
  tone(BUZZ, NOTE_D5, 400); if (checkSnooze(400)) { return; }
  tone(BUZZ, NOTE_C5, 800); if (checkSnooze(800)) { return; }
  tone(BUZZ, NOTE_E5, 600); if (checkSnooze(600)) { return; }
  tone(BUZZ, NOTE_C5, 200); if (checkSnooze(200)) { return; }
  tone(BUZZ, NOTE_D5, 400); if (checkSnooze(400)) { return; }
  tone(BUZZ, NOTE_D5, 400); if (checkSnooze(400)) { return; }
  tone(BUZZ, NOTE_G5, 200); if (checkSnooze(200)) { return; }
  tone(BUZZ, NOTE_E5, 400); if (checkSnooze(400)) { return; }
  tone(BUZZ, NOTE_D5, 200); if (checkSnooze(200)) { return; }
  tone(BUZZ, NOTE_C5, 1200); if (checkSnooze(1200)) { return; }
  tone(BUZZ, NOTE_E5, 200); if (checkSnooze(200)) { return; }
  tone(BUZZ, NOTE_C5, 200); if (checkSnooze(200)) { return; }
  tone(BUZZ, NOTE_D5, 1200); if (checkSnooze(1200)) { return; }

  tone(BUZZ, NOTE_G5, 400); if (checkSnooze(400)) { return; }

  tone(BUZZ, NOTE_E5, 800); if (checkSnooze(800)) { return; }
  tone(BUZZ, NOTE_C5, 600); if (checkSnooze(600)) { return; }
  tone(BUZZ, NOTE_E5, 200); if (checkSnooze(200)) { return; }
  tone(BUZZ, NOTE_G5, 400); if (checkSnooze(400)) { return; }
  tone(BUZZ, NOTE_G5, 800); if (checkSnooze(800)) { return; }
  tone(BUZZ, NOTE_G5, 400); if (checkSnooze(400)) { return; }
  tone(BUZZ, NOTE_A5, 800); if (checkSnooze(800)) { return; }
  tone(BUZZ, NOTE_C6, 600); if (checkSnooze(600)) { return; }
  tone(BUZZ, NOTE_A5, 200); if (checkSnooze(200)) { return; }
  tone(BUZZ, NOTE_G5, 400); if (checkSnooze(400)) { return; } 
  tone(BUZZ, NOTE_E5, 800); if (checkSnooze(800)) { return; }
  tone(BUZZ, NOTE_D5, 400); if (checkSnooze(400)) { return; }
  tone(BUZZ, NOTE_C5, 800); if (checkSnooze(800)) { return; }
  tone(BUZZ, NOTE_E5, 600); if (checkSnooze(600)) { return; }
  tone(BUZZ, NOTE_C5, 200); if (checkSnooze(200)) { return; }
  tone(BUZZ, NOTE_D5, 400); if (checkSnooze(400)) { return; }
  tone(BUZZ, NOTE_D5, 800); if (checkSnooze(800)) { return; } 
  tone(BUZZ, NOTE_E5, 200); if (checkSnooze(200)) { return; }
  tone(BUZZ, NOTE_D5, 200); if (checkSnooze(200)) { return; }
  tone(BUZZ, NOTE_C5, 2400); if (checkSnooze(2400)) { return; }

}

// check if the snooze button is pressed
boolean checkSnooze(int tiempoDelay) {
  int buttonState = digitalRead(BUTTON_SNOOZE);
  boolean out = false;
  for (int i=0;i<tiempoDelay;i++){
    if(buttonState == LOW) {
      out = true;
      snooze=true;
      break;
    }
    delay(1);
  }
  return out;
}

// check if the set time button is pressed
void checkIfSetTime() {
  int buttonState = digitalRead(BUTTON_TIME);
  boolean firstPush=true;
  
  while(buttonState == LOW) {
    int horicas=dt.hour;
    int minuten=dt.minute;
    char userTime[8];
    if (++minuten>59) { minuten=0; horicas++; if (horicas>23) { horicas=0; } }
    userTime[0] = horicas / 10 + '0';
    userTime[1] = horicas % 10 + '0';
    userTime[2] = ':';
    userTime[3] = minuten / 10 + '0';
    userTime[4] = minuten % 10 + '0';
    userTime[5] = ':';
    userTime[6] = '0';
    userTime[7] = '0';
    clock2.setDateTime(__DATE__, userTime);
    dt = clock2.getDateTime();
    display.showNumberDecEx((dt.hour*100)+dt.minute, 0b11100000, true, 4, 0);
    delay(15); if (firstPush) { delay(485); firstPush=false; }
    buttonState = digitalRead(BUTTON_TIME);
  } 
}

// check if the set alarm button is pressed
void checkIfSetAlarm() {
  int buttonState = digitalRead(BUTTON_ALARM);
  boolean firstPush=true;
  boolean saveChanges=false;
  
  while(buttonState == LOW) {
    if (++minuteAlarm>59) { minuteAlarm=0; hourAlarm++; if (hourAlarm>23) { hourAlarm=0; } }

    saveChanges=true;
    
    display.showNumberDec((hourAlarm*100)+minuteAlarm, true, 4, 0);
    delay(15); if (firstPush) { delay(485); firstPush=false; }
    buttonState = digitalRead(BUTTON_ALARM);
  }

  if (saveChanges) { // save alarm time in eeprom when it changes
    EEPROM.write(0, hourAlarm);
    EEPROM.write(2, minuteAlarm);
  }

}

// check if the snooze button is pressed (used when alarm is not running), this switch the alarm on/off
void checkButtonAlarm() {
  int buttonState = digitalRead(BUTTON_SNOOZE);
  
  if(buttonState == LOW) {
    if (alarm) {
      alarm=false;
      delay(400);
    } else {
      alarm=true;
      tone(BUZZ, NOTE_G5, 400); delay(400);
    }
  }

}

// calculate and show the temperature in the display
void displayTemp(){
  
  int tempReading = analogRead(TEMP_APIN);
  double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );
  int tempC = tempK - 275.00; //tempK - 273.15
  //float tempF = (tempC * 9.0)/ 5.0 + 32.0;

  if (tempC<0) {
    uint8_t data[] = { table7segments[18], table7segments[tempC % 10], table7segments[17], table7segments[12] };
    display.setSegments(data);
  } else {
    uint8_t data[] = { table7segments[tempC / 10], table7segments[tempC % 10], table7segments[17], table7segments[12] };
    display.setSegments(data);
  }

}
