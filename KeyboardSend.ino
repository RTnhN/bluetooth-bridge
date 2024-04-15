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

  int startIndex = 0;
  int endIndex = 0;
  Serial.println(input);
  while (startIndex != -1) {
    startIndex = input.indexOf('{', endIndex);
    Serial.print("startIndex: ");
    Serial.println(startIndex);
    if (startIndex != -1) {
      endIndex = input.indexOf('}', startIndex);
      Serial.print("EndIndex: ");
      Serial.println(endIndex);
      if (endIndex == -1) {
        Serial.println("Error: Malformed command");
        break; // Malformed command, break the loop
      }
      String command = input.substring(startIndex + 1, endIndex);
      Serial.print("Command: ");
      Serial.println(command);
      if (command == "SPACE") {
        bleKeyboard.write(' ');
        Serial.println("Executed: SPACE");
      } else if (command == "BACKSPACE") {
        bleKeyboard.write(KEY_BACKSPACE);
        Serial.println("Executed: BACKSPACE");
      } else if (command == "ENTER") {
        bleKeyboard.write(KEY_RETURN);
        Serial.println("Executed: ENTER");
      } else {
        bleKeyboard.write(static_cast<uint8_t>(command[0]));
        Serial.println("sent command");
        }
      endIndex++; // Move past the '}' for the next iteration
    } else {
      if (endIndex < input.length()) {
        bleKeyboard.print(input.substring(endIndex)); // Send remaining text as regular input
        Serial.print("Sent text: ");
        Serial.println(input.substring(endIndex));
      }
      break;
    }
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
