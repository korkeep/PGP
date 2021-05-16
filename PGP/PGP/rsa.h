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

/* Global Variable */
long int p, q, n, t, flag, e[100], d[100], temp[100], j, m[100], en[100], i;
char msg[100];

int isPrime(long int); /* Check if input is Prime */
void calc_E(); /* Calculate E */
long int calc_D(long int); /* Calculate D */
void RSAEncrypt(); /* RSA Encryption */
void RSADecrypt(); /* RSA Decryption */

/* Check if input is Prime */
int isPrime(long int pr)
{
    int i;
    j = sqrt(pr);

    /* Sieve of Eratosthenes */
    for (i = 2; i <= j; i++)
    {
        if (pr % i == 0)
            return 0;
    }

    return 1;
}

/* Calculate E */
void calc_E()
{
    int k;
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

/* Calculate D*/
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
void RSAEncrypt()
{
    long int pt, ct, key = e[0], k, len;
    i = 0;
    len = strlen(msg);
    while (i != len)
    {
        pt = m[i];
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
    printf("\nTHE ENCRYPTED MESSAGE IS\n");
    for (i = 0; en[i] != -1; i++)
        printf("%c", en[i]);
}

/* RSA Decryption */
void RSADecrypt()
{
    long int pt, ct, key = d[0], k;
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
        m[i] = pt;
        i++;
    }
    m[i] = -1;
    printf("\nTHE DECRYPTED MESSAGE IS\n");
    for (i = 0; m[i] != -1; i++)
        printf("%c", m[i]);
}