#include <Arduino.h>
#line 1 "/Users/chris/Code/sunlight/espled/espled.ino"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* Put your SSID & Password */
const char* ssid = "Sunlight";  // Enter SSID here
const char* password = "espleded";  //make sure length of password ≥ 8

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

bool LED1status = LOW;
bool LED2status = LOW;

int orange_val = 0;
int blue_val = 0;

#line 21 "/Users/chris/Code/sunlight/espled/espled.ino"
void setup();
#line 53 "/Users/chris/Code/sunlight/espled/espled.ino"
void loop();
#line 62 "/Users/chris/Code/sunlight/espled/espled.ino"
void handle_OnConnect();
#line 69 "/Users/chris/Code/sunlight/espled/espled.ino"
void handle_led1on();
#line 75 "/Users/chris/Code/sunlight/espled/espled.ino"
void handle_led1off();
#line 81 "/Users/chris/Code/sunlight/espled/espled.ino"
void handle_led2on();
#line 87 "/Users/chris/Code/sunlight/espled/espled.ino"
void handle_led2off();
#line 93 "/Users/chris/Code/sunlight/espled/espled.ino"
void handle_NotFound();
#line 97 "/Users/chris/Code/sunlight/espled/espled.ino"
void handle_slider();
#line 108 "/Users/chris/Code/sunlight/espled/espled.ino"
void handle_slider2();
#line 119 "/Users/chris/Code/sunlight/espled/espled.ino"
String SendHTML(uint8_t led1stat,uint8_t led2stat);
#line 21 "/Users/chris/Code/sunlight/espled/espled.ino"
void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT); //built in LED
  digitalWrite(2, LOW);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  digitalWrite(2, HIGH);

  pinMode(4, OUTPUT); //blue
  pinMode(5, OUTPUT); //orange

  analogWrite(4, 255); //255 tursn off the LED
  analogWrite(5, 255);


  
  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led2off);
  server.onNotFound(handle_NotFound);

  server.on("/slider", handle_slider);
  server.on("/slider2", handle_slider2);
  
  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
  
  if(!LED2status)
  {digitalWrite(2, HIGH);}
  else
  {digitalWrite(2, LOW);}
}

void handle_OnConnect() {
  LED1status = LOW;
  LED2status = LOW;
  Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status)); 
}

void handle_led1on() {
  LED1status = HIGH;
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(true,LED2status)); 
}

void handle_led1off() {
  LED1status = LOW;
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(false,LED2status)); 
}

void handle_led2on() {
  LED2status = HIGH;
  Serial.println("GPIO6 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status,true)); 
}

void handle_led2off() {
  LED2status = LOW;
  Serial.println("GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

void handle_slider() {
  String value = server.arg("value");
  orange_val = value.toInt();
  float percent_a = orange_val *  30/544.0 / 100.0;
  int duty_a = 255 - int(255 * percent_a);
  analogWrite(4, duty_a);
  Serial.print("Set orange light to ");
  Serial.println(percent_a);
  server.send(200, "text/plain", "OK");
}

void handle_slider2() {
  String value = server.arg("value");
  blue_val = value.toInt();
  float percent_b = blue_val *  30/544.0 / 100.0;
  int duty_b = 255 - int(255 * percent_b);
  analogWrite(5, duty_b);
  Serial.print("Set blue light to ");
  Serial.println(percent_b);
  server.send(200, "text/plain", "OK");
}

String SendHTML(uint8_t led1stat,uint8_t led2stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP8266 Web Server</h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
  
  if(led1stat)
  {ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";}
  else
  {ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";}

  if(led2stat)
  {ptr +="<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";}
  else
  {ptr +="<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n";}

  ptr += "<input type=\"range\" min=\"0\" max=\"544\" value=\"" + String(orange_val) + "\" id=\"slider\" onchange=\"updateSlider(this.value)\" style=\"width: 100%; height: 30px;\">\n";
  ptr += "<p>Orange: <span id=\"sliderValue\">" + String(orange_val) + "</span></p>\n";

  ptr += "<input type=\"range\" min=\"0\" max=\"544\" value=\"" + String(blue_val) + "\" id=\"slider2\" onchange=\"updateSlider2(this.value)\" style=\"width: 100%; height: 30px;\">\n";
  ptr += "<p>Blue: <span id=\"sliderValue2\">" + String(blue_val) + "</span></p>\n";

  // ptr += "<input type=\"range\" min=\"0\" max=\"544\" value=\"0\" id=\"slider\" onchange=\"updateSlider(this.value)\" style=\"width: 100%; height: 30px;\">\n";
  // ptr += "<p>Orange: <span id=\"sliderValue\">0</span></p>\n";


  // ptr += "<input type=\"range\" min=\"0\" max=\"544\" value=\"0\" id=\"slider2\" onchange=\"updateSlider2(this.value)\" style=\"width: 100%; height: 30px;\">\n";
  // ptr += "<p>Blue: <span id=\"sliderValue2\">0</span></p>\n";



  ptr += "<script>\n";
  ptr += "function updateSlider(value) {\n";
  ptr += " document.getElementById('sliderValue').textContent = value;\n";
  ptr += " document.body.style.backgroundColor = 'orange';\n";
  ptr += " var xhttp = new XMLHttpRequest();\n";
  ptr += " xhttp.open('GET', '/slider?value=' + value, true);\n";
  ptr += " xhttp.send();\n";
  ptr += "}\n";

  ptr += "function updateSlider2(value) {\n";
  ptr += " document.getElementById('sliderValue2').textContent = value;\n";
  ptr += " document.body.style.backgroundColor = 'blue';\n";
  ptr += " var xhttp = new XMLHttpRequest();\n";
  ptr += " xhttp.open('GET', '/slider2?value=' + value, true);\n";
  ptr += " xhttp.send();\n";
  ptr += "}\n";
  ptr += "</script>\n";


  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}


