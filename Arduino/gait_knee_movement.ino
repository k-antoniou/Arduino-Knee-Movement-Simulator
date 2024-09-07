#include <Servo.h>

// Gait Phases Knee Angles
#define KNEE_INITIAL_CONTACT 0
#define KNEE_FOOT_FLAT 15
#define KNEE_MIDSTANCE 5
#define KNEE_HEEL_OFF 0
#define KNEE_TOE_OFF 30
#define KNEE_EARLY_SWING 60
#define KNEE_MID_SWING 30
#define KNEE_LATE_SWING 0

// Gait Phases
enum phases{
  PHASE_INITIAL_CONTACT = 0,
  PHASE_FOOT_FLAT,
  PHASE_MIDSTANCE,
  PHASE_HEEL_OFF,
  PHASE_TOE_OFF,
  PHASE_EARLY_SWING,
  PHASE_MID_SWING,
  PHASE_LATE_SWING
};

const int flexPin = A0; // Analog input pin for flex sensor
Servo myservo;

int nextPhase = PHASE_INITIAL_CONTACT; // The next (expected) gait phase
int kneeAngle = 0; // The knee angle for the opposite leg

void setup() {
  pinMode(flexPin, INPUT); // Set the Pin as an Input
  myservo.attach(9, 1000, 2000); // Attach the servo to pin 9 with the correct pulse width range
  Serial.begin(9600);
}

void loop() {
  int val = analogRead(flexPin); // Read the analog value from the sensor

  // Map the flex sensor value to an angle between 0 and 90 degrees
  int flexAngle = map(val, 668, 327, 0, 90);

  Serial.print("Flex Angle: ");
  Serial.println(flexAngle);

  // Determine the knee angle based on the next phase and flex angle
  switch (nextPhase) {
    case PHASE_INITIAL_CONTACT:
      if (KNEE_INITIAL_CONTACT <= flexAngle && flexAngle < KNEE_FOOT_FLAT) {
        nextPhase = PHASE_FOOT_FLAT;     // Move to Toe Off
        kneeAngle = KNEE_TOE_OFF;
      }
      break;

    case PHASE_FOOT_FLAT:
      if (KNEE_MIDSTANCE < flexAngle && flexAngle <= KNEE_FOOT_FLAT + 5) {
        nextPhase = PHASE_MIDSTANCE; // Move to Early Swing
        kneeAngle = KNEE_EARLY_SWING;
      }
      break;

    case PHASE_MIDSTANCE:
      if (KNEE_HEEL_OFF <= flexAngle && flexAngle < KNEE_MIDSTANCE) {
        nextPhase = PHASE_HEEL_OFF;   // Move to Mid Swing
        kneeAngle = KNEE_MID_SWING;
      }
      break;

    case PHASE_HEEL_OFF:
      if (KNEE_HEEL_OFF <= flexAngle && flexAngle < KNEE_TOE_OFF ) {
        nextPhase = PHASE_TOE_OFF;  // Move to Late Swing
        kneeAngle = KNEE_LATE_SWING;
      }
      break;

    case PHASE_TOE_OFF:
      if (KNEE_TOE_OFF <= flexAngle && flexAngle < KNEE_EARLY_SWING) {
        nextPhase = PHASE_EARLY_SWING; // Move to Initial Contact
        kneeAngle = KNEE_INITIAL_CONTACT;
      }
      break;

    case PHASE_EARLY_SWING:
      if (KNEE_MID_SWING + 10 < flexAngle && flexAngle <= KNEE_EARLY_SWING) {
        nextPhase = PHASE_MID_SWING;   // Move to Foot Flat
        kneeAngle = KNEE_FOOT_FLAT;
      }
      break;

    case PHASE_MID_SWING:
      if (KNEE_LATE_SWING + 5 < flexAngle && flexAngle <= KNEE_MID_SWING) {
        nextPhase = PHASE_LATE_SWING;   // Move to Midstance
        kneeAngle = KNEE_MIDSTANCE;
      }
      break;

    case PHASE_LATE_SWING:
      if (flexAngle == KNEE_HEEL_OFF) {
        nextPhase = PHASE_INITIAL_CONTACT;    // Move to Heel Off
        kneeAngle = KNEE_HEEL_OFF;
      }
      break;
  }

  // Move servomotor to the knee angle
  smoothServoMove(kneeAngle);

  // Print the current knee angle
  Serial.print("Knee Angle: ");
  Serial.println(kneeAngle);

  delay(1000);
}

// Function to move the servo smoothly to a target angle
void smoothServoMove(int targetAngle) {
  int currentAngle = myservo.read(); // Current position of the servo

  if (currentAngle < targetAngle) {
    for (int angle = currentAngle; angle <= targetAngle; angle++) {
      myservo.write(angle);
      delay(15); 
    }
  } else if (currentAngle > targetAngle) {
    for (int angle = currentAngle; angle >= targetAngle; angle--) {
      myservo.write(angle);
      delay(15);
    }
  }
}
