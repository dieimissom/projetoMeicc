// main.ino
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include "web_server.h"
#include "history_system.h" // Incluído o novo arquivo de histórico

extern History history; // Declarar a instância global de History

const byte DNS_PORT = 53;
DNSServer dnsServer;
ESP8266WebServer server(80);

// Admin credentials
const char* admin_username = "admin";
const char* admin_password = "admin"; // Senha alterada para \'admin\' conforme solicitado

void setup() {
  Serial.begin(115200);
  Serial.println("\nBooting...");

  // Initialize SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }
  Serial.println("File system mounted successfully");

  // Load history from SPIFFS
  history.load();

  // Configure ESP as Access Point
  WiFi.mode(WIFI_AP);
  IPAddress local_IP(8, 8, 8, 8);
  IPAddress gateway(8, 8, 8, 8);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP("EscolaRS_WiFi", ""); // No password for captive portal
  Serial.print("Access Point IP address: ");
  Serial.println(WiFi.softAPIP());

  // Redirect all DNS requests to the ESP\"s IP
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  // Setup web server handlers
  setupWebServer(server, admin_username, admin_password);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
  // history.updateConnectedClients(); // Removido: O histórico será atualizado apenas no clique do botão
}
