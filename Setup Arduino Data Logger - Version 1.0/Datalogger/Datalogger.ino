/*
 * This is the program the Arduino datalogger will run off of
 * 
 * Please refer to the manual for in depth knowledge on changes and how the functions work 
 * 
 * Adding a sensor's address to take measurements from:
 * #define SENSOR_NAME SENSOR_ADDRESS
 * 
 * Taking measurements:
 * float MEASUREMNET_NAME; -> define at begining of void loop{} as seen in sketch
 * MEASUREMNET_NAME = Measurement_Output(SENSOR_ADDRESS, MEASUREMENT_NUM, SLOPE, OFFSET);
 * 
 * Notes on Measurement_Output Function:
 * SENSOR_ADDRESS => Address of sesnor that the measurement is taken from
 * MEASUREMENT_NUM => The number of the measurement taken form the sensor (Ie: Pressure Transducer-> 1 = Water level, 2 = Water temp)
 * SLOPE => Slope appplied to the measurement, will be defined in the sensor manual
 * OFFSET => Offset applied to the measurement, will be defined by external factors such as location
 * 
 */
 
//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
// INITIALIZATION

#include <SDI12.h>              // Importing the SDI12 library
#define DATA_PIN 2              // SDI-12 bus data pin
#define SOIL_SENSOR_ADDRESS 1   // Defines PT12 sensor address
SDI12 busSDI12(DATA_PIN);       // Defining the SDI-12 bus 

#define SERIAL_BAUD 9600        // Serial COM rate

#include <DS3231_Simple.h>      // Importing the real time clock library
DS3231_Simple Clock;            // Initializing thew RTC

#include <SPI.h>                // Importing the SPI library (aids in SD card communication)
#include <SD.h>                 // Importing the SD card library   

const int CHIP_SELECT = 10;              // The pin that the SD card pin CS is connected to
String ORIGINALDATA = "Setup_O.txt";      // Defining the  where ALL the logged data will be saved to (only change <name>)
String DAILYDATA = "Setup_D.txt";         // Daily data file that is reset whenever the daily data function is called (only change <name>)  
int incomingByte;                        // Serial input variable (User input)
static boolean ALARM = false;            // Alarm variable -> true if alarm set off, false if alarm has not beenm set off

//Note for the filename <name> has to be 7 characters or less 


//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
// SETUP LOOP (ran once)
void setup() {
// Serial setup
  Serial.begin(SERIAL_BAUD);            // Begin serial communication
  Serial.print("Starting Program...");  // Sending text to let user know the program has started

// SDI-12 setup
  busSDI12.begin();       // Begining the SDI-12 databus to be able to take data 
  delay(500);             // Sening 0.5 second delay to allow bus time to boot-up 

// SD card setup
  SD.begin(CHIP_SELECT);  // Begining the SD card communication on the pin CHIP_SELECT is defined as

// THE CODE BELLOW WILL RESET THE ENTIRE FILE ONLY UNCOMMENT WHEN NEEDED
//  if(SD.exists(DAILYDATA)){
//    SD.remove(DAILYDATA);     // Reseting the daily data
//  }
//  if(SD.exists(ORIGINALDATA)){
//    SD.remove(ORIGINALDATA);     // Reseting the daily data
//  }
//

  file_write(DAILYDATA, "Date\tTime\tVWC\tSTMP");      // Writing the headings to the file define DAILYDATA: (ADD PARAMETERS THAT WILL BE LOGGED)
  file_write(ORIGINALDATA, "Date\tTime\tVWC\tSTMP");   // Writing the headings to the file define ORIGINALDATA: (ADD PARAMETERS THAT WILL BE LOGGED)


// Clock Setup
  Clock.begin();    
  Clock.disableAlarms();      // Disable any existing alarms
  
  //alarm every hour -> needs a given minute time stamp
  DateTime MyTimestamp;       // Defining the timestamp 
  MyTimestamp.Minute = 0;     // Defining the minute to set the hourly alarm on (will take measesurment on this minute of every hour)
  Clock.setAlarm(MyTimestamp, DS3231_Simple::ALARM_HOURLY); // Setting the hourly alarm on the defined minute

//  // Alarm every min
//  Clock.setAlarm(DS3231_Simple::ALARM_EVERY_MINUTE);
  
}


//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
// MAIN LOOP (continuosly runnning)
void loop() {
// This string will be sent to the SD card or the Serial
  String DataString;   // String that will be compiled of the date, time, and measured parameters. 
  
// ADD THE VARIABLES BEING MEASURED AS FLOATS HERE:
  float VWC; // Taken off of Soil sensor
  float STMP; // Taken off of Soil sensor

  uint8_t AlarmsFired = Clock.checkAlarms();    // Checking to see if the alarm has been set off

// If the alarm has been set off complete the program bellow (where the datalogging is taking place)
  if(AlarmsFired & 2){
  DateTime MyTimestamp = Clock.read();   // Taking a time measurement 
  DataString = Date_time(MyTimestamp);   // Adding the date and time to the string that will be logged to the SD card
  
// Add measurements here:
  VWC = Measurement_Output(SOIL_SENSOR_ADDRESS, 1, 1.0, 0.0);   // 1st measurement on SOIL_SENSOR_ADDRESS
  STMP = Measurement_Output(SOIL_SENSOR_ADDRESS, 2, 1.0, 0.0);     // 2nd measurement on SOIL_SENSOR_ADDRESS
    
// Adding the parameters to thestring that will be logged to the SD card ( will need to add/subtract depending on parameters being logged )
  DataString += String(STMP) + "\t" + String(VWC) ;
  
  file_write(DAILYDATA,DataString);      // Writing the string with all the parameters to the SD card
  file_write(ORIGINALDATA,DataString);      // Writing the string with all the parameters to the SD card

  } 
// End of alarm program


// If there is a serial input take the input into a integer ( will only take numbers -> floats become integers )   
  if (Serial.available() > 0) {
    incomingByte = Serial.parseInt();

// WILL TAKE A LIVE READING AND PRINT TO SERIAL MONITOR IF INPUT IS 1100
    if(incomingByte == 111){
      
      DateTime MyTimestamp = Clock.read();  // Taking a clock reading
      DataString = Date_time(MyTimestamp);  // Adding the date and time to the string that will be printed to the Serial monitor
      
// Add measurements here (should be same as logging to sd card):       
      VWC = Measurement_Output(SOIL_SENSOR_ADDRESS, 1, 1.0, 0.0);   // 1st measurement on SOIL_SENSOR_ADDRESS
      STMP = Measurement_Output(SOIL_SENSOR_ADDRESS, 2, 1.0, 0.0);     // 2nd measurement on SOIL_SENSOR_ADDRESS
      
// Adding the parameters to thestring that will be printed to serial( should be same as logging to sd card )
      DataString += String(VWC) + "\t" + String(STMP);
      Serial.print("\nDate\t\tTime\t\tVWC\tSTMP");
      Serial.println();
      Serial.print(DataString);
     
      }
      
// WILL PRINT ALL THE DATA FROM THE DEFINED ORIGINALDATA FILE AND PRINT TO SERIAL MONITOR IF INPUT IS 1111 (Mainly used for complete data dump)
      else if (incomingByte == 222){
        data_output(ORIGINALDATA); // Outputing the daat to serial monitor       
      }
    
    
// WILL PRINT THE DAILY DATA FROM THE DEFINED DAILYDATA FILE AND PRINT TO SERIAL MONITOR IF INPUT IS 1110 (Mainly used for getting data into WISKI)
      else if (incomingByte == 9999){
        data_output(DAILYDATA);     // Outputing the daat to serial monitor
        SD.remove(DAILYDATA);     // Reseting the daily data

        file_write(DAILYDATA, "Date\tTime\tHG\tTW");  // Writing the headings to the file define DAILYDATA: (ADD PARAMETERS THAT WILL BE LOGGED)
      }
  }
// End of serial input program

}
