// Configure the framework
#include "bconf/MCU_ESP8266.h"              // Load the code directly on the ESP8266
#include "conf/Gateway.h"                   // The main node is the Gateway, we have just one node
#include "conf/IPBroadcast.h"

// Define the WiFi name and password
#define WIFICONF_INSKETCH
#define WiFi_SSID               "SSID"
#define WiFi_Password           "PASSWORD"    

// Include framework code and libraries
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include "Souliss.h"

// This identify the number of the LED logic
#define CURRENT             0
#define WATTS               2

#define Debug Serial1        //Change to Serial1 if you want to use the GPIO2 to TX

void setup()
{   
    Initialize();
    
    Serial.begin(9600); 
    Debug.begin(115200);

    // Connect to the WiFi network and get an address from DHCP
    GetIPAddress();     
    SetAsGateway(myvNet_dhcp);
    // This node will serve all the others in the network providing an address
    
    // This is the vNet address for this node, used to communicate with other
    // nodes in your Souliss network
    SetAddress(0xAB01, 0xFF00, 0x0000);
    //Example of Peer Definition
    //SetAsPeerNode(0xAB02, 1);
    
    Set_Current(CURRENT);
    Set_Power(WATTS);
}

void loop()
{ 
    
    EXECUTEFAST() {                     
        UPDATEFAST();   
        
        FAST_510ms()    {
           Logic_Current(CURRENT);
           Logic_Power(WATTS);
        }
        FAST_710ms()    {
           float current = 0;
           if(Serial.available()){
               float current = Serial.parseFloat();
               Debug.println(current);
               
               Souliss_ImportAnalog(memory_map, CURRENT, &current); 
               float watt = current*230; 
               Souliss_ImportAnalog(memory_map, WATTS, &watt);
           }
        }
        // Here we handle here the communication with Android
        FAST_GatewayComms();                                        
    }
}    
