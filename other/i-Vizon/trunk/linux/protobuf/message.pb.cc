// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: message.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "message.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::Descriptor* log_packet_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  log_packet_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_message_2eproto() {
  protobuf_AddDesc_message_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "message.proto");
  GOOGLE_CHECK(file != NULL);
  log_packet_descriptor_ = file->message_type(0);
  static const int log_packet_offsets_[7] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(log_packet, log_time_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(log_packet, log_micro_sec_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(log_packet, sequence_no_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(log_packet, shm_app_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(log_packet, packet_id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(log_packet, log_level_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(log_packet, log_msg_),
  };
  log_packet_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      log_packet_descriptor_,
      log_packet::default_instance_,
      log_packet_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(log_packet, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(log_packet, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(log_packet));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_message_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    log_packet_descriptor_, &log_packet::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_message_2eproto() {
  delete log_packet::default_instance_;
  delete log_packet_reflection_;
}

void protobuf_AddDesc_message_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\rmessage.proto\"\225\001\n\nlog_packet\022\020\n\010log_ti"
    "me\030\001 \002(\006\022\025\n\rlog_micro_sec\030\002 \002(\007\022\023\n\013seque"
    "nce_no\030\003 \002(\007\022\022\n\nshm_app_id\030\004 \002(\007\022\021\n\tpack"
    "et_id\030\005 \002(\t\022\021\n\tlog_level\030\006 \002(\t\022\017\n\007log_ms"
    "g\030\007 \002(\t", 167);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "message.proto", &protobuf_RegisterTypes);
  log_packet::default_instance_ = new log_packet();
  log_packet::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_message_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_message_2eproto {
  StaticDescriptorInitializer_message_2eproto() {
    protobuf_AddDesc_message_2eproto();
  }
} static_descriptor_initializer_message_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int log_packet::kLogTimeFieldNumber;
const int log_packet::kLogMicroSecFieldNumber;
const int log_packet::kSequenceNoFieldNumber;
const int log_packet::kShmAppIdFieldNumber;
const int log_packet::kPacketIdFieldNumber;
const int log_packet::kLogLevelFieldNumber;
const int log_packet::kLogMsgFieldNumber;
#endif  // !_MSC_VER

log_packet::log_packet()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void log_packet::InitAsDefaultInstance() {
}

log_packet::log_packet(const log_packet& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void log_packet::SharedCtor() {
  _cached_size_ = 0;
  log_time_ = GOOGLE_ULONGLONG(0);
  log_micro_sec_ = 0u;
  sequence_no_ = 0u;
  shm_app_id_ = 0u;
  packet_id_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  log_level_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  log_msg_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

log_packet::~log_packet() {
  SharedDtor();
}

void log_packet::SharedDtor() {
  if (packet_id_ != &::google::protobuf::internal::kEmptyString) {
    delete packet_id_;
  }
  if (log_level_ != &::google::protobuf::internal::kEmptyString) {
    delete log_level_;
  }
  if (log_msg_ != &::google::protobuf::internal::kEmptyString) {
    delete log_msg_;
  }
  if (this != default_instance_) {
  }
}

void log_packet::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* log_packet::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return log_packet_descriptor_;
}

const log_packet& log_packet::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_message_2eproto();
  return *default_instance_;
}

log_packet* log_packet::default_instance_ = NULL;

log_packet* log_packet::New() const {
  return new log_packet;
}

void log_packet::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    log_time_ = GOOGLE_ULONGLONG(0);
    log_micro_sec_ = 0u;
    sequence_no_ = 0u;
    shm_app_id_ = 0u;
    if (has_packet_id()) {
      if (packet_id_ != &::google::protobuf::internal::kEmptyString) {
        packet_id_->clear();
      }
    }
    if (has_log_level()) {
      if (log_level_ != &::google::protobuf::internal::kEmptyString) {
        log_level_->clear();
      }
    }
    if (has_log_msg()) {
      if (log_msg_ != &::google::protobuf::internal::kEmptyString) {
        log_msg_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool log_packet::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required fixed64 log_time = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_FIXED64) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_FIXED64>(
                 input, &log_time_)));
          set_has_log_time();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(21)) goto parse_log_micro_sec;
        break;
      }

      // required fixed32 log_micro_sec = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_FIXED32) {
         parse_log_micro_sec:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_FIXED32>(
                 input, &log_micro_sec_)));
          set_has_log_micro_sec();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(29)) goto parse_sequence_no;
        break;
      }

      // required fixed32 sequence_no = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_FIXED32) {
         parse_sequence_no:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_FIXED32>(
                 input, &sequence_no_)));
          set_has_sequence_no();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(37)) goto parse_shm_app_id;
        break;
      }

      // required fixed32 shm_app_id = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_FIXED32) {
         parse_shm_app_id:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_FIXED32>(
                 input, &shm_app_id_)));
          set_has_shm_app_id();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(42)) goto parse_packet_id;
        break;
      }

      // required string packet_id = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_packet_id:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_packet_id()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->packet_id().data(), this->packet_id().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(50)) goto parse_log_level;
        break;
      }

      // required string log_level = 6;
      case 6: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_log_level:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_log_level()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->log_level().data(), this->log_level().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(58)) goto parse_log_msg;
        break;
      }

      // required string log_msg = 7;
      case 7: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_log_msg:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_log_msg()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->log_msg().data(), this->log_msg().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void log_packet::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required fixed64 log_time = 1;
  if (has_log_time()) {
    ::google::protobuf::internal::WireFormatLite::WriteFixed64(1, this->log_time(), output);
  }

  // required fixed32 log_micro_sec = 2;
  if (has_log_micro_sec()) {
    ::google::protobuf::internal::WireFormatLite::WriteFixed32(2, this->log_micro_sec(), output);
  }

  // required fixed32 sequence_no = 3;
  if (has_sequence_no()) {
    ::google::protobuf::internal::WireFormatLite::WriteFixed32(3, this->sequence_no(), output);
  }

  // required fixed32 shm_app_id = 4;
  if (has_shm_app_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteFixed32(4, this->shm_app_id(), output);
  }

  // required string packet_id = 5;
  if (has_packet_id()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->packet_id().data(), this->packet_id().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      5, this->packet_id(), output);
  }

  // required string log_level = 6;
  if (has_log_level()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->log_level().data(), this->log_level().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      6, this->log_level(), output);
  }

  // required string log_msg = 7;
  if (has_log_msg()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->log_msg().data(), this->log_msg().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      7, this->log_msg(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* log_packet::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required fixed64 log_time = 1;
  if (has_log_time()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFixed64ToArray(1, this->log_time(), target);
  }

  // required fixed32 log_micro_sec = 2;
  if (has_log_micro_sec()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFixed32ToArray(2, this->log_micro_sec(), target);
  }

  // required fixed32 sequence_no = 3;
  if (has_sequence_no()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFixed32ToArray(3, this->sequence_no(), target);
  }

  // required fixed32 shm_app_id = 4;
  if (has_shm_app_id()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteFixed32ToArray(4, this->shm_app_id(), target);
  }

  // required string packet_id = 5;
  if (has_packet_id()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->packet_id().data(), this->packet_id().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        5, this->packet_id(), target);
  }

  // required string log_level = 6;
  if (has_log_level()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->log_level().data(), this->log_level().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        6, this->log_level(), target);
  }

  // required string log_msg = 7;
  if (has_log_msg()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->log_msg().data(), this->log_msg().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        7, this->log_msg(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int log_packet::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required fixed64 log_time = 1;
    if (has_log_time()) {
      total_size += 1 + 8;
    }

    // required fixed32 log_micro_sec = 2;
    if (has_log_micro_sec()) {
      total_size += 1 + 4;
    }

    // required fixed32 sequence_no = 3;
    if (has_sequence_no()) {
      total_size += 1 + 4;
    }

    // required fixed32 shm_app_id = 4;
    if (has_shm_app_id()) {
      total_size += 1 + 4;
    }

    // required string packet_id = 5;
    if (has_packet_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->packet_id());
    }

    // required string log_level = 6;
    if (has_log_level()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->log_level());
    }

    // required string log_msg = 7;
    if (has_log_msg()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->log_msg());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void log_packet::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const log_packet* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const log_packet*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void log_packet::MergeFrom(const log_packet& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_log_time()) {
      set_log_time(from.log_time());
    }
    if (from.has_log_micro_sec()) {
      set_log_micro_sec(from.log_micro_sec());
    }
    if (from.has_sequence_no()) {
      set_sequence_no(from.sequence_no());
    }
    if (from.has_shm_app_id()) {
      set_shm_app_id(from.shm_app_id());
    }
    if (from.has_packet_id()) {
      set_packet_id(from.packet_id());
    }
    if (from.has_log_level()) {
      set_log_level(from.log_level());
    }
    if (from.has_log_msg()) {
      set_log_msg(from.log_msg());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void log_packet::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void log_packet::CopyFrom(const log_packet& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool log_packet::IsInitialized() const {
  if ((_has_bits_[0] & 0x0000007f) != 0x0000007f) return false;

  return true;
}

void log_packet::Swap(log_packet* other) {
  if (other != this) {
    std::swap(log_time_, other->log_time_);
    std::swap(log_micro_sec_, other->log_micro_sec_);
    std::swap(sequence_no_, other->sequence_no_);
    std::swap(shm_app_id_, other->shm_app_id_);
    std::swap(packet_id_, other->packet_id_);
    std::swap(log_level_, other->log_level_);
    std::swap(log_msg_, other->log_msg_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata log_packet::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = log_packet_descriptor_;
  metadata.reflection = log_packet_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
