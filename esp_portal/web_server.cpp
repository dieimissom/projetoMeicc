#include "web_server.h"

// ---------------------------------------------------------------------------
// Utilitário: serve arquivo do SPIFFS
// ---------------------------------------------------------------------------

void serveSpiffsFile(ESP8266WebServer& server,
                     const String& path,
                     const String& contentType) {
  if (!SPIFFS.exists(path)) {
    server.send(404, "text/plain", "Arquivo nao encontrado: " + path);
    return;
  }

  File f = SPIFFS.open(path, "r");
  if (!f) {
    server.send(500, "text/plain", "Erro ao abrir arquivo: " + path);
    return;
  }

  server.streamFile(f, contentType);
  f.close();
}

// ---------------------------------------------------------------------------
// setupWebServer
// ---------------------------------------------------------------------------

void setupWebServer(ESP8266WebServer& server,
                    const char* admin_username,
                    const char* admin_password) {

  // --- Página principal ---
  server.on("/", HTTP_GET, [&server]() {
    serveSpiffsFile(server, "/index.html", "text/html");
  });

  // --- Página de aviso ---
  server.on("/aviso", HTTP_GET, [&server]() {
    serveSpiffsFile(server, "/aviso.html", "text/html");
  });

  server.on("/aviso.html", HTTP_GET, [&server]() {
    serveSpiffsFile(server, "/aviso.html", "text/html");
  });

  // --- Painel administrativo (protegido por autenticação HTTP Basic) ---
  server.on("/admin", HTTP_GET, [&server, admin_username, admin_password]() {
    if (!server.authenticate(admin_username, admin_password)) {
      return server.requestAuthentication();
    }
    serveSpiffsFile(server, "/admin.html", "text/html");
  });

  server.on("/admin.html", HTTP_GET, [&server, admin_username, admin_password]() {
    if (!server.authenticate(admin_username, admin_password)) {
      return server.requestAuthentication();
    }
    serveSpiffsFile(server, "/admin.html", "text/html");
  });

  // --- Endpoint: histórico de dispositivos (JSON) ---
  server.on("/history", HTTP_GET, [&server, admin_username, admin_password]() {
    if (!server.authenticate(admin_username, admin_password)) {
      return server.requestAuthentication();
    }
    // Atualiza a lista antes de exibir
    history.updateConnectedClients();
    String page = history.generateHistoryPage();
    server.send(200, "text/html", page);
  });

  // --- Endpoint: limpar histórico ---
  server.on("/clear_history", HTTP_POST, [&server, admin_username, admin_password]() {
    if (!server.authenticate(admin_username, admin_password)) {
      return server.requestAuthentication();
    }
    history.clear();
    server.sendHeader("Location", "/admin");
    server.send(303);
  });

  // --- Endpoint: dados JSON para o painel admin ---
  server.on("/api/devices", HTTP_GET, [&server, admin_username, admin_password]() {
    if (!server.authenticate(admin_username, admin_password)) {
      return server.requestAuthentication();
    }
    history.updateConnectedClients();

    String json = "[";
    for (int i = 0; i < history.getDeviceCount(); i++) {
      DeviceInfo& d = history.getDevice(i);
      if (i > 0) json += ",";
      json += "{";
      json += "\"mac\":\"" + d.mac + "\",";
      json += "\"ip\":\"" + d.ip + "\",";
      json += "\"firstSeen\":\"" + d.firstSeen + "\",";
      json += "\"lastSeen\":\"" + d.lastSeen + "\",";
      json += "\"isNew\":" + String(d.isNew ? "true" : "false");
      json += "}";
    }
    json += "]";
    server.send(200, "application/json", json);
  });

  // ---------------------------------------------------------------------------
  // URLs de detecção de portal cativo (Android, Chrome OS, Windows, iOS, macOS)
  // ---------------------------------------------------------------------------

  // Android / Chrome OS
  server.on("/generate_204", HTTP_GET, [&server]() {
    server.sendHeader("Location", "http://8.8.8.8/");
    server.send(302, "text/plain", "");
  });

  // Windows NCSI
  server.on("/ncsi.txt", HTTP_GET, [&server]() {
    server.sendHeader("Location", "http://8.8.8.8/");
    server.send(302, "text/plain", "");
  });

  // Windows (algumas versões)
  server.on("/success.html", HTTP_GET, [&server]() {
    server.sendHeader("Location", "http://8.8.8.8/");
    server.send(302, "text/plain", "");
  });

  // iOS / macOS hotspot detection
  server.on("/hotspot-detect.html", HTTP_GET, [&server]() {
    server.sendHeader("Location", "http://8.8.8.8/");
    server.send(302, "text/plain", "");
  });

  // iOS / macOS (alternativo)
  server.on("/library/test/success.html", HTTP_GET, [&server]() {
    server.sendHeader("Location", "http://8.8.8.8/");
    server.send(302, "text/plain", "");
  });

  // Firefox
  server.on("/canonical.html", HTTP_GET, [&server]() {
    server.sendHeader("Location", "http://8.8.8.8/");
    server.send(302, "text/plain", "");
  });

  // ---------------------------------------------------------------------------
  // Rota padrão: redireciona qualquer URL desconhecida para a página principal
  // ---------------------------------------------------------------------------
  server.onNotFound([&server]() {
    server.sendHeader("Location", "http://8.8.8.8/");
    server.send(302, "text/plain", "");
  });

  Serial.println("[WebServer] Rotas configuradas.");
}
