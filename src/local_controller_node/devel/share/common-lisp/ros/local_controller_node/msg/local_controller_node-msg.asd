
(cl:in-package :asdf)

(defsystem "local_controller_node-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "RControllerCommands" :depends-on ("_package_RControllerCommands"))
    (:file "_package_RControllerCommands" :depends-on ("_package"))
  ))