func int test_endless()
{
	var i int = 0
	var n int = 0

	for(true) {
		i += 1
		n += i
		if(i >= 10) {
			return n
		}
	}
}

func int test_onecond()
{
	var i int = 0
	for(i < 10) {
		i += 1
	}
	return i
}

func int test_fortwo()
{
	var i int = 10
	for(i >= 0; i -= 1) {
		print ""
	}
	return i
}

func int test_fortwoinit()
{
	var n int = 0
	for(var i int = 5; i >= 0; ) {
		i -= 1
		n += i
	}

	return n
}

func int test_double_for()
{
	var v int = 0

	for(var x int = 0; x < 10; x += 1) {
		for(var y int = 0; y < 5; y += 1) {
			v += 1
		}
	}

	return v
}

func void test(pass bool)
{
	if(pass) {
		println "good" } else {
		println "error"
	}
}

func void main()
{
	test(test_endless() == 55)
	test(test_onecond() == 10)
	test(test_fortwo() == -1)
	test(test_fortwoinit() == 9)
	test(test_double_for() == 50)
}
