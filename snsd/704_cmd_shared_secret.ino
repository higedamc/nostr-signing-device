/**
   @brief Build a shared secret (using Diffie-Hellman) between the private key on the device and an external public key.

   @param publicKeyHex: String. The public key of the other party.
                        The format must be 128 hex characters, 64 bytes (x,y). No `02`, `03` or `04` prefixes.
   @return CommandResponse
*/

String generateSharedSecret(String publicKeyHex) {
  // Reconstruct full public key if only X-coordinate is provided
  if (publicKeyHex.length() == 64) {
    publicKeyHex = reconstructPublicKey(publicKeyHex);
  }

  String privateKeyHex = global.privateKeys[global.activeKeyIndex];

  int byteSize =  32;
  byte privateKeyBytes[byteSize];
  fromHex(privateKeyHex, privateKeyBytes, byteSize);
  PrivateKey privateKey(privateKeyBytes);

  byte sharedSecret[32];

  byte publicKeyBin[64];
  fromHex(publicKeyHex, publicKeyBin, 64);
  PublicKey otherPublicKey(publicKeyBin, true);
  privateKey.ecdh(otherPublicKey, sharedSecret, false);

  return toHex(sharedSecret, sizeof(sharedSecret));
}

CommandResponse executeSharedSecret(String publicKeyHex) {
  global.onLogo = false;
  setDisplay(true);

  if (global.privateKeys.empty()) {
    showMessage("No keys available", "Add a key first");
    return {"Error", "No keys available"};
  }

  String privateKeyHex = global.privateKeys[global.activeKeyIndex];
  String keyPreview = padRightWithSpaces(global.keyNames.count(privateKeyHex) ? global.keyNames[privateKeyHex] : previewString(hexToNostr(getPublicKey(privateKeyHex), "npub")), 20);

  // Identify the counterparty on screen so the user can tell a self-ECDH
  // (e.g. a host vault deriving its own conversation key) from a shared
  // secret with someone else's key.
  String counterparty;
  if (publicKeyHex.length() >= 64) {
    String otherX = publicKeyHex.substring(0, 64);
    if (otherX.equalsIgnoreCase(getPublicKey(privateKeyHex))) {
      counterparty = "SELF (this key)";
    } else {
      counterparty = previewString(hexToNostr(otherX, "npub"));
    }
  } else {
    counterparty = previewString(publicKeyHex);
  }

  if (!confirmOnDevice("Secret Request:", "Key: " + keyPreview, "With: " + counterparty)) {
    // Reject
    sendCommandOutput(COMMAND_SHARED_SECRET, "Rejected"); // Notify rejection
    showMessage("Request Rejected", "Shared secret aborted.");

    return {"Rejected", "Shared secret aborted"};
  }

  // Accept — derive the secret only after the user confirmed.
  showMessage("Please wait", "Deriving secret...");

  String sharedSecretHex = generateSharedSecret(publicKeyHex);
  sendCommandOutput(COMMAND_SHARED_SECRET, sharedSecretHex);
  showMessage("Shared Secret", "sent...");

  return {"Shared Secret", "sent..."};
}
