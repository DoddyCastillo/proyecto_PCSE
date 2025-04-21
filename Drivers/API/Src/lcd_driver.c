/*
 * lcd_driver.h
 *
 *  Created on: Mar 30, 2025
 *      Author: doddy
 */


#include "lcd_driver.h"
#include "lcd_port.h"
#include "mpu6050_driver.h"
#include "string.h"
#include "stdio.h"

static I2C_LCD_Conf lcd_conf = {0};

static void LCD_Init();
static void LCD_SendData(uint8_t data);
static void LCD_SendCommand(uint8_t cmd);
static void LCD_SendNibble( uint8_t nibble, uint8_t mode);
static void FormatIntDecimal(char *buf, int32_t value, uint8_t decimal);
static void LCD_PrintLine(uint8_t row, char* text);

/**
 * @brief Envía un nibble (4 bits) al LCD a través de la interfaz I2C.
 *
 * Esta función está diseñada para trabajar con pantallas LCD que utilizan un expansor de bus I2C
 * (como el PCF8574) para comunicarse con el microcontrolador. El LCD opera en modo de 4 bits, por lo que
 * los datos deben enviarse en dos partes (nibbles altos y bajos). Esta función se encarga de enviar
 * un solo nibble, especificando si se trata de un comando o de datos.
 *
 * @param nibble El valor de 4 bits que se desea enviar (generalmente parte de un byte completo).
 * @param mode   Indica si el nibble es un comando o datos:
 *               - 0 para comandos (RS = 0)
 *               - LCD_RS para datos (RS = 1), siendo LCD_RS una máscara que activa el bit correspondiente.
 *
 * @details
 * 1. Combina el nibble con la retroiluminación (LCD_BACKLIGHT) y el modo (RS).
 * 2. Activa la señal de ENABLE en alto para latch del dato.
 * 3. Espera 5 ms para asegurar que el LCD procese el nibble (puede ser reducido dependiendo del LCD usado).
 * 4. Desactiva ENABLE para completar el ciclo de escritura.
 *
 * @note
 * - Esta función es utilizada internamente por funciones de más alto nivel que manejan el envío de bytes completos.
 * - Asegúrese que el periférico I2C esté correctamente inicializado antes de utilizar esta función.
 * - La constante MASK debe asegurar que solo los bits relacionados al nibble y líneas de control estén activos.
 */

static void LCD_SendNibble(uint8_t nibble, uint8_t mode)
{
	uint8_t data = (nibble & MASK) | LCD_BACKLIGHT | mode;
	LCD_PortI2C_WriteRegister(data | ENABLE);
    HAL_Delay(5);
    LCD_PortI2C_WriteRegister(data & ~ENABLE);
}

/**
 * @brief Envía un comando de 8 bits al LCD usando interfaz de 4 bits por I2C.
 *
 * Esta función divide un byte de comando en dos nibbles (alto y bajo) y los envía secuencialmente
 * al LCD usando la función auxiliar LCD_SendNibble. El comando puede incluir instrucciones como
 * limpiar la pantalla, mover el cursor, cambiar el modo de entrada, etc.
 *
 * @param cmd Comando de 8 bits a enviar al LCD.
 *
 * @details
 * 1. En el modo de 4 bits, los comandos se envían en dos partes:
 *    - Primero se envía el nibble más significativo (MSB).
 *    - Luego se envía el nibble menos significativo (LSB), desplazado 4 bits a la izquierda.
 * 2. Ambos nibbles se envían en modo "Instrucción/Comando" (RS = 0), indicado por `MODE_RS_IR`.
 * 3. Se realiza un retardo breve de 2 ms tras enviar el comando, para asegurar que el LCD tenga tiempo suficiente para procesarlo.
 *
 * @note
 * - Esta función debe utilizarse para inicializar o controlar el comportamiento del LCD.
 * - Asegúrese que el LCD esté correctamente inicializado en modo de 4 bits antes de utilizar esta función.
 */

static void LCD_SendCommand( uint8_t cmd)
{
	LCD_SendNibble( cmd, MODE_RS_IR);
	LCD_SendNibble((cmd << 4), MODE_RS_IR );
	HAL_Delay(2);
}

/**
 * @brief Envía un dato (carácter) de 8 bits al LCD.
 *
 * Esta función permite mostrar caracteres en el LCD, enviando un byte de datos que representa
 * un carácter ASCII o similar. El byte se divide en dos nibbles y se envía mediante la función
 * `LCD_SendNibble` en modo de datos (RS = 1).
 *
 * @param data Byte de datos a mostrar en el LCD (por ejemplo, un carácter ASCII).
 *
 * @details
 * 1. Se divide el byte en dos partes de 4 bits (nibble alto y nibble bajo).
 * 2. Cada nibble se envía individualmente, en modo de datos (`MODE_RS_DR`).
 * 3. El segundo nibble (parte baja) es enmascarado con `MASK` para evitar afectar otras líneas de control.
 * 4. Se aplica un pequeño retardo de 1 ms para garantizar que el LCD procese correctamente el carácter.
 *
 * @note
 * - Asegurarse que el LCD esté en modo 4 bits y correctamente inicializado antes de enviar datos.
 * - Esta función debe usarse para imprimir texto carácter por carácter.
 */

static void LCD_SendData(uint8_t data)
{
	LCD_SendNibble( data, MODE_RS_DR);
	LCD_SendNibble((data << 4) & MASK, MODE_RS_DR );
	HAL_Delay(1);
}

/**
 * @brief Formatea un número entero con parte decimal simulada y lo convierte a cadena.
 *
 * Esta función toma un valor entero (por ejemplo, un valor de ADC escalado) y lo convierte
 * en una representación decimal con punto flotante simulada, guardando el resultado como una cadena.
 * Es especialmente útil para entornos embebidos donde se evita el uso de `float` por eficiencia o limitaciones de hardware.
 *
 * @param buf      Puntero al buffer donde se almacenará la cadena formateada.
 * @param value    Valor entero con 2 cifras decimales implícitas (por ejemplo, 1234 representa 12.34).
 * @param decimals Número de cifras decimales deseadas (1 o 2).
 *
 * @details
 * - Divide el valor en parte entera (`ent`) y parte decimal (`dec`).
 * - Si el valor es negativo, antepone el signo "-" al resultado.
 * - Usa `sprintf` con formato `"%d.%02d"` para 2 decimales o `"%d.%d"` para 1 decimal.
 * - El valor decimal siempre se calcula como `value % 100`, ya que se asume que el número está escalado por 100.
 *
 * @example
 * ```c
 * char buffer[10];
 * FormatIntDecimal(buffer, -1234, 2); // buffer = "-12.34"
 * FormatIntDecimal(buffer, 567, 1);   // buffer = "5.6"
 * ```
 *
 * @note
 * - El buffer `buf` debe ser lo suficientemente grande para almacenar la cadena completa (recomendado al menos 10 bytes).
 * - No realiza redondeo, solo truncamiento de los decimales si `decimals == 1`.
 */
static void FormatIntDecimal(char *buf, int32_t value, uint8_t decimals)
{
	int abs_value = (value < 0) ? -value : value;
    int ent = abs_value / 100;
    int dec = abs_value % 100;

    if (value < 0) {
        if (decimals == 2) {
            sprintf(buf, "-%d.%02d", ent, dec);
        } else {
            sprintf(buf, "-%d.%d", ent, dec / 10);
        }
    } else {
        if (decimals == 2) {
            sprintf(buf, "%d.%02d", ent, dec);
        } else {
            sprintf(buf, "%d.%d", ent, dec / 10);
        }
    }
}

/**
 * @brief Imprime una línea de texto alineada a la izquierda en una fila específica del LCD.
 *
 * Esta función copia el texto indicado a un buffer temporal, lo ajusta al ancho total de la pantalla
 * rellenando con espacios si es necesario, y lo envía al LCD en la fila especificada.
 *
 * @param row   Número de fila donde se imprimirá el texto (por ejemplo, 0 o 1 para una pantalla 2x16).
 * @param text  Cadena de texto a imprimir. Si el texto es más largo que el número de columnas, se trunca.
 *
 * @details
 * 1. Se copia el texto a un buffer temporal `buf` cuyo tamaño es el número de columnas del LCD (`lcd_conf.I2C_LCD_nCol`) + 1 (para el `'\0'`).
 * 2. Si el texto es más largo que las columnas disponibles, se trunca a `lcd_conf.I2C_LCD_nCol` caracteres.
 *    (⚠️ El código tiene un límite fijo de 20, lo cual podría corregirse si `lcd_conf.I2C_LCD_nCol` es menor a 20).
 * 3. Se rellenan los espacios sobrantes con `' '` (espacio) para borrar caracteres anteriores si el texto nuevo es más corto.
 * 4. Se posiciona el cursor en el inicio de la fila con `LCD_SetCursor`.
 * 5. Finalmente, se imprime el texto formateado mediante `LCD_SendString`.
 *
 * @note
 * - Asegurate de que la función `LCD_SetCursor` esté correctamente implementada para posicionar por fila y columna.
 * - El uso de `strncpy` y padding manual asegura que no queden residuos visuales en la línea del LCD.
 *
 * @example
 * ```c
 * LCD_PrintLine(0, "Temperatura:");
 * LCD_PrintLine(1, "23.5 C");
 * ```
 */
static void LCD_PrintLine(uint8_t row, char* text)
{
	char buf [lcd_conf.I2C_LCD_nCol + 1];
	size_t len = strlen(text);

	if(len > lcd_conf.I2C_LCD_nCol) len = 20 ;
	strncpy(buf, text, len);

	for(size_t i = len; i < lcd_conf.I2C_LCD_nCol ; i++)
	{
		buf[i] = ' ';
	}

	buf[lcd_conf.I2C_LCD_nCol] = '\0';
    LCD_SetCursor(0, row);
    LCD_SendString(buf);
}

/**
 * @brief Envía una cadena de texto al LCD carácter por carácter.
 *
 * Esta función recorre una cadena de caracteres terminada en nulo (`\0`) y envía cada carácter
 * individualmente al LCD usando `LCD_SendData`. Es útil para imprimir mensajes en pantalla
 * de forma sencilla y legible.
 *
 * @param str Puntero a la cadena de texto (tipo `char*`) que se desea mostrar en el LCD.
 *
 * @details
 * 1. Utiliza un bucle `while` que recorre la cadena hasta encontrar el carácter nulo (`\0`).
 * 2. Cada carácter es casteado a `uint8_t` y enviado al LCD usando `LCD_SendData`, que lo
 *    convierte internamente en nibbles y lo envía por I2C.
 * 3. Se aplica un retardo final de 2 ms después de enviar toda la cadena, lo cual da tiempo
 *    al LCD para completar la visualización (especialmente útil en pantallas lentas o al mostrar texto completo).
 *
 * @note
 * - La cadena no debe exceder el número de columnas disponibles en el LCD si se quiere evitar texto partido o líneas solapadas.
 * - Asegurate que `LCD_SendData` y `LCD_SetCursor` estén correctamente implementadas para que el texto se muestre donde corresponde.
 *
 */

void LCD_SendString(char *str) {
    while(*str) {
    	LCD_SendData((uint8_t)(*str));
        str++;
    }

    HAL_Delay(2);
}

/**
 * @brief Limpia completamente el contenido del LCD.
 *
 * Envía el comando correspondiente para borrar toda la pantalla del LCD y colocar el cursor
 * en la posición inicial (fila 0, columna 0).
 *
 * @details
 * 1. El comando `LCD_CLEARDISPLAY` (generalmente `0x01`) es parte del conjunto de instrucciones del controlador HD44780.
 * 2. Este comando borra todos los caracteres del display y restablece la dirección del cursor.
 * 3. Se incluye un retardo de 2 ms posterior, ya que este comando es uno de los más lentos
 *    en ser procesados por el LCD y requiere tiempo adicional de espera.
 *
 * @note
 * - Esta función debería usarse con moderación, ya que limpiar la pantalla frecuentemente puede causar parpadeos visibles.
 * - Es recomendable usarla durante inicializaciones o transiciones importantes de interfaz.
 *
 */

void LCD_Clear() {
    LCD_SendCommand(LCD_CLEARDISPLAY);
    HAL_Delay(2);
}

/**
 * @brief Retorna el cursor a la posición inicial (fila 0, columna 0) sin borrar el contenido del LCD.
 *
 * Esta función envía el comando `LCD_RETURNHOME`, que reubica el cursor en la esquina superior izquierda
 * del display y también resetea el desplazamiento de pantalla (si se ha aplicado desplazamiento horizontal).
 *
 * @details
 * 1. El comando `LCD_RETURNHOME` (normalmente `0x02`) es parte del conjunto de instrucciones estándar
 *    del controlador HD44780 y similares.
 * 2. Este comando no borra el contenido del display, a diferencia de `LCD_Clear`.
 * 3. Se requiere un retardo de al menos 1.52 ms para permitir que el controlador lo procese correctamente.
 *    En esta implementación se da un margen de 2 ms.
 *
 * @note
 * - Utilizar esta función cuando se quiera reubicar el cursor o reiniciar el desplazamiento de texto en pantalla.
 * - Es útil en aplicaciones donde se quiere sobrescribir el contenido desde el inicio sin borrar toda la pantalla.
 *
 */

void LCD_Home(void)
{
    LCD_SendCommand(LCD_RETURNHOME);
    HAL_Delay(2);
}

/**
 * @brief Posiciona el cursor en una ubicación específica del LCD.
 *
 * Calcula y envía la dirección adecuada de DDRAM para colocar el cursor en la posición indicada
 * por `col` (columna) y `row` (fila), respetando los límites del display configurado.
 *
 * @param col Número de columna (desde 0 hasta `lcd_conf.I2C_LCD_nCol - 1`).
 * @param row Número de fila (desde 0 hasta `lcd_conf.I2C_LCD_nRow - 1`).
 *
 * @details
 * 1. El LCD organiza su memoria DDRAM con direcciones específicas para cada fila.
 *    Por ejemplo, para un LCD de 20x4, las direcciones típicas son:
 *    - Línea 0: 0x00
 *    - Línea 1: 0x40
 *    - Línea 2: 0x14
 *    - Línea 3: 0x54
 *    Estas se definen como constantes (`LCD_LINE_0`, etc.) y se almacenan en `row_offsets`.
 *
 * 2. Se limita el valor de `row` y `col` a los máximos configurados en `lcd_conf` para evitar errores.
 * 3. Luego se calcula la dirección en DDRAM sumando la columna al offset de la fila y se envía con el
 *    comando `LCD_SETDDRAMADDR`.
 *
 * @note
 * - Esta función es indispensable antes de imprimir texto en una posición específica.
 * - Requiere que el LCD esté inicializado correctamente en modo de 4 bits.
 *
 * @example
 * ```c
 * LCD_SetCursor(5, 1);          // Mueve el cursor a la columna 5 de la fila 1
 * LCD_SendString("Hola");      // Imprime el texto en esa posición
 * ```
 */

void LCD_SetCursor(uint8_t col, uint8_t row)
{
    const uint8_t row_offsets[] = {LCD_LINE_0, LCD_LINE_1, LCD_LINE_2, LCD_LINE_3};
    if(row >= lcd_conf.I2C_LCD_nRow) row = lcd_conf.I2C_LCD_nRow - 1;
    if(col >= lcd_conf.I2C_LCD_nCol) col = lcd_conf.I2C_LCD_nCol - 1;
    LCD_SendCommand(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

/**
 * @brief Imprime una cadena de texto en el LCD, limpiando previamente la pantalla.
 *
 * Esta función borra el contenido actual del LCD, posiciona el cursor al inicio
 * (posición 0,0) y luego imprime la cadena de texto proporcionada.
 *
 * @param str Puntero a la cadena de texto a mostrar.
 *
 * @details
 * 1. Se llama a `LCD_Clear()` para borrar todo el contenido de la pantalla.
 * 2. Luego, `LCD_Home()` posiciona el cursor en la primera columna de la primera fila.
 * 3. Finalmente, `LCD_SendString()` envía la cadena de texto carácter por carácter.
 *
 * @note
 * - Esta función es útil para mostrar mensajes completos que reemplazan el contenido anterior.
 * - Si la cadena es más larga que la capacidad del display, los caracteres extra pueden desplazarse fuera del área visible.
 * - Si necesitás mayor control sobre formato y ubicación, usá `LCD_SetCursor` y `LCD_PrintLine` en su lugar.
 *
 */

void LCD_Print(char *str)
{
    LCD_Clear();
    LCD_Home();
    LCD_SendString(str);
}

/**
 * @brief Inicializa el LCD en modo de 4 bits y configura sus parámetros básicos.
 *
 * Esta función implementa la secuencia de inicialización recomendada por el controlador HD44780
 * (o compatible) para configurar el LCD en modo de 4 bits, dos líneas y caracteres de 5x8 puntos.
 * Además, apaga la pantalla, limpia el contenido, configura el modo de entrada y luego enciende
 * el display sin cursor ni parpadeo.
 *
 * @details
 * 1. La secuencia de inicialización sigue los pasos descritos en la hoja de datos del HD44780.
 *    Primero se envían comandos en modo de 8 bits para forzar la configuración inicial.
 * 2. Luego se establece el modo de 4 bits, que permite reducir las líneas de datos necesarias.
 * 3. Se configura el número de líneas y el formato de los caracteres.
 * 4. Se apaga el display, se limpia, se configura el modo de entrada y finalmente se enciende.
 *
 * @note
 * - Es fundamental respetar los retardos (`HAL_Delay`) entre comandos, especialmente al inicio,
 *   ya que algunos LCDs pueden tardar en arrancar después del encendido.
 * - Esta función debe llamarse una sola vez al comenzar el programa, antes de enviar cualquier comando o texto.
 *
 */

void LCD_Init()
{
    HAL_Delay(50);

    LCD_SendNibble(FUNCTION_SET_8BIT, MODE_RS_IR);
    HAL_Delay(5);
    LCD_SendNibble(FUNCTION_SET_8BIT, MODE_RS_IR);
    HAL_Delay(5);
    LCD_SendNibble(FUNCTION_SET_8BIT, MODE_RS_IR);
    HAL_Delay(1);

    LCD_SendNibble(LCD_FUNCTIONSET, MODE_RS_IR);
    HAL_Delay(1);

    LCD_SendCommand(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS);
    LCD_SendCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYOFF | LCD_CURSOROFF | LCD_BLINKOFF);
    LCD_SendCommand(LCD_CLEARDISPLAY);
    HAL_Delay(2);
    LCD_SendCommand(LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT);
    LCD_SendCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF);
}

/**
 * @brief Inicia el LCD configurando sus dimensiones y ejecutando la secuencia de inicialización.
 *
 * Esta función permite especificar el número de columnas y filas del display LCD antes de llamarse `LCD_Init()`.
 * Es útil para adaptar el controlador a diferentes modelos de pantallas (por ejemplo, 16x2, 20x4, etc.).
 *
 * @param cols Número de columnas (ancho) del LCD.
 * @param row  Número de filas (alto) del LCD.
 *
 * @details
 * 1. Se almacenan las dimensiones proporcionadas en la estructura `lcd_conf`, que es utilizada internamente
 *    por funciones como `LCD_SetCursor` o `LCD_PrintLine` para controlar el formato de salida.
 * 2. Luego se invoca `LCD_Init()`, que ejecuta la secuencia estándar de inicialización del controlador HD44780 en modo 4 bits.
 *
 * @note
 * - Esta función debe ser llamada al inicio del programa antes de imprimir cualquier texto en el LCD.
 * - Asegurate de que los valores `cols` y `row` coincidan con las especificaciones físicas del display conectado.
 * - Si se usan valores mayores que los soportados por el hardware, puede haber resultados inesperados o visualización incorrecta.
 *
 */

void LCD_Begin(uint8_t cols, uint8_t row)
{
	lcd_conf.I2C_LCD_nCol = cols;
	lcd_conf.I2C_LCD_nRow = row;
	LCD_Init();
}

/**
 * @brief Muestra datos de sensores (temperatura, giroscopio y acelerómetro) en el LCD.
 *
 * Esta función convierte los valores enteros escalados (x100) de tres sensores a formato decimal
 * y los presenta en tres líneas consecutivas del display. Cada valor es formateado y alineado dentro
 * de la línea correspondiente.
 *
 * @param temp_x100 Valor de temperatura multiplicado por 100 (por ejemplo, 2534 representa 25.3 °C).
 * @param gx_x100   Valor del eje X del giroscopio, también multiplicado por 100 (en grados por segundo).
 * @param ax_x100   Valor del eje X del acelerómetro, multiplicado por 100 (en "g").
 *
 * @details
 * 1. Usa `FormatIntDecimal()` para convertir los valores enteros a cadenas con formato decimal.
 *    - Temperatura con 1 decimal (`23.4`)
 *    - Giroscopio y acelerómetro con 2 decimales (`-12.34`)
 * 2. Construye cada línea de texto (`line`) con un encabezado descriptivo y el valor formateado.
 * 3. Muestra cada línea en el LCD utilizando `LCD_PrintLine()`, una para cada fila.
 *
 * @note
 * - Requiere que el LCD tenga al menos 3 líneas. Si el número de filas es menor, la última línea se sobrescribirá.
 * - Asegurate de que el buffer `line` tenga suficiente espacio para acomodar los textos formateados.
 *
 * @example
 * ```
 * LCD_Begin(20, 4);
 * LCD_PrintSensorData(2345, -1578, 980);
 * // Muestra:
 * // Temp: 23.4 C
 * // Gx: -15.78 deg/s
 * // Ax: 9.80 g
 * ```
 */
void LCD_PrintSensorData(int16_t temp_x100, int16_t gx_x100, int16_t ax_x100) {
    char line[lcd_conf.I2C_LCD_nCol + 1 ];  // 20 caracteres + nulo
    char value[10];

    // Línea 1: Temperatura
    FormatIntDecimal(value, temp_x100, 1); // 1 decimal
    sprintf(line, "Temp: %s C", value);
    LCD_PrintLine(0, line);

    // Línea 2: Gyro X
    FormatIntDecimal(value, gx_x100, 2); // 2 decimales
    sprintf(line, "Gx: %s deg/s", value);
    LCD_PrintLine(1, line);

    // Línea 3: Accel X
    FormatIntDecimal(value, ax_x100, 2);
    sprintf(line, "Ax: %s g", value);
    LCD_PrintLine(2, line);
}
