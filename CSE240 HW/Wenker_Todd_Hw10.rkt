#lang racket
;;Todd Wenker
;;CSE240 Chen
;;Homework 10
;;11/23/14
(define list-push-front (lambda (lst new-list)(append new-list lst)))
(list-push-front '(4 3 7 1 2 9) '(1 2))

(define list-push-back (lambda (lst new-list)(append lst new-list)))
(list-push-back '(4 3 7 1 2 9) '(1 2))

(define list-draw-front (lambda (n lst)
                          (drop-right lst (- (length lst) n)) 
                        )
  )
(list-draw-front 3 '(4 3 7 1 2 9))

(define list-draw-back(lambda (n lst)
                        (take-right lst n)
                       )
)                              
(list-draw-back 3 '(4 3 7 1 2 9))

(define list-shuffle (lambda (lst1 lst2)
                       (cond ((and (empty? lst1) (empty? lst2)) '())
                             (else (list* (car lst1) (car lst2) (list-shuffle(cdr
                                 lst1) (cdr lst2))))                               
                       )
                      ) 
  )
(list-shuffle '(a b) '(1 2))
                              