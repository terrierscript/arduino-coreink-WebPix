#include "M5CoreInk.h"
// #include "ModWifi.h"
// #include "ModVoltage.h"
#include <HTTPClient.h>
// #include <ArduinoHttpClient.h>
#include "ArduinoJson.h"
// #include <ArduinoBearSSL.h>

// #include <WiFiClientSecure.h>
Ink_Sprite InkPageSprite(&M5.M5Ink);
ModWifi Mod_Wifi;
ModVoltage Mod_Voltage;


void connectWifi (){
  char *ssid = "*******";
  char *password = "********";
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(500);
  }
  return 0;
}

void drawWarning(const char *str)
{
  M5.M5Ink.clear(1);
  InkPageSprite.clear(1);
  InkPageSprite.drawString(10, 10, str);
  InkPageSprite.pushSprite();
}

float getBatVoltage()
{
  analogSetPinAttenuation(35, ADC_11db);
  esp_adc_cal_characteristics_t *adc_chars = (esp_adc_cal_characteristics_t *)calloc(1, sizeof(esp_adc_cal_characteristics_t));
  esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 3600, adc_chars);
  uint16_t ADCValue = analogRead(35);

  uint32_t BatVolmV = esp_adc_cal_raw_to_voltage(ADCValue, adc_chars);
  float BatVol = float(BatVolmV) * 25.1 / 5.1 / 1000;
  return BatVol;
}

void checkBatteryVoltage(bool powerDownFlag)
{
  float batVol = getBatVoltage();
  Serial.printf("Bat Voltage %.2f\r\n", batVol);

  if (batVol > 3.2)
    return;

  drawWarning("Battery voltage is low");
  if (powerDownFlag == true)
  {
    M5.shutdown();
  }
  while (1)
  {
    batVol = getBatVoltage();
    if (batVol > 3.2)
      return;
  }
}


const char *endpoint = "https://coreink-web.terrierscript.vercel.app/api/dog/pix";

String loadPage()
{
  // DynamicJsonDocument doc(500);
  HTTPClient http;
  http.begin(endpoint);
  int httpCode = http.GET();
  Serial.printf("httpcode %d", httpCode);
  String result = "";
  if (httpCode > 0)
  {
    //jsonオブジェクトの作成
    result = http.getString();
  }
  else
  {
    Serial.println("Error on HTTP request");
  }
  http.end(); //リソースを解放
  return result;
}

int SIZE = 5000;
unsigned char pool[5000]; //ここが一定値以上だと壊れる。
void loadPool(unsigned char *pool)
{
  String buf = loadPage();
  for (int i = 0; i < SIZE; i++)
  {
    String pp = buf.substring(i * 2, i * 2 + 2);
    int bbb = (int)strtol(pp.c_str(), 0, 16);
    pool[i] = bbb;
    // Serial.printf("%d|", bbb);
  }
}

void loadData()
{
  Serial.printf("loadData\n");
  for (int i = 0; i < SIZE; i++)
  {
    pool[i] = 0xff;
  }

  loadPool(pool);
  delay(3000);
  M5.M5Ink.drawBuff((uint8_t *)pool);
}

void setup()
{
  M5.begin();

  // M5.M5Ink.clear(1);

  if (!M5.M5Ink.isInit())
  {
    Serial.printf("Ink Init faild");
    while (1)
      delay(100);
  }
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  connectWifi();
  delay(1000);
  if (InkPageSprite.creatSprite(0, 0, 200, 200, true) != 0)
  {
    Serial.printf("Ink Sprite creat faild");
  }
}

void loop()
{
  Serial.printf("serial\n");

  if (M5.BtnMID.isPressed())
  {
    Serial.printf("Btn %d was pressed \r\n", BUTTON_EXT_PIN);
    digitalWrite(LED_EXT_PIN, LOW);
    M5.M5Ink.clear();
    M5.shutdown();
  }
  
    loadData();

  digitalWrite(LED_EXT_PIN, LOW);
  M5.shutdown(300);

  delay(1000 * 20);
  M5.update();
}