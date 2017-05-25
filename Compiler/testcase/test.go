/* Sigma.go
 *
 * Compute sum = 1 + 2 + ... + n
 */

// variables
const n = 10
var sum int = 0
var index int

func void test_func(sum int) {
	println "sum conflict"
}

// main function
func void sigma ( ) {
  for (index = 0; index <= n; ) {
    sum = sum + index
    index = index + 1
  }
  print "The sum is "
  println sum
}



/* fib.sc
 *
 * This test program computes the Nth Fibonacci number
 */

// variables
const Fn = 1
// var n int  = 8
var FNminus1 int = 1

// compute the nth Fibonacci number
func void fib_main( ) {
  for (n > 2; n = n - 1) {
	var temp int
    temp = Fn
    Fn = Fn + FNminus1
    FNminus1 = temp
  }

  /* print result */
  print "Result of computation: "
  println n
}

/*
 * Example with Functions
 */

// variables
var a int = 5

// function declaration
func int add(A int, b int) {
  return A+b
}

// main function
func void main( ) {
	var c int
  c = add(a, 10)
  if (c > 10) {
    print -c
  }
  else {
    print c
  }
  println "Hello World"
}
