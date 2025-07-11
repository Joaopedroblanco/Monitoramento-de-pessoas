# Sistema de Contagem de Pessoas com Radar LD2410 e ESP32

## Descrição

Este projeto implementa um sistema embarcado para **contagem de pessoas**, utilizando o sensor de radar **LD2410** acoplado a um microcontrolador **ESP32**. O sistema detecta movimentos e variações de distância para identificar entradas e saídas em um ambiente.

Os dados são enviados via protocolo **MQTT** para a plataforma **Adafruit IO**, permitindo monitoramento remoto em tempo real, sem a necessidade do uso de câmeras.

## Funcionalidades

- Detecção de presença e movimento através do radar LD2410  
- Contagem precisa de pessoas que entram e saem do ambiente  
- Comunicação com a nuvem via MQTT  
- Visualização dos dados na plataforma Adafruit IO  
- Baixo custo e fácil implementação  
- Aplicável em laboratórios, salas de aula, banheiros, corredores e outros ambientes

## Componentes Utilizados

- [ESP32](https://www.espressif.com/en/products/socs/esp32)  
- [Sensor Radar LD2410](https://www.example.com/ld2410)  
- Plataforma Adafruit IO para visualização dos dados  
