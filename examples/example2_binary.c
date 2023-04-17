#include "../Packer.h"

#include <stdio.h>
#include <string.h>

typedef struct{
    char name[12];
    int age;
}ExampleData;

int main(int argc, char **argv){
    ExampleData data;
    strcpy(data.name, "hello world");
    data.age = 55;

    PPACKER protocol = packer_init();
    packer_add_data(protocol,&data,sizeof(ExampleData));
    packer_add_string(protocol, "hello world2");

    size_t prot_len = 0;
    size_t offset = 0;
    ExampleData *receive = (ExampleData *)packer_get_data(protocol->buffer,&prot_len,&offset);
    printf("name: %s\n", receive->name);
    printf("age: %d\n", receive->age);
    
    printf("str: %s", packer_get_string(protocol->buffer, &offset));
}
