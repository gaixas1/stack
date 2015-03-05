/*
 * CDAP North bound API
 *
 *    Bernat Gastón <bernat.gaston@i2cat.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301  USA
 */

#ifndef CDAP_PROVIDER_H_
#define CDAP_PROVIDER_H_
#include <string>

namespace cdap_api {

/// Authentication information
typedef struct auth_info
{
  enum AuthTypes
  {
    AUTH_NONE,
    AUTH_PASSWD,
    AUTH_SSHRSA,
    AUTH_SSHDSA
  };
  /// AuthenticationMechanismName (authtypes), optional, not validated by CDAP.
  /// Identification of the method to be used by the destination application to
  /// authenticate the source application
  AuthTypes auth_mech_;
} auth_info_t;

typedef struct destination_info
{
  /// DestinationApplication-Entity-Instance-Id (string), optional, not validated by CDAP.
  /// Specific instance of the Application Entity that the source application
  /// wishes to connect to in the destination application.
  std::string dest_ae_inst_;
  /// DestinationApplication-Entity-Name (string), mandatory (optional for the response).
  /// Name of the Application Entity that the source application wishes
  /// to connect to in the destination application.
  std::string dest_ae_name_;
  /// DestinationApplication-Process-Instance-Id (string), optional, not validated by CDAP.
  /// Name of the Application Process Instance that the source wishes to
  /// connect to a the destination.
  std::string dest_ap_inst_;
  /// DestinationApplication-Process-Name (string), mandatory (optional for the response).
  /// Name of the application process that the source application wishes to connect to
  /// in the destination application
  std::string dest_ap_name_;
} dest_info_t;

typedef struct source_info
{
  /// DestinationApplication-Entity-Instance-Id (string), optional, not validated by CDAP.
  /// Specific instance of the Application Entity that the source application
  /// wishes to connect to in the destination application.
  std::string src_ae_inst_;
  /// DestinationApplication-Entity-Name (string), mandatory (optional for the response).
  /// Name of the Application Entity that the source application wishes
  /// to connect to in the destination application.
  std::string src_ae_name_;
  /// DestinationApplication-Process-Instance-Id (string), optional, not validated by CDAP.
  /// Name of the Application Process Instance that the source wishes to
  /// connect to a the destination.
  std::string src_ap_inst_;
  /// DestinationApplication-Process-Name (string), mandatory (optional for the response).
  /// Name of the application process that the source application wishes to connect to
  /// in the destination application
  std::string src_ap_name_;
} src_info_t;

typedef struct flags
{
  enum Flags
  {
    NONE_FLAGS,
    F_SYNC,
    F_RD_INCOMPLETE
  };
  /// flags (enm, int32), conditional, may be required by CDAP.
  /// set_ of Boolean values that modify the meaning of a
  /// message in a uniform way when true.
  Flags flags_;
} flags_t;

typedef struct object_info
{
  /// ObjectClass (string). Identifies the object class definition of the
  /// addressed object.
  std::string obj_class_;
  /// ObjectInstance (int64). Object instance uniquely identifies a single object
  /// with a specific ObjectClass and ObjectName in an application's RIB. Either
  /// the ObjectClass and ObjectName or this value may be used, if the ObjectInstance
  /// value is known. If a class and name are supplied in an operation,
  /// an ObjectInstance value may be returned, and that may be used in future operations
  /// in lieu of obj_class and obj_name for the duration of this connection.
  long obj_inst_;
  /// ObjectName (string). Identifies a named object that the operation is
  /// to be applied to. Object names identify a unique object of the designated
  /// ObjectClass within an application.
  std::string obj_name_;
  /// ObjectValueInterface (ObjectValueInterface). The value of the object.
  void *obj_value_;
  /// Result (int32). Mandatory in the responses, forbidden in the requests
  /// The result of an operation, indicating its success (which has the value zero,
  /// the default for this field), partial success in the case of
  /// synchronized operations, or reason for failure
} obj_info_t;

typedef struct filtering_info
{
  /// Filter (bytes). Filter predicate function to be used to determine whether an operation
  /// is to be applied to the designated object (s).
  char* filter_;
  /// Scope (int32). Specifies the depth of the object tree at
  /// the destination application to which an operation (subject to filtering)
  /// is to apply (if missing or present and having the value 0, the default,
  /// only the target_ed application's objects are addressed)
  int scope_;
} filt_info_t;

typedef struct result_info
{
  /// Result of the operation
  int result_;
  /// Result-Reason (string), optional in the responses, forbidden in the requests
  /// Additional explanation of the result_
  std::string result_reason_;
} res_info_t;

/// CDAP message parameters
typedef struct version_info
{
  /// Version (int32). Mandatory in connect request and response, optional otherwise.
  /// Version of RIB and object set_ to use in the conversation. Note that the
  /// abstract syntax refers to the CDAP message syntax, while version refers to the
  /// version of the AE RIB objects, their values, vocabulary of object id's, and
  /// related behaviors that are subject to change over time. See text for details
  /// of use.
  long version_;
} vers_info_t;

typedef struct connection_handler
{
  int id_;
  src_info_t src_;
  dest_info_t dest_;
  auth_info_t auth_;
  vers_info_t version_;
} con_handle_t;

class CDAPProvider
{
 public:
  con_handle_t open_connection(const vers_info_t ver, const src_info_t &src,
                               const dest_info_t &dest, const auth_info &auth);
  void close_connection(con_handle_t &con);

  void create_remote(const con_handle_t &con, const obj_info_t &obj,
                     const flags_t &flags, const filt_info_t &filt);
  void delete_remote(const con_handle_t &con, const obj_info_t &obj,
                     const flags_t &flags, const filt_info_t &filt);
  void read_remote(const con_handle_t &con, const obj_info_t &obj,
                   const flags_t &flags, const filt_info_t &filt);
  void cancel_read_remote(const con_handle_t &con, const obj_info_t &obj,
                          const flags_t &flags, const filt_info_t &filt);
  void write_remote(const con_handle_t &con, const obj_info_t &obj,
                    const flags_t &flags, const filt_info_t &filt);
  void start_remote(const con_handle_t &con, const obj_info_t &obj,
                    const flags_t &flags, const filt_info_t &filt);
  void stop_remote(const con_handle_t &con, const obj_info_t &obj,
                   const flags_t &flags, const filt_info_t &filt);

  void create_response(const con_handle_t &con, const obj_info_t &obj,
                       const flags_t &flags, const res_info_t &res, int id);
  void delete_response(const con_handle_t &con, const obj_info_t &obj,
                       const flags_t &flags, const res_info_t &res, int id);
  void read_response(const con_handle_t &con, const obj_info_t &obj,
                     const flags_t &flags, const res_info_t &res, int id);
  void cancel_read_response(const con_handle_t &con, const obj_info_t &obj,
                            const flags_t &flags, const res_info_t &res,
                            int id);
  void write_response(const con_handle_t &con, const obj_info_t &obj,
                      const flags_t &flags, const res_info_t &res, int id);
  void start_response(const con_handle_t &con, const obj_info_t &obj,
                      const flags_t &flags, const res_info_t &res, int id);
  void stop_response(const con_handle_t &con, const obj_info_t &obj,
                     const flags_t &flags, const res_info_t &res, int id);
};

}

#endif /* CDAP_PROVIDER_H_ */
