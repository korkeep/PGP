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

#define MAC_LEN 17 //MAC Length = 16(Digest Length) + 1(Flag)

/* Global Variable */
long int p, q, n, t, flag, i, j, k;
long int e[100], d[100], temp[100]; //Key List
char de[MAC_LEN], en[MAC_LEN]; //Data

int isPrime(long int); /* Check if input is Prime */
void calc_E(); /* Calculate E */
long int calc_D(long int); /* Calculate D */
void RSAEncrypt(int); /* RSA Encryption */
void RSADecrypt(); /* RSA Decryption */

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

/* RSA Encryption */
void RSAEncrypt(int len)
{
    long int pt, ct, key = e[0];
    i = 0;
    while (i != len)
    {
        pt = de[i];
        pt = pt - 96;
        k = 1;
        for (j = 0; j < key; j++)
        {
            k = k * pt;
            k = k % n;
        }
        temp[i] = k;
        ct = k + 96;
        en[i] = ct;
        i++;
    }
    en[i] = -1;
    MDPrint(en);
}

/* RSA Decryption */
void RSADecrypt()
{
    long int pt, ct, key = d[0];
    i = 0;
    while (en[i] != -1)
    {
        ct = temp[i];
        k = 1;
        for (j = 0; j < key; j++)
        {
            k = k * ct;
            k = k % n;
        }
        pt = k + 96;
        de[i] = pt;
        i++;
    }
    de[i] = -1;
    MDPrint(de);
}