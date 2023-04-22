package packer

import (
	"encoding/binary"
)

type Parser struct {
	buffer []byte
	length int
}

func NewParser(buffer []byte, length int) *Parser {
	return &Parser{
		buffer: buffer,
		length: length,
	}
}

func (p *Parser) ParseInt() int32 {
	data := binary.LittleEndian.Uint32(p.buffer[:4])
	p.buffer = p.buffer[4:]
	return int32(data)
}

func (p *Parser) ParseBytes() []byte {
	length := int(binary.LittleEndian.Uint32(p.buffer[:4]))
	p.buffer = p.buffer[4:]
	data := p.buffer[:length]
	p.buffer = p.buffer[length:]
	return data
}

func (p *Parser) ParseStr() string {
	data := p.ParseBytes()
	return string(data)
}
