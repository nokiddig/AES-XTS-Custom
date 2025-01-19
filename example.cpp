/* This program is released under the Common Public License V1.0
 *
 * You should have received a copy of Common Public License V1.0 along with
 * with this program.
 *
 * Copyright IBM Corp. 2016
 *
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <ica_api.h>

#define AES_CIPHER_BLOCK_SIZE  16

/* This example uses a static keys. In real life you would
 * use real AES keys, which is negotiated between the
 * encrypting and the decrypting entity.
 *
 * Note: AES-128 key size is 16 bytes (AES_KEY_LEN128)
 */
unsigned char aes_xts_key1[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
};
unsigned char aes_xts_key2[] = {
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
};

/* This is the plain data, you want to encrypt. For the
 * encryption mode used in this example, it is necessary,
 * that the length of the encrypted data is at least as
 * large as the AES cipher block size (AES_CIPHER_BLOCK_SIZE),
 * but it does not have to be a multiple of the cipher block size.
 */
unsigned char plain_data[] = {
	0x55, 0x73, 0x69, 0x6e, 0x67, 0x20, 0x6c, 0x69,
	0x62, 0x69, 0x63, 0x61, 0x20, 0x77, 0x69, 0x74,
	0x68, 0x20, 0x41, 0x45, 0x53, 0x2d, 0x58, 0x54,
	0x53, 0x20, 0x69, 0x73, 0x20, 0x73, 0x6d, 0x61,
	0x72, 0x74, 0x20, 0x61, 0x6e, 0x64, 0x20, 0x65,
	0x61, 0x73, 0x79, 0x21, 0x00
};

/* Prints hex values to standard out. */
static void dump_data(unsigned char *data, unsigned long length);
/* Prints a description of the return value to standard out. */
static int handle_ica_error(int rc);

int main(char **argv, int argc)
{
	int rc;

	/* This is the AES XTS tweak value.
	 * We are generating it per random number generator. In real life
	 * you would use an tweak value which is negotiated between the
	 * encrypting and the decrypting entity.
	 */
	unsigned char random_tweak_value[AES_CIPHER_BLOCK_SIZE];

	/* Since libica function ica_aes_xts updates the tweak value
	 * we let ica_aes_xts work on a copy of the generated tweak
	 * value. We will need the original tweak value for decrypting
	 * the data later on.
	 */
    unsigned char tweak_value[AES_CIPHER_BLOCK_SIZE];

	unsigned char cipher_data[sizeof(plain_data)];
	unsigned char decrypt_data[sizeof(plain_data)];

	/* Generate the tweak value by random */
	rc = ica_random_number_generate(sizeof(random_tweak_value),
	                                random_tweak_value);

	/* Error handling (if necessary). */
	if (rc)
		return handle_ica_error(rc);

	/* Dump keys, tweak value and plain data to standard output, just for
	 * a visual control.
	 */
	printf("AES key1:\n");
	dump_data(aes_xts_key1, sizeof(aes_xts_key1));
	printf("AES key2:\n");
	dump_data(aes_xts_key2, sizeof(aes_xts_key2));
	printf("TWEAK:\n");
	dump_data(random_tweak_value, sizeof(random_tweak_value));
	printf("plain data:\n");
	dump_data(plain_data, sizeof(plain_data));

	/* Copy the generated tweak value so that we still
	 * have the original one available after the call to ica_aes_xts.
	 */
	memcpy(tweak_value,random_tweak_value,sizeof(tweak_value));

	/* Encrypt plain data to cipher data, using libica API.
	 */
	rc = ica_aes_xts(plain_data, cipher_data, sizeof(plain_data),
			 aes_xts_key1, aes_xts_key2, AES_KEY_LEN128, tweak_value,
			 ICA_ENCRYPT);

	/* Error handling (if necessary). */
	if (rc)
		return handle_ica_error(rc);

	/* Dump encrypted data.
	 */
	printf("encrypted data:\n");
	dump_data(cipher_data, sizeof(plain_data));

	/* Get the original tweak value, because ica_aes_xts
	 * has modified the tweak_value variable on encryption.
	 */
	memcpy(tweak_value,random_tweak_value,sizeof(tweak_value));

	/* Decrypt cipher data to decrypted data, using libica API.
	 * Note: The same AES keys and tweak value must be used for
	 * encryption and decryption.
	 */
	rc = ica_aes_xts(cipher_data, decrypt_data, sizeof(plain_data),
			 aes_xts_key1, aes_xts_key2, AES_KEY_LEN128, tweak_value,
			 ICA_DECRYPT);

	/* Error handling (if necessary). */
	if (rc)
		return handle_ica_error(rc);

	/* Dump decrypted data.
	 * Note: Please compare output with the plain data, they are the same.
	 */
	printf("decrypted data:\n");
	dump_data(decrypt_data, sizeof(plain_data));

	/* Surprise... :-)
	 * Note: The following will only work in this example!
	 */
	printf("%s\n", decrypt_data);
}

static void dump_data(unsigned char *data, unsigned long length)
{
	unsigned char *ptr;
	int i;

	for (ptr = data, i = 1; ptr < (data+length); ptr++, i++) {
		printf("0x%02x ", *ptr);
		if ((i % AES_CIPHER_BLOCK_SIZE) == 0)
			printf("\n");
	}
	if (i % AES_CIPHER_BLOCK_SIZE)
		printf("\n");
}

static int handle_ica_error(int rc)
{
	switch (rc) {
	case 0:
		printf("OK\n");
		break;
	case EINVAL:
		printf("Incorrect parameter.\n");
		break;
	case EPERM:
		printf("Operation not permitted by Hardware (CPACF).\n");
		break;
	case EIO:
		printf("I/O error.\n");
		break;
	default:
		printf("unknown error.\n");
	}

	return rc;
}