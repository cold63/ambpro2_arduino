/*
 WiFi UDP Send and Receive String

 This sketch wait an UDP packet on localPort.
 When a packet is received an Acknowledge packet is sent to the client on port remotePort

 created 30 December 2012
 by dlf (Metodo2 srl)

 modified 17 Jan 2023
 by Realtek SG

 Example guide:
 https://www.amebaiot.com/en/amebapro2-arduino-ameba-udp-server/
 */

#include <WiFi.h>
#include <WiFiUdp.h>

char ssid[] = "Network_SSID";       // your network SSID (name)
char pass[] = "Password";           // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                   // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;        // Indicater of Wifi status

unsigned int localPort = 2390;      // local port to listen on

char packetBuffer[255]; //buffer to hold incoming packet
char ReplyBuffer[] = "acknowledged";       // a string to send back

WiFiUDP Udp;

void setup() {
    //Initialize serial and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    // attempt to connect to Wifi network:
    while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid,pass);

        // wait 10 seconds for connection:
        delay(10000);
    }
    Serial.println("Connected to wifi");
    printWifiStatus();

    Serial.println("\nStarting connection to server...");
    // if you get a connection, report back via serial:
    Udp.begin(localPort);
}

void loop() {
    // if there's data available, read a packet
    int packetSize = Udp.parsePacket();
    if (packetSize) {
        Serial.print("Received packet of size ");
        Serial.println(packetSize);
        Serial.print("From ");
        IPAddress remoteIp = Udp.remoteIP();
        uint16_t remotePort = Udp.remotePort();
        Serial.print(remoteIp);
        Serial.print(", port ");
        Serial.println(remotePort);

        // read the packet into packetBufffer
        int len = Udp.read(packetBuffer, 255);
        if (len > 0) {
            packetBuffer[len] = 0;
        }
        Serial.println("Contents:");
        Serial.println(packetBuffer);

        // send a reply, to the IP address and port that sent us the packet we received
        Udp.beginPacket(remoteIp, remotePort);
        Udp.write(ReplyBuffer);
        Udp.endPacket();
    }
}

void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}
