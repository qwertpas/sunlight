#include <Arduino.h>
#line 1 "/Users/chris/Code/sunlight/espled/espled.ino"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <math.h>

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

int C_val = 0;
int I_val = 0;

#line 22 "/Users/chris/Code/sunlight/espled/espled.ino"
void setup();
#line 54 "/Users/chris/Code/sunlight/espled/espled.ino"
void loop();
#line 63 "/Users/chris/Code/sunlight/espled/espled.ino"
void handle_OnConnect();
#line 70 "/Users/chris/Code/sunlight/espled/espled.ino"
void handle_led1on();
#line 76 "/Users/chris/Code/sunlight/espled/espled.ino"
void handle_led1off();
#line 82 "/Users/chris/Code/sunlight/espled/espled.ino"
void handle_led2on();
#line 88 "/Users/chris/Code/sunlight/espled/espled.ino"
void handle_led2off();
#line 94 "/Users/chris/Code/sunlight/espled/espled.ino"
void handle_NotFound();
#line 98 "/Users/chris/Code/sunlight/espled/espled.ino"
void handle_C_slider();
#line 104 "/Users/chris/Code/sunlight/espled/espled.ino"
void handle_I_slider();
#line 110 "/Users/chris/Code/sunlight/espled/espled.ino"
void update_lights();
#line 146 "/Users/chris/Code/sunlight/espled/espled.ino"
String SendHTML(uint8_t led1stat,uint8_t led2stat);
#line 22 "/Users/chris/Code/sunlight/espled/espled.ino"
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

  server.on("/I_slider", handle_I_slider);
  server.on("/C_slider", handle_C_slider);
  
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

void handle_C_slider() {
  String value = server.arg("value");
  C_val = value.toInt(); 
  update_lights();
}

void handle_I_slider() {
  String value = server.arg("value");
  I_val = value.toInt();
  update_lights();
}

void update_lights(){
  float orange_val = 0;
  float blue_val = 0;
  float cutoff = 0.05;
  
  if(I_val != 0){
    float I_exp = 100*(exp(0.03*I_val)-1) / (exp(0.03*100)-1); //inverse log scaling
    orange_val = I_exp*(100-C_val)*0.6/10000.0 + cutoff;
    blue_val = I_exp*C_val*0.6/10000.0 + cutoff;

    float net = orange_val + blue_val;
    if(net > 0.6){
      orange_val -= (net-0.6)/2.;
      blue_val -= (net-0.6)/2.;
    }
    if(C_val == 0){
      blue_val = 0;
    }else if(C_val == 100){
      orange_val = 0;
    }

  }
  

  int duty_a = 255 - int(255 * orange_val);
  int duty_b = 255 - int(255 * blue_val);

  analogWrite(4, duty_a);
  analogWrite(5, duty_b);
  Serial.print("orange: ");
  Serial.print(orange_val);
  Serial.print(", blue: ");
  Serial.println(blue_val);
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

  ptr += "<input type=\"range\" min=\"0\" max=\"100\" value=\"" + String(C_val) + "\" id=\"C_slider\" oninput=\"update_C_slider(this.value)\" style=\"width: 100%; height: 30px;\">\n";
  ptr += "<p>Color: <span id=\"C_slider_value\">" + String(C_val) + "</span></p>\n";



  ptr += "<input type=\"range\" min=\"0\" max=\"100\" value=\"" + String(I_val) + "\" id=\"I_slider\" oninput=\"update_I_slider(this.value)\" style=\"width: 100%; height: 30px;\">\n";
  ptr += "<p>Intensity: <span id=\"I_slider_value\">" + String(I_val) + "</span></p>\n";



  ptr += "<script>\n";
  ptr += "function update_C_slider(value) {\n";
  ptr += " document.getElementById('C_slider_value').textContent = value;\n";
  // ptr += " document.body.style.backgroundColor = 'orange';\n";
  ptr += " var xhttp = new XMLHttpRequest();\n";
  ptr += " xhttp.open('GET', '/C_slider?value=' + value, true);\n";
  ptr += " xhttp.send();\n";
  ptr += "}\n";

  ptr += "function update_I_slider(value) {\n";
  ptr += " document.getElementById('I_slider_value').textContent = value;\n";
  // ptr += " document.body.style.backgroundColor = 'blue';\n";
  ptr += " var xhttp = new XMLHttpRequest();\n";
  ptr += " xhttp.open('GET', '/I_slider?value=' + value, true);\n";
  ptr += " xhttp.send();\n";
  ptr += "}\n";
  ptr += "</script>\n";


  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}


