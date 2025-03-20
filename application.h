#ifndef __APPLICATION__H
#define __APPLICATION__H

#include "cl_base.h"
#include "cl_1.h"
#include "cl_2.h"
#include "cl_3.h"
#include "cl_4.h"
#include "cl_5.h"
#include "cl_6.h"

class application : public cl_base {
private:
    vector<TYPE_SIGNAL> p_signals; // контейнер сигналов для каждого класса
    vector<TYPE_HANDLER> p_handlers; // контейнер обработчиков для каждого класса
    cl_base* p_root_obj; // корневой объект дерева иерархии

public:
    application(cl_base* parent); // конструктор
    void build_tree_objects();
    int exec_app();
};

#endif
