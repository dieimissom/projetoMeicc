#include "history_system.h"

// Caminho do arquivo de histórico no SPIFFS
#define HISTORY_FILE "/history.txt"

History::History() : deviceCount(0) {}

// ---------------------------------------------------------------------------
// Persistência
// ---------------------------------------------------------------------------

void History::load() {
  if (!SPIFFS.exists(HISTORY_FILE)) {
    Serial.println("[History] Nenhum histórico salvo encontrado.");
    return;
  }

  File f = SPIFFS.open(HISTORY_FILE, "r");
  if (!f) {
    Serial.println("[History] Erro ao abrir arquivo de histórico.");
    return;
  }

  deviceCount = 0;
  while (f.available() && deviceCount < MAX_DEVICES) {
    String line = f.readStringUntil('\n');
    line.trim();
    if (line.length() == 0) continue;

    // Formato: mac|ip|firstSeen|lastSeen
    int sep1 = line.indexOf('|');
    int sep2 = line.indexOf('|', sep1 + 1);
    int sep3 = line.indexOf('|', sep2 + 1);

    if (sep1 < 0 || sep2 < 0 || sep3 < 0) continue;

    devices[deviceCount].mac       = line.substring(0, sep1);
    devices[deviceCount].ip        = line.substring(sep1 + 1, sep2);
    devices[deviceCount].firstSeen = line.substring(sep2 + 1, sep3);
    devices[deviceCount].lastSeen  = line.substring(sep3 + 1);
    devices[deviceCount].isNew     = false;
    deviceCount++;
  }

  f.close();
  Serial.printf("[History] %d dispositivo(s) carregado(s).\n", deviceCount);
}

void History::save() {
  File f = SPIFFS.open(HISTORY_FILE, "w");
  if (!f) {
    Serial.println("[History] Erro ao salvar histórico.");
    return;
  }

  for (int i = 0; i < deviceCount; i++) {
    f.print(devices[i].mac);
    f.print('|');
    f.print(devices[i].ip);
    f.print('|');
    f.print(devices[i].firstSeen);
    f.print('|');
    f.println(devices[i].lastSeen);
  }

  f.close();
  Serial.println("[History] Histórico salvo.");
}

// ---------------------------------------------------------------------------
// Atualização de clientes
// ---------------------------------------------------------------------------

void History::updateConnectedClients() {
  struct station_info* stationList = wifi_softap_get_station_info();
  struct station_info* station = stationList;

  while (station != nullptr) {
    // Formatar MAC
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
             station->bssid[0], station->bssid[1], station->bssid[2],
             station->bssid[3], station->bssid[4], station->bssid[5]);

    // Formatar IP
    IPAddress ip((&station->ip)->addr);
    String ipStr = ip.toString();

    addOrUpdate(String(macStr), ipStr);
    station = STAILQ_NEXT(station, next);
  }

  wifi_softap_free_station_info();
  save();
}

// ---------------------------------------------------------------------------
// Acesso aos dados
// ---------------------------------------------------------------------------

int History::getDeviceCount() {
  return deviceCount;
}

DeviceInfo& History::getDevice(int index) {
  return devices[index];
}

// ---------------------------------------------------------------------------
// Geração de HTML
// ---------------------------------------------------------------------------

String History::generateHistoryPage() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>Histórico de Dispositivos</title>";
  html += "<style>";
  html += "body{font-family:Arial,sans-serif;background:#1a1a2e;color:#eee;margin:0;padding:20px;}";
  html += "h1{color:#e94560;text-align:center;}";
  html += "table{width:100%;border-collapse:collapse;margin-top:20px;}";
  html += "th{background:#16213e;color:#e94560;padding:10px;text-align:left;}";
  html += "td{padding:8px 10px;border-bottom:1px solid #333;}";
  html += "tr:hover{background:#0f3460;}";
  html += ".badge-new{background:#e94560;color:#fff;border-radius:4px;padding:2px 6px;font-size:0.75em;}";
  html += "</style></head><body>";
  html += "<h1>Dispositivos Conectados</h1>";
  html += "<p style='text-align:center'>Total: <strong>" + String(deviceCount) + "</strong></p>";
  html += "<table><tr><th>#</th><th>MAC</th><th>IP</th><th>Primeira Vez</th><th>Última Vez</th><th>Status</th></tr>";

  for (int i = 0; i < deviceCount; i++) {
    html += "<tr>";
    html += "<td>" + String(i + 1) + "</td>";
    html += "<td>" + devices[i].mac + "</td>";
    html += "<td>" + devices[i].ip + "</td>";
    html += "<td>" + devices[i].firstSeen + "</td>";
    html += "<td>" + devices[i].lastSeen + "</td>";
    html += "<td>" + String(devices[i].isNew ? "<span class='badge-new'>Novo</span>" : "Conhecido") + "</td>";
    html += "</tr>";
  }

  html += "</table></body></html>";
  return html;
}

// ---------------------------------------------------------------------------
// Limpeza
// ---------------------------------------------------------------------------

void History::clear() {
  deviceCount = 0;
  if (SPIFFS.exists(HISTORY_FILE)) {
    SPIFFS.remove(HISTORY_FILE);
  }
  Serial.println("[History] Histórico limpo.");
}

// ---------------------------------------------------------------------------
// Helpers privados
// ---------------------------------------------------------------------------

int History::findByMac(const String& mac) {
  for (int i = 0; i < deviceCount; i++) {
    if (devices[i].mac.equalsIgnoreCase(mac)) return i;
  }
  return -1;
}

void History::addOrUpdate(const String& mac, const String& ip) {
  int idx = findByMac(mac);
  String ts = getTimestamp();

  if (idx >= 0) {
    devices[idx].ip       = ip;
    devices[idx].lastSeen = ts;
    devices[idx].isNew    = false;
  } else if (deviceCount < MAX_DEVICES) {
    devices[deviceCount].mac       = mac;
    devices[deviceCount].ip        = ip;
    devices[deviceCount].firstSeen = ts;
    devices[deviceCount].lastSeen  = ts;
    devices[deviceCount].isNew     = true;
    deviceCount++;
    Serial.printf("[History] Novo dispositivo: %s (%s)\n", mac.c_str(), ip.c_str());
  }
}

String History::getTimestamp() {
  unsigned long ms = millis();
  unsigned long s  = ms / 1000;
  unsigned long m  = s / 60;
  unsigned long h  = m / 60;

  char buf[16];
  snprintf(buf, sizeof(buf), "%02lu:%02lu:%02lu", h % 24, m % 60, s % 60);
  return String(buf);
}
