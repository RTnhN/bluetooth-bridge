#include <BleKeyboard.h>

BleKeyboard bleKeyboard("MrArduinoKeyboard", "Espressif", 100);

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();
}

void handleCommands(const String& input) {
  if (!bleKeyboard.isConnected()) {
    Serial.println("BLE Keyboard is not connected.");
    return;  // Exit the function if not connected
  }

  int startIndex = input.indexOf('{');
  int endIndex = input.indexOf('}', startIndex);
  if (startIndex == -1 || endIndex == -1) {
    Serial.println("Error: Malformed command");
    return; // Malformed command, exit the function
  }
  
  String command = input.substring(startIndex + 1, endIndex);
  if (command == "SPACE") {
    bleKeyboard.write(' ');
  } else if (command == "BACKSPACE") {
    bleKeyboard.write(KEY_BACKSPACE);
  } else if (command == "ENTER") {
    bleKeyboard.write(KEY_RETURN);
  } else if (command == "UP") {
    bleKeyboard.write(KEY_UP_ARROW);
  } else if (command == "DOWN") {
    bleKeyboard.write(KEY_DOWN_ARROW);
  } else if (command == "LEFT") {
    bleKeyboard.write(KEY_LEFT_ARROW);
  } else if (command == "RIGHT") {
    bleKeyboard.write(KEY_RIGHT_ARROW);
  } else if (command == "TAB") {
    bleKeyboard.write(KEY_TAB);
  } else {
    bleKeyboard.write(static_cast<uint8_t>(command[0]));
  }
}

void loop() {
  static String buffer = "";
  while (Serial.available()) {
    char ch = Serial.read();
    buffer += ch;
    if (buffer.indexOf('}') != -1) { // Process when we have a complete command
      handleCommands(buffer);
      buffer = ""; // Reset buffer after processing
    }
  }

  // Periodically send connection status
  if (!bleKeyboard.isConnected()) {
    Serial.println("BLE Keyboard is not connected.");
    delay(5000); // Reduce message frequency when disconnected
  }
}
