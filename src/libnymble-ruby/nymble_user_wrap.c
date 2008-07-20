#include "nymble_user_wrap.h"

VALUE rb_user_initialize(VALUE rb_self, VALUE rb_pseudonym, VALUE rb_mac_np, VALUE rb_verify_key_n)
{
  Check_Type(rb_pseudonym, T_STRING);
  Check_Type(rb_mac_np, T_STRING);
  Check_Type(rb_verify_key_n, T_STRING);
  Check_Size(rb_pseudonym, DIGEST_SIZE);
  Check_Size(rb_mac_np, DIGEST_SIZE);
  
  pseudonym_t pseudonym;
  memcpy(pseudonym.pseudonym, RSTRING_PTR(rb_pseudonym), DIGEST_SIZE);
  memcpy(pseudonym.mac_np, RSTRING_PTR(rb_mac_np), DIGEST_SIZE);

  const u_char* verify_key_n = (const u_char *)RSTRING_PTR(rb_verify_key_n);

  RSA* rsa = d2i_RSAPublicKey(NULL, &verify_key_n, RSTRING_LEN(rb_verify_key_n));
  user_t* user = user_initialize(&pseudonym, rsa);

  return Data_Wrap_Struct(rb_self, NULL, user_free, user);
}

VALUE rb_user_pseudonym(VALUE rb_self, VALUE rb_user_state)
{
  Check_Type(rb_user_state, T_DATA);
  
  user_t* user = (user_t*)DATA_PTR(rb_user_state);
  
  pseudonym_t* pseudonym = user_pseudonym(user);

  return rb_str_new((char*)pseudonym->pseudonym, DIGEST_SIZE);
}

VALUE rb_user_pseudonym_mac(VALUE rb_self, VALUE rb_user_state)
{
  Check_Type(rb_user_state, T_DATA);
  
  user_t* user = (user_t*)DATA_PTR(rb_user_state);
  
  pseudonym_t* pseudonym = user_pseudonym(user);

  return rb_str_new((char*)pseudonym->mac_np, DIGEST_SIZE);
}

VALUE rb_user_entry_initialize(VALUE rb_self, VALUE rb_user_state, VALUE rb_server_id, VALUE rb_credential)
{
  Check_Type(rb_user_state, T_DATA);
  Check_Type(rb_server_id, T_STRING);
  Check_Type(rb_credential, T_DATA);
  Check_Size(rb_server_id, DIGEST_SIZE);
  
  user_t* user = (user_t*)DATA_PTR(rb_user_state);
  u_char* server_id = (u_char*)RSTRING_PTR(rb_server_id);
  credential_t* credential = (credential_t*)DATA_PTR(rb_credential);
  u_int L = NUM2UINT(rb_gv_get("L"));
  
  user_entry_initialize(user, server_id, credential, L);

  return Qtrue;
}

VALUE rb_user_entry_exists(VALUE rb_self, VALUE rb_user_state, VALUE rb_server_id)
{
  Check_Type(rb_user_state, T_DATA);
  Check_Type(rb_server_id, T_STRING);
  Check_Size(rb_server_id, DIGEST_SIZE);

  user_t* user = (user_t*)DATA_PTR(rb_user_state);
  u_char* server_id = (u_char*)RSTRING_PTR(rb_server_id);

  if (user_entry_exists(user, server_id)) {
    return Qtrue;
  } else {
    return Qfalse;
  }
}

VALUE rb_user_blacklist_update(VALUE rb_self, VALUE rb_user_state, VALUE rb_server_id, VALUE rb_blacklist, VALUE rb_link_window, VALUE rb_time_period)
{
  Check_Type(rb_user_state, T_DATA);
  Check_Type(rb_server_id, T_STRING);
  Check_Type(rb_blacklist, T_DATA);
  Check_Type(rb_link_window, T_FIXNUM);
  Check_Type(rb_time_period, T_FIXNUM);
  Check_Size(rb_server_id, DIGEST_SIZE);

  user_t* user = (user_t*)DATA_PTR(rb_user_state);
  u_char* server_id = (u_char*)RSTRING_PTR(rb_server_id);
  blacklist_t* blacklist = (blacklist_t*)DATA_PTR(rb_blacklist);
  u_int link_window = NUM2UINT(rb_link_window);
  u_int time_period = NUM2UINT(rb_time_period);

  if (user_blacklist_update(user, server_id, blacklist, link_window, time_period)) {
    return Qtrue;
  } else {
    return Qfalse;
  }
}

VALUE rb_user_blacklist_check(VALUE rb_self, VALUE rb_user_state, VALUE rb_server_id)
{
  Check_Type(rb_user_state, T_DATA);
  Check_Type(rb_server_id, T_STRING);
  Check_Size(rb_server_id, DIGEST_SIZE);

  user_t* user = (user_t*)DATA_PTR(rb_user_state);
  u_char* server_id = (u_char*)RSTRING_PTR(rb_server_id);

  if (user_blacklist_check(user, server_id)) {
    return Qtrue;
  } else {
    return Qfalse;
  }
}

VALUE rb_user_credential_get(VALUE rb_self, VALUE rb_user_state, VALUE rb_server_id, VALUE rb_time_period)
{
  Check_Type(rb_user_state, T_DATA);
  Check_Type(rb_server_id, T_STRING);
  Check_Type(rb_time_period, T_FIXNUM);
  Check_Size(rb_server_id, DIGEST_SIZE);

  user_t* user = (user_t*)DATA_PTR(rb_user_state);
  u_char* server_id = (u_char*)RSTRING_PTR(rb_server_id);
  u_int time_period = NUM2UINT(rb_time_period);

  ticket_t* ticket = user_credential_get(user, server_id, time_period);
  
  if (ticket) {
    return Data_Wrap_Struct(rb_self, NULL, NULL, ticket);
  } else {
    return Qnil;
  }
}
