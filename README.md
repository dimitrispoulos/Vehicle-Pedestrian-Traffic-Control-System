# Vehicle-Pedestrian-Traffic-Control-System

An advanced, Arduino-based smart intersection management system. This project controls both vehicle traffic lights and pedestrian signals using real-time vehicle detection via ultrasonic telemetry to maximize urban safety

-----
-----

## Key Features
- **Dual-Flow Management:** Synchronized control for vehicles (Red/Yellow/Green) and pedestrians (Red/Green).
- **Adaptive Safety Logic:** The system monitors oncoming traffic. If a pedestrian requests a crossing while a vehicle is detected within a danger zone (<150cm), the transition is delayed until the vehicle passes.
- **Interactive UI:** A 16x2 I2C LCD provides live road status and visual countdowns for both phases.
- **Audio Guidance:** Pulsing buzzer signals provide accessibility for pedestrians during the crossing phase. This can be very helpful to pedestrians with visibility issues.
- **Responsive Polling:** High-frequency button monitoring ensures no pedestrian request is missed during the vehicle-green cycle.

-----

## Mathematical Formula
The system calculates the distance based on the travel time of the ultrasonic pulse:
* Speed of sound is $340\ \text{m/s}$ or $0.034\ \text{cm/}\mu\text{s}$.
* The sound travels to the object and back, so the total distance is divided by $2$.
* $Distance = (duration \times 0.034) / 2$
* This is equivalent to: **$Distance = duration / 58.2$** (In the implementation, we use 58 for integer math stability).

-----

## Hardware
- **Microcontroller:** Arduino Uno
- **Sensor:** Ultrasonic Ranger Module (HC-SR04)
- **Display:** 16x2 LCD (LM016L) with I2C Interface (PCF8574)
- **Indicators:** 5x LEDs (Vehicle: Red, Yellow, Green | Pedestrian: Red, Green)
- **Audio:** Piezo Sounder
- **Input:** Push Button (Active Low with internal pull-up)

-----

## Pin Configuration
| Component | Arduino Pin | Function |
| :--- | :---: | :--- |
| **Vehicle Red LED** | **10** | Stop Signal for Vehicles |
| **Vehicle Yellow LED** | **9** | Transition Signal |
| **Vehicle Green LED** | **8** | Go Signal for Vehicles |
| **Pedestrian Red LED** | **4** | Stop Signal for Pedestrians |
| **Pedestrian Green LED** | **5** | Cross Signal for Pedestrians |
| **Piezo Sounder** | **7** | Audio Signal |
| **Push Button** | **2** | Pedestrian Request Button |
| **Ultrasonic Trig** | **11** | Trigger Pulse Output |
| **Ultrasonic Echo** | **13** | Echo Pulse Input |
| **LCD SDA** | **A4** | I2C Serial Data |
| **LCD SCL** | **A5** | I2C Serial Clock |

-----

## Software
### Development Environment
- **Arduino IDE**: For coding and uploading to the microcontroller.
- **Proteus 8 Professional**: For virtual circuit simulation.
### Required Libraries
- **LiquidCrystal_I2C.h**: External library for controlling the LCD display (by Frank de Brabander).
- **Wire.h**: Built-in library used for I2C communication.

-----
-----

## Author
### Dimitrios Poulos
- **Electrical & Computer Engineering Student**
