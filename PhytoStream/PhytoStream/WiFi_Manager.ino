void displayConnection(){
  switch(CONNECTION_STATE){
    case DISCONNECTED: 
      wifiLed.blink(50, 1450);
      break;
    case AP_CONNECTED:
      wifiLed.blink(1500, 1500);
      break;
    case STA_CONNECTED:
      wifiLed.on();
      break;
  }
}

void connectToWifi_AP(){ 
  if(CONNECTION_TYPE == ACCESS_POINT && CONNECTION_STATE != AP_CONNECTED){
    logger.write(Log::LOG_INFO, "Creating AP", "SSID: " + (String)ssidAP + " | Password: " + (String)passwordAP);  
    WiFi.mode(WIFI_AP);
    if(WiFi.softAP(ssidAP, passwordAP)){
      CONNECTION_STATE = AP_CONNECTED;
      logger.write(Log::LOG_INFO, "AP has been created", "AP IP address: " + WiFi.softAPIP().toString());
    }
    else{
      logger.write(Log::LOG_WARN, "AP has not been created");
    }
  }
}

void connectToWifi_STA(){
  if(CONNECTION_TYPE == WIFI_CLIENT && CONNECTION_STATE != STA_CONNECTED){
    logger.write(Log::LOG_INFO, "Connecting to Wi-Fi network", "SSID: " + ssidSTA);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssidSTA, passwordSTA);
  }
}

void onStationConnected(const WiFiEventStationModeGotIP& evt){
  CONNECTION_STATE = STA_CONNECTED;
  logger.write(Log::LOG_INFO, "Connected to Wi-Fi network", "Local IP address: " + WiFi.localIP().toString());
  ipAddress = WiFi.localIP();
  writeToFlash("/sta_ip.txt", WiFi.localIP().toString());
}

void onStationDisconnected(const WiFiEventStationModeDisconnected& evt){
  CONNECTION_STATE = DISCONNECTED;
  logger.write(Log::LOG_WARN, "Disonnected from Wi-Fi network", "SSID: " + ssidSTA);
}

void disconnect(){
  logger.write(Log::LOG_WARN, "Disconnecting ...");
    
  if(CONNECTION_STATE == STA_CONNECTED)
    WiFi.disconnect();
  else if(CONNECTION_STATE == AP_CONNECTED){
    WiFi.softAPdisconnect(true);
    logger.write(Log::LOG_WARN, "AP is disabled", "SSID: " + (String)ssidAP);
  }

  CONNECTION_STATE = DISCONNECTED;
}
