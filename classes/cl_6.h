#ifndef __CL_6__H
#define __CL_6__H

#include "cl_base.h"

class cl_6 : public cl_base {
public:
    cl_6(cl_base* p_parent, string s_name = "Base_object"); // конструктор
    void signal_f(string& s_msg); // метод сигнала
    void handler_f(string s_msg); // метод обработчика
};

#endif
