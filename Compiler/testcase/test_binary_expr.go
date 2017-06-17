func void main()
{
	var a bool = true
	const b = false // auto typed const

	println "if(2333 >= 7122 || a == b)"
	if(2333 >= 7122 || a == b) {
		println "error"
	} else {
		println "good"
	}

	println "if(a)"
	if(a) {
		println "good"
	} else {
		println "error"
	}

	println "if(!a)"
	if(!a) {
		println "error"
	} else {
		println "good"
	}

	println "if(a && !b)"
	if(a && !b) {
		println "good"
	} else {
		println "error"
	}

	println "if(a == b)"
	if(a == b) {
		println "error"
	} else {
		println "good"
	}

	println "if(5 == 7)"
	if(5 == 7) {
		println "error"
	} else {
		println "good"
	}

	println "if(2333 < 7122 && a != b)"
	if(2333 < 7122 && a != b) {
		println "good"
	} else {
		println "error"
	}

	println "if(2333 < 7122 || a != b)"
	if(2333 < 7122 || a != b) {
		println "good"
	} else {
		println "error"
	}

	println "if(2333 >= 7122 || a == b)"
	if(2333 >= 7122 || a == b) {
		println "error"
	} else {
		println "good"
	}

	println "if(2333 >= 7122 && a == b || true)"
	if(2333 >= 7122 && a == b || true) {
		println "good"
	} else {
		println "error"
	}

	println "doing simple integer compare test"

	if(2 > 1) {
		println "good"
	} else {
		println "error"
	}

	if(2 >= 1) {
		println "good"
	} else {
		println "error"
	}

	if(2 != 1) {
		println "good"
	} else {
		println "error"
	}

	if(2 < 1) {
		println "error"
	} else {
		println "good"
	}

	if(2 <= 1) {
		println "error"
	} else {
		println "good"
	}

	if(2 == 1) {
		println "error"
	} else {
		println "good"
	}

	println "swap two int"

	if(1 > 2) {
		println "error"
	} else {
		println "good"
	}

	if(1 >= 2) {
		println "error"
	} else {
		println "good"
	}

	if(1 != 2) {
		println "good"
	} else {
		println "error"
	}

	if(1 < 2) {
		println "good"
	} else {
		println "error"
	}

	if(1 <= 2) {
		println "good"
	} else {
		println "error"
	}

	if(1 == 2) {
		println "error"
	} else {
		println "good"
	}
}
