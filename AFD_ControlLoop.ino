// Include the motor shield V1 library
#include <AFMotor.h>
#include <Servo.h>

Servo Servo1;

int servo_pos = 0;

// Define the pins for control button
#define pushButton A1
#define IRpin A0
#define LEDpin A4
//#define eStop A2

int IRval = 1;
int count = 0;
int running = 1;
int running2 = 1;

bool pushButtonState = HIGH;
//bool eStopState = HIGH;

// Connect the DC motor to M1 on the motor control board and pump to M4
AF_DCMotor DCmotor(1);
AF_DCMotor pump(4);

void setup() {
  // Set up serial communication to print things to the serial monitors
  Serial.begin(9600);
  pinMode(pushButton, INPUT_PULLUP);
  pinMode(IRpin, INPUT);
  pinMode(LEDpin, OUTPUT);
  Servo1.attach(9);
  //pinMode(eStop, INPUT_PULLUP);
}

void loop() {

  while (running = 1 && running2 = 1) {
  Serial.print("pushButtonState: ");
  Serial.println(pushButtonState);

  while (pushButtonState == HIGH) {
    pushButtonState = digitalRead(pushButton);
  }

  Serial.print("pushButtonState: ");
  Serial.println(pushButtonState);

  if (pushButtonState == LOW) {
    //SET DC MOTOR INTO MOTION
    DCmotor.setSpeed(50);
    DCmotor.run(FORWARD);
    Serial.println("Motor started.");

    eStopState = digitalRead(eStop);

    delay(1000);

    IRval = digitalRead(IRpin);  // !!! IMPORTANT NOTE: The value will be 1 if no object is detcted, and 0 if object is present. !!!
    Serial.print("IR reading:");
    Serial.println(IRval);

    while (IRval == 1) {
      IRval = digitalRead(IRpin);  // !!! IMPORTANT NOTE: The value will be 1 if no object is detcted, and 0 if object is present. !!!
      Serial.print("IR reading:");
      Serial.println(IRval);
      eStopState = digitalRead(eStop);

    if (eStop == LOW) {
      running2 = 0;
      Serial.print("running2 set to 0.");
      break;
    }
    }

    //IF THERE IS A TEST TUBE PRESENT
    if (IRval == 0) {

      //STOP DC MOTOR
      DCmotor.run(RELEASE);

      Serial.println("Motor stopped.");

      //TURN SERVO AWAY FROM THE CONE
      for (servo_pos = 0; servo_pos <= 45; servo_pos += 1) {
        Servo1.write(servo_pos);
        delay(20);
        eStopState = digitalRead(eStop);

    if (eStop == LOW) {
      Servo2.write(45);
      Serial.println("Servo stopped due to eStop.");
      running2 = 0;
      Serial.print("running2 set to 0.");
      break;
    }
      }

      Serial.println("Servo turning away.");

      //LET POWDER FLOW FOR ??? SECONDS
      delay(5000);

      Serial.println("Servo flowing.");

      //CLOSE SERVO BACK TOWARDS THE CONE
      for (servo_pos = 45; servo_pos >= 0; servo_pos -= 1) {
        Servo1.write(servo_pos);
        delay(20);
        eStopState = digitalRead(eStop);

if (eStop == LOW) {
      Servo2.write(45);
      Serial.println("Servo stopped due to eStop.");
      running2 = 0;
      Serial.print("running2 set to 0.");
      break;
    }
      }

      Serial.println("Servo turning back");

      //START PUMP
      pump.run(FORWARD);
      pump.setSpeed(255);

      eStopState = digitalRead(eStop);

    if (eStop == LOW) {
      pump.setSpeed(RELEASE);
      Serial.println("Pump stopped due to eStop.");
      running2 = 0;
      Serial.print("running2 set to 0.");
      break;
    }

      //LET PUMP RUN FOR ??? SECONDS
      delay(5000);

      Serial.println("Pump running");

      //STOP PUMP
      pump.run(RELEASE);

      Serial.println("Pump stopped");

      count++;

      Serial.print("Count: ");
      Serial.println(count);

      if (count = 5) {
        running = 0;
        Serial.println("running set to 0.");
      }

    }
  }
}
Serial.print("running: ");
Serial.println(running);
Serial.print("running2: ");
Serial.println(running2);
delay(100);
}