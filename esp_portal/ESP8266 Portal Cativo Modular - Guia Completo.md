ESP8266 Portal Cativo Modular
Introdução

Este projeto implementa um portal cativo educativo utilizando um ESP8266. A ideia principal é demonstrar como funcionam redes que exigem login antes de liberar acesso à internet, algo comum em aeroportos, hotéis ou redes públicas.

Para isso, o sistema utiliza o SPIFFS (SPI Flash File System) para armazenar e servir páginas HTML diretamente da memória flash do microcontrolador. Dessa forma, o ESP8266 consegue hospedar páginas web simples sem depender de um servidor externo.

O projeto foi organizado de forma modular, separando as principais responsabilidades em diferentes arquivos. Isso ajuda bastante na organização do código e facilita tanto a manutenção quanto futuras melhorias.

Os módulos principais são:

Servidor web (web_server.h / web_server.cpp)

Sistema de histórico de dispositivos (history_system.h / history_system.cpp)

Arquivo principal do projeto (esp_portal.ino)

Essa divisão deixa o projeto mais fácil de entender e modificar.

Estrutura do Projeto
projeto_esp8266_modular/
├── esp_portal.ino          # Arquivo principal do Arduino
├── web_server.h            # Cabeçalho do servidor web
├── web_server.cpp          # Implementação do servidor web
├── history_system.h        # Cabeçalho do sistema de histórico
├── history_system.cpp      # Implementação do sistema de histórico
├── data/                   # Pasta com arquivos do SPIFFS
│   ├── index.html          # Página principal
│   ├── aviso.html          # Página de aviso
│   └── admin.html          # Painel administrativo
└── README.md               # Documentação do projeto
Principais Características

Algumas funcionalidades importantes do projeto:

Arquitetura modular para facilitar manutenção e organização

Portal cativo otimizado para aumentar a chance de aparecer a notificação de login nos dispositivos

Uso de SPIFFS para armazenar páginas web diretamente na memória do ESP8266

Monitoramento de dispositivos conectados

Interface web responsiva, funcionando tanto em celular quanto em computador

Área administrativa protegida

Configuração de Rede

Uma curiosidade do projeto é a configuração de IP utilizada.

IPAddress local_IP(8, 8, 8, 8);
IPAddress gateway(8, 8, 8, 8);
IPAddress subnet(255, 255, 255, 0);

Esse endereço foi escolhido porque 8.8.8.8 é um servidor DNS público muito conhecido (Google DNS).

Muitos sistemas operacionais usam esse endereço para testar se existe conexão com a internet. Quando o ESP8266 intercepta essas requisições, o sistema entende que existe um portal cativo, exibindo a notificação de login na rede Wi-Fi.

Requisitos de Hardware

Para montar o projeto, você vai precisar de:

ESP8266 (NodeMCU, Wemos D1 Mini ou equivalente)

Memória flash de pelo menos 4MB

Conectividade Wi-Fi (já integrada no ESP8266)

Requisitos de Software

Arduino IDE 1.8.19 ou superior
(ou Arduino IDE 2.x)

Pacote de placas ESP8266 versão 3.0 ou superior

Bibliotecas utilizadas:

ESP8266WiFi.h

ESP8266WebServer.h

DNSServer.h

FS.h

SPIFFS.h

Essas bibliotecas normalmente já vêm incluídas com o pacote ESP8266.

Upload de Arquivos para o SPIFFS

Para enviar os arquivos da pasta data para o ESP8266 é necessário instalar o plugin:

ESP8266 LittleFS Data Upload

Passos básicos:

Baixe o plugin no repositório oficial

Extraia na pasta tools do Arduino

Reinicie a IDE

A opção aparecerá em:

Ferramentas → ESP8266 LittleFS Data Upload
Instalação do Projeto
1. Preparar o ambiente

Instale o pacote ESP8266 na Arduino IDE

Configure a placa correta

Instale o plugin de upload SPIFFS/LittleFS

2. Organizar os arquivos

Crie uma pasta para o projeto

Coloque os arquivos .ino, .h e .cpp dentro dela

Crie uma pasta chamada data

Coloque os arquivos HTML dentro dessa pasta

3. Configuração da placa

Na Arduino IDE:

CPU Frequency: 80 MHz
Flash Size: 4MB (FS:2MB OTA:~1019KB)
Upload Speed: 115200
4. Upload do firmware

Abra esp_portal.ino

Compile

Envie para o ESP8266

Observe o Monitor Serial para verificar se tudo iniciou corretamente

5. Upload das páginas

Depois do firmware:

Ferramentas → ESP8266 LittleFS Data Upload

Isso enviará os arquivos HTML para o SPIFFS.

Arquitetura do Sistema
Arquivo Principal (esp_portal.ino)

Esse arquivo inicializa todo o sistema:

Inicialização do Serial

Montagem do SPIFFS

Criação da rede Wi-Fi

Inicialização do servidor DNS

Inicialização do servidor web

Loop principal de funcionamento

Servidor Web (web_server)

Esse módulo cuida de toda a parte HTTP:

Definição das rotas

Servir páginas HTML

Redirecionamento para o portal cativo

Autenticação do painel administrativo

Sistema de Histórico (history_system)

Responsável por registrar informações sobre os dispositivos conectados:

Identificação de novos dispositivos

Registro de acessos

Geração da página de histórico

Atualização contínua da lista de clientes

Funcionamento do Portal Cativo

Para que o portal seja detectado automaticamente, o sistema utiliza algumas estratégias:

DNS que redireciona todas as consultas

IP específico para interceptar verificações de conectividade

Redirecionamento automático de páginas

URLs de Detecção

Alguns sistemas operacionais verificam URLs específicas para descobrir se existe um portal cativo.

O projeto responde a URLs como:

/generate_204   (Android)
/success.html   (Windows)
/ncsi.txt       (Windows)

Todas acabam sendo redirecionadas para a página principal.

Fluxo de Funcionamento

O usuário conecta na rede EscolaRS_WiFi

O dispositivo tenta verificar se existe internet

O DNS redireciona a requisição para o ESP8266

O portal cativo é detectado

O sistema mostra a notificação "Fazer login na rede Wi-Fi"

Páginas do Sistema
index.html

Simula uma página de login com:

design inspirado no padrão gov.br

layout responsivo

formulário interativo

aviso.html

Página educativa explicando os riscos de redes públicas e boas práticas de segurança.

admin.html

Painel administrativo com:

estatísticas de acessos

lista de dispositivos conectados

interface moderna

Personalização
Alterar credenciais do admin

No arquivo principal:

const char* admin_username = "seu_usuario";
const char* admin_password = "sua_senha";
Alterar nome da rede Wi-Fi
WiFi.softAP("Seu_Nome_WiFi", "");
Solução de Problemas
SPIFFS não monta

verifique se o upload foi feito

confirme o tamanho da flash

verifique mensagens no Serial

Portal não aparece

esqueça a rede Wi-Fi no dispositivo

desligue dados móveis

reconecte novamente

Páginas não carregam

confirme se estão na pasta data

faça novamente o upload SPIFFS

Considerações de Segurança
Uso Educacional

Este projeto foi criado apenas para fins educacionais, com o objetivo de demonstrar como portais cativos funcionam e como redes públicas podem representar riscos.

Ele não deve ser usado para coletar dados reais ou realizar qualquer atividade maliciosa.

Conclusão

Esse projeto demonstra como um ESP8266 pode ser usado para criar um portal cativo funcional, utilizando uma arquitetura organizada e modular.

A divisão do código em módulos facilita bastante o desenvolvimento e permite que novas funcionalidades sejam adicionadas com facilidade. Além disso, o uso do SPIFFS torna possível hospedar páginas web diretamente no microcontrolador, sem depender de servidores externos.
