#include <stdio.h>
#include <stdlib.h>
// add
int main() {
    int *m = calloc(5,sizeof(int));
    for (int i = 0; i < 10; ++i) {
        m[i] = i;
    }
    for (int i = 0; i < 10; ++i) {
        printf("%d\n",m[i]);
    } 
    
    m = realloc(m, 4 * sizeof(int));
    for (int i = 0; i < 10; ++i) {
        printf("%d\n",m[i]);
    } 
    free(m);
    return 0;
}
