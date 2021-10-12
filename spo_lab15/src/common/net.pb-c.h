/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: net.proto */

#ifndef PROTOBUF_C_net_2eproto__INCLUDED
#define PROTOBUF_C_net_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1002001 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _Common__ColumnValue Common__ColumnValue;
typedef struct _Common__Request Common__Request;
typedef struct _Common__Response Common__Response;


/* --- enums --- */


/* --- messages --- */

struct  _Common__ColumnValue
{
  ProtobufCMessage base;
  char *title;
  protobuf_c_boolean has_column_type_code;
  int32_t column_type_code;
  char *value;
  protobuf_c_boolean has_is_key;
  protobuf_c_boolean is_key;
  protobuf_c_boolean has_is_required;
  protobuf_c_boolean is_required;
  protobuf_c_boolean has_is_start;
  protobuf_c_boolean is_start;
};
#define COMMON__COLUMN_VALUE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&common__column_value__descriptor) \
    , NULL, 0,0, NULL, 0,0, 0,0, 0,0 }


struct  _Common__Request
{
  ProtobufCMessage base;
  int32_t command_code;
  char *table_name;
  size_t n_columns;
  Common__ColumnValue **columns;
};
#define COMMON__REQUEST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&common__request__descriptor) \
    , 0, NULL, 0,NULL }


struct  _Common__Response
{
  ProtobufCMessage base;
  int32_t status_code;
  int32_t command_code;
  size_t n_columns;
  Common__ColumnValue **columns;
  char *text;
  int32_t amount_columns;
};
#define COMMON__RESPONSE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&common__response__descriptor) \
    , 0, 0, 0,NULL, NULL, 0 }


/* Common__ColumnValue methods */
void   common__column_value__init
                     (Common__ColumnValue         *message);
size_t common__column_value__get_packed_size
                     (const Common__ColumnValue   *message);
size_t common__column_value__pack
                     (const Common__ColumnValue   *message,
                      uint8_t             *out);
size_t common__column_value__pack_to_buffer
                     (const Common__ColumnValue   *message,
                      ProtobufCBuffer     *buffer);
Common__ColumnValue *
       common__column_value__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   common__column_value__free_unpacked
                     (Common__ColumnValue *message,
                      ProtobufCAllocator *allocator);
/* Common__Request methods */
void   common__request__init
                     (Common__Request         *message);
size_t common__request__get_packed_size
                     (const Common__Request   *message);
size_t common__request__pack
                     (const Common__Request   *message,
                      uint8_t             *out);
size_t common__request__pack_to_buffer
                     (const Common__Request   *message,
                      ProtobufCBuffer     *buffer);
Common__Request *
       common__request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   common__request__free_unpacked
                     (Common__Request *message,
                      ProtobufCAllocator *allocator);
/* Common__Response methods */
void   common__response__init
                     (Common__Response         *message);
size_t common__response__get_packed_size
                     (const Common__Response   *message);
size_t common__response__pack
                     (const Common__Response   *message,
                      uint8_t             *out);
size_t common__response__pack_to_buffer
                     (const Common__Response   *message,
                      ProtobufCBuffer     *buffer);
Common__Response *
       common__response__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   common__response__free_unpacked
                     (Common__Response *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Common__ColumnValue_Closure)
                 (const Common__ColumnValue *message,
                  void *closure_data);
typedef void (*Common__Request_Closure)
                 (const Common__Request *message,
                  void *closure_data);
typedef void (*Common__Response_Closure)
                 (const Common__Response *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor common__column_value__descriptor;
extern const ProtobufCMessageDescriptor common__request__descriptor;
extern const ProtobufCMessageDescriptor common__response__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_net_2eproto__INCLUDED */
