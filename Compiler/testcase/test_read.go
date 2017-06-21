func void greeting(name string)
{
	println "Hello, " + name + "!"
}

func void main()
{
	var name string

	print "What's your name? >"
	read name

	greeting(name)

	print "Enter two number: "

	var a int
	var b int

	read a
	read b

	println "sum of int = " + (a + b)
}
