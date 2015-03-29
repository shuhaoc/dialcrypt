#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "global.h"
#include "fake_md5.h"
#include "d3des.h"
#include "base64.h"
#include "../include/dialcrypt.h"

extern char des_randpart_key[768];

extern char des_password_key[768];

void cumt_dial_get_entire_cipher(const char password[9], char cipher[117]) {
	char rand_str[19], md5_res[16], md5_hex[33], rand_part[77], password_part[13];
	unsigned char rand_verify[57], rand_des[56];
	MD5_CTX md5_ctx;
	int i;

	// ����18�ֽڷ�0��������ַ�
	srand((unsigned)time(NULL));
	for (i = 0; i < 18; i++) rand_str[i] = rand() % 9 + '1';
	rand_str[18] = 0;

	// ����MD5������HEX����
	MD5Init(&md5_ctx);
	FakeMD5Update(&md5_ctx, rand_str, 18);
	MD5Final(md5_res, &md5_ctx);

	for (i = 0; i < 16; i++) sprintf(md5_hex + i * 2, "%02X", md5_res[i] & 0xFF);

	// ƴ�����������֤�ַ���������DES����
	sprintf(rand_verify, 
			"%s%s%s",
			rand_str,
			"*3^F$k",
			md5_hex);

	importkey(des_randpart_key, EN0);
	for (i = 0; i < 7; i++) {
		des(rand_verify + i * 8, rand_des + i * 8);
	}

	memcpy(rand_part, base64_encode_bin(rand_des, 56), 76);
	rand_part[76] = 0;

	// �������벿��
	cumt_dial_encrypt_password_part(password, password_part);

	// ���ƴ����������
	sprintf(cipher,
			"%s%s%s%s",
			"824ef19fb7ff1afanji^3!",
			rand_part,
			"i0~!8^",
			password_part);
}

void cumt_dial_encrypt_password_part(const char password[9], char partial_cipher[13]) {
	unsigned char pwd_align[8], des_res[8];
	int len, i;

	// ������벻��8λ���Կո���
	len = strlen(password);
	memcpy(pwd_align, password, len);
	if (len < 8) {
		for (i = len; i < 8; i++) pwd_align[i] = ' ';
	}

	// DES����
	importkey(des_password_key, EN0);
	des(pwd_align, des_res);

	// BASE64����
	memcpy(partial_cipher, base64_encode_bin(des_res, 8), 12);
	partial_cipher[12] = 0;
}

void cumt_dial_decrypt_password_part(const char partial_cipher[13], char password[9]) {
	unsigned char des_res[8];

	// BASE64����
	memcpy(des_res, base64_decode(partial_cipher), 8);

	// DES����
	importkey(des_password_key, DE1);
	des(des_res, password);
	password[8] = 0;
}