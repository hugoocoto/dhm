#ifndef DHM_H_
#define DHM_H_

typedef struct {
        char *key;
        void *value;
        int d;
} DHnode;

typedef struct {
        int d;
        DHnode **data;
} DHmap;

void dhmadd(DHmap *, char *key, void *value);
void dhmremove(DHmap *, char *key);
void *dhmget(DHmap *, char *key);
void dhmdestroy(DHmap *);
void dhmprint(DHmap hm);

#endif //! DHM_H_
