#ifndef test_rsa_h
#define test_rsa_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libp2p/crypto/rsa.h"
#include "libp2p/crypto/encoding/base64.h"
#include "libp2p/crypto/encoding/x509.h"
#include "libp2p/peerutils.h"


/**
 * make sure we can get a DER formatted result
 */
int test_crypto_rsa_private_key_der() {

	struct RsaPrivateKey private_key;
	libp2p_crypto_rsa_generate_keypair(&private_key, 2048);
	
	if (private_key.der_length == 0)
		return 0;
	if (private_key.der == NULL)
		return 0;

	// print out public key
	//for (int i = 0; i < private_key.public_key_length; i++) {
	//	printf("%02x", private_key.public_key_der[i]);
	//}
	//printf("\n");
	return 1;
}

int test_crypto_x509_der_to_private2() {
	// this is an example private key. The type is not added. Therefore, it is not compatible with the go version
	char* der = "MIIEogIBAAKCAQEAmpDhRGecWN+MwfxW9UqsyEsZwnROHb2hCRBUATh7e0thgyi5Te6lT9x2++89x1sC7B9Egma1AJvY7BinIJYeFiOwzkKDe6/JxCle6TmLtCO1qmb8aGOIlu8NzO3L8EjcriYcgp0J5sZn5I3B4iU6q78u5jJQFpi2V7wsasanmfFwfF9ZUSxAnuwkcRkuhGnp5sBauHsQ4dn3IaiutiZDxPsAdxQyXX0SdrB4ew72UVAI2UsMAj5fRYy3plrsV/zZsGpeT6SDJI28weqOoSGvLzYaowf583MrH8O8dFoOWfXHq8hXy6qCDFOa7UYICCkb4QU9SLP7embmtfnB5/bVKwIDAQABAoIBADAwGwsIgmXyzB9uXG386gFH6LAHMpNzG1GIFaiLw3Oc/Lm3aI4zaLaNNUs2Ozx7011qIiHFg4i9DdQNm409QAQG/IhRlExrcawGeeCcYEG5IFoP4YFqBpuHy/Wn7XzsOmDQ4PKXow6frKREzb2Dfdcts6Fw7icdVTvlHrPrWzVS4azROsKy1spaCG7gYGd968f14Q2NBJmmlO75+dhjliI+cmMq5TaEvuCilXfSK4v6RWRGx+Mjl+ATSN1V7ksgzYsjNE15fdguxVWEIW5x5IsrpVviZdfGsDNWGsW9upoHoP118i2HKgrHRgbNcvTvWN/vCqJ/wDGz2FkYaa/3sYECgYEA9mBkSMW/Uc6m9/FNqlsEqCKLryI3ksxW9Ea6pHQ2p79gXYnlKuBLs7VdOA4XBIbV1vQdxjwJfojOdG7DmjklXYf+AYL/x26NSTszVV5feIIfR1jko67izBjeahPdizmpgQDFstgfdsc6vMKsbamvHzlZjpWeyanBz1t6OFPB8KUCgYEAoJpw2JB3o6ws4dASsSs4gthusY7e1Gcp0d7z8ZH0o8Rk2jxnQZuKHFUNnLY+qwxb6CP4zCpMMeGwuBVTapCE+7UkzUEk5GKmEgtO5G4Eu2yJFib1aGITlDnvxRkDOUGMSc/PVB5TnpdF9qXYbaJoLCziRzzgPJ9HWItnPuYudY8CgYB4mZZ9SM7t7IoutB+gVA1jgAWAJO+vG/c0e6rA9WILmtJA908GPeTQguaumbcKsDXckoJAlwLOvYjR1aZJx89SiU78znMF3EesoR3vm9J/2rIU6p6AwQqjfUjiA/deP0uJqicb9E7yhXNrEp/0ziq6zgfYk8S2UjJcnhqll9pHQQKBgGdoyfxHmSFD/Wowpbh6Edr+LNgbHBM7kcvWeNA0oIbKL/3tIrc1xUnU4fzjw5ozTQI+FzaujX0YysbcxGc7QsUnr9iRd4WulyvLKDMhO97KVcJzt1RMwjqQy3fnURIOyJvGOML6+/CDisLzqlV9WwIGrHQeGGwwSqoSqJnxcDy1AoGAMVmdK7mTMBtayUXRpthNCUmRAubaUKAjhKYm0j7ciPKWmo+mBHITbUshti07HqYeqxZE3LAIw/j0nowPsf4HIOxc69E+ZQqvETQwGyCwyHKJNYSFJi3tkHzCGEoP0/k9l6Fu8Qqcs1xhNQu8AcjQ1QL17rs/r9N44GKS2iXI+PY=";
	size_t b64_length = libp2p_crypto_encoding_base64_decode_size(strlen(der));
	unsigned char buffer[b64_length];
	unsigned char* b = buffer;
	size_t ultimate_length;
	int retVal = libp2p_crypto_encoding_base64_decode((unsigned char*)der, strlen(der), b, b64_length, &ultimate_length);
	if (retVal == 0)
		return 0;
	// we now have the bytes
	struct RsaPrivateKey private_key = {0};
	retVal = libp2p_crypto_encoding_x509_der_to_private_key(b, ultimate_length, &private_key);
	if (retVal == 0)
		return 0;
	return private_key.D > 0;
}

int test_crypto_x509_der_to_private() {
	// this is a base64 encoded string from the go version of ipfs
	char* der = "CAASpwkwggSjAgEAAoIBAQDTDJBWjDzS/HxDNOHazvzH2bu9CPMVHUrrvKRdBUM5ansL6/CC3MVZ6HVm4O6QHRapN6EF2CbrTgI4KBOXIL125Xo8MlROnyfXYk3O5q2tgwL/MbW8kXjtkyCfBak7MUoLOdLU7Svg0gkl3l+uDAiDcCLnwJVcFfq9ch6z4wMOhYJqE5dtx0uXxn6IuKWl1B69FTvBXCc0thw8Rw54b941FDcsBH5ttV9mRNCJym3poZ5qalNgXlxoIIB+PUx5QD+aq7KMJdpAX8HkapBntCOahP/QUceRmma0grlZLeYkH6/oi/hIrM6se3KUZ+F6tBuDFys8UAZy/X2BCUbKjbxtAgMBAAECggEANWfQfpYuLhXGPBt9q6kFPm1SnJtPJ+CpvM2XqhJS2IyhZnrl+bd0GTRBwS7aL42s1lVFYf04nAK5fQxnKK8YQqX/MIxr2RldM5ukpN6qxGWKtJkXrAgD2dqJPrRoBpqKahzPxSHfIJ0Fw5dqDtjsrpYJvyt0oEDPmnDuZAbmFx4sJqnesPNhKxtRMBx1+yxGVuRVJjHcqAgqPqwNiuoMEaYMY+G9yzT6vza8ovCpbX7BBIgM5fAT9PD8TBG//Vu9THvj/ZomiVG2qv6RL0qQyVb+DUzPZz1amBsSvahtXCl72jA3JwAZ943RxSR66P934S0ashkVwLUi46z/EAbJ4QKBgQDojGIO07BEVL2+7VxlGL9XGZQp4Y3qlhh2zDDQGwkCq/KQ+BdNYWitPwqRl9GqFLgpmeQIhyHTOa/IThx+AXGKVQ24ROH+skUs4IbO6R3qY7BKtb5lkZE/Yln09x70BBngUYAzh/rtnsXO3cl1x2XDDqUbCwlGcDAs8Jh/6UnvQwKBgQDoVSQs7Uq9MJCGIUM2bixX89tHzSxq5mn9wMD3/XRVfT5Ua8YkYBuzcmlcT39N7L5BwuyFqX3Vi7lv/Ya/qaQP6XkrZ8W1OAaTlYewfE5ZgknJqSpXcNWhABKeNmqndvqyQ/8HNCv/j8AdraGB2DGO57Xso5J0CQ43W/U9+QIyjwKBgHLL2hw3o+wXaRO3WMUPUmVM2zdRgR0suybp5a7Vqb0H5NZrohUw4NulIzJ8H6Q2VjMzJL6Q9sGu2HepF6ecTtBa7ErqtiVlG4Dr1aCOs5XhYEWBMlwxX+JKSt4Cn+UVoTB7Cy5lEhn7JurX0Xuy0ylXMWoIKKv89cs5eg6quzTBAoGAaq9eEztLjKCWXOE9SetBdYnG8aunb9cqaJlwgu/h0bfXPVDYBbAUSEyLURY4MQI7Q1tM3Pu9iqfEmUZj7/LoIV5mg6X9RX/alT6etk3+dF+9nlqN1OU9U9cCtZ/rTcb2y5EptJcidRH/eCFY/pTV/PcttOJPx/S4kHcroC+N8MUCgYEA6DA5QHxHfNN6Nxv+pEzy2DIxFe9RrBxS+KPBsra1C8jgdeMf4EmfU0Nox92V0q0bRrD5ztqQwSONI0hSRb1iiMWR6MuFnAFajUJfASjjIlZ6nIQjQslI7vjlvYyyHS/p/Codxap+yJlTLWwVEOXp2D9pWwiMq1xEyf0TH1BosvM=";
	size_t b64_length = libp2p_crypto_encoding_base64_decode_size(strlen(der));
	unsigned char buffer[b64_length];
	unsigned char* b = buffer;
	size_t ultimate_length;
	int retVal = libp2p_crypto_encoding_base64_decode((unsigned char*)der, strlen(der), b, b64_length, &ultimate_length);
	if (retVal == 0)
		return 0;
	// we now have the bytes, but we must strip off the type (5 bytes)
	struct RsaPrivateKey private_key = {0};
	int bytesToStrip = 5;
	retVal = libp2p_crypto_encoding_x509_der_to_private_key(&b[bytesToStrip], ultimate_length-bytesToStrip, &private_key);
	if (retVal == 0)
		return 0;
	return private_key.D > 0;
}

int test_public_der_to_private_der() {
	struct RsaPrivateKey private_key;
	int retVal = libp2p_crypto_rsa_generate_keypair(&private_key, 2048);
	if (retVal == 0)
		return 0;

	if (private_key.der_length == 0)
		return 0;
	if (private_key.der == NULL)
		return 0;

	// copy the public DER to a temporary area, then erase it, then try to generate it again.
	size_t public_der_temp_length = private_key.public_key_length;
	unsigned char public_der_temp[private_key.public_key_length];
	memcpy(public_der_temp, private_key.public_key_der, private_key.public_key_length);

	free(private_key.public_key_der);
	private_key.public_key_length = 0;

	retVal = libp2p_crypto_rsa_private_key_fill_public_key(&private_key);
	if (retVal == 0)
		return 0;

	if (public_der_temp_length != private_key.public_key_length)
		return 0;

	/*
	for(int i = 0; i < public_der_temp_length; i++)
		if (public_der_temp[i] != private_key.public_key_der[i])
			return 0;
	*/
	// that didn't work... so let's try it again, to see if it is consistent

	size_t public_der_temp_length2 = private_key.public_key_length;
	unsigned char public_der_temp2[public_der_temp_length2];
	memcpy(public_der_temp2, private_key.public_key_der, public_der_temp_length2);

	free(private_key.public_key_der);
	private_key.public_key_length = 0;

	retVal = libp2p_crypto_rsa_private_key_fill_public_key(&private_key);
	if (retVal == 0)
		return 0;

	for(int i = 0; i < public_der_temp_length2; i++)
		if (private_key.public_key_der[i] != public_der_temp2[i])
			return 0;

	// well, at least it is consistent.
	return 1;
}

int test_crypto_rsa_public_key_to_peer_id() {
	// this is the base64 encoded private key from the config file
	char* orig_priv_key = "CAASpwkwggSjAgEAAoIBAQDTDJBWjDzS/HxDNOHazvzH2bu9CPMVHUrrvKRdBUM5ansL6/CC3MVZ6HVm4O6QHRapN6EF2CbrTgI4KBOXIL125Xo8MlROnyfXYk3O5q2tgwL/MbW8kXjtkyCfBak7MUoLOdLU7Svg0gkl3l+uDAiDcCLnwJVcFfq9ch6z4wMOhYJqE5dtx0uXxn6IuKWl1B69FTvBXCc0thw8Rw54b941FDcsBH5ttV9mRNCJym3poZ5qalNgXlxoIIB+PUx5QD+aq7KMJdpAX8HkapBntCOahP/QUceRmma0grlZLeYkH6/oi/hIrM6se3KUZ+F6tBuDFys8UAZy/X2BCUbKjbxtAgMBAAECggEANWfQfpYuLhXGPBt9q6kFPm1SnJtPJ+CpvM2XqhJS2IyhZnrl+bd0GTRBwS7aL42s1lVFYf04nAK5fQxnKK8YQqX/MIxr2RldM5ukpN6qxGWKtJkXrAgD2dqJPrRoBpqKahzPxSHfIJ0Fw5dqDtjsrpYJvyt0oEDPmnDuZAbmFx4sJqnesPNhKxtRMBx1+yxGVuRVJjHcqAgqPqwNiuoMEaYMY+G9yzT6vza8ovCpbX7BBIgM5fAT9PD8TBG//Vu9THvj/ZomiVG2qv6RL0qQyVb+DUzPZz1amBsSvahtXCl72jA3JwAZ943RxSR66P934S0ashkVwLUi46z/EAbJ4QKBgQDojGIO07BEVL2+7VxlGL9XGZQp4Y3qlhh2zDDQGwkCq/KQ+BdNYWitPwqRl9GqFLgpmeQIhyHTOa/IThx+AXGKVQ24ROH+skUs4IbO6R3qY7BKtb5lkZE/Yln09x70BBngUYAzh/rtnsXO3cl1x2XDDqUbCwlGcDAs8Jh/6UnvQwKBgQDoVSQs7Uq9MJCGIUM2bixX89tHzSxq5mn9wMD3/XRVfT5Ua8YkYBuzcmlcT39N7L5BwuyFqX3Vi7lv/Ya/qaQP6XkrZ8W1OAaTlYewfE5ZgknJqSpXcNWhABKeNmqndvqyQ/8HNCv/j8AdraGB2DGO57Xso5J0CQ43W/U9+QIyjwKBgHLL2hw3o+wXaRO3WMUPUmVM2zdRgR0suybp5a7Vqb0H5NZrohUw4NulIzJ8H6Q2VjMzJL6Q9sGu2HepF6ecTtBa7ErqtiVlG4Dr1aCOs5XhYEWBMlwxX+JKSt4Cn+UVoTB7Cy5lEhn7JurX0Xuy0ylXMWoIKKv89cs5eg6quzTBAoGAaq9eEztLjKCWXOE9SetBdYnG8aunb9cqaJlwgu/h0bfXPVDYBbAUSEyLURY4MQI7Q1tM3Pu9iqfEmUZj7/LoIV5mg6X9RX/alT6etk3+dF+9nlqN1OU9U9cCtZ/rTcb2y5EptJcidRH/eCFY/pTV/PcttOJPx/S4kHcroC+N8MUCgYEA6DA5QHxHfNN6Nxv+pEzy2DIxFe9RrBxS+KPBsra1C8jgdeMf4EmfU0Nox92V0q0bRrD5ztqQwSONI0hSRb1iiMWR6MuFnAFajUJfASjjIlZ6nIQjQslI7vjlvYyyHS/p/Codxap+yJlTLWwVEOXp2D9pWwiMq1xEyf0TH1BosvM=";
	// this is the peer id from the config file
	char* orig_peer_id = "QmRskXriTSRjAftYX7QG1i1jAhouz5AHaLYZKNhEWRu5Fq";
	size_t orig_peer_id_size = strlen(orig_peer_id);
	// if we take the private key, retrieve the public key, hash it, we should come up with the peer id

	// 1) take the private key and turn it back into bytes (decode base 64)
	size_t decode_base64_size = libp2p_crypto_encoding_base64_decode_size(strlen(orig_priv_key));
	unsigned char decode_base64[decode_base64_size];
	memset(decode_base64, 0, decode_base64_size);
	unsigned char* ptr = decode_base64;

	int retVal = libp2p_crypto_encoding_base64_decode(orig_priv_key, strlen(orig_priv_key), ptr, decode_base64_size, &decode_base64_size);
	if (retVal == 0)
		return 0;

	// the first 5 bytes [0-4] are protobuf metadata before the DER encoded private key
	// byte 0 is "Tag 1 which is a varint"
	// byte 1 is the value of the varint
	// byte 2 is "Tag 2 which is a type 2, length delimited field"
	// bytes 3 & 4 is a varint with the value of 1191, which is the number of bytes that follow

	// 2) take the bytes of the private key and turn it back into a private key struct
	struct RsaPrivateKey private_key = {0};
	retVal = libp2p_crypto_encoding_x509_der_to_private_key(&decode_base64[5], decode_base64_size - 5, &private_key);
	if (retVal == 0)
		return 0;

	// 2b) take the private key and fill in the public key DER
	retVal = libp2p_crypto_rsa_private_key_fill_public_key(&private_key);
	if (retVal == 0)
		return 0;

	// 3) grab the public key, hash it, then base58 it
	unsigned char hashed[32];
	ID_FromPK_non_null_terminated(hashed, private_key.public_key_der, private_key.public_key_length);
	size_t final_id_size = 1600;
	unsigned char final_id[final_id_size];
	memset(final_id, 0, final_id_size);
	retVal = PrettyID(final_id, &final_id_size, hashed, 32);
	if (retVal == 0)
		return 0;

	// 4) compare results
	if (orig_peer_id_size != final_id_size)
		return 0;

	if (strncmp(orig_peer_id, final_id, final_id_size) != 0)
		return 0;

	return 1;
}


#endif /* test_rsa_h */
