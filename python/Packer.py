import struct

class Packer:
    def __init__(self):
        self.buffer = bytearray()

    def add_str(self, value : str):
        value_bytes = value.encode('utf-8')
        length = len(value_bytes)
        self.add_int32(length)
        self.buffer.extend(value_bytes)

    def add_data(self, value : bytes):
        length = len(value)
        self.add_int32(length)
        self.buffer.extend(value)

    def add_int32(self, value : int):
        self.buffer.extend(struct.pack('!i', value))

    def add_int64(self, value):
        self.buffer.extend(struct.pack('!q', value))

    def get_buffer(self):
        return bytes(self.buffer)

class Parser:
    def __init__(self, buffer):
        self.buffer = buffer
        self.offset = 0

    def get_str(self):
        length = self.get_int32()
        value = self.buffer[self.offset:self.offset + length]
        self.offset += length
        return value.decode('utf-8')

    def get_data(self):
        length = self.get_int32()
        value = self.buffer[self.offset:self.offset + length]
        self.offset += length
        return value

    def get_int32(self):
        value = struct.unpack('!i', self.buffer[self.offset:self.offset + 4])[0]
        self.offset += 4
        return value

    def get_int64(self):
        value = struct.unpack('!q', self.buffer[self.offset:self.offset + 8])[0]
        self.offset += 8
        return value
