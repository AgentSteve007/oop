#ifndef __CL_BASE__H
#define __CL_BASE__H

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using namespace std;

class cl_base;

#define SIGNAL_D(signal_f) (TYPE_SIGNAL)(&signal_f)
#define HANDLER_D(handler_f) (TYPE_HANDLER)(&handler_f)

typedef void (cl_base::*TYPE_SIGNAL)(string& s_msg);
typedef void (cl_base::*TYPE_HANDLER)(string s_msg);

struct o_sh { // структура задания одной связи
    TYPE_SIGNAL p_signal; // указатель на метод сигнала
    cl_base* p_target; // указатель на целевой объект
    TYPE_HANDLER p_handler; // указатель на метод обработчика
};

class cl_base {
private:
    int state = 0;
    cl_base* parent;
    vector<cl_base*> children;
    string name;
    vector<o_sh*> p_connects; // контейнер связей между объектами

protected:
    int i_class_number; // номер класса

public:
    cl_base(cl_base* parent, string name = "Object_root");
    ~cl_base();

    bool setName(string name);
    string get_name() const;
    cl_base* get_parent() const;
    cl_base* get_child_by_name(string name);

    // Update
    cl_base* findObjOnTree(string name);
    cl_base* findObjOnBranch(string name);
    void Print(int level = 0);
    void PrintWithState(int level = 0);
    void setState(int state);

    // 3 Часть
    bool set_parent(cl_base* new_parent);
    void remove_child_by_name(string child_name);
    cl_base* get_object_by_path(string path);
    string get_absolute_path();
    int get_class_number();

    // 4 Часть
    vector<cl_base*> get_children();
    void set_connection(TYPE_SIGNAL p_signal, cl_base* p_target, TYPE_HANDLER p_handler);
    void delete_connection(TYPE_SIGNAL p_signal, cl_base* p_target, TYPE_HANDLER p_handler);
    void emit_signal(TYPE_SIGNAL p_signal, string s_message);
};

#endif
