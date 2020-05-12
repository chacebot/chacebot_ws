// Generated by gencpp from file gopigo3_node/SPIResponse.msg
// DO NOT EDIT!


#ifndef GOPIGO3_NODE_MESSAGE_SPIRESPONSE_H
#define GOPIGO3_NODE_MESSAGE_SPIRESPONSE_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace gopigo3_node
{
template <class ContainerAllocator>
struct SPIResponse_
{
  typedef SPIResponse_<ContainerAllocator> Type;

  SPIResponse_()
    : data_in()  {
    }
  SPIResponse_(const ContainerAllocator& _alloc)
    : data_in(_alloc)  {
  (void)_alloc;
    }



   typedef std::vector<uint8_t, typename ContainerAllocator::template rebind<uint8_t>::other >  _data_in_type;
  _data_in_type data_in;





  typedef boost::shared_ptr< ::gopigo3_node::SPIResponse_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::gopigo3_node::SPIResponse_<ContainerAllocator> const> ConstPtr;

}; // struct SPIResponse_

typedef ::gopigo3_node::SPIResponse_<std::allocator<void> > SPIResponse;

typedef boost::shared_ptr< ::gopigo3_node::SPIResponse > SPIResponsePtr;
typedef boost::shared_ptr< ::gopigo3_node::SPIResponse const> SPIResponseConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::gopigo3_node::SPIResponse_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::gopigo3_node::SPIResponse_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace gopigo3_node

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': False, 'IsMessage': True, 'HasHeader': False}
// {'std_msgs': ['/opt/ros/kinetic/share/std_msgs/cmake/../msg'], 'gopigo3_node': ['/home/chace/Github/chacebot/containers/ros/src/gopigo3_node/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::gopigo3_node::SPIResponse_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::gopigo3_node::SPIResponse_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::gopigo3_node::SPIResponse_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::gopigo3_node::SPIResponse_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::gopigo3_node::SPIResponse_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::gopigo3_node::SPIResponse_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::gopigo3_node::SPIResponse_<ContainerAllocator> >
{
  static const char* value()
  {
    return "a70bf8ea6ca66d7c5a18abead186e47d";
  }

  static const char* value(const ::gopigo3_node::SPIResponse_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xa70bf8ea6ca66d7cULL;
  static const uint64_t static_value2 = 0x5a18abead186e47dULL;
};

template<class ContainerAllocator>
struct DataType< ::gopigo3_node::SPIResponse_<ContainerAllocator> >
{
  static const char* value()
  {
    return "gopigo3_node/SPIResponse";
  }

  static const char* value(const ::gopigo3_node::SPIResponse_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::gopigo3_node::SPIResponse_<ContainerAllocator> >
{
  static const char* value()
  {
    return "uint8[] data_in\n\
\n\
";
  }

  static const char* value(const ::gopigo3_node::SPIResponse_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::gopigo3_node::SPIResponse_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.data_in);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct SPIResponse_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::gopigo3_node::SPIResponse_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::gopigo3_node::SPIResponse_<ContainerAllocator>& v)
  {
    s << indent << "data_in[]" << std::endl;
    for (size_t i = 0; i < v.data_in.size(); ++i)
    {
      s << indent << "  data_in[" << i << "]: ";
      Printer<uint8_t>::stream(s, indent + "  ", v.data_in[i]);
    }
  }
};

} // namespace message_operations
} // namespace ros

#endif // GOPIGO3_NODE_MESSAGE_SPIRESPONSE_H
