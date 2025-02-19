CommandResponse executeDecryptMessage(String data) {
  if (data == "") return {"", ""};
  
  // Get the shared secret as the 3rd party public key
  String thirdPartyPublicKey = getTokenAtPosition(data, " ", 0);

  thirdPartyPublicKey.trim();

  thirdPartyPublicKey.toLowerCase();
  if(!isValidHexKey(thirdPartyPublicKey)) {
    showMessage("Decrypt Message", "Invalid 3rd party public key.");
    return {"", ""};
  }

  String sharedSecretHex = generateSharedSecret(thirdPartyPublicKey);
  
  // Get the content as everything after the first space
  String encryptedContent = getTokenAtPosition(data, " ", 1);
  encryptedContent.trim();

  String decryptedMessage = decryptDirectMessageContent(sharedSecretHex, encryptedContent);
  sendCommandOutput(COMMAND_DECRYPT_MESSAGE, decryptedMessage);

  showMessage("Decrypt Message", decryptedMessage.substring(0, 16) + "...");
  
  return {"Decrypt Message", data};
}