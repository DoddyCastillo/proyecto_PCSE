# Proyecto: Sistema Integrado de Monitoreo de Entorno y Movimiento

## Propósito del Proyecto

Este proyecto tiene como objetivo el desarrollo de un sistema embebido capaz de monitorear condiciones ambientales y movimiento en tiempo real, integrando sensores inerciales y barométricos con interfaces de visualización local (LCD) y remota (puerto serie). Su aplicación puede extenderse a sistemas de navegación básica, domótica, o monitoreo ambiental portátil, siendo una excelente base para proyectos de sistemas embebidos con STM32.

## Descripción del Funcionamiento

- El **sensor MPU-6050** se encarga de capturar datos de aceleración y giroscopio (movimiento), los cuales se visualizan directamente en una pantalla LCD.
- El **sensor BMP280** mide presión atmosférica y temperatura, y estos datos se transmiten por UART al puerto serie, permitiendo el monitoreo remoto desde un terminal serial en PC.
- El sistema está basado en un **STM32F446RE**, aprovechando sus capacidades de procesamiento y comunicación para integrar los dispositivos de manera eficiente.

## Componentes Utilizados

| Componente     | Descripción                                                                 |
|----------------|-----------------------------------------------------------------------------|
| STM32F446RE    | Microcontrolador ARM Cortex-M4 con FPU, ideal para tareas de control y DSP.|
| MPU-6050       | Sensor IMU (Inertial Measurement Unit) de 6 ejes: acelerómetro y giroscopio.|
| BMP280         | Sensor barométrico digital que mide presión y temperatura.                 |
| LCD 20x4       | Pantalla de cristal líquido, interfaz paralela o I2C para visualizar datos. |
| UART/USB       | Comunicación serial con PC para mostrar datos del BMP280 en tiempo real.   |
| Nucleo-F446RE  | Placa de desarrollo con depurador ST-Link integrado y headers compatibles. |

## Objetivos Técnicos

- Integración de periféricos I2C para los sensores.
- Configuración del LCD para mostrar datos dinámicos.
- Comunicación serial UART con PC.
- Aplicación de conceptos de temporización y adquisición periódica de datos.
- Uso de HAL o LL para configuración eficiente del microcontrolador.

## Posibles Extensiones

- Registro de datos en memoria externa o tarjeta SD.
- Incorporación de GPS para geolocalización.
- Transmisión inalámbrica de datos (Bluetooth/LoRa/WiFi).
- Interfaz gráfica más avanzada (TFT o pantalla OLED).
