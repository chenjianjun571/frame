// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: jsbn_bc.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "jsbn_bc.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
// @@protoc_insertion_point(includes)

namespace jsbn {
namespace protoc {
namespace bc {

void protobuf_ShutdownFile_jsbn_5fbc_2eproto() {
  delete RegisterRequest::default_instance_;
  delete RegisterResponse::default_instance_;
  delete NetProtocol::default_instance_;
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
void protobuf_AddDesc_jsbn_5fbc_2eproto_impl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#else
void protobuf_AddDesc_jsbn_5fbc_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#endif
  RegisterRequest::default_instance_ = new RegisterRequest();
  RegisterResponse::default_instance_ = new RegisterResponse();
  NetProtocol::default_instance_ = new NetProtocol();
  RegisterRequest::default_instance_->InitAsDefaultInstance();
  RegisterResponse::default_instance_->InitAsDefaultInstance();
  NetProtocol::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_jsbn_5fbc_2eproto);
}

#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AddDesc_jsbn_5fbc_2eproto_once_);
void protobuf_AddDesc_jsbn_5fbc_2eproto() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AddDesc_jsbn_5fbc_2eproto_once_,
                 &protobuf_AddDesc_jsbn_5fbc_2eproto_impl);
}
#else
// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_jsbn_5fbc_2eproto {
  StaticDescriptorInitializer_jsbn_5fbc_2eproto() {
    protobuf_AddDesc_jsbn_5fbc_2eproto();
  }
} static_descriptor_initializer_jsbn_5fbc_2eproto_;
#endif
bool CommandID_IsValid(int value) {
  switch(value) {
    case 0:
    case 4097:
    case 4098:
      return true;
    default:
      return false;
  }
}


// ===================================================================

#ifndef _MSC_VER
const int RegisterRequest::kCityIDFieldNumber;
#endif  // !_MSC_VER

RegisterRequest::RegisterRequest()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:jsbn.protoc.bc.RegisterRequest)
}

void RegisterRequest::InitAsDefaultInstance() {
}

RegisterRequest::RegisterRequest(const RegisterRequest& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:jsbn.protoc.bc.RegisterRequest)
}

void RegisterRequest::SharedCtor() {
  _cached_size_ = 0;
  cityid_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

RegisterRequest::~RegisterRequest() {
  // @@protoc_insertion_point(destructor:jsbn.protoc.bc.RegisterRequest)
  SharedDtor();
}

void RegisterRequest::SharedDtor() {
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
  }
}

void RegisterRequest::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const RegisterRequest& RegisterRequest::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_jsbn_5fbc_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_jsbn_5fbc_2eproto();
#endif
  return *default_instance_;
}

RegisterRequest* RegisterRequest::default_instance_ = NULL;

RegisterRequest* RegisterRequest::New() const {
  return new RegisterRequest;
}

void RegisterRequest::Clear() {
  cityid_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->clear();
}

bool RegisterRequest::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  ::google::protobuf::io::StringOutputStream unknown_fields_string(
      mutable_unknown_fields());
  ::google::protobuf::io::CodedOutputStream unknown_fields_stream(
      &unknown_fields_string);
  // @@protoc_insertion_point(parse_start:jsbn.protoc.bc.RegisterRequest)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required fixed32 cityID = 1;
      case 1: {
        if (tag == 13) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_FIXED32>(
                 input, &cityid_)));
          set_has_cityid();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(
            input, tag, &unknown_fields_stream));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:jsbn.protoc.bc.RegisterRequest)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:jsbn.protoc.bc.RegisterRequest)
  return false;
#undef DO_
}

void RegisterRequest::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:jsbn.protoc.bc.RegisterRequest)
  // required fixed32 cityID = 1;
  if (has_cityid()) {
    ::google::protobuf::internal::WireFormatLite::WriteFixed32(1, this->cityid(), output);
  }

  output->WriteRaw(unknown_fields().data(),
                   unknown_fields().size());
  // @@protoc_insertion_point(serialize_end:jsbn.protoc.bc.RegisterRequest)
}

int RegisterRequest::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required fixed32 cityID = 1;
    if (has_cityid()) {
      total_size += 1 + 4;
    }

  }
  total_size += unknown_fields().size();

  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void RegisterRequest::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const RegisterRequest*>(&from));
}

void RegisterRequest::MergeFrom(const RegisterRequest& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_cityid()) {
      set_cityid(from.cityid());
    }
  }
  mutable_unknown_fields()->append(from.unknown_fields());
}

void RegisterRequest::CopyFrom(const RegisterRequest& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool RegisterRequest::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void RegisterRequest::Swap(RegisterRequest* other) {
  if (other != this) {
    std::swap(cityid_, other->cityid_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.swap(other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string RegisterRequest::GetTypeName() const {
  return "jsbn.protoc.bc.RegisterRequest";
}


// ===================================================================

#ifndef _MSC_VER
const int RegisterResponse::kResultFieldNumber;
const int RegisterResponse::kErrorDescriptionFieldNumber;
#endif  // !_MSC_VER

RegisterResponse::RegisterResponse()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:jsbn.protoc.bc.RegisterResponse)
}

void RegisterResponse::InitAsDefaultInstance() {
}

RegisterResponse::RegisterResponse(const RegisterResponse& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:jsbn.protoc.bc.RegisterResponse)
}

void RegisterResponse::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  result_ = 0u;
  error_description_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

RegisterResponse::~RegisterResponse() {
  // @@protoc_insertion_point(destructor:jsbn.protoc.bc.RegisterResponse)
  SharedDtor();
}

void RegisterResponse::SharedDtor() {
  if (error_description_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete error_description_;
  }
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
  }
}

void RegisterResponse::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const RegisterResponse& RegisterResponse::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_jsbn_5fbc_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_jsbn_5fbc_2eproto();
#endif
  return *default_instance_;
}

RegisterResponse* RegisterResponse::default_instance_ = NULL;

RegisterResponse* RegisterResponse::New() const {
  return new RegisterResponse;
}

void RegisterResponse::Clear() {
  if (_has_bits_[0 / 32] & 3) {
    result_ = 0u;
    if (has_error_description()) {
      if (error_description_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        error_description_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->clear();
}

bool RegisterResponse::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  ::google::protobuf::io::StringOutputStream unknown_fields_string(
      mutable_unknown_fields());
  ::google::protobuf::io::CodedOutputStream unknown_fields_stream(
      &unknown_fields_string);
  // @@protoc_insertion_point(parse_start:jsbn.protoc.bc.RegisterResponse)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required fixed32 result = 1;
      case 1: {
        if (tag == 13) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_FIXED32>(
                 input, &result_)));
          set_has_result();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_error_description;
        break;
      }

      // optional string error_description = 2;
      case 2: {
        if (tag == 18) {
         parse_error_description:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_error_description()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(
            input, tag, &unknown_fields_stream));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:jsbn.protoc.bc.RegisterResponse)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:jsbn.protoc.bc.RegisterResponse)
  return false;
#undef DO_
}

void RegisterResponse::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:jsbn.protoc.bc.RegisterResponse)
  // required fixed32 result = 1;
  if (has_result()) {
    ::google::protobuf::internal::WireFormatLite::WriteFixed32(1, this->result(), output);
  }

  // optional string error_description = 2;
  if (has_error_description()) {
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      2, this->error_description(), output);
  }

  output->WriteRaw(unknown_fields().data(),
                   unknown_fields().size());
  // @@protoc_insertion_point(serialize_end:jsbn.protoc.bc.RegisterResponse)
}

int RegisterResponse::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required fixed32 result = 1;
    if (has_result()) {
      total_size += 1 + 4;
    }

    // optional string error_description = 2;
    if (has_error_description()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->error_description());
    }

  }
  total_size += unknown_fields().size();

  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void RegisterResponse::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const RegisterResponse*>(&from));
}

void RegisterResponse::MergeFrom(const RegisterResponse& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_result()) {
      set_result(from.result());
    }
    if (from.has_error_description()) {
      set_error_description(from.error_description());
    }
  }
  mutable_unknown_fields()->append(from.unknown_fields());
}

void RegisterResponse::CopyFrom(const RegisterResponse& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool RegisterResponse::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void RegisterResponse::Swap(RegisterResponse* other) {
  if (other != this) {
    std::swap(result_, other->result_);
    std::swap(error_description_, other->error_description_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.swap(other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string RegisterResponse::GetTypeName() const {
  return "jsbn.protoc.bc.RegisterResponse";
}


// ===================================================================

#ifndef _MSC_VER
const int NetProtocol::kCommandIDFieldNumber;
const int NetProtocol::kRegisterRequestFieldNumber;
const int NetProtocol::kRegisterResponseFieldNumber;
#endif  // !_MSC_VER

NetProtocol::NetProtocol()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:jsbn.protoc.bc.NetProtocol)
}

void NetProtocol::InitAsDefaultInstance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  registerrequest_ = const_cast< ::jsbn::protoc::bc::RegisterRequest*>(
      ::jsbn::protoc::bc::RegisterRequest::internal_default_instance());
#else
  registerrequest_ = const_cast< ::jsbn::protoc::bc::RegisterRequest*>(&::jsbn::protoc::bc::RegisterRequest::default_instance());
#endif
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  registerresponse_ = const_cast< ::jsbn::protoc::bc::RegisterResponse*>(
      ::jsbn::protoc::bc::RegisterResponse::internal_default_instance());
#else
  registerresponse_ = const_cast< ::jsbn::protoc::bc::RegisterResponse*>(&::jsbn::protoc::bc::RegisterResponse::default_instance());
#endif
}

NetProtocol::NetProtocol(const NetProtocol& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:jsbn.protoc.bc.NetProtocol)
}

void NetProtocol::SharedCtor() {
  _cached_size_ = 0;
  commandid_ = 0;
  registerrequest_ = NULL;
  registerresponse_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

NetProtocol::~NetProtocol() {
  // @@protoc_insertion_point(destructor:jsbn.protoc.bc.NetProtocol)
  SharedDtor();
}

void NetProtocol::SharedDtor() {
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  if (this != &default_instance()) {
  #else
  if (this != default_instance_) {
  #endif
    delete registerrequest_;
    delete registerresponse_;
  }
}

void NetProtocol::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const NetProtocol& NetProtocol::default_instance() {
#ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  protobuf_AddDesc_jsbn_5fbc_2eproto();
#else
  if (default_instance_ == NULL) protobuf_AddDesc_jsbn_5fbc_2eproto();
#endif
  return *default_instance_;
}

NetProtocol* NetProtocol::default_instance_ = NULL;

NetProtocol* NetProtocol::New() const {
  return new NetProtocol;
}

void NetProtocol::Clear() {
  if (_has_bits_[0 / 32] & 7) {
    commandid_ = 0;
    if (has_registerrequest()) {
      if (registerrequest_ != NULL) registerrequest_->::jsbn::protoc::bc::RegisterRequest::Clear();
    }
    if (has_registerresponse()) {
      if (registerresponse_ != NULL) registerresponse_->::jsbn::protoc::bc::RegisterResponse::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->clear();
}

bool NetProtocol::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  ::google::protobuf::io::StringOutputStream unknown_fields_string(
      mutable_unknown_fields());
  ::google::protobuf::io::CodedOutputStream unknown_fields_stream(
      &unknown_fields_string);
  // @@protoc_insertion_point(parse_start:jsbn.protoc.bc.NetProtocol)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .jsbn.protoc.bc.CommandID commandID = 1;
      case 1: {
        if (tag == 8) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::jsbn::protoc::bc::CommandID_IsValid(value)) {
            set_commandid(static_cast< ::jsbn::protoc::bc::CommandID >(value));
          } else {
            unknown_fields_stream.WriteVarint32(tag);
            unknown_fields_stream.WriteVarint32(value);
          }
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(18)) goto parse_registerRequest;
        break;
      }

      // optional .jsbn.protoc.bc.RegisterRequest registerRequest = 2;
      case 2: {
        if (tag == 18) {
         parse_registerRequest:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_registerrequest()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(26)) goto parse_registerResponse;
        break;
      }

      // optional .jsbn.protoc.bc.RegisterResponse registerResponse = 3;
      case 3: {
        if (tag == 26) {
         parse_registerResponse:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_registerresponse()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(
            input, tag, &unknown_fields_stream));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:jsbn.protoc.bc.NetProtocol)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:jsbn.protoc.bc.NetProtocol)
  return false;
#undef DO_
}

void NetProtocol::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:jsbn.protoc.bc.NetProtocol)
  // required .jsbn.protoc.bc.CommandID commandID = 1;
  if (has_commandid()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      1, this->commandid(), output);
  }

  // optional .jsbn.protoc.bc.RegisterRequest registerRequest = 2;
  if (has_registerrequest()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      2, this->registerrequest(), output);
  }

  // optional .jsbn.protoc.bc.RegisterResponse registerResponse = 3;
  if (has_registerresponse()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessage(
      3, this->registerresponse(), output);
  }

  output->WriteRaw(unknown_fields().data(),
                   unknown_fields().size());
  // @@protoc_insertion_point(serialize_end:jsbn.protoc.bc.NetProtocol)
}

int NetProtocol::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required .jsbn.protoc.bc.CommandID commandID = 1;
    if (has_commandid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->commandid());
    }

    // optional .jsbn.protoc.bc.RegisterRequest registerRequest = 2;
    if (has_registerrequest()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->registerrequest());
    }

    // optional .jsbn.protoc.bc.RegisterResponse registerResponse = 3;
    if (has_registerresponse()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->registerresponse());
    }

  }
  total_size += unknown_fields().size();

  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void NetProtocol::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const NetProtocol*>(&from));
}

void NetProtocol::MergeFrom(const NetProtocol& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_commandid()) {
      set_commandid(from.commandid());
    }
    if (from.has_registerrequest()) {
      mutable_registerrequest()->::jsbn::protoc::bc::RegisterRequest::MergeFrom(from.registerrequest());
    }
    if (from.has_registerresponse()) {
      mutable_registerresponse()->::jsbn::protoc::bc::RegisterResponse::MergeFrom(from.registerresponse());
    }
  }
  mutable_unknown_fields()->append(from.unknown_fields());
}

void NetProtocol::CopyFrom(const NetProtocol& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool NetProtocol::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  if (has_registerrequest()) {
    if (!this->registerrequest().IsInitialized()) return false;
  }
  if (has_registerresponse()) {
    if (!this->registerresponse().IsInitialized()) return false;
  }
  return true;
}

void NetProtocol::Swap(NetProtocol* other) {
  if (other != this) {
    std::swap(commandid_, other->commandid_);
    std::swap(registerrequest_, other->registerrequest_);
    std::swap(registerresponse_, other->registerresponse_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.swap(other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string NetProtocol::GetTypeName() const {
  return "jsbn.protoc.bc.NetProtocol";
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace bc
}  // namespace protoc
}  // namespace jsbn

// @@protoc_insertion_point(global_scope)