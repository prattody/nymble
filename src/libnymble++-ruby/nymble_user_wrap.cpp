#include "nymble_user_wrap.h"

VALUE rb_user_new(VALUE rb_self)
{
  return Data_Wrap_Struct(rb_self, NULL, rb_user_delete, new User());
}

VALUE rb_user_init(VALUE rb_self, VALUE rb_pseudonym, VALUE rb_verify_key_n)
{
  Check_Type(rb_self, T_DATA);
  Check_Class(rb_self, rb_cUser);
  Check_Type(rb_pseudonym, T_DATA);
  Check_Class(rb_pseudonym, rb_cPseudonym);
  Check_Type(rb_verify_key_n, T_STRING);
  
  User* user = (User*) DATA_PTR(rb_self);
  Pseudonym* pseudonym = (Pseudonym*) DATA_PTR(rb_pseudonym);
  u_char* verify_key_n = (u_char*) RSTRING_PTR(rb_verify_key_n);
  u_int verify_key_n_len = RSTRING_LEN(rb_verify_key_n);
  
  user->setPseudonym(pseudonym);
  user->setVerifyKeyN(&verify_key_n, verify_key_n_len);
  
  return rb_self;
}

VALUE rb_user_pseudonym(VALUE rb_self)
{
  Check_Type(rb_self, T_DATA);
  Check_Class(rb_self, rb_cUser);
  
  User* user = (User*) DATA_PTR(rb_self);
  
  return Data_Wrap_Struct(rb_cPseudonym, NULL, NULL, user->getPseudonym());
}

VALUE rb_user_add_server(VALUE rb_self, VALUE rb_server_id, VALUE rb_blacklist, VALUE rb_credential)
{
  Check_Type(rb_self, T_DATA);
  Check_Class(rb_self, rb_cUser);
  Check_Type(rb_server_id, T_STRING);
  Check_Size(rb_server_id, DIGEST_SIZE);
  Check_Type(rb_blacklist, T_DATA);
  Check_Class(rb_blacklist, rb_cBlacklist);
  Check_Type(rb_credential, T_DATA);
  Check_Class(rb_credential, rb_cCredential);
  
  User* user = (User*) DATA_PTR(rb_self);
  u_char* server_id = (u_char*) RSTRING_PTR(rb_server_id);
  Blacklist* blacklist = (Blacklist*) DATA_PTR(rb_blacklist);
  Credential* credential = (Credential*) DATA_PTR(rb_credential);
  
  if (user->addServer(server_id, blacklist, credential)) {
    return Qtrue;
  }
  
  return Qfalse;
}

VALUE rb_user_ticket(VALUE rb_self, VALUE rb_server_id)
{
  Check_Type(rb_self, T_DATA);
  Check_Class(rb_self, rb_cUser);
  Check_Type(rb_server_id, T_STRING);
  Check_Size(rb_server_id, DIGEST_SIZE);
  
  User* user = (User*) DATA_PTR(rb_self);
  u_char* server_id = (u_char*) RSTRING_PTR(rb_server_id);
  Ticket* ticket = user->getTicket(server_id);
  
  if (ticket ==  NULL) {
    return Qnil;
  }
  
  return Data_Wrap_Struct(rb_cTicket, NULL, NULL, ticket);
}

void rb_user_delete(User* user)
{
  delete user;
}