# Orkestrator

This is the Program manager had has the job to make sure that Systems are isolated between Operas.

The Orkestrator makes sure that the Global Statics are sincked with the current opera. This does not procude that operas can share systems.

During a cicle ( Updata, Start, ... ) there can only be one active opera.

Opera switching is possible only inside main.

## Counter Example:
When debugging can there bee a Debug Opera that when an error occurs is woken up: the Error Opera is frozzen ( not unloaded ) and provvided as a serializzation interface to the Debug Opera:

### Mockup

```
class Orkestrato
{
    private:
        static vector < Opera * > _operas;
        static Opera * active_opera;

    friend int main ( ); // only the main function can call orkestrator
};
```
