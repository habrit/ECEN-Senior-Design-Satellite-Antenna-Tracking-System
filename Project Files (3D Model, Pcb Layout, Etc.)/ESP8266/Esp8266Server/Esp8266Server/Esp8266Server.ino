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
MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDYNTVYXPlGmlL2
o7YxFUvpCIsCSbDhNU8YXUz8V2s+s29qlgkMJFl2MSKPFunZ9oefugJ2u+jvlTmh
mwDO2wjb8jN5RIXL8pJ7xOw3lWyNl5p1VPNQc2bDEC4D8mOVg/OvELubrSdsBO9N
HMH93Vxl/DeOfw3jVKnQgrlB8kTfrdnj/SYe7mjOlEkPnmPldnvZhdN9B036aTfz
l1Tqny+9UewXQ9IpdMYk/H5wCRzumyV9lBLzJCWah1wdWrXcyA/y38iT6nfHnkno
CxhrowknIUUwyT29sotvSxKmEwgCCVoakSwUXmxlg6kpigBKC5KZ94jvE8DnEpQ5
MCGlOvsvAgMBAAECggEABjf5kdISxWoY6skoq16+cTRysx0lfkTHQp8kYLmu9v24
PauVFQlSnWiqYs046rEBk6GJAptDBukW4EQSEOER7ymXO2yyIHyYpAC0/qBIoAX1
RlysJus60G99s1Vcced6sdVcYAzp7lxx+ZjTJ5cfWjuX6XxUDO7Zd4rbAz+RpZFQ
AxmDLMoGT8KTdRLZdPPIW78oXBQrFGXK3ul+WaWrzce/4PFoWLPK9mTUwfNLBasM
quyNhNJ67h2zbATCEgSvhOOGMwSiiq1rt8WWJMCz2nZ/cAayqeAL1KC5uqBIRkbD
amikUkxR1RDN1j0i+uvDa0XgVNz7nZWxPFit5FLLyQKBgQDunFHXiG9Xo2tb6JpJ
IVDpqcF55qEio6nRt6pZJeopQTV3pAUbRUDKcZo3PHsqfB+i/vKeqApAIt/LbFvO
cx/k0BHNevnB9NLwnMNiXo72jw6acGVe/od9dzQ3NqOqbxuanq8nLDyMxvKyeEaP
h+9RPnWQI27Mc0HykoKYJUcf1QKBgQDn9u1on0v9O+Ewj67hWI2xwhlaGT9Ob8D+
G9NHqqEACl0lc3pLll2WFxiDZPtpsVow4ITfzZ0fGvJ1bo+HSlFzCWcGkUoh4o2j
SKiN8PyhlBY2J/52gN8Knj53BlE6ux9+84XXazycNK9DJafGCdo0uOd5k+uCtALE
9MeUf/608wKBgCKaSMQY5QywhkEt2jIAwtsf4w7qgzOiyF3BZNLIzMxuyHIxNwxd
Xqf9EYAxJStkFLJuhoC7ncDvTcUSUb3+tAFaqzpBOzcFahG6qhr524Gl/VgoNIsy
HjPztX7MTa/JrRcfLAdiQuWndEqtZo4qCGqfxCzC9q5viIDE/mtR/J6BAoGBALUf
vU71G5u3duW5O79fqvxC6BLPJfx0DIkeZwJNvHtQnFzS54o/9gFtNUnbvuhPgHIr
zfUHEwmnu+xwEQh9wk2F6Pla1zzQLvJxmJOEuW7dN96/qdUUWZRr7bB4sVKfrD8v
nbGQXXpHNRbEP1XGeaizPKNVGg8rygHKdFCDBPp7AoGBALbAfLpl2i9VtwAOeofK
drTKvUi5L/iA52IdupBx1+nIA2hRVQRnwoo4uSaarEAXg3FuDrLQmcA9FpMBW1eM
d0YH37RaUMZDxmiCkEpFkyqtb4G2/m8UZ1W6B+HwX2AybnWpe3KIwva5Itb9aVFx
EHHyeyYu/+0oPosWVYtNeN/o
-----END PRIVATE KEY-----
)EOF";

// Set Server Certificate
const char server_cert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDnzCCAocCFHWLd/xNg9sbWNql8Yk8TBljv+WYMA0GCSqGSIb3DQEBCwUAMIGL
MQswCQYDVQQGEwJVUzEOMAwGA1UECAwFVGV4YXMxGDAWBgNVBAcMD0NvbGxlZ2Ug
U3RhdGlvbjEMMAoGA1UECgwDQSZNMQ0wCwYDVQQLDARFQ0VOMRQwEgYDVQQDDAtE
YXZpZFNhbnRvczEfMB0GCSqGSIb3DQEJARYQZG1zNDk5NUB0YW11LmVkdTAeFw0y
MzAzMjgxNjUwMTlaFw0yNDAzMjcxNjUwMTlaMIGLMQswCQYDVQQGEwJVUzEOMAwG
A1UECAwFVGV4YXMxGDAWBgNVBAcMD0NvbGxlZ2UgU3RhdGlvbjEMMAoGA1UECgwD
QSZNMQ0wCwYDVQQLDARFQ0VOMRQwEgYDVQQDDAtEYXZpZFNhbnRvczEfMB0GCSqG
SIb3DQEJARYQZG1zNDk5NUB0YW11LmVkdTCCASIwDQYJKoZIhvcNAQEBBQADggEP
ADCCAQoCggEBANg1NVhc+UaaUvajtjEVS+kIiwJJsOE1TxhdTPxXaz6zb2qWCQwk
WXYxIo8W6dn2h5+6Ana76O+VOaGbAM7bCNvyM3lEhcvyknvE7DeVbI2XmnVU81Bz
ZsMQLgPyY5WD868Qu5utJ2wE700cwf3dXGX8N45/DeNUqdCCuUHyRN+t2eP9Jh7u
aM6USQ+eY+V2e9mF030HTfppN/OXVOqfL71R7BdD0il0xiT8fnAJHO6bJX2UEvMk
JZqHXB1atdzID/LfyJPqd8eeSegLGGujCSchRTDJPb2yi29LEqYTCAIJWhqRLBRe
bGWDqSmKAEoLkpn3iO8TwOcSlDkwIaU6+y8CAwEAATANBgkqhkiG9w0BAQsFAAOC
AQEAG8scg+QVUYnxJJ3e9WPnrEwzTUGPNGbLdhULnNNXDxdGY/20Ne64vJmyMh9h
cGIsJeUUtgWuNSsTC9sWNISsh96NH297AuLpYs4NcbPJaZMB/vApFuI01V6g74WQ
pXd+9Atrgs5W9wTDCUW63QNkF2YDO5jDYG1AYvAFyKf51MZvD6+cMeXhQx7B5V8j
x8OGU5HqrYu/yzgX0/O9cNcS5vxlvUd4yZ6w6OXtvWXBDjTh4kPyTqGWVLz3U1Tz
WDZw81GLkuSc9MIEkJX5yKw0huY8b2OvaDOtErDu5jNMHTPkfSRifvhybCSKdIux
53PU70olAhSLqZmiIDam+FJvSA==
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