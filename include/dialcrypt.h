/**
 * �й���ҵ��ѧУ԰�����֤�����㷨��2011���
 * coder: shuhaoc
 * homepage: shuhaoc.cnblogs.cn
 * see also: 
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * ��ȡ��֤����
 * @param password	���룬���8�ֽڣ������ֽڵ���Ϊ�ո�
 * @param cipher	������֤��116�ֽ�����
 */
void cumt_dial_get_entire_cipher(const char password[9], char cipher[117]);

/**
 * DES�����û����룬��Ϊ��֤������һ���֣������ṩ��������Լ�����Ľ��ܺ�����Ϊ�˷��㱣������
 * @param password			���룬���8�ֽڣ������ֽڵ���Ϊ�ո�
 * @param partial_cipher	BASE64��ʽ��DES���ģ�12�ֽ�
 */
void cumt_dial_encrypt_password_part(const char password[9], char partial_cipher[13]);

/**
 * DES�����û����룬��Ҫ���ڲ���Ŀ��Ӵ
 * @param partial_cipher	BASE64��ʽ��DES���ģ�12�ֽ�
 * @param password			�û����룬8�ֽڣ�β���Ŀո��������
 */
void cumt_dial_decrypt_password_part(const char partial_cipher[13], char password[9]);


#ifdef __cplusplus
} // extern "C"
#endif

#ifdef WIN32
#pragma warning(disable: 4996)
#endif