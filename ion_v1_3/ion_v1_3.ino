

// Iontophoresis Electro Antiperspirant v3.5
// Code and Circuit by Chris Hawkins 2014 - Updated 2020
//
#define ledPWR 13 //PowerLED
#define button  12 //Start/Stop button
#define phasePolA 7 //LED stage 1
#define phasePolB 8 //LED Stage 2
#define pwrForward 9 // Trigger A
#define pwrBackward 10 // trigger B reverse polarity
#define phaseDuration 900000 // duration for each hand default 15 minutes (for 20 minutes =1200000)

int phaseNumber = 0 ;
unsigned long lastpressed = 0;
//volatile int state = LOW;
//unsigned long lastpressed1 = 0;
int buttonstate = LOW;
//int startedPower = LOW;

// import lib for tunes
#include "pitches.h"

int melodyEnd[] = {
  NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_C4
};
int melodyStart[] = {
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5
};
int melodyStop[] = {
  NOTE_G4, NOTE_C4
};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurationsEnd[] = {
  4, 8, 8, 4, 4, 4, 4, 8, 8, 4
};
int noteDurationsStart[] = {
  4, 4, 4, 2
};
int noteDurationsStop[] = {
  4, 2
};

#define pinSpeaker 3

unsigned long ulngTimer = 0;

void setup() {
  //  Serial.begin(9600);
  // LEDS
  pinMode (pwrForward, OUTPUT);
  pinMode (pwrBackward, OUTPUT);
  pinMode (ledPWR, OUTPUT);

  //Switch polarity
  pinMode (phasePolA, OUTPUT);
  pinMode (phasePolB, OUTPUT);

  //Switches
  pinMode (button, INPUT);

  //Reset outputs to low
  alloutputslow ();

  //night rider leds
  fancyStartUp();

}

void fancyStartUp() {
  digitalWrite (ledPWR, HIGH);
  delay (300);
  digitalWrite (ledPWR, LOW);
  digitalWrite (phasePolA, HIGH);
  delay (300);
  digitalWrite (phasePolA, LOW);
  digitalWrite (phasePolB, HIGH);
  delay (300);
  digitalWrite (phasePolA, HIGH);
  digitalWrite (phasePolB, LOW);
  delay (300);
  digitalWrite (ledPWR, HIGH);
  digitalWrite (phasePolA, LOW);
}

void loop() {

  buttonstate = digitalRead (button);
  if (buttonstate == HIGH && millis() - lastpressed > 500) {

    if (phaseNumber == 0) {
      //Serial.print ("P1\n");
      phaseOne ();
    }   else
    {
      if (phaseNumber == 1 || phaseNumber == 2) { // Stop phases
        alloutputslow ();
        ulngTimer = 0;
        phaseNumber = 0;
        flashLEDsEnd();
        toneStop();
      }
    }
    lastpressed = millis();
  }

  if (ulngTimer  > 0 ) {

    if (phaseNumber == 1) {

      if ((millis() - ulngTimer ) >  phaseDuration ) {

        phaseTwo ();

      }
    }
    if (phaseNumber == 2) {

      if ((millis() - ulngTimer ) > phaseDuration ) {
        //Serial.print ("P END\n");
        alloutputslow ();
        ulngTimer = 0;
        phaseNumber = 0;
        flashLEDsEnd();
        toneFinish();

      }
    }
  }
} //end loop

void phaseOne () {
  alloutputslow();
  flashLEDsStart ();
  digitalWrite (phasePolA, HIGH);
  digitalWrite (pwrForward, HIGH);
  phaseNumber = 1;
  ulngTimer = millis();
  toneStart();//play start tune
  //Serial.print ("P1 Started\n");

} // PahseOne

void phaseTwo () {

  alloutputslow();

  digitalWrite (phasePolA, HIGH);
  digitalWrite (phasePolB, HIGH);
  delay(10000); // pause before moving on
  digitalWrite (pwrBackward, HIGH);
  phaseNumber = 2;
  ulngTimer = millis();
} // PahseOne



void alloutputslow() {
  digitalWrite (phasePolA, LOW);
  digitalWrite (phasePolB, LOW);
  digitalWrite (pwrForward, LOW);
  digitalWrite (pwrBackward, LOW);
}

void flashLEDsStart () {
  for (int x = 0; x < 10; x++) {
    digitalWrite (phasePolA, HIGH);
    digitalWrite (phasePolB, LOW);
    delay(200);
    digitalWrite (phasePolA, LOW);
    digitalWrite (phasePolB, HIGH);
    delay(200);
  }
  digitalWrite (phasePolA, LOW);
  digitalWrite (phasePolB, LOW);
}

void flashLEDsEnd () {
  for (int x = 0; x < 10; x++) {
    digitalWrite (phasePolA, HIGH);
    digitalWrite (phasePolB, HIGH);
    delay(200);
    digitalWrite (phasePolA, LOW);
    digitalWrite (phasePolB, LOW);
    delay(200);
  }
}

void toneStart() {
  for (int thisNote = 0; thisNote < 4; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurationsStart[thisNote];
    tone(pinSpeaker, melodyStart[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(pinSpeaker);

  }
}

void toneFinish () {
  for (int thisNote = 0; thisNote < 10; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurationsEnd[thisNote];
    tone(pinSpeaker, melodyEnd[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(pinSpeaker);
  }
}
void toneStop () {
  for (int thisNote = 0; thisNote < 2; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurationsStop[thisNote];
    tone(pinSpeaker, melodyStop[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(pinSpeaker);
  }
}
