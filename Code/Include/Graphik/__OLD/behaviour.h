#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H
template < typename T >
class _behaviour {
public:
    T* obj;
    virtual void Start ( ) { }
    virtual void Update ( ) { }

    virtual void Collision ( T* obj ) { }
    virtual void CollisionTrigger ( T* obj ) { }
};

#endif