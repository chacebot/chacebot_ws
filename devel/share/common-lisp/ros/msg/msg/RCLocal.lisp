; Auto-generated. Do not edit!


(cl:in-package msg-msg)


;//! \htmlinclude RCLocal.msg.html

(cl:defclass <RCLocal> (roslisp-msg-protocol:ros-message)
  ((steering
    :reader steering
    :initarg :steering
    :type cl:fixnum
    :initform 0)
   (drive
    :reader drive
    :initarg :drive
    :type cl:fixnum
    :initform 0)
   (status
    :reader status
    :initarg :status
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass RCLocal (<RCLocal>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <RCLocal>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'RCLocal)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name msg-msg:<RCLocal> is deprecated: use msg-msg:RCLocal instead.")))

(cl:ensure-generic-function 'steering-val :lambda-list '(m))
(cl:defmethod steering-val ((m <RCLocal>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader msg-msg:steering-val is deprecated.  Use msg-msg:steering instead.")
  (steering m))

(cl:ensure-generic-function 'drive-val :lambda-list '(m))
(cl:defmethod drive-val ((m <RCLocal>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader msg-msg:drive-val is deprecated.  Use msg-msg:drive instead.")
  (drive m))

(cl:ensure-generic-function 'status-val :lambda-list '(m))
(cl:defmethod status-val ((m <RCLocal>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader msg-msg:status-val is deprecated.  Use msg-msg:status instead.")
  (status m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <RCLocal>) ostream)
  "Serializes a message object of type '<RCLocal>"
  (cl:let* ((signed (cl:slot-value msg 'steering)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'drive)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 65536) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    )
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'status) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <RCLocal>) istream)
  "Deserializes a message object of type '<RCLocal>"
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'steering) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'drive) (cl:if (cl:< unsigned 32768) unsigned (cl:- unsigned 65536))))
    (cl:setf (cl:slot-value msg 'status) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<RCLocal>)))
  "Returns string type for a message object of type '<RCLocal>"
  "msg/RCLocal")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'RCLocal)))
  "Returns string type for a message object of type 'RCLocal"
  "msg/RCLocal")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<RCLocal>)))
  "Returns md5sum for a message object of type '<RCLocal>"
  "aad865e688304e7732de05868a861e8e")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'RCLocal)))
  "Returns md5sum for a message object of type 'RCLocal"
  "aad865e688304e7732de05868a861e8e")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<RCLocal>)))
  "Returns full string definition for message of type '<RCLocal>"
  (cl:format cl:nil "int16 steering~%int16 drive~%bool status~%~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'RCLocal)))
  "Returns full string definition for message of type 'RCLocal"
  (cl:format cl:nil "int16 steering~%int16 drive~%bool status~%~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <RCLocal>))
  (cl:+ 0
     2
     2
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <RCLocal>))
  "Converts a ROS message object to a list"
  (cl:list 'RCLocal
    (cl:cons ':steering (steering msg))
    (cl:cons ':drive (drive msg))
    (cl:cons ':status (status msg))
))
