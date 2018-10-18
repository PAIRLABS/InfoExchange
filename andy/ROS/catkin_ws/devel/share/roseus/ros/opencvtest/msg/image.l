;; Auto-generated. Do not edit!


(when (boundp 'opencvtest::image)
  (if (not (find-package "OPENCVTEST"))
    (make-package "OPENCVTEST"))
  (shadow 'image (find-package "OPENCVTEST")))
(unless (find-package "OPENCVTEST::IMAGE")
  (make-package "OPENCVTEST::IMAGE"))

(in-package "ROS")
;;//! \htmlinclude image.msg.html
(if (not (find-package "SENSOR_MSGS"))
  (ros::roseus-add-msgs "sensor_msgs"))


(defclass opencvtest::image
  :super ros::object
  :slots (_data _ros_image ))

(defmethod opencvtest::image
  (:init
   (&key
    ((:data __data) "")
    ((:ros_image __ros_image) (instance sensor_msgs::Image :init))
    )
   (send-super :init)
   (setq _data (string __data))
   (setq _ros_image __ros_image)
   self)
  (:data
   (&optional __data)
   (if __data (setq _data __data)) _data)
  (:ros_image
   (&rest __ros_image)
   (if (keywordp (car __ros_image))
       (send* _ros_image __ros_image)
     (progn
       (if __ros_image (setq _ros_image (car __ros_image)))
       _ros_image)))
  (:serialization-length
   ()
   (+
    ;; string _data
    4 (length _data)
    ;; sensor_msgs/Image _ros_image
    (send _ros_image :serialization-length)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _data
       (write-long (length _data) s) (princ _data s)
     ;; sensor_msgs/Image _ros_image
       (send _ros_image :serialize s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _data
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _data (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; sensor_msgs/Image _ros_image
     (send _ros_image :deserialize buf ptr-) (incf ptr- (send _ros_image :serialization-length))
   ;;
   self)
  )

(setf (get opencvtest::image :md5sum-) "c52d0b491ac982e395f7f606723c9815")
(setf (get opencvtest::image :datatype-) "opencvtest/image")
(setf (get opencvtest::image :definition-)
      "string data
sensor_msgs/Image ros_image

================================================================================
MSG: sensor_msgs/Image
# This message contains an uncompressed image
# (0, 0) is at top-left corner of image
#

Header header        # Header timestamp should be acquisition time of image
                     # Header frame_id should be optical frame of camera
                     # origin of frame should be optical center of cameara
                     # +x should point to the right in the image
                     # +y should point down in the image
                     # +z should point into to plane of the image
                     # If the frame_id here and the frame_id of the CameraInfo
                     # message associated with the image conflict
                     # the behavior is undefined

uint32 height         # image height, that is, number of rows
uint32 width          # image width, that is, number of columns

# The legal values for encoding are in file src/image_encodings.cpp
# If you want to standardize a new string format, join
# ros-users@lists.sourceforge.net and send an email proposing a new encoding.

string encoding       # Encoding of pixels -- channel meaning, ordering, size
                      # taken from the list of strings in include/sensor_msgs/image_encodings.h

uint8 is_bigendian    # is this data bigendian?
uint32 step           # Full row length in bytes
uint8[] data          # actual matrix data, size is (step * rows)

================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
# 0: no frame
# 1: global frame
string frame_id

")



(provide :opencvtest/image "c52d0b491ac982e395f7f606723c9815")


