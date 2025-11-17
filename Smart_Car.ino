/* Libraries and Hardware Declarations */
#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

// ===== LCD 1602 (I2C) =====
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ===== Servo =====
Servo myServo;
const int SERVO_PIN = 11; // Use const int instead of define for pin

// ===== Motor Pins (L298N) =====
const int IN1 = 6;
const int IN2 = 7;
const int IN3 = 5;
const int IN4 = 4;

// ===== Joystick =====
const int JOY_X = A0;
const int JOY_Y = A1;
const int JOY_SW = 2;

// ===== Mode Indicator LEDs =====
const int LED_GREEN = 12; // Manual mode
const int LED_RED = 13;   // Auto mode

// ===== Ultrasonic Sensor SRF05 =====
const int TRIG_PIN = 9;
const int ECHO_PIN = 10;

// ===== Buzzer =====
const int BUZZER_PIN = 3;

// ===== Global Variables =====
bool isAutoMode = false;
bool lastAutoMode = false; // Used to detect mode changes

// ====== Setup ======
void setup() {
  // Motor setup
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // LEDs
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  // Joystick
  pinMode(JOY_SW, INPUT_PULLUP);

  // Ultrasonic sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Buzzer
  pinMode(BUZZER_PIN, OUTPUT);

  // LCD
  lcd.init();
  lcd.backlight();

  // Serial debug
  Serial.begin(9600);

  // Initial mode: Manual (updated later in loop)
  myServo.attach(SERVO_PIN);
  updateModeDisplay(); // Display initial mode
}

// ====== Distance Measurement Function ======
long getDistance(byte angle) {
  myServo.write(angle);
  delay(50); // Wait for servo to reach position

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Timeout added to avoid lock if no echo is received
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30 ms timeout

  if (duration == 0) {
    return 999; // Return large value on timeout
  }

  long dist = duration * 0.034 / 2; // Convert to cm
  return dist;
}

// ====== Car Movement Functions ======
void moveForward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// ====== Update LCD & LED Display ======
void updateModeDisplay() {
  if (isAutoMode) {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Mode: Auto    ");
  } else {
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Mode: Manual  ");
    lcd.setCursor(0, 1); // Clear second line in manual mode
    lcd.print("                ");
  }
}

// ====== MAIN LOOP ======
void loop() {

  // === 1. Handle mode switching with debounce ===
  if (digitalRead(JOY_SW) == LOW) {
    delay(50); // Debounce
    if (digitalRead(JOY_SW) == LOW) {
      isAutoMode = !isAutoMode; // Toggle mode

      // Wait until button is released
      while (digitalRead(JOY_SW) == LOW) {
        // Hold
      }
    }
  }

  // === 2. Update display only when mode changes ===
  if (isAutoMode != lastAutoMode) {
    stopCar();          
    updateModeDisplay(); 
    lastAutoMode = isAutoMode;
  }

  // === 3. Mode-specific logic ===
  if (isAutoMode) {

    // === Auto Mode ===
    long distForward = getDistance(90);
    lcd.setCursor(0, 1);
    lcd.print("Dist: ");
    lcd.print(distForward);
    lcd.print("cm   ");

    if (distForward > 20) {
      moveForward();
      digitalWrite(BUZZER_PIN, LOW);
    } else {
      stopCar();
      digitalWrite(BUZZER_PIN, HIGH);
      delay(300);

      moveBackward();
      delay(500);
      stopCar();
      delay(100);

      long distLeft = getDistance(180);
      delay(50);
      long distRight = getDistance(0);

      myServo.write(90);
      delay(50);

      if (distLeft > distRight) {
        turnLeft();
        delay(700);
      } else {
        turnRight();
        delay(700);
      }
      stopCar();
      digitalWrite(BUZZER_PIN, LOW);
    }

  } else {

    // === Manual Mode ===
    int xVal = analogRead(JOY_X);
    int yVal = analogRead(JOY_Y);

    // Debug joystick values
    Serial.print("X: "); Serial.print(xVal);
    Serial.print(" Y: "); Serial.println(yVal);

    if (yVal > 700) moveForward();
    else if (yVal < 300) moveBackward();
    else if (xVal < 300) turnRight();
    else if (xVal > 700) turnLeft();
    else stopCar();
  }
}
