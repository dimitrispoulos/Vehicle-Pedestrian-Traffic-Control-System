/*
    Project: Smart Pedestrian Crossing Controller
    Description: An interactive traffic light system that uses an ultrasonic sensor to detect oncoming traffic and adjust pedestrian crossing requests accordingly
    Author: Dimitrios Poulos
*/

#include <LiquidCrystal_I2C.h>
#include <Wire.h>


// Pin Configuration
// Traffic Light Pins for Vehicles
int vehicleRed = 10;    
int vehicleYellow = 9;   
int vehicleGreen = 8;    

// Traffic Light Pins for Pedestrians
int pedRed = 4;
int pedGreen = 5;

// Interface and Sensor Pins
int button = 2;      // Pedestrian request button
int buzzer = 7;      // Audio feedback device
int trigPin = 11;    // Ultrasonic sensor Trigger pin
int echoPin = 13;    // Ultrasonic sensor Echo pin

LiquidCrystal_I2C LCD(0x27, 16, 2);    // 0x27 for a 16 chars and 2 line display

long baselineDistance = 300;    // Road width (in cm)

// Function declarations
void stopMessage();
int getDistance();




void setup()
{
  // Set Traffic Light pins as OUTPUT
  pinMode(vehicleRed, OUTPUT); 
  pinMode(vehicleYellow, OUTPUT); 
  pinMode(vehicleGreen, OUTPUT);
  pinMode(pedRed, OUTPUT); 
  pinMode(pedGreen, OUTPUT);
  
  // Set Interface pins
  pinMode(button, INPUT_PULLUP); // Use internal resistor to avoid floating state
  pinMode(buzzer, OUTPUT);
  
  // Set Sensor pins
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);

  // Initialize LCD Screen
  LCD.init();
  LCD.backlight();
  
  // Initial Traffic State: Cars flowing (Green), Pedestrians waiting (Red)
  digitalWrite(vehicleGreen, HIGH);
  digitalWrite(pedRed, HIGH);
  stopMessage();
}

void loop()
{
  // PHASE 1: Green for vehicles (Red for pedestrians)
  // Default green light duration for cars is 10 seconds
  for(int i=10; i>0; i--)
  {
    int distance = getDistance();    // Continuously monitor vehicle distance

    LCD.setCursor(0, 0);
    if((distance<(baselineDistance-50)) && (distance>5))
    {
      LCD.print("! CAR DETECTED !");
    }
    else
    {
      LCD.print("  ROAD IS CLEAR ");
    }

    LCD.setCursor(0, 1);
    LCD.print("CAR GREEN: ");
    LCD.print(i);    //countdown for vehicle green light
    LCD.print("   ");    // Space used for buffer issues

    // BUTTON: Check for pedestrian request (The program checks 10 times per second)
    for(int j=0; j<10; j++)
    {
      if(digitalRead(button) == LOW)
      {
        LCD.clear();
        LCD.setCursor(0, 0);
        LCD.print("REQUEST RECEIVED");
        
        if(distance < 150)    // If a car is closer than 150cm, it's safer to let it pass before changing the lights.
        {
          LCD.setCursor(0, 1);
          LCD.print("WAIT FOR VEHICLE");    // The pedestrian lets the close car pass first for safety reasons
          delay(3000); 
        }
        else    //If not, transition starts faster.
        {
          LCD.setCursor(0, 1);
          LCD.print("PLEASE WAIT...  ");
          delay(1000);
        }
        i = 0; // Trigger cycle change
        break; 
      }
      delay(100); 
    }

    if(i == 0)
    {
      break; // Exit monitoring loop
    }
  }

  // PHASE 2: Transition sequence
  tone(buzzer, 800, 100);      // Beep once to alert pedestrians
  digitalWrite(vehicleGreen, LOW);
  digitalWrite(vehicleYellow, HIGH);
  delay(2000);
  
  digitalWrite(vehicleYellow, LOW);
  digitalWrite(vehicleRed, HIGH);    // Vehicles stop
  delay(1000);
  
  // PHASE 3: Pedestrians crossing
  digitalWrite(pedRed, LOW);
  digitalWrite(pedGreen, HIGH);    // Green for pedestrians
  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.print("   CROSS NOW!   ");
  
  // Pedestrian countdown with audio alert
  for(int i=10; i>0; i--)
  { 
    LCD.setCursor(0, 1);
    LCD.print("  TIME LEFT: ");
    LCD.print(i);
    LCD.print("   ");
    
    tone(buzzer, 450, 200);
    delay(1000);
  }

  // PHASE 4: After this phase, the sequence returns to phase 1 again
  digitalWrite(pedGreen, LOW);    // Time is up for pedestrians
  digitalWrite(pedRed, HIGH);

  LCD.clear();
  stopMessage();
  delay(2000);

  digitalWrite(vehicleRed, LOW);
  digitalWrite(vehicleGreen, HIGH);    // Vehicles can move again
}



// Displays safety message for pedestrians on the LCD
void stopMessage()
{
  LCD.setCursor(0, 0);
  LCD.print("    STOP!!!!    ");
  LCD.setCursor(0, 1);
  LCD.print("  DO NOT CROSS  ");
}



/* FORMULA EXPLANATION:
    Speed of sound is 340 m/s or 0.034 cm/microsecond.
    The sound travels to the object and back, so it is divided by 2.
    Distance = (duration*0.034)/2
    This is equivalent to: Distance = duration/58.2
*/
int getDistance()
{
  digitalWrite(trigPin, LOW);    // Resets trigger pin
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);    // Send 10us pulse
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);    // Travel time of the wave. pulseIn measures the time (in microseconds) for the echo pulse to return.

  int distance = duration/58;    // Convert time to cm
  
  return distance; 
}