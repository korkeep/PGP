#pragma once

/*
* 2021-1 KyungHee Information Protection Project
* PGP: Pretty Good Privacy
* Author: korkeep (Sungsu Kim)
* Student ID: 2016104109
* rsa.h: I manually changed origin source code(rsa.c) because it would be cleaner if I partially modified the source code.
*/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "sdes.h"

#define MAC_LEN 17 //MAC Length = 16(Digest Length) + 1(Flag)
#define KEY_LEN 11 //KEY Length = 10(Key Length) + 1(Flag)

/* Global Variable */
long int p, q, n, t, flag, i, j, k;
long int e[100], d[100], temp_MAC[100], temp_Key[100]; //Key List
char de_MAC[MAC_LEN], en_MAC[MAC_LEN]; //MAC Data
int de_Key[KEY_LEN], en_Key[KEY_LEN]; //Key Data

int isPrime(long int); /* Check if input is Prime */
void calc_E(); /* Calculate E */
long int calc_D(long int); /* Calculate D */
void MAC_RSAEncrypt(int); /* MAC RSA Encryption */
void MAC_RSADecrypt(); /* MAC RSA Decryption */
void Key_RSAEncrypt(int); /* Key RSA Encryption */
void Key_RSADecrypt(); /* Key RSA Decryption */

/* Check if input is Prime */
int isPrime(long int pr)
{
    j = sqrt(pr);
    /* Sieve of Eratosthenes */
    for (int i = 2; i <= j; i++)
    {
        if (pr % i == 0)
            return 0;
    }
    return 1;
}

/* Calculate E */
void calc_E()
{
    k = 0;
    for (i = 2; i < t; i++)
    {
        if (t % i == 0)
            continue;
        flag = isPrime(i);
        if (flag == 1 && i != p && i != q)
        {
            e[k] = i;
            flag = calc_D(e[k]);
            if (flag > 0)
            {
                d[k] = flag;
                k++;
            }
            if (k == 99)
                break;
        }
    }
}

/* Calculate D */
long int calc_D(long int x)
{
    long int k = 1;
    while (1)
    {
        k = k + t;
        if (k % x == 0)
            return (k / x);
    }
}

/* MAC RSA Encryption */
void MAC_RSAEncrypt(int len)
{
    //Encrypt Digest with Sender's Private Key
    long int pt, ct, key = e[0];
    i = 0;
    while (i != len)
    {
        pt = de_MAC[i];
        pt = pt - 96;
        k = 1;
        for (j = 0; j < key; j++)
        {
            k = k * pt;
            k = k % n;
        }
        temp_MAC[i] = k;
        ct = k + 96;
        en_MAC[i] = ct;
        i++;
    }
    en_MAC[i] = -1;
    MDPrint(en_MAC);
}
/* MAC RSA Decryption */
void MAC_RSADecrypt()
{
    //Decrypt Digest with Sender's Public Key
    long int pt, ct, key = d[0];
    i = 0;
    while (en_MAC[i] != -1)
    {
        ct = temp_MAC[i];
        k = 1;
        for (j = 0; j < key; j++)
        {
            k = k * ct;
            k = k % n;
        }
        pt = k + 96;
        de_MAC[i] = pt;
        i++;
    }
    de_MAC[i] = -1;
    MDPrint(de_MAC);
}
/* Key RSA Encryption */
void Key_RSAEncrypt(int len)
{
    //Encrypt SDES Key with Receiver's Public Key
    long int pt, ct, key = d[1];
    i = 0;
    while (i != len)
    {
        pt = sdes_key[i];
        pt = pt - 96;
        k = 1;
        for (j = 0; j < key; j++)
        {
            k = k * pt;
            k = k % n;
        }
        temp_Key[i] = k;
        ct = k + 96;
        en_Key[i] = ct;
        i++;
    }
    en_Key[i] = -1;
    for (i = 0; i < KEY_LEN - 1; i++) {
        printf("%d", en_Key[i]);
    }
    printf("\n");
}
/* Key RSA Decryption */
void Key_RSADecrypt()
{
    //Decrypt SDES Key with Receiver's Private Key
    long int pt, ct, key = e[1];
    i = 0;
    while (en_Key[i] != -1)
    {
        ct = temp_Key[i];
        k = 1;
        for (j = 0; j < key; j++)
        {
            k = k * ct;
            k = k % n;
        }
        pt = k + 96;
        de_Key[i] = pt;
        i++;
    }
    de_Key[i] = -1;
    for (i = 0; i < KEY_LEN - 1; i++) {
        printf("%d", de_Key[i]);
    }
    printf("\n");
}