/*
 * Contador de Aforo - Máximo 7 Personas
 * BOG Project
 *
 * Conexiones:
 *   Botón ENTRADA  -> Pin 2 (con resistencia pull-down 10kΩ a GND)
 *   Botón SALIDA   -> Pin 3 (con resistencia pull-down 10kΩ a GND)
 *   LED 1          -> Pin 4 (con resistencia 220Ω a GND)
 *   LED 2          -> Pin 5 (con resistencia 220Ω a GND)
 *   LED 3          -> Pin 6 (con resistencia 220Ω a GND)
 *   LED 4          -> Pin 7 (con resistencia 220Ω a GND)
 *   LED 5          -> Pin 8 (con resistencia 220Ω a GND)
 *   LED 6          -> Pin 9 (con resistencia 220Ω a GND)
 *   LED 7          -> Pin 10 (con resistencia 220Ω a GND)
 */

const int PIN_ENTRADA = 2;
const int PIN_SALIDA  = 3;

const int PINES_LED[] = {4, 5, 6, 7, 8, 9, 10};
const int MAX_PERSONAS = 7;

int contador = 0;
bool estadoAnteriorEntrada = LOW;
bool estadoAnteriorSalida  = LOW;

void setup() {
  pinMode(PIN_ENTRADA, INPUT);
  pinMode(PIN_SALIDA,  INPUT);

  for (int i = 0; i < MAX_PERSONAS; i++) {
    pinMode(PINES_LED[i], OUTPUT);
  }

  Serial.begin(9600);
  Serial.println("=== Contador de Aforo BOG ===");
  Serial.println("Capacidad maxima: 7 personas");
  Serial.println("Personas en local: 0");
}

void loop() {
  bool estadoEntrada = digitalRead(PIN_ENTRADA);
  bool estadoSalida  = digitalRead(PIN_SALIDA);

  // Flanco ascendente botón ENTRADA
  if (estadoEntrada == HIGH && estadoAnteriorEntrada == LOW) {
    if (contador < MAX_PERSONAS) {
      contador++;
      actualizarLEDs();
      imprimirEstado();
    } else {
      Serial.println("¡AFORO COMPLETO! No pueden entrar mas personas.");
    }
    delay(200); // debounce
  }

  // Flanco ascendente botón SALIDA
  if (estadoSalida == HIGH && estadoAnteriorSalida == LOW) {
    if (contador > 0) {
      contador--;
      actualizarLEDs();
      imprimirEstado();
    } else {
      Serial.println("El local ya esta vacio.");
    }
    delay(200); // debounce
  }

  estadoAnteriorEntrada = estadoEntrada;
  estadoAnteriorSalida  = estadoSalida;
}

void actualizarLEDs() {
  for (int i = 0; i < MAX_PERSONAS; i++) {
    digitalWrite(PINES_LED[i], i < contador ? HIGH : LOW);
  }
}

void imprimirEstado() {
  Serial.print("Personas en local: ");
  Serial.print(contador);
  Serial.print(" / ");
  Serial.println(MAX_PERSONAS);

  if (contador == MAX_PERSONAS) {
    Serial.println(">> AFORO COMPLETO <<");
  }
}
