/**
 * 中国矿业大学校园宽带认证加密算法，2011年版
 * coder: shuhaoc
 * homepage: shuhaoc.cnblogs.cn
 * see also: 
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 获取认证密文
 * @param password	密码，最多8字节，不足字节的视为空格
 * @param cipher	用于认证的116字节密文
 */
void cumt_dial_get_entire_cipher(const char password[9], char cipher[117]);

/**
 * DES加密用户密码，作为认证密文中一部分，单独提供这个函数以及下面的解密函数是为了方便保存密码
 * @param password			密码，最多8字节，不足字节的视为空格
 * @param partial_cipher	BASE64格式的DES密文，12字节
 */
void cumt_dial_encrypt_password_part(const char password[9], char partial_cipher[13]);

/**
 * DES解密用户密码，不要用于不良目的哟
 * @param partial_cipher	BASE64格式的DES密文，12字节
 * @param password			用户密码，8字节，尾部的空格可以无视
 */
void cumt_dial_decrypt_password_part(const char partial_cipher[13], char password[9]);


#ifdef __cplusplus
} // extern "C"
#endif

#ifdef WIN32
#pragma warning(disable: 4996)
#endif