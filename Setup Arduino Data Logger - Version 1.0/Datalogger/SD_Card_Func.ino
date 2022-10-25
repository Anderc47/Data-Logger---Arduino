//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
// Function that opens and writes a line to the SD card

void file_write(String filename, String text){
  
  File dataFile = SD.open(filename, FILE_WRITE);
  
  if(dataFile){
    dataFile.println(text);
  }
  dataFile.close(); 
  
}

//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
// Function that opens and reads data from the given filename and then outputs it to the serial

void data_output(String filename){
  File dataFile = SD.open(filename);
  while(dataFile.available()){
    Serial.write(dataFile.read());
  }
  dataFile.close();
  
  if(!dataFile.available()){
//    Serial.println("\t\terror opening file");
  }
  
}
