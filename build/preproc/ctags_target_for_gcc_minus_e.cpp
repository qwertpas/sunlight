# 1 "/Users/chris/Code/sunlight/espled/espled.ino"
# 2 "/Users/chris/Code/sunlight/espled/espled.ino" 2
# 3 "/Users/chris/Code/sunlight/espled/espled.ino" 2

/* Put your SSID & Password */
const char* ssid = "Sunlight"; // Enter SSID here
const char* password = "espleded"; //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

bool LED1status = 0x0;
bool LED2status = 0x0;

void setup() {
  Serial.begin(115200);
  pinMode(2, 0x01);
  digitalWrite(2, 0x0);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  digitalWrite(2, 0x1);


  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led2off);
  server.onNotFound(handle_NotFound);

  server.on("/slider", handle_slider);

  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();

  if(LED2status)
  {digitalWrite(2, 0x1);}
  else
  {digitalWrite(2, 0x0);}
}

void handle_OnConnect() {
  LED1status = 0x0;
  LED2status = 0x0;
  Serial.println("GPIO7 Status: OFF | GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status));
}

void handle_led1on() {
  LED1status = 0x1;
  Serial.println("GPIO7 Status: ON");
  server.send(200, "text/html", SendHTML(true,LED2status));
}

void handle_led1off() {
  LED1status = 0x0;
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(false,LED2status));
}

void handle_led2on() {
  LED2status = 0x1;
  Serial.println("GPIO6 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status,true));
}

void handle_led2off() {
  LED2status = 0x0;
  Serial.println("GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,false));
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

void handle_slider() {
  String value = server.arg("value");
  Serial.print("Slider Value: ");
  Serial.println(value);
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


  ptr += "<input type=\"range\" min=\"0\" max=\"255\" value=\"0\" id=\"slider\" onchange=\"updateSlider(this.value)\">\n";
  ptr += "<p>Slider Value: <span id=\"sliderValue\">0</span></p>\n";


  ptr += "<script>\n";
  ptr += "function updateSlider(value) {\n";
  ptr += " document.getElementById('sliderValue').textContent = value;\n";
  ptr += " if (value > 90) {\n";
  ptr += " document.body.style.backgroundColor = 'brown';\n";
  ptr += " } else {\n";
  ptr += " document.body.style.backgroundColor = 'purple';\n";
  ptr += " }\n";
  ptr += " var xhttp = new XMLHttpRequest();\n";
  ptr += " xhttp.open('GET', '/slider?value=' + value, true);\n";
  ptr += " xhttp.send();\n";
  ptr += "}\n";
  ptr += "</script>\n";


  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
