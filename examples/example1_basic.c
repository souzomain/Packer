#include "../Packer.h"
#include <stdio.h>

int main(int argc, char **argv){
    PPACKER packer = packer_init();
    if(!packer){ printf("Error in packer initialization"); return 1; }
    packer_add_string(packer, "hello world");
    packer_add_string(packer, "test");
    packer_add_string(packer, "test1");
    packer_add_string(packer, "test23");
    packer_add_string(packer, "test45678");

    size_t offset = 0;
    printf("%s\n", packer_get_string(packer->buffer, &offset));
    printf("%s\n", packer_get_string(packer->buffer, &offset));
    printf("%s\n", packer_get_string(packer->buffer, &offset));
    printf("%s\n", packer_get_string(packer->buffer, &offset));
    printf("%s\n", packer_get_string(packer->buffer, &offset));

    packer_free(packer);
    return 0;
}
