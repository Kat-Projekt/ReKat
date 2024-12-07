#include <engine.hpp>

class Test : public Behavoiur {
public:
    void Start ( ) { DEBUG ( 3, "Loaded test component" ); }
    void Update ( ) { DEBUG ( 3, "Updating test component" ); }
};
