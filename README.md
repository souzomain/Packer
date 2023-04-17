# Overview

Packer is a compact, fast and crosss-platform serialization library for store data in a buffer.

The packer lib, works on several projects. You can build your protocol in an easy way using the packer!

example:

```c
PPACKER protocol = packer_init();
packer_add_data(protocol, information, sizeof(information));
send(fd, protocol->buffer, protocol->offset, 0);
packer_free(protocol);
```

You can find more examples in example folder.

* cross-platform

# Compilation

you can compile using gcc or any other C/C++ compiler.

nix example:
gcc yourfile.c Packer.c

Simple!

in windows you have to add the flag -lws2_32

# Thanks

Thanks to the demon of havoc framework, this repository is wonderful!
