#pragma once

/*
* 2021-1 KyungHee Information Protection Project
* PGP: Pretty Good Privacy
* Author: korkeep (Sungsu Kim)
* Student ID: 2016104109
* sdes.h: I manually changed origin source code(sdes.c) because it would be cleaner if I partially modified the source code.
*/

#include <stdio.h>
#include <stdlib.h>

/* Global Variable */
int l[4], r[4], sdes_key[10], keys[2][8], ct[8];

/* SBOX */
void SBOX(int sip[], int p[], int sbno, int i)
{
	int sbox[2][4][4] = { 1, 0, 3, 2, 3, 2, 1, 0, 0, 2, 1, 3, 3, 1, 3, 2, 0, 1, 2, 3, 2, 0, 1, 3, 3, 0, 1, 0, 2, 1, 0, 3 };
	int rw, c, sop;
	rw = sip[3] + sip[0] * 2;
	c = sip[2] + sip[1] * 2;
	sop = sbox[sbno][rw][c]; //sop gives decimal value of S-Box Output
	for (; sop != 0; sop /= 2)
		p[i--] = sop % 2;
}

/* DES Algorithm */
void func_K(int round)
{
	int EP[] = { 4, 1, 2, 3, 2, 3, 4, 1 }, i, epd[8];
	int slip[4], srip[4];
	int p[4] = { 0 }, p4[] = { 2, 4, 3, 1 }, np[4];

	for (i = 0; i < 8; i++) // E/P Permutation
		epd[i] = r[EP[i] - 1];

	for (i = 0; i < 8; i++) //Performing XOR with Key
		if (i < 4)
			slip[i] = epd[i] ^ keys[round][i]; // Using Key _ 1=>0
		else
			srip[i - 4] = epd[i] ^ keys[round][i];

	SBOX(slip, p, 0, 1);	//Calling SBox 1, 0->SBOX 1
	SBOX(srip, p, 1, 3);	//Calling SBox 1, 1->SBOX 2

	for (i = 0; i < 4; i++) //P4 permutation
		np[i] = p[p4[i] - 1];

	for (i = 0; i < 4; i++)
		l[i] = l[i] ^ np[i];
}

/* Left Shift Function */
void left_shift(int keyip[], int nob)
{
	int t1, t2, i;
	while (nob > 0)
	{
		t1 = keyip[0], t2 = keyip[5];
		for (i = 0; i < 9; i++)
			if (i < 4)
				keyip[i] = keyip[i + 1];
			else if (i > 4)
				keyip[i] = keyip[i + 1];
		keyip[4] = t1, keyip[9] = t2;
		nob--;
	}
}

/* Generate Key */
void gen_keys()
{
	int i, keyip[10];
	int p10[] = { 3, 5, 2, 7, 4, 10, 1, 9, 8, 6 }, p8[] = { 6, 3, 7, 4, 8, 5, 10, 9 };
	
	// Get Key Input
	/*printf(">> Enter Key(10-bits): ");
	for (i = 0; i < 10; i++) {
		scanf("%d", &key[i]);
	}*/
	printf("+ Random SDES Key(10-bits): ");
	for (i = 0; i < 10; i++) {
		sdes_key[i] = rand() % 2;
		printf("%d", sdes_key[i]);
	}
	printf("\n");
	
	// Permutation P10
	for (i = 0; i < 10; i++) {
		keyip[i] = sdes_key[p10[i] - 1];
	}
	
	// Generating Key1
	left_shift(keyip, 1); // Left Shifting
	printf("+ Key1(8-bits): ");
	//Permuting P8 on key1
	for (i = 0; i < 8; i++)
	{
		keys[0][i] = keyip[p8[i] - 1]; // Key1 Generated!!
		printf("%d", keys[0][i]);
	}
	printf("\n");
	
	// Generating Key2
	left_shift(keyip, 2); // Left Shifting
	printf("+ Key2(8-bits): ");
	for (i = 0; i < 8; i++)
	{
		keys[1][i] = keyip[p8[i] - 1]; // Key2 Generated!!
		printf("%d", keys[1][i]);
	}
	printf("\n");
}

/* Encryption, Decryption */
void SDES(int pt[], int c)
{
	int ip[] = { 2, 6, 3, 1, 4, 8, 5, 7 }, ipi[] = { 4, 1, 3, 5, 7, 2, 8, 6 }, t[8], i;
	
	for (i = 0; i < 8; i++) // Performing Permutation on input bits!!
		if (i < 4)
			l[i] = pt[ip[i] - 1];
		else
			r[i - 4] = pt[ip[i] - 1];
	
	func_K(c);				//Round 0+1 using key 0+1
	for (i = 0; i < 4; i++) //Swapping left & right
		r[i] = l[i] + r[i], l[i] = r[i] - l[i], r[i] = r[i] - l[i];
	
	func_K(!c); // Round 1+1 wid key1+1 wid swapped bits
	for (i = 0; i < 8; i++)
		if (i < 4)
			t[i] = l[i];
		else
			t[i] = r[i - 4];
	
	for (i = 0; i < 8; i++)
		ct[i] = t[ipi[i] - 1];
}