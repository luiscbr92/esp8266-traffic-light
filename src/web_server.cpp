#include <uri/UriBraces.h>
#include "web_server.h"
#include "light.h"
#include <ESP8266WebServer.h>

static ESP8266WebServer server(80);

static const char PAGE[] PROGMEM = R"HTML(
<!DOCTYPE html>
<html><head>
<meta name="viewport" content="width=device-width,initial-scale=1">
<meta name="apple-mobile-web-app-capable" content="yes">
<title>Focus light</title>
<style>
  body { font-family: sans-serif; background: #111; color: #eee;
         margin: 0; padding: 20px; text-align: center; }
  h1 { font-size: 1.2em; opacity: 0.6; }
  .btn { display: block; width: 100%; padding: 32px; margin: 12px 0;
         border: none; border-radius: 16px; font-size: 1.5em;
         color: white; cursor: pointer; }
  .red    { background: #c0392b; }
  .yellow { background: #d68910; }
  .green  { background: #27ae60; }
  .off    { background: #2c3e50; }
</style></head><body>
<h1>Focus light</h1>
<button class="btn red"    onclick="fetch('/state/red',{method:'POST'})">Focus</button>
<button class="btn yellow" onclick="fetch('/state/yellow',{method:'POST'})">Wrapping up</button>
<button class="btn green"  onclick="fetch('/state/green',{method:'POST'})">Available</button>
<button class="btn off"    onclick="fetch('/state/off',{method:'POST'})">Off</button>
</body></html>
)HTML";

static void handle_root() {
    server.send_P(200, "text/html", PAGE);
}

static void handle_state() {
    String color = server.pathArg(0);
    LightState s;
    if      (color == "red")    s = LightState::RED;
    else if (color == "yellow") s = LightState::YELLOW;
    else if (color == "green")  s = LightState::GREEN;
    else if (color == "off")    s = LightState::OFF;
    else { server.send(400, "text/plain", "bad color"); return; }

    light_set(s);
    server.send(200, "text/plain", "ok");
}

static void handle_status() {
    String json = "{\"state\":\"";
    json += light_state_name(light_get());
    json += "\"}";
    server.send(200, "application/json", json);
}

void web_setup() {
    server.on("/", HTTP_GET, handle_root);
    server.on(UriBraces("/state/{}"), HTTP_POST, handle_state);
    server.on("/status", HTTP_GET, handle_status);
    server.begin();
    Serial.println("server up");
}

void web_loop() {
    server.handleClient();
}