/*
 *   Copyright (C) 2013 by Jonathan Naylor G4KLX
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


// 0 and 1 are used for serial
const int RFTRANSMIT  = 2;
const int EXTTRANSMIT = 3;
const int HEARTBEAT1  = 4;
const int HEARTBEAT2  = 13;
const int ACTIVE      = 5;
const int OUTPUT1     = 6;
const int OUTPUT2     = 7;
const int OUTPUT3     = 8;
const int OUTPUT4     = 9;


const int RFSQUELCH1  = A0;
const int RFSQUELCH2  = A1;
const int EXTSQUELCH  = A2;
const int BATTERY     = A3;
const int DISABLE     = A4;


void setup() {
  // Set up the outut pins with pull ups
  pinMode(RFSQUELCH1, INPUT);
  pinMode(RFSQUELCH2, INPUT);
  pinMode(EXTSQUELCH, INPUT);
  pinMode(BATTERY,    INPUT);
  pinMode(DISABLE,    INPUT);

  digitalWrite(RFSQUELCH1, HIGH);
  digitalWrite(RFSQUELCH2, HIGH);
  digitalWrite(EXTSQUELCH, HIGH);
  digitalWrite(BATTERY,    HIGH);
  digitalWrite(DISABLE,    HIGH);

  // Set up the output pins and set low
  pinMode(RFTRANSMIT,  OUTPUT);
  pinMode(EXTTRANSMIT, OUTPUT);
  pinMode(HEARTBEAT1,  OUTPUT);
  pinMode(HEARTBEAT2,  OUTPUT);
  pinMode(ACTIVE,      OUTPUT);
  pinMode(OUTPUT1,     OUTPUT);
  pinMode(OUTPUT2,     OUTPUT);
  pinMode(OUTPUT3,     OUTPUT);
  pinMode(OUTPUT4,     OUTPUT);

  digitalWrite(RFTRANSMIT,  LOW);
  digitalWrite(EXTTRANSMIT, LOW);
  digitalWrite(HEARTBEAT1,  LOW);
  digitalWrite(HEARTBEAT2,  HIGH);
  digitalWrite(ACTIVE,      LOW);
  digitalWrite(OUTPUT1,     LOW);
  digitalWrite(OUTPUT2,     LOW);
  digitalWrite(OUTPUT3,     LOW);
  digitalWrite(OUTPUT4,     LOW);

  // Set up the serial port
  Serial.begin(19200);
}

void loop() {
  if (Serial.available() > 0) {
    int out = Serial.read();

    digitalWrite(RFTRANSMIT,  (out & 0x01) == 0x01 ? HIGH : LOW);
    digitalWrite(EXTTRANSMIT, (out & 0x02) == 0x02 ? HIGH : LOW);
    digitalWrite(HEARTBEAT1,  (out & 0x04) == 0x04 ? HIGH : LOW);
    digitalWrite(HEARTBEAT2,  (out & 0x04) == 0x04 ? HIGH : LOW);
    digitalWrite(ACTIVE,      (out & 0x08) == 0x08 ? HIGH : LOW);
    digitalWrite(OUTPUT1,     (out & 0x10) == 0x10 ? HIGH : LOW);
    digitalWrite(OUTPUT2,     (out & 0x20) == 0x20 ? HIGH : LOW);
    digitalWrite(OUTPUT3,     (out & 0x40) == 0x40 ? HIGH : LOW);
    digitalWrite(OUTPUT4,     (out & 0x80) == 0x80 ? HIGH : LOW);
  }

  byte val = 0x00;
  val |= (digitalRead(RFSQUELCH1) == LOW) ? 0x01 : 0x00;
  val |= (digitalRead(RFSQUELCH2) == LOW) ? 0x02 : 0x00;
  val |= (digitalRead(EXTSQUELCH) == LOW) ? 0x04 : 0x00;
  val |= (digitalRead(BATTERY)    == LOW) ? 0x08 : 0x00;
  val |= (digitalRead(DISABLE)    == LOW) ? 0x10 : 0x00;

  Serial.write(val);
 
  delay(20);      // 20ms
}

