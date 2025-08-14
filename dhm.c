#include "dhm.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void dhmadd(DHmap *, char *key, void *value);
void dhmremove(DHmap *, char *key);
void *dhmget(DHmap *, char *key);
void dhmdestroy(DHmap *);

static int dhmhash(char *key);
static int hashmask(int hash, int bits);
static void grow(DHmap *hm);

static inline void
grow(DHmap *hm)
{
        assert(hm >= 0);
        ++hm->d;
        int size = sizeof *hm->data * 1 << (hm->d);
        hm->data = realloc(hm->data, size);
        memcpy((char *) hm->data + size / 2, hm->data, size / 2);
}

int
dhmhash(char *key)
{
        /* just a random function */
        int n = 1;
        while (*key) {
                n <<= 2;
                n += *key;
                ++key;
        }
        return n;
}

static int
hashmask(int hash, int bits)
{
        assert(bits >= 0);
        return hash & ((1 << bits) - 1);
}

void
dhmadd(DHmap *hm, char *key, void *value)
{
        if (hm == NULL) return;
        if (key == NULL) return;
        if (hm->data == NULL) {
                hm->data = calloc(1, sizeof(DHnode *));
                hm->data[0] = calloc(1, sizeof(DHnode));
                hm->d = 0;
        }

        int k = dhmhash(key);
        int m = hashmask(k, hm->d);

        /* Hash points to an empty cell */
        if (hm->data[m]->key == 0) {
                hm->data[m]->key = strdup(key);
                hm->data[m]->value = value;
                return;
        }

        /* Same key: repace */
        if (!strcmp(hm->data[m]->key, key)) {
                hm->data[m]->value = value;
                return;
        }

        /* have to grow main table */
        if (hm->d == hm->data[m]->d) {
                grow(hm);
                return dhmadd(hm, key, value);
        }

        /* have to split bucket */
        DHnode prev = *hm->data[m];
        int d = 1 << prev.d;
        int i;

        hm->data[m]->d++;
        hm->data[m]->key = NULL;
        hm->data[m]->value = NULL;

        hm->data[m ^ d] = calloc(1, sizeof(DHnode));
        *hm->data[m ^ d] = *hm->data[m];

        for (i = m ^ d; i < (1 << hm->d); i += d << 1) {
                hm->data[i] = hm->data[m ^ d];
        }

        dhmadd(hm, prev.key, prev.value);
        free(prev.key);
        return dhmadd(hm, key, value);
}

void
dhmremove(DHmap *hm, char *key)
{
        assert("No yet implemented dhmremove" && 0);
}

void *
dhmget(DHmap *hm, char *key)
{
        if (hm == NULL) return NULL;
        if (key == NULL) return NULL;
        if (hm->data == NULL) return NULL;
        if (*hm->data == NULL) return NULL;

        int k = dhmhash(key);
        int m = hashmask(k, hm->d);

        if (hm->data[m]->key == NULL) return NULL;

        if (!strcmp(hm->data[m]->key, key)) return hm->data[m]->value;

        return NULL;
}

void
dhmdestroy(DHmap *hm)
{
        int i, j;
        if (hm == NULL) return;
        if (hm->data == NULL) return;
        for (i = 0; i < (1 << hm->d); i++) {
                if (hm->data[i] == NULL) continue;
                if (hm->data[i]->key == NULL) continue;
                free(hm->data[i]->key);
                hm->data[i]->key = NULL;
                for (j = i; j < (1 << hm->d); j++) {
                        if (hm->data[i] == hm->data[j])
                                hm->data[j] = NULL;
                }
                free(hm->data[i]);
        }
        free(hm->data);
        hm->data = NULL;
        hm->d = 0;
}

void
dhmprint(DHmap hm)
{
        int i;
        printf("------------------------------\n");
        for (i = 0; i < (1 << hm.d); i++) {
                printf("[%2d] --> %s = %p\n",
                       i, hm.data[i]->key, hm.data[i]->value);
        }
        printf("------------------------------\n");
}
