#include <ESP8266WiFi.h>
#include <time.h>
#include <ArduinoJson.h>

#ifndef STASSID
//#define STASSID "ASUS-10 2.4G"
//#define STAPSK "97955757"
//192.168.50.100

#define STASSID "DavidPhone"
#define STAPSK "DavidSantos"
//172.20.10.3
#endif

const char *ssid = STASSID;
const char *pass = STAPSK;

// Set Server Port
BearSSL::WiFiServerSecure server(443);

// Set Server Private Key: 
const char server_private_key[] PROGMEM = R"EOF(
-----BEGIN PRIVATE KEY-----
[Removed]
-----END PRIVATE KEY-----
)EOF";

// Set Server Certificate
const char server_cert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
[Removed]
-----END CERTIFICATE-----
)EOF";


#define CACHE_SIZE 5  // Cache Sessions
#define USE_CACHE     // Enable Cache
//#define DYNAMIC_CACHE //Enable Dynamic

#if defined(USE_CACHE) && defined(DYNAMIC_CACHE)
// Dynamically allocated cache.
BearSSL::ServerSessions serverCache(CACHE_SIZE);
#elif defined(USE_CACHE)
// Statically allocated cache.
ServerSession store[CACHE_SIZE];
BearSSL::ServerSessions serverCache(store, CACHE_SIZE);
#endif

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  // Connect Wifi Network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Attach Certificate/Key to Server
  BearSSL::X509List *serverCertList = new BearSSL::X509List(server_cert);
  BearSSL::PrivateKey *serverPrivKey = new BearSSL::PrivateKey(server_private_key);

#ifndef USE_EC
  server.setRSACert(serverCertList, serverPrivKey);
#else
  server.setECCert(serverCertList, BR_KEYTYPE_KEYX | BR_KEYTYPE_SIGN, serverPrivKey);
#endif

  // Setup Cache
#if defined(USE_CACHE)
  server.setCache(&serverCache);
#endif

  // Start Server
  server.begin();
}

void loop() {

  // Check for Client
  BearSSL::WiFiClientSecure client = server.accept();
  if (!client) { return; }

  // Read Request
  const String request = client.readStringUntil('\r');
  client.flush();

  // Respond to Request
  respond(client, request);

  // End Connection
  client.flush();
  client.stop();
  Serial.printf("Client Disconnected\n");
}


void respond(BearSSL::WiFiClientSecure client, String request) {
  //[TO-DO]: Make RequestType work for all types
  // Initialize Response Variables
  //Serial.println("Client Request:: " + request);
  String RequestType = request.substring(request.indexOf('/') + 1);
  RequestType = RequestType.substring(0,RequestType.indexOf('/'));
  Serial.println("Client Request:: " + RequestType);
  String Header = "HTTP/1.1 200 OK";
  String Methods = "GET";

  // Construct a JSON array with the response data
  DynamicJsonDocument doc(200);
  doc["RequestType"] = RequestType;


  //[TO-DO] - respond to "favicon.ico" request
  //[TO-DO] - get real coordinates/position from mcu
  //[TO-DO] - send payload to mcu if needed
  if (RequestType == "Connect") {
    doc["NAME"] = "David's Rotator";
    doc["IP"] = WiFi.localIP();
    doc["CALL_SIGN"] = "W5QZ";
    doc["MODEL"] = "Yaesu G-5500";
    doc["LONGITUDE"] = "30.6280";
    doc["LATITUDE"] = "-96.3344";
    doc["HEIGHT"] = "1";    
    
  } else if (RequestType == "Stop"){
    //tell mcu to stop tracking
    //continue once mcu has stopped
    doc["message"] = "Rotator has stopped tracking";
 
  }else if(RequestType == "StartAuto"){
    doc["message"] = "Rotator has started auto-tracking"; 
 
  }else if(RequestType == "StartManual"){
    doc["message"] = "Rotator has started manual-tracking";
  
  }else {
    doc["message"] = "HTTP/1.1 404 Not Found";
    Header += "HTTP/1.1 404 Not Found";
    Methods = "*";
  }

  // Serialize the JSON array to a string
  String jsonString;
  serializeJson(doc, jsonString);

  // Headers
  client.println(Header);
  client.println("Content-Type: application/json");
  client.println("Access-Control-Allow-Origin: *");
  client.println("Access-Control-Allow-Methods: " + Methods);
  client.println("");

  //Send Data
  client.print(jsonString);
}