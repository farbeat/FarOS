ifeq ($(CONFIG_LLVM), y)
	CLANG_CROSS += LLVM=1 LLVM_IAS=1
	CC:=clang
	AR:=llvm-ar
	STRIP:=llvm-strip
	OBJCOPY:=llvm-objcopy
else
	CC:=$(CROSS)gcc
	AR:=$(CROSS)ar
	STRIP:=$(CROSS)strip
	OBJCOPY:=$(CROSS)objcopy
endif

MBED_ROOT_PATH = ../../..
MBED_LIB_PATH = $(MBED_ROOT_PATH)/library

SHARED = y

GENERATED_FILES := \
	error.c version_features.c \
	ssl_debug_helpers_generated.c \
	psa_crypto_driver_wrappers.h \
	psa_crypto_driver_wrappers_no_static.c

CFLAGS	?= -O2
WARNING_CFLAGS ?= -Wall -Wextra -Wformat=2 -Wno-format-nonliteral
LDFLAGS ?=

CFLAGS += -DCRYPTO_SOFT_HMAC_SUPPORT
CFLAGS += -DMBEDTLS_USER_CONFIG_FILE='"mbedtls_platform_hardware_config.h"'

# if you want to view the log, open this macro
# CFLAGS += -DMBED_HARDEN_DEBUG

LOCAL_CFLAGS = $(WARNING_CFLAGS) -I$(MBED_LIB_PATH) -I$(MBED_ROOT_PATH)/include -D_FILE_OFFSET_BITS=64

CFLAGS  += -I$(MBED_ROOT_PATH)/harden/src/internal_include
CFLAGS  += -I$(MBED_ROOT_PATH)/harden/platform/vision
CFLAGS  += -I$(MBED_ROOT_PATH)/harden/cipher_adapt_include

ifdef DEBUG
LOCAL_CFLAGS += -g3
endif

PERL ?= perl

PYTHON ?= $(shell if type python3 >/dev/null 2>/dev/null; then echo python3; else echo python; fi)

LOCAL_CFLAGS += -fPIC -fpic

SOEXT_TLS?=so.21
SOEXT_X509?=so.7
SOEXT_CRYPTO?=so.16

# Set AR_DASH= (empty string) to use an ar implementation that does not accept
# the - prefix for command line options (e.g. llvm-ar)
AR_DASH ?= -

ARFLAGS = $(AR_DASH)src

OBJS_CRYPTO= \
		 $(MBED_LIB_PATH)/aes.o \
	     $(MBED_LIB_PATH)/aesni.o \
		 $(MBED_LIB_PATH)/aesce.o \
	     $(MBED_LIB_PATH)/aria.o \
	     $(MBED_LIB_PATH)/asn1parse.o \
	     $(MBED_LIB_PATH)/asn1write.o \
	     $(MBED_LIB_PATH)/base64.o \
	     $(MBED_LIB_PATH)/bignum_core.o \
	     $(MBED_LIB_PATH)/bignum_mod.o \
	     $(MBED_LIB_PATH)/bignum_mod_raw.o \
	     $(MBED_LIB_PATH)/block_cipher.o \
		 $(MBED_LIB_PATH)/bignum.o \
	     $(MBED_LIB_PATH)/camellia.o \
	     $(MBED_LIB_PATH)/ccm.o \
	     $(MBED_LIB_PATH)/chacha20.o \
	     $(MBED_LIB_PATH)/chachapoly.o \
	     $(MBED_LIB_PATH)/cipher.o \
	     $(MBED_LIB_PATH)/cipher_wrap.o \
	     $(MBED_LIB_PATH)/cmac.o \
	     $(MBED_LIB_PATH)/constant_time.o \
	     $(MBED_LIB_PATH)/ctr_drbg.o \
	     $(MBED_LIB_PATH)/des.o \
	     $(MBED_LIB_PATH)/dhm.o \
	     $(MBED_LIB_PATH)/ecdh.o \
	     $(MBED_LIB_PATH)/ecdsa.o \
	     $(MBED_LIB_PATH)/ecjpake.o \
		 $(MBED_LIB_PATH)/ecp.o \
	     $(MBED_LIB_PATH)/ecp_curves.o \
	     $(MBED_LIB_PATH)/ecp_curves_new.o \
	     $(MBED_LIB_PATH)/entropy.o \
	     $(MBED_LIB_PATH)/entropy_poll.o \
	     $(MBED_LIB_PATH)/error.o \
	     $(MBED_LIB_PATH)/gcm.o \
	     $(MBED_LIB_PATH)/hkdf.o \
	     $(MBED_LIB_PATH)/hmac_drbg.o \
	     $(MBED_LIB_PATH)/lmots.o \
	     $(MBED_LIB_PATH)/lms.o \
		 $(MBED_LIB_PATH)/md.o \
	     $(MBED_LIB_PATH)/md5.o \
	     $(MBED_LIB_PATH)/memory_buffer_alloc.o \
	     $(MBED_LIB_PATH)/nist_kw.o \
	     $(MBED_LIB_PATH)/oid.o \
	     $(MBED_LIB_PATH)/padlock.o \
	     $(MBED_LIB_PATH)/pem.o \
	     $(MBED_LIB_PATH)/pk.o \
	     $(MBED_LIB_PATH)/pk_ecc.o \
	     $(MBED_LIB_PATH)/pk_wrap.o \
		 $(MBED_LIB_PATH)/pkcs5.o \
	     $(MBED_LIB_PATH)/pkcs12.o \
	     $(MBED_LIB_PATH)/pkparse.o \
	     $(MBED_LIB_PATH)/pkwrite.o \
	     $(MBED_LIB_PATH)/platform.o \
	     $(MBED_LIB_PATH)/platform_util.o \
	     $(MBED_LIB_PATH)/poly1305.o \
	     $(MBED_LIB_PATH)/psa_crypto.o \
	     $(MBED_LIB_PATH)/psa_crypto_aead.o \
	     $(MBED_LIB_PATH)/psa_crypto_cipher.o \
	     $(MBED_LIB_PATH)/psa_crypto_client.o \
	     $(MBED_LIB_PATH)/psa_crypto_driver_wrappers_no_static.o \
	     $(MBED_LIB_PATH)/psa_crypto_ecp.o \
	     $(MBED_LIB_PATH)/psa_crypto_ffdh.o \
	     $(MBED_LIB_PATH)/psa_crypto_hash.o \
	     $(MBED_LIB_PATH)/psa_crypto_mac.o \
	     $(MBED_LIB_PATH)/psa_crypto_pake.o \
	     $(MBED_LIB_PATH)/psa_crypto_rsa.o \
	     $(MBED_LIB_PATH)/psa_crypto_se.o \
	     $(MBED_LIB_PATH)/psa_crypto_slot_management.o \
	     $(MBED_LIB_PATH)/psa_crypto_storage.o \
	     $(MBED_LIB_PATH)/psa_its_file.o \
	     $(MBED_LIB_PATH)/psa_util.o \
	     $(MBED_LIB_PATH)/ripemd160.o \
	     $(MBED_LIB_PATH)/rsa.o \
	     $(MBED_LIB_PATH)/rsa_alt_helpers.o \
	     $(MBED_LIB_PATH)/sha1.o \
	     $(MBED_LIB_PATH)/sha256.o \
	     $(MBED_LIB_PATH)/sha512.o \
	     $(MBED_LIB_PATH)/sha3.o \
	     $(MBED_LIB_PATH)/threading.o \
	     $(MBED_LIB_PATH)/timing.o \
	     $(MBED_LIB_PATH)/version.o \
	     $(MBED_LIB_PATH)/version_features.o \
	     # This line is intentionally left blank

THIRDPARTY_INCLUDES+=-I$(MBED_ROOT_PATH)/3rdparty/everest/include \
	-I$(MBED_ROOT_PATH)/3rdparty/everest/include/everest \
	-I$(MBED_ROOT_PATH)/3rdparty/everest/include/everest/kremlib

THIRDPARTY_CRYPTO_OBJECTS+= \
	$(MBED_ROOT_PATH)/3rdparty/everest/library/everest.o \
	$(MBED_ROOT_PATH)/3rdparty/everest/library/x25519.o \
	$(MBED_ROOT_PATH)/3rdparty/everest/library/Hacl_Curve25519_joined.o

HARDEN_OBJECTS += $(MBED_ROOT_PATH)/harden/src/gcm_harden.o
HARDEN_OBJECTS += $(MBED_ROOT_PATH)/harden/src/cmac_harden.o
HARDEN_OBJECTS += $(MBED_ROOT_PATH)/harden/src/dfx.o
HARDEN_OBJECTS += $(MBED_ROOT_PATH)/harden/src/bignum_harden.o
HARDEN_OBJECTS += $(MBED_ROOT_PATH)/harden/src/entropy_harden.o
HARDEN_OBJECTS += $(MBED_ROOT_PATH)/harden/src/pbkdf2_hmac_harden.o
HARDEN_OBJECTS += $(MBED_ROOT_PATH)/harden/src/ecc_harden_common.o
HARDEN_OBJECTS += $(MBED_ROOT_PATH)/harden/src/ecc_harden.o
HARDEN_OBJECTS += $(MBED_ROOT_PATH)/harden/src/hash_harden_common.o
HARDEN_OBJECTS += $(MBED_ROOT_PATH)/harden/src/cipher_common.o
HARDEN_OBJECTS += $(MBED_ROOT_PATH)/harden/src/cipher_adapt.o
HARDEN_OBJECTS += $(MBED_ROOT_PATH)/harden/src/sha1_harden.o
HARDEN_OBJECTS += $(MBED_ROOT_PATH)/harden/src/sha256_harden.o
HARDEN_OBJECTS += $(MBED_ROOT_PATH)/harden/src/sha512_harden.o
HARDEN_OBJECTS += $(MBED_ROOT_PATH)/harden/src/aes_alt.o

CFLAGS += $(THIRDPARTY_INCLUDES)
OBJS_CRYPTO  += $(THIRDPARTY_CRYPTO_OBJECTS)
OBJS_CRYPTO  += $(HARDEN_OBJECTS)

OBJS_X509= \
	   $(MBED_LIB_PATH)/x509.o \
	   $(MBED_LIB_PATH)/x509_create.o \
	   $(MBED_LIB_PATH)/x509_crl.o \
	   $(MBED_LIB_PATH)/x509_crt.o \
	   $(MBED_LIB_PATH)/x509_csr.o \
	   $(MBED_LIB_PATH)/x509write.o \
	   $(MBED_LIB_PATH)/x509write_crt.o \
	   $(MBED_LIB_PATH)/x509write_csr.o \
	   $(MBED_LIB_PATH)/pkcs7.o
	   # This line is intentionally left blank

OBJS_TLS= \
	  $(MBED_LIB_PATH)/debug.o \
	  $(MBED_LIB_PATH)/mps_reader.o \
	  $(MBED_LIB_PATH)/mps_trace.o \
	  $(MBED_LIB_PATH)/net_sockets.o \
	  $(MBED_LIB_PATH)/ssl_cache.o \
	  $(MBED_LIB_PATH)/ssl_ciphersuites.o \
	  $(MBED_LIB_PATH)/ssl_client.o \
	  $(MBED_LIB_PATH)/ssl_cookie.o \
	  $(MBED_LIB_PATH)/ssl_debug_helpers_generated.o \
	  $(MBED_LIB_PATH)/ssl_msg.o \
	  $(MBED_LIB_PATH)/ssl_ticket.o \
	  $(MBED_LIB_PATH)/ssl_tls.o \
	  $(MBED_LIB_PATH)/ssl_tls12_client.o \
	  $(MBED_LIB_PATH)/ssl_tls12_server.o \
	  $(MBED_LIB_PATH)/ssl_tls13_keys.o \
	  $(MBED_LIB_PATH)/ssl_tls13_client.o \
	  $(MBED_LIB_PATH)/ssl_tls13_server.o \
	  $(MBED_LIB_PATH)/ssl_tls13_generic.o \
	  # This line is intentionally left blank

.SILENT:

.PHONY: all static shared clean

ifndef SHARED
all: static
else
all: shared static
	if [ ! -d "out" ]; then mkdir out; fi
	mv libmbedtls.*    out
	mv libmbedx509.*   out
	mv libmbedcrypto.* out
endif

static: libmbedcrypto.a libmbedx509.a libmbedtls.a
	cd $(MBED_LIB_PATH)/../tests && echo "This is a seedfile that contains 64 bytes (65 on Windows)......" > seedfile

shared: libmbedcrypto.so libmbedx509.so libmbedtls.so

# tls
libmbedtls.a: $(OBJS_TLS)
	echo "  AR    $@"
	$(AR) $(ARFLAGS) $@ $(OBJS_TLS)

libmbedtls.$(SOEXT_TLS): $(OBJS_TLS) libmbedx509.so
	echo "  LD    $@"
	$(CC) -shared -Wl,-soname,$@ -o $@ $(OBJS_TLS) -L. -lmbedx509 -lmbedcrypto $(LDFLAGS)

ifneq ($(SOEXT_TLS),so)
libmbedtls.so: libmbedtls.$(SOEXT_TLS)
	echo "  LN    $@ -> $<"
	ln -sf $< $@
endif

# x509
libmbedx509.a: $(OBJS_X509)
	echo "  AR    $@"
	$(AR) $(ARFLAGS) $@ $(OBJS_X509)

libmbedx509.$(SOEXT_X509): $(OBJS_X509) libmbedcrypto.so
	echo "  LD    $@"
	$(CC) -shared -Wl,-soname,$@ -o $@ $(OBJS_X509) -L. -lmbedcrypto $(LDFLAGS)

ifneq ($(SOEXT_X509),so)
libmbedx509.so: libmbedx509.$(SOEXT_X509)
	echo "  LN    $@ -> $<"
	ln -sf $< $@
endif

# crypto
libmbedcrypto.a: $(OBJS_CRYPTO)
	echo "  AR    $@"
	$(AR) $(ARFLAGS) $@ $(OBJS_CRYPTO)

libmbedcrypto.$(SOEXT_CRYPTO): $(OBJS_CRYPTO)
	echo "  LD    $@"
	$(CC) -shared -Wl,-soname,$@ -o $@ $(OBJS_CRYPTO) $(LDFLAGS)

ifneq ($(SOEXT_CRYPTO),so)
libmbedcrypto.so: libmbedcrypto.$(SOEXT_CRYPTO)
	echo "  LN    $@ -> $<"
	ln -sf $< $@
endif

.c.o:
	echo "  CC    $<"
	$(CC) $(LOCAL_CFLAGS) $(CFLAGS) -o $@ -c $<

.PHONY: generated_files

generated_files: $(GENERATED_FILES)

# See root Makefile
GEN_FILES ?=
ifdef GEN_FILES
gen_file_dep =
else
gen_file_dep = |
endif

error.c:  $(gen_file_dep) $(MBED_ROOT_PATH)/scripts/generate_errors.pl
error.c:  $(gen_file_dep) $(MBED_ROOT_PATH)/scripts/data_files/error.fmt
error.c:  $(gen_file_dep) $(filter-out %config%,$(wildcard $(MBED_ROOT_PATH)/include/mbedtls/*.h))
error.c:
	echo "  Gen   $@"
	$(PERL) $(MBED_ROOT_PATH)/scripts/generate_errors.pl

ssl_debug_helpers_generated.c: $(gen_file_dep) $(MBED_ROOT_PATH)/scripts/generate_ssl_debug_helpers.py
ssl_debug_helpers_generated.c: $(gen_file_dep) $(filter-out %config%,$(wildcard $(MBED_ROOT_PATH)/include/mbedtls/*.h))
ssl_debug_helpers_generated.c:
	echo "  Gen   $@"
	$(PYTHON) $(MBED_ROOT_PATH)/scripts/generate_ssl_debug_helpers.py --mbedtls-root $(MBED_ROOT_PATH) $(MBED_LIB_PATH)

version_features.c: $(gen_file_dep) $(MBED_ROOT_PATH)/scripts/generate_features.pl
version_features.c: $(gen_file_dep) $(MBED_ROOT_PATH)/scripts/data_files/version_features.fmt
## The generated file only depends on the options that are present in mbedtls_config.h,
## not on which options are set. To avoid regenerating this file all the time
## when switching between configurations, don't declare mbedtls_config.h as a
## dependency. Remove this file from your working tree if you've just added or
## removed an option in mbedtls_config.h.
#version_features.c: $(MBED_ROOT_PATH)/include/mbedtls/mbedtls_config.h
version_features.c:
	echo "  Gen   $@"
	$(PERL) $(MBED_ROOT_PATH)/scripts/generate_features.pl

GENERATED_WRAPPER_FILES = \
                    $(MBED_LIB_PATH)/psa_crypto_driver_wrappers.h \
                    $(MBED_LIB_PATH)/psa_crypto_driver_wrappers_no_static.c
$(GENERATED_WRAPPER_FILES): $(MBED_ROOT_PATH)/scripts/generate_driver_wrappers.py
$(GENERATED_WRAPPER_FILES): $(MBED_ROOT_PATH)/scripts/data_files/driver_templates/psa_crypto_driver_wrappers.h.jinja
$(GENERATED_WRAPPER_FILES): $(MBED_ROOT_PATH)/scripts/data_files/driver_templates/psa_crypto_driver_wrappers_no_static.c.jinja
$(GENERATED_WRAPPER_FILES):
	echo "  Gen   $(GENERATED_WRAPPER_FILES)"
	$(PYTHON) $(MBED_ROOT_PATH)/scripts/generate_driver_wrappers.py

psa_crypto.o:$(MBED_LIB_PATH)/psa_crypto_driver_wrappers.h

clean:
	rm -f *.o libmbed*
	rm -f $(THIRDPARTY_CRYPTO_OBJECTS)
	rm -f $(OBJS_CRYPTO)
	rm -f $(OBJS_X509)
	rm -f $(OBJS_TLS)
	rm -f out/*
	rm -rf $(MBED_ROOT_PATH)/harden/src/*.o
	rm -rf $(HARDEN_OBJECTS)

neat: clean
	rm -f $(GENERATED_FILES)