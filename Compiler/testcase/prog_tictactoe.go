var game[9] int

func int winner()
{
	var x int

	for(var i int = 0; i < 3; i += 1) {
		x = game[i]
		if(x != 0 && x == game[i + 1] && x == game[i + 2]) {
			return x
		}

		x = game[i * 3]
		if(x != 0 && x == game[i + 3] && x == game[i + 6]) {
			return x
		}
	}

	x = game[4]
	if(x == 0) {
		return -1
	}

	if(x == game[0] && x == game[8]) {
		return x
	}
	if(x == game[2] && x == game[6]) {
		return x
	}

	return -1
}

func string p(x int)
{
	if(x == 0) {
		return " "
	} else if (x == 1) {
		return "X"
	} else {
		return "O"
	}
}

func void row(r int)
{
	println " " + p(game[r]) + " | " + p(game[r + 1]) + " | " + p(game[r + 2])
}

func void draw()
{
	println ""
	row(0)
	println "---+---+---"
	row(3)
	println "---+---+---"
	row(6)
	println ""
}

func void put(p int)
{
	var i int
	for(true) {
		read i
		if(i > 0 && i <= 9) {
			if(game[i - 1] == 0) {
				game[i - 1] = p
				return
			}
		}

		print "Invalid choice, retry: "
	}
}

func void main()
{
	var r int = 0
	var i int
	var player int
	var w int

	for(winner() == -1 && r < 9) {
		player = 1 + r % 2
		draw()
		print "Round for player " + player + ", choose a point: "
		put(player)
		r += 1
	}

	draw()

	w = winner()
	if(w == -1) {
		println "no winner :("
	} else {
		println "winner is " + w
	}
}
