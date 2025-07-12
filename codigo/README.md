## Como Usar o Código

Este código foi desenvolvido para rodar no **Arduino IDE** com o ESP32 e o sensor radar **LD2410**. Ele realiza a contagem de pessoas baseado na detecção de movimento e envia os dados para a plataforma Adafruit IO via MQTT.

### Passos para configurar e rodar

1. **Preparação do ambiente**  
   - Instale o [Arduino IDE](https://www.arduino.cc/en/software) (versão recomendada 1.8.13 ou superior)  
   - Adicione a placa **ESP32** no Arduino IDE seguindo o tutorial oficial:  
     [Como adicionar suporte ao ESP32 no Arduino IDE](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/)

2. **Bibliotecas necessárias**  
   Instale as seguintes bibliotecas via Gerenciador de Bibliotecas do Arduino IDE:  
   - `ld2410` (sensor radar)  
   - `WiFi` (já incluída na instalação do ESP32)  
   - `Adafruit MQTT Library` e `Adafruit MQTT Client`

3. **Configurar as credenciais**  
   No código, altere as seguintes definições para o seu ambiente:  
   ```cpp
   #define WIFI_SSID    "seu_ssid_wifi"
   #define WIFI_PASS    "sua_senha_wifi"

   #define AIO_USERNAME    "seu_usuario_adafruit_io"
   #define AIO_KEY         "sua_chave_adafruit_io"

4. **Conectar hardware**  
   - Conecte o sensor LD2410 ao ESP32 usando os pinos RX = 16 e TX = 17 (pode ajustar no código se usar outros)  
   - Ligue o ESP32 ao computador via USB

5. **Upload do código**  
   - Abra o Arduino IDE  
   - Cole o código fonte  
   - Selecione a placa ESP32 correta (ex: ESP32 Dev Module)  
   - Escolha a porta serial correta  
   - Clique em “Upload” para gravar o código no ESP32

6. **Monitorar a execução**  
   - Abra o Monitor Serial do Arduino IDE (baud rate 115200)  
   - Você verá as mensagens de conexão, distâncias medidas e a contagem de pessoas  
   - A cada 5 segundos, o número de pessoas será enviado para o feed MQTT no Adafruit IO

7. **Visualizar dados no Adafruit IO**  
   - Acesse sua conta em [https://io.adafruit.com](https://io.adafruit.com)  
   - Crie um feed chamado `People Count` ou use o feed padrão no código  
   - Você poderá acompanhar o fluxo de pessoas em tempo real através do dashboard

