#include <main.h>
#include <Map.h>

/*
void setup()
{
  // put your setup code here, to run once:
}

void loop()
{
  // put your main code here, to run repeatedly:
}
*/

// TOF sensor test code

// Objects for the L1x
Adafruit_VL53L1X sens1 = Adafruit_VL53L1X(XSHUT_PIN1);
Adafruit_VL53L1X sens2 = Adafruit_VL53L1X(XSHUT_PIN2);
Adafruit_VL53L1X sens3 = Adafruit_VL53L1X(XSHUT_PIN3);
Adafruit_VL53L1X sens4 = Adafruit_VL53L1X(XSHUT_PIN4);

// objects for the vl53l0x
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox4 = Adafruit_VL53L0X();

// this holds the measurement
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;
VL53L0X_RangingMeasurementData_t measure3;
VL53L0X_RangingMeasurementData_t measure4;

void setIDL0()
{
  Serial.println("Second set of sensors");
  // all reset
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);
  delay(RESET_TIME);
  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  digitalWrite(SHT_LOX3, HIGH);
  digitalWrite(SHT_LOX4, HIGH);
  delay(RESET_TIME);

  // activating LOX1
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);

  // initing LOX1
  if (!lox1.begin(LOX1_ADDRESS))
  {
    Serial.println(F("Failed to boot first VL53L0X"));
    while (1)
      ;
  }
  delay(RESET_TIME);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);
  delay(RESET_TIME);

  // initing LOX2
  if (!lox2.begin(LOX2_ADDRESS))
  {
    Serial.println(F("Failed to boot second VL53L0X"));
    while (1)
      ;
  }

  digitalWrite(SHT_LOX3, HIGH);
  digitalWrite(SHT_LOX4, LOW);
  delay(10);
  if (!lox1.begin(LOX3_ADDRESS))
  {
    Serial.println(F("Failed to boot first VL53L0X"));
    while (1)
      ;
  }
  delay(10);

  // activating LOX2
  digitalWrite(SHT_LOX4, HIGH);
  delay(10);

  // initing LOX2
  if (!lox2.begin(LOX4_ADDRESS))
  {
    Serial.println(F("Failed to boot second VL53L0X"));
    while (1)
      ;
  }
}

void read_L0x_sensors()
{

  lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!
  lox3.rangingTest(&measure3, false); // pass in 'true' to get debug data printout!
  lox4.rangingTest(&measure4, false); // pass in 'true' to get debug data printout!

  // print sensor one reading
  Serial.print(F("1: "));
  if (measure1.RangeStatus != 4)
  { // if not out of range
    Serial.print(measure1.RangeMilliMeter);
  }
  else
  {
    Serial.print(F("Out of range"));
  }

  Serial.print(F(" "));

  // print sensor two reading
  Serial.print(F("2: "));
  if (measure2.RangeStatus != 4)
  {
    Serial.print(measure2.RangeMilliMeter);
  }
  else
  {
    Serial.print(F("Out of range"));
  }
  Serial.print(F(" "));

  Serial.print(F("3: "));
  if (measure1.RangeStatus != 4)
  { // if not out of range
    Serial.print(measure3.RangeMilliMeter);
  }
  else
  {
    Serial.print(F("Out of range"));
  }

  Serial.print(F(" "));

  // print sensor two reading
  Serial.print(F("4: "));
  if (measure2.RangeStatus != 4)
  {
    Serial.print(measure4.RangeMilliMeter);
  }
  else
  {
    Serial.print(F("Out of range"));
  }

  Serial.println();
}

// Initializing the L1x sensors
void setIDL1()
{
  Serial.println("First set of sensors");
  delay(RESET_TIME);
  Serial.println("TEST");
  if (!sens1.begin(L1X1_ADDRESS, &Wire))
  {
    Serial.print(F("Error on init of VL sensor: "));
    Serial.println(sens1.vl_status);
    while (1)
    {
      delay(10);
      Serial.println("FAILURE");
    }
  }
  Serial.println("Sens1 Works");
  if (!sens2.begin(L1X2_ADDRESS, &Wire))
  {
    Serial.print(F("Error on init of VL sensor: "));
    Serial.println(sens2.vl_status);
    while (1)
      delay(10);
  }
  Serial.println("Sens2 Works");
  if (!sens3.begin(L1X3_ADDRESS, &Wire))
  {
    Serial.print(F("Error on init of VL sensor: "));
    Serial.println(sens3.vl_status);
    while (1)
      delay(10);
  }
  if (!sens4.begin(L1X4_ADDRESS, &Wire))
  {
    Serial.print(F("Error on init of VL sensor: "));
    Serial.println(sens4.vl_status);
    while (1)
      delay(10);
  }
  Serial.println(F("VL53L1X sensors OK!"));
  delay(RESET_TIME);
  Serial.print(F("Sensor ID: 0x"));
  delay(RESET_TIME);
  Serial.println(sens1.sensorID(), HEX);
  delay(RESET_TIME);
  Serial.println(sens2.sensorID(), HEX);
  delay(RESET_TIME);
  Serial.println(sens3.sensorID(), HEX);
  delay(RESET_TIME);
  Serial.println(sens4.sensorID(), HEX);
  delay(RESET_TIME);

  
  Serial.println("Didnt get stuck");
  if (!sens1.startRanging())
  {
    Serial.print(F("Couldn't start ranging: "));
    Serial.println(sens1.vl_status);
    while (1)
      delay(10);
  }
  if (!sens2.startRanging())
  {
    Serial.print(F("Couldn't start ranging: "));
    Serial.println(sens2.vl_status);
    while (1)
      delay(10);
  }
  if (!sens3.startRanging())
  {
    Serial.print(F("Couldn't start ranging: "));
    Serial.println(sens3.vl_status);
    while (1)
      delay(10);
  }
  if (!sens4.startRanging())
  {
    Serial.print(F("Couldn't start ranging: "));
    Serial.println(sens4.vl_status);
    while (1)
      delay(10);
  }
  Serial.println(F("Ranging started"));

  // Valid timing budgets: 15, 20, 33, 50, 100, 200 and 500ms!
  sens1.setTimingBudget(50);
  sens2.setTimingBudget(50);
  sens3.setTimingBudget(50);
  sens4.setTimingBudget(50);
  Serial.print(F("Timing budget (ms): "));
  Serial.println(sens1.getTimingBudget());
  Serial.println(sens2.getTimingBudget());
  Serial.println(sens3.getTimingBudget());
  Serial.println(sens4.getTimingBudget());
}

void setup()
{
  Wire.begin();
  Wire.setSCL(19);
  Wire.setSDA(18);
  Serial.begin(9600);
  while (!Serial)
    delay(10);

  Serial.println(F("Adafruit VL53L1X sensor demo"));
  setIDL1();
  setIDL0();
  read_L0x_sensors();

  byte count = 0;

  for (byte i = 1; i < 120; i++)
  {

    Wire.beginTransmission(i);
    if (Wire.endTransmission() == 0)
    {
      Serial.print("Found address: ");
      Serial.print(i, DEC);
      Serial.print(" (0x");
      Serial.print(i, HEX);
      Serial.println(")");
      count++;
      delay(1); // maybe unneeded?
    }           // end of good response
  }             // end of for loop
  Serial.println("Done.");
  Serial.print("Found ");
  Serial.print(count, DEC);
  Serial.println(" device(s).");

  /*
  vl.VL53L1X_SetDistanceThreshold(100, 300, 3, 1);
  vl.VL53L1X_SetInterruptPolarity(0);
  */
}

void loop()
{
  int16_t distance;
  int16_t distance1;

  if (sens1.dataReady())
  {
    // new measurement for the taking!
    distance = sens1.distance();
    if (distance == -1)
    {
      // something went wrong!
      Serial.print(F("Couldn't get distance: "));
      Serial.println(sens1.vl_status);
      return;
    }
    Serial.print(F("Distance: "));
    Serial.print(distance);
    Serial.println(" mm");

    // data is read out, time for another reading!
    sens1.clearInterrupt();
  }
  if (sens2.dataReady())
  {
    // new measurement for the taking!
    distance1 = sens2.distance();
    if (distance1 == -1)
    {
      // something went wrong!
      Serial.print(F("Couldn't get distance: "));
      Serial.println(sens2.vl_status);
      return;
    }
    Serial.print(F("Distance: "));
    Serial.print(distance1);
    Serial.println(" mm");

    // data is read out, time for another reading!
    sens2.clearInterrupt();
  }
}
