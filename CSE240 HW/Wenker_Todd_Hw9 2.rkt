#lang racket
;; Question 1
(+ 4 5)
(+ 4 5 6)
(+ (* 4 2) (* 8 8))
(+ (* 4 2) (* 9 9) (/ 18 2))
(/ (+ 2 4 6 8 10 12 14) (+ 2 4 6))
;; Question 2
(define four 4);binds the integer 4 to the text 'four'
(+ four 5)
(+ four 5 6)
(+ (* four 2) (* 8 8))
(+ (* four 2) (* 9 9) (/ 18 2))
(/ (+ 2 four 6 8 10 12 14) (+ 2 four 6))
;; Question 3
(define RectArea (lambda (a b) (* a b)))
;;This binds (lambda (a b) (* a b)) to RectArea. When calling RectArea, 2 integers
;;need to be inputed along with it. It then multiplies those two integers and
;;returns the product
(RectArea 4 5)
;; Question 4
(define RectVol (lambda (a b c) (* (RectArea a b) c)))
;;This binds RectVol to (lambda (a b c) (* (RectArea a b) c)). When RectVol is
;;called with 3 integers, the first two are sent to the RectArea function and
;;then the product is multiplied by c. 
(RectVol 3 4 5)
;; Question 5
(define DiffVolume (lambda (a b c d e f) (- (RectVol a b c) (RectVol d e f))))
;;This binds DiffVolume to (lambda (a b c d e f) (- (RectVol a b c)
;;(RectVol d e f)). When this is called, the first three integers are sent to
;;RectVol and the last three integers are sent to a different RectVol before
;;the differenc between the two products is found.
(DiffVolume 6 5 4 3 2 1)