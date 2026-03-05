#ifndef HISTORY_SYSTEM_H
#define HISTORY_SYSTEM_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FS.h>

// Estrutura para armazenar informações de um dispositivo conectado
struct DeviceInfo {
  String mac;
  String ip;
  String firstSeen;
  String lastSeen;
  bool isNew;
};

// Número máximo de dispositivos no histórico
#define MAX_DEVICES 50

class History {
public:
  History();

  // Carrega o histórico salvo do SPIFFS
  void load();

  // Salva o histórico atual no SPIFFS
  void save();

  // Atualiza a lista de clientes conectados ao AP
  void updateConnectedClients();

  // Retorna o número de dispositivos no histórico
  int getDeviceCount();

  // Retorna uma referência ao dispositivo pelo índice
  DeviceInfo& getDevice(int index);

  // Gera uma página HTML com o histórico de dispositivos
  String generateHistoryPage();

  // Limpa todo o histórico
  void clear();

private:
  DeviceInfo devices[MAX_DEVICES];
  int deviceCount;

  // Verifica se um MAC já está no histórico; retorna o índice ou -1
  int findByMac(const String& mac);

  // Adiciona ou atualiza um dispositivo no histórico
  void addOrUpdate(const String& mac, const String& ip);

  // Retorna a hora atual formatada como string (milissegundos desde boot)
  String getTimestamp();
};

#endif // HISTORY_SYSTEM_H
