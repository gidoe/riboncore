/*
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/** \file
    \ingroup realmd
 */

#include "Common.h"
#include "AuthSocket.h"

#include <memory>
#include <algorithm>

#include <openssl/md5.h>

#include <ace/INET_Addr.h>
#include <ace/SOCK_Stream.h>
#include <ace/Proactor.h>
#include <ace/OS_NS_ctype.h>
#include <ace/OS_NS_errno.h>
#include <ace/OS_NS_fcntl.h>
#include <ace/OS_NS_signal.h>
#include <ace/OS_NS_string.h>
#include <ace/OS_NS_unistd.h>
#include <ace/OS_NS_sys_socket.h>
#include <ace/OS_NS_sys_stat.h>
#include <ace/OS_NS_stdlib.h>
#include <ace/os_include/netinet/os_tcp.h>
#include <ace/Task.h>
#include <ace/Atomic_Op.h>
#include <ace/ACE.h>

#include "Database/DatabaseEnv.h"
#include "ByteBuffer.h"
#include "Config/ConfigEnv.h"
#include "Log.h"
#include "RealmList.h"
#include "AuthCodes.h"

#include "Auth/Sha1.h"
#include "Auth/BigNumber.h"
#include "AccountHandler.h"

AuthSocket::AuthSocket () :
ACE_Service_Handler (),
handle_ (ACE_INVALID_HANDLE),
authed_ (false),
closing_ (false),
accountSecurityLevel (SEC_PLAYER),
patch_ (ACE_INVALID_HANDLE)
{
  s.SetRand (s_BYTE_SIZE * 8);
  N.SetHexStr ("894B645E89E1535BBDAD5B8B290650530801B18EBFBF5E8FAB3C82872A3E9BB7");
  g.SetDword (7);

  ACE_NEW_NORETURN (incoming_mb_, ACE_Message_Block (s_INPUT_BUFFER));
  ACE_NEW_NORETURN (outgoing_mb_, ACE_Message_Block (s_OUTPUT_BUFFER));
}

AuthSocket::~AuthSocket ()
{
  DEBUG_LOG ("AuthSocket::~AuthSocket");

  if (this->handle_ != ACE_INVALID_HANDLE)
    ACE_OS::closesocket (this->handle_);

  this->handle_ = ACE_INVALID_HANDLE;

  if (this->incoming_mb_)
    this->incoming_mb_->release ();

  if (this->outgoing_mb_)
    this->outgoing_mb_->release ();

<<<<<<< HEAD:src/realmd/AuthSocket.cpp
  if (this->patch_ != ACE_INVALID_HANDLE)
    ACE_OS::close (patch_);
=======
typedef struct AUTH_LOGON_CHALLENGE_C
{
    uint8   cmd;
    uint8   error;
    uint16  size;
    uint8   gamename[4];
    uint8   version1;
    uint8   version2;
    uint8   version3;
    uint16  build;
    uint8   platform[4];
    uint8   os[4];
    uint8   country[4];
    uint32  timezone_bias;
    uint32  ip;
    uint8   I_len;
    uint8   I[1];
} sAuthLogonChallenge_C;

//typedef sAuthLogonChallenge_C sAuthReconnectChallenge_C;
/*
typedef struct
{
    uint8   cmd;
    uint8   error;
    uint8   unk2;
    uint8   B[32];
    uint8   g_len;
    uint8   g[1];
    uint8   N_len;
    uint8   N[32];
    uint8   s[32];
    uint8   unk3[16];
} sAuthLogonChallenge_S;
*/

typedef struct AUTH_LOGON_PROOF_C
{
    uint8   cmd;
    uint8   A[32];
    uint8   M1[20];
    uint8   crc_hash[20];
    uint8   number_of_keys;
    uint8   securityFlags;                                  // 0x00-0x04
} sAuthLogonProof_C;
/*
typedef struct
{
    uint16  unk1;
    uint32  unk2;
    uint8   unk3[4];
    uint16  unk4[20];
}  sAuthLogonProofKey_C;
*/
typedef struct AUTH_LOGON_PROOF_S
{
    uint8   cmd;
    uint8   error;
    uint8   M2[20];
    uint32  unk1;
    uint32  unk2;
    uint16  unk3;
} sAuthLogonProof_S;

typedef struct AUTH_RECONNECT_PROOF_C
{
    uint8   cmd;
    uint8   R1[16];
    uint8   R2[20];
    uint8   R3[20];
    uint8   number_of_keys;
} sAuthReconnectProof_C;

typedef struct XFER_INIT
{
    uint8 cmd;                                              // XFER_INITIATE
    uint8 fileNameLen;                                      // strlen(fileName);
    uint8 fileName[5];                                      // fileName[fileNameLen]
    uint64 file_size;                                       // file size (bytes)
    uint8 md5[MD5_DIGEST_LENGTH];                           // MD5
}XFER_INIT;

typedef struct XFER_DATA
{
    uint8 opcode;
    uint16 data_size;
    uint8 data[ChunkSize];
}XFER_DATA_STRUCT;
>>>>>>> c6d97c6ec37fe4978cbd521620cc6e8cd926682a:src/realmd/AuthSocket.cpp

  this->patch_ = ACE_INVALID_HANDLE;
}

void
AuthSocket::open (ACE_HANDLE new_handle,
                  ACE_Message_Block&)
{
  this->handle_ = new_handle;

  // Don't buffer serial sends.
  int nodelay = 1;
  ACE_SOCK_Stream option_setter (new_handle);

  if (-1 == option_setter.set_option (ACE_IPPROTO_TCP,
                                      TCP_NODELAY,
                                      &nodelay,
                                      sizeof (nodelay)))
    {
      sLog.outError ("AuthSocket::open set_option TCP_NODELAY %s", ACE_OS::strerror (errno));
    }

  // Open the Asynch streams we will use.
  if (this->ws_.open (*this, this->handle_) == -1)
    {
      sLog.outError ("AuthSocket::open ACE_Asynch_Write_Stream::open");
    }
  else if (this->rs_.open (*this, this->handle_) == -1)
    {
      sLog.outError ("AuthSocket::open ACE_Asynch_Read_Stream::open");
    }
  else if (this->initiate_read () == -1)
    {
      sLog.outError ("AuthSocket::initiate_connection");
    }
  else
    {
      return;
    }

  delete this;
}

void
AuthSocket::addresses (const ACE_INET_Addr &remote_address,
                       const ACE_INET_Addr &local_address)
{
  DEBUG_LOG ("AuthSocket::addresses accepting connection from %s %s:%d",
             remote_address.get_host_name (),
             remote_address.get_host_addr (),
             remote_address.get_port_number ());

  this->remote_address_.set (remote_address);
}

int
AuthSocket::initiate_read (size_t size, bool reset)
{
  // Check integrity.

  // Can't read zero size :)
  ACE_ASSERT (size > 0);
  // We already have initiated read maybe.
  ACE_ASSERT (this->incoming_mb_ != NULL);

  // We got disconnected maybe.
  if (this->handle_ == ACE_INVALID_HANDLE)
    {
      DEBUG_LOG ("AuthSocket::initiate_read socket was closed");
      return -1;
    }

  // Wierd ... we need bigger buffer maybe.
  if (this->incoming_mb_->space () <= size)
    {
      sLog.outError ("AuthSocket::initiate_read input buffer overflow");
      return -1;
    }

  // Reset the indexes of the block, 
  // so that all starts from beginning.
  if (reset)
    this->incoming_mb_->reset ();

  // Read one byte, with the opcode.
  if (this->rs_.read (*this->incoming_mb_, size) == -1)
    {
      DEBUG_LOG ("AuthSocket::initiate_read error, closing connection");
      return -1;
    }

  // OK write started.
  // Give the ownership of the message block to the Proactor.
  this->incoming_mb_ = NULL;

  return 0;
}

int
AuthSocket::send_data (const char* data, size_t size)
{
  // Check integrity.

  // Can't send zero size :)
  ACE_ASSERT (size > 0);

  // We already have initiated connection maybe.
  if (this->outgoing_mb_ == NULL)
    {
      sLog.outError ("AuthSocket::send_data already have output");
      return -1;
    }

  // We got disconnected maybe.
  if (this->handle_ == ACE_INVALID_HANDLE)
    {
      DEBUG_LOG ("AuthSocket::send_data socket was closed");
      return -1;
    }

  // Wierd ... we need bigger buffer maybe.
  if (this->outgoing_mb_->space () <= size)
    {
      sLog.outError ("AuthSocket::send_data output buffer overflow");
      return -1;
    }

  // Put the data on the buffer, it will be sent by flush_data ().
  return this->outgoing_mb_->copy (data, size);
}

int
AuthSocket::flush_data ()
{
  if (this->outgoing_mb_ == NULL)
    return 0;

  if (this->handle_ == ACE_INVALID_HANDLE)
    {
      DEBUG_LOG ("AuthSocket::flush_data socket was closed");
      return -1;
    }

  if (this->outgoing_mb_->length () == 0)
    return 0;

  if (this->ws_.write (*this->outgoing_mb_, this->outgoing_mb_->length ()) == -1)
    return -1;

  // OK IO scheduled.
  this->outgoing_mb_ = NULL;

  // Did the job
  return 1;
}

void
AuthSocket::handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)
{
  // Check integrity.
  // When we shedule input we 
  ACE_ASSERT (this->incoming_mb_ == NULL);

  // Store back the packet we sent for output.
  this->incoming_mb_ = &result.message_block ();

  // Check for errors.
  if (result.error () != 0 ||
      this->closing_ ||
      result.success () == 0 ||
      result.bytes_transferred () != result.bytes_to_read ()
      )
    {
      DEBUG_LOG ("AuthSocket::handle_read_stream closing connection");
      delete this;
      return;
    }

<<<<<<< HEAD:src/realmd/AuthSocket.cpp
  // Reset the read pointer in case some handler readed something.
  this->incoming_mb_->rd_ptr (this->incoming_mb_->base ());

  // Ok, we got the data. Now process it.
  uint8 cmd = *((uint8*) (this->incoming_mb_->rd_ptr ()));
  this->incoming_mb_->rd_ptr (1);

  ssize_t cresult = 0;
  // cresult semantics:
  // -1 - Error, close connection.
  // 0 - Command successfully processed.
  // n ( n > 0 ) - Need `n' more bytes. 
  // In last case the handler will get called, 
  // again when the socket receives these bytes.

  // Avoid returning positive and scheduling write at the same time.
  // Its not good to want to send data,
  // and at the same time to want to read more data.
=======
        ///- Get the command out of it
        ibuf.SoftRead((char *)&_cmd, 1);                    // UQ1: No longer exists in new net code ???

        size_t i;

        ///- Circle through known commands and call the correct command handler
        for (i = 0; i < AUTH_TOTAL_COMMANDS; ++i)
        {
            if ((uint8)table[i].cmd == _cmd &&
                (table[i].status == STATUS_CONNECTED ||
                (_authed && table[i].status == STATUS_AUTHED)))
            {
                DEBUG_LOG("[Auth] got data for cmd %u ibuf length %u", (uint32)_cmd, ibuf.GetLength());
>>>>>>> c6d97c6ec37fe4978cbd521620cc6e8cd926682a:src/realmd/AuthSocket.cpp

  switch (cmd)
    {
    case AUTH_LOGON_CHALLENGE:
      {
        cresult = handle_logon_challenge ();
      }
      break;
    case AUTH_LOGON_PROOF:
      {
        cresult = handle_logon_proof ();
      }
      break;
    case REALM_LIST:
      {
        cresult = handle_realmlist ();
      }
      break;
    case AUTH_RECONNECT_CHALLENGE:
      {
        cresult = handle_reconnect_challenge ();
      }
      break;
    case AUTH_RECONNECT_PROOF:
      {
        cresult = handle_reconnect_proof ();
      }
      break;
    case XFER_ACCEPT:
      {
        cresult = handle_transfer_accept ();
      }
      break;
    case XFER_RESUME:
      {
        cresult = handle_transfer_resume ();
      }
      break;
    case AUTH_NO_CMD:
      {
        sLog.outDetail ("AuthSocket::handle_read_stream AUTH_NO_CMD from %s", this->GetRemoteAddress ());
        cresult = 0;
      }
      break;
    default:
      {
        DEBUG_LOG ("AuthSocket::handle_read_stream unknown packet %u", (uint32) cmd);
        cresult = -1;
      }
      break;
    }

<<<<<<< HEAD:src/realmd/AuthSocket.cpp
  // Reset the read pointer in case some handler readed something.
  this->incoming_mb_->rd_ptr (this->incoming_mb_->base ());

  // Maybe we need more to read ?
  if (cresult > 0)
    {
      const size_t bytes_to_read = static_cast<size_t> (cresult);
      cresult = this->initiate_read (bytes_to_read, false);
    }
  else if (cresult == 0)
    {
      // Will we write ?
      cresult = this->flush_data ();

      // If we didn`t write , lets read.
      if (cresult == 0)
        cresult = initiate_read ();
=======
        ///- Report unknown commands in the debug log
        if (i == AUTH_TOTAL_COMMANDS)
        {
            DEBUG_LOG("[Auth] got unknown packet %u", (uint32)_cmd);
            return;
        }
>>>>>>> c6d97c6ec37fe4978cbd521620cc6e8cd926682a:src/realmd/AuthSocket.cpp
    }

  if (cresult == -1)
    {
      delete this;
    }
  // DONT PUT CODE HERE !!!
}

void
AuthSocket::handle_write_stream (const ACE_Asynch_Write_Stream::Result &result)
{
<<<<<<< HEAD:src/realmd/AuthSocket.cpp
  ACE_ASSERT (this->outgoing_mb_ == NULL);

  // Store it 1st, so its memory is managed.
  this->outgoing_mb_ = &result.message_block ();

  // Check for errors.
  if (result.error () != 0 ||
      this->closing_ ||
      result.success () == 0 ||
      result.bytes_transferred () != result.bytes_to_write ()
      )
    {
      DEBUG_LOG ("AuthSocket::handle_write_stream closing connection");
      delete this;
      return;
    }

  // Reset the output buffer.
  this->outgoing_mb_->reset ();

  // Now initiate some output
  if (initiate_read () == -1)
    delete this;
=======
    BigNumber I;
    I.SetHexStr(rI.c_str());

    // In case of leading zeros in the rI hash, restore them
    uint8 mDigest[SHA_DIGEST_LENGTH];
    memset(mDigest, 0, SHA_DIGEST_LENGTH);
    if (I.GetNumBytes() <= SHA_DIGEST_LENGTH)
        memcpy(mDigest, I.AsByteArray(), I.GetNumBytes());

    std::reverse(mDigest, mDigest + SHA_DIGEST_LENGTH);

    Sha1Hash sha;
    sha.UpdateData(s.AsByteArray(), s.GetNumBytes());
    sha.UpdateData(mDigest, SHA_DIGEST_LENGTH);
    sha.Finalize();
    BigNumber x;
    x.SetBinary(sha.GetDigest(), sha.GetLength());
    v = g.ModExp(x, N);
    // No SQL injection (username escaped)
    const char *v_hex, *s_hex;
    v_hex = v.AsHexStr();
    s_hex = s.AsHexStr();
    loginDatabase.PExecute("UPDATE account SET v = '%s', s = '%s' WHERE username = '%s'", v_hex, s_hex, _safelogin.c_str() );
    OPENSSL_free((void*)v_hex);
    OPENSSL_free((void*)s_hex);
>>>>>>> c6d97c6ec37fe4978cbd521620cc6e8cd926682a:src/realmd/AuthSocket.cpp
}

ssize_t
AuthSocket::handle_logon_challenge ()
{
<<<<<<< HEAD:src/realmd/AuthSocket.cpp
  DEBUG_LOG ("AuthSocket::handle_logon_challenge");

  // read error and size
  if (this->incoming_mb_->length () < 3)
=======
    DEBUG_LOG("Entering _HandleLogonChallenge");
    if (ibuf.GetLength() < sizeof(sAuthLogonChallenge_C))
        return false;

    ///- Read the first 4 bytes (header) to get the length of the remaining of the packet
    std::vector<uint8> buf;
    buf.resize(4);

    ibuf.Read((char *)&buf[0], 4);

    EndianConvert(*((uint16*)(buf[0])));
    uint16 remaining = ((sAuthLogonChallenge_C *)&buf[0])->size;
    DEBUG_LOG("[AuthChallenge] got header, body is %#04x bytes", remaining);

    if ((remaining < sizeof(sAuthLogonChallenge_C) - buf.size()) || (ibuf.GetLength() < remaining))
        return false;

    //No big fear of memory outage (size is int16, i.e. < 65536)
    buf.resize(remaining + buf.size() + 1);
    buf[buf.size() - 1] = 0;
    sAuthLogonChallenge_C *ch = (sAuthLogonChallenge_C*)&buf[0];

    // BigEndian code, nop in little endian case
    // size already converted
    EndianConvert(*((uint32*)(&ch->gamename[0])));
    EndianConvert(ch->build);
    EndianConvert(*((uint32*)(&ch->platform[0])));
    EndianConvert(*((uint32*)(&ch->os[0])));
    EndianConvert(*((uint32*)(&ch->country[0])));
    EndianConvert(ch->timezone_bias);
    EndianConvert(ch->ip);

    ///- Read the remaining of the packet
    ibuf.Read((char *)&buf[4], remaining);
    DEBUG_LOG("[AuthChallenge] got full packet, %#04x bytes", ch->size);
    DEBUG_LOG("[AuthChallenge] name(%d): '%s'", ch->I_len, ch->I);

    ByteBuffer pkt;

    _login = (const char*)ch->I;
    _build = ch->build;

    ///- Normalize account name
    //utf8ToUpperOnlyLatin(_login); -- client already send account in expected form

    //Escape the user login to avoid further SQL injection
    //Memory will be freed on AuthSocket object destruction
    _safelogin = _login;
    loginDatabase.escape_string(_safelogin);

    pkt << (uint8) AUTH_LOGON_CHALLENGE;
    pkt << (uint8) 0x00;

    ///- Verify that this IP is not in the ip_banned table
    // No SQL injection possible (paste the IP address as passed by the socket)
    loginDatabase.Execute("DELETE FROM ip_banned WHERE unbandate<=UNIX_TIMESTAMP() AND unbandate<>bandate");

    std::string address = GetRemoteAddress();
    loginDatabase.escape_string(address);
    QueryResult *result = loginDatabase.PQuery(  "SELECT * FROM ip_banned WHERE ip = '%s'",address.c_str());
    if(result)
>>>>>>> c6d97c6ec37fe4978cbd521620cc6e8cd926682a:src/realmd/AuthSocket.cpp
    {
      return 3 - this->incoming_mb_->length ();
    }

  // Skip `error'
  this->incoming_mb_->rd_ptr (1);

  // read the size of the structure to know what is the remainig size.
  uint16 remaining = *((uint16*) (this->incoming_mb_->rd_ptr ()));
  this->incoming_mb_->rd_ptr (2);

  // Endian convert
  EndianConvert (remaining);

  // Check remaining.
  if (remaining < sizeof (sAuthLogonChallengeC))
    {
      sLog.outError ("AuthSocket::handle_logon_challenge size underflow");
      return -1;
    }

  // read the rest of the structure.
  if (remaining > this->incoming_mb_->length ())
    return remaining;

  // Need one more byte fro the null terminator of I.
  if (this->incoming_mb_->space () < 1)
    return -1;

  // Null terminate the whole thing.
  *(this->incoming_mb_->wr_ptr ()) = '\0';
  this->incoming_mb_->wr_ptr (1);

<<<<<<< HEAD:src/realmd/AuthSocket.cpp
  // OK we have the struct. Now check it.
  sAuthLogonChallengeC& challange = *((sAuthLogonChallengeC*) (this->incoming_mb_->rd_ptr ()));
=======
                    b.SetRand(19 * 8);
                    BigNumber gmod = g.ModExp(b, N);
                    B = ((v * 3) + gmod) % N;
>>>>>>> c6d97c6ec37fe4978cbd521620cc6e8cd926682a:src/realmd/AuthSocket.cpp

  // I_len is strlen(I), check it.
  if ((challange.I_len + sizeof (sAuthLogonChallengeC)) > this->incoming_mb_->length ())
    {
      sLog.outError ("AuthSocket::handle_logon_challenge I_len size overflow");
      return -1;
    }

<<<<<<< HEAD:src/realmd/AuthSocket.cpp
  // Endian convert
  EndianConvert (challange.build);
  EndianConvert (challange.ip);
  EndianConvert (challange.timezone_bias);

  // Ok, the struct is valid, have fun.
  return handle_logon_challenge (challange);
}

ssize_t
AuthSocket::handle_logon_challenge (sAuthLogonChallengeC& cs)
{
  // The previous function checked the struct, we asume its valid.
=======
                    BigNumber unk3;
                    unk3.SetRand(16 * 8);

                    ///- Fill the response packet with the result
                    pkt << uint8(REALM_AUTH_SUCCESS);

                    // B may be calculated < 32B so we force minimal length to 32B
                    pkt.append(B.AsByteArray(32), 32);      // 32 bytes
                    pkt << uint8(1);
                    pkt.append(g.AsByteArray(), 1);
                    pkt << uint8(32);
                    pkt.append(N.AsByteArray(), 32);
                    pkt.append(s.AsByteArray(), s.GetNumBytes());// 32 bytes
                    pkt.append(unk3.AsByteArray(), 16);
                    uint8 securityFlags = 0;
                    pkt << uint8(securityFlags);            // security flags (0x0...0x04)

                    if(securityFlags & 0x01)                // PIN input
                    {
                        pkt << uint32(0);
                        pkt << uint64(0) << uint64(0);      // 16 bytes hash?
                    }

                    if(securityFlags & 0x02)                // Matrix input
                    {
                        pkt << uint8(0);
                        pkt << uint8(0);
                        pkt << uint8(0);
                        pkt << uint8(0);
                        pkt << uint64(0);
                    }

                    if(securityFlags & 0x04)                // Security token input
                    {
                        pkt << uint8(1);
                    }
>>>>>>> c6d97c6ec37fe4978cbd521620cc6e8cd926682a:src/realmd/AuthSocket.cpp

  DEBUG_LOG ("AuthSocket::handle_logon_challenge");
  DEBUG_LOG ("Account %s", (const char*) cs.I);

<<<<<<< HEAD:src/realmd/AuthSocket.cpp
  AcctMgr.ReloadEverythingIfNeed();

  login_ = (const char*) cs.I;

  safelogin_ = login_;
  dbRealmServer.escape_string (safelogin_);

  // Check if the client has one of the expected version numbers
  bool valid_version = false;
  int accepted_versions[] = EXPECTED_MANGOS_CLIENT_BUILD;
  for (int i = 0; accepted_versions[i]; i++)
    if (cs.build == accepted_versions[i])
      {
        valid_version = true;
        break;
      }

  // Maybe send new version to client ?
  if (!valid_version)
    return this->handle_clent_old_version (cs);

  ByteBuffer pkt;

  pkt << (uint8) AUTH_LOGON_CHALLENGE;
  pkt << (uint8) 0x00;

  if(sConfig.GetIntDefault("AccountCaching", 0) != 1)
  {

      // Update `ip_banned' table
      dbRealmServer.Execute ("DELETE FROM ip_banned "
                             "WHERE unbandate<=UNIX_TIMESTAMP() "
                             "AND unbandate<>bandate");

      std::string address = GetRemoteAddress ();
      dbRealmServer.escape_string (address);

      // Verify that this IP is not in the ip_banned table
      QueryResult *result = dbRealmServer.PQuery ("SELECT * "
                                                  "FROM ip_banned "
                                                  "WHERE ip = '%s'",
                                                  address.c_str ());

      // IP is banned
      if (result)
        {
          pkt << (uint8) REALM_AUTH_ACCOUNT_BANNED;
          sLog.outBasic ("AuthSocket::handle_logon_challenge Banned ip %s tries to login!", GetRemoteAddress ());
          delete result;
          this->closing_ = true;
          return send_data ((const char*) pkt.contents (), pkt.size ());
=======
                    _localizationName.resize(4);
                    for(int i = 0; i < 4; ++i)
                        _localizationName[i] = ch->country[4-i-1];

                    sLog.outBasic("[AuthChallenge] account %s is using '%c%c%c%c' locale (%u)", _login.c_str (), ch->country[3], ch->country[2], ch->country[1], ch->country[0], GetLocaleByName(_localizationName));
                }
            }
            delete result;
>>>>>>> c6d97c6ec37fe4978cbd521620cc6e8cd926682a:src/realmd/AuthSocket.cpp
        }
      delete result;
      // Check if account is locked.
      result = dbRealmServer.PQuery ("SELECT "
                                     "sha_pass_hash,"
                                     "id,locked,"
                                     "last_ip,"
                                     "gmlevel "
                                     "FROM account "
                                     "WHERE username = '%s'",
                                     safelogin_.c_str ());

      if (!result)
        {
          // Account doesnt exist at all.
          pkt << (uint8) REALM_AUTH_NO_MATCH;
          this->closing_ = true;
          return send_data ((const char*) pkt.contents (), pkt.size ());
        }

<<<<<<< HEAD:src/realmd/AuthSocket.cpp
      // If the IP is 'locked', check that the player comes indeed from the correct IP address
      if ((*result)[2].GetUInt8 () == 1) // if ip is locked
        {
          DEBUG_LOG ("AuthSocket::handle_logon_challenge Account '%s' is locked to IP - '%s'",
                     login_.c_str (), (*result)[3].GetString ());
=======
/// Logon Proof command handler
bool AuthSocket::_HandleLogonProof()
{
    DEBUG_LOG("Entering _HandleLogonProof");
    ///- Read the packet
    if (ibuf.GetLength() < sizeof(sAuthLogonProof_C))
        return false;
    sAuthLogonProof_C lp;
    ibuf.Read((char *)&lp, sizeof(sAuthLogonProof_C));

    ///- Check if the client has one of the expected version numbers
    bool valid_version = false;
    int accepted_versions[] = EXPECTED_MANGOS_CLIENT_BUILD;
    for(int i = 0; accepted_versions[i]; ++i)
    {
        if(_build == accepted_versions[i])
        {
            valid_version = true;
            break;
        }
    }
>>>>>>> c6d97c6ec37fe4978cbd521620cc6e8cd926682a:src/realmd/AuthSocket.cpp

          DEBUG_LOG ("AuthSocket::handle_logon_challenge Player address is '%s'", GetRemoteAddress ());
          if (strcmp ((*result)[3].GetString (), GetRemoteAddress ()))
            {
              DEBUG_LOG ("AuthSocket::handle_logon_challenge Account IP differs");
              pkt << (uint8) REALM_AUTH_ACCOUNT_FREEZED;
              this->closing_ = true;
              return send_data ((const char*) pkt.contents (), pkt.size ());
            }
          else
            {
              DEBUG_LOG ("AuthSocket::handle_logon_challenge Account IP matches");
            }
        }
      else
        {
          DEBUG_LOG ("AuthSocket::handle_logon_challenge Account '%s' is not locked to ip", login_.c_str ());
        }

<<<<<<< HEAD:src/realmd/AuthSocket.cpp
      // Check account name BANs
      //set expired bans to inactive
      dbRealmServer.Execute ("UPDATE account_banned "
                             "SET active = 0 "
                             "WHERE unbandate<=UNIX_TIMESTAMP() "
                             "AND unbandate<>bandate");

      ///- If the account is banned, reject the logon attempt
      QueryResult *banresult = dbRealmServer.PQuery ("SELECT "
                                                     "bandate, "
                                                     "unbandate "
                                                     "FROM account_banned "
                                                     "WHERE id = %u "
                                                     "AND active = 1",
                                                     (*result)[1].GetUInt32 ());

      // Looks like he/she is banned.
      if (banresult)
        {
          if ((*banresult)[0].GetUInt64 () == (*banresult)[1].GetUInt64 ())
            {
              pkt << (uint8) REALM_AUTH_ACCOUNT_BANNED;
              sLog.outBasic ("AuthSocket::handle_logon_challenge Banned account %s tries to login!", login_.c_str ());
            }
          else
            {
              pkt << (uint8) REALM_AUTH_ACCOUNT_FREEZED;
              sLog.outBasic ("AuthSocket::handle_logon_challenge Temporarily banned account %s tries to login!", login_.c_str ());
            }
=======
        // 24 = len("./patches/65535enGB.mpq")+1
        char tmp[24];
        // No buffer overflow (fixed length of arguments)
        sprintf(tmp, "./patches/%d%s.mpq", _build, _localizationName.c_str());
        // This will be closed at the destruction of the AuthSocket (client disconnection)
        FILE *pFile = fopen(tmp, "rb");
>>>>>>> c6d97c6ec37fe4978cbd521620cc6e8cd926682a:src/realmd/AuthSocket.cpp

          this->closing_ = true;
          return send_data ((const char*) pkt.contents (), pkt.size ());
        }
      delete banresult;

      // Now for the real thing :)
      // Get the password from the account table, upper it, and make the SRP6 calculation
      std::string rI = (*result)[0].GetCppString ();
      SetVSFields (rI);

      b.SetRand (19 * 8);

      BigNumber gmod = g.ModExp (b, N);
      B = ((v * 3) + gmod) % N;

      ACE_ASSERT (gmod.GetNumBytes () <= 32);

      BigNumber unk3;
      unk3.SetRand (16 * 8);

      // Fill the response packet with the result
      pkt << (uint8) REALM_AUTH_SUCCESS;

      // B may be calculated < 32B so we force minnimal length to 32B
      pkt.append (B.AsByteArray (32), 32); // 32 bytes
      pkt << (uint8) 1;
      pkt.append (g.AsByteArray (), 1);
      pkt << (uint8) 32;
      pkt.append (N.AsByteArray (), 32);
      pkt.append (s.AsByteArray (), s.GetNumBytes ()); // 32 bytes
      pkt.append (unk3.AsByteArray (), 16);
      pkt << (uint8) 0; // Added in 1.12.x client branch

      uint8 secLevel = (*result)[4].GetUInt8 ();
      delete result;
      accountSecurityLevel = secLevel <= SEC_ADMINISTRATOR ? AccountTypes (secLevel) : SEC_ADMINISTRATOR;

      std::string localeName;
      localeName.resize (4);
      for (int i = 0; i < 4; ++i)
        localeName[i] = cs.country[4 - i - 1];

      locale_ = GetLocaleByName (localeName);
  }
  else
  {
        if(AcctMgr.IpBan(GetRemoteAddress()))
        {
<<<<<<< HEAD:src/realmd/AuthSocket.cpp
            pkt << (uint8)REALM_AUTH_ACCOUNT_BANNED;
=======
            ByteBuffer pkt;
            pkt << (uint8) AUTH_LOGON_CHALLENGE;
            pkt << (uint8) 0x00;
            pkt << (uint8) REALM_AUTH_WRONG_BUILD_NUMBER;
            DEBUG_LOG("[AuthChallenge] %u is not a valid client version!", _build);
            DEBUG_LOG("[AuthChallenge] Patch %s not found", tmp);
            SendBuf((char const*)pkt.contents(), pkt.size());
            return true;
>>>>>>> c6d97c6ec37fe4978cbd521620cc6e8cd926682a:src/realmd/AuthSocket.cpp
        }
        else
        {
<<<<<<< HEAD:src/realmd/AuthSocket.cpp
            Account* acct = AcctMgr.FindAccountByName(safelogin_);

            if(acct)
            {
                bool locked = false;

                if(acct->locked == 1)                   // if ip is locked
                {
                    if(strcmp(acct->lastip.c_str(),GetRemoteAddress()))
                    {
                        pkt << (uint8) REALM_AUTH_ACCOUNT_FREEZED;
                        locked = true;
                    }
                }

                if (!locked)
                {
                    if(acct->banned)
                    {
                        pkt << (uint8) REALM_AUTH_ACCOUNT_BANNED;
                    }
                    else
                    {
                        ///- Get the password from the account table, upper it, and make the SRP6 calculation
                        std::string rI = acct->I;
                        SetVSFields(rI);
                        b.SetRand(19 * 8);
                        BigNumber gmod=g.ModExp(b, N);
                        B = ((v * 3) + gmod) % N;

                        if (B.GetNumBytes() < 32)
                            sLog.outDetail("Interesting, calculation of B in realmd is < 32.");

                        ACE_ASSERT(gmod.GetNumBytes() <= 32);

                        BigNumber unk3;
                        unk3.SetRand(16*8);

                        ///- Fill the response packet with the result
                        pkt << (uint8)REALM_AUTH_SUCCESS;
                        pkt.append(B.AsByteArray(), B.GetNumBytes());
                        pkt << (uint8)1;
                        pkt.append(g.AsByteArray(), 1);
                        pkt << (uint8)32;
                        pkt.append(N.AsByteArray(), 32);
                        pkt.append(s.AsByteArray(), s.GetNumBytes());
                        pkt.append(unk3.AsByteArray(), 16);
                        pkt << (uint8)0;                // Added in 1.12.x client branch

                        uint8 secLevel = acct->security;
                          accountSecurityLevel = secLevel <= SEC_ADMINISTRATOR ? AccountTypes (secLevel) : SEC_ADMINISTRATOR;

                          std::string localeName;
                          localeName.resize (4);
                          for (int i = 0; i < 4; ++i)
                            localeName[i] = cs.country[4 - i - 1];

                          locale_ = GetLocaleByName (localeName);
                        //sLog.outBasic("[AuthChallenge] account %s is using '%c%c%c%c' locale (%u)", _login.c_str (), ch->country[3],ch->country[2],ch->country[1],ch->country[0], GetLocaleByName(_localizationName));

                    }
                }
                else
                {
                    pkt << (uint8) REALM_AUTH_ACCOUNT_FREEZED;
                }
            }
            else                                        //no account
            {
                pkt << (uint8) REALM_AUTH_NO_MATCH;
            }
        }
    }


  DEBUG_LOG ("AuthSocket::handle_logon_challenge sending AUTH_LOGON_CHALLENGE");

  return send_data ((const char*) pkt.contents (), pkt.size ());
}

/// Make the SRP6 calculation from hash in dB

void
AuthSocket::SetVSFields (const std::string& rI)
{
  BigNumber I;
  I.SetHexStr (rI.c_str ());

  //In case of leading zeroes in the rI hash, restore them
  uint8 mDigest[SHA_DIGEST_LENGTH];
  memset (mDigest, 0, SHA_DIGEST_LENGTH);
  if (I.GetNumBytes () <= SHA_DIGEST_LENGTH)
    memcpy (mDigest, I.AsByteArray (), I.GetNumBytes ());

  std::reverse (mDigest, mDigest + SHA_DIGEST_LENGTH);

  Sha1Hash sha;
  sha.UpdateData (s.AsByteArray (), s.GetNumBytes ());
  sha.UpdateData (mDigest, SHA_DIGEST_LENGTH);
  sha.Finalize ();
  BigNumber x;
  x.SetBinary (sha.GetDigest (), sha.GetLength ());
  v = g.ModExp (x, N);
  const char *v_hex, *s_hex;
  v_hex = v.AsHexStr ();
  s_hex = s.AsHexStr ();

  dbRealmServer.DirectPExecute ("UPDATE account "
                          "SET v = '%s', "
                          "s = '%s' "
                          "WHERE username = '%s'",
                          v_hex,
                          s_hex,
                          safelogin_.c_str ());

  OPENSSL_free ((void*) v_hex);
  OPENSSL_free ((void*) s_hex);
}

ssize_t
AuthSocket::handle_logon_proof ()
{
  DEBUG_LOG ("AuthSocket::handle_logon_proof");

  // Check if we have all data.
  if (this->incoming_mb_->length () < sizeof (sAuthLogonProofC))
    return sizeof (sAuthLogonProofC) - this->incoming_mb_->length ();

  // Get the data.
  sAuthLogonProofC& lp = *((sAuthLogonProofC*)this->incoming_mb_->rd_ptr ());
  this->incoming_mb_->rd_ptr (sizeof (sAuthLogonProofC));

  ///- Continue the SRP6 calculation based on data received from the client
  BigNumber A;
  A.SetBinary (lp.A, 32);

  Sha1Hash sha;
  sha.UpdateBigNumbers (&A, &B, NULL);
  sha.Finalize ();
  BigNumber u;
  u.SetBinary (sha.GetDigest (), 20);
  BigNumber S = (A * (v.ModExp (u, N))).ModExp (b, N);

  uint8 t[32];
  uint8 t1[16];
  uint8 vK[40];
  memcpy (t, S.AsByteArray (), 32);
  for (int i = 0; i < 16; i++)
    {
      t1[i] = t[i * 2];
    }
  sha.Initialize ();
  sha.UpdateData (t1, 16);
  sha.Finalize ();
  for (int i = 0; i < 20; i++)
    {
      vK[i * 2] = sha.GetDigest ()[i];
    }
  for (int i = 0; i < 16; i++)
    {
      t1[i] = t[i * 2 + 1];
    }
  sha.Initialize ();
  sha.UpdateData (t1, 16);
  sha.Finalize ();
  for (int i = 0; i < 20; i++)
=======
            pPatch = pFile;
            XFER_INIT xferh;

            ///- Get the MD5 hash of the patch file (get it from preloaded Patcher cache or calculate it)
            if(PatchesCache.GetHash(tmp, (uint8*)&xferh.md5))
            {
                DEBUG_LOG("\n[AuthChallenge] Found precached patch info for patch %s", tmp);
            }
            else
            {                                               // calculate patch md5
                printf("\n[AuthChallenge] Patch info for %s was not cached.", tmp);
                PatchesCache.LoadPatchMD5(tmp);
                PatchesCache.GetHash(tmp, (uint8*)&xferh.md5);
            }

            ///- Send a packet to the client with the file length and MD5 hash
            uint8 data[2] = { AUTH_LOGON_PROOF, REALM_AUTH_UPDATE_CLIENT };
            SendBuf((const char*)data, sizeof(data));

            memcpy(&xferh, "0\x05Patch", 7);
            xferh.cmd = XFER_INITIATE;
            fseek(pPatch, 0, SEEK_END);
            xferh.file_size = ftell(pPatch);

            SendBuf((const char*)&xferh, sizeof(xferh));
            return true;
        }
    }
    /// </ul>

    ///- Continue the SRP6 calculation based on data received from the client
    BigNumber A;
    A.SetBinary(lp.A, 32);

    Sha1Hash sha;
    sha.UpdateBigNumbers(&A, &B, NULL);
    sha.Finalize();
    BigNumber u;
    u.SetBinary(sha.GetDigest(), 20);
    BigNumber S = (A * (v.ModExp(u, N))).ModExp(b, N);

    uint8 t[32];
    uint8 t1[16];
    uint8 vK[40];
    memcpy(t, S.AsByteArray(), 32);
    for (int i = 0; i < 16; ++i)
    {
        t1[i] = t[i * 2];
    }
    sha.Initialize();
    sha.UpdateData(t1, 16);
    sha.Finalize();
    for (int i = 0; i < 20; ++i)
    {
        vK[i * 2] = sha.GetDigest()[i];
    }
    for (int i = 0; i < 16; ++i)
    {
        t1[i] = t[i * 2 + 1];
    }
    sha.Initialize();
    sha.UpdateData(t1, 16);
    sha.Finalize();
    for (int i = 0; i < 20; ++i)
    {
        vK[i * 2 + 1] = sha.GetDigest()[i];
    }
    K.SetBinary(vK, 40);

    uint8 hash[20];

    sha.Initialize();
    sha.UpdateBigNumbers(&N, NULL);
    sha.Finalize();
    memcpy(hash, sha.GetDigest(), 20);
    sha.Initialize();
    sha.UpdateBigNumbers(&g, NULL);
    sha.Finalize();
    for (int i = 0; i < 20; ++i)
>>>>>>> c6d97c6ec37fe4978cbd521620cc6e8cd926682a:src/realmd/AuthSocket.cpp
    {
      vK[i * 2 + 1] = sha.GetDigest ()[i];
    }
  K.SetBinary (vK, 40);

  uint8 hash[20];

  sha.Initialize ();
  sha.UpdateBigNumbers (&N, NULL);
  sha.Finalize ();
  memcpy (hash, sha.GetDigest (), 20);
  sha.Initialize ();
  sha.UpdateBigNumbers (&g, NULL);
  sha.Finalize ();
  for (int i = 0; i < 20; i++)
    {
      hash[i] ^= sha.GetDigest ()[i];
    }
<<<<<<< HEAD:src/realmd/AuthSocket.cpp
  BigNumber t3;
  t3.SetBinary (hash, 20);

  sha.Initialize ();
  sha.UpdateData (login_);
  sha.Finalize ();
  uint8 t4[SHA_DIGEST_LENGTH];
  memcpy (t4, sha.GetDigest (), SHA_DIGEST_LENGTH);

  sha.Initialize ();
  sha.UpdateBigNumbers (&t3, NULL);
  sha.UpdateData (t4, SHA_DIGEST_LENGTH);
  sha.UpdateBigNumbers (&s, &A, &B, &K, NULL);
  sha.Finalize ();
  BigNumber M;
  M.SetBinary (sha.GetDigest (), 20);

  // Check if hashes match
  if (memcmp (M.AsByteArray (), lp.M1, 20))
    return handle_wrong_password ();

  sLog.outBasic ("AuthSocket::handle_logon_proof user '%s' successfully authenticated", login_.c_str ());

  // Update the sessionkey, last_ip, last login time and 
  // reset number of failed logins in the account table for this account
  const char* K_hex = K.AsHexStr ();
  dbRealmServer.DirectPExecute ("UPDATE account "
                          "SET sessionkey = '%s', "
                          "last_ip = '%s', "
                          "last_login = NOW(), "
                          "locale = '%u', "
                          "failed_logins = 0 "
                          "WHERE username = '%s'",
                          K_hex,
                          GetRemoteAddress (),
                          locale_,
                          safelogin_.c_str ());

  OPENSSL_free ((void*) K_hex);

  // Finish SRP6 and send the final result to the client
  sha.Initialize ();
  sha.UpdateBigNumbers (&A, &M, &K, NULL);
  sha.Finalize ();

  // @@ Endian convert maybe.
  sAuthLogonProofS proof;
  memcpy (proof.M2, sha.GetDigest (), 20);
  proof.cmd = AUTH_LOGON_PROOF;
  proof.error = 0;
  proof.unk1 = 0x00800000;
  proof.unk2 = 0x00;
  proof.unk3 = 0x00;

  // Set authed_ to true!
  this->authed_ = true;
  return this->send_data ((const char*) & proof, sizeof (proof));
}

ssize_t
AuthSocket::handle_wrong_password ()
{
  sLog.outBasic ("AuthSocket::handle_wrong_password account %s tried to login with wrong password!", login_.c_str ());

  uint32 MaxWrongPassCount = sConfig.GetIntDefault ("WrongPass.MaxCount", 0);
=======
    else
    {
        char data[4]= { AUTH_LOGON_PROOF, REALM_AUTH_NO_MATCH, 3, 0};
        SendBuf(data, sizeof(data));
        sLog.outBasic("[AuthChallenge] account %s tried to login with wrong password!",_login.c_str ());
>>>>>>> c6d97c6ec37fe4978cbd521620cc6e8cd926682a:src/realmd/AuthSocket.cpp

  if (MaxWrongPassCount > 0)
    {
      // Increment number of failed logins by one.
      // If it reaches the limit temporarily ban that account or IP.
      dbRealmServer.PExecute ("UPDATE account "
                              "SET failed_logins = failed_logins + 1 "
                              "WHERE username = '%s'",
                              safelogin_.c_str ());

      if (QueryResult * loginfail = dbRealmServer.PQuery ("SELECT "
                                                          "id, "
                                                          "failed_logins "
                                                          "FROM account "
                                                          "WHERE username = '%s'",
                                                          safelogin_.c_str ()))
        {

          Field* fields = loginfail->Fetch ();
          uint32 failed_logins = fields[1].GetUInt32 ();

          if (failed_logins >= MaxWrongPassCount)
            {
              uint32 WrongPassBanTime = sConfig.GetIntDefault ("WrongPass.BanTime", 600);
              bool WrongPassBanType = sConfig.GetBoolDefault ("WrongPass.BanType", false);

              if (WrongPassBanType)
                {
                  uint32 acc_id = fields[0].GetUInt32 ();
                  dbRealmServer.PExecute ("INSERT INTO "
                                          "account_banned "
                                          "VALUES ('%u',"
                                          "UNIX_TIMESTAMP(),UNIX_TIMESTAMP()+'%u',"
                                          "'MaNGOS realmd',"
                                          "'Failed login autoban',"
                                          "1)",
                                          acc_id,
                                          WrongPassBanTime);

                  sLog.outBasic ("AuthSocket::handle_wrong_password account %s got banned for "
                                 "'%u' seconds because it failed to authenticate '%u' times",
                                 login_.c_str (), WrongPassBanTime, failed_logins);
                }
              else
                {
                  std::string current_ip = GetRemoteAddress ();
                  dbRealmServer.escape_string (current_ip);
                  dbRealmServer.PExecute ("INSERT INTO "
                                          "ip_banned "
                                          "VALUES ('%s',"
                                          "UNIX_TIMESTAMP(),UNIX_TIMESTAMP()+'%u',"
                                          "'MaNGOS realmd',"
                                          "'Failed login autoban')",
                                          current_ip.c_str (),
                                          WrongPassBanTime);
                  sLog.outBasic ("AuthSocket::handle_wrong_password IP %s got banned for '%u' seconds "
                                 "because account %s failed to authenticate '%u' times",
                                 current_ip.c_str (), WrongPassBanTime, login_.c_str (), failed_logins);
                }
            }
            delete loginfail;
        }
    }

  // Send response and close the socket.
  char data[4] = {AUTH_LOGON_PROOF, REALM_AUTH_NO_MATCH, 3, 0};
  this->closing_ = true;
  return this->send_data ((const char*) data, sizeof (data));
}

ssize_t
AuthSocket::handle_realmlist ()
{
  DEBUG_LOG ("AuthSocket::handle_realmlist");
  // Extract 4 bytes that I dont know what are.
  if (this->incoming_mb_->length () < 4)
    return 4 - this->incoming_mb_->length ();

  if (!this->authed_)
    {
      sLog.outBasic ("AuthSocket::handle_realmlist user is not authed");
      return -1;
    }

    std::string rI;
    uint32 id;

    if(sConfig.GetIntDefault("AccountCaching", 0) != 1)
    {
        ///- Get the user id (else close the connection) AND `activekey` = '0'
        // No SQL injection (escaped user name)

        QueryResult *result = dbRealmServer.PQuery("SELECT id,sha_pass_hash FROM account WHERE username = '%s'",safelogin_.c_str());
        if(!result)
        {
            sLog.outError("[ERROR] user %s tried to login and we cannot find him in the database.",safelogin_.c_str());
            return -1;
        }

        id = (*result)[0].GetUInt32();
        rI = (*result)[1].GetCppString();
        delete result;
    }
    else
    {
        Account* acct = AcctMgr.FindAccountByName(safelogin_);
        id = acct->id;
        rI = acct->I;
    }

  // Update realm list if need
  m_realmList.UpdateIfNeed ();

  // Circle through realms in the RealmList and construct the return packet (including # of user characters in each realm)
  ByteBuffer pkt;
  pkt << (uint32) 0;
  pkt << (uint16) m_realmList.size ();
  RealmList::RealmMap::const_iterator i;
  for (i = m_realmList.begin (); i != m_realmList.end (); i++)
    {
      uint8 AmountOfCharacters;

        if(sConfig.GetIntDefault("AccountCaching", 0) != 1)
        {
            // No SQL injection. id of realm is controlled by the database.
            QueryResult* result = dbRealmServer.PQuery( "SELECT numchars FROM realmcharacters WHERE realmid = '%d' AND acctid='%u'",i->second.m_ID,id);
            if( result )
            {
                Field *fields = result->Fetch();
                AmountOfCharacters = fields[0].GetUInt8();
                delete result;
            }
            else
                AmountOfCharacters = 0;
        }
        else
            AmountOfCharacters = AcctMgr.GetNumChar(i->second.m_ID, id);


      uint8 lock = (i->second.allowedSecurityLevel > accountSecurityLevel) ? 1 : 0;

      pkt << i->second.icon; // realm type
      pkt << lock; // if 1, then realm locked
      pkt << i->second.color; // if 2, then realm is offline
      pkt << i->first;
      pkt << i->second.address;
      pkt << i->second.populationLevel;
      pkt << AmountOfCharacters;
      pkt << i->second.timezone; // realm category
      pkt << (uint8) 0x2C; // unk, may be realm number/id?
    }
  pkt << (uint8) 0x10;
  pkt << (uint8) 0x00;

  ByteBuffer hdr;
  hdr << (uint8) REALM_LIST;
  hdr << (uint16) pkt.size ();
  hdr.append (pkt);

<<<<<<< HEAD:src/realmd/AuthSocket.cpp
  SetVSFields (rI);
  return this->send_data ((char const*) hdr.contents (), hdr.size ());
=======
    Field* fields = result->Fetch ();
    K.SetHexStr (fields[0].GetString ());
    delete result;

    ///- Sending response
    ByteBuffer pkt;
    pkt << (uint8)  AUTH_RECONNECT_CHALLENGE;
    pkt << (uint8)  0x00;
    _reconnectProof.SetRand(16 * 8);
    pkt.append(_reconnectProof.AsByteBuffer());             // 16 bytes random
    pkt << (uint64) 0x00 << (uint64) 0x00;                  // 16 bytes zeros
    SendBuf((char const*)pkt.contents(), pkt.size());
    return true;
>>>>>>> c6d97c6ec37fe4978cbd521620cc6e8cd926682a:src/realmd/AuthSocket.cpp
}

ssize_t
AuthSocket::handle_reconnect_challenge ()
{
  DEBUG_LOG ("AuthSocket::handle_reconnect_challenge");

  // check if we need more data

  // read error and size
  if (this->incoming_mb_->length () < 3)
    {
      return 3 - this->incoming_mb_->length ();
    }

  // Skip `error'
  this->incoming_mb_->rd_ptr (1);

  // read the size of the structure to know what is the remainig size.
  uint16 remaining = *((uint16*) (this->incoming_mb_->rd_ptr ()));
  this->incoming_mb_->rd_ptr (2);

  EndianConvert (remaining);

  // Check remaining.
  if (remaining < sizeof (sAuthLogonChallengeC))
    {
      sLog.outError ("AuthSocket::handle_logon_challenge size underflow");
      return -1;
    }

  // read the rest of the structure.
  if (remaining > this->incoming_mb_->length ())
    return remaining;

  // Need one more byte for the null terminator of I.
  if (this->incoming_mb_->space () < 1)
    return -1;

<<<<<<< HEAD:src/realmd/AuthSocket.cpp
  // Null terminate the whole thing.
  *(this->incoming_mb_->wr_ptr ()) = '\0';
  this->incoming_mb_->wr_ptr (1);

  // OK we have the struct. Now check it.
  sAuthReconChallengeC& challange = *((sAuthReconChallengeC*) (this->incoming_mb_->rd_ptr ()));

  // I_len is strlen(I), check it.
  if ((challange.I_len + sizeof (sAuthLogonChallengeC)) > this->incoming_mb_->length ())
=======
    ///- Circle through realms in the RealmList and construct the return packet (including # of user characters in each realm)
    ByteBuffer pkt;
    pkt << (uint32) 0;
    pkt << (uint16) m_realmList.size();
    RealmList::RealmMap::const_iterator i;
    for( i = m_realmList.begin(); i != m_realmList.end(); ++i )
>>>>>>> c6d97c6ec37fe4978cbd521620cc6e8cd926682a:src/realmd/AuthSocket.cpp
    {
      sLog.outError ("AuthSocket::handle_logon_challenge I_len size overflow");
      return -1;
    }

  // Endian convert
  EndianConvert (challange.build);
  EndianConvert (challange.ip);
  EndianConvert (challange.timezone_bias);

  return handle_reconnect_challenge (challange);
}

ssize_t
AuthSocket::handle_reconnect_challenge (sAuthReconChallengeC& cs)
{
  login_ = (const char*) cs.I;

  safelogin_ = login_;
  dbRealmServer.escape_string (safelogin_);

  QueryResult *result = dbRealmServer.PQuery ("SELECT sessionkey FROM account WHERE username = '%s'", safelogin_.c_str ());

  // Stop if the account is not found
  if (!result)
    {
      sLog.outError ("AuthSocket::handle_reconnect_challenge user %s tried to login and we cannot find his session key in the database.", login_.c_str ());
      return -1;
    }

  Field* fields = result->Fetch ();
  K.SetHexStr (fields[0].GetString ());
  delete result;

  rB.SetRand (16 * 8);

  ByteBuffer buf;
  buf << uint8 (AUTH_RECONNECT_CHALLENGE);
  buf << uint8 (0);
  buf.append (rB.AsByteArray (16), 16);
  buf << uint64 (0);
  buf << uint64 (0);

  return this->send_data ((char const*) buf.contents (), buf.size ());
}

ssize_t
AuthSocket::handle_reconnect_proof ()
{
<<<<<<< HEAD:src/realmd/AuthSocket.cpp
  DEBUG_LOG ("AuthSocket::handle_reconnect_proof");

  if (this->incoming_mb_->length () < sizeof (sAuthReconnectProofC))
    return sizeof (sAuthReconnectProofC) - this->incoming_mb_->length ();

  // Read the packet
  sAuthReconnectProofC& lp = *((sAuthReconnectProofC*)this->incoming_mb_->rd_ptr ());

  if (login_.empty () || !rB.GetNumBytes () || !K.GetNumBytes ())
    return -1;

  BigNumber t1;
  t1.SetBinary (lp.rA, 16);

  Sha1Hash sha;
  sha.Initialize ();
  sha.UpdateData (login_);
  sha.UpdateBigNumbers (&t1, &rB, &K, NULL);
  sha.Finalize ();

  if (memcmp (sha.GetDigest (), lp.rM1, SHA_DIGEST_LENGTH))
=======
    DEBUG_LOG("Entering _HandleXferResume");
    ///- Check packet length and patch existence
    if (ibuf.GetLength() < 9 || !pPatch)
>>>>>>> c6d97c6ec37fe4978cbd521620cc6e8cd926682a:src/realmd/AuthSocket.cpp
    {
      sLog.outError ("AuthSocket::handle_reconnect_proof user %s tried to login, but session invalid.", login_.c_str ());
      return -1;
    }

<<<<<<< HEAD:src/realmd/AuthSocket.cpp
  // Ok, client authed.
  ByteBuffer pkt;
  pkt << (uint8) AUTH_RECONNECT_PROOF;
  pkt << (uint8) 0x00;
  pkt << (uint16) 0x00; // 2 bytes zeros

  authed_ = true;
=======
    ///- Launch a PatcherRunnable thread starting at given patch file offset
    uint64 start;
    ibuf.Remove(1);
    ibuf.Read((char*)&start,sizeof(start));
    fseek(pPatch, start, 0);
>>>>>>> c6d97c6ec37fe4978cbd521620cc6e8cd926682a:src/realmd/AuthSocket.cpp

  return this->send_data ((char const*) pkt.contents (), pkt.size ());
}

class PatchSender : protected ACE_Task_Base
{
public:
  PatchSender (ACE_HANDLE peer_sock, ACE_HANDLE patch_fd, uint64 start_pos = 0);
  ~PatchSender ();

  int initiate ();

  virtual int svc (void);
  virtual int close (u_long flags = 0);
private:
  int set_options ();

  ACE_HANDLE peer_socket_;
  ACE_HANDLE patch_fd_;
  uint64 start_pos_;

  static ACE_Atomic_Op<ACE_SYNCH_MUTEX, long> thread_count;
  static const long max_thread_count;
};

static ACE_Atomic_Op<ACE_SYNCH_MUTEX, long> thread_count (0);
static const long max_thread_count = 32;

PatchSender::PatchSender (ACE_HANDLE peer_sock, ACE_HANDLE patch_fd, uint64 start_pos) :
peer_socket_ (peer_sock),
patch_fd_ (patch_fd),
start_pos_ (start_pos) { }

PatchSender::~PatchSender ()
{
  if (this->peer_socket_ != ACE_INVALID_HANDLE)
    ACE_OS::closesocket (this->peer_socket_);

  if (this->patch_fd_ != ACE_INVALID_HANDLE)
    ACE_OS::close (this->patch_fd_);
}

int
PatchSender::initiate ()
{
  if (this->peer_socket_ == ACE_INVALID_HANDLE ||
      this->patch_fd_ == ACE_INVALID_HANDLE)
    {
      return -1;
    }

<<<<<<< HEAD:src/realmd/AuthSocket.cpp
  if (this->activate (THR_NEW_LWP | THR_DETACHED | THR_INHERIT_SCHED) == -1)
    {
      this->peer_socket_ = this->patch_fd_ = ACE_INVALID_HANDLE;
      sLog.outError ("PatchSender::initiate Failed to spawn thread");
      return -1;
    }
=======
    ///- Launch a PatcherRunnable thread, starting at the beginning of the patch file
    ibuf.Remove(1);                                         // clear input buffer
    fseek(pPatch, 0, 0);
>>>>>>> c6d97c6ec37fe4978cbd521620cc6e8cd926682a:src/realmd/AuthSocket.cpp

  return 0;
}

int
PatchSender::close (u_long flags)
{
<<<<<<< HEAD:src/realmd/AuthSocket.cpp
  DEBUG_LOG ("PatchSender::close");

  delete this;
  return 0;
=======
    return (TCP_BUFSIZE_READ-GetOutputLength() < 2 * ChunkSize);
}

PatcherRunnable::PatcherRunnable(class AuthSocket * as)
{
    mySocket = as;
>>>>>>> c6d97c6ec37fe4978cbd521620cc6e8cd926682a:src/realmd/AuthSocket.cpp
}

int
PatchSender::set_options ()
{
  int nodelay = 0;
  ACE_SOCK_Stream option_setter (peer_socket_);

  if (-1 == option_setter.set_option (ACE_IPPROTO_TCP,
                                      TCP_NODELAY,
                                      &nodelay,
                                      sizeof (nodelay)))
    {
      sLog.outError ("PatchSender::set_options set_option TCP_NODELAY %s", ACE_OS::strerror (errno));
    }

#if defined(TCP_CORK)
  int cork = 1;
  if (-1 == option_setter.set_option (ACE_IPPROTO_TCP,
                                      TCP_CORK,
                                      &cork,
                                      sizeof (cork)))
    {
<<<<<<< HEAD:src/realmd/AuthSocket.cpp
      sLog.outError ("PatchSender::set_options set_option TCP_CORK %s", ACE_OS::strerror (errno));
=======
        ///- Wait until output buffer is reasonably empty
        while(mySocket->Ready() && mySocket->IsLag())
        {
        ACE_Based::Thread::Sleep(1);
        }
        ///- And send content of the patch file to the client
        xfdata.data_size = fread(&xfdata.data, 1, ChunkSize, mySocket->pPatch);
        mySocket->SendBuf((const char*)&xfdata, xfdata.data_size + (sizeof(XFER_DATA_STRUCT) - ChunkSize));
>>>>>>> c6d97c6ec37fe4978cbd521620cc6e8cd926682a:src/realmd/AuthSocket.cpp
    }
#endif //TCP_CORK

  (void) option_setter.disable (ACE_NONBLOCK);

  return 0;
}

int
PatchSender::svc (void)
{
  DEBUG_LOG ("PatchSender::svc");

  // Do 1 second sleep, similar to the one in game/WorldSocket.cpp
  // Seems client have problems with too fast sends.
  ACE_OS::sleep (1);

  ACE_OFF_T file_size = ACE_OS::filesize (patch_fd_);

  if (file_size == -1 || start_pos_ >= file_size)
    return -1;

  if (ACE_OS::lseek (patch_fd_, start_pos_, SEEK_SET) == -1)
    return -1;

  (void) set_options ();

#ifdef MSG_NOSIGNAL
  int flags = MSG_NOSIGNAL;
#else
  int flags = 0;
#endif // MSG_NOSIGNAL

  sAuthTransferChunkS data;
  data.opcode = XFER_DATA;

  ssize_t r;

  while ((r = ACE_OS::read (patch_fd_, &data.data, (size_t) XFER_CHUNK_SIZE)) > 0)
    {
<<<<<<< HEAD:src/realmd/AuthSocket.cpp
      data.data_size = (uint16) r;

      EndianConvert (data.data_size);

      if (ACE_OS::send (peer_socket_,
                        (const char*) & data,
                        ((size_t) r) + sizeof (data) - XFER_CHUNK_SIZE,
                        flags) == -1)
=======
        errno = 0;
        if ((dp = readdir(dirp)) != NULL)
        {
            int l = strlen(dp->d_name);
            if(l < 8)
                continue;
            if(!memcmp(&dp->d_name[l-4],".mpq",4))
                LoadPatchMD5(dp->d_name);
        }
        else
>>>>>>> c6d97c6ec37fe4978cbd521620cc6e8cd926682a:src/realmd/AuthSocket.cpp
        {
          DEBUG_LOG ("PatchSender::svc ACE_OS::send %s",
                     ACE_OS::strerror (errno));
          return -1;
        }
    }

  if (r == -1)
    {
      DEBUG_LOG ("PatchSender::svc ACE_OS::read %s",
                 ACE_OS::strerror (errno));
      return -1;
    }

  return 0;
}

ssize_t
AuthSocket::handle_clent_old_version (sAuthLogonChallengeC& cs)
{
<<<<<<< HEAD:src/realmd/AuthSocket.cpp
  DEBUG_LOG ("AuthSocket::handle_clent_old_version");

  if (this->patch_ != ACE_INVALID_HANDLE)
    return -1;

  // Check if we have the apropriate patch on the disk
  // No buffer overflow (fixed length of arguments)

  ByteBuffer pkt;
  char tmp[64];
  sprintf (tmp, "./patches/%d%c%c%c%c.mpq", cs.build, cs.country[3],
           cs.country[2], cs.country[1], cs.country[0]);

  // Open the file for readonly, using mostly win32 flags :) 
  // GENERIC_READ = O_RDONLY, in ACE
  {
    char filename[PATH_MAX];
    if (ACE_OS::realpath (tmp, filename) != NULL)
      {
        this->patch_ = ACE_OS::open (filename, GENERIC_READ | FILE_FLAG_SEQUENTIAL_SCAN);
      }
    else
      {
        DEBUG_LOG ("AuthSocket::handle_clent_old_version realpath failed");
      }
  }
=======
    WIN32_FIND_DATA fil;
    HANDLE hFil=FindFirstFile("./patches/*.mpq", &fil);
    if(hFil == INVALID_HANDLE_VALUE)
        return;                                             // no patches were found
>>>>>>> c6d97c6ec37fe4978cbd521620cc6e8cd926682a:src/realmd/AuthSocket.cpp

  if (this->patch_ == ACE_INVALID_HANDLE)
    {
      pkt << (uint8) AUTH_LOGON_CHALLENGE;
      pkt << (uint8) 0x00;
      pkt << (uint8) REALM_AUTH_WRONG_BUILD_NUMBER;

      DEBUG_LOG ("AuthSocket::handle_clent_old_version %u is not a valid client version!", cs.build);
      DEBUG_LOG ("AuthSocket::handle_clent_old_version Patch %s not found", tmp);

      this->closing_ = true;
      return this->send_data ((char const*) pkt.contents (), pkt.size ());
    }
<<<<<<< HEAD:src/realmd/AuthSocket.cpp

  ACE_OFF_T size = ACE_OS::filesize (this->patch_);
  if (size == -1)
    return -1;

  uint8 md5[MD5_DIGEST_LENGTH];

  // Get the MD5 hash of the patch file (get it from preloaded Patcher cache or calculate it)
  if (PatchCache::instance ()->GetHash (tmp, md5))
    {
      DEBUG_LOG ("AuthSocket::handle_clent_old_version Found precached patch info for patch %s", tmp);
    }
  else
    {
      //calculate patch md5
      sLog.outBasic ("AuthSocket::handle_clent_old_version Patch info for %s was not cached.", tmp);
      PatchCache::instance ()->LoadPatchMD5 (tmp);
      PatchCache::instance ()->GetHash (tmp, md5);
    }

  pkt << (uint8) AUTH_LOGON_PROOF;
  pkt << (uint8) REALM_AUTH_UPDATE_CLIENT;

  pkt << (uint8) XFER_INITIATE;

  // magic patch name
  const char* patchName = "Patch";

  pkt << (uint8) strlen (patchName);
  pkt.append (patchName, strlen (patchName));

  // size of the file
  pkt << (uint64) size;

  pkt.append (md5, MD5_DIGEST_LENGTH);

  return this->send_data ((char const*) pkt.contents (), pkt.size ());
=======
    while(FindNextFile(hFil, &fil));
>>>>>>> c6d97c6ec37fe4978cbd521620cc6e8cd926682a:src/realmd/AuthSocket.cpp
}

ssize_t
AuthSocket::handle_transfer_accept ()
{
<<<<<<< HEAD:src/realmd/AuthSocket.cpp
  DEBUG_LOG ("AuthSocket::handle_transfer_accept");

  // Check packet length and patch existence
  if (this->patch_ == ACE_INVALID_HANDLE)
    {
      sLog.outError ("AuthSocket::handle_transfer_accept Error!");
      return -1;
=======
    ///- Try to open the patch file
    std::string path = "./patches/";
    path += szFileName;
    FILE *pPatch = fopen(path.c_str(), "rb");
    sLog.outDebug("Loading patch info from %s\n", path.c_str());
    if(!pPatch)
    {
        sLog.outError("Error loading patch %s\n", path.c_str());
        return;
>>>>>>> c6d97c6ec37fe4978cbd521620cc6e8cd926682a:src/realmd/AuthSocket.cpp
    }

  PatchSender* sender = new PatchSender (this->handle_, this->patch_);

  if (sender->initiate () == -1)
    {
      delete sender;
      return -1;
    }

<<<<<<< HEAD:src/realmd/AuthSocket.cpp
  this->handle_ = ACE_INVALID_HANDLE;
  this->patch_ = ACE_INVALID_HANDLE;

  // Means destroy this object.
  // PatchSender will manage the connection from now on.
  return -1;
}

ssize_t
AuthSocket::handle_transfer_resume ()
{
  DEBUG_LOG ("AuthSocket::handle_transfer_resume ");

  // Check packet length and patch existence
  if (this->patch_ == ACE_INVALID_HANDLE)
    {
      sLog.outError ("AuthSocket::handle_transfer_resume Error!");
      return -1;
=======
    ///- Store the result in the internal patch hash map
    _patches[path] = new PATCH_INFO;
    MD5_Final((uint8 *)&_patches[path]->md5, &ctx);
}

/// Get cached MD5 hash for a given patch file
bool Patcher::GetHash(char * pat, uint8 mymd5[16])
{
    for( Patches::iterator i = _patches.begin(); i != _patches.end(); ++i )
        if(!stricmp(pat, i->first.c_str()))
    {
        memcpy(mymd5, i->second->md5, 16);
        return true;
>>>>>>> c6d97c6ec37fe4978cbd521620cc6e8cd926682a:src/realmd/AuthSocket.cpp
    }

  // read size.
  if (this->incoming_mb_->length () < 8)
    return 8 - this->incoming_mb_->length ();

  // will be checked later.
  uint64& size = *((uint64*)this->incoming_mb_->rd_ptr ());

<<<<<<< HEAD:src/realmd/AuthSocket.cpp
  EndianConvert (size);

  PatchSender* sender = new PatchSender (this->handle_, this->patch_, size);

  if (sender->initiate () == -1)
    {
      delete sender;
      return -1;
    }

  this->handle_ = ACE_INVALID_HANDLE;
  this->patch_ = ACE_INVALID_HANDLE;

  // Means destroy this object.
  return -1;
=======
/// Empty and delete the patch map on termination
Patcher::~Patcher()
{
    for(Patches::iterator i = _patches.begin(); i != _patches.end(); ++i )
        delete i->second;
>>>>>>> c6d97c6ec37fe4978cbd521620cc6e8cd926682a:src/realmd/AuthSocket.cpp
}
