(define dbl_atm
  (lambda (lst atom)
	(display "Input: ")
	(write lst)
	(display "\n")
    (if (equal? (length lst) 0) ; empty? lst
      '()
       (if (list? (car lst)) ; if first element is list
         (cons
           ; apply `dbl_atm` on both first element (list) and other
           ; left elements, then concat them
           (dbl_atm (car lst) atom)
           (dbl_atm (cdr lst) atom)
           )
         ; else first element is not list
         (if (equal? (car lst) atom)
           ; if first elemnt equals atom
           (append
             ; duplicate atom
             (list atom atom)
             ; iter over left elements
             (dbl_atm (cdr lst) atom)
             )
           ; else, check other elements
           (cons
             ; put first element
             (car lst)
             ; iter left elements
             (dbl_atm (cdr lst) atom)
             )
           )
         )
      )
    )
  )

(write (dbl_atm '(1 2 (3 4) 3 4) 4))
