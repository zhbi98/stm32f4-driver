
#ifndef __PASSWD_H__
#define __PASSWD_H__

struct password_auth_t {
	unsigned char password_data[16];
	unsigned char password_buf[16];

	unsigned char password_length;
	unsigned char lock_status;
};

extern struct password_auth_t param_setting_auth;
extern struct password_auth_t test_function_auth;

extern unsigned char pwd_auth(struct password_auth_t * auth);
extern unsigned char get_lock_status(struct password_auth_t * auth);
extern void read_password(struct password_auth_t * auth, unsigned char key);

#endif
