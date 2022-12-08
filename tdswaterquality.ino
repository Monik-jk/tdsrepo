#include <LiquidCrystal_I2C.h>
#define USE_NODE_MCU_BOARD

int DSPIN = D4; // Dallas Temperature Sensor
int TDS_Sensor = A0;
float Aref = 3.3;

float ec_Val = 0;
unsigned int tds_value = 0;
float ecCal = 1;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void sendSensor()
{
  double wTemp = TempRead()* 0.0625;  // conversion accuracy is 0.0625 / LSB
  float V_level= Aref / 1024.0;
  float rawEc = analogRead(TDS_Sensor) * V_level;  // Raw  data of EC
  float T_Cof = 1.0 + 0.02 * (wTemp - 25.0);  // Temperature Coefficient
  ec_Val = (rawEc / T_Cof) * ecCal;// temperature and calibration compensation
  tds_value = (133.42 * pow(ec_Val, 3) - 255.86 * ec_Val * ec_Val + 857.39 * ec_Val) * 0.5; 
  double Far= (((wTemp *9)/5) + 32); // Temp in Far*
  
  Serial.print("TDS:"); Serial.println(tds_value);
  Serial.print("EC:"); Serial.println(ec_Val, 2);
  Serial.print("Temperature (oC):"); Serial.println(wTemp,2);   
  Serial.print("Temperature (oF):"); Serial.println(Far,2);
  Serial.print("");
  lcd.setCursor(0, 0);
      lcd.print("tds:");
      lcd.print(tds_value);
      lcd.print("ppm");
      lcd.setCursor(10,0);
      lcd.print("EC:"); 
      lcd.println(ec_Val, 2);
      lcd.setCursor(0,1);
      lcd.print("tempC:");
      lcd.println(wTemp);
      lcd.setCursor(9,1);
      lcd.print("F:");
      lcd.println(Far);
  
   

}
void setup() 
{
  Serial.begin(115200); // Dubugging on hardware Serial 0
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Water Quality");
  lcd.setCursor(4, 1);
  lcd.print("Monitor");
  delay(4+000);
  lcd.clear();
  delay(2000); 
}
 
void loop() 
{
  sendSensor();
}
