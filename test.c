#include "dhm.h"
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* If it doesn't show any output it end without any error */

int
main()
{
        DHmap hm = { 0 };
        assert(dhmget(&hm, NULL) == NULL);
        assert(dhmget(&hm, "hello") == NULL);
        assert(dhmget(&hm, "") == NULL);
        dhmadd(&hm, "hugo", (void *) 0x1);
        dhmadd(&hm, "hola", (void *) 0x2);
        dhmadd(&hm, "uno", (void *) 0x3);
        dhmadd(&hm, "dos", (void *) 0x4);
        assert(dhmget(&hm, "hugo") == (void *) 0x1);
        assert(dhmget(&hm, "hola") == (void *) 0x2);
        assert(dhmget(&hm, "uno") == (void *) 0x3);
        assert(dhmget(&hm, "dos") == (void *) 0x4);
        dhmdestroy(&hm);
        assert(dhmget(&hm, NULL) == NULL);
        assert(dhmget(&hm, "hello") == NULL);
        assert(dhmget(&hm, "") == NULL);
        dhmadd(&hm, "hugo", (void *) 0x1);
        dhmadd(&hm, "hola", (void *) 0x2);
        dhmadd(&hm, "uno", (void *) 0x3);
        dhmadd(&hm, "dos", (void *) 0x4);
        assert(dhmget(&hm, "hugo") == (void *) 0x1);
        assert(dhmget(&hm, "hola") == (void *) 0x2);
        assert(dhmget(&hm, "uno") == (void *) 0x3);
        assert(dhmget(&hm, "dos") == (void *) 0x4);
        dhmdestroy(&hm);
        return 0;
}
