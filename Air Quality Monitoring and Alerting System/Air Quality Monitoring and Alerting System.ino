//Including libraries for DHT-11 sensor and LCD display
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

LiquidCrystal_I2C lcd(0x27, 16, 2); // Setting the LCD address to 0x27 for a 16 characters and 2 line display

//Declaring the variables
int airSensorValue = 0;
int thresholdValue = 300;
float tempValue = 0;
float humidValue = 0;
int columnNumber = 0;
int redLED = 9;
int greenLED = 7;
int buzzerPin = 8;
DHT dht;  

void setup()
{
	lcd.begin(); // Initializing the LCD
  dht.setup(11); //Initializing the DHT-11 sensor on digital pin 11
  pinMode(redLED, OUTPUT); //Initializing digital pin redLed as an output
  pinMode(greenLED, OUTPUT); //Initializing digital pin greenLed as an output
  pinMode(buzzerPin, OUTPUT); //Initializing digital pin buzzerPin as an output
}


void loop()
{
  sensorReading(); 
  airQualityDetection();
  Serial.println(airSensorValue); //Printing the air quality value in the serial monitor
}


void sensorReading()
{
  //This function is to read value from MQ-135 and DHT-11 sensor  
  
  airSensorValue = analogRead(A1); //Reading air quality value from MQ-135 connected in analog pin A1 
  
  delay(dht.getMinimumSamplingPeriod()); //Adding delay to DHT-11 sensor
  tempValue = dht.getTemperature(); //Reading temperature value from DHT-11 sensor
  humidValue = dht.getHumidity(); //Reading humidity value from DHT-11 sensor
  
  delay(100); //Adding delay
}


void airQualityDetection()
{
  //This function is to check whether the air quality is healthy or critical
  
  if (airSensorValue >= thresholdValue)
  {
    //Checking for critical air quality
    lcd.clear(); //Clearing the LCD screen
    displayTempHumid();
    lcd.setCursor(0,1); //Setting cursor on LCD screen to first character of second row
    lcd.print("Poor Air (>_<)");
    alertState();
  }
  else
  {
    //Checking for healthy air quality
    lcd.clear(); //Clearing the LCD screen
    displayTempHumid();
    lcd.setCursor(0,1); //Setting cursor on LCD screen to first character of second row
    lcd.print("Good Air (^_^)");
    normalState();
  }
  delay(100);  
}


void normalState()
{
  //This function is to bring the LED and buzzer to normal state
  
  digitalWrite(greenLED, HIGH); //Turning on green LED 
  digitalWrite(redLED, LOW); //Turning off red LED
  digitalWrite(buzzerPin, LOW); //Turning off buzzer
}


void alertState()
{
  //This function is to bring the LED and buzzer to alerting state
  
  digitalWrite(greenLED, LOW); //Turning off green LED 
  digitalWrite(redLED, HIGH); //Turning on red LED 
  digitalWrite(buzzerPin, HIGH); //Turning on buzzer
}


void displayTempHumid()
{
  //This function is to display temperature and humidity in the LCD screen in scrolling effect
  
  String tempHumidInfo = "| " + String(tempValue) + String((char)223) + "C | " + String(humidValue) + "%H  ";
  int textLength = tempHumidInfo.length(); //Finding the total numbers of characters of displaying text

  if (columnNumber == (textLength - 1))
  {
    //Resetting for scroll over effect
    columnNumber = 0;  
  }

  lcd.setCursor(0,0); //Setting cursor on LCD screen to first character of first row

  if (columnNumber < (textLength - 15))
  {
    //Scrolling effect for current string
    for (int i = columnNumber; i < (columnNumber + 15); i++)
    {
      //Looping to print 16 characters
      lcd.print(tempHumidInfo[i]);
    }
  }

  else
  {
    //Scrolling effect for new strings at the end
    for (int i = columnNumber; i < (textLength - 1); i++)  
    {
      //Looping to print remaining characters of current strings 
      lcd.print(tempHumidInfo[i]);
    }

    for (int i = 0; i <= (15 - (i - columnNumber)); i++)
    {
      //Looping to print initial characters of new strings
       lcd.print(tempHumidInfo[i]);
    }
  }
  
  columnNumber++; //Increasing the column number by 1 
}
