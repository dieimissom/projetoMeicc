#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include "history_system.h"

// Referência à instância global de History definida no .ino
extern History history;

/**
 * Configura todas as rotas e manipuladores do servidor web.
 *
 * @param server           Instância do ESP8266WebServer
 * @param admin_username   Nome de usuário para o painel administrativo
 * @param admin_password   Senha para o painel administrativo
 */
void setupWebServer(ESP8266WebServer& server,
                    const char* admin_username,
                    const char* admin_password);

/**
 * Serve um arquivo diretamente do SPIFFS.
 *
 * @param server    Instância do ESP8266WebServer
 * @param path      Caminho do arquivo no SPIFFS (ex.: "/index.html")
 * @param contentType Tipo MIME do arquivo (ex.: "text/html")
 */
void serveSpiffsFile(ESP8266WebServer& server,
                     const String& path,
                     const String& contentType);

#endif // WEB_SERVER_H
