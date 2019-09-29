# ESPNOW_ESP8266

ESP_NOW Protocol on ESP8266. 

peer to peer communication between two ESPs

Press the button in one ESP to lightup a LED on the other board and vice versa
 
   How does it Works : 
   
    Upload this code as it is to your ESP module
    Open up serial monitor you will se something like this
    MAC address of this node is XX:XX:XX:XX:XX:XX
    Note down the MAC address
    Do the Same for both Modules
    Now change the code of the FIRST node ( 00 in the PEER[]) with the actual MAC address values of the SECOND node
    upload the code and it's done!
