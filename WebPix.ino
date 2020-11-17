#include "M5CoreInk.h"
#include <HTTPClient.h>
#include "ArduinoJson.h"

void connectWifi (){
  char *ssid = "*******";
  char *password = "********";
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(500);
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
  if (!M5.M5Ink.isInit())
  {
    Serial.printf("Ink Init faild");
    while (1)
      delay(100);
  }
  
  connectWifi();
  delay(1000);
  
  Serial.printf("serial\n");

  if (M5.BtnMID.isPressed())
  {
    Serial.printf("Btn %d was pressed \r\n", BUTTON_EXT_PIN);
    digitalWrite(LED_EXT_PIN, LOW);
    M5.M5Ink.clear();
    M5.shutdown();
  } else {
    loadData();
    digitalWrite(LED_EXT_PIN, LOW);
    M5.shutdown(300);
  }

}

void loop()
{
}