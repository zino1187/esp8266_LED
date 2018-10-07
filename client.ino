#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#define AP_NAME "와이파이명"
#define AP_PASSWORD "비밀번호"
#define SERVER_IP "접속할 서버IP"
#define SERVER_PORT 7777

ESP8266WiFiMulti WiFiMulti;
WiFiClient client;// Use WiFiClient class to create TCP connections
    
void setup() {
    ESP.wdtDisable();
    ESP.wdtEnable(WDTO_8S);
    pinMode(D5, OUTPUT);   
    
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network

    /*--------------------------------------------
    wifi 공유기 네트워크명 / 비밀번호
    --------------------------------------------*/
    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(AP_NAME, AP_PASSWORD);

    Serial.println();
    Serial.println();
    Serial.print("WiFi 접속시도 중입니다... ");

    /*--------------------------------------------
    wifi 에 접속될 때까지 무한 연결을 시도한다  
    --------------------------------------------*/
    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    /*--------------------------------------------
    wifi 에 연결되면 공유기로 부터 할당받은 ip를 출력한다
    --------------------------------------------*/
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    delay(500);

    /*--------------------------------------------
    wifi 에 연결되었으면, 이제 소켓 서버에 접속을 시도한다
    --------------------------------------------*/
    const uint16_t port = SERVER_PORT;
    const char * host = SERVER_IP; // ip or dns
    
    Serial.print("connecting to ");
    Serial.println(host);

    Serial.println("소켓서버에 접속을 시도하고 있습니다");
    while(!client.connect(host, port)){
        Serial.println(".");
        delay(1000);
    }
    Serial.println("소켓서버에 접속하였습니다");    
    delay(1000);
}

void loop() {
    ESP.wdtFeed();
    
    /*--------------------------------------------
     서버로부터 한줄 읽어오기(read back one line from server)
    --------------------------------------------*/
    String msg = client.readStringUntil('\n');
    
    //Serial.println("서버에서 보내온 메세지는");    
    Serial.println(msg);

    //D5에 전기 출력하기
    if(msg.equals("on")){
      digitalWrite(D5 , HIGH);      
      Serial.println("LED 켭니다");
    }else if(msg.equals("off")){
      digitalWrite(D5 , LOW);
      Serial.println("LED 끕니다");
    }
    
    
    //Serial.println("closing connection");
    //client.stop();
    //Serial.println("wait 5 sec...");
    delay(10);
}
