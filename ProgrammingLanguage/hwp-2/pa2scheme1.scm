(define c
  (lambda (L)
    (append L (reverse L))
  )
)

(display (c '(1 2 3)))
