/**
 * Ϊbase64.c��д��ͷ�ļ����Ա��ⲿ����
 */

#pragma once

const char* base64_encode(const char *decoded_str);

const char* base64_encode_bin(const char *data, int len);

char* base64_decode(const char *p);
