/*
  This file is part of the ArduinoECCX08 library.
  Copyright (c) 2019 Arduino SA. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "PEMUtils.h"
#include "b64.h"

String PEMUtilsClass::base64Encode(const byte in[], unsigned int length, const char* prefix, const char* suffix)
{
  static const char* CODES = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

  int b;
  String out;

  int reserveLength = 4 * ((length + 2) / 3) + ((length / 3 * 4) / 76) + strlen(prefix) + strlen(suffix);
  out.reserve(reserveLength);

  if (prefix) {
    out += prefix;
  }
  
  for (unsigned int i = 0; i < length; i += 3) {
    if (i > 0 && (i / 3 * 4) % 76 == 0) { 
      out += '\n'; 
    }

    b = (in[i] & 0xFC) >> 2;
    out += CODES[b];

    b = (in[i] & 0x03) << 4;
    if (i + 1 < length) {
      b |= (in[i + 1] & 0xF0) >> 4;
      out += CODES[b];
      b = (in[i + 1] & 0x0F) << 2;
      if (i + 2 < length) {
         b |= (in[i + 2] & 0xC0) >> 6;
         out += CODES[b];
         b = in[i + 2] & 0x3F;
         out += CODES[b];
      } else {
        out += CODES[b];
        out += '=';
      }
    } else {
      out += CODES[b];
      out += "==";
    }
  }

  if (suffix) {
    out += suffix;
  }

  return out;
}

int PEMUtilsClass::base64Decode(const String in, byte out[])
{
  String inBase64 = String(in);
  inBase64.trim();
  // check PEM encoded
  if ( inBase64.startsWith("-----BEGIN ") && inBase64.endsWith("-----")) {
      int endLine1Char = inBase64.indexOf('\n');
      if (endLine1Char < 0) {
          return -2;
      }
      int endLastLineChar = inBase64.lastIndexOf('\n');
      if (endLastLineChar < 0) {
          return -3;
      }
      inBase64 = inBase64.substring(endLine1Char, endLastLineChar);
      for (int i=inBase64.indexOf('\n'); i != -1; i=inBase64.indexOf('\n')) {
        inBase64.remove(i,1);
      }
  }

  int len = decode_base64((unsigned char*)inBase64.c_str(), out); 
  return len;
}

int PEMUtilsClass::xyFromPubKeyPEM(const String publicKeyPem, byte xy[64])
{
    byte derBytes[256];
    int derLen = PEMUtils.base64Decode(publicKeyPem, derBytes);
    if (derLen < 1) {
        return -4;
    }
    if (derLen < 65) {
        return -5;
    }

    int j = derLen-64;
    if (derBytes[j-1] != 0x04) { // ASN.1 uncompressed public key
        return -6;
    }

    // the public key X and Y values are the last 64 bytes
    for (int i = 0; i < 64; i++) {
        xy[i] = derBytes[j++];
    }

    return 0;
}

PEMUtilsClass PEMUtils;
