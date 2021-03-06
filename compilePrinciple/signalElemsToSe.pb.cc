// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: signalElemsToSe.proto

#include "signalElemsToSe.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// This is a temporary google only hack
#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
#include "third_party/protobuf/version.h"
#endif
// @@protoc_insertion_point(includes)
class signalElemsDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<signalElems>
      _instance;
} _signalElems_default_instance_;
namespace protobuf_signalElemsToSe_2eproto {
void InitDefaultssignalElemsImpl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

#ifdef GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  ::google::protobuf::internal::InitProtobufDefaultsForceUnique();
#else
  ::google::protobuf::internal::InitProtobufDefaults();
#endif  // GOOGLE_PROTOBUF_ENFORCE_UNIQUENESS
  {
    void* ptr = &::_signalElems_default_instance_;
    new (ptr) ::signalElems();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::signalElems::InitAsDefaultInstance();
}

void InitDefaultssignalElems() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &InitDefaultssignalElemsImpl);
}

::google::protobuf::Metadata file_level_metadata[1];

const ::google::protobuf::uint32 TableStruct::offsets[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::signalElems, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::signalElems, elems_length_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::signalElems, noindex_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::signalElems, noitem_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::signalElems, noindex_explain_),
};
static const ::google::protobuf::internal::MigrationSchema schemas[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, sizeof(::signalElems)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&::_signalElems_default_instance_),
};

void protobuf_AssignDescriptors() {
  AddDescriptors();
  ::google::protobuf::MessageFactory* factory = NULL;
  AssignDescriptors(
      "signalElemsToSe.proto", schemas, file_default_instances, TableStruct::offsets, factory,
      file_level_metadata, NULL, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_PROTOBUF_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::internal::RegisterAllTypes(file_level_metadata, 1);
}

void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n\025signalElemsToSe.proto\"]\n\013signalElems\022\024"
      "\n\014elems_Length\030\001 \001(\005\022\017\n\007NoIndex\030\002 \003(\005\022\016\n"
      "\006NoItem\030\003 \003(\t\022\027\n\017NoIndex_explain\030\004 \003(\tb\006"
      "proto3"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 126);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "signalElemsToSe.proto", &protobuf_RegisterTypes);
}

void AddDescriptors() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;
}  // namespace protobuf_signalElemsToSe_2eproto

// ===================================================================

void signalElems::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int signalElems::kElemsLengthFieldNumber;
const int signalElems::kNoIndexFieldNumber;
const int signalElems::kNoItemFieldNumber;
const int signalElems::kNoIndexExplainFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

signalElems::signalElems()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    ::protobuf_signalElemsToSe_2eproto::InitDefaultssignalElems();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:signalElems)
}
signalElems::signalElems(const signalElems& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      noindex_(from.noindex_),
      noitem_(from.noitem_),
      noindex_explain_(from.noindex_explain_),
      _cached_size_(0) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  elems_length_ = from.elems_length_;
  // @@protoc_insertion_point(copy_constructor:signalElems)
}

void signalElems::SharedCtor() {
  elems_length_ = 0;
  _cached_size_ = 0;
}

signalElems::~signalElems() {
  // @@protoc_insertion_point(destructor:signalElems)
  SharedDtor();
}

void signalElems::SharedDtor() {
}

void signalElems::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* signalElems::descriptor() {
  ::protobuf_signalElemsToSe_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_signalElemsToSe_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const signalElems& signalElems::default_instance() {
  ::protobuf_signalElemsToSe_2eproto::InitDefaultssignalElems();
  return *internal_default_instance();
}

signalElems* signalElems::New(::google::protobuf::Arena* arena) const {
  signalElems* n = new signalElems;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void signalElems::Clear() {
// @@protoc_insertion_point(message_clear_start:signalElems)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  noindex_.Clear();
  noitem_.Clear();
  noindex_explain_.Clear();
  elems_length_ = 0;
  _internal_metadata_.Clear();
}

bool signalElems::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:signalElems)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(127u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // int32 elems_Length = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(8u /* 8 & 0xFF */)) {

          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &elems_length_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // repeated int32 NoIndex = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(18u /* 18 & 0xFF */)) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPackedPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, this->mutable_noindex())));
        } else if (
            static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(16u /* 16 & 0xFF */)) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadRepeatedPrimitiveNoInline<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 1, 18u, input, this->mutable_noindex())));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // repeated string NoItem = 3;
      case 3: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(26u /* 26 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->add_noitem()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->noitem(this->noitem_size() - 1).data(),
            static_cast<int>(this->noitem(this->noitem_size() - 1).length()),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "signalElems.NoItem"));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // repeated string NoIndex_explain = 4;
      case 4: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(34u /* 34 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->add_noindex_explain()));
          DO_(::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
            this->noindex_explain(this->noindex_explain_size() - 1).data(),
            static_cast<int>(this->noindex_explain(this->noindex_explain_size() - 1).length()),
            ::google::protobuf::internal::WireFormatLite::PARSE,
            "signalElems.NoIndex_explain"));
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:signalElems)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:signalElems)
  return false;
#undef DO_
}

void signalElems::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:signalElems)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 elems_Length = 1;
  if (this->elems_length() != 0) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->elems_length(), output);
  }

  // repeated int32 NoIndex = 2;
  if (this->noindex_size() > 0) {
    ::google::protobuf::internal::WireFormatLite::WriteTag(2, ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED, output);
    output->WriteVarint32(static_cast< ::google::protobuf::uint32>(
        _noindex_cached_byte_size_));
  }
  for (int i = 0, n = this->noindex_size(); i < n; i++) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32NoTag(
      this->noindex(i), output);
  }

  // repeated string NoItem = 3;
  for (int i = 0, n = this->noitem_size(); i < n; i++) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->noitem(i).data(), static_cast<int>(this->noitem(i).length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "signalElems.NoItem");
    ::google::protobuf::internal::WireFormatLite::WriteString(
      3, this->noitem(i), output);
  }

  // repeated string NoIndex_explain = 4;
  for (int i = 0, n = this->noindex_explain_size(); i < n; i++) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->noindex_explain(i).data(), static_cast<int>(this->noindex_explain(i).length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "signalElems.NoIndex_explain");
    ::google::protobuf::internal::WireFormatLite::WriteString(
      4, this->noindex_explain(i), output);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), output);
  }
  // @@protoc_insertion_point(serialize_end:signalElems)
}

::google::protobuf::uint8* signalElems::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:signalElems)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 elems_Length = 1;
  if (this->elems_length() != 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->elems_length(), target);
  }

  // repeated int32 NoIndex = 2;
  if (this->noindex_size() > 0) {
    target = ::google::protobuf::internal::WireFormatLite::WriteTagToArray(
      2,
      ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED,
      target);
    target = ::google::protobuf::io::CodedOutputStream::WriteVarint32ToArray(
        static_cast< ::google::protobuf::int32>(
            _noindex_cached_byte_size_), target);
    target = ::google::protobuf::internal::WireFormatLite::
      WriteInt32NoTagToArray(this->noindex_, target);
  }

  // repeated string NoItem = 3;
  for (int i = 0, n = this->noitem_size(); i < n; i++) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->noitem(i).data(), static_cast<int>(this->noitem(i).length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "signalElems.NoItem");
    target = ::google::protobuf::internal::WireFormatLite::
      WriteStringToArray(3, this->noitem(i), target);
  }

  // repeated string NoIndex_explain = 4;
  for (int i = 0, n = this->noindex_explain_size(); i < n; i++) {
    ::google::protobuf::internal::WireFormatLite::VerifyUtf8String(
      this->noindex_explain(i).data(), static_cast<int>(this->noindex_explain(i).length()),
      ::google::protobuf::internal::WireFormatLite::SERIALIZE,
      "signalElems.NoIndex_explain");
    target = ::google::protobuf::internal::WireFormatLite::
      WriteStringToArray(4, this->noindex_explain(i), target);
  }

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:signalElems)
  return target;
}

size_t signalElems::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:signalElems)
  size_t total_size = 0;

  if ((_internal_metadata_.have_unknown_fields() &&  ::google::protobuf::internal::GetProto3PreserveUnknownsDefault())) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        (::google::protobuf::internal::GetProto3PreserveUnknownsDefault()   ? _internal_metadata_.unknown_fields()   : _internal_metadata_.default_instance()));
  }
  // repeated int32 NoIndex = 2;
  {
    size_t data_size = ::google::protobuf::internal::WireFormatLite::
      Int32Size(this->noindex_);
    if (data_size > 0) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
            static_cast< ::google::protobuf::int32>(data_size));
    }
    int cached_size = ::google::protobuf::internal::ToCachedSize(data_size);
    GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
    _noindex_cached_byte_size_ = cached_size;
    GOOGLE_SAFE_CONCURRENT_WRITES_END();
    total_size += data_size;
  }

  // repeated string NoItem = 3;
  total_size += 1 *
      ::google::protobuf::internal::FromIntSize(this->noitem_size());
  for (int i = 0, n = this->noitem_size(); i < n; i++) {
    total_size += ::google::protobuf::internal::WireFormatLite::StringSize(
      this->noitem(i));
  }

  // repeated string NoIndex_explain = 4;
  total_size += 1 *
      ::google::protobuf::internal::FromIntSize(this->noindex_explain_size());
  for (int i = 0, n = this->noindex_explain_size(); i < n; i++) {
    total_size += ::google::protobuf::internal::WireFormatLite::StringSize(
      this->noindex_explain(i));
  }

  // int32 elems_Length = 1;
  if (this->elems_length() != 0) {
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::Int32Size(
        this->elems_length());
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void signalElems::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:signalElems)
  GOOGLE_DCHECK_NE(&from, this);
  const signalElems* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const signalElems>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:signalElems)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:signalElems)
    MergeFrom(*source);
  }
}

void signalElems::MergeFrom(const signalElems& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:signalElems)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  noindex_.MergeFrom(from.noindex_);
  noitem_.MergeFrom(from.noitem_);
  noindex_explain_.MergeFrom(from.noindex_explain_);
  if (from.elems_length() != 0) {
    set_elems_length(from.elems_length());
  }
}

void signalElems::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:signalElems)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void signalElems::CopyFrom(const signalElems& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:signalElems)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool signalElems::IsInitialized() const {
  return true;
}

void signalElems::Swap(signalElems* other) {
  if (other == this) return;
  InternalSwap(other);
}
void signalElems::InternalSwap(signalElems* other) {
  using std::swap;
  noindex_.InternalSwap(&other->noindex_);
  noitem_.InternalSwap(&other->noitem_);
  noindex_explain_.InternalSwap(&other->noindex_explain_);
  swap(elems_length_, other->elems_length_);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata signalElems::GetMetadata() const {
  protobuf_signalElemsToSe_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_signalElemsToSe_2eproto::file_level_metadata[kIndexInFileMessages];
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
