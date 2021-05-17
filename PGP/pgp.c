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

#define MAX_LEN 2048 //Max Text Length
#define MID_LEN 1024 //Mid Text Length
#define DIGEST_LEN 16 //Digest Length
#define SDES_KEY_LEN 10 //SDES Key length

int main() {
	
	/*
	PGP Transmission mode
	# Step 1: Read Text.txt
	# Step 2: Message Digest using MD5 Algorithm
	# Step 3: Encrypt Digest with Sender Private Key (MAC)
	# Step 4: Concat Origin Content & Encrypted MAC
	# Step 5: Encrypt Concatated Data with Symmetric Key
	# Step 6: Encrypt Symmetric Key with Receiver Public Key
	# Step 7: Concat Encrypted Data & Write EText.txt
	*/

	//PGP Transmission mode
	printf("### PGP Transmission Mode ###\n\n");

	//Step 1: Read Text.txt
	printf("# Step 1: Read Text.txt\n");
	char text[MAX_LEN] = "";
	char temp_s[2];
	//Open Text.txt as READ option
	FILE* fp_r = fopen("Text.txt", "r");
	//File open error
	if (fp_r == NULL) {
		fprintf(stdout, "Error: Unable to find Text.txt\n");
		exit(1);
	}
	//READ Text.txt
	while (feof(fp_r) == 0) {
		temp_s[0] = fgetc(fp_r);
		temp_s[1] = '\0';
		strcat(text, temp_s);
	}
	printf("%s\n\n", text);
	fclose(fp_r);

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
	// +++++++++++++++++++++++++++++++++++ Notice ++++++++++++++++++++++++++++++++++++ //
	// 『 Since the MAC's type is different, Output looks like broken. (hex ≧ string) 『 //
	printf("# Step 4: Concat Origin Content & Encrypted MAC\n");
	strcat(text, "||"); //Concat Flag = "||"
	strcat(text, en_MAC); //Append Encrypted MAC
	printf("%s", text);
	printf("\n\n");

	//Step 5: Encrypt Concatated Data with Symmetric Key
	//Inputs: Binary Format
	printf("# Step 5: Encrypt Concatated Data with Symmetric Key\n");
	//Generate 10-Bits SDES Key Randomly
	gen_keys();
	//Encrypt Data
	int pt[8] = { 0 };
	int sdes_text[MAX_LEN][8] = { pt };
	printf("Encrypted Binary Text: ");
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
	}
	printf("\n\n");

	//Step 6: Encrypt Symmetric Key with Receiver Public Key
	printf("# Step 6: Encrypt Symmetric Key with Receiver Public Key\n");
	//Encrypt SDES Key with Receiver's Public Key
	printf("Encrypted Symmetric Key (10-bits): ");
	Key_RSAEncrypt(SDES_KEY_LEN); 
	printf("\n");

	//Step 7: Concat Encrypted Data & Write EText.txt
	printf("# Step 7: Concat Encrypted Data & Write EText.txt\n");
	printf("Encrypted Binary Text || Encrypted Symmetric Key: ");
	int sdes_key[SDES_KEY_LEN] = { 0 };
	//Open EText.txt as WRITE option
	FILE* fp_w = fopen("EText.txt", "w");
	//File open error
	if (fp_w == NULL) {
		fprintf(stdout, "Error: Unable to Open File EText.txt\n");
		exit(1);
	}
	for (int idx = 0; idx < MAX_LEN; idx++) {
		if (text[idx] == NULL) {
			//Concat Flag = "||"
			fputs("||", fp_w);
			printf("||");
			//Concat Encrypted Key
			for (int i = 0; i < 10; i++) {
				sdes_key[i] = en_Key[i];
				printf("%d", sdes_key[i]);
				fprintf(fp_w, "%d", sdes_key[i]);
			}
			break;
		}
		else {
			for (i = 0; i < 8; i++) {
				printf("%d", sdes_text[idx][i]);
				fprintf(fp_w, "%d", sdes_text[idx][i]);
			}
		}
	}
	fclose(fp_w);
	printf("\n\n\n");


	/*
	PGP Receiving mode
	# Step 1: Read EText.txt
	# Step 2: Decrypt Symmetric Key with Receiver Private Key
	# Step 3: Decrypt Encrypted Data with Symmetric Key
	# Step 4: Decrypt MAC with Sender Public Key
	# Step 5: Parse Plain Text Data
	# Step 6: Hash Plain Text Data & Compare with MAC
	# Step 7: Write DText.txt
	*/

	//PGP Receiving mode
	printf("### PGP Receiving Mode ###\n\n");

	//Step 1: Read EText.txt
	printf("# Step 1: Read EText.txt\n");
	char etext[MAX_LEN] = "";
	char ekey[MID_LEN] = "";
	//text[0] = '\0';
	int flag = -1;
	char c = '|';
	//Open Text.txt as READ option
	fp_r = fopen("EText.txt", "r");
	//File open error
	if (fp_r == NULL) {
		fprintf(stdout, "Error: Unable to find EText.txt\n");
		exit(1);
	}
	//READ EText.txt
	while (feof(fp_r) == 0) {
		temp_s[0] = fgetc(fp_r);
		temp_s[1] = '\0';
		//Concat Flag = "||"
		if (temp_s[0] == c) {
			temp_s[0] = fgetc(fp_r);
			temp_s[1] = '\0';
			if (temp_s[0] == c) {
				flag = 0;
				continue;
			}
		}
		//etext
		if (flag == -1) {
			strcat(etext, temp_s);
		}
		//ekey
		else {
			strcat(ekey, temp_s);
		}
	}
	printf("Encrypted Binary Text: %s\n", etext);
	printf("Encrypted Symmetric Key: %s\n", ekey);
	printf("\n");
	fclose(fp_r);

	//Step 2: Decrypt Symmetric Key with Receiver Private Key
	printf("# Step 2: Decrypt Symmetric Key with Receiver Private Key\n");
	printf("Decrypted SDES Key (10-bits): ");
	Key_RSADecrypt(ekey);
	printf("\n");

	//Step 3: Decrypt Encrypted Data with Symmetric Key
	printf("# Step 3: Decrypt Encrypted Data with Symmetric Key\n");
	char dtext[MAX_LEN] = "";
	char temp_c[2] = "";
	//Decrypt Data
	printf("Decrypted Binary Text: ");
	for (int idx = 0; idx < MAX_LEN; ) {
		if (etext[idx] == NULL) break;
		//Put Encrypted Data into ct
		for (i = 0; i < 8; i++, idx++){
			ct[i] = etext[idx] - 48; //ASCII '0' = 48
		}
		//Decrypt Data using SDES
		SDES(ct, 1);
		temp_c[0] = 0;
		temp_c[1] = '\0';
		for (i = 0; i < 8; i++) {
			printf("%d", ct[i]);
			switch (i) {
			case 0:
				temp_c[0] = temp_c[0] + ct[i] * 128;
				break;
			case 1:
				temp_c[0] = temp_c[0] + ct[i] * 64;
				break;
			case 2:
				temp_c[0] = temp_c[0] + ct[i] * 32;
				break;
			case 3:
				temp_c[0] = temp_c[0]  + ct[i] * 16;
				break;
			case 4:
				temp_c[0] = temp_c[0] + ct[i] * 8;
				break;
			case 5:
				temp_c[0] = temp_c[0] + ct[i] * 4;
				break;
			case 6:
				temp_c[0] = temp_c[0] + ct[i] * 2;
				break;
			case 7:
				temp_c[0] = temp_c[0] + ct[i] * 1;
				break;
			}
		}
		strcat(dtext, temp_c);
	}
	printf("\nDecrypted Plain Text: \n");
	printf("%s", dtext);
	printf("\n\n");

	// Step 4: Decrypt MAC with Sender Public Key
	printf("# Step 4: Decrypt MAC with Sender Public Key\n");
	printf("Decrypted Digest: ");
	MAC_RSADecrypt();
	printf("\n");

	// Step 5: Parse Plain Text Data
	printf("# Step 5: Parse Plain Text Data\n");
	char parse[2][MAX_LEN];
	char* temp;
	temp = strtok(dtext, "||");
	i = 0;
	//Parse Plain Text(parse[0]) & MAC(parse[1])
	while (temp != NULL) {
		strcpy(parse[i++], temp);
		temp = strtok(NULL, "||");
	}
	printf("Parsed Plain Text: \n");
	printf("%s\n\n", parse[0]);
	
	//Step 6: Hash Plain Text Data & Compare with Digest
	printf("# Step 6: Hash Plain Text Data & Compare with Digest\n");
	//Message Digest
	int auth = 1; //Authentication Flag
	char new_digest[DIGEST_LEN];
	//Message Digest using MD5
	MD5Init(&context);
	MD5Update(&context, (unsigned char*)parse[0], strlen(parse[0]));
	MD5Final(new_digest, &context);
	//Prints the Result
	printf("Old MD5 Digest: ");
	MDPrint(de_MAC); // From rsa.h
	printf("New MD5 Digest: ");
	MDPrint(new_digest);
	//Compare Hashed Result with Original Digest
	for (int i = 0; i < DIGEST_LEN; i++) {
		if (de_MAC[i] != new_digest[i]) {
			printf("『『『 Authentication Result: Failed 『『『\n");
			auth = 0;
			break;
		}
	}
	if(auth == 1) {
		printf("『『『 Authentication Result: Success 『『『\n");
	}
	printf("\n");

	// Step 7: Write DText.txt
	printf("# Step 7: Write DText.txt\n");
	//Open DText.txt as WRITE option
	fp_w = fopen("DText.txt", "w");
	//File open error
	if (fp_w == NULL) {
		fprintf(stdout, "Error: Unable to Open File DText.txt\n");
		exit(1);
	}
	//Wirte Plain Test to DText.txt
	fputs("Plain Text: \n", fp_w);
	fputs(parse[0], fp_w);
	fputs("\n\n", fp_w);
	//Wirte MD5 Digest to DText.txt
	// +++++++++++++++++++++++++++++++++++ Notice ++++++++++++++++++++++++++++++++++++ //
	// 『 Since the MAC's type is different, Output looks like broken. (hex ≧ string) 『 //
	fputs("MD5 Digest: \n", fp_w);
	for (int i = 0; i < 16; i++) {
		fprintf(fp_w, "%c", parse[1][i]);
	}
	printf("DText.txt Wirte Success\n");
	fclose(fp_w);
	printf("\n\n");

	return 0;
}