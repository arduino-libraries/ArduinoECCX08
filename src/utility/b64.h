#ifndef _B64_H_
#define _B64_H_
/*
  This base64 decode function is copied from v1.1.1 of: https://github.com/Densaugeo/base64_arduino
  
  MIT License (MIT) Copyright (c) 2016 Densaugeo

  Any other base64 decode function would do if there is an Arduino preferred one somewhere?
*/ 
unsigned char base64_to_binary(unsigned char c) {
  // Capital letters - 'A' is ascii 65 and base64 0
  if('A' <= c && c <= 'Z') return c - 'A';

  // Lowercase letters - 'a' is ascii 97 and base64 26
  if('a' <= c && c <= 'z') return c - 71;

  // Digits - '0' is ascii 48 and base64 52
  if('0' <= c && c <= '9') return c + 4;

  // '+' is ascii 43 and base64 62
  if(c == '+') return 62;

  // '/' is ascii 47 and base64 63
  if(c == '/') return 63;

  return 255;
}

unsigned int decode_base64_length(unsigned char input[]) {
  unsigned char *start = input;

  while(base64_to_binary(input[0]) < 64) {
    ++input;
  }

  unsigned int input_length = input - start;

  unsigned int output_length = input_length/4*3;

  switch(input_length % 4) {
    default: return output_length;
    case 2: return output_length + 1;
    case 3: return output_length + 2;
  }
}

unsigned int decode_base64(unsigned char input[], unsigned char output[]) {
  unsigned int output_length = decode_base64_length(input);

  // While there are still full sets of 24 bits...
  for(unsigned int i = 2; i < output_length; i += 3) {
    output[0] = base64_to_binary(input[0]) << 2 | base64_to_binary(input[1]) >> 4;
    output[1] = base64_to_binary(input[1]) << 4 | base64_to_binary(input[2]) >> 2;
    output[2] = base64_to_binary(input[2]) << 6 | base64_to_binary(input[3]);

    input += 4;
    output += 3;
  }

  switch(output_length % 3) {
    case 1:
      output[0] = base64_to_binary(input[0]) << 2 | base64_to_binary(input[1]) >> 4;
      break;
    case 2:
      output[0] = base64_to_binary(input[0]) << 2 | base64_to_binary(input[1]) >> 4;
      output[1] = base64_to_binary(input[1]) << 4 | base64_to_binary(input[2]) >> 2;
      break;
  }

  return output_length;
}

#endif
