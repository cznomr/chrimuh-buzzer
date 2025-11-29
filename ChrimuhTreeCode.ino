#include "pitches.h"

#define melodyPin 9

// Jingle Bells
int melody[] = {
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
  NOTE_E5,
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
  NOTE_D5, NOTE_G5
};

int tempo[] = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8,
  2,
  8, 8, 8, 8,
  8, 8, 8, 16, 16,
  8, 8, 8, 8,
  4, 4
};

// We wish you a merry Christmas
int wish_melody[] = {
  NOTE_B3, 
  NOTE_F4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4,
  NOTE_D4, NOTE_D4, NOTE_D4,
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4,
  NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_A4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4,
  NOTE_F4, NOTE_D4, NOTE_B3, NOTE_B3,
  NOTE_D4, NOTE_G4, NOTE_E4,
  NOTE_F4
};

int wish_tempo[] = {
  4,
  4, 8, 8, 8, 8,
  4, 4, 4,
  4, 8, 8, 8, 8,
  4, 4, 4,
  4, 8, 8, 8, 8,
  4, 4, 8, 8,
  4, 4, 4,
  2
};

// Santa Claus is coming to town
int santa_melody[] = {
  NOTE_G4,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, NOTE_C5,
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4,
  NOTE_A4, NOTE_G4, NOTE_F4, NOTE_F4,
  NOTE_E4, NOTE_G4, NOTE_C4, NOTE_E4,
  NOTE_D4, NOTE_F4, NOTE_B3,
  NOTE_C4
};

int santa_tempo[] = {
  8,
  8, 8, 4, 4, 4,
  8, 8, 4, 4, 4,
  8, 8, 4, 4, 4,
  8, 8, 4, 2,
  4, 4, 4, 4,
  4, 2, 4,
  1
};

int switchOne = 0;
int switchTwo = 0;
int switchThree = 0;

int song = 0;

void setup(void) {
  pinMode(melodyPin, OUTPUT); // Buzzer
  //pinMode(13, OUTPUT);        // LED indicator when singing a note
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);        // extra LEDs
  pinMode(7, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);




  pinMode(2, INPUT);          // Button 1 (external pulldown)
  pinMode(3, INPUT);          // Button 2
  pinMode(4, INPUT);          // Button 3

  Serial.begin(9600);
}

void loop() {
  switchOne   = digitalRead(2);
  switchTwo   = digitalRead(3);
  switchThree = digitalRead(4);

  if (switchOne == HIGH) {
    sing(1);          // Jingle Bells
  } else if (switchTwo == HIGH) {
    sing(2);          // Santa Claus is coming to town
  } else if (switchThree == HIGH) {
    sing(3);          // We wish you a Merry Christmas
  }
}

void sing(int s) {
  song = s;

  if (song == 3) {
    Serial.println(" 'We wish you a Merry Christmas'");
    int size = sizeof(wish_melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {
      int noteDuration = 1000 / wish_tempo[thisNote];

      buzz(melodyPin, wish_melody[thisNote], noteDuration);

      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // rest / silence
      buzz(melodyPin, 0, noteDuration);
    }

  } else if (song == 2) {
    Serial.println(" 'Santa Claus is coming to town'");
    int size = sizeof(santa_melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {
      int noteDuration = 900 / santa_tempo[thisNote];

      buzz(melodyPin, santa_melody[thisNote], noteDuration);

      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // rest / silence
      buzz(melodyPin, 0, noteDuration);
    }

  } else {
    Serial.println(" 'Jingle Bells'");
    int size = sizeof(melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {
      int noteDuration = 1000 / tempo[thisNote];

      buzz(melodyPin, melody[thisNote], noteDuration);

      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      // rest / silence
      buzz(melodyPin, 0, noteDuration);
    }
  }
}

void buzz(int targetPin, long frequency, long length) {
  // Handle "no sound" safely: frequency 0 = rest
  if (frequency <= 0) {
    digitalWrite(targetPin, LOW); // make sure buzzer is off
    // turn all LEDs off during rest
    //digitalWrite(13, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    digitalWrite(7, LOW);
    digitalWrite(12, LOW);
    digitalWrite(6, LOW);
    digitalWrite(5, LOW);

    delay(length);                // just wait silently
    return;
  }

  // SIMPLE SOFTWARE "VOLUME" TRICK:
  // push the buzzer closer to its loud resonant range by raising frequency
  long boostedFreq = frequency * 2;     // one octave up
  if (boostedFreq > 8000) boostedFreq = 8000;  // clamp to something reasonable

  // turn all LEDs on while buzzing
  //digitalWrite(13, HIGH);
  digitalWrite(11, HIGH);
  digitalWrite(10, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);

  long delayValue = 1000000L / boostedFreq / 2;   // us between transitions
  long numCycles  = boostedFreq * length / 1000L; // total cycles

  for (long i = 0; i < numCycles; i++) {
    digitalWrite(targetPin, HIGH);
    delayMicroseconds(delayValue);
    digitalWrite(targetPin, LOW);
    delayMicroseconds(delayValue);
  }

  // turn all LEDs off at the end of the note
  //digitalWrite(13, LOW);
  digitalWrite(11, LOW);
  digitalWrite(10, LOW);
  digitalWrite(7, LOW);
  digitalWrite(12, LOW);
  digitalWrite(6, LOW);
  digitalWrite(5, LOW);
}

/*
tnias 11.28.25
*/
