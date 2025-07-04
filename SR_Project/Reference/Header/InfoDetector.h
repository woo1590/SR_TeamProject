#pragma once

#include "ObjectComponent.h"
#include "IObserver.h"

BEGIN(Engine)

template<typename T>
class InfoDetector : public ObjectComponent, public IObserver<UIEvent<T>>
{
private:
    explicit InfoDetector(Object* owner)
        :ObjectComponent(owner) {
    }

public:
    static InfoDetector* Create(Object* owner) { return new InfoDetector(owner); }

    template<typename FieldType>
    void Watch(FieldType T::* fieldPtr, function<void(FieldType)> onChanged)
    {
        auto getter = [fieldPtr](const T& obj)->FieldType { return obj.*fieldPtr; };

        fieldWatchers.emplace_back([=](const T& prev, const T& curr) {
            FieldType before = getter(prev);
            FieldType after = getter(curr);
            if (before != after)
                onChanged(after);
            });
    }

    void OnNotify(const UIEvent<T>& event) override
    {
        for (auto& watcher : fieldWatchers)
            watcher(prevInfo, event.info);

        prevInfo = event.info;
    }

private:
    T prevInfo{};
    vector<function<void(const T&, const T&)>> fieldWatchers;
};

END