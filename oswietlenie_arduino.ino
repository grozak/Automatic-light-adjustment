#include <myI2C.h>
#include <SoftwareSerial.h>

myI2C bh1750; // Czujnik BH1750
uint16_t my_measure;

// RX to TX, TX to RX
SoftwareSerial BTSerial(10, 11); // TX / RX
char cmd[10];
int cmd_index = 0;

int ledPin = 9;

boolean hold_mode = false;
uint16_t hold_value = 0;
int diff;

int output = 0;
int output_percent = 0;

void setup() {
  BTSerial.begin(9600);
  Serial.begin(9600);         
  Serial.println("Uruchomiono czujnik.");
  pinMode(ledPin, OUTPUT);
}


void loop() {
  if (BTSerial.available()) {
    char ch = (char) BTSerial.read();
    if(ch == '\n') {
      cmd[cmd_index]='\n';
      Serial.write(cmd);
      cmd[cmd_index]=0;
      exec();
      cmd_index=0;
      clear_cmd();
    }
    else {
      cmd[cmd_index] = ch;
      if(cmd_index<99) cmd_index++;
    }
  }

  if(hold_mode) {
    my_measure = bh1750.readOnce();
    my_measure/=1.2;
    if(my_measure < hold_value) {
      diff = hold_value - my_measure;
      if(diff>100) {
        output+=5;
      }
      else if(diff>10) {
        output+=2;
      }
      else if(diff>0) {
        output+=1;
      }
      if(output>255) {
        output = 255;
      }
    }
    else if(my_measure > hold_value) {
      diff = my_measure - hold_value;
      if(diff>100) {
        output-=5;
      }
      else if(diff>10) {
        output-=2;
      }
      else if(diff>0) {
        output-=1;
      }
      if(output<0) {
        output = 0;
      }
    }
    //DEBUG
    Serial.print("Measurement: ");
    Serial.print(my_measure);
    Serial.print("   Output: ");
    Serial.print(output);
    Serial.print("   Hold_value: ");
    Serial.println(hold_value);
  }
  analogWrite(ledPin, output);
}



void clear_cmd() {
  for(int i=0;i<10;i++) {
    cmd[i]=0;
  }
}

void exec() {
  if(cmd[0] == 'D') {
    hold_mode = false;
    output_percent = atoi(cmd+2);
    output=output_percent*2.55;
    analogWrite(ledPin, output);
  }
  else if (cmd[0] == 'H') {
    hold_mode = true;
    hold_value = atoi(cmd+2);
  }

  else if (cmd[0] == 'C') {
    String report;
    String value1_string;
    String value2_string;
    if(hold_mode) {
      output_percent = output / 2.55;
      value1_string = String(hold_value, DEC);
      value2_string = String(output_percent, DEC);
      report = String("H " + value1_string + " " + value2_string + " \n");
    }
    else {
      my_measure = bh1750.readOnce();
      my_measure/=1.2;

      value1_string = String(output_percent, DEC);
      value2_string = String(my_measure, DEC);
      report = String("D " + value1_string + " " + value2_string + " \n");
    }
      BTSerial.print(report);
  }
}
