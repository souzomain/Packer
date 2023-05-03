import struct

class Packer:
    def __init__(self):
        self.__buffer__ = bytearray()

    def add_str(self, value : str):
        value_bytes = value.encode('utf-8')
        length = len(value_bytes)
        self.add_uint32(length)
        self.__buffer__.extend(value_bytes)

    def add_data(self, value : bytes):
        length = len(value)
        self.add_uint32(length)
        self.__buffer__.extend(value)

    def add_str64(self, value : str):
        value_bytes = value.encode('utf-8')
        length = len(value_bytes)
        self.add_uint64(length)
        self.__buffer__.extend(value_bytes)

    def add_data64(self, value : bytes):
        length = len(value)
        self.add_uint64(length)
        self.__buffer__.extend(value)

    def add_int32(self, value : int):
        self.__buffer__.extend(struct.pack('!i', value))

    def add_uint32(self, value: int):
        self.__buffer__.extend(struct.pack('!I', value))

    def add_int64(self, value : int):
        self.__buffer__.extend(struct.pack('!q', value))

    def add_uint64(self, value: int):
        self.__buffer__.extend(struct.pack('!Q', value))

    def get_buffer(self):
        return bytes(self.__buffer__)

class Parser:
    def __init__(self, buffer):
        self.buffer = buffer
        self.offset = 0

    def get_str(self):
        length = self.get_uint32()
        value = self.buffer[self.offset:self.offset + length]
        self.offset += length
        return value.decode('utf-8')

    def get_data(self):
        length = self.get_uint32()
        value = self.buffer[self.offset:self.offset + length]
        self.offset += length
        return value

    def get_str64(self):
        length = self.get_uint64()
        value = self.buffer[self.offset:self.offset + length]
        self.offset += length
        return value.decode('utf-8')

    def get_data64(self):
        length = self.get_uint64()
        value = self.buffer[self.offset:self.offset + length]
        self.offset += length
        return value

    def get_int32(self):
        value = struct.unpack('!i', self.buffer[self.offset:self.offset + 4])[0]
        self.offset += 4
        return value

    def get_uint32(self):
        value = struct.unpack('!I', self.buffer[self.offset:self.offset + 4])[0]
        self.offset += 4
        return value

    def get_int64(self):
        value = struct.unpack('!q', self.buffer[self.offset:self.offset + 8])[0]
        self.offset += 8
        return value

    def get_uint64(self):
        value = struct.unpack('!Q', self.buffer[self.offset:self.offset + 8])[0]
        self.offset += 8
        return value
