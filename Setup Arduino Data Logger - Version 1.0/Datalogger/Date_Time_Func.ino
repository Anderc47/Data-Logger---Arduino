//----------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------
// Function that returns a string of the date and time

String Date_time(DateTime TimeStamp){
  String DATE_TIME = "";
  String Month = String(TimeStamp.Month);
  String Day = String(TimeStamp.Day);
  String Year = "20" + String(TimeStamp.Year);
  String Hour = String(TimeStamp.Hour);
  String Min = String(TimeStamp.Minute);
  String Sec = String(TimeStamp.Second);
  if(Month.toInt() < 10){
    Month = "0" + Month;
  }
  if(Day.toInt() < 10){
    Day = "0" + Day;
  }
  if(Hour.toInt() < 10){
    Hour = "0" + Hour;
  }
  if(Min.toInt() < 10){
    Min = "0" + Min;
  }
    if(Sec.toInt() < 10){
    Sec = "0" + Sec;
  }
    
  DATE_TIME += Year + "-" + Month + "-" + Day + "\t" + Hour + ":" + Min + ":" + Sec + "\t";
  return DATE_TIME;
}
