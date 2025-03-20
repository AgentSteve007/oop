#include "cl_3.h"

cl_3::cl_3(cl_base* p_parent, string s_name) : cl_base(p_parent, s_name) {
    i_class_number = 3;
}

void cl_3::signal_f(string& msg) {
    cout << "Signal from " << get_absolute_path() << endl;
    msg = msg + " (class: " + to_string(i_class_number) + ")";
}

void cl_3::handler_f(string s_msg) {
    cout << "Signal to " << get_absolute_path() << " Text: " << s_msg << endl;
}
