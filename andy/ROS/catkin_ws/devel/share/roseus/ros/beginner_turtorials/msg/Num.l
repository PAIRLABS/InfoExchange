;; Auto-generated. Do not edit!


(when (boundp 'beginner_turtorials::Num)
  (if (not (find-package "BEGINNER_TURTORIALS"))
    (make-package "BEGINNER_TURTORIALS"))
  (shadow 'Num (find-package "BEGINNER_TURTORIALS")))
(unless (find-package "BEGINNER_TURTORIALS::NUM")
  (make-package "BEGINNER_TURTORIALS::NUM"))

(in-package "ROS")
;;//! \htmlinclude Num.msg.html


(defclass beginner_turtorials::Num
  :super ros::object
  :slots (_num1 _num2 ))

(defmethod beginner_turtorials::Num
  (:init
   (&key
    ((:num1 __num1) 0)
    ((:num2 __num2) 0)
    )
   (send-super :init)
   (setq _num1 (round __num1))
   (setq _num2 (round __num2))
   self)
  (:num1
   (&optional __num1)
   (if __num1 (setq _num1 __num1)) _num1)
  (:num2
   (&optional __num2)
   (if __num2 (setq _num2 __num2)) _num2)
  (:serialization-length
   ()
   (+
    ;; int64 _num1
    8
    ;; int64 _num2
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int64 _num1
#+(or :alpha :irix6 :x86_64)
       (progn (sys::poke _num1 (send s :buffer) (send s :count) :long) (incf (stream-count s) 8))
#-(or :alpha :irix6 :x86_64)
       (cond ((and (class _num1) (= (length (_num1 . bv)) 2)) ;; bignum
              (write-long (ash (elt (_num1 . bv) 0) 0) s)
              (write-long (ash (elt (_num1 . bv) 1) -1) s))
             ((and (class _num1) (= (length (_num1 . bv)) 1)) ;; big1
              (write-long (elt (_num1 . bv) 0) s)
              (write-long (if (>= _num1 0) 0 #xffffffff) s))
             (t                                         ;; integer
              (write-long _num1 s)(write-long (if (>= _num1 0) 0 #xffffffff) s)))
     ;; int64 _num2
#+(or :alpha :irix6 :x86_64)
       (progn (sys::poke _num2 (send s :buffer) (send s :count) :long) (incf (stream-count s) 8))
#-(or :alpha :irix6 :x86_64)
       (cond ((and (class _num2) (= (length (_num2 . bv)) 2)) ;; bignum
              (write-long (ash (elt (_num2 . bv) 0) 0) s)
              (write-long (ash (elt (_num2 . bv) 1) -1) s))
             ((and (class _num2) (= (length (_num2 . bv)) 1)) ;; big1
              (write-long (elt (_num2 . bv) 0) s)
              (write-long (if (>= _num2 0) 0 #xffffffff) s))
             (t                                         ;; integer
              (write-long _num2 s)(write-long (if (>= _num2 0) 0 #xffffffff) s)))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int64 _num1
#+(or :alpha :irix6 :x86_64)
      (setf _num1 (prog1 (sys::peek buf ptr- :long) (incf ptr- 8)))
#-(or :alpha :irix6 :x86_64)
      (setf _num1 (let ((b0 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4)))
                  (b1 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4))))
              (cond ((= b1 -1) b0)
                     ((and (= b1  0)
                           (<= lisp::most-negative-fixnum b0 lisp::most-positive-fixnum))
                      b0)
                    ((= b1  0) (make-instance bignum :size 1 :bv (integer-vector b0)))
                    (t (make-instance bignum :size 2 :bv (integer-vector b0 (ash b1 1)))))))
   ;; int64 _num2
#+(or :alpha :irix6 :x86_64)
      (setf _num2 (prog1 (sys::peek buf ptr- :long) (incf ptr- 8)))
#-(or :alpha :irix6 :x86_64)
      (setf _num2 (let ((b0 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4)))
                  (b1 (prog1 (sys::peek buf ptr- :integer) (incf ptr- 4))))
              (cond ((= b1 -1) b0)
                     ((and (= b1  0)
                           (<= lisp::most-negative-fixnum b0 lisp::most-positive-fixnum))
                      b0)
                    ((= b1  0) (make-instance bignum :size 1 :bv (integer-vector b0)))
                    (t (make-instance bignum :size 2 :bv (integer-vector b0 (ash b1 1)))))))
   ;;
   self)
  )

(setf (get beginner_turtorials::Num :md5sum-) "0b7233452eef3749ca025e90606b0956")
(setf (get beginner_turtorials::Num :datatype-) "beginner_turtorials/Num")
(setf (get beginner_turtorials::Num :definition-)
      "int64 num1
int64 num2

")



(provide :beginner_turtorials/Num "0b7233452eef3749ca025e90606b0956")


