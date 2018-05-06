#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <scrapheep.h>

const char *ssid = "PleaseDontHackMeImAPoorRobot";

ESP8266WebServer server(80);

/* Web Routes */

/* Generic Message to all those who enter the root. */
void rootPath() {
  server.sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  server.send(200, "text/html", "<h1>I'm Just a Poor Robot.</h1>");
}

/* Control the Robot, awful code, but lel got to give robots rights.... */
void actionPath() {
  int delayTime = 1000;
  int speed = 255;
  bool continuous = false;
  bool reverse = false;
  server.sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  if (server.arg("reverse") == "true") {
    reverse = true;
  }
  if (server.arg("continuous") == "true") {
    continuous = true;
  }
  if (server.arg("time") != "") {
    delayTime = server.arg("time").toInt();
  }
  if (server.arg("speed") != "") {
    speed = server.arg("speed").toInt();
  }
  if (server.arg("action") != "") {
    /* Parse through the actions given. */
    if (server.arg("action") == "left") {
      server.send(200, "text/html", "<h1>Left</h1>");
      if (reverse == true) {
        digitalWrite(PIN_MOTOR_A_DIR, 1);
      } else {
        digitalWrite(PIN_MOTOR_A_DIR, 0);
      }
      digitalWrite(PIN_MOTOR_A_SPEED, HIGH);
      if (!continuous) {
        delay(delayTime);
        digitalWrite(PIN_MOTOR_A_SPEED, LOW);
      }
      
    } else if (server.arg("action") == "right") {
      server.send(200, "text/html", "<h1>Right</h1>");
      if (reverse == true) {
        digitalWrite(PIN_MOTOR_B_DIR, 1);
      } else {
        digitalWrite(PIN_MOTOR_B_DIR, 0);
      }
      digitalWrite(PIN_MOTOR_B_SPEED, HIGH);
      if (!continuous) {
        delay(delayTime);
        digitalWrite(PIN_MOTOR_B_SPEED, LOW);
      }

    } else if (server.arg("action") == "both") {
      server.send(200, "text/html", "<h1>Both</h1>");
      if (reverse == true) {
        digitalWrite(PIN_MOTOR_A_DIR, 1);
        digitalWrite(PIN_MOTOR_B_DIR, 1);
      } else {
        digitalWrite(PIN_MOTOR_A_DIR, 0);
        digitalWrite(PIN_MOTOR_B_DIR, 0);
      }
      digitalWrite(PIN_MOTOR_A_SPEED, HIGH);
      digitalWrite(PIN_MOTOR_B_SPEED, HIGH);
      if (!continuous) {
        delay(delayTime);
        digitalWrite(PIN_MOTOR_A_SPEED, LOW);
        digitalWrite(PIN_MOTOR_B_SPEED, LOW);
      }
    } else if (server.arg("action") == "stop") {
      /* Stop Every Action */
      server.send(200, "text/html", "<h1>Stoping Actions</h1>");
      digitalWrite(PIN_MOTOR_A_DIR, LOW);
      digitalWrite(PIN_MOTOR_B_DIR, LOW);
      digitalWrite(PIN_MOTOR_A_SPEED, LOW);
      digitalWrite(PIN_MOTOR_B_SPEED, LOW);
    }
  } else {
    server.send(200, "text/html", "<h1>No Action Taken</h1>");
    /* No actions, so give up hope. */
  }
}



void setup() {
  /* Setup The Pins */
  pinMode(PIN_D0, OUTPUT);
  pinMode(PIN_D1, OUTPUT);
  pinMode(PIN_D2, INPUT);
  pinMode(PIN_D4, INPUT);
  pinMode(PIN_MOTOR_A_DIR, OUTPUT);
  pinMode(PIN_MOTOR_A_SPEED, OUTPUT);
  pinMode(PIN_MOTOR_B_DIR, OUTPUT);
  pinMode(PIN_MOTOR_B_SPEED, OUTPUT);
  /* Setup Serial so we can send debug logs */
  Serial.begin(115200);
  /* Sleep for 50ms */
  delay(1000);
  /* Allow us to watch the movement of the motor */
  digitalWrite(PIN_D0, HIGH);
  digitalWrite(PIN_D1, LOW);


  /* Setup THe WiFI network */
  WiFi.softAP(ssid);
  IPAddress myIP = WiFi.softAPIP();
  Serial.println("IP Address:"+myIP);
  /* Mount the Routes */
  server.on("/", HTTP_GET, rootPath);
  server.on("/", HTTP_OPTIONS, []() {
    server.sendHeader("Access-Control-Max-Age", "10000");
    server.sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
    server.send(200, "text/html", "" );
  });
  server.on("/action", HTTP_OPTIONS, []() {
    server.sendHeader("Access-Control-Max-Age", "10000");
    server.sendHeader("Access-Control-Allow-Methods", "POST,GET,OPTIONS");
    server.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
    server.send(200, "text/html", "" );
  });
  server.on("/action", HTTP_GET, actionPath);
  server.begin();
  Serial.println("HTTP Control Server Started");

}

void loop() {
  server.handleClient();
}
