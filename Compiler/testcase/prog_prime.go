func void main()
{
	var primes[1000] int
	var ps int = 1
	primes[0] = 2

	for(var i int = 3; ps < 1000; i += 2) {
		var is_prime bool = true
		for(var j int = 0; j < ps; j += 1) {
			if(i % primes[j] == 0) {
				is_prime = false
			}
		}

		if(is_prime) {
			primes[ps] = i
			ps += 1
		}
	}

	println "first 100 primes:"
	for(var k int = 0; k < 100; k += 1) {
		print primes[k] + ", "
	}
}
