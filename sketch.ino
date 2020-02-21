#define MY_DEBUG
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#define TFT_CS D0
#define TFT_DC D8
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
#include <XPT2046_Touchscreen.h>
#define CS_PIN  D3
XPT2046_Touchscreen ts(CS_PIN); 

#define TS_MINX 3860  // X pion    góra  min wartość
#define TS_MINY 3860  // Y poziom  lewo  min wartość
#define TS_MAXX 313   // X pion    dół   max wartość
#define TS_MAXY 210   // Y poziom  prawo max wartość
#define MINPRESSURE 400
#define MAXPRESSURE 5000

// DEFINICJE ramek przycisków 
int bulb_sr = 18, bulb_start_color = ILI9341_DARKGREY, box_color = ILI9341_DARKGREY;
int boxsize = 120, p;

// DEFINICJE temaów przyciskow 
const char* bulb1_topic_command = "your topic....";
const char* bulb1_topic_state = "your topic....";
const char* bulb1_opis = "TERMA "; //max 18 znaków
int bulb1_value = 0;
int button1_x = 0,   button1_y = 0;
String bulb1_temp;

const char* bulb2_topic_command = "your topic....";
const char* bulb2_topic_state = "your topic....";
const char* bulb2_opis = "      KUCHNIA    ";
int bulb2_value = 0;
int button2_x = 120, button2_y = 0;

int button3_x = 0,   button3_y = 80;
const char* bulb3_topic_command = "your topic....";
const char* bulb3_topic_state =   "your topic....";
const char* bulb3_opis = "    LAZIENKA 1   ";
int bulb3_value = 0;

int button4_x = 120, button4_y = 80;
const char* bulb4_topic_command = "your topic....";
const char* bulb4_topic_state =   "your topic....";
const char* bulb4_opis = "    LAZIENKA 2   ";
int bulb4_value = 0;

int button5_x =   0, button5_y = 160;
const char* bulb5_topic_command = "your topic....";
const char* bulb5_topic_state = "your topic....";
const char* bulb5_opis = "   SPIZARNIA 2    ";
int bulb5_value = 0;

int button6_x = 120, button6_y = 160;
const char* bulb6_topic_command = "your topic....";
const char* bulb6_topic_state = "your topic....";
const char* bulb6_opis = "    PRZYCISK 6    ";
int bulb6_value = 0;
int button7_x =   0, button7_y = 240;
const char* bulb7_topic_command = "your topic....";
const char* bulb7_topic_state = "your topic....";
const char* bulb7_opis = "    PRZYCISK 7    ";
int bulb7_value = 0;

int button8_x = 120, button8_y = 240;
const char* bulb8_topic_command = "your topic....";
const char* bulb8_topic_state = "your topic....";
const char* bulb8_opis = "COS TAM COS TAM";
int bulb8_value = 0;

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const char* ssid = "sieć wifi";
const char* password = "password";
const char* mqtt_server = "adres ip brokera";
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "LCD_touch";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
   if (client.connect(clientId.c_str(),"username brokera", "hasło brokera")) {
      Serial.println("connected");
      // lista tematów do subskrybcji
          client.subscribe("your topic....";");
          .
          . 
          .
          client.subscribe("your topic....";
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
} //end reconnect() ========================================


void setup(void) { 
  Serial.begin(9600);
  // start wifi ----------
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  // end wifi -----------
  tft.begin();
  tft.setRotation(0);
  if (!ts.begin()) {     
    Serial.println("Couldn't start touchscreen controller");
    while (1);
  }
  tft.fillScreen(ILI9341_BLACK);
  rysuj_boxy();
}

void loop() { 
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    }
  if (ts.bufferEmpty()) {
    return;
  }
if (ts.touched()) {
    TS_Point p = ts.getPoint();
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.height()); // !!!!!!pion, dotyk ma odwrotnie współrzędne
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.width());  //!!!!!poziom

    // START button1
    if (p.x > button1_y && p.x < button1_y+80) {
    if (p.y > button1_x && p.y < button1_x+120) { 
      if (bulb1_value ==1){
      client.publish(bulb1_topic_command, "OFF");
      } else {
      client.publish(bulb1_topic_command, "ON");}
      tft.drawRoundRect( button1_x+1, button1_y+1, 118,  78, 5, ILI9341_GREEN); 
      delay(300);
      tft.drawRoundRect( button1_x+1, button1_y+1, 118,  78, 5, ILI9341_DARKGREY);
    }} // END button1
     // START button2
    if (p.x > button2_y && p.x < button2_y+80) {
    if (p.y > button2_x && p.y < button2_x+120) { 
      if (bulb2_value ==1){
      client.publish(bulb2_topic_command, "OFF");
      } else {
      client.publish(bulb2_topic_command, "ON");}
      tft.drawRoundRect( button2_x+1, button2_y+1, 118,  78, 5, ILI9341_GREEN); 
      delay(300);
      tft.drawRoundRect( button2_x+1, button2_y+1, 118,  78, 5, ILI9341_DARKGREY);
    }} // END button2
    // START button3
    if (p.x > button3_y && p.x < button3_y+80) {
    if (p.y > button3_x && p.y < button3_x+120) { 
      if (bulb3_value ==1){
      client.publish(bulb3_topic_command, "OFF");
      } else {
      client.publish(bulb3_topic_command, "ON");}
      tft.drawRoundRect( button3_x+1, button3_y+1, 118,  78, 5, ILI9341_GREEN); 
      delay(300);
      tft.drawRoundRect( button3_x+1, button3_y+1, 118,  78, 5, ILI9341_DARKGREY);
    }} // END button3
    // START button4
    if (p.x > button4_y && p.x < button4_y+80) {
    if (p.y > button4_x && p.y < button4_x+120) { 
      if (bulb4_value ==1){
      client.publish(bulb4_topic_command, "OFF");
      } else {
      client.publish(bulb4_topic_command, "ON");}
      tft.drawRoundRect( button4_x+1, button4_y+1, 118,  78, 5, ILI9341_GREEN);
      delay(300);
      tft.drawRoundRect( button4_x+1, button4_y+1, 118,  78, 5, ILI9341_DARKGREY);
    }} // END button4
    // START button5
    if (p.x > button5_y && p.x < button5_y+80) {
    if (p.y > button5_x && p.y < button5_x+120) { 
      if (bulb5_value ==1){
      client.publish(bulb5_topic_command, "OFF");
      } else {
      client.publish(bulb5_topic_command, "ON");}
      tft.drawRoundRect( button5_x+2, button5_y+2, 116,  84, 5, ILI9341_GREEN);
      delay(300);
      tft.drawRoundRect( button5_x+2, button5_y+2, 116,  84, 5, ILI9341_BLACK);
    }} // END button5
    // START button6
    if (p.x > button6_y && p.x < button6_y+80) {
    if (p.y > button6_x && p.y < button6_x+120) { 
      if (bulb6_value ==1){
      client.publish(bulb6_topic_command, "OFF");
      } else {
      client.publish(bulb6_topic_command, "ON");}
      tft.drawRoundRect( button6_x+2, button6_y+2, 116,  84, 5, ILI9341_GREEN);
      delay(300);
      tft.drawRoundRect( button6_x+2, button6_y+2, 116,  84, 5, ILI9341_BLACK);
    }} // END button6
    // START button7
    if (p.x > button7_y && p.x < button7_y+80) {
    if (p.y > button7_x && p.y < button7_x+120) { 
      if (bulb7_value ==1){
      client.publish(bulb7_topic_command, "OFF");
      } else {
      client.publish(bulb7_topic_command, "ON");}
      tft.drawRoundRect( button7_x+2, button7_y-4, 116,  80, 5, ILI9341_GREEN); 
      delay(300);
      tft.drawRoundRect( button7_x+2, button7_y-4, 116,  80, 5, ILI9341_BLACK);
    }} // END button7
    // START button8
    if (p.x > button8_y && p.x < button8_y+80) {
    if (p.y > button8_x && p.y < button8_x+120) { 
      if (bulb8_value ==1){
      client.publish(bulb8_topic_command, "OFF");
      } else {
      client.publish(bulb8_topic_command, "ON");}
      tft.drawRoundRect( button8_x+2, button8_y-4, 116,  80, 5, ILI9341_GREEN);
      delay(300);
      tft.drawRoundRect( button8_x+2, button8_y-4, 116,  80, 5, ILI9341_BLACK);
    }} // END button8
      

  } 
}  


void callback(char* topic, byte* payload, unsigned int length) {

  if (strcmp(topic,bulb1_topic_state)==0){ 
    payload[length] = '\0';
    if(strcmp((char *)payload, "ON") == 0){
    bulb1_value = 1;
    } else {
    bulb1_value = 0; }
    Serial.println(bulb1_value);
    bulb1();
  }
  if (strcmp(topic," topic temp. wody";")==0){ 
    payload[length] = '\0';
    bulb1_temp = String((char *)payload) + "C";
    Serial.print("temperatura:");
    Serial.println(bulb1_temp);
    tft.setTextSize(1);
    tft.fillRect(button1_x+65, button1_y+59, 30, 8, ILI9341_BLACK);
    tft.setCursor(button1_x+6, button1_y+59);
    tft.setTextColor(ILI9341_GREEN);
    tft.print(bulb1_opis);
    tft.setTextColor(ILI9341_YELLOW);
    tft.println(bulb1_temp);
  } 
    if (strcmp(topic,bulb3_topic_state)==0){
    payload[length] = '\0';
    if(strcmp((char *)payload, "ON") == 0){
    bulb3_value = 1;
    } else {
    bulb3_value = 0; }
    Serial.println(bulb3_value);
    bulb3();
  } 
  if (strcmp(topic,bulb4_topic_state)==0){ 
    payload[length] = '\0';
    if(strcmp((char *)payload, "ON") == 0){
    bulb4_value = 1;
    } else {
    bulb4_value = 0; }
    Serial.println(bulb4_value);
    bulb4();
  } 
   //// dalej lista definicji nałuchiwania wartosci na topicach
}  


void bulb1() { 
    // wyświetla kolor żarówki w zależności od warości na topicu STATE
    if (bulb1_value == 0){
    tft.fillCircle( button1_x+60, button1_y+25, 18, ILI9341_DARKGREY);
    tft.fillRect(button1_x+50, button1_y+42, 20, 8, ILI9341_DARKGREY);
    } else {
    tft.fillCircle( button1_x+60, button1_y+25, 18, ILI9341_YELLOW);
    tft.fillRect(button1_x+50, button1_y+42, 20, 8, ILI9341_YELLOW);}
} // END bulb1()

void bulb2() { 
    // wyświetla kolor żarówki w zależności od warości na topicu STATE
    if (bulb2_value == 0){
    tft.fillCircle( button2_x+60, button2_y+25, 18, ILI9341_DARKGREY);
    tft.fillRect(button2_x+50, button2_y+42, 20, 8, ILI9341_DARKGREY);
    } else {
    tft.fillCircle( button2_x+60, button2_y+25, 18, ILI9341_YELLOW);
    tft.fillRect(button2_x+50, button2_y+42, 20, 8, ILI9341_YELLOW);}
} // END bulb1()

void bulb3() { 
    // wyświetla kolor żarówki w zależności od warości na topicu STATE
    if (bulb3_value == 0){
    tft.fillCircle( button3_x+60, button3_y+25, 18, ILI9341_DARKGREY);
    tft.fillRect(button3_x+50, button3_y+42, 20, 8, ILI9341_DARKGREY);
    } else {
    tft.fillCircle( button3_x+60, button3_y+25, 18, ILI9341_YELLOW);
    tft.fillRect(button3_x+50, button3_y+42, 20, 8, ILI9341_YELLOW);}
} // END bulb1()

void bulb4() { 
    // wyświetla kolor żarówki w zależności od warości na topicu STATE
    if (bulb4_value == 0){
    tft.fillCircle( button4_x+60, button4_y+25, 18, ILI9341_DARKGREY);
    tft.fillRect(button4_x+50, button4_y+42, 20, 8, ILI9341_DARKGREY);
    } else {
    tft.fillCircle( button4_x+60, button4_y+25, 18, ILI9341_YELLOW);
    tft.fillRect(button4_x+50, button4_y+42, 20, 8, ILI9341_YELLOW);}
} // END bulb1()

void bulb5() { 
    // wyświetla kolor żarówki w zależności od warości na topicu STATE
    if (bulb5_value == 0){
    tft.fillCircle( button5_x+60, button5_y+25, 18, ILI9341_DARKGREY);
    tft.fillRect(button5_x+50, button5_y+42, 20, 8, ILI9341_DARKGREY);
    } else {
    tft.fillCircle( button5_x+60, button5_y+25, 18, ILI9341_YELLOW);
    tft.fillRect(button5_x+50, button5_y+42, 20, 8, ILI9341_YELLOW);}
} // END bulb1()

void bulb6() { 
    // wyświetla kolor żarówki w zależności od warości na topicu STATE
    if (bulb6_value == 0){
    tft.fillCircle( button6_x+60, button6_y+25, 18, ILI9341_DARKGREY);
    tft.fillRect(button6_x+50, button6_y+42, 20, 8, ILI9341_DARKGREY);
    } else {
    tft.fillCircle( button6_x+60, button6_y+25, 18, ILI9341_YELLOW);
    tft.fillRect(button6_x+50, button6_y+42, 20, 8, ILI9341_YELLOW);}
} // END bulb1()

void bulb7() { 
    // wyświetla kolor żarówki w zależności od warości na topicu STATE
    if (bulb7_value == 0){
    tft.fillCircle( button7_x+60, button7_y+25, 18, ILI9341_DARKGREY);
    tft.fillRect(button7_x+50, button7_y+42, 20, 8, ILI9341_DARKGREY);
    } else {
    tft.fillCircle( button7_x+60, button7_y+25, 18, ILI9341_YELLOW);
    tft.fillRect(button7_x+50, button7_y+42, 20, 8, ILI9341_YELLOW);}
} // END bulb1()

void bulb8() { 
    // wyświetla kolor żarówki w zależności od warości na topicu STATE
    if (bulb8_value == 0){
    tft.fillCircle( button8_x+60, button8_y+25, 18, ILI9341_DARKGREY);
    tft.fillRect(button8_x+50, button8_y+42, 20, 8, ILI9341_DARKGREY);
    } else {
    tft.fillCircle( button8_x+60, button8_y+25, 18, ILI9341_YELLOW);
    tft.fillRect(button8_x+50, button8_y+42, 20, 8, ILI9341_YELLOW);}
} // END bulb1()


void rysuj_boxy() {
    tft.drawRoundRect( button1_x+1, button1_y+1, 118,  78, 5, box_color); 
    tft.fillCircle( button1_x+60, button1_y+25, 18, bulb_start_color); 
    tft.fillRect(button1_x+50, button1_y+42, 20, 8, bulb_start_color); 
    tft.setTextSize(1);
    tft.setCursor(button1_x+6, button1_y+59);
    tft.setTextColor(ILI9341_GREEN);
    tft.print(bulb1_opis);
    tft.setTextColor(ILI9341_YELLOW);
    tft.println(bulb1_temp);
    
    tft.drawRoundRect( button2_x+1, button2_y+1, 118,  78, 5, box_color); 
    tft.fillCircle( button2_x+60, button2_y+25, 18, bulb_start_color); 
    tft.fillRect(button2_x+50, button2_y+42, 20, 8, bulb_start_color); 
    tft.setTextSize(1);
    tft.setCursor(button2_x+6, button2_y+59);
    tft.setTextColor(ILI9341_WHITE);
    tft.println(bulb2_opis);
    
    tft.drawRoundRect( button3_x+1, button3_y+1, 118,  78, 5, box_color); 
    tft.fillCircle( button3_x+60, button3_y+25, 18, bulb_start_color); 
    tft.fillRect(button3_x+50, button3_y+42, 20, 8, bulb_start_color); 
    tft.setTextSize(1);
    tft.setCursor(button3_x+6, button3_y+59);
    tft.setTextColor(ILI9341_GREEN);
    tft.println(bulb3_opis);
    
    tft.drawRoundRect( button4_x+1, button4_y+1, 118,  78, 5, box_color); 
    tft.fillCircle( button4_x+60, button4_y+25, 18, bulb_start_color); 
    tft.fillRect(button4_x+50, button4_y+42, 20, 8, bulb_start_color); 
    tft.setTextSize(1);
    tft.setCursor(button4_x+6, button4_y+59);
    tft.setTextColor(ILI9341_GREEN);
    tft.println(bulb4_opis);
    
// rysowanie trójkątów zamiast żarówek ;)
    tft.fillTriangle(button5_x+40, button5_y+60,button5_x+80, button5_y+60 ,button5_x+60, button5_y+20 , ILI9341_DARKGREY);
    tft.fillTriangle(button6_x+40, button6_y+60,button6_x+80, button6_y+60 ,button6_x+60, button6_y+20 , ILI9341_DARKGREY);
    tft.fillTriangle(button7_x+40, button7_y+20,button7_x+80, button7_y+20 ,button7_x+60, button7_y+60 , ILI9341_DARKGREY);
    tft.fillTriangle(button8_x+40, button8_y+20,button8_x+80, button8_y+20 ,button8_x+60, button8_y+60 , ILI9341_DARKGREY);
    tft.drawRoundRect( button5_x+1, button5_y+1, 118, 158, 5, box_color);
    tft.drawRoundRect( button6_x+1, button6_y+1, 118, 158, 5, box_color);
    tft.setTextSize(1);
    tft.setCursor(button5_x+6, button5_y+78);
    tft.setTextColor(ILI9341_WHITE);
    tft.println("   ROLETY SALON");
     tft.setTextSize(1);
    tft.setCursor(button6_x+6, button6_y+78);
    tft.println("  ROLETY KUCHNIA");

    
/*  
    tft.drawRoundRect( button5_x+1, button5_y+1, 118,  78, 5, box_color); 
    tft.fillCircle( button5_x+60, button5_y+25, 18, bulb_start_color); 
    tft.fillRect(button5_x+50, button5_y+42, 20, 8, bulb_start_color); 
    tft.setTextSize(1);
    tft.setCursor(button5_x+6, button5_y+59);
    tft.println(bulb5_opis);
    
    tft.drawRoundRect( button6_x+1, button6_y+1, 118,  78, 5, box_color); 
    tft.fillCircle( button6_x+60, button6_y+25, 18, bulb_start_color); 
    tft.fillRect(button6_x+50, button6_y+42, 20, 8, bulb_start_color); 
    tft.setTextSize(1);
    tft.setCursor(button6_x+6, button6_y+59);
    tft.println(bulb6_opis);
    
    
    tft.drawRoundRect( button7_x+1, button7_y+1, 118,  78, 5, box_color); 
    tft.fillCircle( button7_x+60, button7_y+25, 18, bulb_start_color); 
    tft.fillRect(button7_x+50, button7_y+42, 20, 8, bulb_start_color); 
    tft.setTextSize(1);
    tft.setCursor(button7_x+6, button7_y+59);
    tft.println(bulb7_opis);
    
    tft.drawRoundRect( button8_x+1, button8_y+1, 118,  78, 5, box_color); 
    tft.fillCircle( button8_x+60, button8_y+25, 18, bulb_start_color); 
    tft.fillRect(button8_x+50, button8_y+42, 20, 8, bulb_start_color);
    tft.setTextSize(1);
    tft.setCursor(button8_x+6, button8_y+59);
    tft.println(bulb8_opis);
*/
}
