/*
ESP-COAP Client
*/
#include <WiFi.h>
#include "coap_client.h"

//instance for coapclient
coapClient coap;

//WiFi connection info
const char* ssid = "Oizom2G";
const char* password = "9510810866";
//ip address and default port of coap server in which your interested in
//IPAddress ip(129,132,15,80);//take ETH Zurich or coap.me server to run and check client 
IPAddress ip(192,168,0,197);
IPAddress leshan(5,39,83,206);
int port =5683;

// coap client response callback
void callback_response(coapPacket &packet, IPAddress ip, int port);

// coap client response callback
void callback_response(coapPacket &packet, IPAddress ip, int port) {
    char p[packet.payloadlen + 1];
    memcpy(p, packet.payload, packet.payloadlen);
    p[packet.payloadlen] = NULL;
    String message(p);

    //response from coap server
    if(packet.type==3 && packet.code==0)
    {
      Serial.println("ping ok");
    }
    if(packet.type == 2 )
    {
      Serial.println("ACK");
    }
    Serial.print("packet:");Serial.print(packet.payloadlen);
    Serial.print("->");
    // Serial.print((char)packet.payload);
    for(int i=0;i<packet.payloadlen;i++){
        char _temp = (char)packet.payload[i];
        if(_temp) Serial.print(_temp);
    }
    // memset(packet.payload,int(0), packet.payloadlen);
    Serial.print("->");Serial.println(packet.type);
    Serial.print("String:");Serial.println(message);
    Serial.print("msgID:");Serial.println(packet.messageid);



    for(int i = 0 ; i < packet.optionnum;i++)
    {
        Serial.print("number:");Serial.println(packet.options[i].number);
    }
}

void setup() {
   
    Serial.begin(115200);

    WiFi.begin(ssid, password);
    Serial.println(" ");

    // Connection info to WiFi network
    Serial.println();
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

    //get request to server (arguments ip adrress of server,default port,resource(uri))
    int msgid = coap.get(ip,port,"time");
    delay(1000);
    
    char* payload = "</1/1>";
    String query[] = {"ep=EspOizom","lt=1246543","lwm2m=1"};
    uint8_t query_number[] = {15,15,15};

//    int id = coap.post(ip,port,"rd",payload, strlen(payload), query_number, query);
    delay(1000);
    coap.post(leshan,port,"rd",payload, strlen(payload), query_number, query);

    delay(1000);
    payload = "</1>, </2>, </3>, </4>, </5>";
    coap.post(leshan,port,"rd",payload, strlen(payload), query_number, query);

    //observe request (arguments ip adrress of server,deafult port,resource name,interger(0) ) 
//    int _id= coap.observe(ip,port,"rd?ep=Oizom&lt=1246543&lwm2m=1",0);

    //reset observe cancel
    //int msgid=coap.observecancel(ip,port,"resoucename");
    
}
//int i=0;

void loop() {
    bool state;

    // Requests

    //get request
    //int msgid = coap.get(ip,port,"hello");

    //put request 
    //arguments server ip address,default port,resource name, payload,payloadlength
    //int msgid =coap.put(ip,port,"resourcename","0",strlen("0"));

    //post request
    //arguments server ip address,default port,resource name, payload,payloadlength
    //int msgid =coap.post(ip,port,"resourcename","0",strlen("0"));

    //delete request
    //int msgid = coap.delet(ip,port,"resourcename");

    //ping
    //int msgid=coap.ping(ip,port);
    
    // int msgid=coap.observe(ip,port,"obs",0);
//      String query[] = {"ep=EspOizom","lt=1246543","lwm2m=1"};
//      uint8_t query_number[] = {15,15,15};
//      char* payload = "</1>, </2>, </3>, </4>, </5>";
//      coap.post(leshan,port,"rd",payload, strlen(payload), query_number, query);
    state = coap.loop();
    Serial.print("state:");Serial.println(state);
    if(state==1){
       
    }   
  
    //Serial.print("i=");
    //Serial.println(i);
      //if(i==3)
    //{
        //Serial.println("cancel observe");
        //coap.observeCancel(ip,port,"resourcename");
    //}
  
    //Serial.println(msgid);
    delay(1000);
}
