/*
 * ��Ȩ���� (c) ��Ϊ�������޹�˾ 2014-2022
 * ��������: 2022��7��20��
 * ��������: ��mbedtls���У�����polarssl��API�ӿڡ�
 * ʹ�ñ����HUAWEI_POLARSSL_API_C���и��롣
 */
#ifndef HW_POLARSSL_API_H
#define HW_POLARSSL_API_H

#include "mbedtls/debug.h"
#include "mbedtls/x509_crt.h"
#include "mbedtls/ssl.h"
#include "mbedtls/platform.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/pem.h"
#include "mbedtls/des.h"
#include "mbedtls/ssl_cache.h"
#include "mbedtls/pkcs5.h"
#include "mbedtls/md5.h"
#include "mbedtls/sha1.h"
#include "mbedtls/dhm.h"
#include "mbedtls/oid.h"
#include "mbedtls/pk.h"

/* ��һģ�� ����cyassl��ʹ�÷�ʽ */
typedef struct {
    mbedtls_ssl_config * config;
    mbedtls_entropy_context * entropy;
    mbedtls_ctr_drbg_context * ctr_drbg;
    mbedtls_x509_crt * pubcert;
    mbedtls_pk_context *prv;
    mbedtls_x509_crt * chainHead; /* calist */
    mbedtls_ssl_cache_context *cache;
} mbedtls_ssl_config_adapt;

typedef struct {
    mbedtls_ssl_context *ssl;
    mbedtls_net_context bio;
    mbedtls_ssl_session *saved_session;
} mbedtls_ssl_adapt;

/* polarssl Ĭ��������hostname�ͷ���sni �˲�����Ҫ���� */
#define MAX_SNI_LEN 256
typedef struct {
    int send_sni;
    char hostname[MAX_SNI_LEN];
} mbedtls_ssl_sni;

#if !defined(CIVETWEB_SSL)
/* ���仪Ϊ���� */
#define POLAR_SSL mbedtls_ssl_adapt
#define POLAR_SSL_CFG mbedtls_ssl_config_adapt
#define POLAR_CRT  mbedtls_x509_crt
#define POLAR_PUBKEY  mbedtls_pk_context
#define POLAR_RSA mbedtls_rsa_context
#define POLAR_MD_CTX mbedtls_md_context_t
#define SSL_CTX POLAR_SSL_CFG
#define SSL POLAR_SSL
#define DH_KEY mbedtls_dhm_context
#define AES_KEY mbedtls_aes_context
#define PEM_CRT mbedtls_pem_context
#define POLAR_MPI mbedtls_mpi
#endif

/* ���仪Ϊ����ģʽ */
#define Polarssl_New_Ssl polarssl_new_ssl
#define Polarssl_New_Ctx_Ssl_Conf polarssl_new_ctx_ssl_conf
#define Polarssl_Ssl_Set_Bio polarssl_ssl_set_bio
#define Polarssl_Enable_Ca_Update polarssl_enable_ca_update
#define Polarssl_Ssl_Get_Ciphersuite polarssl_ssl_get_ciphersuite
#define Polarssl_Conf_SetCaList polarssl_conf_setCaList
#define Polarssl_Rsassl_Vertify polarssl_rsassl_vertify
#define Polarssl_Ssl_Get_Version polarssl_ssl_get_version
#define Polarssl_Ssl_Write polarssl_ssl_write
#define Polarssl_Ssl_Read polarssl_ssl_read
#define Polarssl_Set_Pub_Prv_To_Conf polarssl_set_pub_prv_to_conf
#define Polarssl_Pk_Parse_Subpubkey polarssl_pk_parse_subpubkey
#define Polarssl_X509_Crt_Parse_File polarssl_x509_crt_parse_file
#define Polarssl_Hmac_With_Sha256 polarssl_hmac_with_sha256
#define Polarssl_Conf_Authmode polarssl_conf_authmode
#define Polarssl_Free_Config_Adapt polarssl_free_config_adapt
#define Polarssl_Free polarssl_free
#define Polarssl_Accept polarssl_accept
#define Polarssl_Connect polarssl_connect
#define Polarssl_X509_Crt_Free polarssl_x509_crt_free
#define Polarssl_X509_Crt_Check_Date polarssl_x509_crt_check_date
#define Polarssl_Shutdown polarssl_shutdown
#define Polarssl_Set_Hostname polarssl_ssl_set_hostname
#define Polarssl_Dgst polarssl_dgst
#define Polarssl_Dgst_Init polarssl_dgst_init
#define Polarssl_Dgst_Update polarssl_dgst_update
#define Polarssl_Dgst_Final polarssl_dgst_final
#define Polarssl_Md_Hmac512 polarssl_md_hmac512
#define Polarssl_Md_Hmacsha1 polarssl_hmac_with_sha1
#define Polarssl_Sha512 polarssl_sha512
#define PolarssL_Pk_Free polarssl_pk_free
#define Polarssl_Dhm_Free polarssl_dhm_free
#define Polarssl_Dhm_Init polarssl_dhm_init
#define Polarssl_Mpi_Read_String polarssl_mpi_read_string
#define Polarssl_Mpi_Size polarssl_mpi_size
#define Polarssl_Dhm_Make_Public polarssl_dhm_make_public
#define Polarssl_Aes_Init polarssl_aes_init
#define Polarssl_Aes_SetKey_Dec polarssl_aes_setkey_dec
#define Polarssl_Aes_Crypt_Cbc polarssl_aes_crypt_cbc
#define Polarssl_Aes_SetKey_Enc polarssl_aes_setkey_enc
#define Polarssl_Mpi_Write_Binary polarssl_mpi_write_binary
#define Polarssl_Mpi_Read_Binary polarssl_mpi_read_binary
#define Polarssl_Dhm_Calc_Secret polarssl_dhm_calc_secret
#define Polarssl_Ssl_Conf_Ciphersuites polarssl_ssl_conf_ciphersuites
#define Polarssl_Pem_Read_Buffer polarssl_pem_read_buffer
#define Polarssl_Pem_Free polarssl_pem_free
#define Polarssl_Set_Sni polarssl_ssl_set_sni
#define Polarssl_SetIgnoreTime SetIgnoreTime

/* AES�ļ���ģ���СΪ16�ֽ� */
#define AES_BLOCK_SIZE 16

/* ����sni */
int polarssl_ssl_set_sni(mbedtls_ssl_adapt *ssl, const char *hostname);

/* hmac��װ��ժҪ����Ϊsha1 */
int polarssl_hmac_with_sha1(const unsigned char *key, size_t keylen,
                            const unsigned char *input, size_t ilen,
                            unsigned char *output,size_t *outlen);

/* free */
void polarssl_pem_free(mbedtls_pem_context *ctx);

/* ����PUBLIC KEY֤�� */
int polarssl_pem_read_buffer(mbedtls_pem_context *pem,unsigned char *pub);

/* ����Э��ʱ���㷨����,�ⲿ��ʹ�þ�̬���� */
void polarssl_ssl_conf_ciphersuites(mbedtls_ssl_config_adapt *conf, const int *ciphersuites);

/* Derive and export the shared secret (G^Y)^X mod P */
int polarssl_dhm_calc_secret(mbedtls_dhm_context *ctx,
                             unsigned char *output, size_t output_size, unsigned int *olen);

/* Import X from unsigned binary data, big endian */
int polarssl_mpi_read_binary(mbedtls_mpi *X, const unsigned char *buf, unsigned int buflen);

/* Export X into unsigned binary data, big endian */
int polarssl_mpi_write_binary(const mbedtls_mpi *X, unsigned char *buf, size_t buflen);

/* setAesKey */
int polarssl_aes_setkey_enc(mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits);

/* AES-CBC buffer encryption/decryption */
int polarssl_aes_crypt_cbc(mbedtls_aes_context *ctx, int mode, unsigned int length,
                           unsigned char iv[16], const unsigned char *input, unsigned char *output);

/* setAesKey */
int polarssl_aes_setkey_dec(mbedtls_aes_context *ctx, const unsigned char *key, unsigned int keybits);

/* init the para */
void polarssl_aes_init(mbedtls_aes_context *ctx);

/* Create own private value X and export G^X */
int polarssl_dhm_make_public(mbedtls_dhm_context *ctx, int x_size, unsigned char *output, size_t olen);

/* Return the total size in bytes */
size_t polarssl_mpi_size(const mbedtls_mpi *X);

/* Import from an ASCII string */
int polarssl_mpi_read_string(mbedtls_mpi *X, int radix, const char *s);

/* init the dh */
void polarssl_dhm_init(mbedtls_dhm_context *ctx);

/* Free the components of a DHM key */
void polarssl_dhm_free(mbedtls_dhm_context *ctx);

/* free pk */
void polarssl_pk_free(mbedtls_pk_context *ctx);

/* sha512 */
void polarssl_sha512(const unsigned char *input, size_t ilen, unsigned char output[64], int is384);

/* hmac512 */
int polarssl_md_hmac512(const unsigned char *key, size_t keylen, const unsigned char *input, size_t ilen,
                        unsigned char *output);

/* ���cyasslģ�ʹ���ssl���� */
mbedtls_ssl_adapt *polarssl_new_ssl(mbedtls_ssl_config_adapt * config);

/* ���cyassl����ģ�� ���亯�� */
mbedtls_ssl_config_adapt *polarssl_new_ctx_ssl_conf(int connect_type);

/* ��socket���õ�ssl������ */
void polarssl_ssl_set_bio(mbedtls_ssl_adapt *ssl, int fd);

/* scp�е�����ҵ������ */
void polarssl_enable_ca_update(int enable);

int polarssl_get_caupdate(void);

/* ��ȡssl������ʹ�õļ����㷨 */
const char *polarssl_ssl_get_ciphersuite(const mbedtls_ssl_adapt *ssl);

/* ��cafile��cadirĿ¼�µ������ļ����õ�conf��ȥ,û����Ч֤����ʧ�� */
int polarssl_conf_setCaList(mbedtls_ssl_config_adapt *conf, char *cafile, char *cadir, char *crlFile);

/* hash256 ժҪ�� x509 rsa֤��У���ж� */;
int polarssl_rsassl_vertify(unsigned char *sig, unsigned int siglen,
                            unsigned char *data, unsigned int datalen,
                            mbedtls_rsa_context	* rsa);
/* SSL��ȡversion */
const char *polarssl_ssl_get_version(const mbedtls_ssl_adapt *ssl);

/* SSL��write���� */
int polarssl_ssl_write(mbedtls_ssl_adapt *ssl, const unsigned char *buf, size_t len);

/* SSL��read���� */
int polarssl_ssl_read(mbedtls_ssl_adapt *ssl, unsigned char *buf, size_t len);

/* ��һ��pub/priv key���õ����� */
int polarssl_set_pub_prv_to_conf(mbedtls_ssl_config_adapt *conf, const char *prvPath,
                                 const char *pubPath, const char *prvPassword);

/* ��der��ʽ�Ĺ�Կ�н�����N/E rsa key��Ҫ��der��ʽ��������ֻ������һ�� */
int polarssl_pk_parse_subpubkey(unsigned char *buf, int len, mbedtls_pk_context *pk);

/* �����ļ�����crt/der��ʽ��֤�� */
mbedtls_x509_crt *polarssl_x509_crt_parse_file(const char *path);

/* hmac��װ��ժҪ����Ϊsha256 */
int polarssl_hmac_with_sha256(const unsigned char *key, size_t keylen,
                              const unsigned char *input, size_t ilen,
                              unsigned char *output, size_t outlen);
/* ������֤ģʽ */
void polarssl_conf_authmode(mbedtls_ssl_config_adapt *conf, int authmode);

/* SSL���ñ����ͷ� */
void polarssl_free_config_adapt(mbedtls_ssl_config_adapt *conf);

/* SSL free���� */
void polarssl_free(mbedtls_ssl_adapt *ssl);

/* SSL accept���� */
int polarssl_accept(mbedtls_ssl_adapt *ssl);

/* SSL accept���� */
int polarssl_connect(mbedtls_ssl_adapt *ssl);

/* free crt ֤�� */
void polarssl_x509_crt_free(mbedtls_x509_crt *crt);

/* У��֤�������Ƿ���Ч */
int polarssl_x509_crt_check_date(const char* filename);

/* ֪ͨ�Զ˹ر����� */
int polarssl_shutdown(mbedtls_ssl_adapt *ssl);

/* ����CN��������������У��,ͬʱ�ᷢ��SNI */
int polarssl_ssl_set_hostname(mbedtls_ssl_adapt *ssl, const char *hostname);

/* ժҪ�㷨�ϼ�ռʱ֧��md5/sha1 */
void polarssl_dgst(int type, unsigned char *input, size_t ilen, unsigned char *output, size_t *olen);

/* ����init/update/finalժҪ�㷨��ʽ�ϼ� */
int polarssl_dgst_init(mbedtls_md_context_t *ctx, int type);

/* ����init/update/finalժҪ�㷨��ʽ�ϼ� */
void polarssl_dgst_update(mbedtls_md_context_t *ctx, unsigned char *input, size_t ilen);

/* ����init/update/finalժҪ�㷨��ʽ�ϼ� */
void polarssl_dgst_final(mbedtls_md_context_t *ctx, unsigned char *output);

/* ���ò�У��ʱ���־ */
int SetIgnoreTime(int value);
int polarssl_get_checktime(void);

int polarssl_ischeck_crttime(mbedtls_x509_crt *crt);
/* ��Ʒ������Ƿ�ҪУ��֤��ʱ��ĺ�����ҪУ�鷵��1����У����������0 */
typedef int (*PdtCertTimeCheckFunc)(const mbedtls_x509_crt *crt);
int SetPdtCertTimeCheckFunc(PdtCertTimeCheckFunc func);

int polarssl_ischeck_crltime(mbedtls_x509_crl *crl);
/* ��Ʒ������Ƿ�ҪУ��CRLʱ��ĺ�����ҪУ�鷵��1����У����������0 */
typedef int (*PdtCrlTimeCheckFunc)(const mbedtls_x509_crl *crl);
int SetPdtCrlTimeCheckFunc(PdtCrlTimeCheckFunc func);

int x509_info_subject_alt_name(char **buf, size_t *size, const mbedtls_x509_sequence *subject_alt_name, const char *prefix );

/* ����dh���� */
int  polarssl_ssl_conf_dh_param_ctx(mbedtls_ssl_config_adapt *conf, mbedtls_dhm_context *ctx);

/* ���㹲����Կ */
int polarssl_ssl_calc_key(const unsigned char *pw, unsigned int pwLen, const unsigned char *salt, unsigned int saltLen,
    unsigned char *key, unsigned int keyLen);

/* ���ù�����Կ */
void polarssl_ssl_conf_psk(mbedtls_ssl_config_adapt *ctx, unsigned char *key, unsigned int keyLen,
    const unsigned char *ident, unsigned int identLen);

/* aes�ӽ��� */
int mbedtls_aescrypt2( int mode, const char *filein, const char *fileout, unsigned char *key, size_t keylen );

/* ʹ�ܷ���˻��� */
void polarssl_enable_server_cache(mbedtls_ssl_config_adapt *conf, uint32_t cache_num, uint32_t timeout);

/* ʹ�ܿͻ��˻��� */
void polarssl_enable_client_cache(mbedtls_ssl_adapt *ssl);

int polarssl_pk_parse_keyfile(mbedtls_pk_context *ctx, const char *path, const char *password);

int polarssl_pk_parse_key(mbedtls_pk_context *ctx, const unsigned char *key, size_t keylen,
    const unsigned char *pwd, size_t pwdlen);

int polarssl_pk_check_pair(const mbedtls_pk_context *pub, const mbedtls_pk_context *prv);

int polarss_feather_support(char *featureName);

int ssl_deprecated_kexsize_support(void);

void mbedtls_set_udm_ssl_conf(mbedtls_ssl_config_adapt *config);

// ��x509_crt.h���ƹ�����API
int mbedtls_x509_crt_check_date(const char* filename);

// ��pk.h���ƹ�����API
int mbedtls_pk_write_crypt_key_pem(mbedtls_pk_context *key, const unsigned char *pwd, unsigned char *buf, size_t size);

// ��pem.h���ƹ�����API
int mbedtls_pem_write_crypt_buffer(const char *header, const char *footer,
                                   unsigned char *der_data, size_t der_len,
                                   const unsigned char * pwd, size_t pwdlen, 
                                   unsigned char *buf, size_t buf_len, size_t *olen);

/*?????? md2,md4?????? */
int polarssl_dgst_check(mbedtls_md_type_t md_alg);

#endif
