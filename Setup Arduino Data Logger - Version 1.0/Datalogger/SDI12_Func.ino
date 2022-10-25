
//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
// Takes raw measurement outputs as a string to be broken up

String TakeMeasurement(int Sensor_Address){
  
  //Setup the command measuremt cmd
  String Command = "";
  Command += Sensor_Address;
  Command += "M!";
  busSDI12.sendCommand(Command);
  delay(2000);
  busSDI12.clearBuffer();


  //set up data request cmd
  Command = "";
  Command += Sensor_Address;
  Command += "D0!";

  String Measurement = "";    //Assigning an output

  //Starting bus and sending command

  busSDI12.sendCommand(Command);
  delay(30);

  //Compliling reading and appending to the empy output string
  while(busSDI12.available()){
    char c = busSDI12.read();
    if ((c != '\n') && (c != '\r')) {
      Measurement += c;
      delay(10);  // 1 character ~ 7.5ms
    }
  }
  busSDI12.clearBuffer();

  return Measurement;   //Returning raw measurement 
}

//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
// Function that can split up a string (split raw data into usable data)

String getValue(String data, char separator, int index){
  
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
// Function allows the sensor to take a measurement and convert it into a float

float Measurement_Output(int Sensor_Address, int num_reading, float Slope, float Offset ){
  
  String raw_data = getValue(TakeMeasurement(Sensor_Address), '+', num_reading);
  
  float reading_actual = (raw_data.toFloat() * Slope) + Offset;
  
  return reading_actual;
  
}
