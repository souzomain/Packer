package main

import (
    "fmt"
    "github.com/souzomain/Packer/go"
)

func main() {
	p := packer.NewPacker()
	p.AddInt(42)
	p.AddStr("Hello, World!")
	p.AddData([]byte{1, 2, 3})
	p.Dump()

	parser := parser.NewParser(p.GetBuffer(), p.GetSize())
	fmt.Println(parser.ParseInt())
	fmt.Println(parser.ParseStr())
	fmt.Println(parser.ParseBytes())
}
