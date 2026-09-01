/**
   @brief Sign a random message.

   @param messageHex: String. The message to be signed in hex format.
   @return CommandResponse
*/
CommandResponse executeSignMessage(String messageHex) {
  global.onLogo = false;
  setDisplay(true);

  if (global.privateKeys.empty()) {
    showMessage("No keys available", "Add a key first");
    return {"Error", "No keys available"};
  }

  int selectedIndex = global.activeKeyIndex;
  String privateKeyHex = global.privateKeys[selectedIndex];
  String keyPreview = padRightWithSpaces(global.keyNames.count(privateKeyHex) ? global.keyNames[privateKeyHex] : previewString(hexToNostr(getPublicKey(privateKeyHex), "npub")), 20);

  if (!confirmOnDevice("Sign Request:", "Key: " + keyPreview, "")) {
    // Reject
    sendCommandOutput(COMMAND_SIGN_MESSAGE, "Rejected"); // Notify rejection
    showMessage("Request Rejected", "Message signing aborted.");

    return {"Rejected", "Message signing aborted"};
  }

  // Accept
  showMessage("Please wait", "Signing message...");

  int byteSize = 32;
  byte privateKeyBytes[byteSize];
  fromHex(privateKeyHex, privateKeyBytes, byteSize);
  PrivateKey privateKey(privateKeyBytes);

  byte messageBytes[byteSize];
  fromHex(messageHex, messageBytes, byteSize);
  SchnorrSignature signature = privateKey.schnorr_sign(messageBytes);

  String signatureHex = String(signature);
  sendCommandOutput(COMMAND_SIGN_MESSAGE, signatureHex);
  showMessage("Signed Message", signatureHex.substring(0, 16) + "...");

  return {"Signed Message", signatureHex.substring(0, 16) + "..."};
}
