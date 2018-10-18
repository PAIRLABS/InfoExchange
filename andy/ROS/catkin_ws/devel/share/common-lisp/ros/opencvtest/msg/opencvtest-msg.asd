
(cl:in-package :asdf)

(defsystem "opencvtest-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :sensor_msgs-msg
)
  :components ((:file "_package")
    (:file "image" :depends-on ("_package_image"))
    (:file "_package_image" :depends-on ("_package"))
  ))