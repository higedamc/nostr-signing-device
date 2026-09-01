CommandResponse executeReboot(String data) {
  global.onLogo = false;
  setDisplay(true);

  if (!confirmOnDevice("Reboot Device?", "", "")) {
    sendCommandOutput(COMMAND_REBOOT, "Rejected");
    showMessage("Request Rejected", "Reboot aborted.");
    return {"Rejected", "Reboot aborted"};
  }

  ESP.restart();
  showMessage("Reset", "Device is rebooting.");
  return {"Reset", "Device is rebooting."};
}

CommandResponse executeWipe(String data) {
  global.onLogo = false;
  setDisplay(true);

  if (!confirmOnDevice("WIPE DEVICE?", "ERASES ALL KEYS", "Cannot be undone")) {
    sendCommandOutput(COMMAND_WIPE, "Rejected");
    showMessage("Request Rejected", "Wipe aborted.");
    return {"Rejected", "Wipe aborted"};
  }

  SPIFFS.format();
  ESP.restart();
  showMessage("Wipe", "Wiping device of all data.");
  return {"Wipe", "Wiping device of all data."};
}