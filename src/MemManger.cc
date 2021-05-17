#include "MemManger.h"
#include "JsObject.h"
#include "stdafx.h"
namespace agumi
{

    MemManger::MemManger() : gc_root(JsObject()) {}
    MemManger::~MemManger() {}

    Vector<const JsObjectMap *> MemAllocCollect::obj_quene = {};
    Vector<const JsArrayVec *> MemAllocCollect::vec_quene = {};
    MemManger *MemManger::mem = nullptr;
    MemManger &MemManger::Get()
    {
        if (mem == nullptr)
        {
            mem = new MemManger();
        }
        return *mem;
    }

    void MemManger::ReachObjectNode(JsObject start)
    {
        std::vector<JsObject> obj_set = {start};
        std::vector<JsArray> arr_set;
        while (obj_set.size() + arr_set.size())
        {
            if (obj_set.size())
            {
                auto obj = obj_set.back();
                obj_set.pop_back();
                for (const auto &i : obj.Src())
                {
                    auto next = i.second;
                    if (next.Type() == JsType::object && can_reach_obj.find(next.Object().Ptr()) == can_reach_obj.end())
                    {
                        auto object_node = next.Object();
                        can_reach_obj.insert(object_node.Ptr());
                        obj_set.push_back(object_node);
                    }
                    else if (next.Type() == JsType::array && can_reach_arr.find(next.Array().Ptr()) == can_reach_arr.end())
                    {
                        auto arr_node = next.Array();
                        can_reach_arr.insert(arr_node.Ptr());
                        arr_set.push_back(arr_node);
                    }
                }
            }
            else if (arr_set.size())
            {
                auto arr = arr_set.back();
                arr_set.pop_back();
                for (const auto &i : arr.Src())
                {
                    auto next = i;
                    if (next.Type() == JsType::array && can_reach_arr.find(next.Array().Ptr()) == can_reach_arr.end())
                    {
                        auto arr_node = next.Array();
                        can_reach_arr.insert(arr_node.Ptr());
                        arr_set.push_back(arr_node);
                    }
                    else if (next.Type() == JsType::object && can_reach_obj.find(next.Object().Ptr()) == can_reach_obj.end())
                    {
                        auto object_node = next.Object();
                        can_reach_obj.insert(object_node.Ptr());
                        obj_set.push_back(object_node);
                    }
                }
            }
        }
    }

    void MemManger::GC()
    {
        can_reach_obj.insert(gc_root.Object().Ptr());
        ReachObjectNode(gc_root.Object());
        for (auto i : MemAllocCollect::obj_quene)
        {
            if (can_reach_obj.find(i) == can_reach_obj.end())
            {
                delete i;
            }
        }
        for (auto i : MemAllocCollect::vec_quene)
        {
            if (can_reach_arr.find(i) == can_reach_arr.end())
            {
                delete i;
            }
        }
        MemAllocCollect::vec_quene.resize(can_reach_arr.size());
        MemAllocCollect::obj_quene.resize(can_reach_obj.size());
        copy(can_reach_arr.begin(), can_reach_arr.end(), MemAllocCollect::vec_quene.begin());
        copy(can_reach_obj.begin(), can_reach_obj.end(), MemAllocCollect::obj_quene.begin());
        can_reach_obj.clear();
        can_reach_arr.clear();
#ifdef JS_RUNTIME_TEST
        cout << "GC完成  对象数量：" << MemAllocCollect::obj_quene.size() << "   "
             << "数组数量：" << MemAllocCollect::vec_quene.size() << endl;
#endif
    }
}