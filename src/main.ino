/*
ESP-COAP Client
*/
#include <WiFi.h>
#include "coap_client.h"

//instance for coapclient
coapClient coap;
coapPacket responsePacket;
String CoapPayload;
//WiFi connection info
// const char* ssid = "Oizom2G";
// const char* password = "9510810866";

const char* ssid = "Omm";
const char* password = "physicsddd@12332";
//ip address and default port of coap server in which your interested in
//IPAddress ip(129,132,15,80);//take ETH Zurich or coap.me server to run and check client 
IPAddress ip(192,168,0,197);
IPAddress leshan(5,39,83,206);
int port =5683;

// coap client response callback
void callback_response(coapPacket packet, IPAddress ip, int port);

// coap client response callback
void callback_response(coapPacket packet, IPAddress ip, int port) {


    

    //response from coap server
    if(packet.type==3 && packet.code==0)
    {
      Serial.println("ping ok");
    }
    if(packet.type == 2 )
    {
      Serial.println("ACK");
    }

    responsePacket = packet;
    memcpy(responsePacket.payload,packet.payload, packet.payloadlen);
    memcpy(responsePacket.token,packet.token, packet.tokenlen);
    Serial.println("data:");
    for(int i=0;i<packet.payloadlen;i++){
        char _temp = (char)packet.payload[i];
        if(_temp) Serial.print(_temp);
    }

    char p[responsePacket.payloadlen + 1];
    memcpy(p, responsePacket.payload, responsePacket.payloadlen);
    p[responsePacket.payloadlen] = NULL;
    CoapPayload = String(p);
}


void setup() {
   
    Serial.begin(115200);

    WiFi.begin(ssid, password);

    // Connection info to WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");

    // Print the IP address of client
    Serial.println(WiFi.localIP());

    // client response callback.
    // this endpoint is single callback.
    coap.response(callback_response);

    // start coap client
    coap.start();
    
    char* payload = "</1/1>";
    String query[] = {"ep=EspOizom","lt=1246543","lwm2m=1"};
    uint8_t query_number[] = {15,15,15};

    delay(1000);
    coap.post(leshan,port,"rd",payload, strlen(payload), query_number, query);

    delay(1000);
    payload = "</1>, </2>, </3>, </4>, </5>";
    coap.post(leshan,port,"rd",payload, strlen(payload), query_number, query);
}


void loop() {
    bool state;
    state = coap.loop();
    if(state == true){

        Serial.println("\n");
        Serial.print("len   :");Serial.println(responsePacket.payloadlen);
        Serial.print("type  :");Serial.println(responsePacket.type);
        Serial.print("msgID :");Serial.println(responsePacket.messageid);
        Serial.print("total :");Serial.println(responsePacket.optionnum);
        Serial.println("\n");
        
        String message = "";

        for (int i = 0; i < responsePacket.optionnum; i++) {
            
            message = "";
            char urlname[responsePacket.options[i].length + 1];
            memcpy(urlname, responsePacket.options[i].buffer, responsePacket.options[i].length);
            urlname[responsePacket.options[i].length] = NULL;
            message += urlname;
            Serial.print("\nnumber:");Serial.println(responsePacket.options[i].number);
            Serial.print("length:");Serial.println(responsePacket.options[i].length);
            Serial.print("data  :");Serial.println(message);
        }
        if(responsePacket.payloadlen > 0)
        {
            char p[responsePacket.payloadlen + 1];
            memcpy(p, responsePacket.payload, responsePacket.payloadlen);
            p[responsePacket.payloadlen] = NULL;
            Serial.print("Experimental payload:");Serial.println(String(p));
            Serial.print("Payload:");Serial.println(CoapPayload);
        }
    }
    delay(10);
}
