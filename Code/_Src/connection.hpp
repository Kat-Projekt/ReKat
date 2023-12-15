#include "command.hpp"

void Connections ( ) { 
    while ( !Main_shutdown ) { 
        online::New_Connection();
        output << "connected to: " + online::Connected()[online::Connected().size()-1] + '\n';
        Send_positon();
    }
    std::cout << "stop getting connections\n";
}

void Recive ( std::string node, int * is_closed ) {
    int status;
    size_t size = 0;
    char *_buf; 
    while ( !Main_shutdown ) {
        _buf = online::Recv ( node, &status, &size, -1 );
        if ( status == online::DISCONNECTED ) { 
            output << "connection with " + node + " closed\n";
            std::cout << "connection with" + node + " closed\n";
            online::Close_sock (node);
            break; 
        }
        switch ( _buf[size-1] ) {
        case 'v':
            // modify position of node
            //Objects[node].Move(remove_ending<glm::vec2>(_buf));        
        break;
        case 't': output << "fr " + node + ": " + std::string(_buf).erase(size-1,1) + '\n'; break;
        }
        
    }
    std::cout << "closed recive with " + node + '\n';
    // revove obj
    //Objects.erase(node);
    *is_closed = 1;
    return;
}

void Check_connections ( ) {
    // store threads and their state
    std::vector < std::pair < std::pair < int *, std::string > , std::thread > > threads;
    while ( !Main_shutdown ) {
        // if a new connection starts add its thread and status
        std::vector < std::string > missing;
        for ( auto s : online::Connected() ) {
            bool found = false;
            for (size_t i = 0; i < threads.size(); i++) { 
                if ( ( s == threads[i].first.second ) && ( ( *threads[i].first.first ) == 0 ) ) 
                    { found = true; } }
            found == true ? 0 : missing.push_back (s);
        }

        int * end;
        for ( auto m : missing ) {
            // start missing threads 
            std::cout << "adding " + m + '\n'; 
            end = (int*) calloc (1,sizeof(int));
            threads.push_back ( std::make_pair ( std::make_pair ( end, m ), std::thread ( Recive, m, end ) ) );
            //Objects.insert ( {m, Object ( m, Manager::Sprite_Get( "sprite" ), {100*(threads.size()+1),100}, {100,100} )} );
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // if notthing to close
    if ( threads.size() == 0 ) { return; }

    // close nodes
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::cout << "closing local nodes\n";
    for ( size_t i = 0; i < threads.size(); i++ ) { if ( threads[i].first.first ) { online::Close_sock ( threads[i].first.second ); } }

    // terminate nodes
    for ( size_t i = 0; i < threads.size ( ); i++ ) {
        if ( threads[i].second.joinable ( ) ) { 
            std::cout << "thread: " + threads[i].first.second + '\n';
            threads[i].second.detach ( );
            std::cout << "joined\n";
        }
    }

    std::cout << "local nodes closed\n";
}