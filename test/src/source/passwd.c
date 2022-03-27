
#include "passwd.h"

struct password_auth_t param_setting_auth = {
    .password_data   = {F1_KEY_EVT, F2_KEY_EVT, F2_KEY_EVT, F1_KEY_EVT, F1_KEY_EVT, F1_KEY_EVT, F2_KEY_EVT, F2_KEY_EVT},
    .password_buf    = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
    .password_length = 8,
    .lock_status     = false,
};

struct password_auth_t test_function_auth = {
    .password_data   = {F1_KEY_EVT, F2_KEY_EVT, F2_KEY_EVT, F1_KEY_EVT},
    .password_buf    = {0xff, 0xff, 0xff, 0xff},
    .password_length = 4,
    .lock_status     = false,
};

unsigned char pwd_auth(struct password_auth_t * auth)
{
    for (unsigned char i = 0; i < auth->password_length; i++)
        if (auth->password_buf[i] != auth->password_data[i])
            return false;
    return true;
}

unsigned char get_lock_status(struct password_auth_t * auth)
{
    return auth->lock_status;
}

void read_password(struct password_auth_t * auth, unsigned char key)
{
    if (auth->lock_status == false) {
        for (unsigned char bits = 0; bits < auth->password_length - 1; bits++)
            auth->password_buf[bits] = auth->password_buf[bits + 1];
        auth->password_buf[auth->password_length - 1] = key;
    }

    if (pwd_auth(auth) == false) {
        auth->lock_status = false;
        return;
    }

    // password auth success
    memset(auth->password_buf, 0xff, auth->password_length);
    auth->lock_status = true;
}
