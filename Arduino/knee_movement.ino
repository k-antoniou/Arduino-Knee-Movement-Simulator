#include <Servo.h>

Servo myservo;  

int pos = 0;    

void setup() {
  myservo.attach(9);  
}

//The targeted knee movement is between 0-130 degrees
void loop() {
  for (pos = 0; pos <= 130; pos += 1) {
    myservo.write(pos);             
    delay(15);                       
  }
  
//The reverse movement
  for (pos = 130; pos >= 0; pos -= 1) { 
    myservo.write(pos);              
    delay(15);     
    }

}
