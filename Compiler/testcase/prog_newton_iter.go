func real f(x real)
{
	return x * x - 1.0 * x - 6.0
}

func real p(x real)
{
	const dist = 0.001
	return (f(x + dist) - f(x - dist)) / (dist * 2)
}

func real abs(x real) {
	if(x < 0.0) {
		return -x
	} else {
		return x
	}
}

func real newton(x real)
{
	var c int = 0
	for(abs(f(x)) > 0.001 && c < 50) {
		println x
		c += 1
		x = x - f(x) / p(x)
	}

	return x
}

func void main()
{
	var x real
	println "f(x) = x^2 - x - 6"
	x = newton(9487)
	println "x = " + x + ", f(x) = " + f(x)
	x = newton(-777)
	println "x = " + x + ", f(x) = " + f(x)
}
