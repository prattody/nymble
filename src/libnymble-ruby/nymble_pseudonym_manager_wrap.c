#include "nymble_pseudonym_manager_wrap.h"

VALUE rb_pm_initialize(VALUE rb_self, VALUE rb_hmac_key_np)
{
  Check_Type(rb_hmac_key_np, T_STRING);
  Check_Size(rb_hmac_key_np, DIGEST_SIZE);
  
  u_char* hmac_key_np = (u_char*)RSTRING_PTR(rb_hmac_key_np);

  pm_t* pm = pm_initialize(hmac_key_np);

  return Data_Wrap_Struct(rb_self, NULL, pm_free, pm);
}

VALUE rb_pm_pseudonym_create(VALUE rb_self, VALUE rb_pm_state, VALUE rb_user_id, VALUE rb_link_window)
{
  Check_Type(rb_pm_state, T_DATA);
  Check_Type(rb_user_id, T_STRING);
  Check_Type(rb_link_window, T_FIXNUM);
  Check_Size(rb_user_id, DIGEST_SIZE);
    
  pm_t* pm = (pm_t*)DATA_PTR(rb_pm_state);
  u_char* user_id = (u_char*)RSTRING_PTR(rb_user_id);
  u_int link_window = NUM2UINT(rb_link_window);
  
  pseudonym_t* pseudonym = pm_pseudonym_create(pm, user_id, link_window);
  
  VALUE ret = rb_ary_new();
  
  rb_ary_push(ret, rb_str_new((char *)pseudonym->pseudonym, DIGEST_SIZE));
  rb_ary_push(ret, rb_str_new((char *)pseudonym->mac_np, DIGEST_SIZE));
  
  free(pseudonym);
  
  return ret;
}
