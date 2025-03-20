#include "cl_2.h"

cl_2::cl_2(cl_base* p_parent, string s_name) : cl_base(p_parent, s_name) {
    i_class_number = 2;
}

void cl_2::signal_f(string& msg) {
    cout << "Signal from " << get_absolute_path() << endl;
    msg = msg + " (class: " + to_string(i_class_number) + ")";
}

void cl_2::handler_f(string s_msg) {
    cout << "Signal to " << get_absolute_path() << " Text: " << s_msg << endl;
}
