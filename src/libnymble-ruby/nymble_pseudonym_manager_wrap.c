#include "nymble_pseudonym_manager_wrap.h"

VALUE rb_pm_initialize(VALUE rb_self, VALUE rb_hmac_key_np) {
  if (TYPE(rb_hmac_key_np) != T_STRING) {
    return Qnil;
  }

  u_char *hmac_key_np = (u_char *)RSTRING(rb_hmac_key_np)->ptr;

  pm_t *pm = pm_initialize(hmac_key_np);

  return Data_Wrap_Struct(rb_self, NULL, pm_free, pm);
}

VALUE rb_pm_pseudonym_create(VALUE rb_self, VALUE rb_pm_state, VALUE rb_user_id, VALUE rb_link_window) {
  if ((TYPE(rb_pm_state) != T_DATA) ||
      (TYPE(rb_user_id) != T_STRING) ||
      (TYPE(rb_link_window) != T_FIXNUM)) {
    return Qnil;
  }
    
  pm_t    *pm; Data_Get_Struct(rb_pm_state, pm_t, pm);
  u_char  *user_id = (u_char *)RSTRING(rb_user_id)->ptr;
  u_int   link_window = NUM2UINT(rb_link_window);
  
  pseudonym_t *pseudonym = pm_pseudonym_create(pm, user_id, link_window);
  
  VALUE ret = rb_ary_new();
  
  rb_ary_push(ret, rb_str_new((char *)pseudonym, sizeof(pseudonym_t) / 2));
  rb_ary_push(ret, rb_str_new((char *)pseudonym + sizeof(pseudonym_t) / 2, sizeof(pseudonym_t) / 2));
  
  free(pseudonym);
  
  return ret;
}