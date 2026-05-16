#include "web_server.h"
#include "light.h"
#include "pomodoro.h"
#include <ESP8266WebServer.h>
#include <uri/UriBraces.h>

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
  h1 { font-size: 1.2em; opacity: 0.6; margin: 8px 0 16px; }
  .status { font-size: 1.1em; opacity: 0.7; margin-bottom: 16px;
            min-height: 1.4em; }
  .btn { display: block; width: 100%; padding: 28px; margin: 10px 0;
         border: none; border-radius: 16px; font-size: 1.3em;
         color: white; cursor: pointer; }
  .red    { background: #c0392b; }
  .yellow { background: #d68910; }
  .green  { background: #27ae60; }
  .off    { background: #2c3e50; }
  .pomo   { background: #8e44ad; }
  .stop   { background: #555; }
  .row    { display: flex; gap: 10px; }
  .row .btn { flex: 1; }
</style></head><body>
<h1>Focus light</h1>
<div class="status" id="status">—</div>

<button class="btn pomo" onclick="post('/pomodoro/start')">Start pomodoro</button>
<button class="btn stop" onclick="post('/pomodoro/stop')">Stop pomodoro</button>

<div style="opacity: 0.4; font-size: 0.85em; margin: 20px 0 10px;">manual override</div>
<button class="btn red"    onclick="post('/state/red')">Focus</button>
<button class="btn yellow" onclick="post('/state/yellow')">Wrapping up</button>
<button class="btn green"  onclick="post('/state/green')">Available</button>
<button class="btn off"    onclick="post('/state/off')">Off</button>

<script>
function post(url) { fetch(url, {method:'POST'}).then(refresh); }
function fmt(s) {
  const m = Math.floor(s/60), sec = s%60;
  return m + ':' + String(sec).padStart(2,'0');
}
function refresh() {
  fetch('/status').then(r=>r.json()).then(d=>{
    const el = document.getElementById('status');
    if (d.pomodoro_running) {
      el.textContent = d.phase + ' - Remaining ' + fmt(d.remaining_seconds);
    } else {
      el.textContent = d.state;
    }
  }).catch(()=>{});
}
refresh();
setInterval(refresh, 1000);
</script>
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

    pomodoro_stop();
    light_set(s);
    server.send(200, "text/plain", "ok");
}

static void handle_pomodoro_start() {
    pomodoro_start();
    server.send(200, "text/plain", "started");
}

static void handle_pomodoro_stop() {
    pomodoro_stop();
    server.send(200, "text/plain", "stopped");
}

static void handle_status() {
    String json = "{";
    json += "\"state\":\"";
    json += light_state_name(light_get());
    json += "\",\"pomodoro_running\":";
    json += pomodoro_is_running() ? "true" : "false";
    json += ",\"phase\":\"";
    json += pomodoro_phase_name(pomodoro_current_phase());
    json += "\",\"remaining_seconds\":";
    json += pomodoro_remaining_seconds();
    json += "}";
    server.send(200, "application/json", json);
}

void web_setup() {
    server.on("/", HTTP_GET, handle_root);
    server.on(UriBraces("/state/{}"), HTTP_POST, handle_state);
    server.on("/pomodoro/start", HTTP_POST, handle_pomodoro_start);
    server.on("/pomodoro/stop", HTTP_POST, handle_pomodoro_stop);
    server.on("/status", HTTP_GET, handle_status);
    server.begin();
    Serial.println("server up");
}

void web_loop() {
    server.handleClient();
}