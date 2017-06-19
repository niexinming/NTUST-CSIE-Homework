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
	println "-- fib calcualtor --"
	var n int

	for(n = 1; n < 15; n += 1) {
		println "fib(" + n + ") = " + fib(n)
	}
}
