/**
   @brief Ask the user to confirm an action with the physical buttons.

   Draws a confirmation screen and blocks until the user presses
   Top (accept) or Bottom (reject). The accept path is debounced with
   global.debounceDelay; a bounced press redraws the screen and keeps
   waiting, matching the original /sign-message loop this was extracted
   from.

   @param title: Heading shown in the foreground color.
   @param line1: First detail line, shown in the accent color.
   @param line2: Optional second detail line (skipped when empty).
   @return true if the user accepted (Top), false if rejected (Bottom).
*/
bool confirmOnDevice(const String &title, const String &line1, const String &line2) {
  unsigned long lastButton2Press = 0;

  while (true) {
    tft.fillScreen(global.backgroundColor);
    tft.setTextColor(global.foregroundColor, global.backgroundColor);
    tft.setTextSize(2);
    tft.setCursor(0, 10);
    tft.println(title);
    tft.println("");
    tft.setTextColor(global.accentColor, global.backgroundColor);
    tft.println(line1);
    if (line2.length() > 0) {
      tft.println(line2);
    }
    tft.println("");

    tft.setTextColor(global.foregroundColor, global.backgroundColor);
    tft.setCursor(0, 100 * global.scaleFactor);
    tft.println("Top to accept");
    tft.println("Bottom to reject");

    EventData event = awaitEvent();

    if (event.type == EVENT_BUTTON_ACTION) {
      String buttonNumber = getWordAtPosition(event.data, 0);
      String buttonState = getWordAtPosition(event.data, 1);

      if (buttonState == "1") {
        if (buttonNumber == "2") {
          unsigned long currentMillis = millis();
          if (currentMillis - lastButton2Press > global.debounceDelay) {
            lastButton2Press = currentMillis;
            return true;
          }
        } else if (buttonNumber == "1") {
          return false;
        }
      }
    }
  }
}
