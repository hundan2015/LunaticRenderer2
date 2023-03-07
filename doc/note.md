# 超级坑

他妈的能不能好好解决几个link问题啊大傻逼（单指作者自己）\
原来我才是傻逼！

```mermaid
classDiagram
    shit ..> Entity
    class Entity {
        - string name_
        + weak_ptr<Entity> parent
        + vector<shared_ptr<Entity>> child
        + weak_ptr<EntityManager> mEntityManager
    }

    class shit {
        + string name_
        + Entity shit
    }
```
