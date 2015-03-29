/**
 * 为base64.c编写的头文件，以便外部调用
 */

#pragma once

const char* base64_encode(const char *decoded_str);

const char* base64_encode_bin(const char *data, int len);

char* base64_decode(const char *p);
