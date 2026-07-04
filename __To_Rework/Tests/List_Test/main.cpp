#include <utility/list.h>

int main ( ) {
    int ints [ 20 ] = { 1,2,3,4,5,6,7,8,9,10 };
    List < int > L;

    for ( auto P : L ) {
        std::cout << P;
    } std::cout << '\n';

    for ( int i = 0; i < 10; i++ ) {
        std::cout << L << '\n';
        L.append ( ints[i] );
    }

    for ( auto P : L ) {
        std::cout << P;
        if ( P == 5 ) { L.remove ( 5 );}
    } std::cout << '\n';

    std::cout << " COMPLETE \n" << L << "\n\n";

    for ( int i = 0; i < 10; i++ ) {
        L.remove ( ints[i] );
        std::cout << L << '\n';
    }
    std::cout << " REM \n" << L << "\n\n";

    for ( int i = 0; i < 10; i++ ) {
        std::cout << L << '\n';
        L.append ( ints[i] );
    }
    std::cout << " COMPLETE \n" << L << "\n\n";

    for ( int i = 0; i < 10; i++ ) {
        L.remove ( ints[i] );
        std::cout << L << '\n';
    }
    std::cout << " REM \n" << L << "\n\n";

    for ( int i = 0; i < 10; i++ ) {
        std::cout << L << '\n';
        L.append ( ints[i] );
    }
    std::cout << " COMPLETE \n" << L << "\n\n";

    for ( int i = 0; i < 10; i++ ) {
        L.remove ( ints[i] );
        std::cout << L << '\n';
    }
    std::cout << " REM \n" << L << "\n\n";

    for ( int i = 0; i < 10; i++ ) {
        std::cout << L << '\n';
        L.append ( ints[i] );
    }
    std::cout << " COMPLETE \n" << L << "\n\n";

    for ( auto P : L ) {
        std::cout << P;
    } std::cout << '\n';

    for ( int i = 0; i < 10; i++ ) {
        L.remove ( ints[i] );
        std::cout << L << '\n';
    }
    std::cout << " REM \n" << L << "\n\n";

    for ( auto P : L ) {
        std::cout << P;
    } std::cout << '\n';

    for ( int i = 0; i < 10; i++ ) {
        std::cout << L << '\n';
        L.append ( ints[i] );
    }
    std::cout << " COMPLETE \n" << L << "\n\n";

    for ( auto P : L ) {
        std::cout << P;
    } std::cout << '\n';
}