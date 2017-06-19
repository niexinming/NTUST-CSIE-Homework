func void main( ) {
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
