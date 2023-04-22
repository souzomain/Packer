package packer

import (
	"encoding/binary"
	"fmt"
)

type Packer struct {
	buffer []byte
	length int
}

func NewPacker() *Packer {
	return &Packer{
		buffer: make([]byte, 0),
		length: 0,
	}
}

func (p *Packer) GetBuffer() []byte {
	return p.buffer
}

func (p *Packer) GetSize() int {
	return p.length
}

func (p *Packer) GetBufferWithLength() []byte {
	lengthBytes := make([]byte, 4)
	binary.LittleEndian.PutUint32(lengthBytes, uint32(p.length))
	return append(lengthBytes, p.buffer...)
}

func (p *Packer) encodeUTF8(data interface{}) []byte {
	if str, ok := data.(string); ok {
		return []byte(str)
	}
	if bytes, ok := data.([]byte); ok {
		return bytes
	}
	return nil
}

func (p *Packer) AddInt(data int32) {
	dataBytes := make([]byte, 4)
	binary.LittleEndian.PutUint32(dataBytes, uint32(data))
	p.buffer = append(p.buffer, dataBytes...)
	p.length += 4
}

func (p *Packer) AddStr(data string) {
	encodedData := p.encodeUTF8(data)
	lengthBytes := make([]byte, 4)
	binary.LittleEndian.PutUint32(lengthBytes, uint32(len(encodedData)+1))
	p.buffer = append(p.buffer, lengthBytes...)
	p.buffer = append(p.buffer, encodedData...)
	p.buffer = append(p.buffer, 0x00)
	p.length += len(lengthBytes) + len(encodedData) + 1
}

func (p *Packer) AddData(data []byte) {
	lengthBytes := make([]byte, 4)
	binary.LittleEndian.PutUint32(lengthBytes, uint32(len(data)))
	p.buffer = append(p.buffer, lengthBytes...)
	p.buffer = append(p.buffer, data...)
	p.length += len(lengthBytes) + len(data)
}

func (p *Packer) Dump() {
	fmt.Printf("[*] Buffer: [%d] [%x]\n", p.length, p.buffer)
}

