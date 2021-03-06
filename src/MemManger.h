#pragma once
#include "stdafx.h"
#include "Value.h"
#include "Object.h"
namespace agumi
{
    

    class MemAllocCollect
    {
    public:
        static Vector<const ObjectMap *> obj_quene;
        static Vector<const ArrayVec *> vec_quene;
        static size_t size() ;
    };

    class MemManger
    {
    private:
        static MemManger *mem;

    public:
        MemManger();
        ~MemManger();
        Value gc_root;
        bool first ;
        size_t last_gc = 10000;
        size_t gc_step = 10000;
        bool gc_log = true;
        bool enable_gc = false;
        std::set<const ObjectMap *> can_reach_obj;
        std::set<const ArrayVec *> can_reach_arr;
        void ReachObjectNode(Object start);
        void GC();
        Object& Closure ();
        static MemManger &Get();
    };
}