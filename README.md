# Overview

Simple, fast and Light-weigh, Packer is a crosss-platform serialization C library.

The packer library, works on several projects. You can build your protocol in an easy way using the packer!

example:

```c
PPACKER protocol = packer_init();
packer_add_data(protocol, information, sizeof(information));
send(fd, packer_get_buffer(protocol), packer_get_size(protocol), 0);
packer_free(protocol);
```

and, you can get values stored in a buffer

example:

```c
recv(fd, buffer, size, 0);
size_t offset = 0;
char *value1 = packer_get_string(buffer, &offset);
int value2 = packer_get_int32(buffer, &offset);
void *value3 = packer_get_data(buffer, value2, &offset);
```

You can find more examples in example folder.

* cross-platform

# Compilation

you can compile using gcc or any other C/C++ compiler.

nix example:
gcc yourfile.c Packer.c

Simple!

# Python support

This serialization library have python support and other languages

```python
from Packer import Packer, Parser

packer = Packer()
packer.add_str("hello world")

print(f"packet size: {packer.get_size()} | packet: {packer.get_buffer()}")

parser = Parser(packer.get_buffer(), packer.get_size())
print(parser.parse_str())
```

more examples from other languages in example folder 

# Thanks

Thanks to the Havoc Framework and its contributors, this repository is wonderful!
