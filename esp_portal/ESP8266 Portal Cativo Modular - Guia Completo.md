# ESP8266 Portal Cativo Modular - Guia Completo

## Introdução

Este projeto implementa um portal cativo educativo para ESP8266 utilizando uma arquitetura modular com separação de responsabilidades. O sistema utiliza SPIFFS (SPI Flash File System) para servir páginas HTML estáticas e foi projetado especificamente para maximizar a exibição da notificação "Fazer login na rede Wi-Fi" em dispositivos clientes.

A arquitetura modular separa as funcionalidades em diferentes módulos: servidor web (`web_server.h/cpp`), sistema de histórico (`history_system.h/cpp`) e arquivo principal (`esp_portal.ino`). Esta abordagem facilita a manutenção, extensão e depuração do código.

## Estrutura do Projeto

```
projeto_esp8266_modular/
├── esp_portal.ino          # Arquivo principal do Arduino
├── web_server.h            # Cabeçalho do servidor web
├── web_server.cpp          # Implementação do servidor web
├── history_system.h        # Cabeçalho do sistema de histórico
├── history_system.cpp      # Implementação do sistema de histórico
├── data/                   # Pasta para arquivos SPIFFS
│   ├── index.html         # Página principal do portal
│   ├── aviso.html         # Página de aviso de segurança
│   └── admin.html         # Painel administrativo
└── README.md              # Este arquivo
```

## Características Principais

- **Arquitetura Modular**: Código organizado em módulos específicos para facilitar manutenção
- **Portal Cativo Otimizado**: Configuração específica para maximizar a detecção de portal cativo
- **Sistema SPIFFS**: Armazenamento de páginas web no sistema de arquivos flash
- **Monitoramento de Dispositivos**: Rastreamento em tempo real de dispositivos conectados
- **Interface Responsiva**: Páginas otimizadas para dispositivos móveis e desktop
- **Autenticação Admin**: Acesso protegido ao painel administrativo

## Configuração de Rede Otimizada

O projeto utiliza a configuração de IP `8.8.8.8` tanto para o gateway quanto para o IP local do ESP8266. Esta configuração específica foi escolhida para maximizar a probabilidade de acionamento da notificação "Fazer login na rede Wi-Fi" em dispositivos clientes.

```cpp
IPAddress local_IP(8, 8, 8, 8);
IPAddress gateway(8, 8, 8, 8);
IPAddress subnet(255, 255, 255, 0);
```

Esta configuração funciona porque muitos sistemas operacionais tentam acessar servidores DNS públicos conhecidos (como 8.8.8.8 do Google) para verificar a conectividade com a internet. Quando essas requisições são interceptadas pelo ESP8266, o sistema operacional detecta a presença de um portal cativo.

## Requisitos de Hardware

- **Microcontrolador**: ESP8266 (NodeMCU, Wemos D1 Mini, ou similar)
- **Memória Flash**: Mínimo 4MB para suporte adequado ao SPIFFS
- **Conectividade**: Wi-Fi integrado do ESP8266

## Requisitos de Software

### IDE Arduino
- **Versão**: Arduino IDE 1.8.19 ou superior (ou Arduino IDE 2.0+)
- **Placa ESP8266**: Pacote de placas ESP8266 versão 3.0.0 ou superior

### Bibliotecas Necessárias
As seguintes bibliotecas são necessárias e geralmente já estão incluídas no pacote ESP8266:

- `ESP8266WiFi.h` - Gerenciamento de conectividade Wi-Fi
- `ESP8266WebServer.h` - Servidor HTTP
- `DNSServer.h` - Servidor DNS para redirecionamento
- `FS.h` - Sistema de arquivos base
- `SPIFFS.h` - Sistema de arquivos SPIFFS específico

### Plugin para Upload de Dados
Para fazer upload dos arquivos da pasta `data` para o SPIFFS:

1. Baixe o plugin ESP8266 LittleFS Data Upload do repositório oficial
2. Extraia na pasta `tools` do diretório Arduino
3. Reinicie a IDE Arduino
4. O menu "Ferramentas > ESP8266 LittleFS Data Upload" deve aparecer

## Instalação e Configuração

### Passo 1: Preparação do Ambiente

1. Instale o pacote de suporte ESP8266 na IDE Arduino
2. Configure a placa ESP8266 apropriada
3. Instale o plugin ESP8266 LittleFS Data Upload

### Passo 2: Preparação dos Arquivos

1. Crie uma nova pasta para o projeto
2. Copie todos os arquivos do projeto (.ino, .h, .cpp) para a pasta
3. Crie uma subpasta chamada `data`
4. Copie os arquivos HTML para a pasta `data`

### Passo 3: Configuração da Placa

1. Selecione sua placa ESP8266 em **Ferramentas > Placa**
2. Configure os parâmetros:
   - **CPU Frequency**: 80 MHz
   - **Flash Size**: 4MB (FS:2MB OTA:~1019KB)
   - **Upload Speed**: 115200

### Passo 4: Upload do Código

1. Abra o arquivo `esp_portal.ino` na IDE Arduino
2. Compile e faça upload do código para o ESP8266
3. Monitore o Serial para verificar a inicialização

### Passo 5: Upload dos Arquivos SPIFFS

1. Use **Ferramentas > ESP8266 LittleFS Data Upload**
2. Aguarde o processo de upload dos arquivos da pasta `data`
3. Verifique no Monitor Serial se o SPIFFS foi montado com sucesso

## Arquitetura do Sistema

### Arquivo Principal (esp_portal.ino)

O arquivo principal é responsável pela inicialização do sistema e coordenação dos módulos. Suas principais funções incluem:

- Inicialização do sistema serial e SPIFFS
- Configuração do ponto de acesso Wi-Fi
- Inicialização do servidor DNS
- Configuração do servidor web através do módulo `web_server`
- Loop principal para processamento de requisições

### Módulo Servidor Web (web_server.h/cpp)

Este módulo encapsula toda a lógica do servidor HTTP, incluindo:

- Configuração de rotas e manipuladores
- Servir arquivos do SPIFFS
- Redirecionamento para detecção de portal cativo
- Autenticação para páginas administrativas
- Manipulação de URLs específicas de detecção de portal cativo

### Módulo Sistema de Histórico (history_system.h/cpp)

Responsável pelo monitoramento e registro de dispositivos conectados:

- Estrutura de dados para informações de dispositivos
- Detecção de novos dispositivos vs. dispositivos conhecidos
- Geração de páginas HTML para exibição do histórico
- Atualização contínua da lista de clientes conectados

## Funcionamento do Portal Cativo

### Detecção de Portal Cativo

O sistema implementa várias estratégias para maximizar a detecção de portal cativo:

1. **Configuração de IP Específica**: Uso do IP 8.8.8.8 para interceptar tentativas de conectividade
2. **Servidor DNS Universal**: Redirecionamento de todas as consultas DNS para o ESP8266
3. **URLs de Detecção**: Manipuladores específicos para URLs comuns de detecção de portal cativo
4. **Redirecionamento Agressivo**: Qualquer requisição não encontrada é redirecionada para a página principal

### URLs de Detecção Implementadas

O sistema responde especificamente às seguintes URLs utilizadas por diferentes sistemas operacionais para detectar portais cativos:

- `/generate_204` - Usado por Android e Chrome OS
- `/success.html` - Usado por algumas versões do Windows
- `/ncsi.txt` - Network Connectivity Status Indicator do Windows

Todas essas URLs são redirecionadas para a página principal do portal, forçando a detecção.

### Fluxo de Redirecionamento

1. Dispositivo conecta à rede "EscolaRS_WiFi"
2. Sistema operacional tenta verificar conectividade com a internet
3. Servidor DNS do ESP8266 redireciona todas as consultas para 8.8.8.8
4. Requisições HTTP são interceptadas e redirecionadas para a página de login
5. Sistema operacional detecta o portal cativo e exibe a notificação

## Páginas Web

### index.html - Página Principal

A página principal simula um portal de login governamental brasileiro com:

- Design responsivo adaptável a diferentes dispositivos
- Tema visual baseado no padrão gov.br
- Formulário de login interativo
- JavaScript para alternância entre telas

### aviso.html - Página de Aviso

Página educativa sobre segurança em redes Wi-Fi com:

- Design impactante com animações CSS
- Conteúdo educativo sobre riscos de segurança
- Links para navegação entre seções
- Otimização para dispositivos móveis

### admin.html - Painel Administrativo

Interface administrativa moderna com:

- Dashboard com estatísticas em tempo real
- Tabela de dispositivos conectados
- Design moderno com gradientes e animações
- Funcionalidades de atualização automática

## Personalização e Extensões

### Modificando Credenciais de Admin

Para alterar as credenciais de administrador, modifique as variáveis no arquivo principal:

```cpp
const char* admin_username = "seu_usuario";
const char* admin_password = "sua_senha";
```

### Adicionando Novas Rotas

Para adicionar novas rotas ao servidor web, edite a função `setupWebServer` em `web_server.cpp`:

```cpp
server.on("/nova_rota", HTTP_GET, [&]() {
    serveSpiffsFile(server, "/nova_pagina.html", "text/html");
});
```

### Modificando o Nome da Rede

Para alterar o nome da rede Wi-Fi, modifique a chamada `WiFi.softAP` no arquivo principal:

```cpp
WiFi.softAP("Seu_Nome_WiFi", ""); // Sem senha para portal cativo
```

## Solução de Problemas

### SPIFFS Não Monta

Se o SPIFFS não montar corretamente:

1. Verifique se o upload dos dados foi realizado
2. Confirme a configuração de Flash Size
3. Monitore mensagens de erro no Serial

### Portal Cativo Não Detectado

Se a notificação não aparecer:

1. Esqueça a rede Wi-Fi no dispositivo cliente
2. Desative dados móveis temporariamente
3. Reinicie o dispositivo cliente
4. Teste com diferentes dispositivos e sistemas operacionais

### Páginas Não Carregam

Se as páginas não carregarem:

1. Verifique se os arquivos estão na pasta `data`
2. Confirme se o upload SPIFFS foi bem-sucedido
3. Monitore o Serial para mensagens de erro

## Considerações de Segurança

### Uso Educativo

Este projeto foi desenvolvido exclusivamente para fins educativos e demonstração de conceitos de segurança. Não deve ser utilizado para interceptar dados reais ou realizar atividades maliciosas.

### Limitações

- O sistema não implementa criptografia real
- Dados transmitidos não são protegidos
- Não há autenticação real de usuários
- O objetivo é demonstrar vulnerabilidades, não explorá-las

### Boas Práticas

- Sempre informe aos usuários que se trata de uma demonstração
- Use apenas em ambientes controlados e com consentimento
- Não colete dados pessoais reais
- Mantenha o foco no aspecto educativo

## Conclusão

Este projeto oferece uma implementação robusta e modular de um portal cativo educativo para ESP8266. A arquitetura modular facilita a manutenção e extensão do código, enquanto a configuração otimizada maximiza as chances de detecção do portal cativo pelos dispositivos clientes.

A separação em módulos específicos permite que desenvolvedores modifiquem ou estendam funcionalidades específicas sem afetar outras partes do sistema. O uso do SPIFFS proporciona flexibilidade na gestão de conteúdo web, permitindo atualizações rápidas das páginas sem recompilação do firmware.

