// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: protocol.proto

#ifndef PROTOBUF_protocol_2eproto__INCLUDED
#define PROTOBUF_protocol_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_protocol_2eproto();
void protobuf_AssignDesc_protocol_2eproto();
void protobuf_ShutdownFile_protocol_2eproto();


enum NetMsgId {
  eConnectSvrReq = 1,
  eConnectSvrAck = 2
};
bool NetMsgId_IsValid(int value);
const NetMsgId NetMsgId_MIN = eConnectSvrReq;
const NetMsgId NetMsgId_MAX = eConnectSvrAck;
const int NetMsgId_ARRAYSIZE = NetMsgId_MAX + 1;

const ::google::protobuf::EnumDescriptor* NetMsgId_descriptor();
inline const ::std::string& NetMsgId_Name(NetMsgId value) {
  return ::google::protobuf::internal::NameOfEnum(
    NetMsgId_descriptor(), value);
}
inline bool NetMsgId_Parse(
    const ::std::string& name, NetMsgId* value) {
  return ::google::protobuf::internal::ParseNamedEnum<NetMsgId>(
    NetMsgId_descriptor(), name, value);
}
enum ClientGateMsgId {
  eClientGateMsgIdMin = 1000,
  eLoginReq = 1001,
  eLoginAck = 1002,
  eEncryptKeyNtf = 1003,
  eAuthReq = 1004,
  eAuthAck = 1005,
  ePing = 1006,
  ePong = 1007,
  eSpeedTest = 1008,
  eLatencyTest = 1009,
  eClientGateMsgIdMax = 2000
};
bool ClientGateMsgId_IsValid(int value);
const ClientGateMsgId ClientGateMsgId_MIN = eClientGateMsgIdMin;
const ClientGateMsgId ClientGateMsgId_MAX = eClientGateMsgIdMax;
const int ClientGateMsgId_ARRAYSIZE = ClientGateMsgId_MAX + 1;

const ::google::protobuf::EnumDescriptor* ClientGateMsgId_descriptor();
inline const ::std::string& ClientGateMsgId_Name(ClientGateMsgId value) {
  return ::google::protobuf::internal::NameOfEnum(
    ClientGateMsgId_descriptor(), value);
}
inline bool ClientGateMsgId_Parse(
    const ::std::string& name, ClientGateMsgId* value) {
  return ::google::protobuf::internal::ParseNamedEnum<ClientGateMsgId>(
    ClientGateMsgId_descriptor(), name, value);
}
enum GameToDBMsgId {
  eGameToDBMsgIdMin = 2000,
  ePostSql = 2001,
  eGameToDBMsgIdMax = 3000
};
bool GameToDBMsgId_IsValid(int value);
const GameToDBMsgId GameToDBMsgId_MIN = eGameToDBMsgIdMin;
const GameToDBMsgId GameToDBMsgId_MAX = eGameToDBMsgIdMax;
const int GameToDBMsgId_ARRAYSIZE = GameToDBMsgId_MAX + 1;

const ::google::protobuf::EnumDescriptor* GameToDBMsgId_descriptor();
inline const ::std::string& GameToDBMsgId_Name(GameToDBMsgId value) {
  return ::google::protobuf::internal::NameOfEnum(
    GameToDBMsgId_descriptor(), value);
}
inline bool GameToDBMsgId_Parse(
    const ::std::string& name, GameToDBMsgId* value) {
  return ::google::protobuf::internal::ParseNamedEnum<GameToDBMsgId>(
    GameToDBMsgId_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================


// @@protoc_insertion_point(namespace_scope)

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< ::NetMsgId>() {
  return ::NetMsgId_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::ClientGateMsgId>() {
  return ::ClientGateMsgId_descriptor();
}
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::GameToDBMsgId>() {
  return ::GameToDBMsgId_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_protocol_2eproto__INCLUDED
