/*#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Wire.h>*/
#include "arduinoFFT.h" // Standard Arduino FFT library https://github.com/kosme/arduinoFFT 
#include "font.h"       // The font.h file must be in the same folder as this sketch
#include "SH1106.h"     // https://github.com/squix78/esp8266-oled-ssd1306

#define SAMPLES 1024             // Must be a power of 2
#define SAMPLING_FREQUENCY 40000 // Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.
#define amplitude 150            // Depending on your audio source level, you may need to increase this value

//var global pour le réseau
const char* ssid = "tardigrade";
const char* password = "cryptide";

//var global pour la frequence
unsigned int sampling_period_us;
unsigned long microseconds;
byte peak[] = {0,0,0,0,0,0,0,0};
double vReal[SAMPLES];
double vImag[SAMPLES];
unsigned long newTime, oldTime;
int dominant_value;

SH1106 display(0x3c, 5,4); // 1.3" OLED display object definition (address, SDA, SCL) Connect OLED SDA , SCL pins to ESP SDA, SCL pins
//WebServer server(80);
arduinoFFT FFT = arduinoFFT();

//unsigned long long serverUpdateMillis = 0;

/*String rootHTML = "\
<!doctype html> <html> <head> <title> EMF Sensor </title> </head> <body>\
<br> <br> <span>EMF : <b id=\"ev\">emf_val<b/></span> <br> <br> </body>\
<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js\"></script>\
<script>setInterval( () => { $.ajax('/emf_val', { success: (d, ts, j) => { $('#ev').html(d.emf) } }) }, 900);</script>\
</html>\
";
*/

/*void handleRoot() {
  server.send(200, "text/plain", "Je ne comprends rien!");
}*/

void setup() {
  //Serial.begin(115200);
  //paramétre pour lecture de fréquence sur ecran de la wemos wroom lolin32
  Wire.begin(5,4); // SDA, SCL
  display.init();
  display.setFont(ArialMT_Plain_10);
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));

  //paramétre pour le serveur
 /* WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  //Serial.println("");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  //Affichage du réseau de connection + ip
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.begin();
  Serial.println("HTTP server started");
  pinMode(0, ANALOG);*/
}

void loop() {
  //chargement du server
  

  //nettoyage de l'écran et définition des fréquence afficher
  display.clear();        
  display.drawString(0,0,"125 250 500 1K  2K 4K 8K 16K");
  //display.drawString(0,10,String(millis()/100));
  
  for (int i = 0; i < SAMPLES; i++) {
    newTime = micros();
    vReal[i] = analogRead(12); // Using Arduino ADC nomenclature. A conversion takes about 1uS on an ESP32
    //Serial.println(vReal[i]);

    vImag[i] = 0;
    while ((micros() - newTime) < sampling_period_us) { /* do nothing to wait */ }
  }

  //transformation du son en fréquence
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

  //Affichage de la fréquence
  for (int i = 2; i < (SAMPLES/2); i++){ 
    // Don't use sample 0 and only the first SAMPLES/2 are usable.
    // Each array element represents a frequency and its value, is the amplitude. Note the frequencies are not discrete.
    
    if (vReal[i] > 1500) { // Add a crude noise filter, 10 x amplitude or more
      if (i <= 2)             displayBand(0,(int)vReal[i]); // 125Hz
      if (i > 2 && i <= 4)    displayBand(1,(int)vReal[i]); // 250Hz
      if (i > 4 && i <= 7)    displayBand(2,(int)vReal[i]); // 500Hz
      if (i > 7 && i <= 15)   displayBand(3,(int)vReal[i]); // 1000Hz
      if (i > 15 && i <= 40)  displayBand(4,(int)vReal[i]); // 2000Hz
      if (i > 40 && i <= 70)  displayBand(5,(int)vReal[i]); // 4000Hz
      if (i > 70 && i <= 288) displayBand(6,(int)vReal[i]); // 8000Hz
      if (i > 288)            displayBand(7,(int)vReal[i]); // 16000Hz
    }
    
    for (byte band = 0; band <= 7; band++) {
      display.drawHorizontalLine(1+16*band,64-peak[band],14);
    }
  }
  
  if (millis()%4 == 0) {
    for (byte band = 0; band <= 7; band++) {
      if (peak[band] > 0) {
        peak[band] -= 1;
      }
    }
  }
  display.display();
  
  
  
  //WiFi.begin();
  //server.handleClient();
  //WiFi.disconnect();
}

void displayBand(int band, int dsize){
  int dmax = 50;
  dsize /= amplitude;
  if (dsize > dmax) dsize = dmax;
  for (int s = 0; s <= dsize; s=s+2){display.drawHorizontalLine(1+16*band,64-s, 14);}
  if (dsize > peak[band]) {peak[band] = dsize;}
}
