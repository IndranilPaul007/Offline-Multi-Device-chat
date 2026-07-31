#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Set the network name and password for your private chatroom
const char* ssid = "ESP_Offline_Chat";
const char* password = "password123";

ESP8266WebServer server(80);

// String to hold the chat history in the ESP's RAM
String chatHistory = "";

// The main HTML page with CSS and JavaScript
const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Off-Grid Chat</title>
  <style>
    body { font-family: 'Segoe UI', sans-serif; background-color: #0d1117; color: #c9d1d9; margin: 0; padding: 0; display: flex; flex-direction: column; height: 100vh; }
    #header { background: #161b22; padding: 15px; text-align: center; border-bottom: 1px solid #30363d; font-weight: bold; font-size: 1.2em; color: #58a6ff;}
    #chatBox { flex: 1; padding: 15px; overflow-y: auto; display: flex; flex-direction: column; gap: 10px; }
    .msg { background: #21262d; padding: 12px; border-radius: 8px; word-wrap: break-word; border: 1px solid #30363d; }
    .msg b { color: #3fb950; }
    #inputArea { display: flex; padding: 15px; background: #161b22; border-top: 1px solid #30363d; gap: 10px; }
    input[type="text"] { padding: 12px; border-radius: 6px; border: 1px solid #30363d; background: #0d1117; color: white; font-size: 16px; outline: none; }
    #username { width: 80px; }
    #message { flex: 1; }
    button { background: #238636; color: white; border: none; padding: 12px 20px; border-radius: 6px; cursor: pointer; font-size: 16px; font-weight: bold; transition: 0.2s; }
    button:hover { background: #2ea043; }
  </style>
</head>
<body>
  <div id="header">📡 Local Secure Chat</div>
  <div id="chatBox"></div>
  <div id="inputArea">
    <input type="text" id="username" placeholder="Name">
    <input type="text" id="message" placeholder="Type a message...">
    <button onclick="sendMessage()">Send</button>
  </div>

  <script>
    // Fetch new messages from the ESP8266
    function fetchChat() {
      fetch('/get')
        .then(response => response.text())
        .then(data => {
          let box = document.getElementById('chatBox');
          if(box.innerHTML !== data) { // Only scroll if there's new data
            box.innerHTML = data;
            box.scrollTop = box.scrollHeight;
          }
        });
    }
    
    // Auto-refresh the chat every 2 seconds
    setInterval(fetchChat, 2000); 

    // Send message to the ESP8266
    function sendMessage() {
      let user = document.getElementById('username').value;
      let msg = document.getElementById('message').value;
      
      if(!user) user = "Anon";
      if(!msg) return; // Don't send empty messages
      
      document.getElementById('message').value = ""; // Clear input box
      
      // Send data to the server
      fetch('/send?user=' + encodeURIComponent(user) + '&msg=' + encodeURIComponent(msg))
        .then(() => fetchChat()); // Instantly refresh after sending
    }
    
    // Allow pressing "Enter" on keyboard to send
    document.getElementById('message').addEventListener('keypress', function (e) {
        if (e.key === 'Enter') sendMessage();
    });
    
    // Load chat history when the page first opens
    fetchChat();
  </script>
</body>
</html>
)rawliteral";

// Serve the HTML page
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

// Serve the chat history
void handleGet() {
  server.send(200, "text/html", chatHistory);
}

// Receive a new message and add it to the history
void handleSend() {
  if (server.hasArg("user") && server.hasArg("msg")) {
    String user = server.arg("user");
    String msg = server.arg("msg");
    
    // Basic security: prevent HTML injection
    user.replace("<", "&lt;");
    msg.replace("<", "&lt;");

    // Format the new message
    String newMsg = "<div class='msg'><b>" + user + ":</b> " + msg + "</div>";
    
    // Add it to the main log
    chatHistory += newMsg;
    
    // Keep history from overflowing the ESP's RAM (clears oldest messages if it gets too big)
    if (chatHistory.length() > 3000) {
      chatHistory = chatHistory.substring(chatHistory.indexOf("</div>", 500) + 6);
    }
    
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

void setup() {
  Serial.begin(115200);
  
  // Set ESP8266 to Access Point mode
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  
  Serial.println("\n[SYSTEM] Chat AP Started");
  Serial.print("[SYSTEM] IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Setup server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/get", HTTP_GET, handleGet);
  server.on("/send", HTTP_GET, handleSend);

  server.begin();
}

void loop() {
  server.handleClient();
}