;; Auto-generated. Do not edit!


(when (boundp 'msg::RCLocal)
  (if (not (find-package "MSG"))
    (make-package "MSG"))
  (shadow 'RCLocal (find-package "MSG")))
(unless (find-package "MSG::RCLOCAL")
  (make-package "MSG::RCLOCAL"))

(in-package "ROS")
;;//! \htmlinclude RCLocal.msg.html


(defclass msg::RCLocal
  :super ros::object
  :slots (_steering _drive _status ))

(defmethod msg::RCLocal
  (:init
   (&key
    ((:steering __steering) 0)
    ((:drive __drive) 0)
    ((:status __status) nil)
    )
   (send-super :init)
   (setq _steering (round __steering))
   (setq _drive (round __drive))
   (setq _status __status)
   self)
  (:steering
   (&optional __steering)
   (if __steering (setq _steering __steering)) _steering)
  (:drive
   (&optional __drive)
   (if __drive (setq _drive __drive)) _drive)
  (:status
   (&optional __status)
   (if __status (setq _status __status)) _status)
  (:serialization-length
   ()
   (+
    ;; int16 _steering
    2
    ;; int16 _drive
    2
    ;; bool _status
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; int16 _steering
       (write-word _steering s)
     ;; int16 _drive
       (write-word _drive s)
     ;; bool _status
       (if _status (write-byte -1 s) (write-byte 0 s))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; int16 _steering
     (setq _steering (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; int16 _drive
     (setq _drive (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; bool _status
     (setq _status (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;;
   self)
  )

(setf (get msg::RCLocal :md5sum-) "aad865e688304e7732de05868a861e8e")
(setf (get msg::RCLocal :datatype-) "msg/RCLocal")
(setf (get msg::RCLocal :definition-)
      "int16 steering
int16 drive
bool status



")



(provide :msg/RCLocal "aad865e688304e7732de05868a861e8e")

