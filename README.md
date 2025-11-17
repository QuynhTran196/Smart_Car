# Smart Robot Car: Dual-Mode (Manual + Autonomous)

This is an Arduino-based smart robot car featuring two independent operating modes. This project was built to demonstrate skills in microcontroller programming, sensor integration, and hardware design.

* **Manual Mode:** The car is controlled directly via a joystick module.
* **Autonomous Mode:** The car uses an ultrasonic sensor to navigate automatically and avoid obstacles.

---

## Key Features

* **Dual-Mode Operation:** Seamlessly switch between Manual and Auto modes by pressing the joystick's built-in button.
* **Visual Feedback:**
    * **LEDs:** A green LED indicates Manual Mode, and a red LED indicates Auto Mode.
    * **LCD 1602:** The screen clearly displays the current mode and, in Auto Mode, the distance to the nearest obstacle.
* **Obstacle Avoidance (Auto Mode):**
    * The car moves forward by default.
    * An ultrasonic sensor (SRF05) mounted on a servo scans for obstacles.
    * If an obstacle is detected within 20cm, the car stops, activates a buzzer, reverses, and then scans left and right to find the clearest path before turning.
* **Manual Control:**
    * Uses the KY-023 Joystick for intuitive control (forward, reverse, left, right).

---

## Bill of Materials (Hardware)

* 1x Arduino (Uno, Nano, or compatible)
* 1x L298N Motor Driver Module
* 1x 4WD Robot Car Chassis (4x DC motors, 4x wheels)
* 1x HC-SR04 or SRF05 Ultrasonic Sensor
* 1x SG90 Servo Motor
* 1x KY-023 Joystick Module
* 1x LCD 1602 Display with I2C Module
* 1x Red LED (5mm)
* 1x Green LED (5mm)
* 2x 220Ω Resistors (for LEDs)
* 1x Active or Passive Buzzer
* 1x Power Source (e.g., 4x 7V batteries or power bank)
* Jumper Wires & Breadboard

---

**Pinout Summary:**

* **L298N Motor Driver:**
    * IN1, IN2 -> Pins 6, 7
    * IN3, IN4 -> Pins 5, 4
* **Servo (SG90):**
    * Signal -> Pin 11
* **LCD 1602 (I2C):**
    * SDA -> Pin A4
    * SCL -> Pin A5
* **Joystick (KY-023):**
    * X -> Pin A0
    * Y -> Pin A1
    * SW -> Pin 2
* **Ultrasonic Sensor (SRF05):**
    * TRIG -> Pin 9
    * ECHO -> Pin 10
* **LEDs:**
    * Green LED -> Pin 12
    * Red LED -> Pin 13
* **Buzzer:**
    * Positive -> Pin 3

---

## How to Use

1.  **Build:** Assemble the hardware according to the wiring diagram.
2.  **Upload:** Open [Smart_Car.ino](Smart_Car.ino) in the Arduino IDE. Select your board and port, then click "Upload".
3.  **Run:** Power on the robot.
    * It will start in **Manual Mode** (Green LED). Use the joystick to drive.
    * Press the joystick button to switch to **Auto Mode** (Red LED). The car will now drive and avoid obstacles on its own.

---

## Project Gallery & Demo

[The car switching modes](manualtoauto.gif)
[Photo of the car from the side](Smart_Car.png)
