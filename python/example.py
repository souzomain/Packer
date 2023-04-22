from Packer import Packer, Parser

packer = Packer()
packer.add_str("hello world")

print(f"packet size: {packer.get_size()} | packet: {packer.get_buffer()}")

parser = Parser(packer.get_buffer(), packer.get_size())
print(parser.parse_str())
