func int fib(x int)
{
	if(x <= 2) {
		return 1
	} else {
		return fib(x - 1) + fib(x - 2)
	}
}

func void main()
{
	println "fib calcualtor"
	var n int = 0

	for(n == -1) {
		print "Input n>"
		read n

		print "fib("
		print n
		print ") = "
		println fib(n)
	}
}
