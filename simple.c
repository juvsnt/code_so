#include <stdio.h>
#include <stdlib.h>

/* Essa funcao é chamada quando o módulo é carregado. */
int simple_init(void) {
    printf("Loading Module\n");
    return 0;
}

/* Essa função é chamada quando o módulo é removido. */
void simple_exit(void) {
    printf("Removing Module\n");
}

/* Macros para o registro dos pontos de entrada e saída do módulo. */
#define module_init(x) int main() { return x(); }
#define module_exit(x) void __attribute__((destructor)) cleanup() { x(); }

/* Registro dos pontos de entrada e saída. */
module_init(simple_init);
module_exit(simple_exit);

/* Informações sobre o módulo */
#define MODULE_LICENSE(x)
#define MODULE_DESCRIPTION(x)
#define MODULE_AUTHOR(x)

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");
