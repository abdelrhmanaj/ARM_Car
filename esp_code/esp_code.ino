#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>

// ESP32 is the Wi-Fi gateway. Change these credentials before deployment.
static const char *AP_NAME = "SmartCar-ESP32";
static const char *AP_PASSWORD = "smartcar123"; // 8+ characters

// UART2: ESP32 RX2 (GPIO16) <- STM TX, ESP32 TX2 (GPIO17) -> STM RX.
// Serial2 is the board-supported HardwareSerial instance for UART2.
WebServer server(80);
String telemetry = "{\"distance\":0,\"leftDistance\":0,\"rightDistance\":0,\"mode\":\"AUTO_SCAN\"}";
unsigned long commandCount = 0;
unsigned long telemetryCount = 0;

void forwardToStm(const String &line) {
  Serial.print("[UART -> STM] ");
  Serial.println(line);
  Serial2.println(line);
}

void handleHealth() {
  Serial.println("[HTTP] GET /api/health");
  server.send(200, "application/json", "{\"ok\":true,\"device\":\"esp32\"}");
}

void handleState() {
  Serial.print("[HTTP] GET /api/state -> ");
  Serial.println(telemetry);
  server.send(200, "application/json", telemetry);
}

void handleCommand() {
  Serial.println("[HTTP] POST /api/command");
  if (!server.hasArg("plain")) {
    Serial.println("[HTTP] ERROR: request body is empty");
    server.send(400, "application/json", "{\"error\":\"missing body\"}");
    return;
  }

  const String body = server.arg("plain");
  Serial.print("[HTTP BODY] ");
  Serial.println(body);
  if (body.indexOf("\"type\":\"stop\"") >= 0) {
    forwardToStm("CMD STOP");
  } else if (body.indexOf("\"type\":\"light\"") >= 0) {
    if (body.indexOf("\"state\":true") >= 0 || body.indexOf("\"action\":\"on\"") >= 0) {
      forwardToStm("CMD LIGHT ON");
    } else {
      forwardToStm("CMD LIGHT OFF");
    }
  } else if (body.indexOf("\"type\":\"horn\"") >= 0) {
    if (body.indexOf("\"state\":true") >= 0 || body.indexOf("\"action\":\"on\"") >= 0) {
      forwardToStm("CMD HORN ON");
    } else {
      forwardToStm("CMD HORN OFF");
    }
  } else if (body.indexOf("\"type\":\"speed\"") >= 0) {
    const int marker = body.indexOf("\"value\":");
    const int speed = marker >= 0 ? body.substring(marker + 8).toInt() : 0;
    forwardToStm("CMD SPEED " + String(constrain(speed, 0, 100)));
  } else if (body.indexOf("\"type\":\"move\"") >= 0) {
    String direction = "stop";
    const int marker = body.indexOf("\"direction\":\"");
    if (marker >= 0) {
      const int start = marker + 13;
      const int end = body.indexOf('"', start);
      direction = body.substring(start, end);
      direction.toUpperCase();
    }
    forwardToStm("CMD MOVE " + direction);
  } else {
    Serial.println("[HTTP] ERROR: unknown command");
    server.send(400, "application/json", "{\"error\":\"unknown command\"}");
    return;
  }
  commandCount++;
  Serial.print("[HTTP] command accepted #");
  Serial.println(commandCount);
  server.send(200, "application/json", "{\"accepted\":true}");
}

void handleNotFound() {
  Serial.print("[HTTP] 404 ");
  Serial.println(server.uri());
  server.send(404, "application/json", "{\"error\":\"not found\"}");
}

void readStmTelemetry() {
  static String line;
  while (Serial2.available()) {
    const char c = static_cast<char>(Serial2.read());
    if (c == '\n') {
      line.trim();
      if (line.length() > 0) {
        Serial.print("[UART <- STM] ");
        Serial.println(line);
        if (line.startsWith("TEL DIST ")) {
          int dist = 0;
          sscanf(line.c_str(), "TEL DIST %d", &dist);
          telemetry = "{\"distance\":" + String(dist) + ",\"mode\":\"AUTO_SCAN\"}";
          telemetryCount++;
          Serial.print("[TELEMETRY] updated #");
          Serial.print(telemetryCount);
          Serial.print(" -> ");
          Serial.println(telemetry);
        } else if (line.startsWith("TEL ")) {
          int left = 0, right = 0;
          char mode[12] = "AUTO_SCAN";
          if (sscanf(line.c_str(), "TEL LEFT %d RIGHT %d MODE %11s", &left, &right, mode) >= 2) {
            telemetry = "{\"distance\":" + String(left) + ",\"mode\":\"" + String(mode) + "\"}";
            telemetryCount++;
            Serial.print("[TELEMETRY] updated #");
            Serial.print(telemetryCount);
            Serial.print(" -> ");
            Serial.println(telemetry);
          }
        }
      }
      line = "";
    } else if (c != '\r' && line.length() < 100) {
      line += c;
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_NAME, AP_PASSWORD);

  Serial.println();
  Serial.println("=== SmartCar ESP32 Debug ===");
  Serial.print("AP SSID: ");
  Serial.println(AP_NAME);
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());
  Serial.println("UART2: RX GPIO16, TX GPIO17, 115200 baud");

  server.on("/api/health", HTTP_GET, handleHealth);
  server.on("/api/state", HTTP_GET, handleState);
  server.on("/api/command", HTTP_POST, handleCommand);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  readStmTelemetry();
}
