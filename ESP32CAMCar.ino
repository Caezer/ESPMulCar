

// Import required libraries
#ifdef ESP32
  #include <WiFi.h>
  #include <ESPAsyncWebServer.h>
#else
  #include <Arduino.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
#endif
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is connected to GPIO 4
#define ONE_WIRE_BUS 12

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

// Variables to store values
String temperatureF = "";
String temperatureC = "";
String noise = "";
String vibration = "";
String light = "";


// Timer variables
unsigned long lastTime = 0;  
unsigned long timerDelay = 1000;


int sound_analog = 26;
int sound_digital = 27;
int shocksensor = 14;
int sensorPin = 25;
int ambient;



// Replace with your network credentials
const char* ssid = "yourssid";
const char* password = "yourpassword";




// Create AsyncWebServer object on port 80
AsyncWebServer server(80);




String readDSTemperatureC() {
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
  float tempC = sensors.getTempCByIndex(0);

  if(tempC == -127.00) {
    Serial.println("Failed to read from DS18B20 sensor");
    return "--";
  } else {
    Serial.print("Temperature Celsius: ");
    Serial.println(tempC); 
  }
  return String(tempC);
}

String readDSTemperatureF() {
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
  float tempF = sensors.getTempFByIndex(0);

  if(int(tempF) == -196){
    Serial.println("Failed to read from DS18B20 sensor");
    return "--";
  } else {
    Serial.print("Temperature Fahrenheit: ");
    Serial.println(tempF);
  }
  return String(tempF);
}

String sound(){
  String nois = "No sound";
  int valdigital = digitalRead(sound_digital);
  int valanalog = analogRead(sound_analog);
  if (valdigital == HIGH){
    nois = "Detected sound";
  }
  else if (valdigital == LOW){
    nois = "No sound";
  }
  return nois;
}

String shock(){
  String vibe = "None";
  int shock = 0;
  shock = digitalRead(shocksensor);
  if (shock == 1){
    vibe = "Vibration";
  }else if(shock == 0) {
    vibe = "None";
   }
   return vibe;
  }


String bright(){
  String lit = "Normal";
  int ambi;
  ambi = analogRead(sensorPin);
  Serial.println(ambient);
  Serial.println(ambi);
  if (ambient > 3000){
    lit = "Normal";
  }
  else if(ambient > 2300) {
    lit = "Low";
  } else if (ambient < 1000){
    lit = "Bright";
  }
  return lit;
}





const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .ds-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>ESP Car Sensor Server</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="ds-labels">Temperature Celsius</span> 
    <span id="temperaturec">%TEMPERATUREC%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="ds-labels">Temperature Fahrenheit</span>
    <span id="temperaturef">%TEMPERATUREF%</span>
    <sup class="units">&deg;F</sup>
  </p>
   <p>
    <i class="fas fa-solid fa-volume-high"></i>    
    <span class="ds-labels">Sound</span>
    <span id="sound">%SOUND%</span>
  </p>
   <p>
    <i class="fas fa-regular fa-lightbulb"></i>
    <span class="ds-labels">Brightness</span>
    <span id="Light">%LIGHT%</span>
  </p>
   <p>
    <i class="fas fa-solid fa-burst" style="color: #8aff97;"></i>    
    <span class="ds-labels">Shock</span>
    <span id="Shock">%SHOCK%</span>
  </p>    
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperaturec").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperaturec", true);
  xhttp.send();
}, 10000) ;
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperaturef").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperaturef", true);
  xhttp.send();
}, 10000) ;
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("sound").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/sound", true);
  xhttp.send();
}, 10000) ;
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("Light").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/Light", true);
  xhttp.send();
}, 10000) ;
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("Shock").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/Shock", true);
  xhttp.send();
}, 10000) ;
</script>
</html>)rawliteral";

// Replaces placeholder with DS18B20 values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATUREC"){
    return temperatureC;
  }
  else if(var == "TEMPERATUREF"){
    return temperatureF;
  }
  else if (var == "SOUND"){
    return noise;
  }
  else if (var == "LIGHT"){
    return light;
  }
  else if (var == "SHOCK"){
    return vibration;
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();
  pinMode(sound_digital, INPUT);  
  //ambient = analogRead(sensorPin);
  pinMode(shocksensor,INPUT);



  
  // Start up the DS18B20 library
  sensors.begin();

  temperatureC = readDSTemperatureC();
  temperatureF = readDSTemperatureF();
  noise = sound();
  vibration = shock();
  light = bright();
  

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperaturec", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", temperatureC.c_str());
  });
  server.on("/temperaturef", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", temperatureF.c_str());
  });
  server.on("/sound", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", noise.c_str());
  });  
  server.on("/Shock", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", vibration.c_str());
  });
  server.on("/Light", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", light.c_str());
  });  // Start server
  server.begin();
}
 
void loop(){
  if ((millis() - lastTime) > timerDelay) {
    temperatureC = readDSTemperatureC();
    temperatureF = readDSTemperatureF();
    noise = sound();
    vibration = shock();
    light = bright();
    lastTime = millis();
  }  
}
