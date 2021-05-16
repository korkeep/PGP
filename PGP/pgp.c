/*
* 2021-1 KyungHee Information Protection Project
* PGP: Pretty Good Privacy
* ID: 2016104109
* Author: korkeep (Sungsu Kim)
* main.c: I manually changed origin source code that need some correction.
*/

#include "md5.h"
#include "rsa.h"
#include "sdes.h"
#include <string.h>

#define MAX_LEN 1024 //Max Text Length
#define DIGEST_LEN 16 //Digest Length
#define SDES_KEY_LEN 16 //SDES Key length (8*2)
#define RSA_LEN 128 //RSA Output Length

int main() {
	
	/*
	PGP Transmission mode
	# Step 1: Read Text.txt
	# Step 2: Message Digest using MD5 Algorithm
	# Step 3: Encrypt Digest with Sender Private Key (MAC)
	# Step 4: Concat Origin Content & Encrypted MAC
	# Step 5: Encrypt Concatated Data with Symmetric Key
	# Step 6: Encrypt Symmetric Key with Receiver Public Key
	# Step 7: Concat Encrypted Data & Encrypted Symmetric Key
	# Step 8: Write EText.txt
	*/

	//PGP Transmission mode
	printf("### PGP Transmission Mode ###\n\n");

	//Step 1: Read Text.txt
	printf("# Step 1: Read Text.txt\n");
	char text[MAX_LEN] = "";
	char temp_s[2];
	//Open Text.txt as READ option
	FILE* fp = fopen("Text.txt", "r");
	//File open error
	if (fp == NULL) {
		fprintf(stdout, "Error: Unable to find Text.txt\n");
		exit(1);
	}
	//READ Text.txt
	while (feof(fp) == 0) {
		temp_s[0] = fgetc(fp);
		temp_s[1] = '\0';
		strcat(text, temp_s);
	}
	printf("%s\n\n", text);
	fclose(fp);

	//Step 2: Message Digest using MD5 Algorithm
	printf("# Step 2: Message Digest using MD5 Algorithm\n");
	//Message Digest
	char digest[DIGEST_LEN];
	//Message Digest using MD5
	MD5_CTX context;
	MD5Init(&context);
	MD5Update(&context, (unsigned char*)text, strlen(text));
	MD5Final(digest, &context);
	//Prints the Result
	printf("MD5 Digest: ");
	MDPrint(digest);
	printf("\n");

	//Step 3: Encrypt Digest with Sender Private Key (MAC)
	printf("# Step 3: Encrypt Digest with Sender Private Key (MAC)\n");
	//First Prime Input
	printf(">> Enter first prime number: ");
	scanf("%d", &p);
	//Check First Input is Real Prime
	flag = isPrime(p);
	if (flag == 0) {
		printf("Error: Input is not validate\n");
		exit(1);
	}
	//Second Prime Input
	printf(">> Enter second prime number: ");
	scanf("%d", &q);
	//Check Second Input is Real Prime
	flag = isPrime(q);
	if (flag == 0 || p == q) {
		printf("Error: Input is not validate\n");
		exit(1);
	}
	//Copy Digest to de_MAC
	for (int i = 0; i < DIGEST_LEN; i++) {
		de_MAC[i] = digest[i];
	}
	//Calculate n, t
	n = p * q;
	t = (p - 1) * (q - 1);
	//Calculate E
	calc_E();
	//Print Possible E and D
	printf("Possible values of E and D are");
	for (int i = 0; i < j - 1; i++) {
		printf("\n E:%ld\tD:%ld", e[i], d[i]);
	}
	printf("\n");
	//Encrypt Digest with Sender's Private Key
	printf("MD5 Digest: ");
	MDPrint(digest);
	printf("Encrypted Digest: ");
	MAC_RSAEncrypt(DIGEST_LEN);
	printf("\n");
	
	//Step 4: Concat Origin Content & Encrypted MAC
	printf("# Step 4: Concat Origin Content & Encrypted MAC\n");
	// ¡Ø Since the MAC's type is different, Output looks like broken. (hex ¡æ string) ¡Ø //
	strcat(text, "||"); //Concat Flag = "||"
	strcat(text, en_MAC); //Append Encrypted MAC
	printf("%s", text);
	printf("\n\n");

	//Step 5: Encrypt Concatated Data with Symmetric Key
	//Inputs: Binary Format
	printf("# Step 5: Encrypt Concatated Data with Symmetric Key\n");
	//Generate 10-Bits SDES Key ¡æ Ex) 0 0 0 0 0 1 1 0 1 1
	gen_keys();
	//Encrypt Data
	int pt[8] = { 0 };
	int sdes_text[MAX_LEN][8] = { pt };
	printf("Cipher Text: ");
	for (int idx = 0; idx < MAX_LEN; idx++) {
		if (text[idx] == NULL) break;
		//Character to Binary
		for (int i = 7; i >= 0; i--) {
			pt[7 - i] = ((text[idx] & (1 << i)) ? '1' : '0') - 48; //ASCII '0' = 48
		}
		//Encrypt Data using SDES
		SDES(pt, 0);
		for (i = 0; i < 8; i++) {
			sdes_text[idx][i] = ct[i];
			printf("%d", sdes_text[idx][i]);
		}
		printf(" ");
	}
	printf("\n\n");

	//Step 6: Encrypt Symmetric Key with Receiver Public Key
	printf("# Step 6: Encrypt Symmetric Key with Receiver Public Key\n");
	//Encrypt SDES Key with Receiver's Public Key
	printf("Encrypted SDES Key (Key1 + Key2): ");
	Key_RSAEncrypt(SDES_KEY_LEN); 
	printf("\n");

	//Step 7: Concat Encrypted Data & Encrypted Symmetric Key
	printf("# Step 7: Concat Encrypted Data & Encrypted Symmetric Key\n");
	for (int idx = 0; idx < MAX_LEN; idx++) {
		if (text[idx] == NULL) {
			//Concat Encrypted Key1
			for (int i = 0; i < 8; i++) {
				sdes_text[idx][i] = en_Key[i];
				printf("%d", sdes_text[idx][i]);
			}
			printf(" ");
			//Concat Encrypted Key2
			for (int i = 8; i < 16; i++) {
				sdes_text[idx + 1][i - 8] = en_Key[i];
				printf("%d", sdes_text[idx + 1][i - 8]);
			}
			break;
		}
		else {
			for (i = 0; i < 8; i++) {
				printf("%d", sdes_text[idx][i]);
			}
			printf(" ");
		}
	}
	printf("\n\n");



	printf("\n\n\n");
	printf("Decrypted SDES Key (Key1 + Key2): ");
	Key_RSADecrypt();

	printf("\n\n\n");
	//Decrypt Data
	printf("Plain Text: ");
	for (int idx = 0; idx < MAX_LEN; idx++) {
		if (text[idx] == NULL) break;
		//Put Encrypted Data into ct
		for (i = 0; i < 8; i++) {
			ct[i] = sdes_text[idx][i];
		}
		//Decrypt Data using SDES
		SDES(ct, 1);
		for (i = 0; i < 8; i++) {
			printf("%d", ct[i]);
		}
		printf(" ");
	}
	printf("\n");

	//RSA Decryption
	printf("\n\n\n\n");
	printf("Decrypted Digest: ");
	MAC_RSADecrypt();

	/*md5 test*/
	/*
	char a[100] = "testing123";

	MDString(a);
	printf("\n");
	MDFile("a.txt");
	*/

	/*rsa test*/
	/*
    printf("* Enter first prime number\n");
    scanf("%d", &p);
   
    flag = isPrime(p);
    if (flag == 0)
    {
        printf("\nWRONG INPUT\n");
        exit(1);
    }

    printf("\nENTER ANOTHER Prime NUMBER\n");
    scanf("%d", &q);
    
	flag = isPrime(q);
    if (flag == 0 || p == q)
    {
        printf("\nWRONG INPUT\n");
        exit(1);
    }

    printf("\nENTER MESSAGE\n");
    fflush(stdin);
    scanf("%s", msg);

    for (i = 0; msg[i] != NULL; i++)
        de_MAC[i] = msg[i];
    n = p * q;
    t = (p - 1) * (q - 1);
    calc_E();

    printf("\nPOSSIBLE VALUES OF e AND d ARE\n");
    for (i = 0; i < j - 1; i++)
        printf("\n%ld\t%ld", e[i], d[i]);

    RSAEncrypt();
    RSADecrypt();*/
	

	/* sdes test */
	//Inputs: Binary Format
	//8-Bits Plain Text: 1 0 0 0 1 0 1 1
	//10-Bits Key : 0 0 0 0 0 1 1 0 1 1
	/*
	int pt[8] = { 0 }, i;
	printf("Enter plain text binary bits:");

	for (i = 0; i < 8; i++)
		scanf("%d", &pt[i]);

	gen_keys(); // Generating Keys key1 & key2

	SDES(pt, 0);
	printf("\nCipher Text :");
	for (i = 0; i < 8; i++)
		printf("%d", ct[i]);
	
	//Decrypting - - -
	SDES(ct, 1);
	printf("\nPlain Text (After Decrypting):");
	for (i = 0; i < 8; i++)
		printf("%d", ct[i]);
	*/

	/*
	PGP Receiving mode
	# Step 1: Read EText.txt
	# Step 2: Decrypt Symmetric Key with Receiver Private Key
	# Step 3: Decrypt Dencrypted Data with Symmetric Key
	# Step 4: Decrypt MAC with Sender Public Key
	# Step 5: Hash Origin Data
	# Step 6: Compare MAC with Hashed Data
	# Step 7: Write DText.txt
	*/

	return 0;
}