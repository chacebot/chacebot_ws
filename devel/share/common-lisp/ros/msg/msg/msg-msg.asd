
(cl:in-package :asdf)

(defsystem "msg-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "RCLocal" :depends-on ("_package_RCLocal"))
    (:file "_package_RCLocal" :depends-on ("_package"))
  ))