/*
 * This funciton is used for Campbell Scienfic Air Temperature Sensors
 * Noet that it is only this sensor as the others do not have manuals for setting up the functions
 * Please see manual for the wiring details
 */

float CampbellSciTemp(float AnalogSensor, float InputVoltage){
  double A = 1.129241 / 1000;
  double B = 2.341077 / 10000;
  double C = 8.775468 / 100000000;
  float ConV = 4.9;
  float Vx = InputVoltage * ConV;
  float Vs = AnalogSensor * ConV;

  double Rs = 24900 * ((Vx/Vs) - 1);

  float Temp = (1 / (A + B*log(Rs) + C*pow(log(Rs),3))) - 273.15;

  return Temp;
}
