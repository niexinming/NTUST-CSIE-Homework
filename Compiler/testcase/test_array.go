var gg[87] int
var g_real_a[5] real
var g_string_a[5] string

func void main( ) {
	println "test global array"
	gg[76] = 99
	println gg[0]
	println gg[76]

	g_real_a[0] = 99.9999
	g_real_a[1] = 88.8888
	println g_real_a[0]
	println g_real_a[1]

	g_string_a[0] = "Hello"
	g_string_a[1] = "World"
	for(var q int = 0; q < 2; q += 1) {
		print g_string_a[q]
	}
	println ""

	println "test local array"
	var c[5] int
	var s[5] string

	c[0] = 2
	for(var i int = 0; i < 5; i += 1) {
		c[i] = i + 10
		s[i] = "> " + c[i]
	}

	for(var j int = 4; j >= 0; j -= 1) {
		println c[j]
	}

	for(var k int = 0; k < 17; k += 1) {
		println (k % 5) + ": " + c[k % 5]
	}

	for(var l int = 0; l < 5; l += 1) {
		println "string array: " + s[l]
	}
}
