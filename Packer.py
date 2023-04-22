#Code ported from https://github.com/HavocFramework/havoc-py

from struct import pack,calcsize, unpack

class Packer:

    def __init__(self):
        self.buffer = b''
        self.length = 0

    def get_buffer(self) -> bytes:
        return self.buffer

    def get_size(self) -> int:
        return self.length

    def get_buffer_with_length(self) -> bytes:
        return pack("<L", self.length ) + self.buffer

    def _encode_utf8(self, data: str) -> bytes:
        if isinstance(data, str):
            return data.encode("utf-8")
        return data

    def add_int(self, data : int ) -> None:
        self.buffer += pack("<i", data)
        self.length += 4
        return

    def add_str(self, data: str) -> None:
        encoded_data = self._encode_utf8(data)
        fmt = f"<L{len(encoded_data)}s"
        self.buffer += pack(fmt, len(encoded_data) + 1, encoded_data + b'\x00')
        self.length += calcsize(fmt)

    def add_data(self, data: bytes) -> None:
        fmt = f"<L{len(data)}s"
        self.buffer += pack(fmt, len(data), data)
        self.length += calcsize(fmt)
        
    def dump(self) -> None:
        print( f"[*] Buffer: [{ self.length }] [{ self.get_buffer() }]" )
        return


class Parser:
    buffer: bytes = b''
    length: int = 0

    def __init__(self, buffer, length):
        self.buffer = buffer
        self.length = length
        return

    def parse_int(self) -> int:
        val = unpack( ">i", self.buffer[ :4 ])
        self.buffer = self.buffer[4:]
        return val[ 0 ]

    def parse_bytes( self ) -> bytes:
        length      = self.parse_int()
        buf         = self.buffer[:length]
        self.buffer = self.buffer[length:]
        return buf

    def parse_str( self ) -> str:
        return self.parse_bytes().decode('utf-8')
