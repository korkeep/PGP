#include "md5.h"
#include "rsa.h"
#include "sdes.h"

/*
* 2021-1 KyungHee Information Protection Project
* PGP: Pretty Good Privacy
* ID: 2016104109
* Author: korkeep (Sungsu Kim)
* Notice: I manually changed origin source code that need some correction.
*/

int main() {
	
	/*
	PGP Transmission mode
	# Step 1: Read Testst.txt
	# Step 2: Hash Origin Data (MAC)
	# Step 3: Encrypt MAC with Sender Private Key
	# Step 4: Concat Origin Content & Encrypted MAC
	# Step 5: Zip Data
	# Step 6: Encrypt Zipped Data with Symmetric Key
	# Step 7: Encrypt Symmetric Key with Receiver Public Key
	# Step 8: Concat Encrypted Data & Encrypted Symmetric Key
	*/
	char a[100] = "testing123";

	MDString(a);
	printf("\n");
	MDFile("a.txt");


	/*
	PGP Receiving mode
	# Step 1: Decrypt Symmetric Key with Receiver Private Key
	# Step 2: Decrypt Dencrypted Data with Symmetric Key
	# Step 3: Unzip Data
	# Step 4: Decrypt MAC with Sender Public Key
	# Step 5: Hash Origin Data
	# Step 6: Compare MAC with Hashed Data
	*/

	return 0;
}